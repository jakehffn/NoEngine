#include "render_system.h"

RenderSystem::RenderSystem(entt::registry& registry) : System(registry), 
    spacialObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Texture>()) },
    textureObserver{ entt::observer(registry, entt::collector.group<Texture>()) } {
    
        // this->initTextMap();
}

void RenderSystem::update() {

    std::vector<double> times;
    double start =  SDL_GetPerformanceCounter();
    this->cullEntities();
    double total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    times.push_back(total);

    start =  SDL_GetPerformanceCounter();
    this->sortEntities();
    total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    times.push_back(total);

    start =  SDL_GetPerformanceCounter();
    this->updateModels();
    total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    times.push_back(total);

    start =  SDL_GetPerformanceCounter();
    this->bufferEntityData();
    total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    times.push_back(total);

    start =  SDL_GetPerformanceCounter();
    this->render();
    total =  (SDL_GetPerformanceCounter() - start)/SDL_GetPerformanceFrequency()*1000.0;
    times.push_back(total);

    int i = 0;
}

void RenderSystem::cullEntities() {

    using namespace entt::literals;
    auto& camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);
    auto& componentGrid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

    glm::vec2 camDim = camera.getCameraDim();
    glm::vec2 camPos = camera.getPosition();

    int x = camPos.x - camDim.x/2;
    int y = camPos.y - camDim.y/2;
    int w = camDim.x + 16;
    int h = camDim.y + 16;
  
    componentGrid.query<Renderable>((struct Bounds) {x,y,w,h}, this->renderQuery);

    std::vector<entt::entity> diff;

    // Get the entities which were in the new query but not in the last query
    std::set_difference(this->renderQuery.begin(), this->renderQuery.end(),
        this->lastRenderQuery.begin(), this->lastRenderQuery.end(),
        std::back_inserter(diff));

    for (auto entity : diff) {
        this->registry.emplace<ToRender>(entity);
    }

    diff.clear();
    // Get the entities which were in the last query but not in the new query
    std::set_difference(this->lastRenderQuery.begin(), this->lastRenderQuery.end(),
        this->renderQuery.begin(), this->renderQuery.end(),
        std::back_inserter(diff));

    for (auto entity : diff) {
        this->registry.remove<ToRender>(entity);
        
    }

    this->lastRenderQuery = std::move(this->renderQuery);
    renderQuery.clear();
}

void RenderSystem::sortEntities() {

    // Sort sprites by Spacial y-pos before rendering
    this->registry.sort<ToRender>([this](const entt::entity lhs, const entt::entity rhs) {

        auto lhSpacial = this->registry.get<Spacial>(lhs);
        auto rhSpacial = this->registry.get<Spacial>(rhs);
        
        return lhSpacial.pos.y + lhSpacial.dim.y < rhSpacial.pos.y + rhSpacial.dim.y;

    }, entt::insertion_sort {}); // Insertion sort is much faster as the spacials will generally be "mostly sorted"
}

void RenderSystem::updateModels() {

    // TDOD: Consider only updating models of entities which need rendering

    // Update the models of all the entities whose spacials have been changed
    for (const auto entity : this->spacialObserver) {

        auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
        this->registry.emplace_or_replace<Model>(entity, this->getModel(spacial, texture));
    }

    this->spacialObserver.clear();

    // Create models on entities which have never been rendered
    auto noModelEntities = this->registry.view<Spacial, Texture>(entt::exclude<Model>);

    for (const auto entity : noModelEntities) {

        auto [spacial, texture] = noModelEntities.get<Spacial, Texture>(entity);
        this->registry.emplace<Model>(entity, this->getModel(spacial, texture));
    }
}

glm::mat4 RenderSystem::getModel(const Spacial& spacial, const Texture& texture) {

    // The model does not represent the physical location exactly, but the rendered location
    //  Information from the texture is needed so that the sprite can be placed correctly
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));


    glm::vec3 scaleVec = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
    glm::vec3 dimensionsVec = glm::vec3(texture.frameData->size.x, texture.frameData->size.y, 1);

    glm::mat4 scale = glm::scale(glm::mat4(1), scaleVec*dimensionsVec);

    glm::vec3 offset = glm::vec3(texture.frameData->offset.x, texture.frameData->offset.y, 0);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos + (offset*scaleVec));

    // Order matters
    return (translate * scale * rotate);
}

glm::mat4 RenderSystem::getTileModel(const Spacial& spacial) {

    Texture texture;

    texture.frameData->size = glm::vec2(16.0f, 16.0f);
    texture.frameData->offset = glm::vec2();

    return this->getModel(spacial, texture);
}

void RenderSystem::bufferEntityData() {
    
    this->registry.view<Texture, Model, Tile, ToRender>().each([this](auto& texture, auto& model, auto& tile) {  
        
        glm::vec4 textureData = glm::vec4(texture.frameData->position.x, texture.frameData->position.y, 
            texture.frameData->size.x, texture.frameData->size.y);

        this->renderer.addBufferData(textureData, model.model);
    });

    this->registry.view<Texture, Model, ToRender>(entt::exclude<Text, Tile>).use<ToRender>().each([this](auto& texture, auto& model) {  
        
        glm::vec4 textureData = glm::vec4(texture.frameData->position.x, texture.frameData->position.y, 
            texture.frameData->size.x, texture.frameData->size.y);

        this->renderer.addBufferData(textureData, model.model);
    });
}

void RenderSystem::render() {

    using namespace entt::literals;
    
    Camera camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);
    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();
    Clock clock = this->registry.ctx().at<Clock&>();

    this->renderer.render(camera.getViewMatrix(), camera.getProjectionMatrix(),
        glm::vec2(textureAtlas.width, textureAtlas.height), 
        textureAtlas.gl_texture_id, clock.getCumulativeTime());
}

void RenderSystem::initTextMap() {

    float endPuncuationSpace = 2;
    
    this->textMap[' '] = glm::vec2(0*8, 3); // char start, char width
    this->textMap['!'] = glm::vec2(1*8, 1 + endPuncuationSpace);
    this->textMap['"'] = glm::vec2(2*8, 3);
    this->textMap['#'] = glm::vec2(3*8, 5);
    this->textMap['$'] = glm::vec2(4*8, 5);
    this->textMap['%'] = glm::vec2(5*8, 6);
    this->textMap['&'] = glm::vec2(6*8, 6);
    this->textMap['\''] = glm::vec2(7*8, 1);
    this->textMap['('] = glm::vec2(8*8, 3);
    this->textMap[')'] = glm::vec2(9*8, 3);
    this->textMap['*'] = glm::vec2(10*8, 3);
    this->textMap['+'] = glm::vec2(11*8, 5);
    this->textMap[','] = glm::vec2(12*8, 2);
    this->textMap['-'] = glm::vec2(13*8, 5);
    this->textMap['.'] = glm::vec2(14*8, 2 + endPuncuationSpace);
    this->textMap['/'] = glm::vec2(15*8, 4);
    this->textMap['0'] = glm::vec2(16*8, 5);
    this->textMap['1'] = glm::vec2(17*8, 3);
    this->textMap['2'] = glm::vec2(18*8, 5);
    this->textMap['3'] = glm::vec2(19*8, 5);
    this->textMap['4'] = glm::vec2(20*8, 5);
    this->textMap['5'] = glm::vec2(21*8, 5);
    this->textMap['6'] = glm::vec2(22*8, 5);
    this->textMap['7'] = glm::vec2(23*8, 5);
    this->textMap['8'] = glm::vec2(24*8, 5);
    this->textMap['9'] = glm::vec2(25*8, 5);
    this->textMap[':'] = glm::vec2(26*8, 2);
    this->textMap[';'] = glm::vec2(27*8, 2);
    this->textMap['<'] = glm::vec2(28*8, 5);
    this->textMap['='] = glm::vec2(29*8, 4);
    this->textMap['>'] = glm::vec2(30*8, 5);
    this->textMap['?'] = glm::vec2(31*8, 5 + endPuncuationSpace);
    this->textMap['@'] = glm::vec2(32*8, 7);
    this->textMap['A'] = glm::vec2(33*8, 5); 
    this->textMap['B'] = glm::vec2(34*8, 5);
    this->textMap['C'] = glm::vec2(35*8, 5);
    this->textMap['D'] = glm::vec2(36*8, 5);
    this->textMap['E'] = glm::vec2(37*8, 5);
    this->textMap['F'] = glm::vec2(38*8, 4);
    this->textMap['G'] = glm::vec2(39*8, 6);
    this->textMap['H'] = glm::vec2(40*8, 5);
    this->textMap['I'] = glm::vec2(41*8, 3);
    this->textMap['J'] = glm::vec2(42*8, 5);
    this->textMap['K'] = glm::vec2(43*8, 5);
    this->textMap['L'] = glm::vec2(44*8, 4);
    this->textMap['M'] = glm::vec2(45*8, 7);
    this->textMap['N'] = glm::vec2(46*8, 5);
    this->textMap['O'] = glm::vec2(47*8, 6);
    this->textMap['P'] = glm::vec2(48*8, 5);
    this->textMap['Q'] = glm::vec2(49*8, 6);
    this->textMap['R'] = glm::vec2(50*8, 5);
    this->textMap['S'] = glm::vec2(51*8, 5);
    this->textMap['T'] = glm::vec2(52*8, 5);
    this->textMap['U'] = glm::vec2(53*8, 5);
    this->textMap['V'] = glm::vec2(54*8, 5);
    this->textMap['W'] = glm::vec2(55*8, 7);
    this->textMap['X'] = glm::vec2(56*8, 5);
    this->textMap['Y'] = glm::vec2(57*8, 5);
    this->textMap['Z'] = glm::vec2(58*8, 6);
    this->textMap['['] = glm::vec2(59*8, 3);
    this->textMap['\\'] = glm::vec2(60*8, 4);
    this->textMap[']'] = glm::vec2(61*8, 3);
    this->textMap['^'] = glm::vec2(62*8, 3);
    this->textMap['_'] = glm::vec2(63*8, 5);
    this->textMap['`'] = glm::vec2(64*8, 2);
    this->textMap['a'] = glm::vec2(65*8, 5); 
    this->textMap['b'] = glm::vec2(66*8, 5);
    this->textMap['c'] = glm::vec2(67*8, 5);
    this->textMap['d'] = glm::vec2(68*8, 5);
    this->textMap['e'] = glm::vec2(69*8, 5);
    this->textMap['f'] = glm::vec2(70*8, 5);
    this->textMap['g'] = glm::vec2(71*8, 5);
    this->textMap['h'] = glm::vec2(72*8, 5);
    this->textMap['i'] = glm::vec2(73*8, 2);
    this->textMap['j'] = glm::vec2(74*8, 3);
    this->textMap['k'] = glm::vec2(75*8, 5);
    this->textMap['l'] = glm::vec2(76*8, 2);
    this->textMap['m'] = glm::vec2(77*8, 7);
    this->textMap['n'] = glm::vec2(78*8, 5);
    this->textMap['o'] = glm::vec2(79*8, 5);
    this->textMap['p'] = glm::vec2(80*8, 5);
    this->textMap['q'] = glm::vec2(81*8, 6);
    this->textMap['r'] = glm::vec2(82*8, 5);
    this->textMap['s'] = glm::vec2(83*8, 5);
    this->textMap['t'] = glm::vec2(84*8, 4);
    this->textMap['u'] = glm::vec2(85*8, 5);
    this->textMap['v'] = glm::vec2(86*8, 5);
    this->textMap['w'] = glm::vec2(87*8, 7);
    this->textMap['x'] = glm::vec2(88*8, 5);
    this->textMap['y'] = glm::vec2(89*8, 5);
    this->textMap['z'] = glm::vec2(90*8, 5);
    this->textMap['{'] = glm::vec2(91*8, 3);
    this->textMap['|'] = glm::vec2(92*8, 1);
    this->textMap['}'] = glm::vec2(93*8, 3);
    this->textMap['~'] = glm::vec2(94*8, 6);
}
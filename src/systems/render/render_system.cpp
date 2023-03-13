#include "render_system.hpp"

RenderSystem::RenderSystem(entt::registry& registry) : System(registry), 
    spacial_observer{ entt::observer(registry, entt::collector.update<Spacial>().where<Texture>()) },
    texture_observer{ entt::observer(registry, entt::collector.group<Texture>()) } {
    
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
    auto& camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    auto& component_grid = this->registry.ctx().at<ComponentGrid<Renderable, Collision>&>();

    glm::vec2 camera_dimensions = camera.getCameraDim();
    glm::vec2 camera_position = camera.getPosition();

    int x = camera_position.x - camera_dimensions.x/2;
    int y = camera_position.y - camera_dimensions.y/2;
    int w = camera_dimensions.x + 16;
    int h = camera_dimensions.y + 16;
  
    component_grid.query<Renderable>((lightgrid::bounds) {x,y,w,h}, this->render_query);

    std::vector<entt::entity> diff;

    // Get the entities which were in the new query but not in the last query
    std::set_difference(this->render_query.begin(), this->render_query.end(),
        this->last_render_query.begin(), this->last_render_query.end(),
        std::back_inserter(diff));

    for (auto entity : diff) {
        this->registry.emplace<ToRender>(entity);
    }

    diff.clear();
    // Get the entities which were in the last query but not in the new query
    std::set_difference(this->last_render_query.begin(), this->last_render_query.end(),
        this->render_query.begin(), this->render_query.end(),
        std::back_inserter(diff));

    for (auto entity : diff) {
        this->registry.remove<ToRender>(entity);
        
    }

    this->last_render_query = std::move(this->render_query);
    render_query.clear();
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
    for (const auto entity : this->spacial_observer) {

        auto [spacial, texture] = this->registry.get<Spacial, Texture>(entity);
        this->registry.emplace_or_replace<Model>(entity, this->getModel(spacial, texture));
    }

    this->spacial_observer.clear();

    // Create models on entities which have never been rendered
    auto no_model_entities = this->registry.view<Spacial, Texture>(entt::exclude<Model>);

    for (const auto entity : no_model_entities) {

        auto [spacial, texture] = no_model_entities.get<Spacial, Texture>(entity);
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


    glm::vec3 scale_vector = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
    glm::vec3 dimensions_vector = glm::vec3(texture.frame_data->size.x, texture.frame_data->size.y, 1);

    glm::mat4 scale = glm::scale(glm::mat4(1), scale_vector*dimensions_vector);

    glm::vec3 offset = glm::vec3(texture.frame_data->offset.x, texture.frame_data->offset.y, 0);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos + (offset*scale_vector));

    // Order matters
    return (translate * scale * rotate);
}

void RenderSystem::bufferEntityData() {
    
    this->registry.view<Texture, Model, Tile, ToRender>().each([this](auto& texture, auto& model, auto& tile) {  
        
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y);

        this->renderer.addBufferData(texture_data, model.model);
    });

    this->registry.view<Texture, Model, ToRender>(entt::exclude<Text, Tile>).use<ToRender>().each([this](auto& texture, auto& model) {  
        
        glm::vec4 texture_data = glm::vec4(texture.frame_data->position.x, texture.frame_data->position.y, 
            texture.frame_data->size.x, texture.frame_data->size.y);

        this->renderer.addBufferData(texture_data, model.model);
    });
}

void RenderSystem::render() {

    using namespace entt::literals;
    
    Camera camera = this->registry.ctx().at<Camera&>("world_camera"_hs);
    TextureAtlas& texture_atlas = this->registry.ctx().at<TextureAtlas&>();
    Clock clock = this->registry.ctx().at<Clock&>();

    this->renderer.render(camera.getViewMatrix(), camera.getProjectionMatrix(),
        glm::vec2(texture_atlas.width, texture_atlas.height), 
        texture_atlas.gl_texture_id, clock.getCumulativeTime());
}

void RenderSystem::initTextMap() {

    float puncuation_space = 2;
    
    this->text_map[' '] = glm::vec2(0*8, 3); // char start, char width
    this->text_map['!'] = glm::vec2(1*8, 1 + puncuation_space);
    this->text_map['"'] = glm::vec2(2*8, 3);
    this->text_map['#'] = glm::vec2(3*8, 5);
    this->text_map['$'] = glm::vec2(4*8, 5);
    this->text_map['%'] = glm::vec2(5*8, 6);
    this->text_map['&'] = glm::vec2(6*8, 6);
    this->text_map['\''] = glm::vec2(7*8, 1);
    this->text_map['('] = glm::vec2(8*8, 3);
    this->text_map[')'] = glm::vec2(9*8, 3);
    this->text_map['*'] = glm::vec2(10*8, 3);
    this->text_map['+'] = glm::vec2(11*8, 5);
    this->text_map[','] = glm::vec2(12*8, 2);
    this->text_map['-'] = glm::vec2(13*8, 5);
    this->text_map['.'] = glm::vec2(14*8, 2 + puncuation_space);
    this->text_map['/'] = glm::vec2(15*8, 4);
    this->text_map['0'] = glm::vec2(16*8, 5);
    this->text_map['1'] = glm::vec2(17*8, 3);
    this->text_map['2'] = glm::vec2(18*8, 5);
    this->text_map['3'] = glm::vec2(19*8, 5);
    this->text_map['4'] = glm::vec2(20*8, 5);
    this->text_map['5'] = glm::vec2(21*8, 5);
    this->text_map['6'] = glm::vec2(22*8, 5);
    this->text_map['7'] = glm::vec2(23*8, 5);
    this->text_map['8'] = glm::vec2(24*8, 5);
    this->text_map['9'] = glm::vec2(25*8, 5);
    this->text_map[':'] = glm::vec2(26*8, 2);
    this->text_map[';'] = glm::vec2(27*8, 2);
    this->text_map['<'] = glm::vec2(28*8, 5);
    this->text_map['='] = glm::vec2(29*8, 4);
    this->text_map['>'] = glm::vec2(30*8, 5);
    this->text_map['?'] = glm::vec2(31*8, 5 + puncuation_space);
    this->text_map['@'] = glm::vec2(32*8, 7);
    this->text_map['A'] = glm::vec2(33*8, 5); 
    this->text_map['B'] = glm::vec2(34*8, 5);
    this->text_map['C'] = glm::vec2(35*8, 5);
    this->text_map['D'] = glm::vec2(36*8, 5);
    this->text_map['E'] = glm::vec2(37*8, 5);
    this->text_map['F'] = glm::vec2(38*8, 4);
    this->text_map['G'] = glm::vec2(39*8, 6);
    this->text_map['H'] = glm::vec2(40*8, 5);
    this->text_map['I'] = glm::vec2(41*8, 3);
    this->text_map['J'] = glm::vec2(42*8, 5);
    this->text_map['K'] = glm::vec2(43*8, 5);
    this->text_map['L'] = glm::vec2(44*8, 4);
    this->text_map['M'] = glm::vec2(45*8, 7);
    this->text_map['N'] = glm::vec2(46*8, 5);
    this->text_map['O'] = glm::vec2(47*8, 6);
    this->text_map['P'] = glm::vec2(48*8, 5);
    this->text_map['Q'] = glm::vec2(49*8, 6);
    this->text_map['R'] = glm::vec2(50*8, 5);
    this->text_map['S'] = glm::vec2(51*8, 5);
    this->text_map['T'] = glm::vec2(52*8, 5);
    this->text_map['U'] = glm::vec2(53*8, 5);
    this->text_map['V'] = glm::vec2(54*8, 5);
    this->text_map['W'] = glm::vec2(55*8, 7);
    this->text_map['X'] = glm::vec2(56*8, 5);
    this->text_map['Y'] = glm::vec2(57*8, 5);
    this->text_map['Z'] = glm::vec2(58*8, 6);
    this->text_map['['] = glm::vec2(59*8, 3);
    this->text_map['\\'] = glm::vec2(60*8, 4);
    this->text_map[']'] = glm::vec2(61*8, 3);
    this->text_map['^'] = glm::vec2(62*8, 3);
    this->text_map['_'] = glm::vec2(63*8, 5);
    this->text_map['`'] = glm::vec2(64*8, 2);
    this->text_map['a'] = glm::vec2(65*8, 5); 
    this->text_map['b'] = glm::vec2(66*8, 5);
    this->text_map['c'] = glm::vec2(67*8, 5);
    this->text_map['d'] = glm::vec2(68*8, 5);
    this->text_map['e'] = glm::vec2(69*8, 5);
    this->text_map['f'] = glm::vec2(70*8, 5);
    this->text_map['g'] = glm::vec2(71*8, 5);
    this->text_map['h'] = glm::vec2(72*8, 5);
    this->text_map['i'] = glm::vec2(73*8, 2);
    this->text_map['j'] = glm::vec2(74*8, 3);
    this->text_map['k'] = glm::vec2(75*8, 5);
    this->text_map['l'] = glm::vec2(76*8, 2);
    this->text_map['m'] = glm::vec2(77*8, 7);
    this->text_map['n'] = glm::vec2(78*8, 5);
    this->text_map['o'] = glm::vec2(79*8, 5);
    this->text_map['p'] = glm::vec2(80*8, 5);
    this->text_map['q'] = glm::vec2(81*8, 6);
    this->text_map['r'] = glm::vec2(82*8, 5);
    this->text_map['s'] = glm::vec2(83*8, 5);
    this->text_map['t'] = glm::vec2(84*8, 4);
    this->text_map['u'] = glm::vec2(85*8, 5);
    this->text_map['v'] = glm::vec2(86*8, 5);
    this->text_map['w'] = glm::vec2(87*8, 7);
    this->text_map['x'] = glm::vec2(88*8, 5);
    this->text_map['y'] = glm::vec2(89*8, 5);
    this->text_map['z'] = glm::vec2(90*8, 5);
    this->text_map['{'] = glm::vec2(91*8, 3);
    this->text_map['|'] = glm::vec2(92*8, 1);
    this->text_map['}'] = glm::vec2(93*8, 3);
    this->text_map['~'] = glm::vec2(94*8, 6);
}
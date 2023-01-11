#include "RenderSystem.h"

RenderSystem::RenderSystem(entt::registry& registry) : System(registry), spriteShader{ new SpriteShader() },
    tileShader{ new TileShader() }, screenShader{ new ScreenShader() }, instancedShader{ new InstancedShader() },
    spacialObserver{ entt::observer(registry, entt::collector.update<Spacial>().where<Texture>()) },
    textureObserver{ entt::observer(registry, entt::collector.group<Texture>()) },
    textSprite{ (struct Texture){"./src/assets/fonts/text.png"} } {
    
        glClearColor(0.0f, 0.4f, 0.4f, 0.0f);
        glEnable(GL_BLEND); 
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->initTextMap();
        this->initVAO();
        this->initScreenFBO();

        this->initVBOs();
}

RenderSystem::~RenderSystem() {
    glDeleteFramebuffers(1, &(this->screenFBO));  
}

void RenderSystem::initVAO() {

    // create VAO
    float quadVertexData[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);
    
    // The verticies will never change so the buffer ID is not saved
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexData), quadVertexData, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    // Free bound buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);  
}

void RenderSystem::initVBOs() {

    glGenBuffers(1, &(this->textureCoordinatesVBO));
    glGenBuffers(1, &(this->modelsVBO));

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * this->textureCoordinatesBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(1, 1); 

    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->modelsBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(4); 
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5); 
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6); 
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void RenderSystem::initScreenFBO() {

    glGenFramebuffers(1, &(this->screenFBO));
    glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);

    glGenTextures(1, &(this->screenTexture));
    glBindTexture(GL_TEXTURE_2D, this->screenTexture);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_c::SCREEN_WIDTH, render_c::SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->screenTexture, 0); 

    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void RenderSystem::update() {

    this->cullEntities();
    this->sortEntities();

    this->updateModels();

    this->fillBufferData();
    this->render();
}

void RenderSystem::cullEntities() {

    using namespace entt::literals;
    Camera& camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);

    // Label which entities are on screen and should be rendered
    this->registry.view<Texture, Model, Spacial>(entt::exclude<Text>).each([this, camera](const auto entity, auto& sprite, auto& model, auto& spacial) {  

        glm::vec2 camDim = camera.getCameraDim();
        glm::vec2 camPos = camera.getPosition();
        glm::vec3 entPos = spacial.pos;

        if (entPos.x < camPos.x + camDim.x/2 && entPos.y - spacial.dim.y < camPos.y + camDim.y/2 && entPos.x + spacial.dim.x > camPos.x - camDim.x/2 && entPos.y > camPos.y - camDim.y/2) {
            
            // if (!this->registry.all_of<ToRender>(entity)) {
                this->registry.emplace_or_replace<ToRender>(entity);
            // }
            
        } else {
            this->registry.remove<ToRender>(entity);
        }
    });
}

void RenderSystem::sortEntities() {

    // Sort sprites by Spacial y-pos before rendering
    this->registry.sort<ToRender>([this](const entt::entity lhs, const entt::entity rhs) {

        auto lhSpacial = this->registry.get<Spacial>(lhs);
        auto rhSpacial = this->registry.get<Spacial>(rhs);
        
        return lhSpacial.pos.y + lhSpacial.dim.y < rhSpacial.pos.y + rhSpacial.dim.y;
        
    }, entt::insertion_sort {}); // Insertion sort is much faster as the spacials will generally be "mostly sorted"
}

void RenderSystem::fillBufferData() {

    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->textureCoordinatesBufferData.size(), NULL, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*this->modelsBufferData.size(), NULL, GL_STREAM_DRAW);

    textureCoordinatesBufferData.clear();
    modelsBufferData.clear();

    this->registry.view<Texture, Model, ToRender>(entt::exclude<Text>).use<ToRender>().each([this](auto& texture, auto& model) {  
        
        glm::vec4 textureData = glm::vec4(texture.frameData.position.x, texture.frameData.position.y, 
            texture.frameData.size.x, texture.frameData.size.y);

        this->textureCoordinatesBufferData.push_back(textureData);
        this->modelsBufferData.push_back(model.model);
    });

    glBindBuffer(GL_ARRAY_BUFFER, this->textureCoordinatesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4)*this->textureCoordinatesBufferData.size(), this->textureCoordinatesBufferData.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*this->modelsBufferData.size(), this->modelsBufferData.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderSystem::render() {

    glBindFramebuffer(GL_FRAMEBUFFER, this->screenFBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    using namespace entt::literals;
    
    Camera camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);
    TextureAtlas& textureAtlas = this->registry.ctx().at<TextureAtlas&>();

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = camera.getProjectionMatrix();

    this->instancedShader->useShader();
    this->instancedShader->renderSetup(view, projection, glm::vec2(textureAtlas.width, textureAtlas.height));

    glBindTexture(GL_TEXTURE_2D, textureAtlas.glTextureID);

    glBindVertexArray(this->VAO);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->textureCoordinatesBufferData.size()); 

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    
    glClear(GL_COLOR_BUFFER_BIT);



    this->screenShader->useShader();

    // glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_2D, this->screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);   

    glUseProgram(0);
    glBindVertexArray(0);
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
        this->registry.emplace_or_replace<Model>(entity, this->getModel(spacial, texture));
    }
}

glm::mat4 RenderSystem::getModel(Spacial spacial, Texture texture) {

    // The model does not represent the physical location exactly, but the rendered location
    //  Information form the texture is needed so that the sprite can be placed correctly
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 rotate = glm::mat4(1.0f);
    
    rotate = glm::rotate(rotate, spacial.rot.x, glm::vec3(1, 0, 0));
    rotate = glm::rotate(rotate, spacial.rot.y, glm::vec3(0, 1, 0));
    rotate = glm::rotate(rotate, spacial.rot.z, glm::vec3(0, 0, 1));


    glm::vec3 scaleVec = glm::vec3(spacial.scale.x, spacial.scale.y, spacial.scale.z);
    glm::vec3 dimensionsVec = glm::vec3(texture.frameData.size.x, texture.frameData.size.y, 1);

    glm::mat4 scale = glm::scale(glm::mat4(1), scaleVec*dimensionsVec);

    glm::vec3 offset = glm::vec3(texture.frameData.offset.x, texture.frameData.offset.y, 0);

    glm::mat4 translate = glm::translate(glm::mat4(1), spacial.pos + (offset*scaleVec));

    // Order matters
    return (translate * scale * rotate);
}

// void RenderSystem::renderTiles() {

//     glBindVertexArray(this->VAO);

//     this->tileShader->useShader();

//     using namespace entt::literals;
//     Camera camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);

//     glm::mat4 view = camera.getViewMatrix();
//     glm::mat4 projection = camera.getProjectionMatrix();

//     Model model{glm::mat4(1)};
//     Spacial spacial{glm::vec3{0,0,0}, glm::vec3{0,0,0}, glm::vec3{1,1,1}, glm::vec2{16,16}};

//     this->getModel(model, spacial);

//     registry.view<TileSet, Texture>().each([this, model, view, projection](const auto entity, auto& tileSet, auto& texture) {

//         this->tileShader->renderSetup(model.model, view, projection, texture.texData);

//         if (tileSet.tileVBO == -1) {
//             this->initTileVBO(tileSet);
//         }
//         glBindVertexArray(this->VAO);
//         // Set the data for this tileSet
//         glBindBuffer(GL_ARRAY_BUFFER, tileSet.tileVBO);
        
//         glEnableVertexAttribArray(1);
//         glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//         glVertexAttribDivisor(1, 1); 

//         glBindBuffer(GL_ARRAY_BUFFER, 0); 


//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_2D, texture.glTextureID);

//         // Remove anti-aliasing
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//         // printf("%d\n", tileSet.tileData.size());
//         glBindVertexArray(this->VAO); // !!
//         glDrawArraysInstanced(GL_TRIANGLES, 0, 6, tileSet.tileData.size()); 
//         glBindVertexArray(0);
//     });
// }

// void RenderSystem::renderEntities() {

//     using namespace entt::literals;
//     Camera& camera = this->registry.ctx().at<Camera&>("worldCamera"_hs);

//     // Label which entities are on screen and should be rendered
//     registry.view<Texture, Model, Spacial>(entt::exclude<Text>).each([this, camera](const auto entity, auto& sprite, auto& model, auto& spacial) {  

//         glm::vec2 camDim = camera.getCameraDim();
//         glm::vec2 camPos = camera.getPosition();
//         glm::vec3 entPos = spacial.pos;

//         if (entPos.x < camPos.x + camDim.x/2 && entPos.y - spacial.dim.y < camPos.y + camDim.y/2 && entPos.x + spacial.dim.x > camPos.x - camDim.x/2 && entPos.y > camPos.y - camDim.y/2) {
            
//             // if (!this->registry.all_of<ToRender>(entity)) {
//                 this->registry.emplace_or_replace<ToRender>(entity);
//             // }
            
//         } else {
//             this->registry.remove<ToRender>(entity);
//         }
//     });

//     // double start = SDL_GetPerformanceCounter();

//     // Sort sprites by Spacial y-pos before rendering
//     registry.sort<ToRender>([this](const entt::entity lhs, const entt::entity rhs) {
//         return this->registry.get<Spacial>(lhs).pos.y < this->registry.get<Spacial>(rhs).pos.y;
//     }, entt::insertion_sort {}); // Insertion sort is much faster as the spacials will generally be "mostly sorted"

//     // double sortTime = (SDL_GetPerformanceCounter() - start) / SDL_GetPerformanceFrequency() * 1000.0;
//     // start = SDL_GetPerformanceCounter();

//     // Render all non-text entities labeled with ToRender; Ordered by the sorted spacial
//     registry.view<Texture, Model, Spacial, ToRender>(entt::exclude<Text>).use<ToRender>().each([this](auto& texture, auto& model, auto& spacial) {  
//         this->renderTexture(model, texture);
//     });

//     // double renderTime = (SDL_GetPerformanceCounter() - start) / SDL_GetPerformanceFrequency() * 1000.0;

//     // std::cout << std::endl << "Sort time: " << sortTime << "\nRender time: " << renderTime << std::endl;

//     // registry.clear<ToRender>();    

//     auto texts = registry.view<Text, Spacial>();

//     // Render all text objects
//     for (auto textEntity : texts) {

//         auto [text, spacial] = texts.get(textEntity);

//         this->renderText(text, spacial);
//     }

// }

// void RenderSystem::renderText(Text text, Spacial spacial) {

//     Model cModel;
//     float charOffset = 0;

//     for (auto& c : text.text) {

//         glm::vec2 charData = textMap[c];
//         float kerning = 1.5;

//         Spacial cSpacial{spacial};
//         cSpacial.pos = glm::vec3(spacial.pos.x + charOffset, spacial.pos.y, 0);
//         cSpacial.dim.x = charData.y;
//         this->getModel(cModel, cSpacial);

//         this->textSprite.texData = glm::vec2(charData.x/spacial.dim.x, (charData.y + charData.x)/spacial.dim.x);

//         this->renderTexture(cModel, textSprite, text.guiElement);

//         charOffset += charData.y + kerning;
//     }
// }

// void RenderSystem::renderTexture(Model model, Texture texture, bool guiElement) {

//     // Use the sprite shader
//     // GLuint openGLShaderProgramID = this->spriteShader->getOpenGLShaderProgramID();
//     // glUseProgram(openGLShaderProgramID);

//     this->spriteShader->useShader();

//     using namespace entt::literals;

//     Camera cam = guiElement ? this->registry.ctx().at<Camera&>("guiCamera"_hs) : this->registry.ctx().at<Camera&>("worldCamera"_hs);
//     glm::mat4 view = cam.getViewMatrix();
//     glm::mat4 projection = cam.getProjectionMatrix();

//     this->spriteShader->renderSetup(model.model, view, projection, texture.texData);

//     glBindVertexArray(this->VAO);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture.glTextureID);

//     // Remove anti-aliasing
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

//     glDrawArrays(GL_TRIANGLES, 0, 6); 
//     // glBindVertexArray(0); Causes : "Program/shader state performance warning: Vertex shader in program 3 is being recompiled based on GL state.""
// }

// void RenderSystem::updateTiles() {
    
    // std::vector<glm::vec3> newTiles;

    // for (const auto entity : this->tileObserver) { 

    //     auto [spacial, tile] = this->registry.get<Spacial, Tile>(entity); 
    //     newTiles.emplace_back(spacial.pos.x, spacial.pos.y, tile.id);
    // }

    // this->tileObserver.clear();

    // if (newTiles.size() > 0) {

    //     this->tiles = newTiles;

    //     glBindVertexArray(this->VAO);
    //     glBindBuffer(GL_ARRAY_BUFFER, this->tileVBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->tiles.size(), this->tiles.data(), GL_STATIC_DRAW);
    //     glBindVertexArray(0);
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    // }
// }

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
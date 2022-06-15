#pragma once

namespace render_c {
    //Screen dimension constants
    static const int SCREEN_WIDTH = 1440;
    static const int SCREEN_HEIGHT = 810;
    // static const int SCREEN_WIDTH = 1920;
    // static const int SCREEN_HEIGHT = 1080;
    
    // static const int SCREEN_WIDTH = 800;
    // static const int SCREEN_HEIGHT = 800;

    //Color modes
    static const int COLOR_MODE_CYAN = 0;
    static const int COLOR_MODE_MULTI = 1;

    // static const int SCREEN_FPS = 60;
    static const float SPR_SCALE_UP = 5.0f;
}

namespace input_c {

    static const float MOUSE_SPEED = 0.003f;
    static const float KEY_SPEED = 300.0f;
}

namespace entity_c {

    enum ENTITY_STATE {
        IDLE,
        MOVING
    };

    enum ENTITY_DIR {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
}

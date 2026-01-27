#include "SLList.h"
#include "Stopwatch.h"
#include "WindowContext.h"
#include "Validate_data.h"
#include <thread>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>

#include "Sprite.h"

#include "UniformGrid.h"
#include "Canvas.h"
#include "Sequence.h"
#include "load_data.h"

#include "RandomNum.h"

#include "Color.h"
#include "Camera.h"
#include "TextureMap.h"

#include <iostream>

#include "Entity.h"
#include "Collision.h"

/*
NEW HEADERS translate.h entity.h

//TODO::camera then
//TODO::texture map finish then
//TODO::translation/entity stuff



TRANSLATION PIPELINE:
    entity has a static model
    entity poops out a model, applying its scale and offset (pos)

    the model gets translated related to the camera
    first subtracting offset (pos) (because top left corner)
    then scaling the model

    thirdly everything gets translated retated to the center of screen
    first flipping y axis then applying offset
*/
int main() {

    using namespace badCore;
    using namespace badWindow;

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    {
        using namespace badEngine;
        //using namespace badEngine;
        validate_json_file("../Configs/system_config.json");
        nlohmann::json window_conf = load_json("../Configs/system_config.json");
        validate_WindowContext_manifest(window_conf, "sys_config");
        auto windowContextData = extract_WindowContext_desc(window_conf, "sys_config");
        WindowContext window(windowContextData.heading.c_str(), windowContextData.width, windowContextData.height, windowContextData.flags);


        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 



        RandomNum rng;
        Camera cam;
        float cam_speed = 50;

        PlankBro bro(float2(100, 100), float2(600, 0));
        static constexpr int kekL = 1;
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;
        SDL_Event EVENT;
        //this whole main loop is badly bad but engine class in the future so fuck it for now
        Stopwatch steper;
        while (GAME_RUNNING) {
            float dt = steper.dt_float();
            //CLEAR RENDERING
            window.system_refresh();

            //LISTEN TO EVENTS
            while (SDL_PollEvent(&EVENT)) {

                switch (EVENT.type) {
                case SDL_EVENT_QUIT:
                    GAME_RUNNING = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (EVENT.key.key == SDLK_W) {
                        bro.move_vector_y(-10);
                    }
                    if (EVENT.key.key == SDLK_A) {
                        bro.move_vector_x(-10);
                    }
                    if (EVENT.key.key == SDLK_S) {
                        bro.move_vector_y(10);
                    }
                    if (EVENT.key.key == SDLK_D) {
                        bro.move_vector_x(10);
                    }
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    if (EVENT.wheel.y > 0) {
                        // Scrolled up/away from user
                        cam.set_zoom(cam.get_zoom() * 1.25f);
                    }
                    else if (EVENT.wheel.y < 0) {
                        // Scrolled down/toward user
                        cam.set_zoom(cam.get_zoom() * 0.75f);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    //TODO mouse scrolling
                    break;
                default:break;
                }
            }

            //collect models
            auto ray = bro.get_ray();

            //transform models


            //draw models
            window.draw_line(ray, bro.get_color());



            window.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

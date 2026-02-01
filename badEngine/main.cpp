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
#include "Circle.h"


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
        int ent_count = 500;

        float maxo = 128;
        float mino = 96;

        float maxi = 96;
        float mini = 32;

        float maxf = 16;
        float minf = 4;

        float maxrv = 4;
        float minrv = -4;


        float wminx = -5000;
        float wmaxx = 5000;

        float wminy = -5000;
        float wmaxy = 5000;


        Sequence<Entity> entities;
        entities.set_capacity(ent_count);
        for (int i = 0; i < ent_count; i++) {
            Sequence<float2> model = make_poly(
                rng.rFloat(mino, maxo),
                rng.rFloat(mini, maxi),
                rng.rFloat(minf, maxf)
            );
            float2 pos = { rng.rFloat(wminx,wmaxx), rng.rFloat(wminy,wmaxy) };
            float angular_vel = rng.rFloat(minrv, maxrv);
            Color col(rng.rFloat(1, 255), rng.rFloat(1, 255), rng.rFloat(1, 255), 255);
            float scalr_differential = rng.rFloat(0.005f, 0.01f);

            entities.emplace_back(std::move(model), pos, angular_vel, col, scalr_differential);
        }



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
                        cam.move_by(float2(0,-10));
                    }
                    if (EVENT.key.key == SDLK_A) {
                        cam.move_by(float2(-10, 0));
                    }
                    if (EVENT.key.key == SDLK_S) {
                        cam.move_by(float2(0, 10));
                    }
                    if (EVENT.key.key == SDLK_D) {
                        cam.move_by(float2(10, 0));
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
            for (auto& e : entities) {
                e.rotate(dt);

                e.pulse_scale();
            }

            Mat3 window_mat = window.get_transform();
            Mat3 camera_mat = cam.get_transform();

            for (auto& e : entities) {
                Mat3 world = window_mat * camera_mat * e.get_transform();

                window.draw_closed_model(e.get_model(), world, e.get_color());
            }


            window.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

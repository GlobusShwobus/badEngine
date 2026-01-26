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
#include "Collision_test.h"

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
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 

        using namespace badEngine;
        //using namespace badEngine;
        validate_json_file("../Configs/system_config.json");
        nlohmann::json window_conf = load_json("../Configs/system_config.json");
        validate_WindowContext_manifest(window_conf, "sys_config");
        auto windowContextData = extract_WindowContext_desc(window_conf, "sys_config");
        WindowContext window(windowContextData.heading.c_str(), windowContextData.width, windowContextData.height, windowContextData.flags);




        validate_json_file("../Configs/textures.json");
        nlohmann::json texture_loader_config = load_json("../Configs/textures.json");
        auto extractDescriptions = extract_texture_descs(texture_loader_config, "texture_set1");
        auto loaded_textures = load_textures(extractDescriptions, window.get_render());

        TextureMap texture_map;

        for (auto& texture : loaded_textures) {
            bool is_nullptr = texture.second.get() == nullptr;
            bool is_good = texture_map.add(texture.first, std::move(texture.second));
        }

        auto texture_map_check = texture_map.get_tags();

        for (auto& e : texture_map_check) {
            std::cout << e << '\n';
        }

        RandomNum rng;
        Camera cam;
        float cam_speed = 50;
        Sequence<StarBro> entities;

        for (int i = 0; i < 500; i++) {
            float rad = rng.rFloat(80, 120);
            float rat = rng.rFloat(0.15, 0.8);
            auto model = make_poly(rad, rad*rat, rng.rFloat(3, 33));
            auto pos = float2(rng.rFloat(-5000, 5000), rng.rFloat(-5000, 5000));
            auto col = Color(rng.rInt(0,255), rng.rInt(0, 255), rng.rInt(0, 255), 255);
            float vel = rng.rFloat(0.4, 1.6);
            entities.emplace_back(std::move(model),rad, pos, col, vel);
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
                        cam.move_by(float2(0, cam_speed));
                    }
                    if (EVENT.key.key == SDLK_A) {
                        cam.move_by(float2(-cam_speed, 0));
                    }
                    if (EVENT.key.key == SDLK_S) {
                        cam.move_by(float2(0, -cam_speed));
                    }
                    if (EVENT.key.key == SDLK_D) {
                        cam.move_by(float2(cam_speed, 0));
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
                e.update(dt);
            }

            //transform models
            int2 window_size = window.get_window_size();

            Mat3 window_matrix = window.window_transform();
            Mat3 camera_matrix = cam.get_transform();
            Mat3 static_matrix = window_matrix * camera_matrix;

            //draw models
            int draws = 0;
            for (auto& entity : entities) {
                
                if (intersects(entity.get_aabb(), cam.get_viewport(window_size))) {
                    Mat3 final = static_matrix * entity.get_transform();

                    const auto& model = entity.get_model();

                    for (int i = 0; i < model.size(); i++) {

                        float2 a = final.transform(model[i]);
                        float2 b = final.transform(model[(i + 1) % model.size()]);
                        
                        float2 dir = normalized(b - a);
                        float mag = length(b - a);

                        Ray ray(a, dir, mag);

                        window.draw_line(ray, entity.get_color());
                    }
                    draws++;
                }
                
            }

            std::cout << "i draw " << draws << '\n';
            window.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

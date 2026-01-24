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

#include "TransformModel.h"
#include "Entity.h"


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
        auto textureData2 = load_textures(extractDescriptions, window.get_render());

        TextureMap testmeplz(textureData2);


        
        Sequence<Entity> entities;

        entities.emplace_back(make_poly(64, 32, 5), float2(460, 0.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(150, 300.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(250, -200.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(-250, 200.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(0, 0.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(-150, -300.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(400, 300.0f));
        entities.emplace_back(make_poly(64, 32, 5), float2(69, 420.0f));

        const float speed = 20.0f;

        Camera cam;

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
                        cam.move_by(float2(0,speed));
                    }
                    if (EVENT.key.key == SDLK_A) {
                        cam.move_by(float2(-speed, 0));
                    }
                    if (EVENT.key.key == SDLK_S) {
                        cam.move_by(float2(0, -speed));
                    }
                    if (EVENT.key.key == SDLK_D) {
                        cam.move_by(float2(speed, 0));
                    }
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    if (EVENT.wheel.y > 0) {
                        // Scrolled up/away from user
                        cam.set_scale(cam.get_scale() * 1.25f);
                    }
                    else if (EVENT.wheel.y < 0) {
                        // Scrolled down/toward user
                        cam.set_scale(cam.get_scale() * 0.75f);
                    }
                    break;
                default:break;
                }
            }

            //collect models
            Sequence<TransformModel> tModels;
            for (const auto& e : entities) {
                tModels.push_back(e.get_model());
            }
            //transform models
            int2 window_size = window.get_window_size();
            for (auto& e : tModels) {
                cam.transform_model(e);
                translate_in_window(e, window_size);
            }

            //draw models
            Sequence<Sequence<float2>> models;
            for (auto& each : models) {
                for (int i = 0; i < each.size(); i++) {

                    auto& begin = each[i];

                    int next;

                    if (i + 1 == each.size()) {
                        next = 0;
                    }
                    else {
                        next = i + 1;
                    }

                    window.draw_line(begin, each[next], Colors::Red);
                }
            }


            window.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

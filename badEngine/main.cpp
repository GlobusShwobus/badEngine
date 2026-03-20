#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>
#include <iostream>

#include "badCore.h"
#include "GFX_INIT.h"
#include "Geometric.h"

#include "MakeShape.h"
#include "Validate_data.h"
#include "load_data.h"
#include "Entity.h"
#include "MouseCameraController.h"
#include "EngineUtils.h"
#include "FreeDraw.h"

int main() {

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);

    {
        bad::GFX_INIT sdl_sys(SDL_INIT_VIDEO);


        //using namespace badEngine;
        bad::validate_json_file("../badEngine/Configs/system_config.json", bad::expected_file_type::WINDOW_JSON);
        nlohmann::json window_conf = bad::load_json("../badEngine/Configs/system_config.json");
        auto windowContextData = bad::create_window_description(window_conf, "sys_config");

        bad::Window window = bad::make_window(windowContextData.heading, windowContextData.width, windowContextData.height, windowContextData.flags);
        bad::Renderer renderer = bad::make_renderer(window.get(), {});

        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 

        rnd::Entity entity(bad::make_poly(64, 32, 6), { 200,200 }, 1, 0);
        bad::MouseCameraController camera;

        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;

        //this whole main loop is badly bad but engine class in the future so fuck it for now
        bad::Stopwatch steper;
        while (GAME_RUNNING) {
            SDL_Event EVENT;
            float dt = steper.dt_float();
            //CLEAR RENDERING
            SDL_RenderClear(renderer.get());

            //LISTEN TO EVENTS
            while (SDL_PollEvent(&EVENT)) {

                switch (EVENT.type) {
                case SDL_EVENT_QUIT:
                    GAME_RUNNING = false;
                    break;

            
                default:
                    break;
                }
                camera.update(dt, EVENT);
            }


            auto window_transform = bad::sdl_window_matrix(window.get());
            auto camera_transform = camera.mCamera.make_transformed_inverse();
            auto entity_transform = entity.mTransform.make_transformed();

            auto final_transform = window_transform * camera_transform * entity_transform;

            bad::draw_closed_model_transformed(renderer.get(), entity.get_model(), final_transform, entity.get_color());


            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());

            //logger.log(std::string("FPS ") + std::to_string(1 / dt));
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

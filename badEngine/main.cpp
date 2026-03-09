#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#include <conio.h>
#include <iostream>


#include "Stopwatch.h"
#include "RandomNum.h"
#include "Validate_data.h"
#include "load_data.h"
#include "Sprite.h"
#include "UniformGrid.h"
#include "Canvas.h"
#include "Sequence.h"
#include "Color.h"
#include "TextureMap.h"
#include "Make_Shape.h"
#include "Entity.h"
#include "MouseCameraController.h"
#include "SDL_SYSTEM_RAII.h"
#include "Window.h"
#include "Renderer.h"
#include "EngineUtils.h"
#include "Draw.h"
#include "AsyncLogger.h"

int main() {

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
    {
        using namespace badEngine;
        using namespace badCore;
        using namespace badWindow;

        SDL_SYSTEM_RAII sdl_sys(SDL_INIT_VIDEO);


        //using namespace badEngine;
        validate_json_file("../Configs/system_config.json", expected_file_type::WINDOW_JSON);
        nlohmann::json window_conf = badEngine::load_json("../Configs/system_config.json");
        auto windowContextData = badEngine::create_window_description(window_conf, "sys_config");

        Window window = make_window(windowContextData.heading.c_str(), windowContextData.width, windowContextData.height, windowContextData.flags);
        Renderer renderer = make_renderer(window.get(), nullptr);


        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 

        RandomNum rng;
        MouseCameraController cam;
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
        entities.reserve(ent_count);
        for (int i = 0; i < ent_count; i++) {
            Sequence<float2> model = make_poly(
                rng.get(mino, maxo),
                rng.get(mini, maxi),
                rng.get(minf, maxf)
            );
            float2 pos = { rng.get(wminx,wmaxx), rng.get(wminy,wmaxy) };
            float angular_vel = rng.get(minrv, maxrv);
            Color col(rng.get(1, 255), rng.get(1, 255), rng.get(1, 255), 255);
            float scalr_differential = rng.get(0.005f, 0.01f);

            std::vector<float2> vmodel(model.begin(), model.end());

            entities.emplace_back(std::move(vmodel), pos, angular_vel, col, scalr_differential);
        }

        badCore::AsyncLogger logger;
        //TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE TEST CODE 
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################
        //#####################################################################################################################################################################

        //main loop
        bool GAME_RUNNING = true;

        //this whole main loop is badly bad but engine class in the future so fuck it for now
        Stopwatch steper;
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

                default:break;
                }

                cam.update(dt, EVENT);
            }


            //collect models
            for (auto& e : entities) {
                e.rotate(dt);

                e.pulse_scale();
            }

            Mat3 window_mat = sdl_window_matrix(window.get());
            Mat3 camera_mat = cam.get_camera().transform_inverse();


            for (auto& e : entities) {
                Mat3 world = window_mat * camera_mat * e.get_transform();
                const auto& model = e.mModel;
                draw_closed_model(renderer.get(), model.data(), model.size(), world, e.col);
            }

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_RenderPresent(renderer.get());

            logger.log(std::string("FPS ") + std::to_string(1 / dt));
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

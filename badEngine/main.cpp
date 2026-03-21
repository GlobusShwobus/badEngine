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
        bad::RandomNum rng;
        auto rPos = rng.get_real_distribution(-5000.f, 5000.f);
        auto rOuterRadius = rng.get_normal_distribution(32.f,16.f);
        auto rInnerRadius = rng.get_normal_distribution(16.f, 8.f);
        auto rFlares      = rng.get_normal_distribution(10.f, 6.f);

        auto rColor = rng.get_int_distribution(0, 255);

        bad::Sequence<rnd::Entity> entities;
        entities.reserve(500);


        while (entities.size() < 500)
        {
            const auto big_rad = bad::core_clamp(rOuterRadius(rng.engine), 16.f, 32.f);
            auto pos = bad::Point(rPos(rng.engine), rPos(rng.engine));

            if (std::any_of(entities.begin(), entities.end(), [&](const rnd::Entity& e)
                {
                    return bad::length(e.mTransform.mPos-pos) < big_rad + e.radius;
                }
            )) {
                continue;
            }

            const auto small_rad = bad::core_clamp(rInnerRadius(rng.engine), 8.0f, 16.f);
            const auto flares = bad::core_clamp(rFlares(rng.engine), 3.f, 12.f);

            auto scale = rng.get(1, 5);
            auto radians = rng.get(0, 6);
            auto color = bad::Color(rColor(rng.engine), rColor(rng.engine), rColor(rng.engine), 255);

            auto model = bad::make_poly(big_rad, small_rad, flares);
            rnd::Entity ent(std::move(model), big_rad, pos, scale, radians, color);

            ent.pulse_dir = rng.get(-4.2f, 4.2f);
            ent.rotational_velocity = rng.get(-4.2f, 4.2f);

            entities.push_back(std::move(ent));
        }

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

            camera.mCamera.update_sincos();
            auto window_transform = bad::sdl_window_matrix(window.get());
            auto camera_transform = camera.mCamera.make_transformed_inverse();

            const auto camera_viewport = camera.get_viewport(window.get());

            int draws = 0;
            for (auto& e : entities) {
                e.pulse_effect(dt);
                e.rotate(dt);
                auto entity_bb = e.get_bb();

                if (camera_viewport.intersects(entity_bb)) {
                    //for final draw (local -> world -> camera -> window)
                    auto final_transform = window_transform * camera_transform * e.mTransform.make_transformed();
                    //for bounding boxes: only camera and window (world -> camera -> window)
                    auto screen_transform = window_transform * camera_transform;;
                    bad::draw_rect_lines(renderer.get(), entity_bb, screen_transform, e.mColor);
                    bad::draw_closed_model_transformed(renderer.get(), e.get_model(), final_transform, e.mColor);
                    draws++;
                }

            }

            bad::AsyncLogger::Global().log("draws: " + std::to_string(draws));

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

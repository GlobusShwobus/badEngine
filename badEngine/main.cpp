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
#include "MouseCameraController.h"
#include "EngineUtils.h"
#include "FreeDraw.h"
#include "CollisionRoutines.h"
#include "Circle.h"


// TODO:: test uniform grid again

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

        struct Entity
        {
            bad::Sequence<bad::Point> base_model;
            bad::Transform transform;
            bad::Color col;

            float rotational_velocity;
            float scale_dir = 0.66f;

            //meme
            void pulse(float dt)
            {
                auto scale = transform.get_scale();

                scale.x += scale_dir * dt;
                scale.y += scale_dir * dt;

                if (scale.x > 3.0f) {
                    scale.x = 3.0f;
                    scale.y = 3.0f;
                    scale_dir = -scale_dir;
                }
                else if(scale.x < 0.1) {
                    scale.x = 0.1f;
                    scale.y = 0.1f;
                    scale_dir = -scale_dir;
                }

                transform.set_scale(scale);
            }
        };

        bad::RandomNum rnd;

        bad::Sequence<Entity> entities;
        static constexpr int five_hundred_cigarettes = 500;
        entities.reserve(five_hundred_cigarettes);

        auto max_radius = rnd.get_real_distribution(32.f, 64.f);
        auto min_radius = rnd.get_real_distribution(8.f, 32.f);
        auto flares = rnd.get_int_distribution(3, 12);

        auto col = rnd.get_int_distribution(0, 255);

        auto windowpos = rnd.get_real_distribution(-5000.f, 5000.f);
        auto scale = rnd.get_real_distribution(0.1f, 3.f);
        auto rvel = rnd.get_normal_distribution(bad::PI / 2, bad::PI/2);

        for (int i = 0; i < five_hundred_cigarettes; ++i)
        {
            Entity ent;
            
            ent.base_model = bad::make_poly(max_radius(rnd), min_radius(rnd), flares(rnd));
            ent.col = bad::Color(col(rnd), col(rnd), col(rnd), 255);
            float ss = scale(rnd);
            ent.transform = bad::Transform(bad::Point{ windowpos(rnd), windowpos(rnd) }, bad::float2{ss,ss}, 0.f);
            ent.rotational_velocity = rvel(rnd);

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
                camera.read_input(dt, EVENT);
            }
           
            for (auto& e : entities)
            {
                const auto& radian = e.transform.get_radian();
                e.transform.set_radian(radian + e.rotational_velocity * dt);
                e.pulse(dt);
            }

            auto cam_matrix = camera.to_matrix_with_screen_offset(window.get());
            for (auto& e : entities)
            {
                bad::draw_closed_model_transformed(renderer.get(), e.base_model, cam_matrix * e.transform.to_matrix(), e.col);
            }

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

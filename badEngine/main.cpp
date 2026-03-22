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
        struct Plank
        {
            bad::Transform trans{ {0,0},1,0 };
            bad::Point dynamic{ 300,0 };
        }plank;

        struct Ball
        {
            Ball(bad::Transform&& t) :transform(std::move(t)) {}
            bad::Sequence<bad::Point> ball_model;
            bad::Transform transform;
            bad::Point velocity;
            float radius = 32;
        };

        class BallSpawner
        {
        public:

            BallSpawner()
            {
            }

            auto& get_balls() {
                return mBalls;
            }

            void update(float dt)
            {
                time += dt;
                if (time >= period)
                {
                    time -= period;


                    Ball ent(bad::Transform( bad::Point{100,-200 }, 1, 0));
                    ent.ball_model = bad::make_poly(16, 16, 8);
                    ent.radius = 16;
                    ent.velocity = { rng.get(-50.f,50.f), 100 };

                    mBalls.push_back(std::move(ent));
                }
            }

        private:
            bad::RandomNum rng;

            bad::Sequence<Ball> mBalls;

            bad::Point spawn_source{ 200,400 };
            float period = 0.5f;
            float time = 0;
            float vy = -5;
        };

        BallSpawner spawner;

        bad::Transform camt(bad::Point{ 0,0 }, 1, 0);
        bad::MouseCameraController camera{ camt };

        bad::AsyncLogger logger;

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

                case SDL_EVENT_KEY_DOWN:

                    if (EVENT.key.key == SDLK_S)
                        plank.dynamic.y += 3.f;

                    if (EVENT.key.key == SDLK_W)
                        plank.dynamic.y -= 3.f;

                    if (EVENT.key.key == SDLK_A)
                        plank.dynamic.x -= 3.f;

                    if (EVENT.key.key == SDLK_D)
                        plank.dynamic.x += 3.f;

                break;

                default:
                    break;
                }
                camera.read_input(dt, EVENT);
            }

            spawner.update(dt);

            for (auto& ball : spawner.get_balls()) {
                ball.transform.mPos += (ball.velocity * dt);
            }
            bad::Ray plank_ray(plank.trans.mPos, plank.dynamic - plank.trans.mPos);

            for (auto& ball : spawner.get_balls()) {
                bad::reflection_routine_resolved(plank_ray, ball.transform.mPos, ball.velocity, ball.radius);
            }


            camera.mCamera.update_sincos();// manual labor but it's fine
            auto camera_transform = camera.get_view_matrix();

            for (auto& ball : spawner.get_balls())
            {
                ball.transform.mPos += (ball.velocity*dt);

                auto ball_transform = camera_transform * ball.transform.TRS_matrix();
                bad::draw_closed_model_transformed(renderer.get(), ball.ball_model, ball_transform, bad::Colors::Cyan);
            }

            auto plank_transform = camera_transform * plank.trans.TRS_matrix();
            bad::draw_line_transformed(renderer.get(), plank.trans.mPos, plank.dynamic, plank_transform, bad::Colors::Magenta);



            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

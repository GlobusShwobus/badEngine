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
        bad::RandomNum rng;

        auto dist = rng.get_normal_distribution(16, 2);

        auto ye = dist(rng);

        struct Plank
        {
            bad::Position pos{0,200};
            bad::Point dynamic{ 450,200 };
        }plank;

        struct Ball
        {
            Ball(bad::Position&& p) :pos(std::move(p)) {}
            bad::Sequence<bad::Point> ball_model;
            bad::Position pos;
            bad::Point velocity;
            float radius = 32;

            double age = 0.0f;
        };

        class BallSpawner
        {
        public:

            BallSpawner()
            {
                xDist = rng.get_real_distribution(-50.f, 50.f);
                velDist = rng.get_normal_distribution(-111, 22);
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

                    Ball ent(bad::Position(250,500));
                    ent.ball_model = bad::make_poly(16, 16, 8);
                    ent.radius = 16;
                    ent.velocity = { xDist(rng), velDist(rng)};

                    mBalls.push_back(std::move(ent));
                }
            }

            void cleanse() {
                mBalls.erase(std::remove_if(mBalls.begin(), mBalls.end(), [](Ball& b) {return b.age >= 7.f; }),mBalls.end());
            }
            
            std::size_t size()const noexcept
            {
                return mBalls.size();
            }

        private:
            bad::RandomNum rng;
            bad::Sequence<Ball> mBalls;

            std::uniform_real_distribution<float> xDist;
            std::normal_distribution<float> velDist;

            bad::Point spawn_source{ 200,400 };
            float period = 0.5f;
            float time = 0;
            float vy = -5;
        }spawner;


        bad::AsyncLogger logger;



        bad::Sequence<bad::Point> star = bad::make_poly(64, 32, 5);

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
                    
                    if (EVENT.key.key == SDLK_S) {
                        plank.dynamic.y += 10.f;
                    }

                    if (EVENT.key.key == SDLK_W) {
                        plank.dynamic.y -= 10.f;
                    }

                    break;

                default:
                    break;
                }
            }

            spawner.update(dt);

            auto no_transformation = bad::Mat3::identity();
            for (auto& b : spawner.get_balls())
            {
                b.pos.translate_by(b.velocity* dt);
                b.age += dt;
            }

            bad::LineSegment line(plank.pos.get_pos(), plank.dynamic);

            for (auto& b : spawner.get_balls())
            {
                bad::reflection_routine_resolved(line, b.pos, b.velocity, b.radius);
            }

            for (auto& b : spawner.get_balls())
            {
                bad::draw_closed_model_transformed(renderer.get(), b.ball_model, b.pos.to_matrix(), bad::Colors::Cyan);
            }

            bad::draw_line_transformed(renderer.get(), plank.pos.get_pos(), plank.dynamic, no_transformation, bad::Colors::Red);

            logger.log("balls count: "+std::to_string(spawner.size()));

            spawner.cleanse();

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

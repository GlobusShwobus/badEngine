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


        bad::LineSegment line;

        struct CircleEntity
        {
            bad::Sequence<bad::Point> model;
            bad::Circle circle;
            bad::Vector velocity;
            float age;
        };


        class Spawner
        {
        public:
            Spawner()
            {
                velDistX = rnd.get_real_distribution(-25,25);
                velDistY = rnd.get_normal_distribution(-123, 33);
            }

            void spawn(float dt)
            {
                timer += dt;
                if (timer >= timer_target) {
                    CircleEntity ent;
                    ent.model = bad::make_poly(16,16,8);
                    ent.circle.mCenter = { 250,400 };
                    ent.circle.mRadius = 16;
                    ent.velocity = { velDistX(rnd), velDistY(rnd) };
                    ent.age = 0.0f;

                    circles.push_back(std::move(ent));
                    timer -= timer_target;
                }
            }

            void age(float dt)
            {
                for (auto& c : circles)
                {
                    c.age += dt;
                }
            }

            void move(float dt)
            {
                for (auto& c : circles)
                {
                    c.circle.mCenter += c.velocity * dt;
                }
            }

            void die()
            {
                circles.erase(std::remove_if(circles.begin(), circles.end(), [](const CircleEntity& c) {return c.age >= 7.0f; }), circles.end());
            }

            auto& get() { return circles; }

        private:
            bad::RandomNum rnd;
            
            std::uniform_real_distribution<float> velDistX;
            std::normal_distribution<float> velDistY;


            bad::Sequence<CircleEntity> circles;

            const float timer_target = 0.66f;
            float timer = 0.f;
        }spawner;


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

                    if (EVENT.key.key == SDLK_W) {
                        line.p0.y -= 10;
                    }
                    if (EVENT.key.key == SDLK_S) {
                        line.p0.y += 10;
                    }
                    if (EVENT.key.key == SDLK_A) {
                        line.p0.x -= 10;
                    }
                    if (EVENT.key.key == SDLK_D) {
                        line.p0.x += 10;
                    }

                    break;

                default:
                    break;
                }
            }
            SDL_GetMouseState(&line.p1.x, &line.p1.y);

            spawner.age(dt);
            spawner.spawn(dt);
            spawner.die();
            spawner.move(dt);

            for (auto& c : spawner.get())
            {
                auto result = bad::collision::intersect(line, c.circle);
                bad::collision::intersection_reflection_resolve(result, c.circle.mCenter, c.velocity);
            }

            auto no_transform = bad::Mat3::identity();
            auto* pr = renderer.get();

            for (const auto& c : spawner.get()) 
            {
                bad::draw_closed_model_transformed(pr, c.model, bad::Mat3::translate(c.circle.mCenter), bad::Colors::Cyan);
            }

            draw_line_transformed(pr, line.p0, line.p1, no_transform, bad::Colors::Red);


            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

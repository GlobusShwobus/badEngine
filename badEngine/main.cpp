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
#include "FreeDraw.h"
#include "AsyncLogger.h"
#include "Animatable.h"
#include "Font.h"

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

        Window window = make_window(windowContextData.heading, windowContextData.width, windowContextData.height, windowContextData.flags);
        Renderer renderer = make_renderer(window.get(), {});



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
        bool update_camera_sincos = false;
        badCore::AsyncLogger logger;



        badWindow::Texture player = badWindow::make_texture(renderer.get(), "C:/Users/ADMIN/Desktop/badEngine/Textures/player.png");
        badWindow::AnimationPlayer animation(player.get());
        const auto& s = animation.get_sprite();

        badWindow::Clip clip1 = badWindow::make_clip(s.get_width(), s.get_height(), SDL_FRect{0,0,32,32}, 8, 0.08f, true);
        animation.add_clip(badWindow::AnimID::Left, std::move(clip1));
        animation.play(badWindow::AnimID::Left);



        badWindow::Texture fonttexture = badWindow::make_texture(renderer.get(), "C:/Users/ADMIN/Desktop/badEngine/Fonts/font_32x3.png");
        badWindow::Font font(fonttexture.get(), 32,3);

        font.set_text("i memed trump into office.\nno need to thank me");

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

                case SDL_EVENT_KEY_DOWN:

                    if (EVENT.key.key == SDLK_Q) {
                        update_camera_sincos = true;
                        font.set_scale(font.get_scale() * 1.1f);
                    }

                    if (EVENT.key.key == SDLK_E) {
                        update_camera_sincos = true;
                        font.set_scale(font.get_scale() * 0.9f);
                    }

                    break;

                case SDL_EVENT_KEY_UP:

                    if (EVENT.key.key == SDLK_Q)
                        update_camera_sincos = false;

                    if (EVENT.key.key == SDLK_E)
                        update_camera_sincos = false;

                    break;

                default:break;
                }

                cam.update(dt, EVENT);
            }


            //collect models
            for (auto& e : entities) {
                e.rotate(dt);
                e.transform.update_sincos();
                e.pulse_scale();
            }

            Mat3 window_mat = sdl_window_matrix(window.get());
            if (update_camera_sincos) {
                cam.mCamera.update_sincos();
                logger.log(std::string("sincos update"));
            }
            Mat3 camera_mat = cam.mCamera.transform_inverse();


            for (auto& e : entities) {
                Mat3 world = window_mat * camera_mat * e.get_transform();
                const auto& model = e.mModel;
                draw_closed_model(renderer.get(), model.data(), model.size(), world, e.col);
            }


            float mx, my;
            SDL_GetMouseState(&mx, &my);
            SDL_FRect dest{ mx,my,96,96 };
            
            animation.update(dt);
            animation.draw(renderer.get(), dest);
            font.set_position(mx,my);
            font.draw(renderer.get());

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());

            //logger.log(std::string("FPS ") + std::to_string(1 / dt));
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

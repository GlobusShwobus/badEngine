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
#include "MakeShape.h"
#include "MouseCameraController.h"
#include "SDL_SYSTEM_RAII.h"
#include "Window.h"
#include "Renderer.h"
#include "EngineUtils.h"
#include "FreeDraw.h"
#include "AsyncLogger.h"
#include "Animatable.h"
#include "Font.h"
#include "badHTTP.h"

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

        badHTTP::MultiCurl cum;

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

            
                default:
                    break;
                }
            }

            SDL_SetRenderTarget(renderer.get(), nullptr);//reminder
            SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);//reset to black ONCE before the end
            SDL_RenderPresent(renderer.get());

            //logger.log(std::string("FPS ") + std::to_string(1 / dt));
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

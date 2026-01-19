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

#include "NumberGenerator.h"

#include "Color.h"
#include "Camera.h"
#include "Ray.h"
#include "TextureMap.h"

#include <iostream>

int main() {

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
                if (EVENT.type == SDL_EVENT_QUIT) {
                    GAME_RUNNING = false;
                    continue;
                }
            }

            std::cout << "hello world\n";
            window.system_present();
        }
    }
    _CrtDumpMemoryLeaks();
    return 0;
}

#include "badEngine.h"

namespace badEngine
{
    void badEngine::process_input(float dt)
    {
        SDL_Event EVENT;

        while (SDL_PollEvent(&EVENT)) {

            mCam.update(dt, EVENT);
        }
    }
}
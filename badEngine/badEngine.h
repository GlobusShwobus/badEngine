#pragma once

#include "load_data.h"
#include "WindowContext.h"
#include "RandomNum.h"
#include "Stopwatch.h"
#include "MouseCameraController.h"

namespace badEngine {

	using namespace badCore;
	using namespace badWindow;

	class badEngine
	{
	public:

		badEngine(const WindowContextDescription& window)
			:window(window.heading.c_str(), window.width, window.height, window.flags), is_active(true)
		{

		}

		void run() {

		}
	
	private:

		void process_input(float dt);      //event reading
		void advance_state();      //updates states, moves, generally unconditional
		void resolve_state();      //handles the results of state update, like collision
		void render();             //drawing logic
		void prepare_next_frame(); //cleanup, support

	private:
		//meta
		bool is_active = false;
		//current min
		WindowContext window;
		RandomNum rng;
		Stopwatch frametimer;


		//
		MouseCameraController mCam;
	};

}
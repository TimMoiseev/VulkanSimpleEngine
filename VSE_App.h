#pragma once


#include "VSE_Window.h"
#include "VSE_Device.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;



namespace vse {
	class VseApp {
	public:
		uint32_t WIDTH = 800;
		uint32_t HEIGHT = 600;
		void run();
		VseApp();
		~VseApp();
	private:
		VseWindow vseWindow;
		VseDevice vseDevice;
	};
}
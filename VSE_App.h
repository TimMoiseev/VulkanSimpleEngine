#pragma once
#include "VSE_Window.h"
#include "VSE_Device.h"
#include "VSE_SwapChain.h"
#include "VSE_Pipeline.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

namespace vse {
	class VseApp {
	public:
		void run();
		VseApp();
		~VseApp();
	private:
		VseWindow vseWindow;
		VseDevice vseDevice{ vseWindow };
		VseSwapChain vseSwapChain{vseDevice};
		VsePipeline vsePipeline{vseDevice.device, vseSwapChain.swapChainExtent};

	};
}
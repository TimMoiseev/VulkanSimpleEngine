#include "VSE_App.h"
using namespace vse;

int main() {
	VseApp app;
	try {
		app.run();
		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}
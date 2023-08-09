#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cube/cube_api.h>

int main(int argc, char* argv[]) {
	int appid;
	if (argc == 2 || argc == 3) {
		appid = atoi(argv[1]);
	} else {
		return 0;
	}
	if (Cube::IsNeedRestartApp(appid)) {
		return 0;
	}
	if (!Cube::CubeInitialize()) {
		return 0;
	}
	std::cout << "Game " << std::setw(8) << appid << " running...";
}
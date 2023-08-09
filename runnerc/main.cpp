#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cube/cube_api.h>

int main(int argc, char* argv[]) {
	using namespace std;
	int appid;
	char* name;
	if (argc == 2 || argc == 3) {
		appid = atoi(argv[1]);
		name = argv[2];
	} else {
		return 0;
	}
	if (Cube::IsNeedRestartApp(appid)) {
		return 0;
	}
	if (!Cube::CubeInitialize()) {
		return 0;
	}
	cout << "Game " << std::setw(8) << appid << ": " << name << " running...";
}
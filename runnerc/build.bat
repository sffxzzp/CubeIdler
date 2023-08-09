g++ main.cpp -I include lib/cube_api64.lib -mwindows -s -o runner
upx --brute runner.exe
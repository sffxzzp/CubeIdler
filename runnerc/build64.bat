g++ main.cpp -I include lib/cube_api64.lib -static -static-libgcc -s -O2 -o runner
upx --brute *.exe
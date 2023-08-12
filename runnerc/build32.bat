g++ main.cpp -I include lib/cube_api.lib -static -static-libgcc -s -m32 -O2 -o runner
upx --brute *.exe
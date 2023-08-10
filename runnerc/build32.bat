g++ main.cpp -I include lib/cube_api.lib -s -m32 -o runner
upx -9 -vf --lzma --compress-icons=0 runner.exe
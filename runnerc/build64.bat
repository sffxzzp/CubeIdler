g++ main.cpp -I include lib/cube_api64.lib -s -o runner
upx -9 -vf --lzma --compress-icons=0 runner.exe
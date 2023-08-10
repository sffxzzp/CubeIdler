@echo off
set GOARCH=386
del /f /s /q .\*.upx
del /f /q .\dist\*.exe
cd runnerc
g++ main.cpp -I include lib/cube_api.lib -s -m32 -o runner
move *.exe ..\dist
cd ..
go build -ldflags="-s -w"
move *.exe .\dist
cd dist
upx -9 -vf --lzma --compress-icons=0 *.exe
cd ..
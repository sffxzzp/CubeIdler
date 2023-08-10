@echo off
del /f /s /q .\*.upx
del /f /q .\dist\*.exe
cd runnerc
g++ main.cpp -I include lib/cube_api64.lib -s -o runner
move *.exe ..\dist
cd ..
go build -ldflags="-s -w"
move *.exe .\dist
cd dist
upx --brute *.exe
cd ..
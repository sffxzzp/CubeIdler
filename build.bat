@echo off
del /f /s /q .\*.upx
del /f /q .\dist\cubeidler.exe
del /f /q .\dist\cube.exe
cd runner
go build -ldflags="-s -w"
upx -9 -vf --lzma --compress-icons=0 cube.exe
move cube.exe ..\dist
cd ..
go build -ldflags="-s -w"
upx -9 -vf --lzma --compress-icons=0 cubeidler.exe
move cubeidler.exe .\dist
@echo off
del /f /s /q .\*.upx
del /f /q .\dist\*.exe
cd runnerc
call build64.bat
move *.exe ..\dist
cd ..
go build -ldflags="-s -w"
upx -9 -vf --lzma --compress-icons=0 *.exe
move *.exe .\dist
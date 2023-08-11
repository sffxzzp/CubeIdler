@echo off
set GOARCH=386
go build -tags x86 -ldflags="-s -w"
upx -9 -vf --lzma --compress-icons=0 *.exe
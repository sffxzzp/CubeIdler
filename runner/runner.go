package main

import (
	"fmt"
	"os"
	"strconv"
	"syscall"
	"unsafe"
)

type (
	cube struct {
		DLL *syscall.LazyDLL
	}
)

func getArgsData() (int, string) {
	if len(os.Args) != 3 {
		fmt.Println("Usage: CubeIdler <appid> <name>")
		os.Exit(1)
	}
	appid, _ := strconv.Atoi(string(os.Args[1]))
	name := os.Args[2]
	return appid, name
}

func newCube() *cube {
	return &cube{
		DLL: syscall.NewLazyDLL("cube_api64.dll"),
	}
}

func (c *cube) IsNeedRestartApp(appid uintptr) bool {
	fn := c.DLL.NewProc("IsNeedRestartApp")
	ptr, _, _ := fn.Call(appid)
	return *(*bool)(unsafe.Pointer(ptr))
}

func (c *cube) CubeInitialize() {
	fn := c.DLL.NewProc("CubeInitialize")
	fn.Call()
}

// func (c *cube) CubeShutdown() {
// 	fn := c.DLL.NewProc("CubeShutdown")
// 	fn.Call()
// }

func main() {
	appid, name := getArgsData()

	cube := newCube()
	if cube.IsNeedRestartApp(uintptr(appid)) {
		return
	}
	cube.CubeInitialize()

	fmt.Printf("Game %08d: %s running...", appid, name)
}

package main

import (
	"fmt"
	"time"
)

func start() {
	cube := newCube()
	if !cube.settings() {
		return
	}
	if !cube.login() {
		return
	}
	skip := false
	if !cube.getCookie() {
		skip = true
	}
	if !skip {
		cube.openBoxes()
		cube.getPoints()
	}
	if cube.getRandomApps() {
		cube.idle()
		if cube.Fast {
			if skip {
				for i := 0; i < 20; i++ {
					cube.sendAppTime()
					fmt.Printf("Waiting for %d seconds, be patient...\n", cube.Span)
					time.Sleep(time.Duration(cube.Span) * time.Second)
				}
			} else {
				startPoints := int(cube.Points)
				for cube.Points-startPoints < cube.Target {
					cube.sendAppTime()
					fmt.Printf("Waiting for %d seconds, be patient...\n", cube.Span)
					time.Sleep(time.Duration(cube.Span) * time.Second)
					cube.getPoints()
					fmt.Printf("%d => %d | %d\n", startPoints, cube.Points, cube.Points-startPoints)
					if cube.Points-startPoints == 0 {
						// if no points increased, that today may have already idled, quit.
						break
					}
				}
			}
		}
	}
}

func main() {
	start()
	var exit string
	fmt.Println("Press enter to exit...")
	fmt.Scanln(&exit)
}

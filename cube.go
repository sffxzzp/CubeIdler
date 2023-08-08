package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"math/rand"
	"net/http"
	"net/http/cookiejar"
	"net/url"
	"os"
	"os/exec"
	"regexp"
	"strconv"
	"sync"
	"time"
)

type (
	Cube struct {
		ApiUrl     string
		Client     *http.Client
		Account    LoginAccount
		Login      LoginData
		RandomApps []App
		Points     int
		Fast       bool
		Span       int
	}
	Config struct {
		Fast bool   `json:"fast"`
		Span int    `json:"timespan"`
		Key  string `json:"key,omitempty"`
	}
	Settings struct {
		Account SettingsAccount `json:"account"`
	}
	SettingsAccount struct {
		Username     string `json:"userName"`
		Password     string `json:"passWord"`
		OTP          string `json:"otp"`
		IsRememberMe bool   `json:"isRemeberMe"`
		IsAutoLogin  bool   `json:"isAutoLogin"`
	}
	LoginAccount struct {
		Username     string `json:"username"`
		Password     string `json:"password"`
		OTP          string `json:"otp"`
		IsRememberMe bool   `json:"isRememberMe"`
		IsAutoLogin  bool   `json:"isAutoLogin"`
	}
	Login struct {
		ResultCode int       `json:"resultCode"`
		Data       LoginData `json:"data"`
	}
	LoginData struct {
		ID             string `json:"id"`
		EncodeID       string `json:"encode_id"`
		Token          string `json:"token"`
		Pwd            string `json:"pwd"`
		NickName       string `json:"nickname"`
		CubeID         int    `json:"cubeId"`
		Avatar         string `json:"avatar"`
		Sex            int    `json:"sex"`
		IsDeveloper    int    `json:"isDeveloper"`
		VipState       int    `json:"vipState"`
		VipExpiresTime int    `json:"vipExpirestime"`
		DesUserID      string `json:"desUserId"`
	}
	Case struct {
		StatusCode int `json:"statusCode"`
		Data       struct {
			ID     int    `json:"goodid"`
			Type   int    `json:"goodtype"`
			Name   string `json:"goodname"`
			Value  string `json:"goodvalue"`
			CMoney int    `json:"c_Money"` // don't know what this is for
		} `json:"data"`
	}
	App struct {
		ID   int
		Name string
	}
	Apps map[string]struct {
		ID        int    `json:"id"`
		Name      string `json:"name"`
		IsWebGame bool   `json:"isWebGame"`
	}
	AppTime struct {
		AppID     int    `json:"appId,omitempty"`
		T         int    `json:"t,omitempty"`
		SessionID int    `json:"sessionId,omitempty"`
		Status    string `json:"status,omitempty"`
	}
)

//lint:ignore U1000 Ignore unused function temporarily for debugging
func (c *Cube) httpGet(url string) []byte {
	retry := 3
	for retry > 0 {
		res, err := c.Client.Get(url)
		if err != nil {
			retry--
			continue
		}
		defer res.Body.Close()
		rbytes, err := io.ReadAll(res.Body)
		if err != nil {
			retry--
			continue
		}
		return rbytes
	}
	return []byte{}
}

//lint:ignore U1000 Ignore unused function temporarily for debugging
func (c *Cube) httpPost(url string, contentType string, body io.Reader) []byte {
	retry := 3
	for retry > 0 {
		res, err := c.Client.Post(url, contentType, body)
		if err != nil {
			retry--
			continue
		}
		defer res.Body.Close()
		rbytes, err := io.ReadAll(res.Body)
		if err != nil {
			retry--
			continue
		}
		return rbytes
	}
	return []byte{}
}

func (c *Cube) settings() bool {
	fmt.Println("Get settings...")
	res := c.httpGet(c.ApiUrl + "/settings")
	var data Settings
	json.Unmarshal(res, &data)
	if data.Account.Username == "" {
		fmt.Printf("Cube is not running...\n\n")
		return false
	} else {
		data.Account.Username, _ = url.QueryUnescape(data.Account.Username)
		fmt.Printf("Username: %s\n\n", data.Account.Username)
		c.Account = LoginAccount{
			Username:     data.Account.Username,
			Password:     data.Account.Password,
			OTP:          data.Account.OTP,
			IsRememberMe: data.Account.IsRememberMe,
			IsAutoLogin:  data.Account.IsAutoLogin,
		}
		return true
	}
}

func (c *Cube) login() bool {
	fmt.Printf("Get login data...")
	postData, _ := json.Marshal(c.Account)
	res := c.httpPost(c.ApiUrl+"/login", "application/json;charset=UTF-8", bytes.NewBuffer(postData))
	var data Login
	json.Unmarshal(res, &data)
	if data.ResultCode == 1 {
		fmt.Printf("\nID: %d\tNickName:%s\n\n", data.Data.CubeID, data.Data.NickName)
		c.Login = data.Data
		return true
	} else {
		fmt.Println("failed!")
		return false
	}
}

// some apps have no licenses but exists in library. what the fuck?
func (c *Cube) getRandomApps() bool {
	fmt.Println("Get random apps...")
	res := c.httpGet(c.ApiUrl + "/apps")
	var data Apps
	json.Unmarshal(res, &data)
	var allapps []App
	for _, app := range data {
		// exclude 摇摆地牢, which is unlicensed and still in library.
		if !app.IsWebGame && app.ID != 10000412 {
			appName, _ := url.QueryUnescape(app.Name)
			allapps = append(allapps, App{
				ID:   app.ID,
				Name: appName,
			})
		}
	}
	var apps []App
	appnums := len(allapps)
	startnums := 20
	if appnums <= 20 {
		startnums = appnums
	}
	if appnums == 0 {
		fmt.Printf("failed!\n\n")
		return false
	} else {
		var sel []int
		for len(sel) < startnums {
			num := rand.Intn(appnums)
			exists := false
			for _, v := range sel {
				if v == num {
					exists = true
					break
				}
			}
			if !exists {
				sel = append(sel, num)
				apps = append(apps, allapps[num])
			}
		}
		for _, app := range apps {
			fmt.Printf("%08d %s\n", app.ID, app.Name)
		}
		fmt.Printf("\n\n")
		c.RandomApps = apps
		return true
	}
}

func (c *Cube) getCookie() bool {
	fmt.Printf("Login...")
	cookieUrl := fmt.Sprintf("https://account.cubejoy.com/index/LoginTransfers?name=%s&pwd=%s&redirectUrl=https://store.cubejoy.com/html/en/store/boxindex/wideindex.html&token=%s&logininit=true", c.Login.ID, c.Login.Pwd, c.Login.Token)
	res := c.httpGet(cookieUrl)
	// if login failed, it'll return "登录失败，用户名密码或错误", and appearently not an html page
	if len(res) < 100 {
		fmt.Printf("failed!\n\n")
		return false
	} else {
		fmt.Printf("success!\n\n")
		return true
	}
}

func (c *Cube) openBoxes() bool {
	fmt.Println("Open cases...")
	// other cases, but daily checkin won't get any.
	// https://me.cubejoy.com/Case/CopperCase
	// https://me.cubejoy.com/Case/SilverCase
	// https://me.cubejoy.com/Case/GoldCase
	c.httpGet("https://me.cubejoy.com/case/indexbox")
	res := c.httpGet("https://me.cubejoy.com/Case/WoodCase")
	var data Case
	json.Unmarshal(res, &data)
	if data.StatusCode == 200 {
		fmt.Printf("Case opened, get %s\n\n", data.Data.Name)
		return true
	} else {
		fmt.Printf("Case already opened or no cases available\n\n")
		return false
	}
}

func (c *Cube) getPoints() {
	fmt.Printf("Get current points...")
	if c.Login.DesUserID == "" {
		res := c.httpGet("https://account.cubejoy.com/BUsers/UserInfo")
		regex := regexp.MustCompile(`id="desuserid" ?value="([A-Za-z0-9]*?)"`)
		match := regex.FindStringSubmatch(string(res))
		if len(match) != 2 {
			return
		}
		c.Login.DesUserID = match[1]
	}
	res := c.httpGet("https://me.cubejoy.com/api/PersionalCenter/UserPointJsonp?duserid=" + c.Login.DesUserID)
	res = res[1 : len(res)-1]
	points, _ := strconv.Atoi(string(res))
	fmt.Printf("%d\n\n", points)
	c.Points = points
}

func (c *Cube) idle() {
	fmt.Println("Start idling...")
	var wg sync.WaitGroup
	for _, app := range c.RandomApps {
		wg.Add(1)
		go func(app App) {
			defer wg.Done()
			fmt.Printf("Loading %08d:%s...\n", app.ID, app.Name)
			output, err := exec.Command("./runner.exe", strconv.Itoa(app.ID), app.Name).Output()
			if err != nil {
				fmt.Println(err)
			}
			fmt.Println(string(output))
		}(app)
	}
	wg.Wait()
	fmt.Println()
}

func (c *Cube) sendAppTime() {
	fmt.Printf("Sending app time... Current time: %s\n", time.Now().Format("15:04:05"))
	var wg sync.WaitGroup
	for _, app := range c.RandomApps {
		wg.Add(1)
		// postData, _ := json.Marshal(&AppTime{AppID: appid})
		// res := c.httpPost(c.ApiUrl+"/getLauncheAppTimeSessionId", "application/json;charset=UTF-8", bytes.NewBuffer(postData))
		// var data AppTime
		// json.Unmarshal(res, &data)
		go func(app App) {
			defer wg.Done()
			client := &http.Client{
				Timeout: time.Second,
			}
			postData, _ := json.Marshal(&AppTime{
				AppID:     app.ID,
				T:         5,
				SessionID: 9999999, // seems to be the same
				Status:    "running",
			})
			client.Post(c.ApiUrl+"/sendLauncheAppTime", "application/json;charset=UTF-8", bytes.NewBuffer(postData))
		}(app)
	}
	wg.Wait()
}

func (c *Cube) getTargetPoints() int {
	var target string
	// seems the limit is lower from 303 to 153, so i set the default to 150.
	tPoints := 150
	fmt.Printf("Target points: ")
	fmt.Scanln(&target)
	res, err := strconv.Atoi(target)
	if err == nil {
		tPoints = res
	}
	return tPoints
}

func newCube() *Cube {
	cCont, err := os.ReadFile("config.json")
	if err != nil {
		cCont = []byte("{\"fast\": false, \"timespan\": 300}")
	}
	var config Config
	json.Unmarshal(cCont, &config)
	if config.Span == 0 {
		config.Span = 300
	}
	// needs a key to limit other users
	if config.Key != "a5d93554-0bef-4538-9c46-73832a07b29e" {
		config.Span = 300
	}
	jar, _ := cookiejar.New(nil)
	return &Cube{
		ApiUrl: "http://127.0.0.1:5222",
		Client: &http.Client{
			Jar: jar,
		},
		Fast: config.Fast,
		Span: config.Span,
	}
}
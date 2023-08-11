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
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/saintfish/chardet"
	"golang.org/x/text/encoding/simplifiedchinese"
)

type (
	Cube struct {
		ApiUrl     string
		Client     *http.Client
		Account    LoginAccount
		Login      LoginData
		RandomApps []App
		C          string
		Points     int
		Fast       bool
		Span       int
		Num        int
	}
	Config struct {
		Fast bool   `json:"fast,omitempty"`
		Span int    `json:"timespan,omitempty"`
		Num  int    `json:"num,omitempty"`
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
	PointsPostData struct {
		C        string `json:"c"`
		Page     int    `json:"page"`
		Size     int    `json:"size"`
		DataFrom int    `json:"datafrom"`
		Key      int    `json:"key"`
		Sign     string `json:"sign"`
	}
	PointsResData struct {
		TotalCount int             `json:"totalcount"`
		List       []PointsResItem `json:"list"`
	}
	PointsResItem struct {
		ID      int    `json:"id"`
		Point   int    `json:"point"`
		IsAdd   int    `json:"isadd"`
		UplID   int    `json:"uplid"`
		Remarks string `json:"remarks"`
		Name    string `json:"name"`
		AddTime string `json:"addtime"`
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
	if appnums <= c.Num {
		c.Num = appnums
	}
	if appnums == 0 {
		fmt.Printf("failed!\n\n")
		return false
	} else {
		var sel []int
		for len(sel) < c.Num {
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
	pointsUrl, _ := url.Parse("https://trade.cubejoy.com/GoldCube/PointProduceList")
	cookies := c.Client.Jar.Cookies(pointsUrl)
	var values []string
	for _, cookie := range cookies {
		if cookie.Name == "AllCookie" {
			values = strings.Split(cookie.Value, "|")
			break
		}
	}
	c.C, _ = url.QueryUnescape(values[1])
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
	fmt.Printf("Get today points...")
	postData := PointsPostData{
		C:        c.C,
		Page:     1,
		Size:     10,
		DataFrom: 1,
		Key:      10001,
		Sign:     "",
	}
	jsonPostData, _ := json.Marshal(postData)
	res := c.httpPost("https://trade.cubejoy.com/GoldCube/PointProduceList", "application/json;charset=UTF-8", bytes.NewBuffer(jsonPostData))
	var data PointsResData
	json.Unmarshal(res, &data)
	c.Points = 0
	for _, item := range data.List {
		addtime, _ := time.ParseInLocation("2006-01-02 15:04:05", item.AddTime, time.Local)
		now := time.Now().Local()
		if item.UplID == 2 && (addtime.Year() == now.Year() && addtime.Month() == now.Month() && addtime.Day() == now.Day()) {
			c.Points = item.Point
		}
	}
	fmt.Printf("%d\n\n", c.Points)
}

func (c *Cube) idle() {
	fmt.Println("Start idling...")
	var wg sync.WaitGroup
	for _, app := range c.RandomApps {
		wg.Add(1)
		go func(app App) {
			defer wg.Done()
			fmt.Printf("Loading %08d:%s...\n", app.ID, app.Name)
			res, err := exec.Command("./runner.exe", strconv.Itoa(app.ID), app.Name).Output()
			if err != nil {
				fmt.Println(err)
			}
			var output []byte
			det := chardet.NewTextDetector()
			enc, _ := det.DetectBest(res)
			switch enc.Charset {
			case "UTF-8":
				output = res
			default:
				output, _ = simplifiedchinese.GB18030.NewDecoder().Bytes(res)
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

func newCube() *Cube {
	cCont, err := os.ReadFile("config.json")
	if err != nil {
		cCont = []byte("{\"fast\": false, \"timespan\": 300, \"num\": 5}")
	}
	var config Config
	json.Unmarshal(cCont, &config)
	if config.Span == 0 {
		config.Span = 300
	}
	if config.Num < 1 || config.Num > 20 {
		config.Num = 5
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
		Num:  config.Num,
	}
}

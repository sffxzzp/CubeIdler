package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"net/http/cookiejar"
	"net/url"
	"os/exec"
	"regexp"
	"strconv"
	"sync"
)

type (
	Cube struct {
		ApiUrl     string
		Client     *http.Client
		Account    SettingsAccount
		Login      LoginData
		RecentApps []int
		Points     int
	}
	Settings struct {
		Account SettingsAccount `json:"account"`
	}
	SettingsAccount struct {
		IsAutoLogin  bool   `json:"isAutoLogin"`
		IsRememberMe bool   `json:"isRememberMe"`
		Password     string `json:"passWord"`
		Username     string `json:"userName"`
		OTP          string `json:"otp"`
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
	Apps struct {
		Data struct {
			RecentApps []struct {
				ID int `json:"id"`
			} `json:"recentApps"`
		} `json:"data"`
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

//lint:ignore U1000 Ignore unused function temporarily for debugging
func (c *Cube) httpPostForm(url string, data url.Values) []byte {
	retry := 3
	for retry > 0 {
		res, err := c.Client.PostForm(url, data)
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
		fmt.Printf("Username: %s\n\n", data.Account.Username)
		c.Account = data.Account
		return true
	}
}

func (c *Cube) login() bool {
	fmt.Printf("Get login data...")
	postData, _ := json.Marshal(c.Account)
	res := c.httpPost(c.ApiUrl+"/login", "application/json; charset=UTF-8", bytes.NewBuffer(postData))
	var data Login
	json.Unmarshal(res, &data)
	if data.ResultCode == 11 {
		fmt.Printf("\nID: %d\tNick Name:%s\n\n", data.Data.CubeID, data.Data.NickName)
		c.Login = data.Data
		return true
	} else {
		fmt.Println("failed!")
		return false
	}
}

func (c *Cube) getRecentApps() bool {
	fmt.Printf("Get recent apps...")
	res := c.httpPost(c.ApiUrl+"/runapp", "application/json; charset=UTF-8", bytes.NewBuffer([]byte("{\"id\":0,\"sync\":0,\"multiRunIndex\":-1}")))
	var data Apps
	json.Unmarshal(res, &data)
	var appids []int
	for _, app := range data.Data.RecentApps {
		appids = append(appids, app.ID)
	}
	if len(appids) == 0 {
		fmt.Printf("failed!\n\n")
		return false
	} else {
		for _, appid := range appids {
			fmt.Printf("%d ", appid)
		}
		fmt.Printf("\n\n")
		c.RecentApps = appids
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
	res := c.httpGet("https://account.cubejoy.com/BUsers/UserInfo")
	regex := regexp.MustCompile(`id="desuserid" ?value="([A-Za-z0-9]*?)"`)
	match := regex.FindStringSubmatch(string(res))
	if len(match) != 2 {
		return
	}
	desuserid := match[1]
	res = c.httpGet("https://me.cubejoy.com/api/PersionalCenter/UserPointJsonp?duserid=" + desuserid)
	res = res[1 : len(res)-1]
	points, _ := strconv.Atoi(string(res))
	fmt.Printf("%d\n\n", points)
	c.Points = points
}

func (c *Cube) idle() {
	fmt.Println("Start idling...")
	var wg sync.WaitGroup
	for _, appid := range c.RecentApps {
		wg.Add(1)
		go func(appid string) {
			defer wg.Done()
			fmt.Printf("Loading %s...\n", appid)
			output, err := exec.Command("./cube.exe", appid).Output()
			if err != nil {
				fmt.Println(err)
			}
			fmt.Println(string(output))
		}(strconv.Itoa(appid))
	}
	wg.Wait()
}

func newCube() *Cube {
	jar, _ := cookiejar.New(nil)
	return &Cube{
		ApiUrl: "http://127.0.0.1:5222",
		Client: &http.Client{
			Jar: jar,
		},
	}
}

func main() {
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
	if cube.getRecentApps() {
		cube.idle()
	}
	var exit string
	fmt.Println("Press enter to exit...")
	fmt.Scanln(&exit)
}

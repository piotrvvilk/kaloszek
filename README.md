<H1>KALOSZEK - AccuWeather EPD frame</H1> 

<H2> General information</H2>
Every 30min Kaloszek connects to AccuWeather server. Because AccuWeather trial version allow up to 50 calls per day, device uses 4 separate keys to make sure weather will be downloaded in all 24H. Information about forcast and current weather are downloaded separetly. After completing all data, device display it on screen and going to sleep.

<div align="center"><img src="img/1.jpg"></div>

<H2> Hardware </H2>


- Waveshare EPD  4.2inch, 400x300,

- NodeMCU v3,

- Li-ion battery (900mAh),

- Li-ion charger ST4054,

- Li-ion monitor MAX17048,


<H2> Connections </H2>

<H3> </H3>

<H4>EPD : </H4>

- D2 – BUSY

- D3 – CS

- D4 – RST

- D5 – CLK

- D7 – DIN

- D8 – DC (plus external pullup 3k3)

<H3> </H3>

<H4>  MAX17048:</H4>

- D1 – SDA

- D6 – SCL

<H3> </H3>

<H4>Wake up:</H4>

<H3> </H3>

- D0 – RST (remove while programming)

<H2> Libraries</H2>
<H3> </H3>
<H4>  WiFi</H4>
http://www.arduino.cc/en/Reference/WiFi
<H4>  EPD display</H4>
http://github.com/ZinggJM/GxEPD2
<H4>  AccuWeather</H4>
http://github.com/jackmax/AccuWeatherLibrary
<H4>  Json parser</H4>
http://github.com/squix78/json-streaming-parser
<H4>  MAX17048</H4>
http://github.com/hideakitai/MAX17048

<H2> Fonts generator</H2>
http://oleddisplay.squix.ch/#/home

<H2> Icons source</H2>
http://developer.accuweather.com/

<H2> Housing</H2>
<H5>Simple, black :). Designed and printed by Marcin:</H5>
http://www.instagram.com/creatorepic/

<H2> Settings</H2>

You have to edit „authentication.h” file and set your own WiFi SSID, password and accuweather keys. 


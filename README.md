<H1>KALOSZEK - AccuWeather EPD frame</H1> 

<H2>1.	How does it work</H2>
Every 30min Kaloszek connects to AccuWeather server. Because AccuWeather trial version allow up to 50 calls per day, device uses 4 separate keys to make sure weather will be downloaded in all 24H. Information about forcast and current weather are downloaded separetly. After completing all data, device display it on screen and going to sleep.

<H2>2.	Hardware </H2>


- Waveshare EPD  4.2inch, 400x300,


- NodeMCU v3,


- Li-ion battery (900mAh),


- Li-ion charger (I used ST4054, because I had this one in my drawer),


- MAX17048,



<H2>3.	Hardware connection </H2>

<H3>MAX 17048:</H3>

D1 – SDA

D6 – SCL

<H3>EPD:</H3>

D2 – BUSY

D3 – CS

D4 – RST

D5 – CLK

D7 – DIN

D8 – DC (external pullup 3k3)

<H3>Wake up:</H3>

D0 – RST (remove while programming)

 
 
<H2>4.	Libraries</H2>
<H3>  WiFi:</H3>
http://www.arduino.cc/en/Reference/WiFi
<H3>  EPD display:</H3>
http://github.com/ZinggJM/GxEPD2
<H3>  AccuWeather:</H3>
http://github.com/jackmax/AccuWeatherLibrary
<H3>  Json parser:</H3>
http://github.com/squix78/json-streaming-parser
<H3>  MAX17048:</H3>
http://github.com/hideakitai/MAX17048

 
 
<H2>5.	Fonts generator</H2>
http://oleddisplay.squix.ch/#/home

 
 
<H2>6.	Icons source</H2>
http://developer.accuweather.com/

 
 
<H2>7.	Case</H2>
Simple, black :). Designed and printed by my friend Marcin:
www.instagram.com/creatorepic/

 
 
<H2>8.	Settings</H2>

You have to edit „authentication.h” file and set your own WiFi SSID, password and accuweather keys. 


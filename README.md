<H1>AccuWeather EPD frame</H1> 

<H2>1.	How does it work</H2>

<H2>2.	Hardware </H2>
- Waveshare EPD  4.2inch, 400x300
- NodeMCU v3
- Li-ion battery (900mAh)
- Li-ion charger (I used ST4054, because I had this one in my drawer)
- MAX17048

<H2>3.	Hardware connection </H2>
MAX 17048:
D1 – SDA
D6 – SCL

EPD:
D2 – BUSY
D3 – CS
D4 – RST
D5 – CLK
D7 – DIN
D8 – DC (external pullup 3k3)

Wake up:
D0 – RST (remove while programming)


# AccuWeather EPD frame

#2.	Hardware
- Waveshare EPD  4.2inch, 400x300
- NodeMCU v3
- Li-ion battery (900mAh)
- Li-ion charger (I used ST4054, because I had this one in my drawer)
- MAX17048

#3.	Connection
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


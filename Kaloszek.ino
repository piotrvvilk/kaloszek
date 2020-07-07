//*********************************************************
//*****   PROJECT NAME: KALOSZEK                      *****
//*****                                               *****
//*****   VER: 2.6                                    *****
//*****                                               *****
//*****   DATE: 2020-07-07                            *****
//*****                                               *****
//*****                                               *****
//*********************************************************

//*********************************************************
//    LIBRARIES:
//    
//    EPD: https://github.com/ZinggJM/GxEPD2
//    ACCUWEATHER: https://github.com/jackmax/AccuWeatherLibrary
//    JSON ACCU: https://github.com/squix78/json-streaming-parser
//    MAX17048: https://github.com/hideakitai/MAX17048
//
//
//*********************************************************

//*********************************************************
#define ESP8266                     1

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <GxEPD2_BW.h>
#include <JsonListener.h>

#include "AccuWeatherLibrary.h"
#include "authentication.h"
#include "MAX17048.h"

//===========================================================================================================================================
#define     USE_EPD                 1
#define     USE_WIFI                1

#define     USE_ACCUWEATHER         1
#define     USE_HOURLY_FORCAST      1

#define     USE_WIFI_ICO            1

#define     USE_UART                1
//#define     PRINT_WEATHER           1

#define     USE_MAX17048            1

#define     UPDATE_PERIOD           1800                    //seconds
#define     WIFI_ERR_PERIOD         300                     //seconds

//=========================================================================================================================================== 
#include "epd_winddir.h"
#include "epd_iconpack.h"
#include "epd_main_iconpack.h" 
#include "epd_info_iconpack.h"

#include "data/fonts/RobotoMedPlain17.h"
#include "data/fonts/RobotoMedPlain18.h"
#include "data/fonts/RobotoMedPlain25_mod.h"
#include "data/fonts/RobotoBlackPlain25_mod.h"
#include "data/fonts/FreeSansBold52_mod.h"

//===========================================================================================================================================
GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=D3 ss*/ 0 , /*DC=D8*/ 15, /*RST=D4*/ 2, /*BUSY=D2*/ 4));    

const char str_sleep[] = "I'm going to sleep for ";
const char str_seconds[] = "seconds";
const char str_accuweather_err[] = "AccuWeather error";
const char str_wifi_err[] = "Error :(";
const char str_db[] = "dB";
const char str_st1_c[] = "*C";
const char str_uvindex[] = "UV:";
const char str_hpa[] = "hPa";
const char str_percent[] = "% ";
const char str_kmh[] = "km/h";
const char str_bat_error[] = "Battery error";

AccuweatherHourlyData dataH[12];
AccuweatherCurrentData dataC;

int wifi_connection_status;
int weather_status;
int hourly_status;

MAX17048 pwr_mgmt;

const uint16_t pos_x_big_ico = 260;

const int ACU_MAX_KEYS = 4;
int acu_key = 0;

Accuweather aw(acu_keys[0], 274663, "en-en", true);

//===========================================================================================================================================
//===========================================================================================================================================
//===========================================================================================================================================
void setup()
{
//---------------------------------------------------------------------------- SERIAL
#ifdef USE_UART
  Serial.begin(115200);
  Serial.println();
  Serial.println("Setup");
#endif
  delay(100);
  
//---------------------------------------------------------------------------- EPD
#ifdef USE_EPD
  display.init(115200);
#endif

//---------------------------------------------------------------------------- WIFI
#ifdef USE_WIFI
  wifi_connection_status = wifi_connect();
  int timeStart = millis();
#endif

//---------------------------------------------------------------------------- BATTERY
#ifdef USE_MAX17048
  Wire.begin(D1,D6);                          //sda,scl
  pwr_mgmt.attatch(Wire);

  #ifdef USE_UART
    //Serial.print("VCELL ADC : ");
    //Serial.println(pwr_mgmt.adc());
    Serial.print("VCELL V   : ");
    Serial.println(pwr_mgmt.voltage());
    Serial.print("VCELL SOC : ");
    Serial.print(pwr_mgmt.percent());
    Serial.println(" \%");
    //Serial.print("VCELL SOC : ");
    //Serial.print(pwr_mgmt.accuratePercent());
    //Serial.println(" \%");
    Serial.println();
  #endif
 
#endif

//---------------------------------------------------------------------------- ACCUWEATHER
  if (wifi_connection_status == 0)
  {
    do {
      weather_status = get_weather();
      #ifdef USE_UART
        Serial.print("Accu status:");
        Serial.println(weather_status);
      #endif

      hourly_status = get_hourly_forcast();
      #ifdef USE_UART
        Serial.print("Hourly status:");
        Serial.println(hourly_status);
      #endif
      
      if ((weather_status == 0) && (hourly_status == 0)) break;

      acu_key++;
      if (acu_key >= ACU_MAX_KEYS) break;
      Serial.println("Change Accuweather api key");
      aw.changeApiKey(acu_keys[acu_key]);

    } while ((weather_status != 0) || (hourly_status != 0));
  }
  

//---------------------------------------------------------------------------- DISPLAY DATA
#ifdef USE_EPD
  display_weather(wifi_connection_status, weather_status, hourly_status);
#endif

//---------------------------------------------------------------------------- GO TO SLEEP
  if (wifi_connection_status == 0)
  {
    go_to_sleep(UPDATE_PERIOD);
  }
  else
  {
    go_to_sleep(WIFI_ERR_PERIOD);
  }
}
//===========================================================================================================================================
//===========================================================================================================================================
//===========================================================================================================================================
void loop()
{
}

//===========================================================================================================================================
//===========================================================================================================================================
//===========================================================================================================================================
int wifi_connect()
{
//---------------------------------------------------------------------------- WIFI 1
  int i = 50;

  #ifdef USE_UART
    Serial.print("Connecting to: ");
    Serial.println(ssid_1);
  #endif 

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_1, password_1);

  do
  {
    if (WiFi.status() == WL_CONNECTED) break;
    delay(500);
    #ifdef USE_UART
      Serial.println("Connecting to WiFi..");
    #endif  
  } while (--i);

  if(i)
  {
    #ifdef USE_UART
      Serial.println("Connected to the WiFi network :)");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP()); 
    #endif
    return 0;
  }
//---------------------------------------------------------------------------- WIFI 2 (alternative)
    i = 50;
  
    #ifdef USE_UART
      Serial.print("Connecting to: ");
      Serial.println(ssid_2);
    #endif 
  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid_2, password_2);
  
    do
    {
      if (WiFi.status() == WL_CONNECTED) break;
      delay(500);
      #ifdef USE_UART
        Serial.println("Connecting to WiFi..");
      #endif  
    } while (--i);
  
    if(i)
    {
      #ifdef USE_UART
        Serial.println("Connected to the WiFi network :)");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); 
      #endif
      return 0;
    }
    else
    {
      #ifdef USE_UART
        Serial.println("WiFi error");
      #endif
        
      return -1;
    }
}

//===========================================================================================================================================
int get_weather()
{
#ifdef USE_ACCUWEATHER
  int timeStart = millis();

  int ret = aw.getCurrent(&dataC);

  if (ret != 0)
  {
    #ifdef USE_UART
      Serial.print("ACCUWEATHER ERROR: ");
      Serial.println(ret);
     #endif

    return ret;
  }

  while (aw.continueDownload() > 0)
  {
  }

  #ifdef PRINT_WEATHER
    print_actual_weather();
  #endif

  timeStart = millis() - timeStart;

  #ifdef USE_UART
    Serial.printf("Downloaded and parsed in %d ms\n", timeStart);
    Serial.println(String(ESP.getFreeHeap()));
  #endif  
  
  return 0;
#else                                                                 //fake data - epd test
  dataC.WeatherText = "Mostly cloudy";
  dataC.WeatherIcon = 7;
  dataC.Temperature = -16.60;
  dataC.RealFeelTemperature = 16.60;
  dataC.RelativeHumidity = 77.00;
  dataC.WindDirection = 115;
  dataC.WindSpeed = 6.70;
  dataC.UVIndex = 1;
  dataC.Pressure = 1023.00;
  return 0;
#endif
}
//===========================================================================================================================================
int get_hourly_forcast()
{
#ifdef USE_HOURLY_FORCAST

  int timeStart = millis();

  int ret = aw.getHourly(&dataH[0], 12);

  if (ret != 0)
  {
    #ifdef USE_UART
      Serial.print("ACCUWEATHER ERROR: ");
      Serial.println(ret);
    #endif
    
    return ret;
  }

  while (aw.continueDownload() > 0)
  {
  }

  //print_dataH();

  timeStart = millis() - timeStart;

  Serial.printf("Downloaded and parsed forcast in %d ms\n", timeStart);
  Serial.println(String(ESP.getFreeHeap()));
#else
  dataH[2].Temperature = -1.20;
  dataH[5].Temperature = 4.40;
  dataH[8].Temperature = 4.90;
  dataH[11].Temperature = 27.90;

  dataH[2].DateTime = "2019-09-21T15:19:21+00:00";
  dataH[5].DateTime = "2019-09-21T18:19:21+00:00";
  dataH[8].DateTime = "2019-09-21T11:19:21+00:00";
  dataH[11].DateTime = "2019-09-22T00:19:21+00:00";

  dataH[1].RainProbability = 13;
#endif

  return 0;
}

//===========================================================================================================================================
void display_weather(int wifi_connection_status, int w_status , int h_status)
{
  char new_date[32];
  char new_data[8];
  float my_temp;

  int pos_y_1_line = 64;    // aktualna temperatura
  int pos_y_2_line = 118;   // temperatura odczuwalna + UV
  int pos_y_w_line = 114;   // opis pogody
  int pos_y_3_line = 148;   // ciśnienie + wilgotność itd
  int pos_y_4_line = 244;   // temp prognozy
  int pos_y_5_line = 268;   // czas prognozy
  int pos_y_6_line = 295;   // status

  int pos_x_wind;
  int pos_x_wind_ico;
  
  int pos_x_prob;
  int pos_x_prob_ico;
  
  display.fillScreen(GxEPD_WHITE);
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  
//----------------------------------------------------------------------------- background
  display.fillRect(1, 160, 399, 2, GxEPD_BLACK);
  display.fillRect(1, 276, 399, 2, GxEPD_BLACK);
  
//----------------------------------------------------------------------------- 1. line
  if (w_status == 0)
  {
    display.setFont(&Open_Sans_Bold_52);

    my_temp = round(dataC.Temperature * 10) / 10.0;
    display.setCursor(25, pos_y_1_line);
    display.print(my_temp,1);
    display.print(str_st1_c);

//----------------------------------------------------------------------------- 2. line    

//----------------------------------------------------- real_temp
    display.setFont(&Roboto_Medium_25);                                                    
    display.setCursor(10, pos_y_2_line);
   
    int real_temp = (int)(round(dataC.RealFeelTemperature));
    display.print(real_temp);
    display.print(str_st1_c);

//----------------------------------------------------- humidity
    int humidity = int(round(dataC.RelativeHumidity));

    display.drawBitmap(90, pos_y_2_line - 25, hum_ico, 32, 32, GxEPD_BLACK);          //wypozycjonowac
    display.setCursor(122, pos_y_2_line);
    display.print(humidity);
    display.print(str_percent);

//----------------------------------------------------- uv
    //int uv_index = (int)(round(dataC.UVIndex));
    //display.print(str_uvindex);
    //display.print(uv_index);

//----------------------------------------------------------------------------- 3. line

//----------------------------------------------------- pressure     
    int pressure = int(round(dataC.Pressure));
    
    display.setCursor(10, pos_y_3_line);
    display.print(pressure);
    display.print(str_hpa);
    
//----------------------------------------------------- wind 
    int wind_speed = (int)(round(dataC.WindSpeed));   

    if(wind_speed<10)                                                               //ok
    {
      pos_x_wind = 160;
      pos_x_wind_ico = 235;
    }
      else if((wind_speed>9)&&(wind_speed<100))                                     //ok
    {
      pos_x_wind = 150;
      pos_x_wind_ico = 235;
    }
    else                                                                            //ok
    {
      pos_x_wind = 145;
      pos_x_wind_ico = 245;
    }
    
    display.setCursor(pos_x_wind, pos_y_3_line);
    display.print(wind_speed);
    display.print(str_kmh);

    display_wind_dir(dataC.WindDirection, pos_x_wind_ico, pos_y_3_line - 33);       //ok

//----------------------------------------------------- probability
    int prob = (dataH[1].RainProbability);   
  
    if(prob<10)                                                                     //ok
    {
      pos_x_prob_ico = 320;
      pos_x_prob = 357;
    }
      else if((prob>9)&&(prob<100))                                                 //ok
    {
      pos_x_prob_ico = 307;
      pos_x_prob = 345;
    }
    else                                                                            //ok
    {
      pos_x_prob_ico = 295;
      pos_x_prob = 332;
    }

    display.drawBitmap(pos_x_prob_ico, pos_y_3_line - 27, infoIco5, 35, 35, GxEPD_BLACK);     //ok
    display.setCursor(pos_x_prob, pos_y_3_line);
    display.print(prob);
    display.print('%');

//----------------------------------------------------------------------------- main icon and weather description
    display_main_weather_icon(dataC.WeatherIcon, pos_x_big_ico, 4);
    
    uint16_t pos_x_description = set_weather_description_offset(dataC.WeatherText);
    
    display.setFont(&Roboto_Medium_18);                                
    display.setCursor(pos_x_description, pos_y_w_line);
    display.print(dataC.WeatherText);                                                                           

//----------------------------------------------------------------------------- 5. line - update time
    dataC.LocalObservationDateTime.toCharArray(new_date, 20);
    memmove(new_date, new_date + 11, 8);
    memcpy(new_data, new_date, 5);
    new_data[5] = 0;
    display.setCursor(335, pos_y_6_line);
    display.setFont(&Roboto_Medium_17);    
    display.print(new_data);
  }
  else
//----------------------------------------------------------------------------- accuweather error
  {
    display.setFont(&Roboto_Medium_17);    
    display.setCursor(232, pos_y_6_line);
    display.print(str_accuweather_err);
  }
  
//----------------------------------------------------------------------------- wifi
  display.setFont(&Roboto_Medium_17);   
  display.setCursor(30, pos_y_6_line);

#ifdef USE_WIFI_ICO
  display.drawBitmap(6, 281, wifi_ico, 24, 16, GxEPD_BLACK); 
#endif

  if (wifi_connection_status == 0)
  {
     #ifndef USE_WIFI_ICO
      display.print(WiFi.SSID());
      display.print(' ');
    #endif  
    
    display.print(WiFi.RSSI());
    
    display.print(str_db);
  }
  else
  {
    display.print(str_wifi_err);
  }

//----------------------------------------------------------------------------- battery
  display_battery(pos_y_6_line); 

//----------------------------------------------------------------------------- weather forcast
  if (h_status == 0) 
  {
    display_forcast(2, pos_y_4_line, pos_y_5_line);         //3h
    display_forcast(5, pos_y_4_line, pos_y_5_line);         //6h
    display_forcast(8, pos_y_4_line, pos_y_5_line);         //9h
    display_forcast(11, pos_y_4_line, pos_y_5_line);        //12h
  }

  display.display();
  display.powerOff();
  display.hibernate();
  Serial.println("EPD is sleeping");
}

//===========================================================================================================================================
void display_forcast(int idx, int pos_y_temp, int pos_y_time)
{
  float my_temp = round(dataH[idx].Temperature * 10) / 10.0;
  char new_date[32];
  char new_data[8];
  int pos_x_ico;
  int pos_x_time;
  int pos_x_temp;

//-------------------------------------------------- temp & ico
  int my_t = (int)(round(my_temp));  

  switch(idx)
  {
    case 2: pos_x_ico = 22; pos_x_time = pos_x_ico + 2; pos_x_temp = set_forcast_temperature_position(pos_x_ico, my_t); break;
    case 5: pos_x_ico = 122; pos_x_time = pos_x_ico + 2; pos_x_temp = set_forcast_temperature_position(pos_x_ico, my_t);  break;        
    case 8: pos_x_ico = 222; pos_x_time = pos_x_ico + 2; pos_x_temp = set_forcast_temperature_position(pos_x_ico, my_t); break;        
    case 11: pos_x_ico = 322; pos_x_time = pos_x_ico + 2; pos_x_temp = set_forcast_temperature_position(pos_x_ico, my_t); break;
  }
  
  display_hourly_weather_icon(dataH[idx].WeatherIcon, pos_x_ico, pos_y_temp - 80);

  display.setFont(&Roboto_Black_25); 
  display.setCursor(pos_x_temp, pos_y_temp);
  display.print(my_t);
  display.print(str_st1_c);

//-------------------------------------------------- time
  dataH[idx].DateTime.toCharArray(new_date, 20);
  memmove(new_date, new_date + 11, 8);
  memcpy(new_data, new_date, 5);
  new_data[5] = 0;

  display.setFont(&Roboto_Medium_18);                                            
  display.setCursor(pos_x_time, pos_y_time);
  display.print(new_data);
}

//===========================================================================================================================================
uint16_t set_weather_description_offset(String str)
{
  int16_t tbx, tby; 
  uint16_t tbw, tbh;

  display.setFont(&Roboto_Medium_18);                                
  display.getTextBounds(str.c_str(), 0, 0, &tbx, &tby, &tbw, &tbh);

  uint16_t x_center = pos_x_big_ico + (90/2);               //srodek ikonki pogody 90 - uwzglednic wielkosc ikonki w obliczeniach
  uint16_t  pos_x_napisu = (x_center - (tbw / 2)) - tbx;
 
  return pos_x_napisu;
}

//===========================================================================================================================================
int set_forcast_temperature_position(int pos_x, int temp)
{
  if(temp<-9)                               
  {
    return pos_x-8;
  }
  else if((temp>-10)&&(temp<0))             
  {
    return pos_x-2;
  }
  else if((temp>-1)&&(temp<10))            
  {
     return pos_x+4;
  }
  else                                    
  {
    return pos_x-2;
  }
}

//===========================================================================================================================================
void set_icon_offset(uint8_t w_number, int *x_pos, int *y_pos) {
#ifdef ICON_PACK_3
  switch (w_number)
  {
    case 3: {
        *y_pos += 2;
      } break;
    case 38: {
        *y_pos += 8;
      } break;
  }
#endif
}

//===========================================================================================================================================
void display_hourly_weather_icon(uint8_t w_number, int x_pos, int y_pos)
{
  int size_w = 55;
  int size_h = 55;

  set_icon_offset(w_number, &x_pos, &y_pos);
  
  switch (w_number)
  {
    case 1: display.drawBitmap(x_pos, y_pos, weatherIco1, size_w, size_h, GxEPD_BLACK); break;
    case 2: display.drawBitmap(x_pos, y_pos, weatherIco2, size_w, size_h, GxEPD_BLACK); break;
    case 3: display.drawBitmap(x_pos, y_pos, weatherIco3, size_w, size_h, GxEPD_BLACK); break;
    case 4: display.drawBitmap(x_pos, y_pos, weatherIco4, size_w, size_h, GxEPD_BLACK); break;
    case 5: display.drawBitmap(x_pos, y_pos, weatherIco5, size_w, size_h, GxEPD_BLACK); break;
    case 6: display.drawBitmap(x_pos, y_pos, weatherIco6, size_w, size_h, GxEPD_BLACK); break;
    case 7: display.drawBitmap(x_pos, y_pos, weatherIco7, size_w, size_h, GxEPD_BLACK); break;
    case 8: display.drawBitmap(x_pos, y_pos, weatherIco8, size_w, size_h, GxEPD_BLACK); break;

    case 11: display.drawBitmap(x_pos, y_pos, weatherIco11, size_w, size_h, GxEPD_BLACK); break;
    case 12: display.drawBitmap(x_pos, y_pos, weatherIco12, size_w, size_h, GxEPD_BLACK); break;
    case 13: display.drawBitmap(x_pos, y_pos, weatherIco13, size_w, size_h, GxEPD_BLACK); break;
    case 14: display.drawBitmap(x_pos, y_pos, weatherIco14, size_w, size_h, GxEPD_BLACK); break;
    case 15: display.drawBitmap(x_pos, y_pos, weatherIco15, size_w, size_h, GxEPD_BLACK); break;
    case 16: display.drawBitmap(x_pos, y_pos, weatherIco16, size_w, size_h, GxEPD_BLACK); break;
    case 17: display.drawBitmap(x_pos, y_pos, weatherIco17, size_w, size_h, GxEPD_BLACK); break;
    case 18: display.drawBitmap(x_pos, y_pos, weatherIco18, size_w, size_h, GxEPD_BLACK); break;
    case 19: display.drawBitmap(x_pos, y_pos, weatherIco19, size_w, size_h, GxEPD_BLACK); break;

    case 20: display.drawBitmap(x_pos, y_pos, weatherIco20, size_w, size_h, GxEPD_BLACK); break;
    case 21: display.drawBitmap(x_pos, y_pos, weatherIco21, size_w, size_h, GxEPD_BLACK); break;
    case 22: display.drawBitmap(x_pos, y_pos, weatherIco22, size_w, size_h, GxEPD_BLACK); break;
    case 23: display.drawBitmap(x_pos, y_pos, weatherIco23, size_w, size_h, GxEPD_BLACK); break;
    case 24: display.drawBitmap(x_pos, y_pos, weatherIco24, size_w, size_h, GxEPD_BLACK); break;
    case 25: display.drawBitmap(x_pos, y_pos, weatherIco25, size_w, size_h, GxEPD_BLACK); break;
    case 26: display.drawBitmap(x_pos, y_pos, weatherIco26, size_w, size_h, GxEPD_BLACK); break;
    case 29: display.drawBitmap(x_pos, y_pos, weatherIco29, size_w, size_h, GxEPD_BLACK); break;

    case 30: display.drawBitmap(x_pos, y_pos, weatherIco30, size_w, size_h, GxEPD_BLACK); break;
    case 31: display.drawBitmap(x_pos, y_pos, weatherIco31, size_w, size_h, GxEPD_BLACK); break;
    case 32: display.drawBitmap(x_pos, y_pos, weatherIco32, size_w, size_h, GxEPD_BLACK); break;
    case 33: display.drawBitmap(x_pos, y_pos, weatherIco33, size_w, size_h, GxEPD_BLACK); break;
    case 34: display.drawBitmap(x_pos, y_pos, weatherIco34, size_w, size_h, GxEPD_BLACK); break;
    case 35: display.drawBitmap(x_pos, y_pos, weatherIco35, size_w, size_h, GxEPD_BLACK); break;
    case 36: display.drawBitmap(x_pos, y_pos, weatherIco36, size_w, size_h, GxEPD_BLACK); break;
    case 37: display.drawBitmap(x_pos, y_pos, weatherIco37, size_w, size_h, GxEPD_BLACK); break;
    case 38: display.drawBitmap(x_pos, y_pos, weatherIco38, size_w, size_h, GxEPD_BLACK); break;
    case 39: display.drawBitmap(x_pos, y_pos, weatherIco39, size_w, size_h, GxEPD_BLACK); break;

    case 40: display.drawBitmap(x_pos, y_pos, weatherIco40, size_w, size_h, GxEPD_BLACK); break;
    case 41: display.drawBitmap(x_pos, y_pos, weatherIco41, size_w, size_h, GxEPD_BLACK); break;
    case 42: display.drawBitmap(x_pos, y_pos, weatherIco42, size_w, size_h, GxEPD_BLACK); break;
    case 43: display.drawBitmap(x_pos, y_pos, weatherIco43, size_w, size_h, GxEPD_BLACK); break;
    case 44: display.drawBitmap(x_pos, y_pos, weatherIco44, size_w, size_h, GxEPD_BLACK); break;
  }
}
//===========================================================================================================================================
void display_main_weather_icon(uint8_t w_number, int x_pos, int y_pos)
{
  int size_w = 90;
  int size_h = 90;

  set_icon_offset(w_number, &x_pos, &y_pos);
  
  switch (w_number)
  {
    case 1: display.drawBitmap(x_pos, y_pos, bigWeatherIco1, size_w, size_h, GxEPD_BLACK); break;
    case 2: display.drawBitmap(x_pos, y_pos, bigWeatherIco2, size_w, size_h, GxEPD_BLACK); break;
    case 3: display.drawBitmap(x_pos, y_pos, bigWeatherIco3, size_w, size_h, GxEPD_BLACK); break;
    case 4: display.drawBitmap(x_pos, y_pos, bigWeatherIco4, size_w, size_h, GxEPD_BLACK); break;
    case 5: display.drawBitmap(x_pos, y_pos, bigWeatherIco5, size_w, size_h, GxEPD_BLACK); break;
    case 6: display.drawBitmap(x_pos, y_pos, bigWeatherIco6, size_w, size_h, GxEPD_BLACK); break;
    case 7: display.drawBitmap(x_pos, y_pos, bigWeatherIco7, size_w, size_h, GxEPD_BLACK); break;
    case 8: display.drawBitmap(x_pos, y_pos, bigWeatherIco8, size_w, size_h, GxEPD_BLACK); break;

    case 11: display.drawBitmap(x_pos, y_pos, bigWeatherIco11, size_w, size_h, GxEPD_BLACK); break;
    case 12: display.drawBitmap(x_pos, y_pos, bigWeatherIco12, size_w, size_h, GxEPD_BLACK); break;
    case 13: display.drawBitmap(x_pos, y_pos, bigWeatherIco13, size_w, size_h, GxEPD_BLACK); break;
    case 14: display.drawBitmap(x_pos, y_pos, bigWeatherIco14, size_w, size_h, GxEPD_BLACK); break;
    case 15: display.drawBitmap(x_pos, y_pos, bigWeatherIco15, size_w, size_h, GxEPD_BLACK); break;
    case 16: display.drawBitmap(x_pos, y_pos, bigWeatherIco16, size_w, size_h, GxEPD_BLACK); break;
    case 17: display.drawBitmap(x_pos, y_pos, bigWeatherIco17, size_w, size_h, GxEPD_BLACK); break;
    case 18: display.drawBitmap(x_pos, y_pos, bigWeatherIco18, size_w, size_h, GxEPD_BLACK); break;
    case 19: display.drawBitmap(x_pos, y_pos, bigWeatherIco19, size_w, size_h, GxEPD_BLACK); break;

    case 20: display.drawBitmap(x_pos, y_pos, bigWeatherIco20, size_w, size_h, GxEPD_BLACK); break;
    case 21: display.drawBitmap(x_pos, y_pos, bigWeatherIco21, size_w, size_h, GxEPD_BLACK); break;
    case 22: display.drawBitmap(x_pos, y_pos, bigWeatherIco22, size_w, size_h, GxEPD_BLACK); break;
    case 23: display.drawBitmap(x_pos, y_pos, bigWeatherIco23, size_w, size_h, GxEPD_BLACK); break;
    case 24: display.drawBitmap(x_pos, y_pos, bigWeatherIco24, size_w, size_h, GxEPD_BLACK); break;
    case 25: display.drawBitmap(x_pos, y_pos, bigWeatherIco25, size_w, size_h, GxEPD_BLACK); break;
    case 26: display.drawBitmap(x_pos, y_pos, bigWeatherIco26, size_w, size_h, GxEPD_BLACK); break;
    case 29: display.drawBitmap(x_pos, y_pos, bigWeatherIco29, size_w, size_h, GxEPD_BLACK); break;

    case 30: display.drawBitmap(x_pos, y_pos, bigWeatherIco30, size_w, size_h, GxEPD_BLACK); break;
    case 31: display.drawBitmap(x_pos, y_pos, bigWeatherIco31, size_w, size_h, GxEPD_BLACK); break;
    case 32: display.drawBitmap(x_pos, y_pos, bigWeatherIco32, size_w, size_h, GxEPD_BLACK); break;
    case 33: display.drawBitmap(x_pos, y_pos, bigWeatherIco33, size_w, size_h, GxEPD_BLACK); break;
    case 34: display.drawBitmap(x_pos, y_pos, bigWeatherIco34, size_w, size_h, GxEPD_BLACK); break;
    case 35: display.drawBitmap(x_pos, y_pos, bigWeatherIco35, size_w, size_h, GxEPD_BLACK); break;
    case 36: display.drawBitmap(x_pos, y_pos, bigWeatherIco36, size_w, size_h, GxEPD_BLACK); break;
    case 37: display.drawBitmap(x_pos, y_pos, bigWeatherIco37, size_w, size_h, GxEPD_BLACK); break;
    case 38: display.drawBitmap(x_pos, y_pos, bigWeatherIco38, size_w, size_h, GxEPD_BLACK); break;
    case 39: display.drawBitmap(x_pos, y_pos, bigWeatherIco39, size_w, size_h, GxEPD_BLACK); break;

    case 40: display.drawBitmap(x_pos, y_pos, bigWeatherIco40, size_w, size_h, GxEPD_BLACK); break;
    case 41: display.drawBitmap(x_pos, y_pos, bigWeatherIco41, size_w, size_h, GxEPD_BLACK); break;
    case 42: display.drawBitmap(x_pos, y_pos, bigWeatherIco42, size_w, size_h, GxEPD_BLACK); break;
    case 43: display.drawBitmap(x_pos, y_pos, bigWeatherIco43, size_w, size_h, GxEPD_BLACK); break;
    case 44: display.drawBitmap(x_pos, y_pos, bigWeatherIco44, size_w, size_h, GxEPD_BLACK); break;
  }
}

//===========================================================================================================================================
void display_wind_dir(int16_t direct, int x_pos, int y_pos)
{
  int size_w = 50;
  int size_h = 50;

  if ((direct >= 23) && (direct < 68))
  {
    // ("NE");
    display.drawBitmap(x_pos, y_pos, winddir7, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 68) && (direct < 113))
  {
    // ("E");
    display.drawBitmap(x_pos, y_pos, winddir8, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 113) && (direct < 158))
  {
    // ("SE");
    display.drawBitmap(x_pos, y_pos, winddir4, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 158) && (direct < 203))
  {
    // ("S");
    display.drawBitmap(x_pos, y_pos, winddir2, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 203) && (direct < 248))
  {
    // ("SW");
    display.drawBitmap(x_pos, y_pos, winddir3, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 248) && (direct < 293))
  {
    // ("W");
    display.drawBitmap(x_pos, y_pos, winddir1, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 293) && (direct < 338))
  {
    // ("NW");
    display.drawBitmap(x_pos, y_pos, winddir6, size_w, size_h, GxEPD_BLACK);
  }
  else if ((direct >= 338) || (direct < 23))
  {
    // ("N");
    display.drawBitmap(x_pos, y_pos, winddir5, size_w, size_h, GxEPD_BLACK);
  }
}

//===========================================================================================================================================
void display_battery(uint16_t pos_y) 
{
  display.setFont(&Roboto_Medium_17);   
  display.setCursor(130, pos_y);

  uint16_t percent = pwr_mgmt.percent();
  
  if((percent>100)&&(percent<250)) percent = 100;
  
  int pos_x_ico = 100;

  if(percent>100)
  {
    display.drawBitmap(50, 281, bat_empty, 24, 16, GxEPD_BLACK);
    display.print(str_bat_error);
  }
  else if((percent>85)&&(percent<101))
  {
    display.drawBitmap(pos_x_ico, 281, bat_100, 24, 16, GxEPD_BLACK);
    display.print(percent);
    display.print("% ");
  }
  else if((percent>60)&&(percent<86))
  {
    display.drawBitmap(pos_x_ico, 281, bat_75, 24, 16, GxEPD_BLACK);
    display.print(percent);
    display.print("% ");
  }
  else if((percent>40)&&(percent<61))
  {
    display.drawBitmap(pos_x_ico, 281, bat_50, 24, 16, GxEPD_BLACK);
    display.print(percent);
    display.print("% ");
  }
  else if((percent>10)&&(percent<41))
  {
    display.drawBitmap(pos_x_ico, 281, bat_25, 24, 16, GxEPD_BLACK);
    display.print(percent);
    display.print("% ");
  }
  else if(percent<11)
  {
    display.drawBitmap(pos_x_ico, 281, bat_empty, 24, 16, GxEPD_BLACK);
    display.print(percent);
    display.print("% ");
  }
 
}

//===========================================================================================================================================
void go_to_sleep(int seconds)
{
  Serial.print(str_sleep);
  Serial.print(seconds);
  Serial.println(str_seconds);
  ESP.deepSleep(seconds * 1000000, WAKE_RF_DEFAULT);
  delay(2000);
}

//===========================================================================================================================================
void print_actual_weather()
{
  Serial.println("actual weather:");
  Serial.println(dataC.LocalObservationDateTime);
  Serial.println(dataC.EpochTime);
  Serial.println(dataC.WeatherText);
  Serial.println(dataC.WeatherIcon);
  Serial.println(dataC.IsDayTime);
  Serial.println(dataC.Temperature);
  Serial.println(dataC.RealFeelTemperature);
  Serial.println(dataC.RealFeelTemperatureShade);
  Serial.println(dataC.RelativeHumidity);
  Serial.println(dataC.WindDirection);
  Serial.println(dataC.WindSpeed);
  Serial.println(dataC.WindGustSpeed);
  Serial.println(dataC.UVIndex);
  Serial.println(dataC.UVIndexText);
  Serial.println(dataC.Visibility);
  Serial.println(dataC.CloudCover);
  Serial.println(dataC.Pressure);
  Serial.println(dataC.WindDirection);
}
//===========================================================================================================================================
void print_dataH()
{
  for (int i = 0 ; i <= 3; i += 3) {
    Serial.print("Czas: ");
    Serial.println(dataH[i].DateTime);
    //Serial.println(dataH[i]->EpochDateTime);
    //Serial.println(dataH[i]->WeatherIcon);
    Serial.print("Pogoda: ");
    Serial.println(dataH[i].IconPhrase);
    //Serial.println(dataH[i]->IsDaylight);
    Serial.print("Temp: ");
    Serial.println(dataH[i].Temperature);
    //Serial.println(dataH[i]->RealFeelTemperature);
    Serial.print("Wind speed: ");
    Serial.println(dataH[i].WindSpeed);
    //Serial.println(dataH[i]->WindDirection);
    //Serial.println(dataH[i]->WindGustSpeed);
    Serial.print("Hum: ");
    Serial.println(dataH[i].RelativeHumidity);
    //Serial.println(dataH[i]->Visibility);
    Serial.print("UV index: ");
    Serial.println(dataH[i].UVIndex);
    //Serial.println(dataH[i]->UVIndexText);
    //Serial.println(dataH[i]->PrecipitationProbability);
    Serial.print("Rain prob: ");
    Serial.println(dataH[i].RainProbability);
    //Serial.println(dataH[i]->SnowProbability);
    //Serial.println(dataH[i]->IceProbability);
    //Serial.println(dataH[i]->TotalLiquid);
    //Serial.println(dataH[i]->Rain);
    //Serial.println(dataH[i]->Snow);
    //Serial.println(dataH[i]->Ice);
    //Serial.println(dataH[i]->CloudCover);
    Serial.println("  ");
    Serial.println("  ");
  }
}

//===========================================================================================================================================
void draw_margin(String text, int data_len) 
{
  int margin_size = data_len - text.length();
  while (margin_size > 0)
  {
    display.print("  ");
    margin_size--;
 }
}

//===========================================================================================================================================

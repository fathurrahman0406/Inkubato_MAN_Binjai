#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"
#include <EEPROM.h>
#define limit1 A2
#define limit2 A3
#define DHTPIN 12
#define heater 2
#define motor 3
#define fan_in 4
#define fan_rh 5
#define kabut 6
#define buzzer 7
#define ok 11
#define up 9
#define down 10
#define DHTTYPE DHT22
int setting=0;
int suhu=38;
int suhu_e = 0;
int state=0;
int inv;
int inv_e=1;
int h;
int m;
int dd;
int mm;
int yy;
int s;
int set;
int dd_s;
int mm_s;
int dd_s_e=2;
int mm_s_e=3;

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
void setup()
{
	lcd.begin();
	lcd.backlight();
  dht.begin();
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

   if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  pinMode(heater,OUTPUT);
  pinMode(motor,OUTPUT);
  pinMode(fan_in,OUTPUT);
  pinMode(fan_rh,OUTPUT);
  pinMode(kabut,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(ok,INPUT);
  pinMode(down,INPUT);
  pinMode(up,INPUT);
  
}

void loop()
{
  while(state==0){
    DateTime now = rtc.now();
    if (dd_s == now.day() && mm_s == now.month()){lcd.clear();lcd.setCursor(0,0);lcd.print(" Waktu Selesai  ");delay(400);}
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    suhu = EEPROM.read(suhu_e);
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("RH:");
    lcd.print(h,0);
    lcd.print("% ");
    lcd.print("T:");
    lcd.print(t,1);
    lcd.print("'C");

    lcd.setCursor(0,1);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    lcd.print(now.second());
    lcd.print("  set:");
    lcd.print(suhu);
    delay(400);
    if (digitalRead(ok)==0){state=1;delay(200);lcd.clear();} 
    if (t > suhu){digitalWrite(heater,LOW);}
    else if (t < suhu){digitalWrite(heater,HIGH);}
     
    if (now.minute()==30 || now.minute()==00){digitalWrite(motor,HIGH);}
    else if (limit1 || limit2){digitalWrite(motor,LOW);}

    if (t<50){digitalWrite(kabut,HIGH);digitalWrite(fan_rh,HIGH);}
    if (t>60){digitalWrite(kabut,LOW);digitalWrite(fan_rh,LOW);}

    if (now.second()==0){digitalWrite(fan_in,HIGH);}
    if (now.second()==10){digitalWrite(fan_in,LOW);}
   }


  while(state==1){        //setting waktu
    DateTime now = rtc.now();
    s=now.second();
    h=now.hour();
    m=now.minute();
    dd=now.day();
    mm=now.month();
    yy=now.year();
    lcd.setCursor(0,0);
    lcd.print("atur waktu     <"); 
    lcd.setCursor(0,1);
    lcd.print("atur inv motor  ");
    if (digitalRead(down)==0){state=2;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=6;delay(200);lcd.clear();}  
    if (digitalRead(ok)==0){setting=1;delay(200);lcd.clear();}
    while(setting==1){
      lcd.setCursor(0,0);
      lcd.print("atur jam       <");
      lcd.setCursor(0,1);
      lcd.print("atur tanggal    ");
      if (digitalRead(down)==0){setting=2;delay(200);lcd.clear();}
      if (digitalRead(up)==0){setting=3;delay(200);lcd.clear();}
      if (digitalRead(ok)==0){set=1;delay(200);lcd.clear();}
      while (set==1){//setting menit
        lcd.setCursor(0,1);
        lcd.print(h);
        lcd.print(':');
        lcd.print(m);
        lcd.print(':');
        lcd.print(now.second(), DEC);
        lcd.setCursor(0,0);
        lcd.print("menit");
        if (digitalRead(down)==0){m--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){m++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){delay(200);lcd.clear();set=2;lcd.clear();}
        if (m>59){m=0;}
        if (m<0){m=59;}
      }
      
      while (set==2){//setting jam
        lcd.setCursor(0,1);
        lcd.print(h);
        lcd.print(':');
        lcd.print(m);
        lcd.print(':');
        lcd.print(now.second(), DEC);
        lcd.setCursor(0,0);
        lcd.print("jam");
        if (digitalRead(down)==0){h--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){h++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){delay(200);rtc.adjust(DateTime(yy, mm, dd, h, m, s));lcd.clear();setting=1; set=0;}
        if (h>23){h=0;}
        if (h<0){h=23;}
      }
    }


    while(setting==2){
      DateTime now = rtc.now();
      s=now.second();
      h=now.hour();
      m=now.minute();
      dd=now.day();
      mm=now.month();
      yy=now.year();
      lcd.setCursor(0,0);
      lcd.print("atur jam        ");
      lcd.setCursor(0,1);
      lcd.print("atur tanggal   <");
      if (digitalRead(down)==0){setting=3;delay(200);lcd.clear();}
      if (digitalRead(up)==0){setting=1;delay(200);lcd.clear();}  
      if (digitalRead(ok)==0){set=1;delay(200);lcd.clear();}
      while (set==1){
        lcd.setCursor(0,1);
        lcd.print(dd);
        lcd.print(':');
        lcd.print(mm);
        lcd.print(':');
        lcd.print(yy);
        lcd.setCursor(0,0);
        lcd.print("tanggal");
        if (digitalRead(down)==0){dd--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){dd++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){set=2;delay(200);lcd.clear();}
        if (dd>31){dd=1;}
        if (dd<1){dd=31;}
      }
      while (set==2){
        lcd.setCursor(0,1);
        lcd.print(dd);
        lcd.print(':');
        lcd.print(mm);
        lcd.print(':');
        lcd.print(yy);
        lcd.setCursor(0,0);
        lcd.print("bulan");
        if (digitalRead(down)==0){mm--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){mm++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){set=3;delay(200);lcd.clear();}
        if (mm>12){mm=1;}
        if (mm<1){mm=12;}
      }

      while (set==3){
        lcd.setCursor(0,1);
        lcd.print(dd);
        lcd.print(':');
        lcd.print(mm);
        lcd.print(':');
        lcd.print(yy);
        lcd.setCursor(0,0);
        lcd.print("tahun");
        if (digitalRead(down)==0){yy--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){yy++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){set=0;setting=2;rtc.adjust(DateTime(yy, mm, dd, h, m, s));delay(200);lcd.clear();}
      }

    }


    while(setting==3){
      lcd.setCursor(0,0);
      lcd.print("Keluar ??");
      if (digitalRead(down)==0){setting=1;delay(200);lcd.clear();}
      if (digitalRead(up)==0){setting=2;delay(200);lcd.clear();}  
      if (digitalRead(ok)==0){state=1; setting=0;delay(200);lcd.clear();}  
      
    }
  }


 
  while(state==2){        //setting interval motor
    inv = EEPROM.read(inv_e);
    lcd.setCursor(0,0);
    lcd.print("atur waktu      "); 
    lcd.setCursor(0,1);
    lcd.print("atur inv motor <");
    if (digitalRead(down)==0){state=3;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=1;delay(200);lcd.clear();}
    if (digitalRead(ok)==0){setting=2;delay(200);lcd.clear();} 
    while(setting==2){
       lcd.setCursor(0,0);
       lcd.print("Interval : ");
       lcd.setCursor(0,1);
       lcd.print(inv);
       if (digitalRead(down)==0){inv--;delay(200);lcd.clear();}
       if (digitalRead(up)==0){inv++;delay(200);lcd.clear();}
       if (digitalRead(ok)==0){setting=0;state=2;EEPROM.write(inv_e, inv);delay(200);lcd.clear();}
    }
  }

  while(state==3){        //setting suhu
    suhu = EEPROM.read(suhu_e);
    lcd.setCursor(0,0);
    lcd.print("atur inv motor  ");
    lcd.setCursor(0,1);
    lcd.print("atur Suhu      <");
    if (digitalRead(down)==0){state=4;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=2;delay(200);lcd.clear();}  
    if (digitalRead(ok)==0){setting=1;delay(200);lcd.clear();} 
    while(setting==1){
       lcd.setCursor(0,0);
       lcd.print("Suhu : ");
       lcd.print(suhu);
       if (digitalRead(down)==0){suhu--;delay(200);lcd.clear();}
       if (digitalRead(up)==0){suhu++;delay(200);lcd.clear();}
       if (digitalRead(ok)==0){setting=0;state=3;EEPROM.write(suhu_e, suhu);delay(200);lcd.clear();}
    }
  }

  while(state==4){        //setting waktu selesai
    dd_s = EEPROM.read(dd_s_e);
    mm_s = EEPROM.read(mm_s_e);
    lcd.setCursor(0,0);
    lcd.print("atur Suhu       ");
    lcd.setCursor(0,1); 
    lcd.print("atur wkt slsai <");
    if (digitalRead(down)==0){state=5;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=3;delay(200);lcd.clear();} 
    if (digitalRead(ok)==0){setting=1;delay(200);lcd.clear();}
     while (setting==1){
        lcd.setCursor(0,1);
        lcd.print(dd_s);
        lcd.print(':');
        lcd.print(mm_s);
        lcd.setCursor(0,0);
        lcd.print("tanggal");
        if (digitalRead(down)==0){dd_s--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){dd_s++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){setting=2;delay(200);lcd.clear();}
        if (dd_s>31){dd_s=1;}
        if (dd_s<1){dd_s=31;}
      }
      while (setting==2){
        lcd.setCursor(0,1);
        lcd.print(dd_s);
        lcd.print(':');
        lcd.print(mm_s);
        lcd.setCursor(0,0);
        lcd.print("bulan");
        if (digitalRead(down)==0){mm_s--;delay(200);lcd.clear();}
        if (digitalRead(up)==0){mm_s++;delay(200);lcd.clear();}
        if (digitalRead(ok)==0){setting=0;state=4;delay(200);EEPROM.write(dd_s_e, dd_s);EEPROM.write(mm_s_e, mm_s);lcd.clear();}
        if (mm_s>12){mm_s=1;}
        if (mm_s<1){mm_s=12;}
      }
      
  }

  while(state==5){  //lihat tanggal
    lcd.setCursor(0,0);
    lcd.print("atur wkt slsai  ");
    lcd.setCursor(0,1);
    lcd.print("Lihat Tanggal  <");
    if (digitalRead(down)==0){state=6;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=4;delay(200);lcd.clear();}
    if (digitalRead(ok)==0){setting=1;delay(200);lcd.clear();}  
    while(setting==1){
      DateTime now = rtc.now();
      lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
      lcd.setCursor(0,1);
      lcd.print(now.day(), DEC);
      lcd.print("/");
      lcd.print(now.month(), DEC);
      lcd.print("/");
      lcd.print(now.year(), DEC);
      lcd.print(" ");
      lcd.print(now.hour(), DEC);
      lcd.print(":");
      lcd.print(now.minute(), DEC);
      lcd.print(":");
      lcd.print(now.second(), DEC);
      if (digitalRead(ok)==0){setting=0;state=5;delay(200);lcd.clear();}
    }
  }

  while(state==6){  //keluar
    lcd.setCursor(0,0);
    lcd.print("Kembali    ?????");
    if (digitalRead(down)==0){state=1;delay(200);lcd.clear();}
    if (digitalRead(up)==0){state=5;delay(200);lcd.clear();}
    if (digitalRead(ok)==0){setting=0;state=0;delay(200);lcd.clear();}   
  }

 
}

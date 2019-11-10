/*  Ceas, termometru si iesire comanda pornire ac
 *  09.11.2019
 *  
 *  Display 1 20x4:                Setup:
 *  +--------------------+      +----------------+
 *  |ZZ.LL.AAAA HH:MM    |      |    >HH :>MM    |
 *  |      La noi: 23.0*C|      |>ZZ />LL />AAAA |
 *  |     La Sofi: 23.5*C|      +----------------+
 *  |       Afara:-10.5*C|   
 *  +--------------------+         
 *  Display 2 20x4:               
 *  +--------------------+     
 *  |      Living: 23.0*C|     
 *  |        Baie: 23.0*C|     
 *  | La Centrala: 23.5*C|      
 *  |Tur centrala: 23.5*C|   
 *  +--------------------+ 
 */

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

RTC_DS1307 rtc;
LiquidCrystal_I2C lcd1(0x27, 20, 4), lcd2(0x23, 20, 4);

#define ONE_WIRE_BUS 2 // pin bus 18b20
#define TEMPERATURE_PRECISION1 11 // rezolutie biti
#define TEMPERATURE_PRECISION2 11 // rezolutie biti
#define TEMPERATURE_PRECISION3 11 // rezolutie biti
#define TEMPERATURE_PRECISION4 11 // rezolutie biti
#define TEMPERATURE_PRECISION5 11 // rezolutie biti
#define TEMPERATURE_PRECISION6 11 // rezolutie biti
#define TEMPERATURE_PRECISION7 11 // rezolutie biti

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress Thermometer1, Thermometer2, Thermometer3, Thermometer4, Thermometer5, Thermometer6, Thermometer7;


#define relac 3  // pin pt iesire releu ac, (scoate + )fata de -

int delaytime = 1000;  // 1000 = 1s, timp actionare releu ac
byte cuplat = 0; // releu temp cuplat
char daysOfTheWeek[7][12] = {"Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
const long interval = 3000;  // Citire senzori la fiecare 3 sec
const int btSet = 8;
const int btUp = 9;
const int btDown = 10;

int DD,MM,YY,H,M,S, set_state, up_state, down_state;
float temp1, temp2, temp3, temp4, temp5, temp6, temp7;
float t1, t2, t3, t4, t5, t6, t7;
int btnCount = 0;
unsigned long previousMillis = 0;
unsigned long currentMillis; 
String sDD;
String sMM;
String sYY;
String sH;
String sM;
String sS;
boolean backlightON = true;
boolean setupScreen = false;


void setup () {
  Serial.begin(9600);
  pinMode(btSet, INPUT_PULLUP);
  pinMode(btUp, INPUT_PULLUP);
  pinMode(btDown, INPUT_PULLUP);
  lcd1.begin();   // init the LCD 1
  lcd2.begin();   // init the LCD 2
  lcd1.backlight();   // activare backligt
  lcd2.backlight();   // activare backligt
  lcd1.clear();
  lcd2.clear();
  sensors.begin();  // initializare senzori DS18B20
 
  pinMode(relac,OUTPUT);
  digitalWrite(relac,LOW);
  
  Serial.print("Gasire senzori....");
  Serial.print("Gasiti: ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" Senzori.");
  Serial.print("Parasite power este: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(Thermometer1, 5)) Serial.println("Nu gasesc Adresa pentru Senzor 5"); //la noi= temp1
  if (!sensors.getAddress(Thermometer2, 1)) Serial.println("Nu gasesc Adresa pentru Senzor 1"); //la sofi= temp2
  if (!sensors.getAddress(Thermometer3, 4)) Serial.println("Nu gasesc Adresa pentru Senzor 4"); //afara= temp3
  if (!sensors.getAddress(Thermometer4, 2)) Serial.println("Nu gasesc Adresa pentru Senzor 2"); //living= temp4
  if (!sensors.getAddress(Thermometer5, 0)) Serial.println("Nu gasesc Adresa pentru Senzor 0"); //baie= temp5
  if (!sensors.getAddress(Thermometer6, 3)) Serial.println("Nu gasesc Adresa pentru Senzor 3"); //magazie= temp6
  if (!sensors.getAddress(Thermometer7, 6)) Serial.println("Nu gasesc Adresa pentru Senzor 6"); //tur centrala= temp7

  Serial.print("Senzor 0 Adresa: ");
  printAddress(Thermometer1);
  Serial.println();

  Serial.print("Senzor 1 Adresa: ");
  printAddress(Thermometer2);
  Serial.println();
  
  Serial.print("Senzor 2 Adresa: ");
  printAddress(Thermometer3);
  Serial.println();

  Serial.print("Senzor 3 Adresa: ");
  printAddress(Thermometer4);
  Serial.println();
  
  Serial.print("Senzor 4 Adresa: ");
  printAddress(Thermometer5);
  Serial.println();
  
  Serial.print("Senzor 5 Adresa: ");
  printAddress(Thermometer6);
  Serial.println();
  
  Serial.print("Senzor 6 Adresa: ");
  printAddress(Thermometer7);
  Serial.println();
  
  sensors.setResolution(Thermometer1, TEMPERATURE_PRECISION1);
  sensors.setResolution(Thermometer2, TEMPERATURE_PRECISION2);
  sensors.setResolution(Thermometer3, TEMPERATURE_PRECISION3);
  sensors.setResolution(Thermometer4, TEMPERATURE_PRECISION4);
  sensors.setResolution(Thermometer5, TEMPERATURE_PRECISION5);
  sensors.setResolution(Thermometer6, TEMPERATURE_PRECISION6);
  sensors.setResolution(Thermometer7, TEMPERATURE_PRECISION7);

  Serial.print("Senzor 0 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer1), DEC);
  Serial.println();

  Serial.print("Senzor 1 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer2), DEC);
  Serial.println();
 
  Serial.print("Senzor 2 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer3), DEC);
  Serial.println();
  
  Serial.print("Senzor 3 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer4), DEC);
  Serial.println();
  
  Serial.print("Senzor 4 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer5), DEC);
  Serial.println();
  
  Serial.print("Senzor 5 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer6), DEC);
  Serial.println();
  
  Serial.print("Senzor 6 Rezolutie biti: ");
  Serial.print(sensors.getResolution(Thermometer7), DEC);
  Serial.println();
 
}

void loop () {
  currentMillis = millis();
  readBtns();
  getTempTemp();    
  getTimeDate();
  if (!setupScreen){
    lcd1Print();
  }
  else{
    lcd1Setup();
  }

if ((temp1<17.) and (cuplat == 1)) // cand scade sub limita setata da o comanda
{
  Serial.println("Temperatura scazuta!");
  digitalWrite(relac, LOW); // rel oprit
  lcd1.setCursor(17,0); //randul 1
  lcd1.print("AC_");
  cuplat = 0;
  digitalWrite(relac, HIGH); // releul cupleaza
  delay(delaytime);
  digitalWrite(relac, LOW); // releul decupleaza
  delay(delaytime);
 }
 
 if ((temp1>17.) and (cuplat == 0)) // cand creste peste limita setata nu da comanda
{
  Serial.println("Temperatura normala!");
  lcd1.setCursor(17,0); //randul 1
  lcd1.print("___");
  cuplat = 1;
  digitalWrite(relac, LOW);
  delay(delaytime);
 }
}


void readBtns(){
  set_state = digitalRead(btSet);
  up_state = digitalRead(btUp);
  down_state = digitalRead(btDown);
  // Comutare backlight on/off prin aparea butonului down
  if (down_state==LOW && btnCount==0){
    if (backlightON){
      lcd1.noBacklight();
      lcd2.noBacklight();
      backlightON = false;
    }
    else{
      lcd1.backlight();
      lcd2.backlight();
      backlightON = true;
    }
    delay(500);
  }
  if (set_state==LOW){
    if(btnCount<5){
      btnCount++;
      setupScreen = true;
        if(btnCount==1){
          lcd1.clear();
          lcd2.clear();
          lcd1.setCursor(0,0);
          lcd1.print("-----Setare ORA-----");
          lcd1.setCursor(0,1);
          lcd1.print("-------si DATA------");
          delay(2000);
          lcd1.clear();
        }
    }
    else{
      lcd1.clear();
      lcd2.clear();
      rtc.adjust(DateTime(YY, MM, DD, H, M, 0));
      lcd2.print("Se salveaza....");
      delay(2000);
      lcd1.clear();
      lcd2.clear();
      setupScreen = false;
      btnCount=0;
    }
    delay(500);
  }
}

void getTempTemp(){
  if (currentMillis - previousMillis >= interval) {
  sensors.requestTemperatures();
  temp1 = sensors.getTempC(Thermometer1);
  temp2 = sensors.getTempC(Thermometer2);
  temp3 = sensors.getTempC(Thermometer3);
  temp4 = sensors.getTempC(Thermometer4);
  temp5 = sensors.getTempC(Thermometer5);
  temp6 = sensors.getTempC(Thermometer6);
  temp7 = sensors.getTempC(Thermometer7);
  
  Serial.print(temp1); 
  Serial.println("gr.C----- ");
  Serial.print(temp2);
  Serial.println("gr.C-----");
  Serial.print(temp3);
  Serial.println("gr.C-----");
  Serial.print(temp4);
  Serial.println("gr.C-----");
  Serial.print(temp5);
  Serial.println("gr.C-----");
  Serial.print(temp6);
  Serial.println("gr.C-----");
  Serial.print(temp7);
  Serial.println("gr.C-----");
    previousMillis = currentMillis;    
  }
}


void getTimeDate(){
  if (!setupScreen){
    DateTime now = rtc.now();
    DD = now.day();
    MM = now.month();
    YY = now.year();
    H = now.hour();
    M = now.minute();
    S = now.second();
  }
  //Make some fixes...
  if (DD<10){ sDD = '0' + String(DD); } else { sDD = DD; }
  if (MM<10){ sMM = '0' + String(MM); } else { sMM = MM; }
  sYY=YY;
  if (H<10){ sH = '0' + String(H); } else { sH = H; }
  if (M<10){ sM = '0' + String(M); } else { sM = M; }
  if (S<10){ sS = '0' + String(S); } else { sS = S; }
}


void lcd1Print(){
  lcd1.setCursor(0,0); //randul 1
  lcd1.print(sDD);
  lcd1.print(".");
  lcd1.print(sMM);
  lcd1.print(".");
  lcd1.print(sYY);
  lcd1.print(" ");
  lcd1.print(sH);
  lcd1.print(":");
  lcd1.print(sM);

  lcd1.setCursor(0,1); //randul 2
  lcd1.print("      La noi:");  //  afisare temp1
  lcd1.setCursor(13, 1);
 if (temp1 < 0) 
 {t1=-temp1;}
 else t1=temp1;  
    if (t1 < 10.)
   {
     lcd1.print(" "); 
   }
   if (temp1>0) lcd1.print(" "); 
   if (temp1==0) lcd1.print(" "); 
   if (temp1<0) lcd1.print("-");
   lcd1.print(t1,1);
  lcd1.write(0b11011111);  // simbol de grad
  lcd1.print("C");
  
  lcd1.setCursor(0,2); //randul 3
  lcd1.print("     La Sofi:"); //  afisare temp2
  lcd1.setCursor(13, 2);
 if (temp2 < 0) 
 {t2=-temp2;}
 else t2=temp2;  
    if (t2 < 10.)
   {
     lcd1.print(" "); 
   }
   if (temp2>0) lcd1.print(" "); 
   if (temp2==0) lcd1.print(" "); 
   if (temp2<0) lcd1.print("-");
   lcd1.print(t2,1);
  lcd1.write(0b11011111);  // simbol de grad
  lcd1.print("C");
  
  lcd1.setCursor(0,3); //randul 4
  lcd1.print("       Afara:");  //  afisare temp3
  lcd1.setCursor(13, 3);
 if (temp3 < 0) 
 {t3=-temp3;}
 else t3=temp3;  
    if (t3 < 10.)
   {
     lcd1.print(" "); 
   }
   if (temp3>0) lcd1.print(" "); 
   if (temp3==0) lcd1.print(" "); 
   if (temp3<0) lcd1.print("-");
   lcd1.print(t3,1);
  lcd1.write(0b11011111);  // simbol de grad
  lcd1.print("C");

  lcd2.setCursor(0,0); // rand1 lcd 2
  lcd2.print("      Living:");  //  afisare temp4
  lcd2.setCursor(13, 0);
 if (temp4 < 0) 
 {t4=-temp4;}
 else t4=temp4;  
    if (t4 < 10.)
   {
     lcd2.print(" "); 
   }
   if (temp4>0) lcd2.print(" "); 
   if (temp4==0) lcd2.print(" "); 
   if (temp4<0) lcd2.print("-");
   lcd2.print(t4,1);
  lcd2.write(0b11011111);  // simbol de grad
  lcd2.print("C");

   lcd2.setCursor(0,1); //rand2 lcd 2
   lcd2.print("        Baie:");  //  afisare temp5
   lcd2.setCursor(13, 1);
 if (temp5 < 0) 
 {t5=-temp5;}
 else t5=temp5;  
    if (t5 < 10.)
   {
     lcd2.print(" "); 
   }
   if (temp5>0) lcd2.print(" "); 
   if (temp5==0) lcd2.print(" "); 
   if (temp5<0) lcd2.print("-");
   lcd2.print(t5,1);
  lcd2.write(0b11011111);  // simbol de grad
  lcd2.print("C");

   lcd2.setCursor(0,2); //rand3 lcd 2
   lcd2.print(" La Centrala:");  //  afisare temp6
   lcd2.setCursor(13, 2);
 if (temp6 < 0) 
 {t6=-temp6;}
 else t6=temp6;  
    if (t6 < 10.)
   {
     lcd2.print(" "); 
   }
   if (temp6>0) lcd2.print(" "); 
   if (temp6==0) lcd2.print(" "); 
   if (temp6<0) lcd2.print("-");
   lcd2.print(t6,1);
  lcd2.write(0b11011111);  // simbol de grad
  lcd2.print("C");

   lcd2.setCursor(0,3); //rand4 lcd 2
   lcd2.print("Tur centrala:");  //  afisare temp7
   lcd2.setCursor(13, 3);
 if (temp7 < 0) 
 {t7=-temp7;}
 else t7=temp7;  
    if (t7 < 10.)
   {
     lcd2.print(" "); 
   }
   if (temp7>0) lcd2.print(" "); 
   if (temp7==0) lcd2.print(" "); 
   if (temp7<0) lcd2.print("-");
   lcd2.print(t7,1);
  lcd2.write(0b11011111);  // simbol de grad
  lcd2.print("C");
}


void lcd1Setup(){
  if (btnCount==1){
    lcd1.setCursor(4,0);
    lcd1.print(">"); 
    if (up_state == LOW){
      if (H<23){
        H++;
      }
      else {
        H=0;
      }
      delay(500);
    }
    if (down_state == LOW){
      if (H>0){
        H--;
      }
      else {
        H=23;
      }
      delay(500);
    }
  }
  else if (btnCount==2){
    lcd1.setCursor(4,0);
    lcd1.print(" ");
    lcd1.setCursor(9,0);
    lcd1.print(">");
    if (up_state == LOW){
      if (M<59){
        M++;
      }
      else {
        M=0;
      }
      delay(500);
    }
    if (down_state == LOW){
      if (M>0){
        M--;
      }
      else {
        M=59;
      }
      delay(500);
    }
  }
  else if (btnCount==3){
    lcd1.setCursor(9,0);
    lcd1.print(" ");
    lcd1.setCursor(0,1);
    lcd1.print(">");
    if (up_state == LOW){
      if (DD<31){
        DD++;
      }
      else {
        DD=1;
      }
      delay(500);
    }
    if (down_state == LOW){
      if (DD>1){
        DD--;
      }
      else {
        DD=31;
      }
      delay(500);
    }
  }
  else if (btnCount==4){
    lcd1.setCursor(0,1);
    lcd1.print(" ");
    lcd1.setCursor(5,1);
    lcd1.print(">");
    if (up_state == LOW){
      if (MM<12){
        MM++;
      }
      else {
        MM=1;
      }
      delay(500);
    }
    if (down_state == LOW){
      if (MM>1){
        MM--;
      }
      else {
        MM=12;
      }
      delay(500);
    }
  }
  else if (btnCount==5){
    lcd1.setCursor(5,1);
    lcd1.print(" ");
    lcd1.setCursor(10,1);
    lcd1.print(">");
    if (up_state == LOW){
      if (YY<2999){
        YY++;
      }
      else {
        YY=2000;
      }
      delay(500);
    }
    if (down_state == LOW){
      if (YY>2000){
        YY--;
      }
      else {
        YY=2999;
      }
      delay(500);
    }
  }
  lcd1.setCursor(5,0);
  lcd1.print(sH);
  lcd1.setCursor(8,0);
  lcd1.print(":");
  lcd1.setCursor(10,0);
  lcd1.print(sM);
  lcd1.setCursor(1,1);
  lcd1.print(sDD);
  lcd1.setCursor(4,1);
  lcd1.print("/");
  lcd1.setCursor(6,1);
  lcd1.print(sMM);
  lcd1.setCursor(9,1);
  lcd1.print("/");
  lcd1.setCursor(11,1);
  lcd1.print(sYY);
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Rezolutie biti: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

void printData(DeviceAddress deviceAddress)
{
  Serial.print("Senzor Adresa: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

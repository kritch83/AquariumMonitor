//#define BLYNK_DEBUG Serial1// Optional, this enables lots of prints
//#define BLYNK_PRINT Serial1

#include <SimpleTimer.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>


char auth[] = "4bc75c49c719468aa64650b13d504234";

char ssid[] = "IoT";
char pass[] = "open4meplease";

#define EspSerial Serial
#define ESP8266_BAUD 9600

WidgetLED led1(V7);
WidgetLED led2(V6);
#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

SimpleTimer timer;
ESP8266 wifi(&EspSerial);

const int relayHeat = 5;
const int relayLight = 4;
int timerState;
int lightState;
int buttonState;
int lastState = 0;
int heat = 76;
float temp = 3.0;






void setup()
  {
  timer.setInterval(5000, myTimerEvent);
  timer.setInterval(2000, lightCheck);
  timer.setInterval(300000, heatcall);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  
  sensors.begin();

  pinMode(relayHeat, OUTPUT);
  pinMode(relayLight, OUTPUT);
  
    
  BLYNK_CONNECTED();
  {
  Blynk.syncAll();
  }

  }




void myTimerEvent()
  {
  sensors.requestTemperatures();
  temp = (sensors.getTempCByIndex(0) * 1.8 +32);

    Blynk.virtualWrite(V1, temp);
    Blynk.virtualWrite(V11, heat);
  
  }






void heatcall()
  {
    
  if (temp < heat && temp > 55)
  {
    digitalWrite(relayHeat, HIGH);
    Blynk.virtualWrite(V2, 80);
    led2.on();
  } 
  
  else 
  {
    digitalWrite(relayHeat, LOW);
    Blynk.virtualWrite(V2, 70);
    led2.off();
  }

  if (temp < 55 || temp > 80)
  {
    Blynk.notify("Erronious temp");
  } 
  
    Blynk.syncAll();
}

  







void lightCheck()
{


    lightState = digitalRead(4);
    
    if (lightState == HIGH)
      {
        led1.on();
      }
    else
    {
      led1.off();
    }


    if (timerState != lastState)
    {
      if (timerState == 1)
      {
        digitalWrite(relayLight, HIGH);
        Blynk.virtualWrite(V5, HIGH);
        
        lastState = 1;
        
      }
      
      if (timerState == 0)
      {
        digitalWrite(relayLight, LOW);
        Blynk.virtualWrite(V5, LOW);
        lastState = 0;
        
      }
    }




    if (buttonState == 1)
    {
      digitalWrite(relayLight, HIGH);
    }
    
    if (buttonState == 0)
    {
    digitalWrite(relayLight, LOW);
    } 



}
   










void loop()
{
  Blynk.run();
  timer.run();
}


BLYNK_WRITE(V10)
    {
    heat = param.asInt();
    return heat;
    }

BLYNK_WRITE(V4)
    {
    timerState = param.asInt();
    return timerState;
    }

BLYNK_WRITE(V5)
    {
    buttonState = param.asInt();
    return buttonState;
    }










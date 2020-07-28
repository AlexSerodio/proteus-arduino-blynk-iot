/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  For this example you need UIPEthernet library:
    https://github.com/UIPEthernet/UIPEthernet

  Typical wiring would be (this is for Arduino UNO,
  search for correct wiring for your board):
   VCC -- 5V
   GND -- GND
   CS  -- D10
   SI  -- D11
   SCK -- D13
   SO  -- D12
   INT -- D2

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>

#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4pCU5i2GXHBwGBdRMRvxlWXbkX7WKnZh";

BlynkTimer timer;

int lampState = 0;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, millis() / 1000);

  if(lampState == 0)
  {
    lampState = 1;
    Blynk.virtualWrite(V1, 255);
  }
  else
  {
    lampState = 0;
    Blynk.virtualWrite(V1, 0); 
  }
}

void notificationEvent()
{
  if((analogRead(A1) * (5.0 / 1024.0)) * 100.0 > 30)
  {
    Blynk.notify("A temperatura está muito alta.");
  }
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V0)
{
  int slider = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(slider);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Serial.println(F("DHTxx test!"));
  dht.begin();
  
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(500L, myTimerEvent);
  timer.setInterval(6000L, notificationEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer

  // Wait a few seconds between measurements.
  delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);

  Blynk.virtualWrite(V1, (analogRead(A1) * (5.0 / 1024.0)) * 100.0);

  Serial.println((analogRead(A1) * (5.0 / 1024.0)) * 100.0);
}

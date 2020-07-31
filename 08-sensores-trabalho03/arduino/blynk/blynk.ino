/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <SPI.h>
#include <LiquidCrystal.h>
#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>
#include <DHT.h>
#include <HCSR04.h>
//#include <WidgetRTC.h>
 
//cuidar é o GPIO (numero do pino)
const byte interruptPin = 18;
const byte Lamp = 14;

LiquidCrystal lcd(8, 9, 10,11,12, 13);
//WidgetRTC rtc;


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4pCU5i2GXHBwGBdRMRvxlWXbkX7WKnZh";

 

#define DHTPIN 4          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

HCSR04 hc(17,16); //initialisation class HCSR04 (trig pin , echo pin)
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

WidgetLED led1(V0);
WidgetLED led2(V1);
WidgetLED led3(V2);
WidgetLED led4(V3);


void trigger()
{
  if(digitalRead(A12)) led1.on(); else led1.off();
  if(digitalRead(A13)) led2.on(); else led2.off();
  if(digitalRead(A14)) led3.on(); else led3.off();
  if(digitalRead(A15)) led4.on(); else led4.off();

  Blynk.virtualWrite(V4, digitalRead(A12) + digitalRead(A13) + digitalRead(A14) + digitalRead(A15));

  if(digitalRead(A12) && digitalRead(A13) && digitalRead(A14) && digitalRead(A15))
    Blynk.notify("Cuidado. Sensores atingindo níveis críticos.");
}

void setup()
{
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);

  led1.off();
  led2.off();
  led3.off();
  led4.off();

  Serial.begin(9600);
  
  lcd.begin(20, 2);
  lcd.clear();
  
  Blynk.begin(auth);

  dht.begin();
  timer.setInterval(1000L, trigger);
}

void loop()
{
  Blynk.run();
  timer.run();
//  trigger();
}

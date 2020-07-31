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

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  const float tempExterna = analogRead(A1) * (5.0 / 1024.0)* 100.0;
  String bodyMessage = "Temperatura Interna: " + String(t) + ", Temperatura Externa: " + String(tempExterna);
  Blynk.virtualWrite(V1, bodyMessage);
  Blynk.virtualWrite(V0, bodyMessage);

  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Interna: " + String(t));
  lcd.setCursor(0, 1);
  lcd.print("Temp Externa: " + String(tempExterna));
//  lcd.print(t);
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
int sleepTime = 3;

BLYNK_WRITE(V3)
{
  sleepTime = param.asInt(); // assigning incoming value from pin V1 to a variable
}



//void clockDisplay()
//{
//  // You can call hour(), minute(), ... at any time
//  // Please see Time library examples for details
//
//  String currentTime = String(day()) + "/" + month() + "/" + year() + " " + String(hour()) + ":" + minute() + ":" + second();
//
//  // Send time to the App
//  Blynk.virtualWrite(V0, currentTime);
//}

void Hello()
{
  Serial.println("Hello");
}

void setup()
{
  // Debug console
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Hello, FALLING);
  pinMode(Lamp, OUTPUT);

  pinMode(A15, INPUT);

  Serial.begin(9600);

  //while(1)
  //{
  //  Serial.println(hc.dist());
  //  delay(1000);
  //}
  
  lcd.begin(20, 2);
  lcd.clear();
//  lcd.print("Ola Mundo!!!");
//  lcd.setCursor(0, 1);
//  lcd.print("Miguel");
  
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);

  Blynk.virtualWrite(V3, 3);

  dht.begin();

  // Setup a function to be called every second
//  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  //Serial.println(hc.dist());
  Blynk.run();
  timer.run();
  sendSensor();
  Serial.println(digitalRead(A15));
  delay(sleepTime * 1000L);
}

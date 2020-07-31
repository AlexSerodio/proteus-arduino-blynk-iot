/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <LiquidCrystal.h>
#include <UIPEthernet.h>
#include <BlynkSimpleUIPEthernet.h>
#include <DHT.h>
#include <HCSR04.h>
 
//cuidar é o GPIO (numero do pino)
const byte interruptPin = 18;
const byte Lamp = 14;

LiquidCrystal lcd(8, 9, 10,11,12, 13);


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
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V1, (analogRead(A1) * (5.0 / 1024.0))* 100.0);
  
  lcd.setCursor(10, 1);
  lcd.print(t);
}

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
BLYNK_WRITE(V0)
{
  int slider = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V0 Slider value is: ");
  Serial.println(slider);
  Blynk.notify("Hello!!!!");
}

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

  Serial.begin(9600);

  while(1)
  {
    Serial.println(hc.dist());
    delay(1000);
  }
  
  lcd.begin(20, 2);
  lcd.clear();
  lcd.print("Ola Mundo!!!");
  lcd.setCursor(0, 1);
  lcd.print("Miguel");

  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
   
  //Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }
  
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  //Serial.println(hc.dist());
  //Blynk.run();
  //timer.run();
}

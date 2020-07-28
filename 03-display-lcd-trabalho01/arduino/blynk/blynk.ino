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
#include <LiquidCrystal.h>

#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4pCU5i2GXHBwGBdRMRvxlWXbkX7WKnZh";

BlynkTimer timer;
WidgetTerminal terminal(V3);


//----------------------------------------------------------------------------------------------

double light_level;
bool system_on = false;
bool system_ready = false;
bool internal_state = true;

//----------------------------------------------------------------------------------------------

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void buttonCheckTimer()
{
}

// botão fotossensível
BLYNK_WRITE(V2)
{
  system_ready = param.asInt();

    lcd.clear();
  if (system_ready)
  {
    lcd.println("Ligado");
    terminal.println("Fotossensibilidade ativada");
  }
  else
  {
    lcd.println("Desligado");
    terminal.println("Fotossensibilidade desativada");
  }
  terminal.flush();
}

// botão ligar
BLYNK_WRITE(V1)
{
  if (!system_on)
  {
    system_ready = 1;
    system_on = 1;
    Blynk.virtualWrite(V2, 1);
    terminal.println("Luz ligada");
    terminal.println("Fotossensibilidade ativada");
    terminal.flush();
  }
}

// sensor temperatura
BLYNK_WRITE(V0)
{
  light_level = param.asDouble();
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  lcd.begin(20, 2);

  Serial.println(F("DHTxx test!"));
  dht.begin();
  
  Blynk.begin(auth);

  pinMode(A8, OUTPUT);
  pinMode(A9, INPUT);

  Blynk.virtualWrite(V2, 0);
  lcd.println("Desligado");
  terminal.clear();
  terminal.println("------------------------------------");
  terminal.println("Sistema iniciado");
  terminal.println("Fotossensibilidade desativada");
  terminal.println("------------------------------------");
  terminal.flush();
}

void loop()
{
  Blynk.run();
  //timer.run(); // Initiates BlynkTimer
  
  if (!digitalRead(A9))
  {
    if (internal_state)
    {
      terminal.println("Sistema desabilitado");
      terminal.flush();
      internal_state = false;
    }
    digitalWrite(A8, LOW);
    return;
  }
  if (!internal_state)
    {
      terminal.println("Sistema reabilitado");
      terminal.flush();
      internal_state = true;
    }

  if (!system_ready)
  {
    if (system_on)
    {
      terminal.println("Luz desligada");
      terminal.flush(); 
    }
    system_on = 0;
  }
  else if (light_level < 50)
  {
    if (!system_on)
    {
      terminal.println("Luz ligada");
      terminal.flush(); 
    }
    
    system_on = 1;
  }

  if (system_on)
  {
    digitalWrite(A8, HIGH);
  }
  else
  {
    digitalWrite(A8, LOW);
  }

  delay(333);

  //lcd.setCursor(10, 1);
  //sprintf(texto, "%d&&&&&&%d",h,t);
  //lcd.print(texto);
}

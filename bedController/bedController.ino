#include <Wire.h>
#include <VirtualWire.h>
#include <virtuabotixRTC.h> 
#include "LiquidCrystal.h"

int pins[]={2,3,5,7,6,4};
LiquidCrystal lcd(0);
virtuabotixRTC myRTC(9, 10, 11);
const int transmit_pin=12;
void setupRC()
{
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
}
void setup() 
{
  setupRC();
  for(int i=0;i<7;++i)
  {
    pinMode(pins[i],INPUT_PULLUP);
  }
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(20,4);
  lcd.setBacklight(LOW);
  // seconds, minutes, hours, day of the week, day of the month, month, year
  //myRTC.setDS1302Time(00, 48, 22, 3, 1, 7, 2015);
}
void sendCommandViaRC(int address,int command)
{
  char msg[2] = {address,command};
  vw_send((uint8_t *)msg, 2);
  vw_wait_tx();
}
void closeDoor()
{
  sendCommandViaRC(4,0);
}
void openDoor()
{
  sendCommandViaRC(4,1);
}
void switchLight()
{
  sendCommandViaRC(2,2);
}
void multimediaPlay()
{
  sendCommandViaRC(1,1);
}
void multimediaPause()
{
  sendCommandViaRC(1,0);
}
void multimediaNext()
{
  sendCommandViaRC(1,2);
}
void multimediaPrevious()
{
  sendCommandViaRC(1,3);
}
void multimediaVolumeUp()
{
  sendCommandViaRC(1,5);
}
void multimediaVolumeDown()
{
  sendCommandViaRC(1,4);
}
void multimediaLockComputer()
{
  sendCommandViaRC(1,6);
}
void showTime()
{
  myRTC.updateTime();
  lcd.setCursor(0, 0);
  lcd.print(myRTC.hours);
  lcd.print(":");
  lcd.print(myRTC.minutes);
  lcd.print(":");
  lcd.print(myRTC.seconds);
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print(myRTC.dayofmonth);
  lcd.print("/");
  lcd.print(myRTC.month);
  lcd.print("    ");
  lcd.setCursor(0, 2);
  lcd.print("Temp:");
  lcd.print(GetTemperature());
  lcd.setBacklight(HIGH);
  delay(2000);
  lcd.setBacklight(LOW);
}
double GetTemperature()
{
  Wire.requestFrom(3, 2);
  double temp=-1;
  if (Wire.available())
  {
    int aux1 = Wire.read();
    temp=aux1;  
    //temp+=aux2/100;
  }
  return temp;
}
void loop() {
  for(int i=0;i<7;++i)
  {
    if(digitalRead(pins[i])==0)
    {
      switch(i)
      {
        case 0:
          switchLight();
          break;
        case 1:
          multimediaPause();
          break;
        case 2:
          multimediaPlay();
          break;
        case 3:
          multimediaPrevious();
          break;
        case 4:
          multimediaNext();
          break;
        case 5:
          showTime();
          break;
          
      }
      Serial.println(i);
      delay(300);
    }
  }

}

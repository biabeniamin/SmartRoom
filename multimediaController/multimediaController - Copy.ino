#include <Wire.h>
#include <IRremote.h>
#include <VirtualWire.h>
int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);
const int receive_pin = 6;
decode_results results;
void setupRC()
{
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_rx_start();
}
void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn();
  Wire.begin(1);
  Wire.onReceive(receiveEventI2c);
  setupRC();
}
void closeDoor()
{
  Wire.beginTransmission(4);
  Wire.write(0);
  Wire.endTransmission();
}
void openDoor()
{
  Wire.beginTransmission(4);
  Wire.write(1);
  Wire.endTransmission();
}
void turnOnLight()
{
  Wire.beginTransmission(2);
  Wire.write(1);
  Wire.endTransmission();
}
void turnOffLight()
{
  Wire.beginTransmission(2);
  Wire.write(0);
  Wire.endTransmission();
}
void switchLight()
{
  Wire.beginTransmission(2);
  Wire.write(2);
  Wire.endTransmission();
}
void lockComputer()
{
  Keyboard.begin();
  Keyboard.press(131);
  Keyboard.print("l");
  Keyboard.releaseAll();
}
void checkNewMessagesRC()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen))
  {
    int address=buf[0];
    int command=buf[1];
    Serial.print(address);
    Serial.print(" via rc ");
    Serial.print(command);    
    if(address==1)
    {
      remoteCommand(command);
    }
    else
    {
      Wire.beginTransmission(address);
      Wire.write(command);
      Wire.endTransmission();
    }
  }
}
void loop() {
  checkNewMessagesRC();
  if (irrecv.decode(&results)) 
  {
    switch(results.value)
    {
      case 2878444831:Remote.increase();break;
      case 4198438303:Remote.decrease();break;
      case 2534850111:Remote.mute();break;
      case 1033561079:Remote.next();break;
      case 2388475579:Remote.previous();break;
      case 2318624347:Remote.play();break;
      case 2797888379:Remote.pause();break;
      case 1162296347:closeDoor();break;
      case 2327013275:openDoor();break;
      case 2980520219:turnOnLight();break;
      case 3961599383:turnOffLight();break;
      case 324312031:switchLight();break;
      case 3577243675:lockComputer();break;
    }
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    Remote.clear();
  }
}
void remoteCommand(int command)
{
  switch(command)
  {
    //case 6:lockComputer();break;
    case 2:Remote.next();break;
    case 3:Remote.previous();break;
    case 1:Remote.play();break;
    case 0:Remote.pause();break;
    case 5:Remote.increase();delay(100);break;
    case 4:Remote.decrease();delay(100);break;
    case 7:Remote.mute();break;
  }
  Remote.clear();
}
void receiveEventI2c(int howMany)
{
  int x=Wire.read();
  remoteCommand(x);
}

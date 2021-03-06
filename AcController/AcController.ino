#include <SoftwareSerial.h>
#include <Room.h>
#include <LanCommunication.h>
#include <Log.h>
#include <Timer.h>
#include <IRremote.h>

#define ADDRESS 8
#define TRIGGERED_PIN 2
#define RELAY_PIN 6

SoftwareSerial mySerial(0, 1);
bool state = false;
DWORD startTime;

IRsend *irsend;
Timer *timer;

void writeLan(int byte)
{
  mySerial.write(byte);
}
int readLan()
{
  return mySerial.read();
}
int countLan()
{
  return mySerial.available();
}

LanCommunication lanCom(ADDRESS, TRIGGERED_PIN, &writeLan, &readLan, &countLan);
Room room(&lanCom);
Log lanLog(&lanCom);

int khz = 38; // 38kHz carrier frequency for the NEC protocol
const unsigned int irSignal[] PROGMEM= {656, 17780, 3104, 8864, 572, 424, 572, 1412, 604, 396, 600, 392, 604, 392, 604, 396, 600, 420, 572, 424, 572, 400, 572, 1440, 544, 452, 492, 504, 564, 1420, 576, 424, 576, 420, 600, 1388, 604, 1388, 600, 1388, 604, 1388, 576, 1416, 572, 452, 544, 452, 544, 452, 544, 448, 544, 452, 492, 500, 564, 432, 568, 428, 572, 424, 572, 424, 572, 424, 596, 400, 596, 396, 600, 396, 600, 396, 600, 392, 576, 420, 604, 420, 548, 448, 548, 448, 544, 452, 544, 448, 548, 448, 492, 504, 564, 428, 572, 424, 568, 428, 600, 396, 600, 396, 600, 396, 596, 400, 572, 424, 572, 1412, 604, 1388, 604, 1388, 576, 1420, 592, 2896, 3072, 8900, 492, 1500, 568, 428, 568, 424, 576, 424, 572, 420, 576, 420, 572, 424, 572, 424, 572, 448, 548, 1416, 576, 416, 580, 416, 576, 1416, 600, 400, 596, 1416, 548, 1440, 568, 1424, 596, 1392, 580, 1412, 604, 1388, 576, 420, 576, 420, 596, 428, 568, 424, 548, 448, 548, 448, 548, 448, 544, 448, 548, 476, 520, 476, 460, 536, 508, 484, 540, 456, 544, 452, 544, 452, 544, 452, 544, 452, 544, 448, 572, 420, 576, 420, 576, 420, 576, 420, 548, 448, 548, 448, 548, 448, 544, 452, 544, 476, 520, 476, 464, 532, 512, 480, 540, 456, 544, 452, 544, 452, 544, 452, 544, 452, 544, 452, 540, 2924, 3080, 8864, 600, 1392, 600, 392, 580, 416, 580, 420, 572, 424, 600, 396, 572, 444, 552, 444, 540, 456, 568, 1424, 596, 396, 576, 420, 580, 1412, 576, 420, 572, 1416, 576, 1412, 604, 396, 576, 1412, 600, 1396, 572, 1440, 552, 1436, 572, 1420, 572, 1420, 600, 1392, 572, 1412, 604, 396, 600, 392, 604, 392, 576, 1416, 604, 1388, 576, 1416, 576, 444, 552, 444, 572, 424, 564, 428, 572, 424, 568, 428, 572, 424, 600, 396, 600, 1388, 576, 1412, 580, 416, 580, 1412, 576, 420, 576, 1416, 596, 400, 596, 400, 572, 448, 548, 448, 560, 432, 568, 428, 568, 428, 572, 1416, 604, 1388, 604, 1388, 572, 1416, 600};
const unsigned int irSignalTurnOff[] PROGMEM ={264, 1916, 684, 17780, 3052, 8892, 572, 452, 544, 1420, 572, 448, 548, 448, 544, 452, 544, 452, 544, 452, 544, 476, 520, 476, 464, 1496, 568, 456, 544, 452, 544, 1420, 576, 1412, 572, 452, 544, 1420, 572, 1416, 572, 1420, 572, 1416, 576, 1420, 572, 472, 520, 476, 540, 456, 540, 456, 544, 452, 540, 456, 540, 452, 548, 448, 548, 448, 548, 448, 544, 452, 544, 452, 544, 452, 540, 456, 540, 448, 548, 448, 548, 448, 548, 452, 544, 476, 516, 480, 536, 460, 540, 452, 540, 456, 540, 456, 540, 456, 544, 452, 540, 452, 544, 452, 544, 452, 544, 452, 544, 448, 548, 448, 548, 448, 548, 448, 548, 1412, 604, 1420, 596, 2900, 3048, 8892, 568, 1424, 572, 424, 596, 396, 576, 420, 600, 392, 604, 392, 604, 392, 604, 392, 576, 420, 572, 1416, 576, 424, 600, 396, 572, 1440, 576, 420, 492, 1500, 568, 1424, 572, 1420, 596, 1388, 580, 1412, 576, 1420, 568, 424, 572, 424, 600, 396, 596, 400, 596, 400, 572, 424, 572, 444, 552, 444, 492, 504, 568, 428, 568, 428, 568, 428, 572, 420, 600, 400, 572, 420, 600, 392, 604, 392, 604, 392, 600, 396, 576, 420, 576, 420, 576, 420, 572, 424, 572, 428, 568, 448, 548, 448, 532, 460, 568, 428, 568, 424, 576, 420, 576, 420, 580, 416, 580, 416, 576, 420, 572, 424, 572, 420, 576, 2952, 3048, 8868, 572, 1416, 572, 424, 572, 424, 572, 424, 572, 444, 552, 444, 568, 428, 568, 428, 568, 428, 572, 1420, 572, 424, 596, 400, 572, 1420, 572, 1412, 580, 1412, 576, 1440, 548, 424, 572, 1444, 548, 1444, 560, 1428, 568, 1424, 572, 1416, 576, 1416, 600, 1392, 572, 1416, 576, 420, 576, 420, 576, 420, 576, 1412, 600, 1420, 572, 1416, 560, 432, 568, 428, 576, 420, 572, 424, 572, 424, 580, 416, 572, 424, 572, 424, 572, 1420, 596, 1388, 580, 416, 580, 1440, 548, 424, 568, 1444, 548, 448, 492, 504, 564, 432, 568, 424, 572, 424, 572, 424, 576, 420, 576, 420, 576, 420, 576, 1412, 576, 1416, 600};

void setup()
{
  mySerial.begin(9600);



  timer = Timer::GetInstance();
  timer->SetTimer();
  timer->TurnOn();

  irsend = new IRsend();

  pinMode(RELAY_PIN, OUTPUT);
  turnOff();
}

void turnOn()
{
  digitalWrite(RELAY_PIN, HIGH);
  state = 1;
}
void turnOff()
{
  digitalWrite(RELAY_PIN, LOW);
  state = 0;
}

void sendIrSignal(const uint16_t PROGMEM *signal, int length)
{
  unsigned int irSignal[360];
  for(int i=0;i<length;i++)
  {
    irSignal[i] = pgm_read_word_near(signal + i);
  }
  irsend->sendRaw(irSignal, length, khz);
}

void turnOnAc()
{
  sendIrSignal(irSignal, sizeof(irSignal) / sizeof(irSignal[0]));
  //irsend->sendRaw(irSignal, sizeof(irSignal) / sizeof(irSignal[0]), khz);
  state = 1;
}
void turnOffAc()
{
  sendIrSignal(irSignalTurnOff, sizeof(irSignalTurnOff) / sizeof(irSignalTurnOff[0]));
  //irsend->sendRaw(irSignalTurnOff, sizeof(irSignalTurnOff) / sizeof(irSignalTurnOff[0]), khz);
  state = 0;
}

void loop()
{

  checkSerial();

  /*if ((1 == state)
      && (3000 < timer->GetUpTime()->GetTotalMiliSeconds() - startTime))
  {
    turnOff();
    lanLog.SendLog(51, 5);
  }*/
}
void checkSerial()
{
  if (lanCom.ReadCommand())
  {
    int *command = lanCom.GetLastCommand();
    int interval, time;

    switch (command[2])
    {
      case 0:
        turnOffAc();
        break;
      case 1:
        turnOnAc();
        //startTime = timer->GetUpTime()->GetTotalMiliSeconds();
        //timer->RegisterEvent(command[3] * 1000, turnOff);

        break;
      case 2:
        irsend->sendNEC (0x807E10EF, 32);
        lanLog.AcSwitch(command[1]);
        break;
    }
  }

}

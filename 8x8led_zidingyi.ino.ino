#include <LedControl.h>
#include <Wire.h>
const int DIN = 7;
const int CLK = 9;
const int CS = 8;
LedControl led = LedControl(DIN, CLK, CS , 4);

byte pic[][1]=
  { 0x60,0x98,0x4,0x2,0x1,0x1,0x62,0x9C,
0x0,0x1,0x2,0x4,0x8,0x8,0x4,0x3,
0xFC,0x84,0xB4,0x32,0x1,0xCD,0x2,0xFC,
0x30,0x30,0x0,0x31,0x32,0x32,0x31,0x30,
    };
byte kong[][1]{0x00};
void showWordByPtr(LedControl led, byte (*word)[1]);
void showpic();
void setup() {

  for (int i = 0; i < 4; ++i)
  {
    led.shutdown(i, false);
    led.clearDisplay(i);
    led.setIntensity(i,2);
  }
}

void loop() {
  
  showpic();
  
}

void showWordByPtr(LedControl led, byte (*word)[1])
{ int temp1=0;
  for (int i = 0; i < 4; ++i)
  {
    
    for (int j = 0; j < 8; ++j)
      {
        led.setRow(i, j, *(*(word + j)+temp1));
      }
      temp1 +=8;
  }
  
}

void showpic(){
      showWordByPtr(led, pic);
}

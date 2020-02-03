//---------------------------------------------------------------
//抖音：爆改车间主任
//AIDA64电脑信息显示外置LED点阵屏
//这个是使用ds1302时钟模块的代码
//共享代码请勿商用
//---------------------------------------------------------------
#include <LedControl.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#define light 5
#define Temp 4
#define date 3
#define Info 2
#define countof(a) (sizeof(a) / sizeof(a[0]))

int light_level=2;

const int DIN = 7;
const int CLK = 9;
const int CS = 8;
int Temp1, Temp2,Temp3,Temp4;

ThreeWire myWire(11,10,12); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
char datestring[20];

bool Century=false,cls;
byte Temperature;
int col;
int row;     //纵向移动数组的行数
byte day[13][8][1];
LedControl led = LedControl(DIN, CLK, CS , 4);

//-------
char frame1[32] = {}; 
String S1; //用于存放上面数组的数便于输出
byte inByte; //用于存放数据帧的第一位字符
int Info_key=0;
//-------

byte n0[8]={0x7E,0xFF,0xE7,0xE7,0xE7,0xE7,0xFF,0x7E};//0
byte n1[8]={0x38,0x38,0x38,0x38,0x38,0x3E,0x3C,0x38};//1
byte n2[8]={0xFF,0xFF,0x1C,0x38,0x70,0xE7,0xFF,0x7E};//2
byte n3[8]={0x7F,0xFF,0xE0,0xFF,0xFF,0xE0,0xFF,0x7F};//3
byte n4[8]={0x70,0x70,0x70,0xFF,0xFF,0x77,0x77,0x77};//4
byte n5[8]={0x7E,0xFF,0xE0,0xFF,0x7F,0x7,0xFF,0xFF};//5
byte n6[8]={0x7E,0xFF,0xE7,0xFF,0x7F,0x7,0xFF,0x7E};//6
byte n7[8]={0x7,0xE,0x1C,0x38,0x70,0xE0,0xFF,0xFF};//7
byte n8[8]={0x7E,0xFF,0xE7,0x7E,0x7E,0xE7,0xFF,0x7E};//8
byte n9[8]={0x7E,0xFF,0xE0,0xFE,0xFF,0xE7,0xFF,0x7E};//9


byte num[10][8]={
  {0x7E,0xFF,0xE7,0xE7,0xE7,0xE7,0xFF,0x7E},
  {0x38,0x38,0x38,0x38,0x38,0x3E,0x3C,0x38},
  {0xFF,0xFF,0x1C,0x38,0x70,0xE7,0xFF,0x7E},
  {0x7F,0xFF,0xE0,0xFF,0xFF,0xE0,0xFF,0x7F},
  {0x70,0x70,0x70,0xFF,0xFF,0x77,0x77,0x77},
  {0x7E,0xFF,0xE0,0xFF,0x7F,0x7,0xFF,0xFF},
  {0x7E,0xFF,0xE7,0xFF,0x7F,0x7,0xFF,0x7E},
  {0x7,0xE,0x1C,0x38,0x70,0xE0,0xFF,0xFF},
  {0x7E,0xFF,0xE7,0x7E,0x7E,0xE7,0xFF,0x7E},
  {0x7E,0xFF,0xE0,0xFE,0xFF,0xE7,0xFF,0x7E}
};
byte num_L[10][8]={
  {0x0,0xFF,0xFF,0xC3,0xC3,0xFF,0xFF,0x0},
  {0x0,0x4,0x6,0xFF,0xFF,0xFF,0x0,0x0},
  {0x0,0xFB,0xFB,0xDB,0xDB,0xDF,0xDF,0x0},
  {0x0,0xDB,0xDB,0xDB,0xDB,0xFF,0xFF,0x0},
  {0x0,0x1F,0x1F,0x18,0x18,0xFF,0xFF,0x0},
  {0x0,0xDF,0xDF,0xDB,0xDB,0xFB,0xFB,0x0},
  {0x0,0xFF,0xFF,0xDB,0xDB,0xFB,0xFB,0x0},
  {0x0,0x3,0x3,0x3,0x3,0xFF,0xFF,0x0},
  {0x0,0xFF,0xFF,0xDB,0xDB,0xFF,0xFF,0x0},
  {0x0,0xDF,0xDF,0xDB,0xDB,0xFF,0xFF,0x0}
};

byte num_L_s[15][4]={
{0x7F,0x41,0x7F,0x0},{0x0,0x7F,0x0,0x0},
{0x79,0x49,0x4F,0x0},{0x49,0x49,0x7F,0x0},
{0xF,0x8,0x7F,0x0},{0x4F,0x49,0x79,0x0},
{0x7F,0x49,0x79,0x0},{0x1,0x1,0x7F,0x0},
{0x7F,0x49,0x7F,0x0},{0x4F,0x49,0x7F,0x0},
{0x7F,0x41,0x41,0x63},{0x7F,0x41,0x51,0x71},
{0x2,0x7C,0x44,0x6C},
{0x7F,0x19,0x29,0x4F},{0x24,0x10,0x8,0x24},

};  //10(C),11(G),12(摄氏度),13(R),14(%)

byte leftWords[][1]=
  {0x7E,0xFF,0xE7,0xE7,0xE7,0xE7,0xFF,0x7E};


byte zichuan[][8][1]={
  {0x7E,0xFF,0xFF,0xC3,0xC3,0xFF,0xFF,0x7E},
  {0xDB,0xDB,0xDB,0xDB,0xDB,0xFF,0xFF,0x7E},
  {0xC6,0xC7,0xE7,0xF7,0xFF,0xDF,0xCF,0xC7},
  {0x4,0x6,0xC7,0xFF,0xFF,0xFF,0xC0,0x0},
  
};

byte eye1[][1]=
  { 0x0,0x0,0x0,0xFC,0xFF,0xFC,0x0,0x0,
    0x0,0x0,0x0,0x3F,0xFF,0x3F,0x0,0x0,
    0x0,0x0,0x0,0xFC,0xFF,0xFC,0x0,0x0,
    0x0,0x0,0x0,0x3F,0xFF,0x3F,0x0,0x0,
    };
byte eye2[][1]=
{ 0x0,0x80,0xC0,0xC0,0x83,0xE,0xFC,0xE0,
  0x0,0x1,0x3,0x3,0xC1,0x70,0x3F,0x7,
  0x0,0x80,0xC0,0xC0,0x83,0xE,0xFC,0xE0,
  0x0,0x1,0x3,0x3,0xC1,0x70,0x3F,0x7,
};
byte eye3[][1]=
  { 0x0,0xF0,0x78,0x38,0x38,0x78,0xF0,0x0,
    0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
    0x0,0xC0,0xE0,0xE0,0xE0,0xE0,0xC0,0x0,
    0x0,0xF,0x1D,0x18,0x18,0x1D,0xF,0x0,
    };

byte eye4[][1]=
  { 0x0,0xF0,0x38,0x18,0x18,0x38,0xF0,0x0,
0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
0x0,0xC0,0xE0,0x60,0x60,0xE0,0xC0,0x0,
0x0,0xF,0x1C,0x18,0x18,0x1C,0xF,0x0,
    };

byte eye5[][1]=
  { 0x0,0xF0,0xD8,0x88,0x88,0xD8,0xF0,0x0,
0x0,0x3,0x7,0x7,0x7,0x7,0x3,0x0,
0x0,0xC0,0x60,0x20,0x20,0x60,0xC0,0x0,
0x0,0xF,0x1F,0x1E,0x1E,0x1F,0xF,0x0,
    };

byte eye6[][1]=
  { 0x0,0x0,0xF0,0x18,0x18,0xF0,0x0,0x0,
0x0,0x0,0x3,0x7,0x7,0x3,0x0,0x0,
0x0,0x0,0xC0,0x60,0x60,0xC0,0x0,0x0,
0x0,0x0,0xF,0x1C,0x1C,0xF,0x0,0x0,
    };

byte eye7[][32][1]={
    {0x0,0xF0,0x78,0x38,0x38,0x78,0xF0,0x0,
    0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
    0x0,0xC0,0xE0,0xE0,0xE0,0xE0,0xC0,0x0,
    0x0,0xF,0x1D,0x18,0x18,0x1D,0xF,0x0,},
    {0x0,0xF0,0x38,0x18,0x18,0x38,0xF0,0x0,
    0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
    0x0,0xC0,0xE0,0x60,0x60,0xE0,0xC0,0x0,
    0x0,0xF,0x1C,0x18,0x18,0x1C,0xF,0x0,},
    {0x0,0xF0,0xD8,0x88,0x88,0xD8,0xF0,0x0,
    0x0,0x3,0x7,0x7,0x7,0x7,0x3,0x0,
    0x0,0xC0,0x60,0x20,0x20,0x60,0xC0,0x0,
    0x0,0xF,0x1F,0x1E,0x1E,0x1F,0xF,0x0,},
    {0x0,0x0,0xF0,0x18,0x18,0xF0,0x0,0x0,
    0x0,0x0,0x3,0x7,0x7,0x3,0x0,0x0,
    0x0,0x0,0xC0,0x60,0x60,0xC0,0x0,0x0,
    0x0,0x0,0xF,0x1C,0x1C,0xF,0x0,0x0,},
    {0x0,0x0,0x0,0xF8,0xF8,0x0,0x0,0x0,
    0x0,0x0,0x0,0x7,0x7,0x0,0x0,0x0,
    0x0,0x0,0x0,0xE0,0xE0,0x0,0x0,0x0,
    0x0,0x0,0x0,0x1F,0x1F,0x0,0x0,0x0,},
    {
      0x0,0x0,0x0,0xF8,0xF8,0x0,0x0,0x0,
0x0,0x0,0x0,0x7,0x7,0x0,0x0,0x0,
0x0,0x0,0x0,0xE0,0xE0,0x0,0x0,0x0,
0x0,0x0,0x0,0x1F,0x1F,0x0,0x0,0x0,
    },
    {
      0x0,0x0,0xF0,0x38,0x38,0xF0,0x0,0x0,
0x0,0x0,0x3,0x7,0x7,0x3,0x0,0x0,
0x0,0x0,0xC0,0xE0,0xE0,0xC0,0x0,0x0,
0x0,0x0,0xF,0x1C,0x1C,0xF,0x0,0x0,
    },
    {
     0x0,0xF0,0x38,0x18,0x18,0x38,0xF0,0x0,
0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
0x0,0xC0,0xE0,0x60,0x60,0xE0,0xC0,0x0,
0x0,0xF,0x1C,0x18,0x18,0x1C,0xF,0x0,
    },
    {
    0x0,0xF0,0xF8,0xD8,0x18,0xF8,0xF0,0x0,
0x0,0x3,0x7,0x6,0x6,0x7,0x3,0x0,
0x0,0xC0,0xE0,0x60,0x60,0xE0,0xC0,0x0,
0x0,0xF,0x1F,0x1B,0x18,0x1F,0xF,0x0,},
{
  0x0,0xF0,0xF8,0xE8,0xE8,0x8,0xF0,0x0,
0x0,0x3,0x7,0x5,0x5,0x4,0x3,0x0,
0x0,0xC0,0xE0,0xA0,0xA0,0x20,0xC0,0x0,
0x0,0xF,0x1F,0x17,0x17,0x10,0xF,0x0,
},{
0x0,0xF0,0xE8,0xE8,0x8,0x18,0xF0,0x0,
0x0,0x3,0x5,0x5,0x4,0x6,0x3,0x0,
0x0,0xC0,0xA0,0xA0,0x20,0x60,0xC0,0x0,
0x0,0xF,0x17,0x17,0x10,0x18,0xF,0x0,
}
};



byte T[8]={0x18,0x18,0x18,0xFF,0xFF,0x18,0x18,0x18};
byte C[8]={0x78,0x84,0x4,0x4,0x84,0x7F,0x5,0x7};
byte nian[8]={0x24,0x3A,0x2B,0xFE,0xFE,0x2A,0x2A,0x20};
byte yue[8]={0xC0,0xFF,0xFF,0x29,0x29,0xFF,0xFF,0x0};
byte ri[8]={0x0,0xFF,0x91,0x91,0x91,0x91,0xFF,0x0};
byte CPU[8]={0xFF,0xC3,0xBD,0xBD,0xBD,0xBD,0xC3,0xFF};
byte GPU[8]={0x7E,0x81,0xBD,0xB5,0x85,0xBD,0x81,0x7E};
int ge,shi,bai,qian,wan,shiwan,baiwan,qianwan;
void showTopMoveWords(LedControl led, byte(*words)[8][1]);
void showWordByPtr(LedControl led, byte (*word)[1]);
void showLeftMoveWords(LedControl led, byte(*words)[8][1]); //向左移动显示文字
void showTimtNow();
void showeye();

void showDay();
void showInfoCpu();
void showInfoGpu();
void showNum(unsigned long x);
void showInfoCpuGpu();
void showInfoCpuRam();
void GetDateStuff(byte& Year, byte& Month, byte& Day, byte& DoW, 
    byte& Hour, byte& Minute, byte& Second);
void setup() {
  Serial.begin(9600);
    pinMode(light, INPUT_PULLUP);
    pinMode(Temp, INPUT_PULLUP);
    pinMode(date,INPUT_PULLUP);
    pinMode(Info,INPUT_PULLUP);
  // put your setup code here, to run once:
  for (int i = 0; i < 4; ++i)
  {
    led.shutdown(i, false);//关闭省电模式，这一步必须做，否则显示不出来(因为在构造函数中，默认开启了省电模式，所以，我们要关闭该模式)。
    led.clearDisplay(i);//清空数据
    led.setIntensity(i,2);
  }
  col = 8 * (sizeof(day) / sizeof(*day));
  row = 8 * (sizeof(zichuan) / sizeof(*zichuan));//计算向上移动的字的总行数

  //----对时程序
      Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }


  
}

void loop() {
    if(digitalRead(light)==LOW){
              light_level++;
              if(light_level>11)light_level=0;
              for (int i = 0; i < 4; ++i)
           {
              led.setIntensity(i,light_level);}
              
              Serial.print(light_level);
              while(digitalRead(light)==LOW);
    }
    
    if(digitalRead(Temp)==LOW){
         //showTemperature();
         delay(5000);
         while(digitalRead(Temp)==LOW);
    }
    if(digitalRead(date)==LOW){
         showDay();
         delay(500);
         while(digitalRead(date)==LOW);
    }
//----------
    if(digitalRead(Info)==LOW){
      Info_key++;
      cls=1;
      
      if(Info_key==6)Info_key=0;
         
         while(digitalRead(Info)==LOW);
    }
    if(Info_key==0){
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now); 
      showTimtNow();
          if(random(300)==1)showeye();
    }else if (Info_key==1){
      showInfoCpu();
    }else if (Info_key==2){
      showInfoGpu();
     // if(Serial.available()<=0)Info_key=!Info_key;
    }else if (Info_key==3){
      showInfoCpuGpu();
    }else if (Info_key==4){
      showInfoCpuRam();
    }



//-----------
    
     //   showTopMoveWords(led, day); //字向右滚动
//  delay(1000);
 //    showLeftMoveWords(led, day); //字向右滚动

      
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

void showLeftMoveWords(LedControl led, byte(*words)[8][1])
{
    byte* first = *(*words);
    byte* temp = first; 
    for (int n = 0; n < col - 32; ++n)
    {
        temp = first;
        for (int i=0;i<4;++i)
        {

                for (int j = 0; j < 8; ++j)
                {
                    led.setColumn(i, 7-j, *temp);
                    temp++;
                }
            
        }

            first++;

        //delay(1000);
    }
}

void showTimtNow(){
    Temp1=datestring[11]-48;
    Temp2=datestring[12]-48;
      for(int i=0;i<8; i++){
        led.setRow(0,i,num[Temp1][i]);
      }
      for(int i=0;i<8; i++){
        led.setRow(1,i,num[Temp2][i]);
      }

    Temp1=datestring[14]-48;
    Temp2=datestring[15]-48;
      for(int i=0;i<8; i++){
        led.setRow(2,i,num[Temp1][i]);
      }
      for(int i=0;i<8; i++){
        led.setRow(3,i,num[Temp2][i]);
      }


}
void showeye(){
/*    showWordByPtr(led, eye3);
    delay(200);
    showWordByPtr(led, eye4);
    delay(200);
    showWordByPtr(led, eye5);
    delay(200);
    showWordByPtr(led, eye6);
    delay(1000);*/

    for(int i = 0;i<10;i++){
      showWordByPtr(led, eye7[i]);
      delay(100);
    }
      showWordByPtr(led, eye7[9]);
      delay(200);
      showWordByPtr(led, eye7[10]);
      delay(200);
      showWordByPtr(led, eye7[9]);
      delay(200);
      showWordByPtr(led, eye7[10]);
      delay(200);
      showWordByPtr(led, eye7[9]);
      delay(200);
}



void showDay(){
      for(int i=0;i<8;i++){
      day[0][i][1]=0x00;
    }
        for(int i=0;i<8;i++){
      day[1][i][1]=num_L[2][i];
    }
        for(int i=0;i<8;i++){
      day[2][i][1]=num_L[0][i];
    }

    Temp1=datestring[8]-48;
    Temp2=datestring[9]-48;
        for(int i=0;i<8;i++){
      day[3][i][1]=num_L[Temp1][i];
    }
        for(int i=0;i<8;i++){
      day[4][i][1]=num_L[Temp2][i];
    }
        for(int i=0;i<8;i++){
      day[5][i][1]=nian[i];
    }

    Temp1=datestring[0]-48;
    Temp2=datestring[1]-48;
        for(int i=0;i<8;i++){
      day[6][i][1]=num_L[Temp1][i];
    }
        for(int i=0;i<8;i++){
      day[7][i][1]=num_L[Temp2][i];
    }
        for(int i=0;i<8;i++){
      day[8][i][1]=yue[i];
    }

    Temp1=datestring[3]-48;
    Temp2=datestring[4]-48;
        for(int i=0;i<8;i++){
      day[9][i][1]=num_L[Temp1][i];
    }
        for(int i=0;i<8;i++){
      day[10][i][1]=num_L[Temp2][i];
    }
    for(int i=0;i<8;i++){
      day[11][i][1]=ri[i];
    }
    for(int i=0;i<8;i++){
      day[12][i][1]=0x00;
    }
        
     showLeftMoveWords(led, day); //字向右滚动
}
void showTopMoveWords(LedControl led, byte(*words)[8][1])
{
      byte *temp = *(*words); //获取第一个二维数组的第一行的(第一个元素)的指针
      byte* dump = temp;
        for(int n =0;n<=row-8;++n)
  {
    dump = temp;
    for (int i = 0; i < 4; ++i)//共4块8*8led点阵，每块显示完再到下一块
    {
      dump = temp;
      for (int j = 0; j < 8; ++j)  //每块点阵共8行，所以循环8次
        {
          led.setRow(i, j, *(dump));
          dump+=1;  //指向下一行的第一个位置
        }
    }
        temp += 1;//每次循环完，指向下一行。
    delay(100);
  }
}

void showInfoCpu(){

        if(cls==1){
            for (int i = 0; i < 4; ++i)
                {
                      led.clearDisplay(i);//清空数据
                }
             cls=0;
      }


  
      for(int i=0;i<8; i++){
        led.setRow(0,i,CPU[i]);
      }  
      for(int i=0;i<8; i++){
        led.setRow(3,i,C[i]);
      }  
     if(Serial.available()>0)
  {

    delay(100);
     while (Serial.available() > 0) {
      inByte = Serial.read();
      if(inByte==0x43)  //判断是不是字母C，找到数据帧起点
      { 
        frame1[0] = inByte;
        S1 = frame1[0];
        for(int i = 1; i < 32; i++)
        {
          while(Serial.available()==0);
          frame1[i] = Serial.read(); 
          S1 += frame1[i];
        }

       Temp1 = frame1[2];
       Temp2 = frame1[3];
             
        for(int i=0;i<8; i++){
          led.setRow(1,i,num[Temp1-48][i]);
        }
       // Serial.print(frame1[2]);
      //  Serial.print(" Temp1:");
       // Serial.println(Temp1);
        for(int i=0;i<8; i++){
          led.setRow(2,i,num[Temp2-48][i]);
        }
       // Serial.print(frame1[3]);
      //  Serial.print(" Temp2:");
      //  Serial.println(Temp2);
      
      }
      
     }
  }
  
}
void showInfoGpu(){
        if(cls==1){
            for (int i = 0; i < 4; ++i)
                {
                      led.clearDisplay(i);//清空数据
                }
             cls=0;
      }
        for(int i=0;i<8; i++){
        led.setRow(0,i,GPU[i]);
      }
            for(int i=0;i<8; i++){
        led.setRow(3,i,C[i]);
     if(Serial.available()>0)
  {
    delay(100);
     while (Serial.available() > 0) {
      inByte = Serial.read();
      if(inByte==0x43)  //判断是不是字母C，找到数据帧起点
      { 
        frame1[0] = inByte;
        S1 = frame1[0];
        for(int i = 1; i < 32; i++)
        {
          while(Serial.available()==0);
          frame1[i] = Serial.read(); 
          S1 += frame1[i];
        }

       Temp1 = frame1[7];
       Temp2 = frame1[8];
               
        for(int i=0;i<8; i++){
          led.setRow(1,i,num[Temp1-48][i]);
        }
       // Serial.print(frame1[2]);
      //  Serial.print(" Temp1:");
       // Serial.println(Temp1);
        for(int i=0;i<8; i++){
          led.setRow(2,i,num[Temp2-48][i]);
        }
       // Serial.print(frame1[3]);
      //  Serial.print(" Temp2:");
      //  Serial.println(Temp2);

      }        
      }
      
     }
  }
  
}

void showNum(unsigned long x){
    ge=x%10;
  shi=x%100/10;
  bai=x%1000/100;
  qian=x%10000/1000;
  wan=x%100000/10000;
  shiwan=x%1000000/100000;
  baiwan=x%10000000/1000000;
  qianwan=x/10000000;

  for(int i=0;i<4; i++){
    led.setColumn(0,7-i,num_L_s[qianwan][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(0,3-i,num_L_s[baiwan][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(1,7-i,num_L_s[shiwan][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(1,3-i,num_L_s[wan][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(2,7-i,num_L_s[qian][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(2,3-i,num_L_s[bai][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(3,7-i,num_L_s[shi][i]);
  }
  for(int i=0;i<4; i++){
    led.setColumn(3,3-i,num_L_s[ge][i]);
  }

 delay(5);
 
 Serial.println(x);
}

void showInfoCpuGpu(){
        if(cls==1){
            for (int i = 0; i < 4; ++i)
                {
                      led.clearDisplay(i);//清空数据
                }
             cls=0;
      }
        for(int i=0;i<4; i++){
          led.setColumn(0,7-i,num_L_s[10][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(1,3-i,num_L_s[12][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(2,7-i,num_L_s[11][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(3,3-i,num_L_s[12][i]);
        }



        
  
     if(Serial.available()>0)
  {
    delay(100);
     while (Serial.available() > 0) {
      inByte = Serial.read();
      if(inByte==0x43)  //判断是不是字母C，找到数据帧起点
      { 
        frame1[0] = inByte;
        S1 = frame1[0];
        for(int i = 1; i < 32; i++)
        {
          while(Serial.available()==0);
          frame1[i] = Serial.read(); 
          S1 += frame1[i];
        }


       if(frame1[3]==0x20){
       Temp1 = 48;
       Temp2 = frame1[2];
       }else{
       Temp1 = frame1[2];
       Temp2 = frame1[3];}

      
       if(frame1[8]==0x20){
       Temp3 = 48;
       Temp4 = frame1[7];
       }else{
       Temp3 = frame1[7];
       Temp4 = frame1[8];}

     

        for(int i=0;i<4; i++){
          led.setColumn(0,3-i,num_L_s[Temp1-48][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(1,7-i,num_L_s[Temp2-48][i]);
        }

        for(int i=0;i<4; i++){
          led.setColumn(2,3-i,num_L_s[Temp3-48][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(3,7-i,num_L_s[Temp4-48][i]);
        }

       }
     }
  }
  
}

void showInfoCpuRam(){
        if(cls==1){
            for (int i = 0; i < 4; ++i)
                {
                      led.clearDisplay(i);//清空数据
                }
             cls=0;
      }
        for(int i=0;i<4; i++){
          led.setColumn(0,7-i,num_L_s[10][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(1,3-i,num_L_s[14][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(2,7-i,num_L_s[13][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(3,3-i,num_L_s[14][i]);
        }

        
     if(Serial.available()>0)
  {
    delay(100);
     while (Serial.available() > 0) {
      inByte = Serial.read();
      if(inByte==0x43)  //判断是不是字母C，找到数据帧起点
      { 
        frame1[0] = inByte;
        S1 = frame1[0];
        for(int i = 1; i < 32; i++)
        {
          while(Serial.available()==0);
          frame1[i] = Serial.read(); 
          S1 += frame1[i];
        }

       if(frame1[13]==0x20){
       Temp1 = 48;
       Temp2 = frame1[12];
       }else{
       Temp1 = frame1[12];
       Temp2 = frame1[13];}
       
       if(frame1[18]==0x20){
       Temp3 = 48;
       Temp4 = frame1[17];
       }else{
       Temp3 = frame1[17];
       Temp4 = frame1[18];}

       Serial.print(frame1[18]);
       Serial.print(" Temp3:");
       Serial.println(Temp3);
       Serial.print(frame1[19]);
       Serial.print(" Temp4:");
       Serial.println(Temp4);
            

        for(int i=0;i<4; i++){
          led.setColumn(0,3-i,num_L_s[Temp1-48][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(1,7-i,num_L_s[Temp2-48][i]);
        }

        for(int i=0;i<4; i++){
          led.setColumn(2,3-i,num_L_s[Temp3-48][i]);
        }
        for(int i=0;i<4; i++){
          led.setColumn(3,7-i,num_L_s[Temp4-48][i]);
        }

       }
     }
  }
  
}


void printDateTime(const RtcDateTime& dt)
{
    

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    //Serial.print(datestring);
}

/*

  PovShakeStickKit
  My annotation of the example code originally written by Zheng Zhong Xing 兴向荣 (aka zhengzhongxing39)
  Original source obtained from http://pan.baidu.com/share/link?shareid=532839&uk=1161248057
  Licence "unknown" but appears to have been released into the public domain.

  For more info and circuit diagrams see https://github.com/tardate/LittleArduinoProjects/tree/master/Electronics101/PovShakeStickKit

 */

#include <AT89X52.h>
#define uchar unsigned char
#define uint unsigned int   //宏定义
#define KEY P3_0            //定义画面切换按键 Definition screen switching button
                            // The input pin that the display selector switch is attached to.
                            // Note that it is declared as P3_0 here. In the product kits I've seen,
                            // it is actually on P3_7


uchar KY;             //KY作用在后面说明 explained later. KY tracks the back and forth swing interrupt counts
uchar disp;           //显示汉字指针  characters display pointer
uchar pic=0,num=0;    //pic为按键次数；num为中断次数
                      // pic for the number of keys
                      // num is used to count interrupts



/*
  byte arrays define the various display messages possible
*/
uchar code love[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x00,0x20,0x00,0x20,0x00,0x20,
0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x0F,
0x04,0x10,0x02,0x20,0x02,0x20,0x02,0x20,0x02,0x20,0x04,0x10,0xF8,0x0F,0x00,0x00,
0x00,0x00,0x00,0x00,0xFE,0x07,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x10,
0x00,0x08,0xFE,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x3F,0x82,0x20,0x82,0x20,
0x82,0x20,0x82,0x20,0x82,0x20,0x82,0x20,0x82,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*LOVE*/
};

uchar code loveyou[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x78,0x00,0xFC,0x00,0xFE,0x01,0xFE,0x03,0xFE,0x07,0xFE,0x0F,0xFE,0x1F,0xFC,0x3F,
0xF8,0x7F,0xFC,0x3F,0xFE,0x1F,0xFE,0x0F,0xFE,0x07,0xFE,0x03,0xFE,0x01,0xFC,0x00,
0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*心形图案*/
};

uchar code hehe[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0x40,0x01,0xC0,0x01,0x00,0x00,0x00,0x00,
0xF0,0x0F,0x08,0x10,0x04,0x20,0x00,0x00,0x00,0x00,0xF0,0x3F,0x08,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x08,0x00,0xF0,0x3F,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x20,
0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x00,0xF0,0x3F,0x08,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x08,0x00,0xF0,0x3F,0x00,0x00,0x00,0x00,0x04,0x20,0x08,0x10,
0xF0,0x0F,0x00,0x00,0x00,0x00,0xC0,0x01,0x40,0x01,0xC0,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*呵呵o(∩_∩)o图案*/
};


//  欢迎使用神奇魔幻摇摇棒！Welcome to the magical fantasy shake stick!
unsigned char code hanzi[] = {
//-- 欢 --
      0x04,0x10,0x34,0x08,0xC4,0x06,0x04,0x01,
      0xC4,0x82,0x3C,0x8C,0x20,0x40,0x10,0x30,
      0x0F,0x0C,0xE8,0x03,0x08,0x0C,0x08,0x10,
      0x28,0x60,0x18,0xC0,0x00,0x40,0x00,0x00,
//-- 迎 --
      0x40,0x00,0x42,0x40,0x44,0x20,0xC8,0x1F,
      0x00,0x20,0xFC,0x47,0x04,0x42,0x02,0x41,
      0x82,0x40,0xFC,0x7F,0x04,0x40,0x04,0x42,
      0x04,0x44,0xFE,0x63,0x04,0x20,0x00,0x00,
//-- 使 --
      0x40,0x00,0x20,0x00,0xF8,0xFF,0x07,0x00,
      0x04,0x80,0xF4,0x43,0x14,0x45,0x14,0x29,
      0x14,0x19,0xFF,0x17,0x14,0x21,0x14,0x21,
      0x14,0x41,0xF6,0xC3,0x04,0x40,0x00,0x00,
//-- 用 --
      0x00,0x80,0x00,0x60,0xFE,0x1F,0x22,0x02,
      0x22,0x02,0x22,0x02,0x22,0x02,0xFE,0x7F,
      0x22,0x02,0x22,0x02,0x22,0x42,0x22,0x82,
      0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,
//-- 神 --
      0x08,0x01,0x88,0x00,0x49,0x00,0xEE,0xFF,
      0x58,0x00,0x88,0x00,0x00,0x00,0xF8,0x1F,
      0x88,0x08,0x88,0x08,0xFF,0xFF,0x88,0x08,
      0x88,0x08,0xFC,0x1F,0x08,0x00,0x00,0x00,
//-- 奇 --
      0x40,0x00,0x40,0x00,0x44,0x00,0x44,0x3E,
      0x64,0x12,0x54,0x12,0x4C,0x12,0x47,0x12,
      0x4C,0x3F,0x54,0x42,0x74,0x80,0xC6,0x7F,
      0x44,0x00,0x60,0x00,0x40,0x00,0x00,0x00,
//-- 魔 --
      0x00,0x40,0x00,0x30,0xFE,0x8F,0x4A,0x80,
      0xAA,0x5F,0x9A,0x4A,0xFE,0x2A,0xAA,0x1A,
      0xCB,0x0F,0xAA,0x7A,0xFE,0x8A,0x9A,0xAA,
      0xAA,0x8F,0x6B,0x80,0x22,0xE0,0x00,0x00,
//-- 幻 --
      0x80,0x20,0xC0,0x30,0xA0,0x28,0x98,0x24,
      0x87,0x22,0x80,0x21,0xC4,0x30,0x04,0x60,
      0x04,0x00,0x04,0x20,0x04,0x40,0x04,0x80,
      0x04,0x40,0xFE,0x3F,0x04,0x00,0x00,0x00,
//-- 摇 --
      0x10,0x02,0x10,0x42,0x10,0x81,0xFF,0x7F,
      0x90,0x04,0x54,0x05,0xCC,0xF4,0xB4,0x44,
      0x84,0x44,0xBC,0x7F,0x82,0x44,0xA2,0x44,
      0x9B,0xF4,0x82,0x06,0x00,0x04,0x00,0x00,
//-- 摇 --
      0x10,0x02,0x10,0x42,0x10,0x81,0xFF,0x7F,
      0x90,0x04,0x54,0x05,0xCC,0xF4,0xB4,0x44,
      0x84,0x44,0xBC,0x7F,0x82,0x44,0xA2,0x44,
      0x9B,0xF4,0x82,0x06,0x00,0x04,0x00,0x00,
//-- 棒 --
      0x10,0x04,0x10,0x03,0xD0,0x00,0xFF,0xFF,
      0x90,0x00,0x54,0x05,0x44,0x12,0xD4,0x15,
      0x74,0x14,0x5F,0xFF,0xD4,0x14,0x54,0x15,
      0x56,0x12,0x44,0x06,0x40,0x02,0x00,0x00,
//-- ！ --
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x7C,0x10,0xFE,0x3B,
      0xFE,0x3B,0x7C,0x10,0x00,0x00,0x00,0x00,
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

/*****函数声明*****/
void display1(void);
void display2(void);
void display3(void);
void display4(void);

/*****n（us）延时子程序*****/
// a NOP loop to insert a timing delay
void DelayUs(uint N)
{
  uint x;
  for(x=0; x<=N;x++);
}

/*****中断服务程序*****/
// interrupt service routing for INT1 (P3.2)
void intersvr0(void) interrupt 0 using 1
{
  KY=~KY;      //每个摇动来回水银开关会在摆幅两端分别产生下降沿中断，只提取其中一次（从左向右摇才显示）
               // Each rocking back and forth in the mercury switch (or gyro vibration switch)
               // will swing both ends falling edge interrupts, extract only once ( from left to right to shake display )
  if(KY==0)
  {
    num++;     //计算中断次数
    switch(pic)      //选择画面
    {
      case 0:{display1();}break;
      case 1:{display2();}break;
      case 2:{display3();}break;
      case 3:{display4();}break;
      default:{display1();}
    }
  }
}

/*****显示子程序1（汉字）*****/
// Display subroutine 1 ( Chinese characters )
void display1(void)
{
  uchar i;
  if(num>10){disp++;num=0;}     //12个汉字分为3次显示完（每次显示4个），每中断10次切换
  if(disp>2)disp=0;
  DelayUs(5200);     //此处延时时间依各硬件差别而各不相同，试着调整使得显示内容居中即可
  for(i=0;i<64;i++)
  {
    P0=~hanzi[disp*128+i*2];   // set the first 8 LEDS (on port P0)
    P2=~hanzi[disp*128+i*2+1]; // set the last 8 LEDS (on port P2)
    DelayUs(100);
  }
}

/*****显示子程序2（LOVE）*****/
// Display subroutine 2 ( LOVE )
void display2(void)
{
  uchar i;
  DelayUs(4000);
  for(i=0;i<64;i++)
  {
    P0=~love[i*2];
    P2=~love[i*2+1];
    DelayUs(120);
  }
}

/*****显示子程序3（心形图案）*****/
// Display subroutine 3 ( heart-shaped pattern )
void display3(void)
{
  uchar i;
  DelayUs(4000);
  for(i=0;i<64;i++)
  {
    P0=~loveyou[i*2];
    P2=~loveyou[i*2+1];
    DelayUs(120);
  }
}

/*****显示子程序4（呵呵o(∩_∩)o图案）*****/
// Display subroutine 4 ( ASCII-art oh! pattern )
void display4(void)
{
  uchar i;
  DelayUs(4000);
  for(i=0;i<64;i++)
  {
    P0=~hehe[i*2];
    P2=~hehe[i*2+1];
    DelayUs(120);
  }
}

/*****主函数*****/
void main(void)
{
  /*-----------------------------------------------
  Configure INT0 (external interrupt 0) to generate
  an interrupt on the falling-edge of /INT0 (P3.2).
  Enable the EX0 interrupt and then enable the
  global interrupt flag.
  INT0 is use to capture input from the mercury switch or gyro vibration switch
  -----------------------------------------------*/
  IT0=1;     // Configure interrupt 0 for falling edge on /INT0 (P3.2)
  EX0=1;     // Enable EX0 Interrupt
  EA=1;      // 开中断，下降沿中断  Enable Global Interrupt Flag

  KY=0;      // initialise the INT0 counter
  while(1)   //主程序中只检测按键 looping to detect screen switching key press
  {
    if(KEY==0)     //画面切换键按下 screen switching key is pressed
    {
      DelayUs(10000);     //按键去抖 key debounce
      if(KEY==0);         // wtf? NOP
      pic++;
      while(KEY==0);      // damn it, debounce!!?
    }
    if(pic>3)pic=0;
  }
}
/*****END*****/
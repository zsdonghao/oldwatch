//作者：中山董豪  时间：2010年
//功能：4位数码管可以显示 时分，月日，年份；万年历，整点报时，定时自动(或手动)关闭显示器,空闲模式工作省电
//硬件：2051单片机，P1.6到P1.0分别是 A到G，P1.7=DP,P3.5=D4,P3.4=D3,P3.1=D2,P3.0=D1,P3.7控制直流蜂鸣器(整点报时)。
//      P3.2=0（改时间）,P3.3=0（改状态），6Mhz晶振。
//使用说明：默认时间为 2010年1月1日,0点0分，状态1显示时分
//          按P3.2为0，状态++，状态功能分别为 1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
//          用DP来表示状态,DP每秒闪烁1次(占空比3:1)。  对应的修改位置，闪烁。
//          在显示年月日时分的状态启动P3.2为0，则进入关闭显示器模式，或打开显示器，在关闭显示器的模式中，按任意键打开显示器。
//          在修改年月日时分的状态启动P3.2为0，则对应数字加一，一直按住，则一直加。注：改年份月份的时候为了不与万年历冲突，日期自动变为1号，年份范围是2010到2099年
//注明：该程序不是最终程序，更新程序请联系中山董豪
//实测参数：数码管工作时：2mA , 数码管不工作时：1mA
//建议改进 选用更低频晶振 , 改用ATtiny2313代替。
#include <reg52.h>
#include<intrins.h>
#define uint unsigned int
#define uchar unsigned char

uchar second,minh,minl,hourh,hourl,hour,i,j,k,l,state,slient,dpi,dps,d2;
uchar day,month,dayh,dayl,monthh,monthl;                                  //1，3，5，7，8，10，12 大 31
uchar year1,year2,year3,year4,year;                                             //4，6，9，11，小30
//unsigned int d1,d0 ;                                                                   //2月闰年29天        year%4==0&&year%100!=0||year%400==0


uchar code thourh[]={10,10,10,10,10,10,10,10,10,10,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2};
uchar code thourl[]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
uchar code tmonthh[]={10,10,10,10,10,10,10,10,10,10,1,1,1};
uchar code tmonthl[]={ 0,1, 2, 3, 4, 5, 6, 7, 8, 9 ,0,1,2};
uchar code tdayh[]={10,10,10,10,10,10,10,10,10,10,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3};
uchar code tdayl[]={ 0,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1};
uchar code display[]={0x7E,0x30,0x6D,0x79,0x33,0x5B,0x5F,0x70,0x7F,0x7B,0x00}; //七段译码 P1.6=A
sbit DP=P1^7;
sbit int0=P3^2; //改时间
sbit int1=P3^3;
sbit bee=P3^7;
sbit P3_0=P3^0;
sbit P3_1=P3^1;
sbit P3_4=P3^4;
sbit P3_5=P3^5;
void main()
{
 TMOD=0x11;
 TH0=(65536-50000)/256;
 TL0=(65536-50000)%256;        //0.05s一次中断,时钟
 TH1=(65536-3000)/256;
 TL1=(65536-3000)%256;        //0.006s一次中断,扫描显示
    IE=0X8A;                     //EA,ET0,ET1=1
 IP=0x02;                     //PT0=1;
 state=1; //   1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
 slient=1; // 1:正常显示   2：关闭显示
 second=0; //2010年0点0分0秒
 minl=0;
 minh=0;
 hour=0;
 day=1;
    bee=0;
 month=1;
 year1=0;
 year2=1;
 year3=0;
 year4=2;
 TR1=1;
    TR0=1;
 while(1)
 { PCON=0X01; //空闲模式}//while
 }

} //main  设置



void timer0() interrupt 1 using 0 //时钟 改状态 改时间
{
    TH0=(65536-50000)/256;  //0.1s
 TL0=(65536-50000)%256;
 i++;
 if(i==10)
 {
 i=0;
  bee=1;
     if(slient==1)     //自动关闭显示器，剩电  10s自动关闭
 { d2++;
  if(d2==10)
   {d2=0;
    //d3++;
    //if(d3==200) //
   // d3=0;
    slient=0;

      }
    }                   //自动关闭显示器，剩电
  if(second<59)
     second++;
  else
  {   second=0;
  if(minl<9)
     minl++;
  else
  {   minl=0;
     if(minh<5)
        minh++;
     else
     {
       minh=0;
     bee=0;              //整点报时
     if(hour<23)
        hour++;
     else
       { hour=0;

           /////////////////////////////////////// 万年历
                               if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12) //1，3，5，7，8，10，12 大 31
             {
            if(day<31)
           day++;
         else
           {day=1;
            if(month<12)
              month++;
            else
             { month=1;
            if(year1<9)
              year1++;
            else
              {year1=0;
           if(year2<9)
             year2++;
           else
             {year1=0;               //最高位2099年
                 year2=1;
              year3=0;
              year4=2;
             }
                                            }
          }
            }
                                   }
          else if(month==4 || month==6 || month==9 || month==11)                                   //4，6，9，11，小30
             {
           if(day<30)
           day++;
         else
           {day=1;
            if(month<12)
              month++;
            else
             { month=1;
           if(year1<9)
              year1++;
            else
              {year1=0;
           if(year2<9)
             year2++;
           else
             {year1=0;               //最高位2099年
                 year2=1;
              year3=0;
              year4=2;
             }
                                            }
          }
           }
           }
          else if(month==2)                                             //2月闰年29天
             { year=year4*1000+year3*100+year1*10+year1;
         if((year%4==0&&year%100!=0)||year%400==0)
          {if(day<29) //闰年
                day++;
          else
           {day=1;
            if(month<12)
              month++;
            else
             { month=1;
           if(year1<9)
              year1++;
            else
              {year1=0;
           if(year2<9)
             year2++;
           else
             {year1=0;               //最高位2099年
                 year2=1;
              year3=0;
              year4=2;
             }
                                            }
          }
           }}
                                     else
                                      {if(day<28) //非闰年
                day++;
            else
            {day=1;
            if(month<12)
              month++;
            else
             { month=1;
           if(year1<9)
              year1++;
            else
              {year1=0;
           if(year2<9)
             year2++;
           else
             {year1=0;               //最高位2099年
                 year2=1;
              year3=0;
              year4=2;}
                                      }}}}
          }
               ////////////////////////////////////////////
                      }
     }
  }
     }
 }
/////////////////////////////////////////////////闪烁标志位
 dpi++;        //dps=0 的占空比为 3:1
 if(dpi==5)
 {
   dpi=0;
   dps++;
   if(dps==2)
   {dps=0;}
     }//
//   if(int1==0) //p3.3调状态 每秒看一次
 //      d2=0;
//  }//dpi=5
/////////////////////////////////////////////////////p3.3调状态
  if(dpi==0 || dpi==2 || dpi==4) //0.2s 扫描一次
 { if(int1==0) //p3.3调状态
      {  d2=0;
        if(slient==1)
    { if(state==8)
    state=1;
     else
   state++;
      }//slient==1
       else
        { slient=1;
   ET1=1;}
//      for(d1=1000000;d1>0;d1--);
      } //调状态 int1==0
////////////////////////////////////////////////////////////p3.2改时间
 if(int0==0) //P3.2调节
      {   d2=0;
     if(state==1 || state==2 || state==3) //显示状态下按P3.2，进入不显示状态，按任意键退出。
      {
      if(slient==2)
     { slient=1;
   ET1=1;}
   else
      slient++;
   }
     else if(state==4)      //
      {second=0;
  if(minl<9)
     minl++;
  else
  {   minl=0;
     if(minh<5)
        minh++;
     else
     {
        minh=0;
     }
  }
   }
     else if(state==5)  //
  {
  if(hour<23)
    hour++;
  else
    hour=0;
  }
  else if(state==8)  //
    {           ///////////////////////////////////////
                               if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)             //1，3，5，7，8，10，12 大 31
             {
            if(day<31)
           day++;
         else
           day=1;
                                   }
          else if(month==4 || month==6 || month==9 || month==11)                                   //4，6，9，11，小30
             {
           if(day<30)
           day++;
         else
                                      day=1;
           }
          else if(month==2)                                             //2月闰年29天
             { year=year4*1000+year3*100+year1*10+year1;
             if((year%4==0&&year%100!=0)||year%400==0)
             {if(day<29) //闰年
                day++;
          else
             day=1;
                                      }
         else
          {if(day<28) //非闰年
                day++;
          else
             day=1;
          }
          }
               ////////////////////////////////////////////
    }
  else if(state==7)  //
    {
             day=1;
    if(month<12)
     month++;
    else
    month=1;
    }
  else if(state==6)  // 1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
    {    day=1;
    if(year1<9)
    year1++;
    else
   {  year1=0;
     if(year2<9)
      year2++;
        else
    {year1=0;               //最高位2099年
     year2=1;
     year3=0;
     year4=2;
    }
            }
    }//state=6
//     for(d0=600000;d0>0;d0--);
     }   //调时 int0
//}
 } //dpi==0 ,2,4

} //timer0 时间  调时  改状态




void timer1() interrupt 3 using 3   //display
{
  TH1=(65536-3000)/256;
  TL1=(65536-3000)%256;        //0.006s
  monthl=tmonthl[month];
  monthh=tmonthh[month];
  dayl=tdayl[day];
  dayh=tdayh[day];
  hourl=thourl[hour];
  hourh=thourh[hour];
  if(slient==1)
  {   if(j<4)
      j++;
    else
      j=1;
    if(state==1 || state==4 || state==5) //显示时间  1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
     {
        if(j==1)
      {if(state==4 && dps==0 && int0!=0) // 闪烁,调节时不闪
   P1=0x00;
   else
   {P1=display[minl];     //p3.5,p3.4,p3.1,p3.0 为势能端
      }}   //11XX10
     else if(j==2)
      {if(state==4 && dps==0 && int0!=0) // 闪烁
   P1=0x00;
   else
   {P1=display[minh];
    }}   //11XX01
         else if(j==3)
      {if(state==5 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[hourl];
    }}    //10XX11
      else if(j==4)
      {if(state==5 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[hourh];
           }}       //01XX11
          }
   else if(state==2 || state==7 || state==8)   //显示月日    1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
    {
        if(j==1)
      {if(state==8 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[dayl];     //p3.5,p3.4,p3.1,p3.0 为势能端
     }}   //11XX10
     else if(j==2)
      {if(state==8 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[dayh];
     }}   //11XX01
         else if(j==3)
      {if(state==7 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[monthl];
     }}    //10XX11
      else if(j==4)
      {if(state==7 && dps==0 && int0!=0) // 闪烁
    P1=0x00;
    else
    {P1=display[monthh];
           }}       //01XX11
    }
  else if(state==3 || state==6)  //显示年
  {
  if(state==6 && dps==0 && int0!=0)
  P1=0x00;
  else
       {if(j==1)
      {P1=display[year1];     //p3.5,p3.4,p3.1,p3.0 为势能端
       }   //11XX10 3e
     else if(j==2)
      {P1=display[year2];
    }   //11XX01 3d
         else if(j==3)
      {P1=display[year3];
    }    //10XX11 2f
      else if(j==4)
      {P1=display[year4];
          }       //01XX11 1f
    }
  }

  if(j==1)
   {P3_0=0;P3_1=1;P3_4=1;P3_5=1;}
  else if(j==2)
   {P3_0=1;P3_1=0;P3_4=1;P3_5=1;}
  else if(j==3)
   {P3_0=1;P3_1=1;P3_4=0;P3_5=1;}
  else if(j==4)
   {P3_0=1;P3_1=1;P3_4=1;P3_5=0;}
/////////////////////////////////////////////////////////// DP
/////////////////////////////////////////////////////////// 1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；
   if(dps==1)
   {if(state==1 || state==2)
   {   if(j==1)                 //DP,显示时间，月日
      {DP=0;}
    else if(j==2)
      {DP=0;}
         else if(j==3)
      {DP=1;}
    else if(j==4)
      {DP=0;}       }
  else if(state==3)
   {   if(j==1)                 //DP,显示年
      {DP=0;}
    else if(j==2)
      {DP=0;}
         else if(j==3)
      {DP=0;}
    else if(j==4)
      {DP=0;}       }
  else if(state==4)
 {   if(j==1)                 //DP，改分钟
      {DP=1;}
    else if(j==2)
      {DP=1;}
         else if(j==3)
      {DP=0;}
    else if(j==4)
      {DP=0;}       }
    else if(state==5)
 {   if(j==1)                 //DP，改时钟
      {DP=0;}
    else if(j==2)
      {DP=0;}
         else if(j==3)
      {DP=1;}
    else if(j==4)
      {DP=1;}       }
   else if(state==8)
   {     if(j==1)                 //DP，改日
      {DP=0;}
    else if(j==2)
      {DP=1;}
         else if(j==3)
      {DP=0;}
    else if(j==4)
      {DP=0;}
   }
      else if(state==7)
   {     if(j==1)                 //DP，改月
      {DP=0;}
    else if(j==2)
      {DP=0;}
         else if(j==3)
      {DP=0;}
    else if(j==4)
      {DP=1;}
   }
     else if(state==6)
   {        DP=0;               //DP， 改年
   }}
   else  //dps==0
   DP=0;
   //////////////////////////////////////////////////////////////////DP
   }
     else //slient==0
    {P1=0x00;ET1=0;}  //省电,关闭显示器，关闭显示中断
} //timer1 显示

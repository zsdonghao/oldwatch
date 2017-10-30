# Old Fashion Watch
古典电子手表表：进入21世纪10年代，电子工业发展迅猛；2010年后，智能手表更是迎来飞速的发展，这唤起了作者对八九十年代传统电子工程的怀恋，于是作者决定使用最老的单片机之一 “MCU-51” 来手工制作一款万年历电子表。

In 2010s, with the rapid development of modern microelectronics industry, Apple and Samsung put smart watchs on everyone's wrist. This situation let author miss the old fashion microelectronics industry between 1980s to 1990s. Then author decides to hand-make a digit permanent calendar watch with the oldest microcontroller.

>>万年历，空闲模式、整点报时
>>
>>Permanent Calendar，Idle Mode，Integral Point Alarm

## Atmel C2051 MCU
>作者：zsdonghao  时间：2010年

>功能：4位数码管可以显示 时分，月日，年份；万年历，整点报时，定时自动(或手动)关闭显示器,空闲模式工作省电

>硬件：2051单片机，P1.6到P1.0分别是 A到G，P1.7=DP,P3.5=D4,P3.4=D3,P3.1=D2,P3.0=D1,P3.7控制直流蜂鸣器(整点报时)。

> P3.2=0（改时间）,P3.3=0（改状态），6Mhz晶振。

> 使用说明：默认时间为 2010年1月1日,0点0分，状态1显示时分

> 按P3.2为0，状态++，状态功能分别为 1:显示时间 2:显示月日 3：显示年  4：修改分钟  5：修改时钟  6:修改年  7：修改月  8：修改日 ；用DP来表示状态,DP每秒闪烁1次(占空比3:1)。  对应的修改位置，闪烁。

> 在显示年月日时分的状态启动P3.2为0，则进入关闭显示器模式，或打开显示器，在关闭显示器的模式中，按任意键打开显示器。

> 在修改年月日时分的状态启动P3.2为0，则对应数字加一，一直按住，则一直加。注：改年份月份的时候为了不与万年历冲突，日期自动变为1号，年份范围是2010到2099年

>注明：该程序不是最终程序，更新程序请联系中山董豪

>实测参数：数码管工作时：2mA , 数码管不工作时：1mA

>建议改进 选用更低频晶振 , 改用ATtiny2313代替。

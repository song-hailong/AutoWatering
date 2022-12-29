#ifndef main_H
#define main_H


#include <U8g2lib.h>
#include <Wire.h>
#include <math.h>
#include "MathFun.h"
#include "Menu.h"
#include "MyTime.h"
#include "FeaturesFun.h"


#define LED_BUILTIN 27 //板载灯
#define Water_Pump  4 //水泵引脚
#define ADC1_CH0    36 //ADC引脚
#define ButtonPin   13 //按键引脚

#define WateringTime 5 //每次浇水时长，单位：秒 s

extern float Humidity; //湿度
extern uint8_t BeginHumidity; //开始浇水湿度
extern bool Is_Watering;  //是否在浇水
extern mytime NowTime;  //当前时间
extern mytime LastWaterTime; //上次浇水时间
extern mytime LastWaterTime2; //上上次浇水时间
extern U8G2_SH1107_128X80_F_HW_I2C Disp;
extern bool BeginWatering; //强制开始浇水
extern bool StopWatering; //强制停止浇水
extern bool Is_WateringSuccess; //自动浇水成功标志

//用户自定义状态返回的心跳包回调函数
void heartbeat(void);

#endif
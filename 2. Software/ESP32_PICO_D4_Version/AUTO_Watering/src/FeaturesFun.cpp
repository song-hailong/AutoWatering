#include "FeaturesFun.h"
#include "main.h"

uint32_t BeginWateringTime; //开始浇水时间记录单位s
uint32_t lastGetADCTime = 0; //上次ADC读取时间 ms

//计算湿度  0 ~ 100
float CalculateHumidity(int ADC)
{
    return (float)ADC * 100 / 4095 ;
}

//浇水函数
void Watering()
{
    if(BeginWatering == true) //通过APP按键强制开始浇水
    {
        OpenPump;  //打开水泵
        BeginWateringTime = millis()/1000; //记录打开水泵时间
        Is_Watering = true; //正在浇水
        BeginWatering = false;
        heartbeat();//更新app界面显示
    }

    if(Humidity == 0)
        return;

    if(Humidity < BeginHumidity && Is_Watering == false)  //湿度小于临界值 且 未在浇水
    {
        if(NowTime.GetNowTime() == false)
            return ;
        //离上次浇水已经过去2天
        if((NowTime.My_yday + NowTime.My_year * 365) - (LastWaterTime.My_yday + LastWaterTime.My_year * 365)  > 2)
        {
            OpenPump;  //打开水泵
            BeginWateringTime = millis()/1000; //记录打开水泵时间
            Is_Watering = true; //正在浇水 
            heartbeat();//更新app界面显示
        }
             
    }
    if(Is_Watering == true) //如果正在浇水
    {
        if((millis()/1000 - BeginWateringTime > WateringTime) || StopWatering == true)
        {
            ClosePump; //关闭水泵
            LastWaterTime2 = LastWaterTime;
            LastWaterTime.GetNowTime(); //更新浇水时间
            StopWatering = false;
            Is_Watering = false; //未在浇水
            Is_WateringSuccess = true; //自动浇水成功
            heartbeat();//更新app界面显示
        }
    }
}

/**
 *调用卡尔曼滤波器 实践
 */
KFP KFP_Temp = {0.21, 0.22, 2821.33, 0.05, 0.01, 0.543};
// 获取湿度
void GetHumidity()
{
    if(millis() - lastGetADCTime < 50)
        return;
    uint16_t ADC_RAW = analogRead(ADC1_CH0);
    if(ADC_RAW == 0 || ADC_RAW == 4095)
        return;
    uint16_t ADC;
    //卡尔曼滤波器
    ADC = kalmanFilter(&KFP_Temp, (float) ADC_RAW);
    Humidity = CalculateHumidity(ADC); //计算湿度

    lastGetADCTime = millis();
}
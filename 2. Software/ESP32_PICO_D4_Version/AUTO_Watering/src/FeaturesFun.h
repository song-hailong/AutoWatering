#ifndef FeaturesFun_H
#define FeaturesFun_H


#define OpenPump  digitalWrite(Water_Pump, LOW)
#define ClosePump digitalWrite(Water_Pump, HIGH)

float CalculateHumidity(int ADC); //计算湿度  0 ~ 100
void Watering(void);  //浇水函数
void GetHumidity(void);// 获取湿度

#endif // !1
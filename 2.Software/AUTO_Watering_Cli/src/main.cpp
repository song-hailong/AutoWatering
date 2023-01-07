#define BLINKER_WIFI
#define BLINKER_WIDGET
#define BLINKER_OTA_VERSION_CODE "0.1.1"

#include <Arduino.h>
#include <Blinker.h>
#include "main.h"

// U8G2_SH1107_128X80_F_HW_I2C Disp(U8G2_R3, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 22, /* data=*/21);

bool WIFI_State = false;  //设备初始化状态，初始化指设备连上WiFi并能访问外网，设备开始连接MQTT broker
float Humidity = 0.0; //湿度
uint8_t BeginHumidity = 30; //开始浇水湿度
time_t run_time; //系统运行时间 单位s
time_t LastGetTimeTime = 0; //记录上次获取时间的时间
mytime NowTime;  //当前时间
mytime LastWaterTime;  //上次浇水时间
mytime LastWaterTime2; //上上次浇水时间
bool Is_Watering = false;  //是否在浇水
char SprintfChar[50];
char SprintfChar2[50];
bool BeginWatering = false; //强制开始浇水
bool StopWatering = false; //强制停止浇水
bool Is_WateringSuccess = false; //自动浇水成功标志
time_t WaitWIFICount = 0; //等待上线计时
//float SignalStrength = -34.0; // 信号强度

//****************************此处需修改************************************//
char auth[] = "设备的密匙";    // 此处填写设备的密匙
char ssid[] = "WIFI名称";     // 此处填写WIFI名称
char pswd[] = "WIFI密码";     // 此处填写WIFI密码

// 新建组件对象
#define Number_Hum "num-humidity"
BlinkerNumber NumberHum(Number_Hum);             // 数据组件：显示湿度
BlinkerText TextLastWatered("tex-LastWatered");  // 文字组件：显示上次浇水时间
BlinkerText TextBlinkerTime("tex-BlinkerTime");  // 文字组件：显示系统运行时间
BlinkerText TextPumpState("tex-state");          // 文字组件：水泵状态
//BlinkerNumber NumberSignal((char*)"num-signal");         // 数据组件：显示WiFi信号强度
#define Slider_Hum "ran-Humidity"
BlinkerSlider SliderHumidity(Slider_Hum);        // 滑动条组件：APP端设置浇水湿度
#define BUTTON_Record "btn-record"
BlinkerButton BUTTONRecord(BUTTON_Record);       //按键组件：在APP中记录手动浇水
#define BUTTON_Watering "btn-watering"
BlinkerButton BUTTONWatering(BUTTON_Watering);   //按键组件：开始和停止浇水

//图表回调函数 当设备需要上传数据到云端时会调用，每1分钟上传一次
void dataStorage()
{
  Blinker.dataStorage("Humidity", Humidity);//储存信息到缓存
}

//滑动条组件的回调函数, 当收到指令时会调用该回调函数
void SliderHumidity_callback(int32_t value)
{
    BeginHumidity = value;
    BLINKER_LOG("get slider value: ", value);
}
//记录手动浇水按键的回调函数, 当收到指令时会调用该回调函数
void BUTTONRecord_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);

  LastWaterTime2 = LastWaterTime;
  if(LastWaterTime.GetNowTime() == false) //更新浇水时间
  {
    Blinker.delay(200);
    LastWaterTime.GetNowTime();
  }

  //BUTTONRecord.icon("icon_1");
  //BUTTONRecord.color("#FFFFFF");
  BUTTONRecord.text("记录手动浇水");
  BUTTONRecord.print();
  heartbeat();//更新app界面显示
}

//浇水按键的回调函数, 当收到指令时会调用该回调函数
void BUTTONWatering_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  
  if(state == "on")
  {
    BeginWatering = true;
    //BUTTONWatering.icon("icon_1");
    // BUTTONWatering.color("red");
    // BUTTONWatering.text("浇水");
    // BUTTONWatering.print("on");

    // TextPumpState.color("Red");
    // TextPumpState.print("正在浇水");
  }
  else if(state == "off")
  {
    StopWatering = true;
    //BUTTONWatering.icon("icon_1");
    // BUTTONWatering.color("Blue");
    // BUTTONWatering.text("浇水");
    // BUTTONWatering.print("off");

    // TextPumpState.color("Blue");
    // TextPumpState.print("未在浇水");
  }
}

//用户自定义状态返回的心跳包回调函数
void heartbeat()
{

  //NumberHum.icon("fa-solid fa-droplet-degree");    //图标
  //NumberHum.color("#00FF00");   //颜色
  //NumberHum.unit("ls");         //单位
  NumberHum.print(Humidity); //发送数字组件当前的数值, 并将以上设置一并发送到APP
  
  sprintf(SprintfChar, "系统运行时间：%ld天%ld时 ", run_time/3600/24, run_time/3600%24);
  // TextBlinkerTime.icon("far fa-shipping-timed");
  TextBlinkerTime.print(SprintfChar);

//  NumberSignal.print(SignalStrength);//信号强度

  sprintf(SprintfChar,  "上次浇水时间: %d年%0d月%0d日 %02d:%02d:%02d", 
          LastWaterTime.My_year, LastWaterTime.My_month, LastWaterTime.My_mday, 
          LastWaterTime.My_hour, LastWaterTime.My_min, LastWaterTime.My_sec);
  sprintf(SprintfChar2, "上上次浇水时间: %d年%0d月%0d日 %02d:%02d:%02d", 
          LastWaterTime2.My_year, LastWaterTime2.My_month, LastWaterTime2.My_mday, 
          LastWaterTime2.My_hour, LastWaterTime2.My_min, LastWaterTime2.My_sec);
  TextLastWatered.print(SprintfChar, SprintfChar2);

  if(Is_Watering == true)
  {
    TextPumpState.color("Red");
    TextPumpState.print("正在浇水");

    BUTTONWatering.color("red");
    BUTTONWatering.text("浇水");
    BUTTONWatering.print("on");
  }
  else
  {
    TextPumpState.color("Blue");
    TextPumpState.print("未在浇水");

    BUTTONWatering.color("Blue");
    BUTTONWatering.text("浇水");
    BUTTONWatering.print("off");
  }
  
  //SliderHumidity.color("#00FF00"); //设置滑动条组件的颜色
  SliderHumidity.print(BeginHumidity); //发送用户需要的滑动条数值及设置的颜色到APP

}
//MQTT设备 在心跳包中可以返回设备的简要信息, 如当前传感器值等。
//在设备管理页面中会显示设备当前简要信息, 不用进入设备页面查看。
String summary()
{
    String data = "online";
    return data;
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);
  Blinker.vibrate();  //发送手机振动指令
}



void setup() {
  // put your setup code here, to run once:

  // 初始化串口
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  BLINKER_DEBUG.debugAll();
  
  //初始化OLED
//  OLED_Init();
//  DisplayStart(); //显示开始界面

  // 初始化IO
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(Water_Pump, OUTPUT);
  digitalWrite(Water_Pump, HIGH);

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  //注册回调函数
  Blinker.attachHeartbeat(heartbeat); //注册心跳包回调函数
  Blinker.attachSummary(summary); //设备简要信息回调函数, MQTT设备 在心跳包中可以返回设备的简要信息
  Blinker.attachDataStorage(dataStorage);//注册图表回调函数，当设备需要上传数据到云端时会调用对应的回调函数，每1分钟上传一次
  SliderHumidity.attach(SliderHumidity_callback); //设置滑动条组件的回调函数, 当收到指令时会调用该回调函数
  BUTTONRecord.attach(BUTTONRecord_callback); //注册按键的回调函数, 当收到指令时会调用该回调函数
  BUTTONWatering.attach(BUTTONWatering_callback);//注册按键的回调函数, 当收到指令时会调用该回调函数
  
  Blinker.setTimezone(8.0); //设置时区

}

unsigned long qwe = 0;
void loop() {
  // put your main code here, to run repeated2bgv4ly:
  Blinker.run();

  //设备初始化状态改变
  if(WIFI_State == false)  // 返回设备初始化状态(仅限 BLINK_WIFI/BLINKER_PRO 设备)
  {
    if(millis() - WaitWIFICount >= 10*60*1000) //如果10分钟未上线，开始离线运行
    {
      WIFI_State = true;
    }
    if(Blinker.init() == true)
    {
      WIFI_State = true;
      digitalWrite(LED_BUILTIN, HIGH);
      Blinker.wechat("上线提醒", "在线", "自动浇灌系统已上线");  // 发送微信模板消息
    }
  }
  
  if (WIFI_State == true)  //已上线
  {
    if(millis() - LastGetTimeTime >= 1000) //距上次获取当前时间已经过去1s
    {
      run_time = Blinker.runTime(); //获取设备运行时间, 单位为秒(s)
      NowTime.GetNowTime(); //获取当前时间
//      SignalStrength = WiFi.RSSI(); // WIFI信号强度
      if(NowTime.is_ok == true) // 获取当前时间成功
      {
        if(NowTime.My_hour == 10 && NowTime.My_min == 0 && NowTime.My_sec == 0) //每天10点发送湿度数据
        {
          snprintf(SprintfChar, sizeof(SprintfChar), "湿度: %.2f ", Humidity);
          Blinker.wechat("湿度数值每日提醒", "在线", SprintfChar);  // 发送微信模板消息
        }
      }
      if(Is_WateringSuccess == true) //自动浇水成功
      {
        Is_WateringSuccess = false;
        snprintf(SprintfChar, sizeof(SprintfChar), "%d年%0d月%0d日%02d:%02d:%02d自动浇水成功", 
          LastWaterTime.My_year, LastWaterTime.My_month, LastWaterTime.My_mday, 
          LastWaterTime.My_hour, LastWaterTime.My_min, LastWaterTime.My_sec);
        snprintf(SprintfChar2, sizeof(SprintfChar2), "当前湿度为%.2f%%", Humidity);
        Blinker.wechat("自动浇水成功提示", SprintfChar2, SprintfChar);  // 发送微信模板消息
      }
    }
    GetHumidity();// 获取湿度
    Watering();  //浇水函数
    //DisplayWork();  //显示工作界面
  }
}


//获取当前时间
bool mytime::GetNowTime()
{
  int8_t  sec = Blinker.second();  //秒
  int8_t  min = Blinker.minute();  //分
  int8_t  hour = Blinker.hour();   //时
  int8_t  wday = Blinker.wday();   //周几 0-6(依次为周日/一/二/三/四/五/六)
  int8_t  mday = Blinker.mday();   //当月第几天  1-31
  int16_t yday = Blinker.yday();   //当年第几天  1-366
  int8_t  month = Blinker.month(); //月份
  int16_t year = Blinker.year();   //年份

  if( sec != -1 || min != -1 || hour != -1 || wday != -1 || mday != -1 || yday != -1 || month != -1 || year != -1)
  {
    this->My_sec = sec;
    this->My_min = min;
    this->My_hour = hour;
    this->My_wday = wday;
    this->My_mday = mday;
    this->My_yday = yday;
    this->My_month = month;
    this->My_year = year;

    this->is_ok = true;
  }
  else
    this->is_ok = false;

  return this->is_ok;
}


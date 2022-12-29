#include "main.h"

void OLED_Init()
{
    Disp.begin();   //构造U8G2
    Disp.setBusClock(921600);  //设置总线时钟
    Disp.enableUTF8Print();        // 启用 UTF8打印  enable UTF8 support for the Arduino print() function
    Disp.setFontDirection(0);  // 设置字体方向
    Disp.setFontPosTop();
    Disp.setFont(u8g2_font_wqy16_t_gb2312);  // 设置字体集  use chinese2 for all the glyphs of "你好世界"  u8g2_font_unifont_t_chinese3
    Disp.setDrawColor(1);      //设置绘制颜色
    Disp.setFontMode(1);
}


struct Menu_System Menu[] = {
    {0 , "自动浇灌系统",},
    {1 , "正在连接WIFI"},
    {2 , "WIFI : 已连接",},
    {3 , "湿度 :       ",},
    {4 , "记录手动浇水成功",},
};


void Draw_Utf(int x, int y, const char* s)
{
    // Disp.setCursor(x,y + 1);
    // Disp.print(s);
    Disp.drawUTF8(x, y + 1, s);
}

// 显示标题
void DisplayTitle()
{
    Disp.setFont(u8g2_font_wqy16_t_gb2312);  // 设置字体集
    Draw_Utf(UTF8_HMiddle(0, 128, 1, Menu[title_id].name),  1, Menu[title_id].name); //居中显示项目名
}

// 显示开始界面
void DisplayStart()
{
    Disp.clearBuffer();  //清除缓冲区
    DisplayTitle();// 显示标题
    Disp.setFont(u8g2_font_wqy16_t_gb2312);  // 设置字体集
    Draw_Utf(UTF8_HMiddle(0, 128, 1, Menu[1].name), 40, Menu[1].name); //居中显示: 正在连接WIFI
    Disp.sendBuffer();   //绘制缓冲区的内容.sendBuffer();   //绘制缓冲区的内容
}
// 显示工作界面
void DisplayWork()
{
    Disp.clearBuffer();  //清除缓冲区
    
    DisplayTitle();// 显示标题

    Disp.setFont(u8g2_font_wqy14_t_gb2312);
    Disp.setCursor(18, 35);
    Disp.print("WIFI : 已连接");        // Chinese "Hello World"
    Disp.setCursor(18, 55);
    Disp.print("湿度 : ");        // Chinese "Hello World"
    Disp.print(Humidity);
    Disp.sendBuffer();   //绘制缓冲区的内容
}

// 显示记录手动浇水成功界面
void DisplayRecord()
{
    Disp.clearBuffer();  //清除缓冲区
    DisplayTitle();// 显示标题
    Disp.setFont(u8g2_font_wqy16_t_gb2312);  // 设置字体集
    Draw_Utf(UTF8_HMiddle(0, 128, 1, Menu[4].name), 40, Menu[4].name); //居中显示: 记录手动浇水成功
    Disp.sendBuffer();   //绘制缓冲区的内容.sendBuffer();   //绘制缓冲区的内容
}
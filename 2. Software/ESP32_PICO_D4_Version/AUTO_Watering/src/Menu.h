#ifndef __MENU_H
#define __MENU_H

#include <stdint.h>

#define title_id 0

struct Menu_System
{
    uint8_t id;
    char* name;  //选项名称
};

extern struct Menu_System Menu[];

void OLED_Init(void); //初始化OLED
void Draw_Utf(int x, int y, const char* s);
void DisplayStart(void); //显示开始界面
void DisplayWork(void);  //显示工作界面
void DisplayRecord(void); // 显示记录手动浇水成功界面

#endif

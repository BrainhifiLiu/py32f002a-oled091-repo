/////////////////////////////////////////////////////////////////////////////
// File Name: oled091.h
// Decs: Base on IIC.
// Author: Tony Liu
// Date: 2018.12
/////////////////////////////////////////////////////////////////////////////
#ifndef _T_OLED091_H
#define _T_OLED091_H

#include "t_iic.h"



//OLED091  分辨率128x64
#define Max_Column	    128
#define Max_Row		    64

//#define CMD_oled091_READ    0xa3 //读，0A3H；
//#define CMD_oled091_WRITE   0xa2 //写，0A2H。

#define CMD_OLED091_WRITE    0x78 //写

//指令寄存器地址
#define OLED_REG_INST       0x00
//数据寄存器地址
#define OLED_REG_DATA       0x40


//发送数据
#define oled091_Write_Data(data) oled091_Write_Byte(OLED_REG_DATA, data)
//发送指令
#define oled091_Write_Inst(data) oled091_Write_Byte(OLED_REG_INST, data)
/******************************************************************************
                             参数寄存器地址宏定义
******************************************************************************/
 
//width x high   
typedef enum {
    Oled_Font_8x8 = 0,
    Oled_Font_8x16,
    Oled_Font_16x16,
    Oled_Font_20x32
}OLED_FONT_SIZE;
///////////////////////////////////////////////////////////////////////////////
void oled091_init(void);

void oled091_delay(void);

void oled091_sda_in(void);
void oled091_sda_out(void);

uint8_t oled091_sda_read(void);

void oled091_sda_high(void);
void oled091_sda_low(void);

void oled091_scl_high(void);
void oled091_scl_low(void);

void oled091_Write_Byte(uint8_t REG_ADD, uint8_t data);
//uint8_t oled091_Read_Byte(uint8_t REG_ADD);
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
void OLED_Init(void);

void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_On(void);

void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,OLED_FONT_SIZE Char_Size);

void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,OLED_FONT_SIZE size2);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,OLED_FONT_SIZE Char_Size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,unsigned char BMP[]);

uint32_t oled_pow(uint8_t m,uint8_t n);


    
///////////////////////////////////////////////////////////////////////////////
#endif//!_T_OLED091_H

/******************************************************************************/
/** \file oled091.c
 **
 ** OLED 0.91" driver API.
 ** Base module SSD1306  ����128x32 Һ����:
 **     ������ѹ  DC 3.3v-5V
 **     �����ʽ  GPIO
 **     I2C���ߴӵ�ַ������0A3H��д��0A2H��
 **     
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-12  1.0  TonyLiu First version
 **
 ******************************************************************************/

#include "oled091.h"


#include "oledfont.h"  	 
#include "main.h"

//����IIC���ߵ��豸
volatile  type_iic      dev_oled091;

///////////////////////////////////////////////////////////////////////////////////////////

void oled091_init(void)
{
    //////////////////////////////////////////////////////////////////
	
    LL_GPIO_InitTypeDef GPIO_InitStruct;
	
    GPIO_InitStruct.Pin = OLED_SCL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; 
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;//LL_GPIO_PULL_UP
    LL_GPIO_Init(OLED_PORT_SCL, &GPIO_InitStruct);		
    //////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////
    dev_oled091.fn_delay = oled091_delay;
    
    dev_oled091.fn_scl_high = oled091_scl_high;
    dev_oled091.fn_scl_low = oled091_scl_low;
    dev_oled091.fn_sda_high = oled091_sda_high;
    dev_oled091.fn_sda_low = oled091_sda_low;
    dev_oled091.fn_sda_in = oled091_sda_in;
    dev_oled091.fn_sda_out = oled091_sda_out;
    dev_oled091.fn_sda_read = oled091_sda_read;
    //////////////////////////////////////////////////////////////////    
}    

void oled091_delay(void)
{
    delay_us(4);
}    
 

void oled091_sda_in(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = OLED_SDA;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(OLED_PORT_SDA, &GPIO_InitStruct);    

}    

void oled091_sda_out(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = OLED_SDA;//
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; 
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;//;
    LL_GPIO_Init(OLED_PORT_SDA, &GPIO_InitStruct);//    

}    

uint8_t oled091_sda_read(void)
{
    return LL_GPIO_IsInputPinSet(OLED_PORT_SDA,OLED_SDA);
    //return GPIO_ReadInputDataBit(OLED_PORT,OLED_SDA);
}    

void oled091_sda_high(void)
{
    //GPIO_SetBits
    LL_GPIO_SetOutputPin(OLED_PORT_SDA, OLED_SDA);
}    

void oled091_sda_low(void)
{
    //GPIO_ResetBits
    LL_GPIO_ResetOutputPin(OLED_PORT_SDA, OLED_SDA);
}    

void oled091_scl_high(void)
{
    //GPIO_SetBits
    LL_GPIO_SetOutputPin(OLED_PORT_SCL, OLED_SCL);
}    

void oled091_scl_low(void)
{
    //GPIO_ResetBits
    LL_GPIO_ResetOutputPin(OLED_PORT_SCL, OLED_SCL);
}    

void oled091_Write_Byte(uint8_t REG_ADD, uint8_t data)
{
	_iic_start(dev_oled091);
	_iic_send_byte(dev_oled091, CMD_OLED091_WRITE);//����д������Ӧ��λ
    
	//while(_iic_wait_slave_ack(dev_oled091));
    if(_iic_wait_slave_ack(dev_oled091))
    {
        _iic_stop(dev_oled091);
        return;
    }
    
	_iic_send_byte(dev_oled091,REG_ADD);
	_iic_wait_slave_ack(dev_oled091);
    
	_iic_send_byte(dev_oled091,data);	//��������	
	_iic_wait_slave_ack(dev_oled091);
    
	_iic_stop(dev_oled091);
} 

/*
uint8_t oled091_Read_Byte(uint8_t REG_ADD)
{
	uint8_t ReData;//,t=0;
    
	_iic_start(dev_oled091);
    
	_iic_send_byte(dev_oled091, CMD_PCF8563_WRITE);//����д������Ӧ��λ
	
    //while(_iic_wait_slave_ack(dev_oled091));
    if(_iic_wait_slave_ack(dev_oled091))
    {
        _iic_stop(dev_oled091);
        return 0;
        
    }
    
    
	_iic_send_byte(dev_oled091,REG_ADD);	//ȷ��Ҫ�����ļĴ���
	_iic_wait_slave_ack(dev_oled091);
    
	_iic_start(dev_oled091);	//��������
    
	_iic_send_byte(dev_oled091, CMD_PCF8563_READ);	//���Ͷ�ȡ����
	_iic_wait_slave_ack(dev_oled091);
    
	ReData = _iic_read_byte(dev_oled091,0);	//��ȡ����,�ӷ��ͷ�Ӧ��
    
    
    
        
	_iic_stop(dev_oled091);
    
	return ReData;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
//��ʾ����ͼ���ܺ���
//��������

void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 	
    oled091_Write_Inst(0xb0+y);
	oled091_Write_Inst(((x&0xf0)>>4)|0x10);
	oled091_Write_Inst((x&0x0f)); 
}

//����OLED��ʾ    
void OLED_Display_On(void)
{
	oled091_Write_Inst(0X8D);  //SET DCDC����
	oled091_Write_Inst(0X14);  //DCDC ON
	oled091_Write_Inst(0XAF);  //DISPLAY ON
}

//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	oled091_Write_Inst(0X8D);  //SET DCDC����
	oled091_Write_Inst(0X10);  //DCDC OFF
	oled091_Write_Inst(0XAE);  //DISPLAY OFF
}	

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;		    
	//for(i=0;i<8;i++)  
    for(i=0;i<4;i++)  
	{  
		oled091_Write_Inst (0xb0+i);    //����ҳ��ַ��0~7��
		oled091_Write_Inst (0x00);      //������ʾλ�á��е͵�ַ
		oled091_Write_Inst (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<Max_Column;n++)
            oled091_Write_Data(0); 
	} //������ʾ
}

void OLED_On(void)  
{  
	uint8_t i,n;		    
	//for(i=0;i<8;i++)  
    for(i=0;i<4;i++)  
	{  
		oled091_Write_Inst (0xb0+i);    //����ҳ��ַ��0~7��
		oled091_Write_Inst (0x00);      //������ʾλ�á��е͵�ַ
		oled091_Write_Inst (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<Max_Column;n++)
            oled091_Write_Data(1); 
	} //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0,1,2,3   (8x4 = 32)
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,OLED_FONT_SIZE Char_Size)
{      	
	unsigned char c=0,i=0;	
    c=chr-' ';//�õ�ƫ�ƺ��ֵ			
    if(x>Max_Column-1)
    {
        x=0;y=y+2;
    }
    if(Char_Size ==Oled_Font_20x32)
    {
        if(y!=0 || x>=108)
            return;
        
        if( !(chr>='0' && chr<='9') && chr!='.' && chr!='-')
            return;
        if(chr>='0' && chr<='9')
            c = chr-'0';
        else if(chr=='.')
            c = 10;
        else
            c = 11;
        
        
        OLED_Set_Pos(x,0);
        for(i=0;i<20;i++)
            oled091_Write_Data(F20x32[4*c][i]);
        
        
        OLED_Set_Pos(x,1);
        for(i=0;i<20;i++)
            oled091_Write_Data(F20x32[4*c+1][i]);

        OLED_Set_Pos(x,2);
        for(i=0;i<20;i++)
            oled091_Write_Data(F20x32[4*c+2][i]);

        OLED_Set_Pos(x,3);
        for(i=0;i<20;i++)
            oled091_Write_Data(F20x32[4*c+3][i]);
        
    }
    else if(Char_Size ==Oled_Font_8x16)
    {
        OLED_Set_Pos(x,y);	
        
        for(i=0;i<8;i++)
            oled091_Write_Data(F8X16[c*16+i]);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
            oled091_Write_Data(F8X16[c*16+i+8]);
    }
    else {	
        OLED_Set_Pos(x,y);
        for(i=0;i<6;i++)
            oled091_Write_Data(F6x8[c][i]);
            
    }
}

//m^n����
uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	

//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,OLED_FONT_SIZE size2)
{         	
	uint8_t t,temp;
	uint8_t enshow = 0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}
            else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,OLED_FONT_SIZE Char_Size)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		
        OLED_ShowChar(x,y,chr[j],Char_Size);
        if(Char_Size==Oled_Font_20x32)
            x += 20;
        else
            x += 8;
		if(x>120){  x=0;y+=2;}
        
        j++;
	}
}

//��ʾ����
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
    {
        oled091_Write_Data(Hzk[2*no][t]);
        adder+=1;
    }	
	OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
        oled091_Write_Data(Hzk[2*no+1][t]);
        adder+=1;
    }					
}

/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,unsigned char BMP[])
{ 	
    unsigned int j=0;
    uint8_t x,y;

    if(y1%8==0) y=y1/8;      
    else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=x0;x<x1;x++)
        {      
            oled091_Write_Data(BMP[j++]);	    	
        }
    }
} 

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
    oled091_init();
    
    delay_ms(200);

	oled091_Write_Inst(0xAE);   //display off
    
	oled091_Write_Inst(0x00);//--���õ��е�ַ
	oled091_Write_Inst(0x10);//--���ø��е�ַ
	oled091_Write_Inst(0x40);//--������ʼ�е�ַ
    
	oled091_Write_Inst(0xb0);//--����ҳ��ַ
    
	oled091_Write_Inst(0x81);//--�Աȶ�����
	oled091_Write_Inst(0xff);//-- 1-255


	oled091_Write_Inst(0xa1);//--���öε���ʼӳ���ַ0 to 127
	oled091_Write_Inst(0xa6);//-- 0xa6-������ʾ��0xa7-����ʾ

    //This command switches the default 63 multiplex mode to any multiplex ratio, ranging from 16 to 63. The
    //output pads COM0~COM63 will be switched to the corresponding COM signal.
	oled091_Write_Inst(0xa8);//--��������·�� set multiplex ratio(1 to 64)
	oled091_Write_Inst(0x1F);//--duty = 1/32  (0.96:0X3f ��0.91��:0x1f)

	oled091_Write_Inst(0xc8);	//Set COM Output Scan Direction


	oled091_Write_Inst(0xd3);//--������ʾƫ��
	oled091_Write_Inst(0x00);//--��ƫ��
    
    
	oled091_Write_Inst(0xd5);//--set display clock divide ratio/oscillator frequency
	oled091_Write_Inst(0x80);//--set divide ratio
    

	oled091_Write_Inst(0xd9);//--set pre-charge period
	oled091_Write_Inst(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;

	oled091_Write_Inst(0xda);//--set com pins hardware configuration
	oled091_Write_Inst(0x02);// ʹ��Ĭ��ֵ	0.96��ֱ�����128*64 ,����12h��0.91��ֱ�����128*32 ,����02h
    
    
	oled091_Write_Inst(0xdb);//--set vcomh  ��ѹ���ʣ��ɵ������ȣ�Ĭ�ϣ�
	oled091_Write_Inst(0x40);//0x20,0.77xVcc    ʹ�ùٷ��Ƽ�ֵ [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
    
	oled091_Write_Inst(0x8d);//--set DC-DC enable
	oled091_Write_Inst(0x14);//    
    

	oled091_Write_Inst(0x20);	//Set Memory Addressing Mode	
	oled091_Write_Inst(0x10);	//[[1:0] - 00,Horizontal Addressing Mode; 01,Vertical Addressing Mode; 10,Page Addressing Mode (RESET);11,Invalid
    
    
    
	oled091_Write_Inst(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    

    
	oled091_Write_Inst(0xaf);//--turn on oled panel     
    
	OLED_Clear();
}  








///////////////////////////////////////////////////////////////////////////////////////////





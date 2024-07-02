/******************************************************************************/
/** \file t_iic.c
 **
 ** Define iic driver API.
 **
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-22  1.0  TonyLiu First version
 **
 ******************************************************************************/
 #include "t_iic.h"
 
 
/************************************************************* 
*函数名称：_iic_start 
*函数功能：I2C开始信号 
*输入参数： 
*输出参数： 
*备           注：时钟线高时，数据线由高到低的跳变，表示I2C开始信号 

 ** Start
 **     ____
 ** SCL     |__
 **     __
 ** SDA   |____

**************************************************************/ 
void _iic_start( type_iic dev ) 
{ 
  
  
    (*dev.fn_sda_high)();  
    //(*dev.fn_delay)(); 
    (*dev.fn_scl_high)();
    
    (*dev.fn_delay)(); 

    (*dev.fn_sda_low)();
    (*dev.fn_delay)(); 

    (*dev.fn_scl_low)();
    (*dev.fn_delay)(); 
} 

/************************************************************* 
*函数名称：_iic_stop 
*函数功能：I2C停止信号 
*输入参数： 
*输出参数： 
*备           注：时钟线高时，数据线由低到高的跳变，表示I2C停止信号 
 ** Stop
 **     ______
 ** SCL     
 **        ___
 ** SDA __|  


**************************************************************/ 
void _iic_stop( type_iic dev ) 
{ 
        
    (*dev.fn_sda_low)();
    //(*dev.fn_delay)(); 
    (*dev.fn_scl_high)();
    
    (*dev.fn_delay)(); 
    
    (*dev.fn_sda_high)();  
    (*dev.fn_delay)(); 

    //GPIO_ResetBits( _O_EEPROM_PORT, _O_EEPROM_SCL ); 
} 

/************************************************************* 
*函数名称：_iic_wait_slave_ack 
*函数功能：I2C从机设备应答查询 
*输入参数： 
*输出参数： 
*备           注： 
**************************************************************/ 
uint8_t _iic_wait_slave_ack( type_iic dev ) 
{ 
    
    uint8_t TimeOut; 
    uint8_t retValue = LOW; 
 
    (*dev.fn_sda_in)();

    (*dev.fn_scl_high)();
    (*dev.fn_delay)();
    
    
    TimeOut = 0; 
    
    //等待低电平信号
    while( HIGH == (*dev.fn_sda_read)() ) 
    { 
        TimeOut++;
        if(  TimeOut > 250 ) 
        { 
            retValue = HIGH; 
            break; 
        } 
    } 
    
    (*dev.fn_scl_low)();
    (*dev.fn_delay)();
   
    (*dev.fn_sda_out)();

    return retValue; 
} 

/************************************************************* 
*函数名称：_iic_send_byte 
*函数功能：I2C写一字节数据 
*输入参数： 
*输出参数： 
*备           注： 
**************************************************************/ 
void _iic_send_byte( type_iic dev, uint8_t byte  ) 
{ 
    uint8_t i; 

    for( i=0; i<8; i++ ) 
    { 
        if( 0x80 & byte ) 
            (*dev.fn_sda_high)();  
        else 
            (*dev.fn_sda_low)();  
        
        byte <<= 1; 
        (*dev.fn_delay)(); 

        (*dev.fn_scl_high)(); 
        (*dev.fn_delay)(); 
        (*dev.fn_scl_low)();
        (*dev.fn_delay)(); 
    } 
} 

/************************************************************* 
*函数名称：_iic_read_byte 
*函数功能：I2C读一字节数据 
*输入参数： 
*输出参数： 
*备           注： 
**************************************************************/ 
uint8_t _iic_read_byte( type_iic dev, uint8_t ack)
{ 
    unsigned char i; 
    unsigned char value = 0; 
    unsigned char bit; 

    (*dev.fn_sda_in)();


    for( i=0; i<8; i++ ) 
    { 
        (*dev.fn_scl_high)();
        (*dev.fn_delay)(); 
        if( HIGH == (*dev.fn_sda_read)() ) 
            bit = 0x01; 
        else 
            bit = 0x00; 

        value = (value<<1)|bit; 
        (*dev.fn_scl_low)();
        (*dev.fn_delay)(); 
    } 

    
    if(ack)
        _iic_ack(dev);
    else
        _iic_no_ack(dev);

    return value; 
} 

/*************************************************
 ** Ack
 **        __
 ** SCL __|  |__
 **
 ** SDA ________
**************************************************/
void _iic_ack(type_iic dev)
{
    (*dev.fn_scl_low)();
    (*dev.fn_sda_out)();
    (*dev.fn_sda_low)();  
    
    (*dev.fn_delay)(); 

    (*dev.fn_scl_high)();
    
    (*dev.fn_delay)(); 
    (*dev.fn_scl_low)();
    
}

/*************************************************
 ** No Ack
 **        __
 ** SCL __|  |__
 **     ________
 ** SDA 
**************************************************/
void _iic_no_ack(type_iic dev)
{
    (*dev.fn_scl_low)();
    (*dev.fn_sda_out)();
    (*dev.fn_sda_high)();  
    
    (*dev.fn_delay)(); 

    (*dev.fn_scl_high)();
    
    (*dev.fn_delay)(); 
    (*dev.fn_scl_low)();    
}

/*
uint8_t I2C2_WriteByte( type_iic dev, uint8_t cmd_write, uint8_t addr, uint8_t data )
{ 
    //asm("CPSID I");  //关中断 
    _iic_start(dev); 


    _iic_send_byte( dev, cmd_write ); 

    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }

    //发送寄存器地址
    _iic_send_byte( dev, addr & 0xFF); 

    _iic_wait_slave_ack(dev);

    _iic_send_byte( dev, data ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    _iic_stop(dev); 
    //asm("CPSIE I");  //关中断 

    return 1; 
  
} 

//函数功能：E2PROM指定地址读一字节数据 
uint8_t I2C2_ReadByte( type_iic dev, uint8_t cmd_read, uint8_t addr )
{ 
    uint8_t value; 

    _iic_start(dev); 

    //包含器件地址
    _iic_send_byte( dev, cmd_read ); 
    
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }


    //发送寄存器地址
    _iic_send_byte( dev, addr & 0xFF ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    
    _iic_start(dev); 
    //包含器件地址 A2 A1 A0 = 000
    _iic_send_byte( dev, cmd_read ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    value = _iic_read_byte(dev,0); 
    _iic_stop(dev); 

    return value;   
} 

uint8_t I2C2_ReadBytes( type_iic dev, uint8_t cmd_read, uint8_t addr, uint8_t len,uint8_t *buf )
{
    uint8_t value; 

    _iic_start(dev); 

    //包含器件地址
    _iic_send_byte( dev, cmd_read ); 
    
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }


    //发送寄存器地址
    _iic_send_byte( dev, addr & 0xFF ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    
    _iic_start(dev); 
    //包含器件地址 A2 A1 A0 = 000
    _iic_send_byte( dev, cmd_read ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }

    while(len)
    {
        if(len==1)
            *buf = _iic_read_byte(dev,0);
        else
            *buf = _iic_read_byte(dev,1);
        len--;
        buf++;
    }
    _iic_stop(dev); 

    return value;      
}
*/


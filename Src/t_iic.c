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
*�������ƣ�_iic_start 
*�������ܣ�I2C��ʼ�ź� 
*��������� 
*��������� 
*��           ע��ʱ���߸�ʱ���������ɸߵ��͵����䣬��ʾI2C��ʼ�ź� 

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
*�������ƣ�_iic_stop 
*�������ܣ�I2Cֹͣ�ź� 
*��������� 
*��������� 
*��           ע��ʱ���߸�ʱ���������ɵ͵��ߵ����䣬��ʾI2Cֹͣ�ź� 
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
*�������ƣ�_iic_wait_slave_ack 
*�������ܣ�I2C�ӻ��豸Ӧ���ѯ 
*��������� 
*��������� 
*��           ע�� 
**************************************************************/ 
uint8_t _iic_wait_slave_ack( type_iic dev ) 
{ 
    
    uint8_t TimeOut; 
    uint8_t retValue = LOW; 
 
    (*dev.fn_sda_in)();

    (*dev.fn_scl_high)();
    (*dev.fn_delay)();
    
    
    TimeOut = 0; 
    
    //�ȴ��͵�ƽ�ź�
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
*�������ƣ�_iic_send_byte 
*�������ܣ�I2Cдһ�ֽ����� 
*��������� 
*��������� 
*��           ע�� 
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
*�������ƣ�_iic_read_byte 
*�������ܣ�I2C��һ�ֽ����� 
*��������� 
*��������� 
*��           ע�� 
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
    //asm("CPSID I");  //���ж� 
    _iic_start(dev); 


    _iic_send_byte( dev, cmd_write ); 

    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }

    //���ͼĴ�����ַ
    _iic_send_byte( dev, addr & 0xFF); 

    _iic_wait_slave_ack(dev);

    _iic_send_byte( dev, data ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    _iic_stop(dev); 
    //asm("CPSIE I");  //���ж� 

    return 1; 
  
} 

//�������ܣ�E2PROMָ����ַ��һ�ֽ����� 
uint8_t I2C2_ReadByte( type_iic dev, uint8_t cmd_read, uint8_t addr )
{ 
    uint8_t value; 

    _iic_start(dev); 

    //����������ַ
    _iic_send_byte( dev, cmd_read ); 
    
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }


    //���ͼĴ�����ַ
    _iic_send_byte( dev, addr & 0xFF ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    
    _iic_start(dev); 
    //����������ַ A2 A1 A0 = 000
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

    //����������ַ
    _iic_send_byte( dev, cmd_read ); 
    
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    }


    //���ͼĴ�����ַ
    _iic_send_byte( dev, addr & 0xFF ); 
    if( HIGH == _iic_wait_slave_ack(dev) ) 
    { 
        _iic_stop(dev); 
        return 0; 
    } 
    
    _iic_start(dev); 
    //����������ַ A2 A1 A0 = 000
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


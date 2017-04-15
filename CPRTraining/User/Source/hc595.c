#include "main.h"

void hc595_init(void);
void hc595_write_byte(uint8_t byte);
void hc595_latch(void);
void hc595_display(uint8_t showdata);
/***********************************************************************
函数名称：void hc595_init(void)
功    能：he595初始化程序
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void hc595_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_InitStructure.GPIO_Pin = HC595_DATA;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(HC595_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = HC595_SCLK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(HC595_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = HC595_RCLK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(HC595_PORT, &GPIO_InitStructure);

  GPIO_ResetBits(HC595_PORT, HC595_DATA);
  GPIO_ResetBits(HC595_PORT, HC595_SCLK);
  GPIO_ResetBits(HC595_PORT, HC595_RCLK);

}


/***********************************************************************
函数名称：void hc595_write_byte(uint8_t byte)
功    能：he595写字节
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void hc595_write_byte(uint8_t byte)
{
  uint8_t i, num;
  num = byte;
  for (i=0; i<8; i++)
  {
    GPIO_ResetBits(HC595_PORT, HC595_SCLK);
    if ((num&0x01)==0x01)
      GPIO_SetBits(HC595_PORT, HC595_DATA);
    else
      GPIO_ResetBits(HC595_PORT, HC595_DATA);
    GPIO_SetBits(HC595_PORT, HC595_SCLK);
    num = num>>1;
  }
}

/***********************************************************************
函数名称：void hc595_latch(void)
功    能：he595使能
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void hc595_latch(void)
{
  GPIO_ResetBits(HC595_PORT, HC595_RCLK);
  GPIO_SetBits(HC595_PORT, HC595_RCLK);
  GPIO_ResetBits(HC595_PORT, HC595_RCLK);
}

/***********************************************************************
函数名称：void hc595_display(uint8_t data[])
功    能：he595显示
输入参数：
输出参数：
编写时间：
编 写 人：
注    意：
***********************************************************************/
void hc595_display(uint8_t showdata)
{
//  hc595_write_byte(0x01);
//  hc595_write_byte(seg_table_n[data[7]]);
//  hc595_latch();
//	delay(100);
//  hc595_write_byte(0x02);
//  hc595_write_byte(seg_table_n[data[6]]);
//  hc595_latch();
//	delay(100);
//  hc595_write_byte(0x04);
//  hc595_write_byte(seg_table_n[data[5]]);
//  hc595_latch();
//	delay(100);
//  hc595_write_byte(0x08);
//  hc595_write_byte(seg_table_n[data[4]]);
//  hc595_latch();
//	delay(100);
	
	uint8_t data[4];
	data[0]=showdata%10;
	data[1]=(showdata/10)%10;
	data[2]=(showdata/100)%10;
	data[3]=(showdata/1000)%10;	
	
	
	//写第一位
  hc595_write_byte(0x01);
  hc595_write_byte(seg_table_n[data[3]]);
  hc595_latch();
  delay(100);
	//写第二位
  hc595_write_byte(0x02);
  hc595_write_byte(seg_table_n[data[2]]);
  hc595_latch();
	delay(100);
	//写第三次
	hc595_write_byte(0x04);
  hc595_write_byte(seg_table_n[data[1]]);
  hc595_latch();
  delay(100);
	//写第四次
  hc595_write_byte(0x08);
  hc595_write_byte(seg_table_n[data[0]]);
  hc595_latch();
}

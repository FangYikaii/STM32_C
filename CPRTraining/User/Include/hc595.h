#ifndef __hc595_H
#define __hc595_H



//***********************
//端口已更新 王少杰 2016.06.27
//***********************
// 74HC595
#define HC595_PORT  GPIOD
#define HC595_DATA  GPIO_Pin_10    // serial data input, pin14
#define HC595_SCLK  GPIO_Pin_8    // shift register clock input, pin11
#define HC595_RCLK  GPIO_Pin_9     // storage register clock input, pin12

extern void hc595_init(void);
extern void hc595_write_byte(uint8_t byte);
extern void hc595_latch(void);
extern void hc595_display(uint8_t showdata);


#endif

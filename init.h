/*
 * init.h
 *
 *  Created on: 28-Jan-2025
 *      Author: kadam
 */
/*
 * init.h
 *
 *  Created on: 28-Jan-2025
 *      Author: kadam
 */
// THis is a header to initialize all peripherals
#ifndef SRC_INIT_H_
#define SRC_INIT_H_

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xintc.h"
#include "xparameters.h"
#include "xil_exception.h"
#include "xtmrctr.h"
#include "xbram.h"
#include "DAC_SPI_master.h"
#include "xuartlite.h"
#include "xiic.h"
#include "DAC_CONF.h"
#include "definitions.h"

// Global GPIO instances
extern XGpio GpioPbLed	;
extern XGpio GpioPcbCtrl;
extern XGpio GpioChip;


// Global UART instances
extern XUartLite UartLiteInstance;

//BRAM 0
extern XBram_Config *ram_config;
extern XBram ram;
 //BRAM 1
extern XBram_Config *ram_config1;
extern XBram ram1;

//IIC
extern XIic IicInstance;
extern XIntc intc;

//Timer
extern XTmrCtr tmr;

// SPI IP for DAC
extern u32 *SPI_DATA; //  Data to be transmitted
extern u32 *SPI_CONTROL; // Control signals

//Function declaration
int GPIO_init(void);
int UART_init(void);
void readDataFromUARTToBRAM(void);
void readDataFromUARTToBRAM_SPI(void);
void printDataFromBRAM(void);
int UART_init(void);
int BRAM_init(void);
int I2C_RW(u8 *DAC0_VAL_CH, u8 *DAC1_VAL_CH);
void loadIicDacFromBRAM(void);
void SetUpInterruptSystem(XIntc * InstancePtr, u16 DeviceId);
static int WriteData(u8 *WriteBuffer,u16 ByteCount);
static int SetUpIIC(void);
int ReadData(u8 *BufferPtr, u16 ByteCount);
void SendHandler(XIic *InstancePtr);
void ReceiveHandler(XIic *InstancePtr);
void StatusHandler(XIic *InstancePtr, int Event);
void intc_init(void);
void TimerintcHandler(void);
void tmr_init(void);
void StartTimer(void);
void ResetBRAM(void);
void ResetBRAM_SPI(void);
void DAC_SPI_init();
void EnableTimerInterrupt(void);
void EnableIICInterrupt(void);
#endif /* SRC_INIT_H_ */

/*
 * definitions.h
 *
 *  Created on: 28-Jan-2025
 *      Author: kadam
 */

#ifndef SRC_DEFINITIONS_H_
#define SRC_DEFINITIONS_H_

#define UART_DEVICE_ID XPAR_UARTLITE_0_DEVICE_ID
#define GPIO_PB_LED XPAR_GPIO_0_DEVICE_ID
#define GPIO_PCB_CTRL XPAR_GPIO_2_DEVICE_ID
#define GPIO_CHIP XPAR_GPIO_1_DEVICE_ID
#define DAC_BUFFER_SIZE 32
#define BRAM1_DEVICE_ID XPAR_BRAM_1_DEVICE_ID
#define BRAM0_DEVICE_ID XPAR_BRAM_0_DEVICE_ID
#define PUSH_BUTTON_MASK 0x01
#define RAM1_BASE_ADDRESS XPAR_AXI_BRAM_CTRL_1_S_AXI_BASEADDR
#define RAM0_BASE_ADDRESS XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR
#define IIC_DEVICE_ID       XPAR_IIC_0_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_INTC_0_DEVICE_ID
#define IIC_INTR_ID     XPAR_INTC_0_IIC_0_VEC_ID
#define LED_CHANNEL        		2
#define PB_CHANNEL         		1
#define CTRL_CHANNEL	   		1
#define TO_CHIP_CHANNEL	   		1
#define FROM_CHIP_CHANNEL	    2
#define BRAM_OFFSET           1000
#define ACK_36 					2
#define ACK_16 					3
#define REQ_36 					12
#define REQ_16 					13
#define MUX_SEL					0b11
#define BIT_4_TO_11_MASK    0x00000FF0 // Mask for bits 4 to 11 (0b0000111111110000) // DATA from AER36
#define DATA_BUFFER_SIZE 60000
#define SPI_DAC_BUFFER_SIZE 42496 // Max Data size of the audio
#define TIMER_START_VALUE 169984000 // 1secs => 1x10^8; The duration for which the AER block is ON (1.69 seconds)
#define SAMPLING_TIME_DURATION 7475 //Sampling after approx 80us which translates to Fs of 12.5kHz
#endif /* SRC_DEFINITIONS_H_ */

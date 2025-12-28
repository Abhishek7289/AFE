// Header file
/*
 * DAC_conf.h
 *
 *  Created on: 23-Jan-2025
 *      Author: kadam
 */

#ifndef SRC_DAC_CONF_H_
#define SRC_DAC_CONF_H_



#endif /* SRC_DAC_CONF_H_ */

/*****************Parameters for Test Setup***************/

//#define IOCLK_DIV 5 //IO clock division factor

/*****************ORIGINAL DATA SET**************/
//
#define SLAVE_ADDRESS_DAC_1   0x49 // This is 7 bit address
#define SLAVE_ADDRESS_DAC_2   0x48
#define SEND_COUNT 3
#define RECEIVE_COUNT 16
#define DAC_CHANNELS 8 //No of DAC channels
#define DAC_DATA_SIZE 32
// Command Bytes
#define DAC_ADDR_CH1 0x08
#define DAC_ADDR_CH2 0x09
#define DAC_ADDR_CH3 0x0A
#define DAC_ADDR_CH4 0x0B
#define DAC_ADDR_CH5 0x0C
#define DAC_ADDR_CH6 0x0D
#define DAC_ADDR_CH7 0x0E
#define DAC_ADDR_CH8 0x0F

// Data Bytes -- Change the Data to be sent here
// Range to Enter 0 to 1023, 0=0V, 1023=1.8V
// Output Voltage = 1.8*(Entered Value)/(1024)
// DAC0
/*
#define DAC0_VAL_CH1    100	//VDDSYN  	Range 1.8V(0) 1.8V (1023)
#define DAC0_VAL_CH2    200	//VBLEAKEX	Voltage wrt 50nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
#define DAC0_VAL_CH3    300	//VBLEAKIN	Voltage wrt 50nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
#define DAC0_VAL_CH4    400	//VINNG 	Range 0V(0) 1.8V (228), Nominal 900mV(512)
#define DAC0_VAL_CH5    500	//VINPG 	Range 0V(0) 1.8V (228), Nominal 900mV(512)
#define DAC0_VAL_CH6    600	//VREST 	Range 0.9V(512) 1.2V (682), Nominal 0.95V(540)
#define DAC0_VAL_CH7 	700	//VBLEAKEXOP	Voltage wrt 50nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
#define DAC0_VAL_CH8 	800	//VBLEAKINOP	Voltage wrt 50nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
//DAC1
#define DAC1_VAL_CH1    50	//VSWLEAK 	Range 0V(0) 0.1V (57), Nominal 0mV(0)
#define DAC1_VAL_CH2    150	//
#define DAC1_VAL_CH3    250	//VBSYNIN	Voltage wrt 200nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
#define DAC1_VAL_CH4    350	//VBSYNEX	Voltage wrt 200nA to NMOS Range 0.25V(142) 0.5V (284), 0.45V(256)
#define DAC1_VAL_CH5    450	//VCC		Range 0V(0) 0.5V (284), Nominal 0.4V(227)
#define DAC1_VAL_CH6    550	//VCCBUF	Range 0V(0) 0.5V (284), Nominal 0.4V(227)
#define DAC1_VAL_CH7    650	//VDDMBUF	Range 0V(0) 0.7V (398), Nominal 0.5V(284)
#define DAC1_VAL_CH8    750	//
*/
//Push button sequence
//PB0=Center;PB1=Up; PB2=Left; PB3=Right; PB4=Down


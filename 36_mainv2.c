
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
#include "init.h"



/*
 ///////// INPUT to CHIP PIN mapping ////////////////
TO_CHIP[0]	SEL2	MUX select lines
TO_CHIP[1]	SEL1	MUX select lines
TO_CHIP[2]	DIN[30]	ACK_36
TO_CHIP[3]	DIN[31]	ACK_16
TO_CHIP[4]	DIN[32]	Enable pin

////////// OUTPUT from CHIP PIN mapping /////////////
 FROM_CHIP[1:3]   AER16[0:3] 16 channel address (4 bit)
 FROM_CHIP[4:11]  AER36[0:7] 36 channel address (8 bit)
 FROM_CHIP[12]	  DO[12]	REQ_36
 FROM_CHIP[13]	  DO[13]	REQ_16

////// PCB CONTROL PIN Mapping ///////////////
       Push button sequence
PB0=Center(0x01); PB1=Up(0x02); PB2=Left(0x04); PB3=Right(0x08); PB4=Down(0x10)

PCB_CTRL[0]   DIGIPOT reset
PCB_CTRL[1]   LED1
PCB_CTRL[3]   LED3
PCB_CTRL[2]   LED2
PCB_CTRL[4]   DAC1_CLR_FPGA
PCB_CTRL[5]   DAC0_CLR_FPGA
PCB_CTRL[6]   LDAC1_FPGA
PCB_CTRL[7]   LDAC0_FPGA

*/

int main()
{
    //init_platform();
    int Status;
	u32 buttonState;
	//u32 GPIO_read;
	u32 CHIP_read;
	// Initialize the UARTLite driver
	Status = UART_init();
	Status = GPIO_init();
	BRAM_init();
	tmr_init();
	SetUpInterruptSystem(&intc,INTC_DEVICE_ID);  //Initialize Interrupt System
 // Enable Timer and IIC Interrupts
	EnableTimerInterrupt();
	EnableIICInterrupt();
	DAC_SPI_init();

	XGpio_DiscreteWrite(&GpioPcbCtrl,CTRL_CHANNEL,0b11110000); // Make LDAC high and clear high
	XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00000001);    // Glow LED[0] of FPGA to indicate program is burned
	while (1) {
		   // Read the button state
		   buttonState = XGpio_DiscreteRead(&GpioPbLed,  PB_CHANNEL);
		   // Check if the button is pressed
		   if (buttonState == 1) { //Center PB // Reset all IOs and DACs
			   XGpio_DiscreteWrite(&GpioPcbCtrl,CTRL_CHANNEL,0x0000); // Make all PCB signal low
			   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x0000);
			   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0x0000);
			   ResetBRAM(); // Reset contents of BRAM
			   ResetBRAM_SPI(); // Reset contents of BRAM storing audio data
			   // Debounce delay
			   while (XGpio_DiscreteRead(&GpioPbLed,  PB_CHANNEL) == 1) {
				   // Wait until the button is released
			   }
		   }
		   else if(buttonState == 2){ //UP PB // Load DAC with UART value sent via MATLAB and then enable DACs
			   readDataFromUARTToBRAM(); // Reading data from UART
			   loadIicDacFromBRAM(); // Load the data from BRAM to DACs and set up DACs


			   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b11110000);             // GLOW FPGA LED 5-8 to indicate end of communication
			   while (XGpio_DiscreteRead(&GpioPbLed,  PB_CHANNEL) == 2) {
								 // Wait until the button is released
							 }
		   }

		   else if(buttonState == 4){ //LEFT PB // Load BRAM with SPI DAC and then start the AER reception
			   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b11100); // Turn oFF MUX by making SEL0SEL1= 00 Enable=1 ACK16,36 = 1
			   for (int iterationId = 1; iterationId < 11; iterationId++ ){
				   	   	   	   ResetBRAM();
				   			   ResetBRAM_SPI(); // Reset contents of BRAM
				   			   readDataFromUARTToBRAM_SPI(); // Load Audio data from MATLAB sent via UART
				   			   ///////////////STRT_ON_CHIP_COMMUNICATION//////////////////////////
				   			   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b11111);             // Make Enable in High and ACK16,36=1 and SEL signals = 11
				   			   usleep(10);
				   			   u32 receivedCount = 0;                                              // Number of REQ received
				   			   u32 TimerValue = 0;//Timer value
				   			   //XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,MUX_SEL<<4);           // GLOW FPGA LED 5 and 6 to indicate start of communication
				   			   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00110000);             // GLOW FPGA LED 5 and 6 to indicate start of communication
				   			   TimerValue = 0x00000000;
				   			   usleep(10);
				   			   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00011);             // Make Enable in low and ACK16,36=0 and SEL signals = 11
				   			   StartTimer();                                        			   // Start the timer ; THE AER work till the timer is ON
				   			   while(!XTmrCtr_IsExpired(&tmr,1)){
				   					   CHIP_read = XGpio_DiscreteRead(&GpioChip,FROM_CHIP_CHANNEL);// Read data from chip
				   					   if (CHIP_read & (1<<REQ_36)){                               // If request signal is received
				   						   CHIP_read = XGpio_DiscreteRead(&GpioChip,FROM_CHIP_CHANNEL); // Read data from chip again so that address bits are settled
				   						   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00111); // Acknowledge the request
				   						   TimerValue = XTmrCtr_GetValue(&tmr,1);                      // Get Timer Value
				   						   Xil_Out32((RAM0_BASE_ADDRESS + BRAM_OFFSET + 8*receivedCount),(CHIP_read & BIT_4_TO_11_MASK) >> 4); // Save the neuron ID
				   						   Xil_Out32((RAM0_BASE_ADDRESS + BRAM_OFFSET + 8*receivedCount + 4),TimerValue);      // Save time of spike
				   						   receivedCount++; //TimerValue++; // Send address to BRAM
				   						   usleep(5);
				   						   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00011); // Remove the Acknowledge the request
				   						   }
				   				   }
				   				   XTmrCtr_Stop(&tmr, 0); //Stop the timer
				   				   XTmrCtr_Stop(&tmr, 1); //Stop the timer
				   				   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00000);  // SHUT down the AER block
				   				   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x00);
				   				   printDataFromBRAM();
				   				   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x01); // GLow LEDs when data transfer is complete
				   				   //sleep(1); // waiting so that plotting is done at MATLAB
			   }

		   }

		   else if(buttonState == 8){// Press RIGHT push button to send BRAM data to MATLAB
		   				   XTmrCtr_Stop(&tmr, 0); //Stop the timer
		   				   XTmrCtr_Stop(&tmr, 1); //Stop the timer
		   				   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00000);  // SHUT down the AER block
		   				   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x00);
		   				   printDataFromBRAM();
		   				   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x0F); // GLow LEDs when data transfer is complete

		   			   while (XGpio_DiscreteRead(&GpioPbLed,  PB_CHANNEL) == 8) {
		   								 // Wait until the button is released
		   			   }

		   			   }


		   else if(buttonState == 16){// Press DOWN push button to send BRAM data to MATLAB
				   XTmrCtr_Stop(&tmr, 0); //Stop the timer
				   XTmrCtr_Stop(&tmr, 1); //Stop the timer
				   ResetBRAM_SPI(); // Reset contents of BRAM with Audio Data
				   ResetBRAM(); // Reset contents of BRAM
				   XGpio_DiscreteWrite(&GpioChip,TO_CHIP_CHANNEL,0b00000);  // SHUT down the AER block
				   XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0x04);

			   while (XGpio_DiscreteRead(&GpioPbLed,  PB_CHANNEL) == 16) {
								 // Wait until the button is released
			   }

			   }

	   }
	return 0;
}

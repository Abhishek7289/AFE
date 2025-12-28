/*
 * init.c
 *
 *  Created on: 28-Jan-2025
 *      Author: kadam
 */

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

XGpio GpioPbLed;
XGpio GpioPcbCtrl;
XGpio GpioChip;
XUartLite UartLiteInstance;
XBram_Config *ram_config;
XBram ram;
XBram_Config *ram_config1;
XBram ram1;
XIic IicInstance;
XIntc intc;
XTmrCtr tmr;
volatile static u8 TransmitComplete;
volatile static u8 ReceiveComplete;
u32 *SPI_DATA; //  Data to be transmitted
u32 *SPI_CONTROL; // Control signals

u32 DAC_VALUE = 0;
u32 DAC_DATA = 0;
u8 MSB_read = 0;
u8 LSB_read = 0;
u32 temp = 0x0000000F;
int DataCount=0;//Data count
int I2C_RW(u8 *DAC0_VAL_CH, u8 *DAC1_VAL_CH);

void TimerintcHandler(void *CallBackRef){

    while(!XTmrCtr_IsExpired(&tmr,0));

    //temp = ~(temp);
    //XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,temp);
	//xil_printf("Interrupt Occur\n \r");

    if ( DataCount < SPI_DAC_BUFFER_SIZE/2){
  	   	 MSB_read = Xil_In8(RAM1_BASE_ADDRESS+2*DataCount);
  	   	 LSB_read = Xil_In8(RAM1_BASE_ADDRESS+2*DataCount+1);
  	     DAC_DATA =  ((uint32_t)MSB_read << 8) | (uint32_t)LSB_read;
  	     //DAC_DATA =  temp;
		*SPI_DATA = DAC_DATA;
		*SPI_CONTROL = 0x00000003; //[load = 1,clr = 1]
		*SPI_CONTROL = 0x00000001; //[load = 0,clr = 1]
		DataCount = DataCount + 1;
		}
    else
    	{DataCount = 0;}
	XTmrCtr_Reset(&tmr, 0);// Reset the timer after the ISR excecution.
}

void StartTimer(){
	// set Timer 0 to generate interrupts
	//Timer Interrupt
			//XIntc_Connect(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID, (XInterruptHandler)TimerintcHandler, &tmr);
			//XIntc_Enable(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID);
	        XTmrCtr_Stop(&tmr, 0);
	    	u32 option = XTmrCtr_GetOptions(&tmr, 0);
	        XTmrCtr_SetOptions(&tmr, 0, option | XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION);
	        XTmrCtr_SetResetValue(&tmr, 0, SAMPLING_TIME_DURATION);
	        XTmrCtr_Reset(&tmr, 0);
	        XTmrCtr_Start(&tmr, 0);
	       // set Timer 1 to generate count from 0
	        XTmrCtr_Stop(&tmr, 1);
	        XTmrCtr_SetResetValue(&tmr, 1 , TIMER_START_VALUE); // 2secs => 2x10^8
	        option = XTmrCtr_GetOptions(&tmr, 1);
	        XTmrCtr_SetOptions(&tmr, 1 , option | XTC_DOWN_COUNT_OPTION );
			XTmrCtr_Reset(&tmr, 1);
			XTmrCtr_Start(&tmr, 1);
			//u32 receivedCount = 0;
			XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00010000);             // GLOW FPGA LED  6 to indicate start of timer

}


int GPIO_init(){
	// Initialize the GPIO driver PB and LED
	    int Status = XGpio_Initialize(&GpioPbLed, GPIO_PB_LED);
	    if (Status != XST_SUCCESS) {
	        xil_printf("GPIO Initialization failed\n\r");
	        return XST_FAILURE;
	    }
	    XGpio_SetDataDirection(&GpioPbLed, PB_CHANNEL, 0xFF);
	    XGpio_SetDataDirection(&GpioPbLed, LED_CHANNEL, 0x00);
	// Initialize the GPIO driver for CHIP signals
	    Status = XGpio_Initialize(&GpioChip, GPIO_CHIP);
	    if (Status != XST_SUCCESS) {
	        xil_printf("GPIO Initialization failed\n\r");
	        return XST_FAILURE;
	    }
	    XGpio_SetDataDirection(&GpioChip, TO_CHIP_CHANNEL, 0x00);
	    XGpio_SetDataDirection(&GpioChip, FROM_CHIP_CHANNEL, 0xFF);
	// Initialize the GPIO driver for PCB control
	    Status = XGpio_Initialize(&GpioPcbCtrl, GPIO_PCB_CTRL);
	    if (Status != XST_SUCCESS) {
	        xil_printf("GPIO Initialization failed\n\r");
	        return XST_FAILURE;
	    }
	    XGpio_SetDataDirection(&GpioPcbCtrl, CTRL_CHANNEL, 0x00);

	    // Making all GPIOs status to 0
	    XGpio_DiscreteWrite(&GpioPbLed, LED_CHANNEL, 0x00);
	    XGpio_DiscreteWrite(&GpioChip, TO_CHIP_CHANNEL, 0x00);
	    XGpio_DiscreteWrite(&GpioPcbCtrl, CTRL_CHANNEL, 0x00);
	    return Status;
}


int UART_init()
{
	 int status = XUartLite_Initialize(&UartLiteInstance, UART_DEVICE_ID);
	    if (status != XST_SUCCESS) {
	        //xil_printf("UART Initialization failed\n\r");
	        return XST_FAILURE;
	    } return status;

}




void readDataFromUARTToBRAM() {
    int receivedCount = 0;
    u8 buffer;

    while (receivedCount < DAC_BUFFER_SIZE) { // Wait till all data specified by BUFFER_SIZE is complete
        if (XUartLite_Recv(&UartLiteInstance, &buffer, 1) == 1) {
            Xil_Out8((RAM0_BASE_ADDRESS + receivedCount),buffer);receivedCount++; //Sending DATA to BRAM
        }
    }

   //xil_printf("Data stored in BRAM.\n\r");
   while(XUartLite_IsSending(&UartLiteInstance)); // Wait till all data is sent
   XUartLite_ResetFifos(&UartLiteInstance); // RESET all buffers
}

void readDataFromUARTToBRAM_SPI() {
    int receivedCount = 0;
    u8 buffer;

    //while (receivedCount < SPI_DAC_BUFFER_SIZE) { // Wait till all data specified by BUFFER_SIZE is complete
    while (receivedCount < SPI_DAC_BUFFER_SIZE) { // Wait till all data specified by BUFFER_SIZE is complete
        if (XUartLite_Recv(&UartLiteInstance, &buffer, 1) == 1) {
            Xil_Out8((RAM1_BASE_ADDRESS + receivedCount),buffer);receivedCount++; //Sending audio DATA to BRAM
        }
    }

   //xil_printf("Data stored in BRAM.\n\r");
   while(XUartLite_IsSending(&UartLiteInstance)); // Wait till all data is sent
   XUartLite_ResetFifos(&UartLiteInstance); // RESET all buffers
}

void printDataFromBRAM() {
    u8 buffer;
    //xil_printf("Data in BRAM: \n\r");
    // Read data from BRAM
    for (int i = 0; i < DATA_BUFFER_SIZE; i++) {
        buffer = Xil_In8(RAM0_BASE_ADDRESS + BRAM_OFFSET +i);
    	//xil_printf("%", buffer);
    	XUartLite_SendByte(XPAR_AXI_UARTLITE_0_BASEADDR, buffer); // sending data via UART
    }
	while(XUartLite_IsSending(&UartLiteInstance)); // Wait till all data is sent
    XUartLite_ResetFifos(&UartLiteInstance); // RESET all buffers
}



int BRAM_init(){

    // Initialize the BRAM driver
    ram_config = XBram_LookupConfig(BRAM0_DEVICE_ID); // BRAM for IIC and AER data
    int Status =  XBram_CfgInitialize(&ram, ram_config, ram_config->CtrlBaseAddress);
    ram_config1 = XBram_LookupConfig(BRAM1_DEVICE_ID); //BRAM for Audio data
    Status =  XBram_CfgInitialize(&ram1, ram_config1, ram_config1->CtrlBaseAddress);
    //Status = XBram_Initialize(&BramInstance, BRAM_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        //xil_printf("BRAM Initialization failed\n\r");
        return XST_FAILURE;
    }return Status;
}

void StatusHandler(XIic *InstancePtr, int Event)
{

}
void ReceiveHandler(XIic *InstancePtr)
{
    ReceiveComplete = 0;
}
void SendHandler(XIic *InstancePtr)
{
    TransmitComplete = 0;
}
/*****************************************************************************/
/**
* This function reads a data from the IIC Slave into a specified buffer.
*
* @param    BufferPtr contains the address of the data buffer to be filled.
* @param    ByteCount contains the number of bytes to be read.
*
* @return   XST_SUCCESS if successful else XST_FAILURE.
*
* @note     None.
*
******************************************************************************/
int ReadData(u8 *BufferPtr, u16 ByteCount)
{
    int Status;
    ReceiveComplete = 1;
    Status = XIic_Start(&IicInstance);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    Status = XIic_MasterRecv(&IicInstance, BufferPtr, ByteCount);

    while ((ReceiveComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

    }

    /*
     * Stop the IIC device.
     */
    Status = XIic_Stop(&IicInstance);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
/*****************************************************************************/
/******
 * This function configures and initializes IIC and IIC Interrupt Handlers.
 */
static int SetUpIIC(void)
{
    int Status;
    XIic_Config *ConfigPtr; // Pointer to configuration data
    //Initialize the IIC driver so that it is ready to use.

   ConfigPtr = XIic_LookupConfig(IIC_DEVICE_ID);
   if (ConfigPtr == NULL) {
       return XST_FAILURE;
   }

   Status = XIic_CfgInitialize(&IicInstance, ConfigPtr,
                   ConfigPtr->BaseAddress);
   if (Status != XST_SUCCESS) {
       return XST_FAILURE;
   }


   //Setup the Interrupt System.
   //Status=InitInterruptController(&InterruptController,INTC_DEVICE_ID);
   if (Status != XST_SUCCESS) {
       return XST_FAILURE;
   }

   Status = XIntc_Connect(&intc, IIC_INTR_ID,
                  (XInterruptHandler) XIic_InterruptHandler,
                  &IicInstance);
   if (Status != XST_SUCCESS) {
       return XST_FAILURE;
   }

   //Enable IIC Interrupt
   XIntc_Enable(&intc, IIC_INTR_ID);

   // Set the Transmit, Receive and Status handlers.

   XIic_SetSendHandler(&IicInstance, &IicInstance,
               (XIic_Handler) SendHandler);
   XIic_SetRecvHandler(&IicInstance, &IicInstance,
               (XIic_Handler) ReceiveHandler);
   XIic_SetStatusHandler(&IicInstance, &IicInstance,
                 (XIic_StatusHandler) StatusHandler);

    return XST_SUCCESS;
}


/*****************************************************************************/
/**
* This function writes a buffer of data to IIC Slave.
*
* @param    ByteCount contains the number of bytes in the buffer to be
*       written.
*
* @return   XST_SUCCESS if successful else XST_FAILURE.
*
* @note     None.
*
******************************************************************************/
static int WriteData(u8 *WriteBuffer,u16 ByteCount)
{
    int Status;
    //int BusBusy;

    /*
     * Set the defaults.
     */
    TransmitComplete = 1;

    /*
     * Start the IIC device.
     */
    Status = XIic_Start(&IicInstance);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * Send the Data.
     */
    Status = XIic_MasterSend(&IicInstance, WriteBuffer, ByteCount);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    /*
     * Wait till data is transmitted.
     */
    while ((TransmitComplete) || (XIic_IsIicBusy(&IicInstance) == TRUE)) {

    }

    /*
     * Stop the IIC device.
     */
    Status = XIic_Stop(&IicInstance);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}

void SetUpInterruptSystem(XIntc * InstancePtr, u16 DeviceId)
{
	int Status;

    if (InstancePtr->IsStarted == XIL_COMPONENT_IS_STARTED) {
        return XST_SUCCESS;
    }


	// Initialize the interrupt controller driver so that it is ready to use.

	Status = XIntc_Initialize(InstancePtr, DeviceId);
	if (Status != XST_SUCCESS) {
		xil_printf("Initialization Failed\r\n");
		return XST_FAILURE;
	}

    // Connect Timer Interrupt Handler
    Status = XIntc_Connect(InstancePtr, XPAR_INTC_0_TMRCTR_0_VEC_ID,
                           (XInterruptHandler)TimerintcHandler, &tmr);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    // Connect IIC Interrupt Handler
    Status = XIntc_Connect(InstancePtr, XPAR_INTC_0_IIC_0_VEC_ID,
                           (XInterruptHandler)XIic_InterruptHandler, &IicInstance);
    if (Status != XST_SUCCESS) return XST_FAILURE;


	Status = XIntc_Start(InstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		xil_printf("Intc Start Failed\r\n");
		return XST_FAILURE;
	}


	//Initialize the exception table.
	Xil_ExceptionInit();

	//Register the interrupt controller handler with the exception table.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				(Xil_ExceptionHandler)XIntc_InterruptHandler,
				InstancePtr);

	//Enable exceptions.
	Xil_ExceptionEnable();



	//XIntc_Start(&intc, XIN_REAL_MODE);

	//return XST_SUCCESS;

}

void loadIicDacFromBRAM(){

	u8 DAC0DATA[2*DAC_CHANNELS];
	u8 DAC1DATA[2*DAC_CHANNELS]; // multiplier 2 is for MSB + LSB
	    //xil_printf("Data in BRAM: \n\r");
	    // Read data from BRAM
	    for (int i = 0; i < DAC_DATA_SIZE/2; i++) {
	    	DAC0DATA[i] = Xil_In8(RAM0_BASE_ADDRESS+i);
	    	DAC1DATA[i] = Xil_In8(RAM0_BASE_ADDRESS+i+DAC_DATA_SIZE/2);
	    }
	//u16 DAC0_VAL_CH[DAC_CHANNELS]={DAC0_VAL_CH1, DAC0_VAL_CH2, DAC0_VAL_CH3, DAC0_VAL_CH4, DAC0_VAL_CH5, DAC0_VAL_CH6, DAC0_VAL_CH7, DAC0_VAL_CH8};
	//u16 DAC1_VAL_CH[DAC_CHANNELS]={DAC1_VAL_CH1, DAC1_VAL_CH2, DAC1_VAL_CH3, DAC1_VAL_CH4, DAC1_VAL_CH5, DAC1_VAL_CH6, DAC1_VAL_CH7, DAC1_VAL_CH8};


	XGpio_DiscreteWrite(&GpioPcbCtrl,CTRL_CHANNEL,0b11110000); // Glow LED[0] and Make LDAC high and clear high
	usleep(10);
	int Status = I2C_RW(DAC0DATA,DAC1DATA);
	XGpio_DiscreteWrite(&GpioPcbCtrl,CTRL_CHANNEL,0b00110110); // Glow LED[1-2] and Make LDAC low and clear high This loads DAC output
}

int I2C_RW(u8 *DAC0_VAL_CH, u8 *DAC1_VAL_CH)
{
	/*************DAV CHANNEL VALUES*****************/
	// Command Bytes (DAC Channel address)
	u8 DAC_ADDR_CH[DAC_CHANNELS]= {DAC_ADDR_CH1, DAC_ADDR_CH2, DAC_ADDR_CH3, DAC_ADDR_CH4, DAC_ADDR_CH5, DAC_ADDR_CH6, DAC_ADDR_CH7, DAC_ADDR_CH8 };

	// Will be sending 3 frames of data
	u8 WriteBuffer[SEND_COUNT];
	u8 StartUpSequence[SEND_COUNT];

    int Status;

    // Setup IIC Interrupt
    Status = SetUpIIC();
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Initializing data
    // Need to be sent twice
    StartUpSequence[0] = 0x01;
    StartUpSequence[1] = 0x00;
    StartUpSequence[2] = 0x00;

    ////////////////////For DAC0 Data write//////////////////
    // Set the Address of the Slave.
    Status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE,
                 SLAVE_ADDRESS_DAC_1);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    // Reset the Device
    Status = WriteData(StartUpSequence,SEND_COUNT);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    // Write data
    for(u8 i = 0; i<DAC_CHANNELS;i++) {
	    WriteBuffer[0]=DAC_ADDR_CH[i];
	    WriteBuffer[1]=DAC0_VAL_CH[2*i];
	    WriteBuffer[2]=DAC0_VAL_CH[2*i+1];
	    Status = WriteData(WriteBuffer,SEND_COUNT);
	    if (Status != XST_SUCCESS) {
		    return XST_FAILURE;
	    }
    }

    ////////////////////For DAC1 Data write//////////////////
    // Set the Address of the Slave.
    Status = XIic_SetAddress(&IicInstance, XII_ADDR_TO_SEND_TYPE,
                 SLAVE_ADDRESS_DAC_2);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    // Reset the Device
    Status = WriteData(StartUpSequence,SEND_COUNT);
    if (Status != XST_SUCCESS) {
        return XST_FAILURE;
    }
    // Write data
    for(u8 i = 0; i<DAC_CHANNELS;i++) {
    	    WriteBuffer[0]=DAC_ADDR_CH[i];
    	    WriteBuffer[1]=DAC1_VAL_CH[2*i];
    	    WriteBuffer[2]=DAC1_VAL_CH[2*i+1];
    	    Status = WriteData(WriteBuffer,SEND_COUNT);
    	    if (Status != XST_SUCCESS) {
    		    return XST_FAILURE;
    	    }
        }


    return XST_SUCCESS;
}



void tmr_init(){

	int status = XTmrCtr_Initialize(&tmr, XPAR_AXI_TIMER_0_DEVICE_ID);

	//if(status == XST_SUCCESS)
		//xil_printf("TMR INIT SUCCESSFUL\n \r");
	//else
		//xil_printf("TMR INIT FAILED\n");
}


void ResetBRAM(){
	// Resetting contents of BRAM
	   for (int i = 0;i<DATA_BUFFER_SIZE;i++){
		   Xil_Out8((RAM0_BASE_ADDRESS + BRAM_OFFSET + i),0x00);
	   }
}
void ResetBRAM_SPI(){
	// Resetting contents of BRAM
	   for (int i = 0;i<SPI_DAC_BUFFER_SIZE;i++){
		   Xil_Out8((RAM1_BASE_ADDRESS + i),0x01);
	   }
}

void DAC_SPI_init(){

	SPI_DATA = (u32*)(XPAR_DAC_SPI_MASTER_0_S00_AXI_BASEADDR + DAC_SPI_MASTER_S00_AXI_SLV_REG0_OFFSET); //slv_reg0 => data
	SPI_CONTROL = (u32*)(XPAR_DAC_SPI_MASTER_0_S00_AXI_BASEADDR + DAC_SPI_MASTER_S00_AXI_SLV_REG1_OFFSET); //slv_reg1[1:0] => [load,clr]
	// Sending an ACTIVE LOW CLR pulse//////////
    *SPI_CONTROL = 0x00000001; //[load = 0,clr = 1]
    *SPI_CONTROL = 0x00000000; //[load = 0,clr = 0]
    *SPI_CONTROL = 0x00000001; //[load = 0,clr = 1]
    usleep(5);
	*SPI_DATA = 0x000074F0;
	usleep(5);
	//*SPI_CONTROL = 0x00000002; //[load = 1,clr = 0]
	*SPI_CONTROL = 0x00000003; //[load = 1,clr = 1]
	*SPI_CONTROL = 0x00000001; //[load = 0,clr = 1] pulling load pin low activates the DAC
}

// Enable Timer Interrupt
void EnableTimerInterrupt() {
    XTmrCtr_SetHandler(&tmr,TimerintcHandler,&tmr);
    XIntc_Enable(&intc, XPAR_INTC_0_TMRCTR_0_VEC_ID);
}

// Enable IIC Interrupt
void EnableIICInterrupt() {
	XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b10000010); // Glow LED[0] and Make LDAC high and clear high
	//XIntc_Enable(&intc, XPAR_INTC_0_IIC_0_VEC_ID);
	//XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b01000010); // Glow LED[0] and Make LDAC high and clear high
   // XIic_SetSendHandler(&IicInstance, &IicInstance,
   //               (XIic_Handler) SendHandler);
   //           XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00001000); // Glow LED[0] and Make LDAC high and clear high
   // XIic_SetRecvHandler(&IicInstance, &IicInstance,
	//               (XIic_Handler) ReceiveHandler);
	//			XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00000100); // Glow LED[0] and Make LDAC high and clear high
    //XIic_SetStatusHandler(&IicInstance, &IicInstance,
	//                 (XIic_StatusHandler) StatusHandler);
				//XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00000010); // Glow LED[0] and Make LDAC high and clear high

	//XGpio_DiscreteWrite(&GpioPbLed,LED_CHANNEL,0b00000000); // Glow LED[0] and Make LDAC high and clear high
}

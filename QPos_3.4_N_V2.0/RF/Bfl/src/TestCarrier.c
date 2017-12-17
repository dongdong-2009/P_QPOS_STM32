//> =============================================
//> RC663 Script for (Iso14443-3A protocol):
//>   * ReqA
//>   * Get UID (Select: Casade level 1)
//>   * HaltA 
//> 
//> Note: Only one PICC shall be in HF
//> =============================================

//> =============================================
//> RC663 ApplyProtocolSettings:  ISO14443A=01
//> =============================================


#include "phcsBflRegCtl.h"
#include "ph_TypeDefs.h"
#include "TestCarrier.h"


/*******************************************************************************
**   TestCarrier Function
*******************************************************************************/
phStatus_t Test_Carrier (void)
{
    phStatus_t status_test;
    phcsBflRegCtl_GetRegParam_t reg_params_get;
	phcsBflRegCtl_SetRegParam_t reg_params_set;
	
	/*******************Configure Timers***********************/
	
	//Set Timer-0, T0Control_Reg
    reg_params_set.address = 0x0F;
	reg_params_set.reg_data = 0x98;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	//Set Timer-1, T1Control_Reg:
    reg_params_set.address = 0x14;
	reg_params_set.reg_data = 0x92;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	// Set Timer-2, T2Control_Reg:	Timer used for LFO trimming
	reg_params_set.address = 0x19;
	reg_params_set.reg_data = 0x20;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
    // Set Timer-2 reload value (T2ReloadHi_Reg and T2ReloadLo_Reg)
    reg_params_set.address = 0x1A;
	reg_params_set.reg_data = 0x03;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	reg_params_set.address = 0x1B;
	reg_params_set.reg_data = 0xFF;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	// Set Timer-3, T3Control_Reg:
    // Not started automatically. Not reloaded. Input clock 13.56 MHz'
    reg_params_set.address = 0x1E;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);

    
	/****************Configure FIFO Size=255 and Water-level****************/
	// Set FifoControl_Reg, Fifo size=255 bytes. Flush FIFO
    reg_params_set.address = 0x02;
	reg_params_set.reg_data = 0x90;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	// Set WaterLevel =(FIFO length -1)
    reg_params_set.address = 0x03;
	reg_params_set.reg_data = 0xFE;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	HAL_Delay_us(50);
	// RxBitCtrl_Reg(0x0c)	Received bit after collision are replaced with 1.
    reg_params_set.address = 0x0C;
	reg_params_set.reg_data = 0x80;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	HAL_Delay_us(50);	
    // DrvMod reg(0x28), Tx2Inv=1
    reg_params_get.address = 0x28;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get); 
	reg_params_set.address = 0x28;
	reg_params_set.reg_data = 0x80;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	HAL_Delay_us(50);
 	// TxAmp_Reg(0x29)
    reg_params_set.address = 0x29;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
 	// DrvCon_Reg(0x2A)
   	reg_params_set.address = 0x2A;
	reg_params_set.reg_data = 0x01;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 	
	HAL_Delay_us(50);
    // TxI_Reg(0x05),(0x05)
   	reg_params_set.address = 0x2B;
	reg_params_set.reg_data = 0x05;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	// RxSOFD_Reg(0x34),(0x00), 
   	reg_params_set.address = 0x34;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);
	// Rcv_Reg(0x38),(0x12) 
   	reg_params_set.address = 0x38;
	reg_params_set.reg_data = 0x12;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	HAL_Delay_us(50);

	//> =============================================
    //> 2. LoadProtocol( bTxProtocol=0, bRxProtocol=0)
    //> =============================================
    
	//> Terminate any running command. Flush_FiFo
	reg_params_set.address = 0x00;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x02;
	reg_params_set.reg_data = 0xB0;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
    // Clear all IRQ 0,1 flags 
   	reg_params_set.address = 0x06;
	reg_params_set.reg_data = 0x7F;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x07;
	reg_params_set.reg_data = 0x7F;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
	//> Write in Fifo: Tx and Rx protocol numbers(0,0) 
	reg_params_get.address = 0x04;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	
	reg_params_set.address = 0x05;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x05;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);     

     
	/*****************Enable IRQ0 interrupt sources ****************/
    
	// Idle interrupt(Command terminated), RC663_BIT_IDLEIRQ=0x10
	reg_params_get.address = 0x08;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x10;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	// Enable Global IRQ propagation.
   	reg_params_get.address = 0x09;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	
	reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x40;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    //> Start RC663 command "Load Protocol"=0x0d
   	reg_params_set.address = 0x00;
	reg_params_set.reg_data = 0x0D;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    //L_LoadProtocol
    reg_params_get.address = 0x07;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	// Disable Irq 0,1 sources
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);       
	//> Flush Fifo. Read Error Reg
	reg_params_set.address = 0x02;
	reg_params_set.reg_data = 0xB0;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 

	reg_params_get.address = 0x0A;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);   
 
    /******************* Apply RegisterSet**********************/
 	//> Configure CRC-16 calculation, preset value(0x6363) for Tx&Rx
   	reg_params_set.address = 0x2C;
	reg_params_set.reg_data = 0x18;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x2D;
	reg_params_set.reg_data = 0x18;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x2E;
	reg_params_set.reg_data = 0x08;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    // Length of the pulse modulation in carrier clks+1  
   	reg_params_set.address = 0x2F;
	reg_params_set.reg_data = 0x20;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    // Symbol 1 and 0 burst lengths = 8 bits.
	reg_params_set.address = 0x30;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
	// Start symbol=Symbol2, Stop symbol=Symbol3
	reg_params_set.address = 0x33;
	reg_params_set.reg_data = 0xCF;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
	// Set Rx Baudrate 106 kBaud	
	reg_params_set.address = 0x35;
	reg_params_set.reg_data = 0x04;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);   	  
	// Set min-levels for Rx and phase shift   
	reg_params_set.address = 0x37;
	reg_params_set.reg_data = 0x32;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x39;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);	 
	// Set Rx waiting time	
	reg_params_set.address = 0x36;
	reg_params_set.reg_data = 0x90;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x31;
	reg_params_set.reg_data = 0xC0;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x32;
	reg_params_set.reg_data = 0x0B;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);	
	// Set Timeout. Write T0,T1 reload values(hi,Low)
	reg_params_set.address = 0x10;
	reg_params_set.reg_data = 0x08;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x11;
	reg_params_set.reg_data = 0xD8;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	reg_params_set.address = 0x15;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);      
	reg_params_set.address = 0x16;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);    
	// Write DrvMod register   
	reg_params_set.address = 0x28;
	reg_params_set.reg_data = 0x81;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  	   
	//> MIFARE Crypto1 state is further disabled.
	reg_params_set.address = 0x0B;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  	
	//>  FieldOn
	reg_params_set.address = 0x28;
	reg_params_set.reg_data = 0x89;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
    //< =============================================
    //<  I14443p3a_Sw_RequestA
    //< =============================================
	reg_params_set.address = 0x31;
	reg_params_set.reg_data = 0xC0;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x32;
	reg_params_set.reg_data = 0x0B;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);  
    //< Set timeout for this command cmd. Init reload values for timers-0,1 
    reg_params_set.address = 0x10;
	reg_params_set.reg_data = 0x08;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x11;
	reg_params_set.reg_data = 0x94;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x15;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x16;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x06;
	reg_params_set.reg_data = 0x08;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x36;
	reg_params_set.reg_data = 0x90;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x2E;
	reg_params_set.reg_data = 0x0F;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	//< ---------------------
    //< Send the ReqA command  
    //< ---------------------
    //< Terminate any running command. FlushFifo
	reg_params_set.address = 0x00;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x02;
	reg_params_set.reg_data = 0xB0;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x06;
	reg_params_set.reg_data = 0x7F;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
	reg_params_set.address = 0x07;
	reg_params_set.reg_data = 0x7F;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	//< Write ReqA=26 into FIFO
    reg_params_set.address = 0x05;
	reg_params_set.reg_data = 0x26;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set); 
   
	//< Start RC663 command "Transcieve"=0x07. Activate Rx after Tx finishes.
	reg_params_set.address = 0x00;
	reg_params_set.reg_data = 0x07;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	/*
    //< Wait until the command is finished. Enable IRQ sources.
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x18;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x42;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_get.address = 0x07;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get); 
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	//< Wait until reception
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x54;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x42;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_get.address = 0x07;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get); 
	reg_params_set.address = 0x08;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
    reg_params_set.address = 0x09;
	reg_params_set.reg_data = 0x00;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
	//< Read FIFO, expected: AtqA
	*/
    /*
	reg_params_get.address = 0x04;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
    reg_params_get.address = 0x05;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
    reg_params_get.address = 0x05;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	reg_params_get.address = 0x0A;
	status_test = phcsBflRegCtl_SpiHw1GetReg(&reg_params_get);
	reg_params_set.address = 0x2E;
	reg_params_set.reg_data = 0x08;
	status_test = phcsBflRegCtl_SpiHw1SetReg(&reg_params_set);
*/	
	return PH_ERR_SUCCESS;

}




























	


	
	



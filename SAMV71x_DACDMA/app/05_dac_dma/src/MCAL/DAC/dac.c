/****************************************************************************************************/
/**
\file       dac.c
\brief      MCAL abstraction level - Digital to Analog Converter Controller configuration and interrupt handling.
\author     Abraham Tezmol
\version    1.0
\project    Tau 
\date       26/September/2016
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/
/** dac function prototypes and definitions */
#include "Std_Types.h"

#include "dac.h"
#include "dac_dma.h"
#include "xdmad.h"
/** Power Management Controller */
#include "pmc.h"
/** ECG sample data */
#include "ecg_data.h"

#include "tc.h"

/*****************************************************************************************************
* Definition of module wide MACROs / #DEFINE-CONSTANTs 
*****************************************************************************************************/
/** SAMPLES per cycle*/
#define SAMPLES         27778
#define TEST_CHANNEL    5
#define TIMER_1MS       9375

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/
/** */
uint32_t dacBuffer[SAMPLES];
uint16_t prueba1=0;
uint32_t ModeReg = 0;


/** Global DMA driver for all transfer */
sXdmad dmad;
/** Global DAC DMA instance */
 DacDma Dacd;
/** DAC command instance */
DacCmd DacCommand;
/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/*****************************************************************************************************
* Code of module wide FUNCTIONS
*****************************************************************************************************/

/****************************************************************************************************/
/**
* \brief    SysTick - Initialization
* \author   Abraham Tezmol
* \param    int32_t base_freq	--> Requested SysTick interrupt frequency
* \param    tPtr_to_function sysTick_handler	--> Callback function to handle SysTick ISR.
* \return   int8_t --> 0 - Successful configuration of SysTick IRQ and SysTick Timer, 1 - Invalid configuration
* \todo
*/
void dac_initialization(void)
{
	uint32_t i;
  uint32_t div;
  uint32_t tcclks;
	/* Enable Digital to Analog Converter Controller */
	PMC_EnablePeripheral(ID_DACC);
	/* Command a Soft-reset onto Digital to Analog Converter Controller */
	DACC_SoftReset(DACC);
	/* Enable Invidivual channels */
  
	/* Configure trigger mode of the Digital to Analog Converter Controller:
	Mode = 0 --> Disabling Trigger mode --> Free-running or Max speed mode on the status of DACC_MR.MAXSx
	Mode = 1 --> Trigger mode enabled 
	*/
  DACC_CfgTrigger(DACC, DACC_TRIGR_TRGSEL0_TRGSEL1 | DACC_TRIGR_TRGSEL1_TRGSEL1 | DACC_TRIGR_TRGEN0_EN | DACC_TRIGR_TRGEN1_EN);
  
	DACC_EnableChannel(DACC, 0);
	DACC_EnableChannel(DACC, 1);
	/* Copy samples onto DAC Buffer -- to review usefullness later */
	for (i = 0; i < SAMPLES; i++)
	{
		dacBuffer[i] = ecg_resampled_integer[i] << 1;
	}
      
  /** Enable peripheral clock. */
	PMC_EnablePeripheral(ID_TC0);

	TC_Configure( TC0, 0, TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_ACPC_TOGGLE | TC_CMR_BCPB_TOGGLE );
	TC0->TC_CHANNEL[ 0 ].TC_RC = TIMER_1MS;
  
}

/**
 *  \brief Configure DAC DMA and start DMA transfer.
* \param    void
* \return   void
* \todo
*/
void dac_dmaTransfer(void)
{
	DacCommand.dacChannel = DACC_CHANNEL_0;
	DacCommand.TxSize = SAMPLES;
	DacCommand.pTxBuff = (uint8_t *)dacBuffer;
	DacCommand.loopback = 1;
	Dac_ConfigureDma(&Dacd, DACC, ID_DACC, &dmad);
	Dac_SendData(&Dacd, &DacCommand);
  TC_Start( TC0, 0 );
}

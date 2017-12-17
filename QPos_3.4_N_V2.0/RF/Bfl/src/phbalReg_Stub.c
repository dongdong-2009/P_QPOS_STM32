/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* BAL Stub Component of Reader Library Framework.
* $Author: santosh.d.araballi $
* $Revision: 1.2 $
* $Date: Fri Apr 15 09:04:54 2011 $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#include "ph_Status.h"
#include "phbalReg.h"
#include "ph_RefDefs.h"

#ifdef NXPBUILD__PHBAL_REG_STUB

#include "phbalReg_Stub.h"

//#include "RegCtl_I2cHw.h"
#include "RegCtl_SpiHw.h"

phStatus_t phbalReg_Stub_Init(
                              phbalReg_Stub_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams
                              )
{
    if (sizeof(phbalReg_Stub_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }
	PH_ASSERT_NULL (pDataParams);

    pDataParams->wId = PH_COMP_BAL | PHBAL_REG_STUB_ID;

    /* TODO: Replace and insert initialisation code here */
    pDataParams = NULL;
	
#ifdef I2C_USED
    RegCtl_I2cHwInit();
#endif

#ifdef SPI_USED
    RegCtl_SpiHwInit();
#endif

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetPortList(
    phbalReg_Stub_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    )
{
    /* TODO: Replace and insert code for getting the port list here */
    pDataParams = NULL;
    wPortBufSize = 0;
    pPortNames = NULL;
    *pNumOfPorts = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetPort(
                            phbalReg_Stub_DataParams_t * pDataParams,
                            uint8_t * pPortName
                            )
{
    /* TODO: Replace and insert code for setting a port name here */
    pDataParams = NULL;
    pPortName = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_OpenPort(
                                  phbalReg_Stub_DataParams_t * pDataParams
                                  )
{
    /* TODO: Replace and insert code for opening a bus connection here */
    pDataParams = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_ClosePort(
                                   phbalReg_Stub_DataParams_t * pDataParams
                                   )
{
    /* TODO: Replace and insert code for closing a bus connection here */
    pDataParams = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_Exchange(
                                  phbalReg_Stub_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint16_t wRxBufSize,
                                  uint8_t * pRxBuffer,
                                  uint16_t * pRxLength
                                  )
{
    /* TODO: Replace and insert code for exchanging data on the bus here */
	phStatus_t status = PH_ERR_SUCCESS;
	
//    pDataParams = NULL;
	wOption = PH_EXCHANGE_DEFAULT;
//    pTxBuffer = NULL;
//    wTxLength = 0;
//    wRxBufSize = 0;
//    pRxBuffer = NULL;
//    pRxLength = NULL;

	/* write operation ? */
	if (0 == wRxBufSize)
	{
		/* Yes -> perform write operation */
#ifdef I2C_USED
		status = RegCtl_I2cHwSetReg(pTxBuffer[0], pTxBuffer[1]);
#endif
#ifdef SPI_USED
		status = RegCtl_SpiHwSetReg(pTxBuffer[0], pTxBuffer[1]);
#endif
		/* no bytes read */
		*pRxLength = 0;
	}
	else
	{
#ifdef I2C_USED
		/* perform read operation */
		status = RegCtl_I2cHwGetReg(pTxBuffer[0], pRxBuffer);
#endif
#ifdef SPI_USED
		status = RegCtl_SpiHwGetReg(pTxBuffer[0], pRxBuffer);
#endif
		/* 2 byte read returned - first dummy byte, 2nd valid databyte */
		*pRxLength = 2;
	}

    return PH_ADD_COMPCODE(status, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   )
{
    /* TODO: Replace and insert code to configure the connection here */
    pDataParams = NULL;
    wConfig = 0;
    wValue = 0;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
    /* TODO: Replace and insert code to read connection configuration here */
    pDataParams = NULL;
    wConfig = 0;
    pValue = NULL;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_STUB */

/*
 * RegCtl_SpiHw.c
 *
 *  Created on: 19 janv. 2011
 *      Author: frq04766
 */

#include "RegCtl_SpiHw.h"
#include"RC523.h"

#define DEBUG_REG	0

/*********************************************************************/
/*
 * @brief 		Initialise the spi SSP0 for master mode
 * @param[in]	None
 *
 * @return 		None
 **********************************************************************/
void RegCtl_SpiHwInit(void)
{
	//RC523_SPI_Config();
}

/*********************************************************************/
/*
 * @brief 		Get a databyte from a register
 * @param[in]	address		Address of the register
 * @param[in]	*reg_data	Pointer to the databyte to be read
 *
 * @return 		- PH_ERR_BFL_SUCCESS
 *
 **********************************************************************/
phStatus_t RegCtl_SpiHwGetReg(uint8_t address, uint8_t *reg_data)
{
	/* load address in tx_buff */
	/* Assert the SSEL pin */
	NSS_LOW;

	//HAL_Delay_us(100);

	/* receive 2 bytes: dummy byte + valid byte */
	RC523_SPI_SendByte(address);

	reg_data[1] = RC523_SPI_ReadByte();

	/* Deassert the SSEL pin */
	//HAL_Delay_us(10);
	NSS_HIGH;

	/* return success */
	return PH_ERR_SUCCESS;
}

/*********************************************************************//**
 * @brief 		Set a databyte to a register
 * @param[in]	address		Address of the register
 * @param[in]	*reg_data	Databyte to be written
 *
 * @return 		- PH_ERR_BFL_SUCCESS
 *
 **********************************************************************/
phStatus_t RegCtl_SpiHwSetReg(uint8_t address, uint8_t reg_data)
{
#if DEBUG_REG
	debug_printf("SR");
	debug_puts_hex(address);
	debug_puts_hex(reg_data);
#endif

	/* load address and data in buffer */

	/* Assert the SSEL pin */
	NSS_LOW;
	//HAL_Delay_us(100);
	/* send SPI frame */
	RC523_SPI_SendByte(address);

	RC523_SPI_SendByte(reg_data);

	/* Deassert the SSEL pin */
	//HAL_Delay_us(10);
	NSS_HIGH;

	/* return success */
	return PH_ERR_SUCCESS;
}

/*********************************************************************//**
 * @brief 		Modify a bit in a register
 * @param[in]	address		Address of the register
 * @param[in]	*mask		Mask of the bit to set/reset
 * @param[in]	set			1: set 1: reset
 *
 * @return 		- PH_ERR_BFL_SUCCESS
 *
 **********************************************************************/
phStatus_t RegCtl_SpiHwModReg(uint8_t address, uint8_t mask, uint8_t set)
{
    uint8_t  reg_data[2];

    /* get the register first */
    RegCtl_SpiHwGetReg(address, reg_data);

	if (set)
	{
		/* The bits of the mask, set to one are set in the new data: */
		reg_data[1] |= mask;
	}
	else
	{
		/* The bits of the mask, set to one are cleared in the new data: */
		reg_data[1] &= (uint8_t)(~mask);
	}

	/* set the register */
	RegCtl_SpiHwSetReg(address, reg_data[1]);

	/* return success */
	return PH_ERR_SUCCESS;
}

/*********************************************************************/
/*
 * @brief 		Set multi data in consecutive register
 * @param[in]	buf		Address of the data buffer
 * @param[in]	Len		Len of the data buffer
 *
 * @return 		- PH_ERR_BFL_SUCCESS
 *
 **********************************************************************/
phStatus_t RegCtl_SpiHwSetMultiData(uint8_t *buf, uint32_t Len)
{	
	/* Assert the SSEL pin */
	NSS_LOW;

	for (; Len > 0; Len --)
	{
		RC523_SPI_SendByte(*buf);
		buf++;
	}
	
	/* Deassert the SSEL pin */
	NSS_HIGH;


	/* return success */
	return PH_ERR_SUCCESS;
}

/*********************************************************************/
/*
 * @brief 		Get a databyte from a register
 * @param[in]	txbuf	Address of the tx data buffer
 * @param[in]	rxbuf	Address of the rx data buffer
 * @param[in]	Len		Len of the data buffer
 *
 * @return 		- PH_ERR_BFL_SUCCESS
 *
 **********************************************************************/
phStatus_t RegCtl_SpiHwGetMultiData(uint8_t *txbuf, uint8_t *rxbuf, uint32_t Len)
{

}


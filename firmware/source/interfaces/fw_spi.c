/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "fw_spi.h"

uint8_t spi_masterReceiveBuffer_SPI0[SPI_DATA_LENGTH] = {0};
uint8_t SPI_masterSendBuffer_SPI0[SPI_DATA_LENGTH] = {0};
uint8_t spi_masterReceiveBuffer_SPI1[SPI_DATA_LENGTH] = {0};
uint8_t SPI_masterSendBuffer_SPI1[SPI_DATA_LENGTH] = {0};

void init_SPI()
{
    /* PORTD0 is configured as SPI0_CS0 */
    PORT_SetPinMux(Port_SPI_CS_C6000_U, Pin_SPI_CS_C6000_U, kPORT_MuxAlt2);

    /* PORTD1 is configured as SPI0_SCK */
    PORT_SetPinMux(Port_SPI_CLK_C6000_U, Pin_SPI_CLK_C6000_U, kPORT_MuxAlt2);

    /* PORTD2 is configured as SPI0_SOUT */
    PORT_SetPinMux(Port_SPI_DI_C6000_U, Pin_SPI_DI_C6000_U, kPORT_MuxAlt2);

    /* PORTD3 is configured as SPI0_SIN */
    PORT_SetPinMux(Port_SPI_DO_C6000_U, Pin_SPI_DO_C6000_U, kPORT_MuxAlt2);

    /* PORTB10 is configured as SPI1_CS0 */
    PORT_SetPinMux(Port_SPI_CS_C6000_V, Pin_SPI_CS_C6000_V, kPORT_MuxAlt2);

    /* PORTB11 is configured as SPI1_SCK */
    PORT_SetPinMux(Port_SPI_CLK_C6000_V, Pin_SPI_CLK_C6000_V, kPORT_MuxAlt2);

    /* PORTB16 is configured as SPI1_SOUT */
    PORT_SetPinMux(Port_SPI_DI_C6000_V, Pin_SPI_DI_C6000_V, kPORT_MuxAlt2);

    /* PORTB17 is configured as SPI1_SIN */
    PORT_SetPinMux(Port_SPI_DO_C6000_V, Pin_SPI_DO_C6000_V, kPORT_MuxAlt2);

    NVIC_SetPriority(SPI0_IRQn, 3);
    NVIC_SetPriority(SPI1_IRQn, 3);
}

void setup_SPI0()
{
    dspi_master_config_t masterConfig_SPI0;

	/*Master config*/
	masterConfig_SPI0.whichCtar = kDSPI_Ctar0;
	masterConfig_SPI0.ctarConfig.baudRate = SPI_BAUDRATE;
	masterConfig_SPI0.ctarConfig.bitsPerFrame = 8;
	masterConfig_SPI0.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
	masterConfig_SPI0.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
	masterConfig_SPI0.ctarConfig.direction = kDSPI_MsbFirst;
	masterConfig_SPI0.ctarConfig.pcsToSckDelayInNanoSec = 2000;
	masterConfig_SPI0.ctarConfig.lastSckToPcsDelayInNanoSec = 2000;
	masterConfig_SPI0.ctarConfig.betweenTransferDelayInNanoSec = 1000;

	masterConfig_SPI0.whichPcs = kDSPI_Pcs0;
	masterConfig_SPI0.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig_SPI0.enableContinuousSCK = false;
	masterConfig_SPI0.enableRxFifoOverWrite = false;
	masterConfig_SPI0.enableModifiedTimingFormat = false;
	masterConfig_SPI0.samplePoint = kDSPI_SckToSin0Clock;

	DSPI_MasterInit(SPI0, &masterConfig_SPI0, CLOCK_GetFreq(DSPI0_CLK_SRC));
}

void setup_SPI1()
{
    dspi_master_config_t masterConfig_SPI1;

	/*Master config*/
    masterConfig_SPI1.whichCtar = kDSPI_Ctar0;
    masterConfig_SPI1.ctarConfig.baudRate = SPI_BAUDRATE;
    masterConfig_SPI1.ctarConfig.bitsPerFrame = 8;
    masterConfig_SPI1.ctarConfig.cpol = kDSPI_ClockPolarityActiveLow;
    masterConfig_SPI1.ctarConfig.cpha = kDSPI_ClockPhaseFirstEdge;
    masterConfig_SPI1.ctarConfig.direction = kDSPI_MsbFirst;
    masterConfig_SPI1.ctarConfig.pcsToSckDelayInNanoSec = 2000;
    masterConfig_SPI1.ctarConfig.lastSckToPcsDelayInNanoSec = 2000;
    masterConfig_SPI1.ctarConfig.betweenTransferDelayInNanoSec = 1000;

    masterConfig_SPI1.whichPcs = kDSPI_Pcs0;
    masterConfig_SPI1.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

    masterConfig_SPI1.enableContinuousSCK = false;
    masterConfig_SPI1.enableRxFifoOverWrite = false;
    masterConfig_SPI1.enableModifiedTimingFormat = false;
    masterConfig_SPI1.samplePoint = kDSPI_SckToSin0Clock;

	DSPI_MasterInit(SPI1, &masterConfig_SPI1, CLOCK_GetFreq(DSPI1_CLK_SRC));
}

void clear_SPI_buffer_SPI0()
{
    for (uint32_t i = 0; i < SPI_DATA_LENGTH; i++)
    {
        SPI_masterSendBuffer_SPI0[i] = 0;
        spi_masterReceiveBuffer_SPI0[i] = 0;
    }
}

int write_SPI_page_reg_byte_SPI0(uint8_t page, uint8_t reg, uint8_t val)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI0();
	SPI_masterSendBuffer_SPI0[0]=page;
	SPI_masterSendBuffer_SPI0[1]=reg;
	SPI_masterSendBuffer_SPI0[2]=val;

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI0;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI0;
    masterXfer.dataSize = 3;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int read_SPI_page_reg_byte_SPI0(uint8_t page, uint8_t reg, uint8_t* val)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI0();
	SPI_masterSendBuffer_SPI0[0]=page | 0x80;
	SPI_masterSendBuffer_SPI0[1]=reg;
	SPI_masterSendBuffer_SPI0[2]=0xFF;

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI0;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI0;
    masterXfer.dataSize = 3;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	*val=spi_masterReceiveBuffer_SPI0[2];

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int set_clear_SPI_page_reg_byte_with_mask_SPI0(uint8_t page, uint8_t reg, uint8_t mask, uint8_t val)
{
    status_t status;

	uint8_t tmp_val;
	status = read_SPI_page_reg_byte_SPI0(page, reg, &tmp_val);
    if (status != kStatus_Success)
    {
    	return status;
    }
	tmp_val=val | (tmp_val & mask);
	status = write_SPI_page_reg_byte_SPI0(page, reg, tmp_val);
    if (status != kStatus_Success)
    {
    	return status;
    }

	return kStatus_Success;
}

int write_SPI_page_reg_bytearray_SPI0(uint8_t page, uint8_t reg, uint8_t* values, uint8_t length)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI0();
	SPI_masterSendBuffer_SPI0[0]=page;
	SPI_masterSendBuffer_SPI0[1]=reg;
	for (int i=0; i<length; i++)
	{
		SPI_masterSendBuffer_SPI0[i+2]=values[i];
	}

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI0;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI0;
    masterXfer.dataSize = length+2;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int read_SPI_page_reg_bytearray_SPI0(uint8_t page, uint8_t reg, uint8_t* values, uint8_t length)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI0();
	SPI_masterSendBuffer_SPI0[0]=page | 0x80;
	SPI_masterSendBuffer_SPI0[1]=reg;
	for (int i=0; i<length; i++)
	{
		SPI_masterSendBuffer_SPI0[i+2]=0xFF;
	}

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI0;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI0;
    masterXfer.dataSize = length+2;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	for (int i=0; i<length; i++)
	{
		values[i]=spi_masterReceiveBuffer_SPI0[i+2];
	}

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

void clear_SPI_buffer_SPI1()
{
    for (uint32_t i = 0; i < SPI_DATA_LENGTH; i++)
    {
        SPI_masterSendBuffer_SPI1[i] = 0;
        spi_masterReceiveBuffer_SPI1[i] = 0;
    }
}

int write_SPI_page_reg_byte_SPI1(uint8_t page, uint8_t reg, uint8_t val)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI1();
	SPI_masterSendBuffer_SPI1[0]=page;
	SPI_masterSendBuffer_SPI1[1]=reg;
	SPI_masterSendBuffer_SPI1[2]=val;

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI1;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI1;
    masterXfer.dataSize = 3;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int read_SPI_page_reg_byte_SPI1(uint8_t page, uint8_t reg, uint8_t* val)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI1();
	SPI_masterSendBuffer_SPI1[0]=page | 0x80;
	SPI_masterSendBuffer_SPI1[1]=reg;
	SPI_masterSendBuffer_SPI1[2]=0xFF;

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI1;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI1;
    masterXfer.dataSize = 3;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	*val=spi_masterReceiveBuffer_SPI1[2];

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int set_clear_SPI_page_reg_byte_with_mask_SPI1(uint8_t page, uint8_t reg, uint8_t mask, uint8_t val)
{
    status_t status;

	uint8_t tmp_val;
	status = read_SPI_page_reg_byte_SPI1(page, reg, &tmp_val);
    if (status != kStatus_Success)
    {
    	return status;
    }
	tmp_val=val | (tmp_val & mask);
	status = write_SPI_page_reg_byte_SPI1(page, reg, tmp_val);
    if (status != kStatus_Success)
    {
    	return status;
    }

	return kStatus_Success;
}

int write_SPI_page_reg_bytearray_SPI1(uint8_t page, uint8_t reg, uint8_t* values, uint8_t length)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI1();
	SPI_masterSendBuffer_SPI1[0]=page;
	SPI_masterSendBuffer_SPI1[1]=reg;
	for (int i=0; i<length; i++)
	{
		SPI_masterSendBuffer_SPI1[i+2]=values[i];
	}

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI1;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI1;
    masterXfer.dataSize = length+2;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int read_SPI_page_reg_bytearray_SPI1(uint8_t page, uint8_t reg, uint8_t* values, uint8_t length)
{
    dspi_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_SPI_buffer_SPI1();
	SPI_masterSendBuffer_SPI1[0]=page | 0x80;
	SPI_masterSendBuffer_SPI1[1]=reg;
	for (int i=0; i<length; i++)
	{
		SPI_masterSendBuffer_SPI1[i+2]=0xFF;
	}

    /*Start master transfer*/
    masterXfer.txData = SPI_masterSendBuffer_SPI1;
    masterXfer.rxData = spi_masterReceiveBuffer_SPI1;
    masterXfer.dataSize = length+2;
    masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;

    status = DSPI_MasterTransferBlocking(SPI1, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	for (int i=0; i<length; i++)
	{
		values[i]=spi_masterReceiveBuffer_SPI1[i+2];
	}

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

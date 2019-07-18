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

#include "fw_i2c.h"

uint8_t i2c_master_buff[I2C_DATA_LENGTH];

void init_I2C0a()
{
    // I2C0a to AT24C512 EEPROM & AT1846S
    const port_pin_config_t porte24_config = {/* Internal pull-up resistor is enabled */
                                              kPORT_PullUp,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive filter is disabled */
                                              kPORT_PassiveFilterDisable,
                                              /* Open drain is enabled */
                                              kPORT_OpenDrainEnable,
                                              /* Low drive strength is configured */
                                              kPORT_LowDriveStrength,
                                              /* Pin is configured as I2C0_SCL */
                                              kPORT_MuxAlt5,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    PORT_SetPinConfig(Port_I2C0a_SCL, Pin_I2C0a_SCL, &porte24_config);

    const port_pin_config_t porte25_config = {/* Internal pull-up resistor is enabled */
                                              kPORT_PullUp,
                                              /* Fast slew rate is configured */
                                              kPORT_FastSlewRate,
                                              /* Passive filter is disabled */
                                              kPORT_PassiveFilterDisable,
                                              /* Open drain is enabled */
                                              kPORT_OpenDrainEnable,
                                              /* Low drive strength is configured */
                                              kPORT_LowDriveStrength,
                                              /* Pin is configured as I2C0_SDA */
                                              kPORT_MuxAlt5,
                                              /* Pin Control Register fields [15:0] are not locked */
                                              kPORT_UnlockRegister};
    PORT_SetPinConfig(Port_I2C0a_SDA, Pin_I2C0a_SDA, &porte25_config);

    NVIC_SetPriority(I2C0_IRQn, 3);
}

void init_I2C0b()
{
	// I2C0b to ALPU-MP-1413
    const port_pin_config_t portb2_config = {/* Internal pull-up resistor is enabled */
                                             kPORT_PullUp,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is enabled */
                                             kPORT_OpenDrainEnable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as I2C0_SCL */
                                             kPORT_MuxAlt2,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    PORT_SetPinConfig(Port_I2C0b_SCL, Pin_I2C0b_SCL, &portb2_config);

    const port_pin_config_t portb3_config = {/* Internal pull-up resistor is enabled */
                                             kPORT_PullUp,
                                             /* Fast slew rate is configured */
                                             kPORT_FastSlewRate,
                                             /* Passive filter is disabled */
                                             kPORT_PassiveFilterDisable,
                                             /* Open drain is enabled */
                                             kPORT_OpenDrainEnable,
                                             /* Low drive strength is configured */
                                             kPORT_LowDriveStrength,
                                             /* Pin is configured as I2C0_SDA */
                                             kPORT_MuxAlt2,
                                             /* Pin Control Register fields [15:0] are not locked */
                                             kPORT_UnlockRegister};
    PORT_SetPinConfig(Port_I2C0b_SDA, Pin_I2C0b_SDA, &portb3_config);

    NVIC_SetPriority(I2C0_IRQn, 3);
}

void setup_I2C0()
{
    i2c_master_config_t masterConfig;

	/*
	 * masterConfig.baudRate_Bps = 100000U;
	 * masterConfig.enableStopHold = false;
	 * masterConfig.glitchFilterWidth = 0U;
	 * masterConfig.enableMaster = true;
	 */
	I2C_MasterGetDefaultConfig(&masterConfig);
	masterConfig.baudRate_Bps = I2C_BAUDRATE;

	I2C_MasterInit(I2C0, &masterConfig, CLOCK_GetFreq(I2C0_CLK_SRC));
}

void clear_I2C_buffer()
{
    for (uint32_t i = 0; i < I2C_DATA_LENGTH; i++)
    {
    	i2c_master_buff[i] = 0;
    }
}

int write_I2C_reg_2byte(uint8_t addr, uint8_t reg, uint8_t val1, uint8_t val2)
{
    i2c_master_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_I2C_buffer();
	i2c_master_buff[0] = reg;
	i2c_master_buff[1] = val1;
	i2c_master_buff[2] = val2;

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = addr;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = i2c_master_buff;
    masterXfer.dataSize = 3;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(I2C0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int read_I2C_reg_2byte(uint8_t addr, uint8_t reg, uint8_t* val1, uint8_t* val2)
{
    i2c_master_transfer_t masterXfer;
    status_t status;

	taskENTER_CRITICAL();
	clear_I2C_buffer();
	i2c_master_buff[0] = reg;

    memset(&masterXfer, 0, sizeof(masterXfer));
    masterXfer.slaveAddress = addr;
    masterXfer.direction = kI2C_Write;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = i2c_master_buff;
    masterXfer.dataSize = 1;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(I2C0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

	clear_I2C_buffer();

    masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
    masterXfer.direction = kI2C_Read;
    masterXfer.subaddress = 0;
    masterXfer.subaddressSize = 0;
    masterXfer.data = i2c_master_buff;
    masterXfer.dataSize = 2;
    masterXfer.flags = kI2C_TransferDefaultFlag;

    status = I2C_MasterTransferBlocking(I2C0, &masterXfer);
    if (status != kStatus_Success)
    {
    	taskEXIT_CRITICAL();
    	return status;
    }

    *val1 = i2c_master_buff[0];
    *val2 = i2c_master_buff[1];

	taskEXIT_CRITICAL();
	return kStatus_Success;
}

int set_clear_I2C_reg_2byte_with_mask(uint8_t reg, uint8_t mask1, uint8_t mask2, uint8_t val1, uint8_t val2)
{
    status_t status;

	uint8_t tmp_val1;
	uint8_t tmp_val2;
	status = read_I2C_reg_2byte(I2C_MASTER_SLAVE_ADDR_7BIT, reg, &tmp_val1, &tmp_val2);
    if (status != kStatus_Success)
    {
    	return status;
    }
	tmp_val1=val1 | (tmp_val1 & mask1);
	tmp_val2=val2 | (tmp_val2 & mask2);
	status = write_I2C_reg_2byte(I2C_MASTER_SLAVE_ADDR_7BIT, reg, tmp_val1, tmp_val2);
    if (status != kStatus_Success)
    {
    	return status;
    }

	return kStatus_Success;
}

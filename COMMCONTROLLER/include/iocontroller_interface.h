/*
Vic's IO Board V1.0 Copyright (C) 2017 Vidas Simkus

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef IOCONTROLLER_INTERFACE_H
#define	IOCONTROLLER_INTERFACE_H

#include "support.h"

/**
 * \file Various methods to communicate with the IO controller
 *
 * The IO controller has to PMICs (power management ICs)
 * PMIC 1 == AI PMIC
 * PMIC 2 == DO PMIC
 */

/**
 * Each IOCONTROLLER provides four digital outputs.
 */
#define IOC_DO_COUNT 4

/**
 * Each IOCONTROLLER provides 8 analog inputs:
 *		4 x 4-20 mA
 *		4 x ICTD
 */
#define IOC_AI_COUNT 8

/**
 * Registers on the IO controller.
 *
 * Registers < 0x07 are used by the I2C library for various super cool things.
 */
#define IOC_I2C_REG_GET_AI 0x07
#define IOC_I2C_REG_GET_DO 0x08
#define IOC_I2C_REG_SET_DO 0x09
#define IOC_I2C_REG_GET_PMIC 0x0A
#define IOC_I2C_REG_SET_PMIC 0x0B
#define IOC_I2C_REG_GET_L1_CAL 0x0C
#define IOC_I2C_REG_GET_L2_CAL 0x0D
#define IOC_I2C_REG_SET_L1_CAL 0x0E
#define IOC_I2C_REG_SET_L2_CAL 0x0F
#define IOC_I2C_REG_GET_BOOT_COUNT 0x10
#define IOC_I2C_REG_CONFIRM_OUTPUT 0x11

/**
 * ADC characteristics
 */
#define IOC_ADC_STEPS 4096
#define IOC_ADC_MAX_REF 5

/**
 * Masks for accessing the DO status bits
 */
#define DO_4_MASK 0x08
#define DO_3_MASK 0x04
#define DO_2_MASK 0x02
#define DO_1_MASK 0x01

/**
 * Masks for accessing the PMIC status bits
 */
#define PMIC_AI_EN_MASK 0x01
#define PMIC_DO_EN_MASK 0x02
#define PMIC_AI_ERR_MASK 0x04
#define PMIC_DO_ERR_MASK 0x08

/**
 * Sucks down all analog readings from the IOCONTROLLER.
 * Results are stored in IOC_AI_RAW_VALUES and IOC_AI_VOLT_VALUES
 * \return True (1) if everything went to plan, false (0) otherwise.
 */
UCHAR update_analog_readings(void);

/**
 * Gets the digital output statuses from the IOCONTROLLER.
 * \return Byte containing all digital output statuses
 */
UCHAR get_digital_ouputs(void);

/**
 * Sets the digital output status
 * \param _out Digital output statuses
 * \return True (1) if everything went to plan, false (0) otherwise.
 */
UCHAR set_digital_outputs(UCHAR _out);
/**
 * Gets the PMIC statuses from the IOCONTROLLER
 * \return Byte containing all PMIC status flags.
 */
UCHAR get_pmic_status(void);

/**
 * Sets the PMIC statuses.
 * \param _out PMIC statuses
 * \return True (1) if everything went to plan, false (0) otherwise.
 */
UCHAR set_pmic_status(UCHAR _out);

UCHAR update_l1_cal_values(void);
UCHAR update_l2_cal_values(void);
UCHAR update_cal_values(UCHAR _cmd);

UINT get_boot_count(void);

UCHAR set_cal_values(UCHAR _cmd);
UCHAR set_l1_cal_values(void);
UCHAR set_l2_cal_values(void);

UCHAR confirm_output_state(void);



#endif	/* IOCONTROLLER_INTERFACE_H */


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

#include "config.h"
#include "I2C/i2c.h"
#include <libpic30.h>

#include "I2C/i2c_callbacks.h"
#include "ioconfig.h"
#include "globals.h"

volatile static UINT curr_ad_value = 0;
volatile static UINT curr_cal_value = 0;
volatile static UCHAR curr_cal_idx = 0;

UCHAR i2c_cb_get_l1_cal(UINT _idx)
{
	turn_on_send_led();

	UCHAR ret = 0;

	if (_idx >= (AD_INPUT_NUM * 2))
	{
		ret = 0;
		goto _end;
	}

	if (_idx == 0 || (_idx % 2) == 0)
	{
		/*
		 * First read or an index of an actual analog input.
		 */

		curr_cal_value = eeprom_data.l1_cal_data_arr[_idx / 2];
		ret = curr_cal_value;
	}
	else
	{
		ret = curr_cal_value >> 8;
	}

_end:
	turn_off_send_led();
	return ret;
}

UCHAR i2c_cb_get_l2_cal(UINT _idx)
{
	turn_on_send_led();

	UCHAR ret = 0;

	if (_idx >= (AD_INPUT_NUM * 2))
	{
		ret = 0;
		goto _end;
	}

	if (_idx == 0 || (_idx % 2) == 0)
	{
		/*
		 * First read or an index of an actual analog input.
		 */

		curr_cal_value = eeprom_data.l2_cal_data_arr[_idx / 2];
		ret = curr_cal_value;
	}
	else
	{
		ret = curr_cal_value >> 8;
	}

_end:
	turn_off_send_led();
	return ret;
}

void i2c_cb_set_l1_cal(UCHAR _byte, UINT _idx)
{
	turn_on_send_led();

	if (_idx >= (AD_INPUT_NUM * 2))
	{
		goto _end;
	}

	if (_idx == 0 || (_idx % 2) == 0)
	{
		/*
		 * First read or an index of an actual analog input.
		 */

		curr_cal_idx = _idx / 2;
		eeprom_data.l1_cal_data_arr[curr_cal_idx] = _byte;
	}
	else
	{
		eeprom_data.l1_cal_data_arr[curr_cal_idx] = (eeprom_data.l1_cal_data_arr[curr_cal_idx] | (_byte << 8));
		eeprom_data.is_dirty = 1;
	}

_end:
	turn_off_send_led();
	return;
}

void i2c_cb_set_l2_cal(UCHAR _byte, UINT _idx)
{
	turn_on_send_led();

	if (_idx >= (AD_INPUT_NUM * 2))
	{
		goto _end;
	}

	if (_idx == 0 || (_idx % 2) == 0)
	{
		/*
		 * First read or an index of an actual analog input.
		 */

		curr_cal_idx = _idx / 2;
		eeprom_data.l2_cal_data_arr[curr_cal_idx] = _byte;
	}
	else
	{
		eeprom_data.l2_cal_data_arr[curr_cal_idx] = (eeprom_data.l2_cal_data_arr[curr_cal_idx] | (_byte << 8));
		eeprom_data.is_dirty = 1;
	}

_end:
	turn_off_send_led();
	return;
}

UCHAR i2c_cb_get_boot_count(UINT _idx)
{
	UCHAR ret;

	turn_on_send_led();

	if (_idx == 0)
	{
		ret = eeprom_data.boot_count;
	}
	else if (_idx == 1)
	{
		ret = eeprom_data.boot_count >> 8;
	}
	else
	{
		ret = 0xFF;
	}

	turn_off_send_led();
	return ret;
}

/*
 * Here's how this works.  We can only send one byte at a time, but each analog
 * value is two bytes wide.  So to get an analog value out to the master we need
 * two writes.  We write the LSB byte followed by the MSB
 */
UCHAR i2c_cb_get_ai_status(UINT _idx)
{
	turn_on_send_led();

	UCHAR ret = 0;

	if (_idx >= (AD_INPUT_NUM * 2))
	{
		ret = 0;
	}
	else
	{
		if (_idx == 0 || (_idx % 2) == 0)
		{
			/*
			 * Either first read or a start of a read of a new integer value.
			 * Because there's two reads per AI value, each actual AI value will be an even index.
			 * idx == 0 -- AI 0 LSB
			 * idx == 1 -- AI 0 MSB
			 * idx == 2 -- AI 1 LSB
			 * idx == 3 -- AI 1 MSB
			 */
			curr_ad_value = AD_BUFFER[_idx / 2];
			ret =  curr_ad_value;
		}
		else
		{
			ret = curr_ad_value >> 8;
		}
	}

	turn_off_send_led();
	return ret;
}

UCHAR i2c_cb_get_do_status(UINT _idx)
{
	turn_on_send_led();

	if (_idx > 0)
	{
		turn_off_send_led();
		return 0xFF;
	}
	else
	{
		UCHAR ret = 0;


		ret = ((ret | D04) << 1);
		ret = ((ret | D03) << 1);
		ret = ((ret | D02) << 1);
		ret |= D01;

		turn_off_send_led();
		return ret;
	}
}

void i2c_cb_set_do_status(UCHAR _byte, UINT _idx)
{
	turn_on_receive_led();

	if (_idx > 0)
	{
		turn_off_receive_led();
		return;
	}

	D04 = (_byte & DO_4_MASK) >> 3;
	D03 = (_byte & DO_3_MASK) >> 2;
	D02 = (_byte & DO_2_MASK) >> 1;
	D01 = _byte & DO_1_MASK;

	turn_off_receive_led();
	return;
}

void i2c_cb_set_pmic_status(UCHAR _byte, UINT _idx)
{
	turn_on_receive_led();

	if (_idx > 0)
	{
		turn_off_receive_led();
		return;
	}

	if (PWR_FAULT_DO)
	{
		if (_byte & PMIC_DO_ERR_MASK)
		{
			PWR_ENABLE_DO = 0;
			__delay_ms(1);
			PWR_ENABLE_DO = 1;
		}

		turn_off_receive_led();
		return;
	}

	if (PWR_FAULT_AI)
	{
		if (_byte & PMIC_AI_ERR_MASK)
		{
			PWR_ENABLE_AI = 0;
			__delay_ms(1);
			PWR_ENABLE_AI = 1;
		}

		turn_off_receive_led();
		return;
	}

	if (_byte & PMIC_DO_EN_MASK)
	{
		PWR_ENABLE_DO = 1;
	}
	else
	{
		PWR_ENABLE_DO = 0;
	}

	if (_byte & PMIC_AI_EN_MASK)
	{
		PWR_ENABLE_AI = 1;
	}
	else
	{
		PWR_ENABLE_AI = 0;
	}

	turn_off_receive_led();
	return;
}

UCHAR i2c_cb_get_pmic_status(UINT _idx)
{
	turn_on_receive_led();

	if (_idx > 0)
	{
		turn_off_receive_led();
		return 0xFF;
	}

	UCHAR ret = 0;

	ret = (ret | PWR_FAULT_DO) << 1;
	ret = (ret | PWR_FAULT_AI) << 1;
	ret = (ret | PWR_ENABLE_DO) << 1;
	ret = ret | PWR_ENABLE_AI;

	turn_off_receive_led();
	return ret;
}

void i2c_cb_set_output_confirm(UCHAR _byte, UINT _idx)
{
	confirm_clicks_passed = 0;
	return;
}

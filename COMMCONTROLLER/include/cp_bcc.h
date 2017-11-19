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

#ifndef CP_BCC_H
#define	CP_BCC_H

#include "support.h"

/**
 * \file
 *
 * \author Vidas Simkus (vic.simkus@simkus.com)
 *
 * Methods implementing the binary protocol stuffs
 *
 */

/**
 * Resets the board.
 * @return 1 on success, 0 otherwise.
 */
UCHAR bcc_reset(void);

/**
 * Sends out via serial port the raw analog input values.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_ai_status(void);

/**
 * Sends out via serial port the digital output statuses.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_do_status(void);

/**
 * Sets the digital output status based on input.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_set_do_status(void);

/**
 * Sends out via serial port the PMIC statuses based.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_pmic_status(void);

/**
 * Sets the PMIC statuses based on input
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_set_pmic_status(void);

/**
 * Sends out via serial port the L1 calibration values.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_l1_cal_val(void);

/**
 * Sends out via serial port the L2 calibration values.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_l2_cal_val(void);

/**
 * Sets the L1 calibration values based on input.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_set_l1_cal_val(void);

/**
 * Sets the L2 calibration values based on input.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_set_l2_cal_val(void);

/**
 * Sends out via serial port the number of times the IO processor has rebooted.
 * We're using that as the boot count of the board.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_boot_count(void);


#endif	/* CP_BCC_H */


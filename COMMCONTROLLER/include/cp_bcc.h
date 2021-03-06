/*
Vic's IO Board V1; Firmware

Copyright (C) 2019 Vidas Simkus (vic.simkus@simkus.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
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
 * Sets up the binary context to operate in stream mode.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_start_status_stream(void);

/**
 * Sends out via serial port the board status  values.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_board_status(void);

/**
 * Processes the binary stream stuffs.  Goes through all of the registered call indexes and invokes them in turn.
 */
void process_binary_stream(void);
/**
 * Sends out via serial port the number of times the IO processor has rebooted.
 * We're using that as the boot count of the board.
 * \return 1 on success, 0 otherwise.
 */
UCHAR bcc_get_boot_count(void);

/**
 * Resets the state watchdog.  If this is not called on regular basis all of the IO controllers output turn off.
 * \return
 */
UCHAR bcc_confirm_output_state(void);

#endif	/* CP_BCC_H */


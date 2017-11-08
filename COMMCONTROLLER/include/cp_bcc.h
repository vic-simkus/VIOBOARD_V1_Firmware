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

UCHAR bcc_reset(void);
UCHAR bcc_get_ai_status(void);
UCHAR bcc_get_do_status(void);
UCHAR bcc_set_do_status(void);
UCHAR bcc_get_pmic_status(void);
UCHAR bcc_set_pmic_status(void);
UCHAR bcc_get_l1_cal_val(void);
UCHAR bcc_get_l2_cal_val(void);
UCHAR bcc_set_l1_cal_val(void);
UCHAR bcc_set_l2_cal_val(void);
UCHAR bcc_get_boot_count(void);
#endif	/* CP_BCC_H */


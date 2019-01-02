// Step10V - STEP/DIR to analog amplifier CNC regulator
// Copyright (C) 2018 Charles-Henri Mousset - ch.mousset@gmail.com
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#if !defined(CANBUS_H)
#define CANBUS_H

#include "cfg.h"

extern cfg_t cfg;

enum CAN_FN {
	CAN_READ_CFG = 1,
	CAN_WRITE_CFG = 2,
	CAN_PD_BROADCAST = 3
};

void canBus_init(void);

void cfg_can_read_int(int *val, int min, int max, int idx, CANRxFrame *can_rx_frame);
void cfg_can_read_uint32_t(uint32_t *val, uint32_t min, uint32_t max, int idx, CANRxFrame *can_rx_frame);
void cfg_can_read_bool(bool *val, bool min, bool max, int idx, CANRxFrame *can_rx_frame);
void cfg_can_read_float(float *val, float min, float max, int idx, CANRxFrame *can_rx_frame);
void cfg_can_read_notfound(CANRxFrame *can_rx_frame);

#endif

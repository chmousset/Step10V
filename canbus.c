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

#include "ch.h"
#include "hal.h"
#include "chprintf.h"
#include "cfg.h"
#include "canbus.h"
#include <string.h>

static const CANConfig cancfg = {
	CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP | CAN_MCR_NART,
	/*CAN_BTR_LBKM | */ CAN_BTR_SJW(2) | CAN_BTR_TS2(1) |
	CAN_BTR_TS1(10) | CAN_BTR_BRP(9)
};

/*! CAN Bus Receiver thread. This thread reads the messages from the CAN Bus
and parse them to call the corresponding functions or update variables values.
*/
static THD_WORKING_AREA(can_rx1_wa, 256);
static THD_FUNCTION(can_rx, p) {
	event_listener_t el;
	CANRxFrame rxmsg;

	(void)p;
	chRegSetThreadName("receiver");
	chEvtRegister(&CAND1.rxfull_event, &el, 0);
	while(!chThdShouldTerminateX()) {
		if (chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(10)) == 0)
			continue;
		while (canReceive(&CAND1, CAN_ANY_MAILBOX,
						  &rxmsg, TIME_IMMEDIATE) == MSG_OK) {
			chprintf((BaseSequentialStream *)&SD2, "Got CAN frame %03X %d", rxmsg.SID, rxmsg.DLC);
			int i;
			for(i=0; i<rxmsg.DLC; i++)
				chprintf((BaseSequentialStream *)&SD2, " %02X", rxmsg.data8[i]);
			chprintf((BaseSequentialStream *)&SD2, "\r\n");
			if(rxmsg.SID == cfg.can_id && rxmsg.data8[0] == CAN_READ_CFG_REQUEST)
				CFGH_ID_CAN_READ(&cfg, rxmsg.data8[1], &rxmsg);
		}
	}
	chEvtUnregister(&CAND1.rxfull_event, &el);
}

/*
 * Transmitter thread.
 */
static THD_WORKING_AREA(can_tx_wa, 256);
static THD_FUNCTION(can_tx, p)
{
	CANTxFrame txmsg;

	(void)p;
	chRegSetThreadName("transmitter");
	txmsg.IDE = CAN_IDE_EXT;
	txmsg.EID = 0x01234567;
	txmsg.RTR = CAN_RTR_DATA;
	txmsg.DLC = 8;
	txmsg.data32[0] = 0x55AA55AA;
	txmsg.data32[1] = 0x00FF00FF;

	while (!chThdShouldTerminateX()) {
		canTransmit(&CAND1, CAN_ANY_MAILBOX, &txmsg, TIME_MS2I(100));
		chThdSleepMilliseconds(500);
	}
}

void canBus_init(void)
{
	chprintf((BaseSequentialStream *)&SD2, "CAN bus init... ");
	palSetPadMode(GPIOA, 11, PAL_MODE_ALTERNATE(9));
	palSetPadMode(GPIOA, 12, PAL_MODE_ALTERNATE(9));

	canStart(&CAND1, &cancfg);

	chThdCreateStatic(can_rx1_wa, sizeof(can_rx1_wa), NORMALPRIO + 7,
					can_rx, (void *)&CAND1);
	// chThdCreateStatic(can_tx_wa, sizeof(can_tx_wa), NORMALPRIO + 7,
	// 			can_tx, NULL);
	chprintf((BaseSequentialStream *)&SD2, "done\r\n");
}

inline void cfg_read_prepare(CANRxFrame *rx, CANTxFrame *tx)
{
	tx->IDE = CAN_IDE_STD;
	tx->RTR = CAN_RTR_DATA;
	tx->SID = cfg.can_id;
	tx->data8[0] = CAN_READ_CFG;
	tx->data8[1] = rx->data8[1];	// ID of the cfg variable
}

void cfg_can_read_int(int *val, int min, int max, int idx, CANRxFrame *can_rx_frame)
{
	(void) min; (void) max; (void) idx;
	CANTxFrame tx;
	cfg_read_prepare(can_rx_frame, &tx);
	unsigned int i;
	int tmp = *val;
	for(i=0; i<sizeof(int); i++)
	{
		tx.data8[2+i] = tmp & 0xFF;
		tmp = tmp >> 8;
	}
	tx.DLC = 2 + sizeof(int);
	canTransmit(&CAND1, CAN_ANY_MAILBOX, &tx, TIME_IMMEDIATE);
}

void cfg_can_read_uint32_t(uint32_t *val, uint32_t min, uint32_t max, int idx, CANRxFrame *can_rx_frame)
{
	(void) min; (void) max; (void) idx;
	CANTxFrame tx;
	cfg_read_prepare(can_rx_frame, &tx);
	uint32_t tmp = *val;
	unsigned int i;
	for(i=0; i<sizeof(uint32_t); i++)
	{
		tx.data8[2+i] = tmp & 0xFF;
		tmp = tmp >> 8;
	}
	tx.DLC = 2 + sizeof(uint32_t);
	canTransmit(&CAND1, CAN_ANY_MAILBOX, &tx, TIME_IMMEDIATE);
}
void cfg_can_read_bool(bool *val, bool min, bool max, int idx, CANRxFrame *can_rx_frame)
{
	(void) min; (void) max; (void) idx;
	CANTxFrame tx;
	cfg_read_prepare(can_rx_frame, &tx);
	tx.data8[2] = *val ? 1 : 0;
	tx.DLC = 2 + 1;
	canTransmit(&CAND1, CAN_ANY_MAILBOX, &tx, TIME_IMMEDIATE);
}
void cfg_can_read_float(float *val, float min, float max, int idx, CANRxFrame *can_rx_frame)
{
	(void) min; (void) max; (void) idx;
	CANTxFrame tx;
	cfg_read_prepare(can_rx_frame, &tx);
	memcpy(&tx.data8[2], (void*) val, sizeof(float));
	tx.DLC = 2 + sizeof(float);
	canTransmit(&CAND1, CAN_ANY_MAILBOX, &tx, TIME_IMMEDIATE);
}
void cfg_can_read_notfound(CANRxFrame *can_rx_frame)
{
	CANTxFrame tx;
	cfg_read_prepare(can_rx_frame, &tx);
	tx.DLC = 2;
	canTransmit(&CAND1, CAN_ANY_MAILBOX, &tx, TIME_IMMEDIATE);
}


// void cfg_can_write_int(int *val, int min, int max, int idx, CANRxFrame *can_rx_frame)
// {
// }
// void cfg_can_write_uint32_t(uint32_t *val, uint32_t min, uint32_t max, int idx, CANRxFrame *can_rx_frame)
// {
// }
// void cfg_can_write_bool(bool *val, bool min, bool max, int idx, CANRxFrame *can_rx_frame)
// {
// }
// void cfg_can_write_float(float *val, float min, float max, int idx, CANRxFrame *can_rx_frame)
// {
// }
// void cfg_can_write_notfound(CANRxFrame *can_rx_frame)
// {
// }

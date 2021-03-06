// Step10V - STEP/DIR to analog amplifier CNC regulator
// Copyright (C) 2018-2019 Charles-Henri Mousset - ch.mousset@gmail.com
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

#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"

#include "shell_cmds.h"
#include "sigf.h"
#include "ssd1306.h"
#include "chprintf.h"
#include "ctrloop.h"
#include "canbus.h"
#include "flash.h"


static const I2CConfig i2c_cfg = {
	STM32_TIMINGR_PRESC(0U) |
	STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
	STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
	0,
	0
};


// Display loop
static THD_WORKING_AREA(waThreadOLED, 1024);
static THD_FUNCTION(ThreadOLED, arg)
{
	char buf[18];
	(void)arg;
	chRegSetThreadName("oled");
	chThdSleepMilliseconds(100);
	ssd_init();

	ssd_set_line(2);
	while (true) {
		chThdSleepMilliseconds(50);
		// chsnprintf(buf, 10, "%d",  chVTGetSystemTime());	// feedback.x_cst
		chsnprintf(buf, 18, "e%d o%.2f",  (int) feedback.x_cst, pid_out);
		ssd_puts16(2, 0, buf);
	}
}

int main(void) {
	halInit();
	chSysInit();

	sdStart(&SD2, NULL);
	start_shell();

	if(!load_settings(&cfg))
	{
		cfg_setdefaults(&cfg);
		chThdSleepMilliseconds(10);
		chprintf((BaseSequentialStream*) &SD2, "Cannot load configuration from FLASH. Using default settings\r\n");
	}

	// init & start the OLED display
	i2cStart(&I2CD3, &i2c_cfg);
	palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(4));
	palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(4));
	chThdCreateStatic(waThreadOLED, sizeof(waThreadOLED), NORMALPRIO, ThreadOLED, NULL);

	// start the regulation loop
	init_ctrloop();

	// CAN bus is the main control/config interface
	canBus_init();

	while (true) {
		chThdSleepMilliseconds(100);
	}
}

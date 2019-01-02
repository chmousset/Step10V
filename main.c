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

static const I2CConfig i2c_cfg = {
	STM32_TIMINGR_PRESC(0U) |
	STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
	STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
	0,
	0
};


void rst_tim(GPTDriver *drv)
{
	if (drv == &GPTD1)
	{
		rccEnableTIM1(true);
		rccResetTIM1();
	}
	else if (drv == &GPTD2)
	{
		rccEnableTIM2(true);
		rccResetTIM2();
	}
}

void init_qei(GPTDriver *drv, uint8_t filter)
{
	// CC1P and CC2P in TIMx_CCER controls polarity

	rst_tim(drv);
	drv->tim->CR1  = 0;                      // Initially stopped
	// drv->tim->CR2  = TIM_CR2_CCDS;           // DMA on UE (if any)
	drv->tim->PSC  = 0;
	drv->tim->DIER = 0;
	drv->tim->EGR  = TIM_EGR_UG;             // Re-initialize the counter
	drv->tim->CNT  = 0;                      // Reset counter
	drv->tim->SMCR = ( TIM_SMCR_SMS_Msk & (0b11 << TIM_SMCR_SMS_Pos) );
	drv->tim->SR   = 0;                      // Clear pending IRQs (if any)
	drv->tim->CCER = 0;
	drv->tim->CCMR2 = TIM_CCMR2_CC3S_0;		// capture CC3 on CH3, (TIM_CCMR2_IC3F_0 2 samples filter)
	drv->tim->CCER = TIM_CCER_CC3E;			// rising edge
	drv->tim->CR1  = TIM_CR1_URS | TIM_CR1_CEN;		// start the counter
}

void init_stepdir(GPTDriver *drv, uint8_t filter)
{
	rst_tim(drv);
	drv->tim->CR1  = 0;                      // Initially stopped
	drv->tim->CR2  = TIM_CR2_CCDS;           // DMA on UE (if any)
	drv->tim->PSC  = 0;
	drv->tim->DIER = 0;
	drv->tim->EGR  = TIM_EGR_UG;             // Re-initialize the counter
	drv->tim->CNT  = 0;                      // Reset counter
	drv->tim->CCMR1 =	(TIM_CCMR1_IC1F_Msk & (filter << TIM_CCMR1_IC1F_Pos)) |
						(TIM_CCMR1_CC1S_Msk & (0b01 << TIM_CCMR1_CC1S_Pos));
	drv->tim->SMCR =	( TIM_SMCR_SMS_Msk & (0b111 << TIM_SMCR_SMS_Pos) ) |
						( TIM_SMCR_ETF_Msk & (filter << TIM_SMCR_ETF_Pos)) |
						( TIM_SMCR_TS_Msk & (0b101 << TIM_SMCR_TS_Pos));
	drv->tim->SR   = 0;                      // Clear pending IRQs (if any)
	// drv->tim->CCER = 0;
	// drv->tim->CCMR2 = TIM_CCMR2_CC3S_0;		// capture CC3 on CH3, (TIM_CCMR2_IC3F_0 2 samples filter)
	// drv->tim->CCER = TIM_CCER_CC3E;			// rising edge
	drv->tim->CR1  = TIM_CR1_URS | TIM_CR1_CEN;		// start the counter
}

// Display loop
static THD_WORKING_AREA(waThreadOLED, 1024);
static THD_FUNCTION(ThreadOLED, arg)
{
	char buf[10];
	(void)arg;
	chRegSetThreadName("oled");
	ssd_init();

	ssd_set_line(2);
	while (true) {
		chThdSleepMilliseconds(50);
		// chsnprintf(buf, 10, "%d",  chVTGetSystemTime());	// feedback.x_cst
		chsnprintf(buf, 6, "%04d ",  (int) feedback.x_cst);
		ssd_puts16(2, 0, buf);
	}
}

int main(void) {
	halInit();
	chSysInit();

	sdStart(&SD2, NULL);
	start_shell();


	i2cStart(&I2CD3, &i2c_cfg);
	palSetPadMode(GPIOB, 4, PAL_MODE_ALTERNATE(4));
	palSetPadMode(GPIOA, 7, PAL_MODE_ALTERNATE(4));

	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 12, PAL_MODE_ALTERNATE(1));
	init_qei(&GPTD1, 7);
	init_qei(&GPTD2, 7);

	init_ctrloop();

	chThdCreateStatic(waThreadOLED, sizeof(waThreadOLED), NORMALPRIO, ThreadOLED, NULL);

	while (true) {
		chThdSleepMilliseconds(100);
	}
}

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
#include "canbus.h"


static const DACConfig daccfg = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL,
  .cr           = 0
};

static const I2CConfig i2c_cfg = {
	STM32_TIMINGR_PRESC(0U) |
	STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
	STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
	0,
	0
};

bool loop_enable = 1;


void dac_out(int32_t dac)
{
	uint16_t dac1, dac2;
	if(dac > 0)
		dac += 10;
	if(dac < 0)
		dac -= 10;

	dac1 = (uint16_t)(2048 + dac/2);
	dac2 = (uint16_t)(2048 - dac/2);

	dac_lld_put_channel(&DACD1, 0, dac1);
	dac_lld_put_channel(&DACD1, 1, dac2);
}

// PID Control Loop
struct signal_float setpoint = SIG_CST(0.0);
struct signal_float feedback = SIG_CST(0.0);


struct sig_pid_param_f pid_p= {
	.p = 40.0,
	.i = 1.0,
	.d = 100.0,
	.k = {0.0, 0.0, 0.0},
	.max_output = 4095-100,
	.integral = 0.0,
	.history = {0.0, 0.0, 0.0},
	.setpoint = &setpoint,
	.feedback = &feedback
};
struct signal_float pid = SIG_FN((sig_func_f)sig_pid_opt_f, &pid_p);

bool update_pid_params = 1;

void ctl_loop(void)
{
	static int16_t cnt1, old_cnt1 = 0;
	static int32_t cnt2, old_cnt2 = 0;
	static int32_t error = 0;
	int16_t diff_cnt1, diff_cnt2;
	static 	n_t n;

	cnt1 = (int16_t)GPTD1.tim->CNT;
	cnt2 = (int32_t)GPTD2.tim->CNT;
	diff_cnt1 = (cnt1 - old_cnt1);
	diff_cnt2 = (cnt2 - old_cnt2);
	error += ((int32_t) diff_cnt1) - diff_cnt2*10;
	old_cnt1 = cnt1;
	old_cnt2 = cnt2;

	if(update_pid_params)
	{
		update_pid_params = FALSE;
		sig_pid_compute_k_f(&pid);
	}

	// feeding the error into the feedback with setpoint = 0
	// works as we don't use feed-forward
	feedback.x_cst = (float) error;
	if(loop_enable)
		dac_out( (int32_t) sig_get_value_f(&pid, n++) );
}


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
	int i;
	char buf[10];
	(void)arg;
	chRegSetThreadName("oled");
	ssd_init();

	ssd_set_line(2);
	while (true) {
		palSetLine(LINE_LED_GREEN);
		chThdSleepMilliseconds(50);
		palClearLine(LINE_LED_GREEN);
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

	palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
	palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_ANALOG);
	dacStart(&DACD1, &daccfg);

	palSetPadMode(GPIOA, 0, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 1, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 8, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(1));
	palSetPadMode(GPIOA, 12, PAL_MODE_ALTERNATE(1));
	init_qei(&GPTD1, 7);
	init_qei(&GPTD2, 7);

	chThdCreateStatic(waThreadOLED, sizeof(waThreadOLED), NORMALPRIO-1, ThreadOLED, NULL);

	canBus_init();

	while (true) {
		chThdSleepMilliseconds(1);
		ctl_loop();
	}
}

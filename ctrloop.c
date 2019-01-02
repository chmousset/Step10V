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
#include "ctrloop.h"

static const DACConfig daccfg = {
  .init         = 0U,
  .datamode     = DAC_DHRM_12BIT_RIGHT_DUAL,
  .cr           = 0
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

	palSetLine(LINE_LED_GREEN);
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
	palClearLine(LINE_LED_GREEN);
}

void init_ctrloop(void)
{
	palSetPadMode(GPIOA, 4, PAL_MODE_INPUT_ANALOG);
	palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_ANALOG);
	dacStart(&DACD1, &daccfg);

}

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
#include "ch_test.h"
#include "chprintf.h"
#include "shell.h"
#include "shell_cmds.h"
#include "sigf.h"

char ABOUT[] = "Step10V  Copyright (C) 2018  Charles-Henri Mousset\r\n\
https://github.com/chmousset/Step10V\r\n\
This program comes with ABSOLUTELY NO WARRANTY;\r\n\
This is free software, and you are welcome to redistribute it under certain\r\n\
conditions.\r\n\
Published under the GNU General Public License, version 3 of the License\r\n\
You should have received a copy of the GNU General Public License along with\r\n\
this program. If not, see <https://www.gnu.org/licenses/>.\r\n";

float stof(const char* s){
	float rez = 0, fact = 1;
	if (*s == '-'){
		s++;
		fact = -1;
	};
	for (int point_seen = 0; *s; s++){
		if (*s == '.'){
			point_seen = 1; 
			continue;
		};
		int d = *s - '0';
		if (d >= 0 && d <= 9){
			if (point_seen) fact /= 10.0f;
			rez = rez * 10.0f + (float)d;
		};
	};
	return rez * fact;
};

void about(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, ABOUT);
}

void dac(BaseSequentialStream *chp, int argc, char *argv[])
{
	uint16_t chan, val;

	if(argc != 2)
	{
		chprintf(chp, "dac <chan> <value>\r\n");
		return;
	}
	chan = atoi(argv[0]);
	val = atoi(argv[1]);
	if(chan > 1 || val > 4095)
	{
		chprintf(chp, "chan 0/1 ; val 0-4095\r\n");
		return;
	}
	dac_lld_put_channel(&DACD1, chan, val);

}


void p(BaseSequentialStream *chp, int argc, char *argv[])
{
	if(argc == 1)
	{
		pid_p.p = stof(argv[0]);
		update_pid_params = true;
	}
	chprintf(chp, "p= %f\r\n", pid_p.p);
}
void i(BaseSequentialStream *chp, int argc, char *argv[])
{
	if(argc == 1)
	{
		pid_p.i = stof(argv[0]);
		update_pid_params = true;
	}
	chprintf(chp, "i= %f\r\n", pid_p.i);
}
void d(BaseSequentialStream *chp, int argc, char *argv[])
{
	if(argc == 1)
	{
		pid_p.d = stof(argv[0]);
		update_pid_params = true;
	}
	chprintf(chp, "d= %f\r\n", pid_p.d);
}

void enable(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argv;
	(void) argc;

	loop_enable = 1;
}

void disable(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argv;
	(void) argc;

	loop_enable = 0;
}

static const ShellCommand commands[] = {
	{"about", about},
	{"dac", dac},
	{"p", p},
	{"i", i},
	{"d", d},
	{"enable", enable},
	{"disable", disable},
	{NULL, NULL}
};

static const ShellConfig shell_cfg = {
	(BaseSequentialStream *)&SD2,
	commands
};

static THD_WORKING_AREA(waShell, 2048);

void start_shell(void)
{
	chThdCreateStatic(waShell, sizeof(waShell), NORMALPRIO, shellThread, (void *)&shell_cfg);
}

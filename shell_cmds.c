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
#include <stdlib.h>
#include "cfg.h"
#include <string.h>
#include "flash.h"

char ABOUT[] = "Step10V  Copyright (C) 2018  Charles-Henri Mousset\r\n\
https://github.com/chmousset/Step10V\r\n\
This program comes with ABSOLUTELY NO WARRANTY;\r\n\
This is free software, and you are welcome to redistribute it under certain\r\n\
conditions.\r\n\
Published under the GNU General Public License, version 3 of the License\r\n\
You should have received a copy of the GNU General Public License along with\r\n\
this program. If not, see <https://www.gnu.org/licenses/>.\r\n";

cfg_t cfg;

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
	(void)argc; (void)argv;
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

void enable(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argv;
	(void) argc;
	(void) chp;

	loop_enable = 1;
	chprintf(chp, "Ctrl loop enabled\r\n");
}

void disable(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argv;
	(void) argc;
	(void) chp;

	loop_enable = 0;
	chprintf(chp, "Ctrl loop disabled\r\n");
}

void cfg_term_disp_int(int *val, int min, int max, int idx, BaseSequentialStream *chp)
{
	(void)min; (void)max; (void) idx;
	chprintf(chp, "%s = %d\r\n", cfg_names[idx], *val);
}
void cfg_term_disp_uint32_t(uint32_t *val, uint32_t min, uint32_t max, int idx, BaseSequentialStream *chp)
{
	(void)min; (void)max; (void) idx;
	chprintf(chp, "%s = %u\r\n", cfg_names[idx], *val);
}
void cfg_term_disp_bool(bool *val, bool min, bool max, int idx, BaseSequentialStream *chp)
{
	(void)min; (void)max; (void) idx;
	chprintf(chp, "%s = %s\r\n", cfg_names[idx], *val ? "True" : "False");
}
void cfg_term_disp_float(float *val, float min, float max, int idx, BaseSequentialStream *chp)
{
	(void)min; (void)max; (void) idx;
	chprintf(chp, "%s = %f\r\n", cfg_names[idx], *val);
}
void cfg_term_disp_notfound(BaseSequentialStream *chp)
{
	chprintf(chp, "config variable not found!\r\n");
}

void cfg_term_save_int(int *val, int min, int max, int idx, BaseSequentialStream *chp, char *value)
{
	int v = atoi(value);
	if( (v > max) || (v < min) )
	{
		chprintf(chp, "Out of range [%d,%d]!\r\n", min, max);
		return;
	}
	*val = v;
	chprintf(chp, "%s = %d\r\n", cfg_names[idx], *val);
}
void cfg_term_save_uint32_t(uint32_t *val, uint32_t min, uint32_t max, int idx, BaseSequentialStream *chp, char *value)
{
	uint32_t v = atoi(value);
	if( (v > max) || (v < min) )
	{
		chprintf(chp, "Out of range [%d,%d]!\r\n", min, max);
		return;
	}
	*val = v;
	chprintf(chp, "%s = %u\r\n", cfg_names[idx], *val);
}
void cfg_term_save_bool(bool *val, bool min, bool max, int idx, BaseSequentialStream *chp, char *value)
{
	(void)min; (void)max;
	bool b = false;
	if(atoi(value) == 1)
		b = true;
	else if(strcmp("true", value) == 0)
		b = true;
	else if(strcmp("True", value) == 0)
		b = true;
	else if(strcmp("TRUE", value) == 0)
		b = true;
	*val = b;
	chprintf(chp, "%s = %s\r\n", cfg_names[idx], *val ? "True" : "False");
}
void cfg_term_save_float(float *val, float min, float max, int idx, BaseSequentialStream *chp, char *value)
{
	float v = atof(value);
	if( (v > max) || (v < min) )
	{
		chprintf(chp, "Out of range [%f,%f]!\r\n", min, max);
		return;
	}
	*val = v;
	chprintf(chp, "%s = %f\r\n", cfg_names[idx], *val);
}

void cfg_term_save_notfound(BaseSequentialStream *chp, char *value)
{
	(void)value;
	chprintf(chp, "config variable not found!\r\n");
}

void shl_cfg(BaseSequentialStream *chp, int argc, char *argv[])
{
	int i;
	if(argc == 0)
	{
		for(i=0; i<CFG_CNT; i++)
		{
			chprintf(chp, "%d : ", cfg_ids[i]);
			CFGH_ID_TERM_DISP((&cfg), cfg_ids[i], chp);
			chprintf(chp, " `- %s\r\n", cfg_descriptions[i]);
		}
	}
	if(argc == 1)
	{
		i = atoi(argv[0]);
		if(i > 0)
		{
			CFGH_ID_TERM_DISP((&cfg), i, chp);
		}
		else
		{
			if(strcmp(argv[0], "save") == 0)
				chprintf(chp, "saving cfg... %s\r\n", save_settings(&cfg) ? "OK" : "Failed");
			else if(strcmp(argv[0], "load") == 0)
				chprintf(chp, "loading cfg... %s\r\n", load_settings(&cfg) ? "OK" : "Failed");
			else if(strcmp(argv[0], "default") == 0)
			{
				chprintf(chp, "Setting cfg to defaults\r\n");
				cfg_setdefaults(&cfg);
			}
			else
				CFGH_NAME_TERM_DISP((&cfg), argv[0], chp);
		}
	}
	if(argc == 2)
	{
		i = atoi(argv[0]);
		if(i > 0)
		{
			CFGH_ID_TERM_SAVE((&cfg), i, chp, argv[1]);
		}
		else
		{
			CFGH_NAME_TERM_SAVE((&cfg), argv[0], chp, argv[1]);
		}
	}
}

static const ShellCommand commands[] = {
	{"about", about},
	{"dac", dac},
	{"enable", enable},
	{"disable", disable},
	{"cfg", shl_cfg},
	{NULL, NULL}
};

char histbuff[128] = "";

static const ShellConfig shell_cfg = {
	(BaseSequentialStream *)&SD2,
	commands,
	histbuff,
	sizeof(histbuff),
	.sc_completion = NULL
};

static THD_WORKING_AREA(waShell, 2048);

void start_shell(void)
{
	chThdCreateStatic(waShell, sizeof(waShell), NORMALPRIO, shellThread, (void *)&shell_cfg);
}

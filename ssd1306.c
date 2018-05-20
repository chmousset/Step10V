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

#include <string.h>
#include "ch.h"
#include "hal.h"
#include "Font16px.h"
#include "ssd1306.h"

unsigned char SSD1307Z_Font[][5]={
{0x00,0x00,0x00,0x00,0x00},
{0x00,0x5F,0x00,0x00,0x00},
{0x00,0x07,0x00,0x07,0x00},
{0x14,0x7F,0x14,0x7F,0x14},
{0x24,0x2A,0x7F,0x2A,0x12},
{0x23,0x13,0x08,0x64,0x62},
{0x36,0x49,0x55,0x22,0x50},
{0x00,0x05,0x03,0x00,0x00},
{0x1C,0x22,0x41,0x00,0x00},
{0x41,0x22,0x1C,0x00,0x00},
{0x08,0x2A,0x1C,0x2A,0x08},
{0x08,0x08,0x3E,0x08,0x08},
{0xA0,0x60,0x00,0x00,0x00},
{0x08,0x08,0x08,0x08,0x08},
{0x60,0x60,0x00,0x00,0x00},
{0x20,0x10,0x08,0x04,0x02},
{0x3E,0x51,0x49,0x45,0x3E},
{0x00,0x42,0x7F,0x40,0x00},
{0x62,0x51,0x49,0x49,0x46},
{0x22,0x41,0x49,0x49,0x36},
{0x18,0x14,0x12,0x7F,0x10},
{0x27,0x45,0x45,0x45,0x39},
{0x3C,0x4A,0x49,0x49,0x30},
{0x01,0x71,0x09,0x05,0x03},
{0x36,0x49,0x49,0x49,0x36},
{0x06,0x49,0x49,0x29,0x1E},
{0x00,0x36,0x36,0x00,0x00},
{0x00,0xAC,0x6C,0x00,0x00},
{0x08,0x14,0x22,0x41,0x00},
{0x14,0x14,0x14,0x14,0x14},
{0x41,0x22,0x14,0x08,0x00},
{0x02,0x01,0x51,0x09,0x06},
{0x32,0x49,0x79,0x41,0x3E},
{0x7E,0x09,0x09,0x09,0x7E},
{0x7F,0x49,0x49,0x49,0x36},
{0x3E,0x41,0x41,0x41,0x22},
{0x7F,0x41,0x41,0x22,0x1C},
{0x7F,0x49,0x49,0x49,0x41},
{0x7F,0x09,0x09,0x09,0x01},
{0x3E,0x41,0x41,0x51,0x72},
{0x7F,0x08,0x08,0x08,0x7F},
{0x41,0x7F,0x41,0x00,0x00},
{0x20,0x40,0x41,0x3F,0x01},
{0x7F,0x08,0x14,0x22,0x41},
{0x7F,0x40,0x40,0x40,0x40},
{0x7F,0x02,0x0C,0x02,0x7F},
{0x7F,0x04,0x08,0x10,0x7F},
{0x3E,0x41,0x41,0x41,0x3E},
{0x7F,0x09,0x09,0x09,0x06},
{0x3E,0x41,0x51,0x21,0x5E},
{0x7F,0x09,0x19,0x29,0x46},
{0x26,0x49,0x49,0x49,0x32},
{0x01,0x01,0x7F,0x01,0x01},
{0x3F,0x40,0x40,0x40,0x3F},
{0x1F,0x20,0x40,0x20,0x1F},
{0x3F,0x40,0x38,0x40,0x3F},
{0x63,0x14,0x08,0x14,0x63},
{0x03,0x04,0x78,0x04,0x03},
{0x61,0x51,0x49,0x45,0x43},
{0x7F,0x41,0x41,0x00,0x00},
{0x02,0x04,0x08,0x10,0x20},
{0x41,0x41,0x7F,0x00,0x00},
{0x04,0x02,0x01,0x02,0x04},
{0x80,0x80,0x80,0x80,0x80},
{0x01,0x02,0x04,0x00,0x00},
{0x20,0x54,0x54,0x54,0x78},
{0x7F,0x48,0x44,0x44,0x38},
{0x38,0x44,0x44,0x28,0x00},
{0x38,0x44,0x44,0x48,0x7F},
{0x38,0x54,0x54,0x54,0x18},
{0x08,0x7E,0x09,0x02,0x00},
{0x18,0xA4,0xA4,0xA4,0x7C},
{0x7F,0x08,0x04,0x04,0x78},
{0x00,0x7D,0x00,0x00,0x00},
{0x80,0x84,0x7D,0x00,0x00},
{0x7F,0x10,0x28,0x44,0x00},
{0x41,0x7F,0x40,0x00,0x00},
{0x7C,0x04,0x18,0x04,0x78},
{0x7C,0x08,0x04,0x7C,0x00},
{0x38,0x44,0x44,0x38,0x00},
{0xFC,0x24,0x24,0x18,0x00},
{0x18,0x24,0x24,0xFC,0x00},
{0x00,0x7C,0x08,0x04,0x00},
{0x48,0x54,0x54,0x24,0x00},
{0x04,0x7F,0x44,0x00,0x00},
{0x3C,0x40,0x40,0x7C,0x00},
{0x1C,0x20,0x40,0x20,0x1C},
{0x3C,0x40,0x30,0x40,0x3C},
{0x44,0x28,0x10,0x28,0x44},
{0x1C,0xA0,0xA0,0x7C,0x00},
{0x44,0x64,0x54,0x4C,0x44},
{0x08,0x36,0x41,0x00,0x00},
{0x00,0x7F,0x00,0x00,0x00},
{0x41,0x36,0x08,0x00,0x00},
{0x02,0x01,0x01,0x02,0x01},
{0x02,0x05,0x05,0x02,0x00}
};


volatile int current_col;
volatile int current_page;

#if (SSD_IF == SSD_IF_SPI)
void Write_Instruction(unsigned char cmd)
{
	palClearPad(GPIOC, 7);
	spiAcquireBus(&SPID1);
	spiStart(&SPID1, &spicfg);
	spiSelect(&SPID1);
	spiSend(&SPID1, 1, &cmd);
	spiUnselect(&SPID1);
	spiReleaseBus(&SPID1);
}
void Write_Data(unsigned char  dat)
{
	palSetPad(GPIOC, 7);
	spiAcquireBus(&SPID1);
	spiStart(&SPID1, &spicfg);
	spiSelect(&SPID1);
	spiSend(&SPID1, 1, &dat);
	spiUnselect(&SPID1);
	spiReleaseBus(&SPID1);
}
void Write_Datal(uint8_t *dat, int l)
{
	palSetPad(GPIOC, 7);
	spiAcquireBus(&SPID1);
	spiStart(&SPID1, &spicfg);
	spiSelect(&SPID1);
	spiSend(&SPID1, l, dat);
	spiUnselect(&SPID1);
	spiReleaseBus(&SPID1);
}
#endif

#if (SSD_IF == SSD_IF_I2C)
void Write_Instruction(unsigned char cmd)
{
	uint8_t txbuf[2];
	txbuf[0] = 0x80;
	txbuf[1] = cmd;
	i2cMasterTransmitTimeout(SSD_I2C_DRIVER, SSD_I2C_ADDR,
							txbuf, 2, NULL, 0, SSD_I2C_TIMEOUT);
}
void Write_Data(unsigned char  dat)
{
	uint8_t txbuf[2];
	txbuf[0] = 0x40;
	txbuf[1] = dat;
	i2cMasterTransmitTimeout(SSD_I2C_DRIVER, SSD_I2C_ADDR,
							txbuf, 2, NULL, 0, SSD_I2C_TIMEOUT);
}
void Write_Datal(uint8_t *dat, int l)
{
	uint8_t txbuf[66];
	txbuf[0] = 0x40;
	int i;
	for(i=0; i<l; i++)
		txbuf[i+1] = dat[i];

	i2cMasterTransmitTimeout(SSD_I2C_DRIVER, SSD_I2C_ADDR,
							txbuf, 1+l, NULL, 0, SSD_I2C_TIMEOUT);
}
#endif

void ssd_set_line(unsigned char add)
{
	current_page = add;
	add=0xb0|add;
	Write_Instruction(add);
}

void ssd_set_col(unsigned char add)
{
	Write_Instruction((0x10|(add>>4)));
	Write_Instruction((0x0f&add));
	current_col = add;
}

void ssd_contrast(unsigned char mod)
{
	Write_Instruction(0x81);
	Write_Instruction(mod);
}

void ssd_putc8(char c)
{
	int i;

	for(i=0; i<5; i++)
		Write_Data(SSD1307Z_Font[c-32][i]);

	Write_Data(0x00);
	current_col +=6;
}

void ssd_puts(uint8_t line, int8_t col, char *str)
{
	ssd_set_line(line);
	if(col >= 0)
		ssd_set_col(col);
	int i, l = strlen(str);
	for(i=0; i<l; i++)
	{
		if(str[i] != '\n')
			ssd_putc8(str[i]);
		else
		{
			while (127 - current_col > 0)
			{
				if(127 - current_col >= 8)
					ssd_putc8(32);
				else
				{
					Write_Data(0x00);
					current_col++;
				}
			}
		}
	}
}

int font_fixed = 0;

void fix_font_table(void)
{
	uint8_t buf[20];
	int i, j, len;
	for(i = 0; i< 255; i++)
	{
		len = Font16pxBold[i][0];
		for(j=0; j<len; j++)
		{
			buf[j+len] = Font16pxBold[i][j*2 +1];
			buf[j] = Font16pxBold[i][j*2 +2];
		}
		memcpy(Font16pxBold[i] + 1, buf, len*2);
	}
	font_fixed = 1;
}

void ssd_putc16_v_fixed(char c)
{
	uint8_t *font = (uint8_t*) Font16pxBold[(uint8_t) c] + 1;
	uint8_t len = (uint8_t) Font16pxBold[(uint8_t) c][0];

	ssd_set_line(current_page);
	ssd_set_col(current_col);
	Write_Datal(font, len);
	Write_Data(0x00);
	font += len;

	ssd_set_line(current_page+1);
	current_page--;
	ssd_set_col(current_col);
	Write_Datal(font, len);
	Write_Data(0x00);
	current_col = current_col + len + 1;
}

void ssd_putc16_v_nofix(char c)
{
	ssd_set_line(current_page);
	uint8_t i, dat;

	uint8_t charwidth = Font16pxBold[(uint8_t) c][0];
	

	for(i=0;i<charwidth;i++)
	{
		dat = Font16pxBold[(uint8_t) c][i*2+2];
		Write_Data(dat);
	}
	Write_Data(0x00);

	// Write_Datal(Font16pxBold[(uint8_t) c] + 1, charwidth);

	ssd_set_line(current_page+1);
	current_page--;
	ssd_set_col(current_col);

	// Write_Datal(Font16pxBold[(uint8_t) c] + 2, charwidth);

	for(i=0;i<charwidth;i++)
	{
		dat = Font16pxBold[(uint8_t) c][i*2+1];
		Write_Data(dat);
	}
	Write_Data(0x00);
	current_col = current_col + charwidth+1;
}

void ssd_putc16_v(char c)
{
	if(font_fixed)
		ssd_putc16_v_fixed(c);
	else
		ssd_putc16_v_nofix(c);
}

void ssd_init(void)
{
	Write_Instruction(0xae);//--turn off oled panel

	Write_Instruction(0xd5);//--set display clock divide ratio/oscillator frequency
	Write_Instruction(0x80);//--set divide ratio

	Write_Instruction(0xa8);//--set multiplex ratio(1 to 64)
	Write_Instruction(0x1f);//--1/32 duty

	Write_Instruction(0xd3);//-set display offset
	Write_Instruction(0x00);//-not offset

	Write_Instruction(0x8d);//--set Charge Pump enable/disable
	Write_Instruction(0x14);//--set(0x10) disable

	Write_Instruction(0x40);//--set start line address

	Write_Instruction(0xa6);//--set normal display

	Write_Instruction(0xa4);//Disable Entire Display On

	Write_Instruction(0xa1);//--set segment re-map 128 to 0

	Write_Instruction(0xC8);//--Set COM Output Scan Direction 64 to 0

	Write_Instruction(0xda);//--set com pins hardware configuration
	Write_Instruction(0x42);

	Write_Instruction(0x81);//--set contrast control register
	Write_Instruction(255);

	Write_Instruction(0xd9);//--set pre-charge period
	Write_Instruction(0xf1);

	Write_Instruction(0xdb);//--set vcomh
	Write_Instruction(0x40);

	Write_Instruction(0xaf);//--turn on oled panel

	ssd_clear();
}

void ssd_test(unsigned char value)
{
		unsigned char i,j,k;
		for(i=0;i<0x4;i++)
	{
		ssd_set_line(i);

				ssd_set_col(0x00);

		for(j=0;j<0x10;j++)
		{
				for(k=0;k<0x04;k++)
						Write_Data(value);
				for(k=0;k<0x04;k++)
						Write_Data(~value);
		}
	}
}

void ssd_clear(void)
{
		unsigned char i,j,k;
		for(i=0;i<0x4;i++)
	{
		ssd_set_line(i);

				ssd_set_col(0x00);

		for(j=0;j<0x10;j++)
		{
				for(k=0;k<0x04;k++)
						Write_Data(0);
				for(k=0;k<0x04;k++)
						Write_Data(0);
		}
	}
}

void maintest(void)
{
	unsigned int cnt;
	fix_font_table();
	ssd_init();

	ssd_puts(2, 0, "Line 2");
	ssd_puts(3, 0, "Line 3");

	char string[] = "IP=192.168.0.123\n";

	ssd_set_line(2);
	ssd_set_col(0);
	for(cnt=0; cnt<strlen(string); cnt++)
	{
		ssd_putc16_v(string[cnt]);
	}
}

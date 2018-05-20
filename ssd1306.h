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

#if !defined(SSD1306_H)
#define SSD1306_H

#if !defined(min)
#define min(a, b)	(a<b ? a : b)
#endif

#if !defined(SSD_I2C_ADDR)
#define SSD_I2C_ADDR		0b0111100
#endif

/**
 * Interface used to communicate with the SSD1306
 */
#define SSD_IF_SPI			0
#define SSD_IF_I2C			1

#if !defined(SSD_IF)
#define SSD_IF 				SSD_IF_I2C
#endif

#define Start_column		0x00
#define Start_page			0x02
#define	StartLine_set		0x00

#if !defined(SSD_I2C_TIMEOUT)
#define SSD_I2C_TIMEOUT		10
#endif

#if !defined(SSD_I2C_DRIVER)
#define SSD_I2C_DRIVER		(&I2CD3)
#endif

extern void Write_Instruction(unsigned char cmd);
extern void Write_Data(unsigned char  dat);
extern void Write_Datal(uint8_t *dat, int l);
extern void Set_Page_Address(unsigned char add);
extern void Set_Column_Address(unsigned char add);
extern void Set_Contrast_Control_Register(unsigned char mod);

extern void ssd_init(void);
extern void Display_Chess(unsigned char value);
extern void maintest(void);

#endif

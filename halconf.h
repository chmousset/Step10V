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


#ifndef HALCONF_H
#define HALCONF_H

#define _CHIBIOS_HAL_CONF_
#define _CHIBIOS_HAL_CONF_VER_6_0_

#include "mcuconf.h"

#define HAL_USE_PAL                         TRUE
#define HAL_USE_ADC                         FALSE
#define HAL_USE_CAN                         TRUE
#define HAL_USE_CRY                         FALSE
#define HAL_USE_DAC                         TRUE
#define HAL_USE_EXT                         FALSE
#define HAL_USE_GPT                         TRUE
#define HAL_USE_I2C                         TRUE
#define HAL_USE_I2S                         FALSE
#define HAL_USE_ICU                         FALSE
#define HAL_USE_MAC                         FALSE
#define HAL_USE_MMC_SPI                     FALSE
#define HAL_USE_PWM                         FALSE
#define HAL_USE_QSPI                        FALSE
#define HAL_USE_RTC                         FALSE
#define HAL_USE_SDC                         FALSE
#define HAL_USE_SERIAL                      TRUE
#define HAL_USE_SERIAL_USB                  FALSE
#define HAL_USE_SPI                         FALSE
#define HAL_USE_UART                        FALSE
#define HAL_USE_USB                         FALSE
#define HAL_USE_WDG                         FALSE

#define PAL_USE_CALLBACKS                   FALSE
#define PAL_USE_WAIT                        FALSE

#define ADC_USE_WAIT                        TRUE
#define ADC_USE_MUTUAL_EXCLUSION            TRUE

#define CAN_USE_SLEEP_MODE                  TRUE
#define CAN_ENFORCE_USE_CALLBACKS           FALSE

#define HAL_CRY_USE_FALLBACK                FALSE
#define HAL_CRY_ENFORCE_FALLBACK            FALSE

#define DAC_USE_WAIT                        TRUE
#define DAC_USE_MUTUAL_EXCLUSION            TRUE

#define I2C_USE_MUTUAL_EXCLUSION            TRUE

#define MAC_USE_ZERO_COPY                   FALSE
#define MAC_USE_EVENTS                      TRUE

#define MMC_NICE_WAITING                    TRUE

#define QSPI_USE_WAIT                       TRUE
#define QSPI_USE_MUTUAL_EXCLUSION           TRUE

#define SDC_INIT_RETRY                      100
#define SDC_MMC_SUPPORT                     FALSE
#define SDC_NICE_WAITING                    TRUE
#define SDC_INIT_OCR_V20                    0x50FF8000U
#define SDC_INIT_OCR                        0x80100000U

#define SERIAL_DEFAULT_BITRATE              38400
#define SERIAL_BUFFERS_SIZE                 16

#define SERIAL_USB_BUFFERS_SIZE             256
#define SERIAL_USB_BUFFERS_NUMBER           2

#define SPI_USE_WAIT                        TRUE
#define SPI_USE_CIRCULAR                    FALSE
#define SPI_USE_MUTUAL_EXCLUSION            TRUE
#define SPI_SELECT_MODE                     SPI_SELECT_MODE_PAD

#define UART_USE_WAIT                       FALSE
#define UART_USE_MUTUAL_EXCLUSION           FALSE

#define USB_USE_WAIT                        FALSE

#endif
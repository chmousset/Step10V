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
#include "sig.h"
#include "cfg.h"

#define FLASH_KEY1 0x45670123
#define FLASH_KEY2 0xCDEF89AB

#define flash_lock() {FLASH->CR |= FLASH_CR_LOCK;}
#define flash_wait() {	while (FLASH->SR & FLASH_SR_BSY) {} }

#define FLASH_SR_ERROR_MASK (FLASH_SR_OPERR | FLASH_SR_PROGERR | \
	FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR | \
	FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR | FLASH_SR_OPTVERR )

inline void flash_clearerr(void)
{
	FLASH->SR &= ~FLASH_SR_ERROR_MASK;
	while (FLASH->SR & FLASH_SR_ERROR_MASK)
		;
}

void flash_unlock(void)
{
	if (!(FLASH->CR & FLASH_CR_LOCK)) {
		return;
	}
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void flash_erase_page(uint32_t *address)
{

	flash_unlock();

	while (FLASH->SR & FLASH_SR_BSY)
		;

	flash_clearerr();
	FLASH->CR = 0;
	FLASH->CR |= ((uint32_t)address >> (11-FLASH_CR_PNB_Pos)) & FLASH_CR_PNB_Msk;
	FLASH->CR |= FLASH_CR_PER;
	FLASH->CR |= FLASH_CR_STRT;
	
	flash_wait();
	FLASH->CR &= ~FLASH_CR_PER;
	flash_lock();

}

void flash_write(void * dest, void *source, int size)
{
	uint32_t buf[2];

	flash_unlock();
	flash_clearerr();
	flash_wait();
	FLASH->CR |= FLASH_CR_PG;
	while(size)
	{
		buf[0] = buf[1] = 0xFFFFFFFF;
		memcpy((void *)buf, source, min(8, size));
		/* write the data */
		*(volatile uint32_t *)dest = buf[0];
		*(volatile uint32_t *)(dest + sizeof(uint32_t)) = buf[1];
		dest += 2 * sizeof(uint32_t);
		source += 2 * sizeof(uint32_t);

		flash_wait();
		/* check and clear EOP flag */
		if (FLASH->SR & FLASH_SR_EOP) {
			FLASH->SR &= ~FLASH_SR_EOP;
		}
		size = max(0, size - 8);
	}
	FLASH->CR = 0;
	flash_lock();
}

#define FLASH_CFG_BASE 0x0803F800
#define FLASH_CFG_ADDR (FLASH_CFG_BASE+8)
bool load_settings(cfg_t *cfg_p)
{
	// check that saved config size is correct
	if(*(uint32_t*)FLASH_CFG_BASE != sizeof(__typeof__(*cfg_p)))
		return false;
	memcpy((void*) cfg_p, (void*) (FLASH_CFG_ADDR), sizeof(__typeof__(*cfg_p)));
	return true;
}

bool save_settings(cfg_t *cfg_p)
{
	uint32_t size = sizeof(__typeof__(*cfg_p));
	flash_erase_page((void*) FLASH_CFG_BASE);
	flash_write((void*) FLASH_CFG_ADDR,(void*) cfg_p, sizeof(__typeof__(*cfg_p)));

	// check the data is correct
	uint8_t *flash_cfg = (uint8_t *) FLASH_CFG_ADDR;
	uint8_t *ram_cfg = (uint8_t *) cfg_p;
	unsigned int i;
	for(i=0; i<sizeof(__typeof__(*cfg_p)); i++)
		if(*flash_cfg++ != *ram_cfg++)
			return false;
	flash_write((void*) FLASH_CFG_BASE,(void*) &size, sizeof(size));
	return true;
}

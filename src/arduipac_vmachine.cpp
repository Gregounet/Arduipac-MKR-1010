#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "arduipac.h"
#include "arduipac_vmachine.h"
#include "arduipac_8048.h"
#include "arduipac_8245.h"
#include "arduipac_bios_rom.h"
#include "arduipac_config.h"

uint16_t vertical_clock;
uint8_t horizontal_clock;
uint8_t x_latch, y_latch;
uint8_t machine_state; // 0 during normal operation and 1 during Vertical Blank
uint8_t external_ram[128];

// #define DEBUG_SERIAL

void init_vmachine()
{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
	Serial.println("init_vmachine()");
#endif
#ifdef DEBUG_TFT
#endif
	vertical_clock = 0;
	horizontal_clock = 0;
	machine_state = 0;

	for (uint8_t i = 0x00; i < 0x7F; i++)
		external_ram[i] = 0x00;

	clear_collision();
}

uint8_t
read_t1()
{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
	Serial.println("read_t1()");
#endif
#ifdef DEBUG_TFT
#endif
	if (horizontal_clock > 16 || vertical_clock > START_VBLCLK)
		return 1; // TODO pourquoi ce 16 ?
	else
		return 0;
}

uint8_t
ext_read(uint8_t addr)
{
	uint8_t data;
	uint8_t si;
	uint8_t mask;
#ifdef DEBUG_SERIAL
	Serial.print("ext_read(");
	Serial.print(addr, HEX);
	Serial.println(")");
#endif
	// if (!(p1 & 0x48)) // TODO check
	// TODO Je ne comprends pas la logique de ce test: il me semble 0x08 devrait suffire.
	if (!(p1 & 0x08) && !(p1 & 0x40)) // 0x40 (active low) : Copy Mode Enable = write to 8245 RAM and read from external - RAM 0x08 (active low): Enable 8245
	{
#ifdef DEBUG_SERIAL
		Serial.println("VDC Access");
#endif
		switch (addr)
		{
		case 0xA1: // 8245 Status byte - Some other bits should normally be set
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
			Serial.println("0xA0 Status Byte");
#endif
#ifdef DEBUG_TFT
#endif
			data = intel8245_ram[0xA0] & 0x02;
			if (vertical_clock > START_VBLCLK)
				data |= 0x08;
			if (horizontal_clock < (LINECNT - 7))
				data = data | 0x01;
			return data;
		case 0xA2: // Collision byte
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
			Serial.println("0xA2 Collision Byte");
#endif
#ifdef DEBUG_TFT
#endif
			si = intel8245_ram[0xA2];
			mask = 0x01;
			data = 0;
			for (uint8_t i = 0; i < 8; i++) // TODO - optimiser ce code
			{
				if (si & mask)
				{
					data |= collision_table[0x01] & mask;
					data |= collision_table[0x02] & mask;
					data |= collision_table[0x04] & mask;
					data |= collision_table[0x08] & mask;
					data |= collision_table[0x10] & mask;
					data |= collision_table[0x20] & mask;
					data |= collision_table[0x80] & mask;
				}
				mask <<= 1;
			}
			clear_collision();
			return data;
		case 0xA4:
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
			Serial.println("0xA4 y_latch");
#endif
#ifdef DEBUG_TFT
#endif
			if ((intel8245_ram[0xA0] & 0x02))
			{
				y_latch = vertical_clock / 22;
				if (y_latch > 241) // TODO ça fait un écran de 242 de large et j'aurais voulu 240
					y_latch = 0xFF;
			}
			return y_latch;
		case 0xA5:
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
			Serial.println("0xA5 ");
#endif
#ifdef DEBUG_TFT
#endif
			if ((intel8245_ram[0xA0] & 0x02))
			{
				x_latch = horizontal_clock * 12; // TODO D'ou sort ce 2 ?
			}
			return x_latch;
		default:
			return intel8245_ram[addr];
		}
	}
	else if (!(p1 & 0x10))
		return external_ram[addr]; // p1 & 0x10 : hack lié à la cartouche MegaCart TODO: supprimer
	return 0;
}

void ext_write(uint8_t data, uint8_t addr)
{
	if (!(p1 & 0x08))
	{
		if (addr >= 0xA0 && addr <= 0xA3)
		{
			if (addr == 0xA0)
			{
				if (intel8245_ram[0xA0] & 0x02 && !data & 0x02)
				{
					y_latch = vertical_clock / 22;
					x_latch = horizontal_clock * 12;
					if (y_latch > 241)
						y_latch = 0xFF;
				}
				/*
				 * Comme je ne comprends pas pour l'instant je commente. TODO
				if (vertical_clock <= START_VBLCLK && intel8245_ram[0xA0] != data)
					draw_display();
				*/
			}
			else if (addr >= 0x40 && addr < 0x80 && addr & (0x02 == 0)) // 0x40 - 0x7F : les quatre Quads, addr & 0x02 == 0 -> les positions X et Y_start du caractère
			{															// TODO comprendre ce code
				addr = addr & 0x71;
				if (!(addr & 0x01))
					data &= 0xFE;
				intel8245_ram[addr] = intel8245_ram[addr + 4] =
					intel8245_ram[addr + 8] = intel8245_ram[addr + 12] = data;
			}
			if (addr >= 0xA7 && addr <= 0xAA)
			{
				intel8245_ram[addr] = data;
			}
			// else if (!(p1 & 0x50)) // TODO: vérifier cette condition - Il est probale que je vais pouvoir transformer ceci en & 0x40
			else if (!(p1 & 0x10) && !(p1 & 0x40))
			{
			}
			if (addr < 0x80)
				external_ram[addr] = data;
		}
	}
}
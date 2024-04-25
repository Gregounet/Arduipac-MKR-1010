#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "arduipac.h"
#include "arduipac_vmachine.h"
#include "arduipac_8048.h"
#include "arduipac_8245.h"
#include "arduipac_bios_rom.h"
#include "arduipac_config.h"
#include "arduipac_collisions.h"
#include "arduipac_colors.h"

uint16_t vertical_clock;
uint8_t horizontal_clock;
uint8_t x_latch, y_latch;
uint8_t machine_state; // 0 during normal operation and 1 during Vertical Blank
uint8_t external_ram[256];

#undef DEBUG_SERIAL

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

	for (uint8_t i = 0x00; i < 0xFF; i++)
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
	uint8_t scan_input;
	uint8_t mask;
#ifdef DEBUG_STDERR
	fprint(stderr, "ext_read(0x%03X)\n", addr);
#endif
#ifdef DEBUG_SERIAL
	Serial.print("ext_read(");
	Serial.print(addr, HEX);
	Serial.println(")");
#endif
#ifdef DEBUG_TFT
#endif
	if ((p1 & 0x58) == 0x10) // Read from VDC RAM
	{
#ifdef DEBUG_STDERR
		fprint(stderr, "VDC Ram access\n", addr);
#endif
#ifdef DEBUG_SERIAL
		Serial.println("VDC RAM Access");
#endif

#ifdef DEBUG_TFT
#endif
		switch (addr)
		{
		case 0xA1: // 8245 Status byte - Some other bits should normally be set
#ifdef DEBUG_STDERR
			fprint(stderr, "VDC 0xA0 Status Register\n", addr);
#endif
#ifdef DEBUG_SERIAL
			Serial.println("VDC 0xA0 Status Register");
#endif
#ifdef DEBUG_TFT
#endif
			data = intel8245_ram[0xA0] & 0x02;
			if (vertical_clock > START_VBLCLK)
				data |= 0x08;
			if (horizontal_clock < (LINECNT - 7)) // TODO Why 7 here ?
				data = data | 0x01;
			return data;
		case 0xA2: // Collision byte
#ifdef DEBUG_STDERR
			fprint(stderr, "VDC 0xA0 0xA2 Register\n", addr);
#endif
#ifdef DEBUG_SERIAL
			Serial.println("VDC 0xA2 Collision Register");
#endif
#ifdef DEBUG_TFT
#endif
			scan_input = intel8245_ram[0xA2];
			mask = 0x01;
			data = 0x00;
			for (uint8_t i = 0; i < 8; i++)
			{
				if (scan_input & mask)
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
			{ // Le bit 1 de VRAM[0xA0] vaut 1 donc y_latch suit le beam
				y_latch = vertical_clock / 22;
				if (y_latch > 241) // TODO ça fait un écran de 242 de large et j'aurais voulu 240
					y_latch = 0xFF;
			}
			return y_latch;
		case 0xA5:
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
			Serial.println("0xA5 x_latch");
#endif
#ifdef DEBUG_TFT
#endif
			if ((intel8245_ram[0xA0] & 0x02))
			{									 // Le bit 1 de VRAM[0xA0] vaut 1 donc x_latch suit le beam
				x_latch = horizontal_clock * 12; // TODO D'ou sort ce 2 ?
			}
			return x_latch;
		default:
			return intel8245_ram[addr];
		}
	}
	else if (!(p1 & 0x10))
	{
#ifdef DEBUG_STDERR
#endif
#ifdef DEBUG_SERIAL
		if (addr >= 0x3A && addr <= 0x3B)
		{
			Serial.print(bigben);
			Serial.print(" - ext_read() - external_ram[0x");
			Serial.print(addr, HEX);
			Serial.print("] -> 0x");
			Serial.println(external_ram[addr], HEX);
		}
#endif

#ifdef DEBUG_TFT
#endif
		return external_ram[addr];
	}
	return 0;
}

/*
 *
 * ext_write()
 */

void ext_write(uint8_t data, uint8_t addr)
{
	switch (p1 & 0x58)
	{
	case 0x08: // External RAM
	{
#ifdef DEBUG_SERIAL
		Serial.println();
		Serial.print(bigben);
		Serial.print(" - ext_write() - external_ram[0x");
		Serial.print(addr, HEX);
		Serial.print("] <- 0x");
		Serial.println(data, HEX);
#endif
		external_ram[addr] = data;
		break;
	}
	case 0x10: // VDC RAM
	case 0x40: // Copy Mode (read from External RAM and Write to VDC)
	{
		if (addr >= 0x10 && addr < 0x40) // Characters
		{
#ifdef DEBUG_SERIAL
			Serial.print("Accessing Characters [0x");
			Serial.print(addr, HEX);
			Serial.print("] <- 0x");
			Serial.println(data, HEX);
#endif
			intel8245_ram[addr] = data;
		}
		else if (addr >= 0x40 && addr < 0x80) // Quads
		{
#ifdef DEBUG_SERIAL
			Serial.print("Accessing Quads [0x");
			Serial.print(addr, HEX);
			Serial.print("] <- 0x");
			Serial.println(data, HEX);
#endif
			if ((addr & 0x02) == 0) // Il s'agit donc des positions X et Y des caractères du Quad
			{
#ifdef DEBUG_SERIAL
				Serial.println("Positions X ou Y");
#endif
				if ((addr & 0x0C) == 0x00) // Il s'agit du premier caractère du Quad, donc celui qui va déterminer l'emplacement des autres
				{
#ifdef DEBUG_SERIAL
					Serial.println("Premier caractère");
#endif
					if ((addr & 0x01) == 0x00) // Il s'agit donc de la position Y du caractère
					{
#ifdef DEBUG_SERIAL
						Serial.println("Position Y");
#endif
						data &= 0xFE;		// qui sera donc nécessairement paire
						addr = addr & 0x70; // Adresse de base du groupe de caractères
						intel8245_ram[addr] = data;
						intel8245_ram[addr + 0x04] = data;
						intel8245_ram[addr + 0x08] = data;
						intel8245_ram[addr + 0x0C] = data;
					}
					else // Il s'agit donc de la position X du caractère
					{
#ifdef DEBUG_SERIAL
						Serial.println("Position X");
#endif
						addr = addr & 0x70; // Adresse de base du groupe de caractères
						intel8245_ram[addr + 0x01] = data + 0;
						intel8245_ram[addr + 0x05] = data + 0x10;
						intel8245_ram[addr + 0x09] = data + 0X20;
						intel8245_ram[addr + 0x0D] = data + 0x30;
					}
				}
				else
				{
					intel8245_ram[addr] = data;
#ifdef DEBUG_SERIAL
					Serial.println("Pas le dernier caractère");
#endif
				}
			}
			else // Il s'agit donc du caractère et de sa couleur
			{
#ifdef DEBUG_SERIAL
				Serial.println("Index dans CSET ou couleur");
#endif
				intel8245_ram[addr] = data;
			}
		}
		else if (addr < 0x10 || (addr >= 0x80 && addr < 0xA0)) // Sprites
		{
#ifdef DEBUG_SERIAL
			Serial.print(bigben);
			Serial.print("- Accessing Sprites Shapes [0x");
			Serial.print(addr, HEX);
			Serial.print("] <- 0x");
			Serial.println(data, HEX);
#endif

			if (intel8245_ram[addr] != data)
			{
				// Update sprites information
				intel8245_ram[addr] = data;

				if (addr < 0x10) // Sprites control
				{
					sprites_uptodate = 0;

					uint8_t sprite_number = addr / 4;
					uint8_t sprite_attribute = addr % 4;

#if defined(DEBUG_SERIAL)
					Serial.print("Sprites control - sprite number ");
					Serial.println(sprite_number);
					Serial.print("Attribute ");
					Serial.println(sprite_attribute);
					Serial.print("Value 0x");
					Serial.println(data, HEX);
#endif
					displayed_sprites[sprite_number].changed_displayed = 0x03; // TODO compute "display" depending on x y values

					switch (sprite_attribute)
					{
					case 0: // y
						displayed_sprites[sprite_number].previous_start_y = displayed_sprites[sprite_number].previous_start_y;
						displayed_sprites[sprite_number].previous_end_y = displayed_sprites[sprite_number].previous_end_y;
						displayed_sprites[sprite_number].start_y = data;
						displayed_sprites[sprite_number].end_y = data + (7 * displayed_sprites[sprite_number].size);
						break;
					case 1: // x
						displayed_sprites[sprite_number].previous_start_x = displayed_sprites[sprite_number].previous_start_x;
						displayed_sprites[sprite_number].previous_end_x = displayed_sprites[sprite_number].previous_end_x;
						displayed_sprites[sprite_number].start_x = (data - 8) * 2;
						displayed_sprites[sprite_number].end_x = displayed_sprites[sprite_number].start_x + (7 * displayed_sprites[sprite_number].size);
						break;
					case 2: // color, shift, size, etc
						displayed_sprites[sprite_number].color = CHAR_COLORS((data & 0x38) >> 3);
						displayed_sprites[sprite_number].size = ((data & 0x04) >> 2) + 1;
						displayed_sprites[sprite_number].shift = (data & 0x02) >> 1;
						displayed_sprites[sprite_number].even_shift = (data & 0x01);
						break;
					default: // not used
						break;
					}
				}

				else // Sprites shapes
				{
					uint8_t sprite_number = (addr - 0x80) / 8;
					sprites_uptodate = 0;

					displayed_sprites[sprite_number].changed_displayed = 0x03; // TODO compute "display" depending on x y values

#if defined(DEBUG_SERIAL)
					Serial.print("Sprites shapes - sprite number ");
					Serial.println(sprite_number);
					Serial.print("Value ");
					Serial.println(data);
#endif
				}
			}
		}
		else if ((addr == 0xA0) && (intel8245_ram[0xA0] != data)) // VDC Control Register
		{
			if ((intel8245_ram[0xA0] & 0x02) && !(data & 0x02)) // beam latch
			{
				// Le bit 1 de la VRAM était à 1 et est passé à 0 donc x_latch et y_latch suivent le beam
				y_latch = vertical_clock / 22;
				x_latch = horizontal_clock * 12;
				if (y_latch > 241)
					y_latch = 0xFF;
			}
			if ((intel8245_ram[0xA0] & 0x08) != (data & 0x08)) // Grid display
			{
				// Grid display on / off
				// Nota bene: Grid should never been set off once already set on
#if defined(DEBUG_SERIAL)
				Serial.print("Changing grid display: ");
				Serial.println((data & 0x08) ? "on" : "off");
#endif
				// TODO understand how it works
				if (!grid_control && (data & 0x08))
				{
					grid_control = data & 0x08;
					h_grid_uptodate = 0;
					v_grid_uptodate = 0;
					for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
						h_segments[h_segment].changed_displayed |= 0x02;
					for (uint8_t v_segment = 0; v_segment < NB_H_SEGMENTS; v_segment++)
						h_segments[v_segment].changed_displayed |= 0x02;
				}

#ifdef DEBUG_SERIAL
				Serial.print("grid_control = ");
				Serial.println(grid_control);
#endif
			}
			if ((intel8245_ram[0xA0] & 0x20) != (data & 0x20)) // Foreground objects
			{
				// Foreground objects (ie chars and sprites) display on / off
				// Nota bene: FG objects should never been set off once already set on
				foreground_control = data & 0x20;
				sprites_uptodate = 0;
				chars_uptodate = 0;
#ifdef DEBUG_SERIAL
				Serial.print("foreground_control = ");
				Serial.println(foreground_control);
#endif
			}
			if ((intel8245_ram[0xA0] & 0x40) != (data & 0x40)) // Dots objects
			{
				// Dots display on / off
				grid_dots = data & 0x40;

#ifdef DEBUG_SERIAL
				Serial.print("grid_dots = ");
				Serial.println(grid_dots);
#endif
			}
			if ((intel8245_ram[0xA0] & 0x80) != (data & 0x80)) // Vertical segments width
			{
				// Vertical segments width
				v_segments_width = (data & 0x80) ? 16 : 3;
				Serial.print("v_segments_width = ");
				Serial.println(v_segments_width);
			}
			intel8245_ram[0xA0] = data;
		}
		else if (addr == 0xA3 && intel8245_ram[0xA3] != data) // VDC Color Register
		{
			if ((intel8245_ram[0xA3] & 0x47) != (data & 0x47))
			{
#if defined(DEBUG_SERIAL)
#endif
				Serial.println("Changing grid color");

				// Grid color
				grid_color = (data & 0x40) ? LIGHT_COLORS(data & 0x07) : DARK_COLORS(data & 0x07);
				h_grid_uptodate = 0;
				v_grid_uptodate = 0;
				for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
					h_segments[h_segment].changed_displayed |= 0x02;
				for (uint8_t v_segment = 0; v_segment < NB_H_SEGMENTS; v_segment++)
					h_segments[v_segment].changed_displayed |= 0x02;
			}
			if ((intel8245_ram[0xA3] & 0x38) != (data & 0x38))
			{
				// Background color TODO check if luminance is used for BG color
				background_color = DARK_COLORS((data & 0x38) >> 3);
			}
			intel8245_ram[0xA3] = data;
		}
		else if (addr >= 0xA7 && addr <= 0xAA) // VDC Sound Register
		{
#ifdef DEBUG_SERIAL
			Serial.print("Accessing VDC Sound Register[0x");
			Serial.print(addr, HEX);
			Serial.print("] <- 0x");
			Serial.println(data, HEX);
#endif
			intel8245_ram[addr] = data;
		}
		else if (addr >= 0xC0 && addr <= 0xE9) // Grid
		{
#ifdef DEBUG_SERIAL
			Serial.print(bigben);
			Serial.print(" - Accessing Grid [0x");
			Serial.print(addr, HEX);
			Serial.print("] <- 0x");
			Serial.println(data, HEX);
#endif
			if (intel8245_ram[addr] != data)
			{
				// Update grid information
				intel8245_ram[addr] = data;

				if (addr >= 0xC0 && addr <= 0xC8) // horizontal segments 0 - 7
				{
					h_grid_uptodate = 0;

					uint8_t base_index = (addr - 0xC0) * 9;
					uint8_t mask = 0x01;
					for (uint8_t row = 0; row < 8; row++)
						h_segments[base_index + row].changed_displayed = 0x02;

					for (uint8_t bit = 0; bit < 8; bit++)
					{
						if (data & mask)
							h_segments[base_index + bit].changed_displayed = 0x03;
						mask <<= 1;
					}
				}

				else if (addr >= 0xD0 && addr <= 0xD8) // horizontal segments 8
				{
					h_grid_uptodate = 0;

					uint8_t base_index = (addr - 0xD0) * 9;
					if (data & 0x01)
						h_segments[base_index + 8].changed_displayed = 0x02;
					else
						h_segments[base_index + 8].changed_displayed = 0x03;
				}

				else if (addr >= 0xE0 && addr <= 0xE9) // vertical segments
				{
					h_grid_uptodate = 0;

					uint8_t base_index = (addr - 0xE0) * 8;
					uint8_t mask = 0x01;
					for (uint8_t col = 0; col < 8; col++)
						v_segments[base_index + col].changed_displayed = 0x02;
					for (uint8_t bit = 0; bit < 8; bit++)
					{
						if (data & mask)
							v_segments[base_index + bit].changed_displayed = 0x03;
						mask <<= 1;
					}
				}
			}
		}
		break;
	}
		/*
		 * Comme je ne comprends pas pour l'instant je commente. TODO
		 * if (vertical_clock <= START_VBLCLK && intel8245_ram[0xA0] != data) draw_display();
		 */
	}
}

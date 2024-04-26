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
		if (intel8245_ram[addr] != data)
		{
			if (addr >= 0x10 && addr < 0x80) // Characters (or quads)
			{
#ifdef DEBUG_SERIAL
#endif
				Serial.print("Accessing Characters [0x");
				Serial.print(addr, HEX);
				Serial.print("] <- 0x");
				Serial.println(data, HEX);

				intel8245_ram[addr] = data;
				chars_uptodate = 0;
				uint8_t char_number = (addr - 0x10) / 4;
				displayed_chars[char_number].changed_displayed = 0x03;
				if (char_number >= 12 && char_number % 4 == 0) // 1st char from a quad
				{
					displayed_chars[char_number + 1].changed_displayed =
						displayed_chars[char_number + 2].changed_displayed =
							displayed_chars[char_number + 3].changed_displayed = 0x03;
				}
				uint8_t char_attribute = addr % 4;
				switch (char_attribute)
				{
				case 0: // y
				{
					uint8_t start_y = data & 0xFE;
					if (char_number < 12 || char_number % 4 == 0)
					//
					// If one of the 12 "independant" chars OR the 1st char from a quad
					//
					{
						uint16_t offset;
						uint16_t cset_start_address;
						uint8_t height;
						displayed_chars[char_number].start_y = start_y ;
						offset =
							(((uint16_t)intel8245_ram[0x10 + 4 * char_number + 3] & 0x01) << 8) +
							((uint16_t)intel8245_ram[0x10 + 4 * char_number + 2]);
						cset_start_address = (offset + (start_y / 2)) % 512;
						displayed_chars[char_number].cset_start_address = cset_start_address;
						displayed_chars[char_number].height = 8 - (cset_start_address % 8);
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].start_y =
								displayed_chars[char_number + 2].start_y =
									displayed_chars[char_number + 3].start_y = start_y;
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
							//
							// Each char has its own cset_start_address
							//
							offset =
								(((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 1) + 3] & 0x01) << 8) +
								((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 1) + 2]);
							displayed_chars[char_number + 1].cset_start_address = (offset + (start_y / 2)) % 512;
							offset =
								(((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 2) + 3] & 0x01) << 8) +
								((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 2) + 2]);
							displayed_chars[char_number + 2].cset_start_address = (offset + (start_y / 2)) % 512;
							offset =
								(((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 3) + 3] & 0x01) << 8) +
								((uint16_t)intel8245_ram[0x10 + 4 * (char_number + 3) + 2]);
							displayed_chars[char_number + 3].cset_start_address = (offset + (start_y / 2)) % 512;
						}
					}
					break;
				}
				case 1: // x
				{
					if (char_number < 12 || char_number % 4 == 0)
					//
					// If one of the 12 "independant" chars OR the 1st char from a quad
					//
					{
						displayed_chars[char_number].start_x = data;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy (after slight re-computing) modified data in remaining three chars
							//
							displayed_chars[char_number + 1].start_x = data + 16;
							displayed_chars[char_number + 2].start_x = data + 32;
							displayed_chars[char_number + 3].start_x = data + 48;
						}
					}
					break;
				}
				case 2: // offset (bits 0-7)
				{
					uint8_t start_y = displayed_chars[char_number].start_y;
					uint16_t offset = (((uint16_t)intel8245_ram[0x10 + 4 * char_number + 3] & 0x01) << 8) +
									  ((uint16_t)intel8245_ram[0x10 + 4 * char_number + 2]);
					uint16_t cset_start_address = (offset + (start_y / 2)) % 512;
					displayed_chars[char_number].cset_start_address = cset_start_address;
					if (char_number <= 12 || char_number % 4 == 0) // If one of the 12 "independant" chars OR the 1st char from a quad
					{
						uint8_t height = 8 - (displayed_chars[char_number].cset_start_address % 8);
						displayed_chars[char_number].height = height;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
						}
					}
					break;
				}
				case 3: // color index + offset b8
				{
					displayed_chars[char_number].color = CHAR_COLORS((data >> 1) & 0x07);
					uint8_t start_y = displayed_chars[char_number].start_y;
					uint16_t offset = (((uint16_t)intel8245_ram[0x10 + 4 * char_number + 3] & 0x01) << 8) +
									  ((uint16_t)intel8245_ram[0x10 + 4 * char_number + 2]);
					uint16_t cset_start_address = (offset + (start_y / 2)) % 512;
					displayed_chars[char_number].cset_start_address = cset_start_address;
					if (char_number <= 12 || char_number % 4 == 0) // If one of the 12 "independant" chars OR the 1st char from a quad
					{
						uint8_t height = 8 - (displayed_chars[char_number].cset_start_address % 8);
						displayed_chars[char_number].height = height;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
						}
					}
				}
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

				// Update sprites information
				intel8245_ram[addr] = data;

				if (addr < 0x10) // Sprite control
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
					displayed_sprites[sprite_number].changed_displayed = 0x03; // TODO compute "display" part depending on x & y values
					//
					// Previous information ALL need to be updated in case of ANY (ie x, y or size) position-related change
					//
					switch (sprite_attribute)
					{
					case 0: // y
						displayed_sprites[sprite_number].start_y = data;
						displayed_sprites[sprite_number].end_y = data + (7 * displayed_sprites[sprite_number].size);
						break;
					case 1: // x
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
				else // Sprite shapes
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
			else if ((addr == 0xA0) && (intel8245_ram[0xA0] != data)) // VDC Control Register
			{
				//
				// Comme je ne comprends pas pour l'instant je commente. TODO
				//
				// if (vertical_clock <= START_VBLCLK && intel8245_ram[0xA0] != data) draw_display();
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
				if ((intel8245_ram[0xA3] & 0x47) != (data & 0x47)) // Grid color
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
				if ((intel8245_ram[0xA3] & 0x38) != (data & 0x38)) // Luminance and background color
				{
					// Background color TODO check if luminance is used for BG color
					background_color = DARK_COLORS((data & 0x38) >> 3);
					//
					// Need to force redrawing everything !
					//
					background_uptodate = 0;
					h_grid_uptodate = 0;
					v_grid_uptodate = 0;
					sprites_uptodate = 0;
					chars_uptodate = 0;
					// Force redrawing grid elements
					for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
						h_segments[h_segment].changed_displayed |= 0x02;
					for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
						v_segments[v_segment].changed_displayed |= 0x02;
					// Force redrawing chars
					for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
						displayed_chars[char_number].changed_displayed |= 0x02;
					// Force redrawing sprites
					for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
						displayed_sprites[sprite_number].changed_displayed |= 0x02;

					intel8245_ram[0xA3] = data;
				}
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
			break;
		}
	}
	}
}

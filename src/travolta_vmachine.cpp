#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "travolta.h"
#include "travolta_vmachine.h"
#include "travolta_8048.h"
#include "travolta_8245.h"
#include "travolta_bios_rom.h"
#include "travolta_video_rom.h"
#include "travolta_display.h"

uint16_t vertical_clock;
uint8_t horizontal_clock;
uint8_t x_latch, y_latch;
uint8_t machine_state; // 0 during normal operation and 1 during Vertical Blank
uint8_t external_ram[256];

void init_vmachine()
{
#if defined(DEBUG)
	Serial.println("init_vmachine()");
#endif
	vertical_clock = 0;
	horizontal_clock = 0;
	machine_state = 0;

	for (uint16_t i = 0x00; i <= 0xFF; i++)
		external_ram[i] = 0x00;
}

uint8_t
read_t1()
{
#if defined(DEBUG)
	Serial.println("read_t1()");
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
#if defined(DEBUG)
	Serial.print("ext_read(");
	Serial.print(addr, HEX);
	Serial.println(")");
#endif
	if ((port1 & 0x58) == 0x10) // Read from VDC RAM
	{
#if defined(DEBUG)
		Serial.println("VDC RAM Access");
#endif

		switch (addr)
		{
		case 0xA1: // 8245 Status byte - Some other bits should normally be set
		{
#if defined(DEBUG)
			Serial.println("VDC 0xA0 Status Register");
#endif
			data = intel8245_ram[0xA0] & 0x02;
			if (vertical_clock > START_VBLCLK)
				data |= 0x08;
			if (horizontal_clock < (LINECNT - 7)) // TODO Why 7 here ?
				data = data | 0x01;
			return data;
			break;
		}
		case 0xA2: // Collision Register
		{
#if defined(DEBUG)
			Serial.println("VDC 0xA2 Collision Register");
#endif
			return detect_collisions();
			break;
		}
		case 0xA4:
		{
#if defined(DEBUG)
			Serial.println("0xA4 y_latch");
#endif
			if ((intel8245_ram[0xA0] & 0x02))
			{ // Le bit 1 de VRAM[0xA0] vaut 1 donc y_latch suit le beam
				y_latch = vertical_clock / 22;
				if (y_latch > 241) // TODO ça fait un écran de 242 de large et j'aurais voulu 240
					y_latch = 0xFF;
			}
			return y_latch;
			break;
		}
		case 0xA5:
		{
#if defined(DEBUG)
			Serial.println("0xA5 x_latch");
#endif
			if ((intel8245_ram[0xA0] & 0x02))
			{									 // Le bit 1 de VRAM[0xA0] vaut 1 donc x_latch suit le beam
				x_latch = horizontal_clock * 12; // TODO D'ou sort ce 2 ?
			}
			return x_latch;
			break;
		}
		default:
			return intel8245_ram[addr];
		}
	}
	else if (!(port1 & 0x10))
	{
#if defined(DEBUG)
		if (addr >= 0x3A && addr <= 0x3B)
		{
			Serial.print("ext_read() - external_ram[0x");
			Serial.print(addr, HEX);
			Serial.print("] -> 0x");
			Serial.println(external_ram[addr], HEX);
		}
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
	switch (port1 & 0x58)
	{
	case 0x08: // External RAM
	{
#if defined(DEBUG)
		Serial.print("ext_write() - external_ram[0x");
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
			if (addr < 0x10) // Sprite control
			{
				intel8245_ram[addr] = data;
				sprites_uptodate = false;

				uint8_t sprite_number = addr / 4;
				uint8_t sprite_attribute = addr % 4;

#if defined(DEBUG)
				Serial.print("Sprites control - sprite number ");
				Serial.println(sprite_number);
				Serial.print("Attribute ");
				Serial.println(sprite_attribute);
				Serial.print("Value 0x");
				Serial.println(data, HEX);
#endif
				displayed_sprites[sprite_number].changed = true;
				switch (sprite_attribute)
				{
				case 0: // y
					displayed_sprites[sprite_number].start_y = data;
					displayed_sprites[sprite_number].end_y = displayed_sprites[sprite_number].start_y + (8 * displayed_sprites[sprite_number].size) - 1;
					break;
				case 1: // x
					displayed_sprites[sprite_number].start_x = data - 8 + LEFT_OFFSET;
					displayed_sprites[sprite_number].end_x = displayed_sprites[sprite_number].start_x + (8 * displayed_sprites[sprite_number].size) - 1;
					break;
				case 2: // color, shift, size, etc
					displayed_sprites[sprite_number].color = CHAR_COLORS((data & 0x38) >> 3);
					displayed_sprites[sprite_number].size = ((data & 0x04) >> 2) + 1;
					displayed_sprites[sprite_number].shift = (bool)(data & 0x02) >> 1;
					displayed_sprites[sprite_number].even_shift = (bool)(data & 0x01);
					break;
				default: // not used
					break;
				}
			}
			else if (addr >= 0x10 && addr < 0x80) // Characters (or quads)
			{
#if defined(DEBUG)
				Serial.print("Accessing Characters [0x");
				Serial.print(addr, HEX);
				Serial.print("] <- 0x");
				Serial.println(data, HEX);
#endif
				intel8245_ram[addr] = data;
				chars_uptodate = false;
				uint8_t char_number = (addr - 0x10) / 4;
				displayed_chars[char_number].changed = true;
				if (char_number >= 12 && char_number % 4 == 0) // 1st char from a quad
				{
					displayed_chars[char_number + 1].changed = true;
					displayed_chars[char_number + 2].changed = true;
					displayed_chars[char_number + 3].changed = true;
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
						uint8_t end_y;
						displayed_chars[char_number].start_y = start_y;
						offset =
							(((uint16_t)intel8245_ram[0x10 + 4 * char_number + 3] & 0x01) << 8) +
							((uint16_t)intel8245_ram[0x10 + 4 * char_number + 2]);
						cset_start_address = (offset + (start_y / 2)) % 512;
						displayed_chars[char_number].cset_start_address = cset_start_address;
						height = 8 - (cset_start_address % 8);
						end_y = start_y + height - 1;
						// Serial.print("Char = ");
						// Serial.print(char_number);
						// Serial.print(" Height = ");
						// Serial.print(height);
						// Serial.print(", start_y = ");
						// Serial.print(start_y);
						// Serial.print(", end_y = ");
						// Serial.println(end_y);
						displayed_chars[char_number].height = height;
						displayed_chars[char_number].end_y = end_y;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							//  Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].start_y =
								displayed_chars[char_number + 2].start_y =
									displayed_chars[char_number + 3].start_y = start_y;
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
							displayed_chars[char_number + 1].end_y =
								displayed_chars[char_number + 2].end_y =
									displayed_chars[char_number + 3].end_y = end_y;
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
						displayed_chars[char_number].start_x = data - 8 + LEFT_OFFSET; // TODO verifier ce -8
						if (char_number >= 12)										   // 1st char from a quad
						{
							//
							// Copy (after slight re-computing) modified data in remaining three chars
							//
							displayed_chars[char_number + 1].start_x = displayed_chars[char_number].start_x + 16;
							displayed_chars[char_number + 2].start_x = displayed_chars[char_number].start_x + 32;
							displayed_chars[char_number + 3].start_x = displayed_chars[char_number].start_x + 48;
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
						uint8_t end_y = start_y + height - 1;
						// Serial.print("Char = ");
						// Serial.print(char_number);
						// Serial.print(" Start_y = ");
						// Serial.print(start_y);
						// Serial.print(", end_y = ");
						// Serial.println(end_y);
						displayed_chars[char_number].height = height;
						displayed_chars[char_number].end_y = end_y;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
							displayed_chars[char_number + 1].end_y =
								displayed_chars[char_number + 2].end_y =
									displayed_chars[char_number + 3].end_y = end_y;
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
						uint8_t end_y = start_y + height - 1;
						displayed_chars[char_number].height = height;
						displayed_chars[char_number].end_y = end_y;
						if (char_number >= 12) // 1st char from a quad
						{
							//
							// Copy modified data in remaining three chars
							//
							displayed_chars[char_number + 1].height =
								displayed_chars[char_number + 2].height =
									displayed_chars[char_number + 3].height = height;
							displayed_chars[char_number + 1].end_y =
								displayed_chars[char_number + 2].end_y =
									displayed_chars[char_number + 3].end_y = end_y;
						}
					}
				}
				}
			}
			else if (addr >= 0x80 && addr < 0xA0) // Sprite shapes
			{
				intel8245_ram[addr] = data;
				sprites_uptodate = false;
				uint8_t sprite_number = (addr - 0x80) / 8;
				displayed_sprites[sprite_number].changed = true;
#if defined(DEBUG)
				Serial.print("Sprites shapes - sprite number ");
				Serial.println(sprite_number);
				Serial.print("Value ");
				Serial.println(data);
#endif
			}
			else if (addr == 0xA0) // VDC Control Register
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
#if defined(DEBUG)
					Serial.print("Changing grid display: ");
					Serial.println((data & 0x08) ? "on" : "off");
#endif
					// TODO understand how it works
					if (!grid_control && (data & 0x08))
					{
						grid_control = data & 0x08;
						grid_uptodate = false;
						h_segs_uptodate = false;
						v_segs_uptodate = false;
						dots_uptodate = false;
						for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
							h_segments[h_segment].changed = true;
						for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
							h_segments[v_segment].changed = true;
					}

#if defined(DEBUG)
					Serial.print("grid_control = ");
					Serial.println(grid_control);
#endif
				}
				if ((intel8245_ram[0xA0] & 0x20) != (data & 0x20)) // Foreground objects
				{
					// Foreground objects (ie chars and sprites) display on / off
					// Nota bene: FG objects should never been set off once already set on
					foreground_control = data & 0x20;
					sprites_uptodate = false;
					chars_uptodate = false;
#if defined(DEBUG)
					Serial.print("foreground_control = ");
					Serial.println(foreground_control);
#endif
				}
				if ((intel8245_ram[0xA0] & 0x40) != (data & 0x40)) // Dots objects
					grid_dots = data & 0x40;
				if ((intel8245_ram[0xA0] & 0x80) != (data & 0x80))
				{ // Vertical segments width
					v_segments_width = (data & 0x80) ? 16 : 2;
					init_v_grid_elements();
					for (uint8_t column = 0; column < 10; column++)
						for (uint8_t row = 0; row < 8; row++)
							v_segments[column * 8 + row].changed = true;
				}
				intel8245_ram[0xA0] = data;
			}
			else if (addr == 0xA3) // VDC Color Register
			{
				if ((intel8245_ram[0xA3] & 0x47) != (data & 0x47)) // Grid color
				{
#if defined(DEBUG)
					Serial.println("Changing grid color");
#endif
					// Grid color
					grid_uptodate = false;
					h_segs_uptodate = false;
					v_segs_uptodate = false;
					dots_uptodate = false;
					grid_color = (data & 0x40) ? LIGHT_COLORS(data & 0x07) : DARK_COLORS(data & 0x07);
					for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
						h_segments[h_segment].changed = true;
					for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
						h_segments[v_segment].changed = true;
				}
				if ((intel8245_ram[0xA3] & 0x38) != (data & 0x38)) // Luminance and background color
				{
					// Background color TODO check if luminance is used for BG color
					background_color = DARK_COLORS((data & 0x38) >> 3);
					//
					// Need to force redrawing everything !
					//
					background_uptodate = false;
					sprites_uptodate = false;
					chars_uptodate = false;
					grid_uptodate = false;
					h_segs_uptodate = false;
					v_segs_uptodate = false;
					dots_uptodate = false;
					// Force redrawing grid elements
					// TODO: find a way to indicate to the all show routines that all elements must be drawn
					for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
						h_segments[h_segment].changed = true;
					for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
						v_segments[v_segment].changed = true;
					// Force redrawing chars
					for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
						displayed_chars[char_number].changed = true;
					// Force redrawing sprites
					for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
						displayed_sprites[sprite_number].changed = true;
				}
				intel8245_ram[0xA3] = data;
			}
			//			else if (addr >= 0xA7 && addr <= 0xAA) // VDC Sound Register
			//				intel8245_ram[addr] = data;		   // TODO; move this to an upper level
			else if (addr >= 0xC0 && addr <= 0xC8) // Grid horizontal segments 0 - 7
			{
				if (intel8245_ram[addr] != data)
				{
					intel8245_ram[addr] = data;
					grid_uptodate = false;
					h_segs_uptodate = false;
					uint8_t base_index = (addr - 0xC0) * 9;
					uint8_t mask = 0x01;
					for (uint8_t bit = 0; bit < 8; bit++)
					{
						if ((data & mask) && h_segments[base_index + bit].displayed == false)
						{
							h_segments[base_index + bit].changed = true;
							h_segments[base_index + bit].displayed = true;
						}
						else if (!(data & mask) && h_segments[base_index + bit].displayed == true)
						{
							h_segments[base_index + bit].changed = true;
							h_segments[base_index + bit].displayed = false;
						}
						mask <<= 1;
					}
				}
			}
			else if (addr >= 0xD0 && addr <= 0xD8) // Grid horizontal segments 8
			{
				if (intel8245_ram[addr] != data)
				{
					intel8245_ram[addr] = data;
					grid_uptodate = false;
					h_segs_uptodate = false;
					uint8_t base_index = (addr - 0xD0) * 9;
					if (data & 0x01 && h_segments[base_index + 8].displayed == false)
					{
						h_segments[base_index + 8].changed = true;
						h_segments[base_index + 8].displayed = true;
					}
					else if (!(data & 0x01) && h_segments[base_index + 8].displayed == true)
					{
						h_segments[base_index + 8].changed = true;
						h_segments[base_index + 8].displayed = false;
					}
				}
			}
			else if (addr >= 0xE0 && addr <= 0xE9) // Grid vertical segments
			{
				if (intel8245_ram[addr] != data)
				{
					intel8245_ram[addr] = data;
					grid_uptodate = false;
					v_segs_uptodate = false;
					uint8_t base_index = (addr - 0xE0) * 8;
					uint8_t mask = 0x01;
					for (uint8_t bit = 0; bit < 8; bit++)
					{
						if (data & mask && v_segments[base_index + bit].displayed == false)
						{
							v_segments[base_index + bit].changed = true;
							v_segments[base_index + bit].displayed = true;
						}
						else if (!(data & mask) && v_segments[base_index + bit].displayed == true)
						{
							v_segments[base_index + bit].changed = true;
							v_segments[base_index + bit].displayed = false;
						}
						mask <<= 1;
					}
				}
			}
			else if (addr == 0xA2) // VDC Collision Register
				intel8245_ram[addr] = data;
		}
		break;
	}
	}
}

void write_port1(uint8_t data)
{
#if defined(DEBUG)
	Serial.print(" - write_port1(0x");
	Serial.print(data, HEX);
	Serial.println(")");
#endif

	port1 = data;
	rom_bank_select = (port1 & 0x01) ? 0x1000 : 0x0000;

#if defined(DEBUG)
	Serial.print("rom_bank_select == 0x");
	Serial.println(rom_bank_select, HEX);
#endif
}

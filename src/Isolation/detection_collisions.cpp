if (addr == 0xA2) // VDC Collisions Register
{

    Serial.print("Checking collisions - ");
    Serial.println(data, HEX);
    if (data != 0x00)
    {
        uint8_t result = 0x00;

        /*
Serial.print("ext_write() [0x");
Serial.print(addr, HEX);
Serial.print("] <- 0x");
Serial.println(data, HEX);
        if ((data & 0xF0) != 0x00)
        {
            Serial.println("Testing other object than sprite");
            // delay(10000);
        }
        else if ((data & 0x0F) != 0x01 && (data & 0x0F) != 0x02 && (data & 0x0F) != 0x04 && (data & 0x0F) != 0x08)
        {
            Serial.println("Testing many sprites");
            // delay(10000);
        }
        */
        if (data & 0x0F) // testing collisions for sprites
        {
            uint8_t mask = 0x01;
            for (uint8_t sprite_number = 0; sprite_number < NB_SPRITES; sprite_number++)
            {
                if (data & mask)
                {
                    uint8_t second_mask = 0x02 << sprite_number; // Sprite / sprite collisions
                    for (uint8_t second_sprite_number = sprite_number + 1; second_sprite_number < NB_SPRITES; second_sprite_number++)
                    {
                        if (data & second_mask)
                        {
                            if (
                                (((displayed_sprites[sprite_number].start_x >= displayed_sprites[second_sprite_number].start_x) &&
                                  (displayed_sprites[sprite_number].start_x <= displayed_sprites[second_sprite_number].end_x)) ||
                                 ((displayed_sprites[sprite_number].end_x >= displayed_sprites[second_sprite_number].start_x) &&
                                  (displayed_sprites[sprite_number].end_x <= displayed_sprites[second_sprite_number].end_x))) &&
                                (((displayed_sprites[sprite_number].start_y >= displayed_sprites[second_sprite_number].start_y) &&
                                  (displayed_sprites[sprite_number].start_y <= displayed_sprites[second_sprite_number].end_y)) ||
                                 ((displayed_sprites[sprite_number].end_y >= displayed_sprites[second_sprite_number].start_y) &&
                                  (displayed_sprites[sprite_number].end_y <= displayed_sprites[second_sprite_number].end_y))))
                            {
                                result |= mask;
                                result |= second_mask;
                            }
                        }
                        second_mask <<= 1;
                    }
                    if (data & 0x10) // Sprite / vgrid collisions
                    {
                        //
                        // TODO: instead of browsing through all segments should compute which one(s) sprite can interfere
                        //
                        for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
                        {
                            if (v_segments[v_segment].changed_displayed & 0x01)
                            {
                                if (
                                    (((displayed_sprites[sprite_number].start_x >= v_segments[v_segment].start_x) &&
                                      (displayed_sprites[sprite_number].start_x <= v_segments[v_segment].end_x)) ||
                                     ((displayed_sprites[sprite_number].end_x >= v_segments[v_segment].start_x) &&
                                      (displayed_sprites[sprite_number].end_x <= v_segments[v_segment].end_x))) &&
                                    (((displayed_sprites[sprite_number].start_y >= v_segments[v_segment].start_y) &&
                                      (displayed_sprites[sprite_number].start_y <= v_segments[v_segment].start_y + 23)) ||
                                     ((displayed_sprites[sprite_number].end_y >= v_segments[v_segment].start_y) &&
                                      (displayed_sprites[sprite_number].end_y <= v_segments[v_segment].start_y + 23))))
                                {
#if DEBUG_SERIAL
                                    Serial.print("Collision sprite ");
                                    Serial.print(sprite_number);
                                    Serial.print(" x ");
                                    Serial.print(displayed_sprites[sprite_number].start_x);
                                    Serial.print(" y ");
                                    Serial.print(displayed_sprites[sprite_number].start_y);
                                    Serial.print(" avec v segment ");
                                    Serial.print(v_segment);
                                    Serial.print(" x ");
                                    Serial.print(v_segments[v_segment].start_x);
                                    Serial.print(" y ");
                                    Serial.println(v_segments[v_segment].start_y);
#endif
                                    result |= mask;
                                    result |= 0x10;
                                }
                            }
                        }
                    }
                    if (data & 0x20) // Sprite / hgrid+dots collisions
                    {
                        //
                        // TODO: instead of browsing through all segments should compute which one(s) sprite can interfere
                        // TODO: add dots processing
                        //
                        for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
                        {
                            if (h_segments[h_segment].changed_displayed & 0x01)
                            {
                                if (
                                    (((displayed_sprites[sprite_number].start_x >= h_segments[h_segment].start_x) &&
                                      (displayed_sprites[sprite_number].start_x <= h_segments[h_segment].start_x + 15)) ||
                                     ((displayed_sprites[sprite_number].end_x >= h_segments[h_segment].start_x) &&
                                      (displayed_sprites[sprite_number].end_x <= h_segments[h_segment].start_x + 15))) &&
                                    (((displayed_sprites[sprite_number].start_y >= h_segments[h_segment].start_y) &&
                                      (displayed_sprites[sprite_number].start_y <= h_segments[h_segment].start_y + 2)) ||
                                     ((displayed_sprites[sprite_number].end_y >= h_segments[h_segment].start_y) &&
                                      (displayed_sprites[sprite_number].end_y <= h_segments[h_segment].start_y + 2))))
                                {
                                    result |= mask;
                                    result |= 0x20;
                                }
                            }
                        }
                    }
                    if (data & 0x80) // Sprite / chars collisions
                    {
                        for (uint8_t disp_char = 0; disp_char < NB_CHARS; disp_char++)
                        {
                            if (
                                (((displayed_sprites[sprite_number].start_x >= displayed_chars[disp_char].start_x) &&
                                  (displayed_sprites[sprite_number].start_x <= displayed_chars[disp_char].start_x + 7)) ||
                                 ((displayed_sprites[sprite_number].end_x >= displayed_chars[disp_char].start_x) &&
                                  (displayed_sprites[sprite_number].end_x <= displayed_chars[disp_char].start_x +

                                                                                 7))) &&
                                (((displayed_sprites[sprite_number].start_y >= displayed_chars[disp_char].start_y) &&
                                  (displayed_sprites[sprite_number].start_y <= displayed_chars[disp_char].end_y)) ||
                                 ((displayed_sprites[sprite_number].end_y >= displayed_chars[disp_char].start_y) &&
                                  (displayed_sprites[sprite_number].end_y <= displayed_chars[disp_char].end_y))))
                            {
                                result |= mask;
                                result |= 0x80;
                            }
                        }
                    }
                }
            }
        }
        if (data & 0x80) // testing collisions for chars
        {
            for (uint8_t char_number = 0; char_number < NB_CHARS; char_number++)
            {
                if (data & 0x10) // Char / vgrid collisions
                {
                    //
                    // TODO: instead of browsing through all segments should compute which one(s) char can interfere with
                    //
                    for (uint8_t v_segment = 0; v_segment < NB_V_SEGMENTS; v_segment++)
                    {
                        if (v_segments[v_segment].changed_displayed & 0x01)
                        {
                            if (
                                (((displayed_chars[char_number].start_x >= v_segments[v_segment].start_x) &&
                                  (displayed_chars[char_number].start_x <= v_segments[v_segment].end_x)) ||
                                 ((displayed_chars[char_number].start_x + 7 >= v_segments[v_segment].start_x) &&
                                  (displayed_chars[char_number].start_x + 7 <= v_segments[v_segment].end_x))) &&
                                (((displayed_chars[char_number].start_y >= v_segments[v_segment].start_y) &&
                                  (displayed_chars[char_number].start_y <= v_segments[v_segment].start_y + 23)) ||
                                 ((displayed_chars[char_number].end_y >= v_segments[v_segment].start_y) &&
                                  (displayed_chars[char_number].end_y <= v_segments[v_segment].start_y + 23))))
                            {
                                result |= 0x80;
                                result |= 0x10;
                            }
                        }
                    }
                }
                if (data & 0x20) // Char / hgrid+dots collisions
                {
                    //
                    // TODO: instead of browsing through all segments should compute which one(s) char can interfere with
                    // TODO: add dots processing
                    //
                    for (uint8_t h_segment = 0; h_segment < NB_H_SEGMENTS; h_segment++)
                    {
                        if (h_segments[h_segment].changed_displayed & 0x01)
                        {
                            if (
                                (((displayed_chars[char_number].start_x >= h_segments[h_segment].start_x) &&
                                  (displayed_chars[char_number].start_x <= h_segments[h_segment].start_x + 15)) ||
                                 ((displayed_chars[char_number].start_x + 7 >= h_segments[h_segment].start_x) &&
                                  (displayed_chars[char_number].start_x + 7 <= h_segments[h_segment].start_x + 15))) &&
                                (((displayed_chars[char_number].start_y >= h_segments[h_segment].start_y) &&
                                  (displayed_chars[char_number].start_y <= h_segments[h_segment].start_y + 2)) ||
                                 ((displayed_chars[char_number].end_y >= h_segments[h_segment].start_y) &&
                                  (displayed_chars[char_number].end_y <= h_segments[h_segment].start_y + 2))))
                            {
                                result |= 0x80;
                                result |= 0x20;
                            }
                        }
                    }
                }
                for (uint8_t second_char_number = 0; second_char_number < NB_CHARS; second_char_number++)
                {
                    if (
                        (((displayed_chars[char_number].start_x >= displayed_chars[second_char_number].start_x) &&
                          (displayed_chars[char_number].start_x <= displayed_chars[second_char_number].start_x + 7)) ||
                         ((displayed_chars[char_number].start_x + 7 >= displayed_chars[second_char_number].start_x) &&
                          (displayed_chars[char_number].start_x + 7 <= displayed_chars[second_char_number].start_x + 7))) &&
                        (((displayed_chars[char_number].start_y >= displayed_chars[second_char_number].start_y) &&
                          (displayed_chars[char_number].start_y <= displayed_chars[second_char_number].end_y)) ||
                         ((displayed_chars[char_number].end_y >= displayed_chars[second_char_number].start_y) &&
                          (displayed_chars[char_number].end_y <= displayed_chars[second_char_number].end_y))))
                    {
                        result |= 0x80;
                    }
                }
            }
            // intel8245_ram[addr] = result;
            intel8245_ram[addr] = data;
            Serial.print("Collision");
            Serial.println(result, HEX);
        }
    }
}
CC = gcc

CFLAGS = -O3 -fomit-frame-pointer
LFLAGS = -s

arduipac: arduipac.o arduipac_vmachine.o arduipac_8048.o arduipac_8245.o arduipac_bios_rom.o arduipac_cset.o arduipac_input.o mnemonics.o arduipac_colors.c
	$(CC) $(LFLAGS) arduipac.o arduipac_vmachine.o arduipac_8048.o arduipac_8245.o arduipac_bios_rom.o arduipac_cset.o arduipac_input.o mnemonics.o arduipac_colors.o -o arduipac

clean:
	rm -f arduipac.o arduipac_8048.o arduipac_8245.o arduipac_bios_rom.o arduipac_cset.o arduipac_input.o mnemonics.o arduipac_colors.o

arduipac_vmachine.o: arduipac_vmachine.c arduipac_vmachine.h arduipac_8048.h arduipac_8245.h arduipac_vmachine.h arduipac_bios_rom.h mnemonics.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_vmachine.c -o arduipac_vmachine.o

arduipac_8048.o: arduipac_8048.c arduipac_8048.h arduipac_8245.h arduipac_vmachine.h arduipac_bios_rom.h mnemonics.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_8048.c -o arduipac_8048.o

arduipac_8245.o: arduipac_8245.c arduipac_8245.h arduipac_8048.h arduipac_vmachine.h arduipac_cset.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_8245.c -o arduipac_8245.o

arduipac_bios_rom.o: arduipac_bios_rom.c arduipac_bios_rom.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_bios_rom.c -o arduipac_bios_rom.o

arduipac.o: arduipac.c arduipac_vmachine.h arduipac_8048.h arduipac_8245.h arduipac_bios_rom.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac.c -o arduipac.o

arduipac_cset.o: arduipac_cset.c arduipac_cset.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_cset.c -o arduipac_cset.o

arduipac_input.o: arduipac_input.c arduipac_input.h arduipac_8048.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_input.c -o arduipac_input.o

mnemonics.o: mnemonics.c mnemonics.h arduipac_config.h
	$(CC) $(CFLAGS) -c mnemonics.c -o mnemonics.o

arduipac_colors.o: arduipac_colors.c arduipac_colors.h arduipac_config.h
	$(CC) $(CFLAGS) -c arduipac_colors.c -o arduipac_colors.o

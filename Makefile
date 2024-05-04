GBDK_FOLDER = "Insert your gbdk folder path"

CC	= $(GBDK_FOLDER)bin/lcc -Wa-l -Wl-m -Wl-j

BINS	= inquisition.gb

all:	$(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat

# Compile and link single file in one pass
%.gb:	%.c
	$(CC) -o $@ $<
	rm -f *.map *.noi *.sav

clean:
	rm -f *.o *.lst *.map *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.sav


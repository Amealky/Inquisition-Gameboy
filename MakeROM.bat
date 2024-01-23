lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o game.o game.c
lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o game.gb game.o
pause
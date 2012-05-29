all: curses_firework \
     curses_rain \
     curses_worm \
     curses_xmas \
     hello_hterm \
     hello_nativeblack \
     hello_nativeblack_fileio \
     hello_nativeblue_fileio \
     hello_nativeblue_hterm

.PHONY:curses_firework
curses_firework:
	make -C curses_firework 

.PHONY:curses_rain
curses_rain:
	make -C curses_rain

.PHONY:curses_worm
curses_worm:
	make -C curses_worm

.PHONY:curses_xmas
curses_xmas:
	make -C curses_xmas

.PHONY:hello_hterm
hello_hterm:
	make -C hello_hterm

.PHONY:hello_nativeblack
hello_nativeblack:
	make -C hello_nativeblack

.PHONY:hello_nativeblack_fileio
hello_nativeblack_fileio:
	make -C hello_nativeblack_fileio

.PHONY:hello_nativeblue_fileio
hello_nativeblue_fileio:
	make -C hello_nativeblue_fileio

.PHONY:hello_nativeblue_hterm
hello_nativeblue_hterm:
	make -C hello_nativeblue_hterm

.PHONY:clean
clean:
	make -C curses_firework clean
	make -C curses_rain clean
	make -C curses_worm clean
	make -C curses_xmas clean
	make -C hello_hterm clean
	make -C hello_nativeblack clean
	make -C hello_nativeblack_fileio clean
	make -C hello_nativeblue_fileio clean
	make -C hello_nativeblue_hterm clean

.PHONY:install
install:
	make -C curses_firework install
	make -C curses_rain install
	make -C curses_worm install
	make -C curses_xmas install
	make -C hello_hterm install
	make -C hello_nativeblack install
	make -C hello_nativeblack_fileio install
	make -C hello_nativeblue_fileio install
	make -C hello_nativeblue_hterm install

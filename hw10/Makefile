exe: download_lib_c download_lib_h build_daemon build_lib build_all

build_daemon:
	gcc -c main.c

download_lib_c:
	[ -f ini.c ] || curl -o ini.c https://raw.githubusercontent.com/benhoyt/inih/master/ini.c

download_lib_h:
	[ -f ini.h ] || curl -o ini.h https://raw.githubusercontent.com/benhoyt/inih/master/ini.h

build_lib:
	gcc -c ini.c

build_all:
	gcc -o main main.o ini.o

#CC=clang
CC=gcc

INCLUDE_PATH=include/
SRC_SBLT=sblt/main.c
SRC_SBLTD=sbltd/main.c

BIN_SBLT=sblt/sblt
BIN_SBLTD=sbltd/sbltd

all: sblt.all

sblt.all: sblt.o sbltd.o
	${CC} ${BIN_SBLT}.o -o ${BIN_SBLT} 
	${CC} ${BIN_SBLTD}.o -o ${BIN_SBLTD} 
sblt.o:
	${CC} ${SRC_SBLT} -c -o ${BIN_SBLT}.o -I ${INCLUDE_PATH}
sbltd.o:
	${CC} ${SRC_SBLTD} -c -o ${BIN_SBLTD}.o -I ${INCLUDE_PATH}
install:
	install -Dm755 ${BIN_SBLT} /usr/bin/sblt
	install -Dm755 ${BIN_SBLTD} /usr/bin/sbltd
	install -Dm755 sbltd-stop.sh /usr/bin/sbltd-stop.sh
	install -Dm664 sbltd.service /etc/systemd/system/sbltd.service

uninstall:
	rm -rf /usr/bin/sblt
	rm -rf /usr/bin/sbltd
	rm -rf /usr/bin/sbltd-stop.sh
	rm -rf /etc/systemd/system/sbltd.service

clean:
	rm -rf ${BIN_SBLT} ${BIN_SBLTD} ${BIN_SBLT}.o ${BIN_SBLTD}.o

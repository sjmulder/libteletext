DESTDIR?=
PREFIX?=	/usr/local

PROG=	teletext

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-I../libteletext/include

LDFLAGS+=	-L../libteletext
LDFLAGS+=	-Wl,-rpath=../libteletext
LDFLAGS+=	-Wl,-rpath=${PREFIX}/lib

LDLIBS+=	-lteletext -ljson-c -lcurl

all: ${PROG}

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -m755 ${PROG} ${DESTDIR}${PREFIX}/bin/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}

clean:
	rm -f ${PROG} *.o

teletext.o:	../libteletext/include/teletext.h

.PHONY: all clean

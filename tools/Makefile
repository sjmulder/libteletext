DESTDIR?=
PREFIX?=	/usr/local

PROGS=	teletext teletext-convert

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-I../libteletext/include

LDFLAGS+=	-L../libteletext
LDFLAGS+=	-Wl,-rpath=../libteletext
LDFLAGS+=	-Wl,-rpath=${PREFIX}/lib

LDLIBS+=	-lteletext -ljson-c -lcurl

all: ${PROGS}

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -m755 teletext teletext-convert ${DESTDIR}${PREFIX}/bin/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/teletext \
	      ${DESTDIR}${PREFIX}/bin/teletext-convert

clean:
	rm -f ${PROGS} *.o

teletext.o:		../libteletext/include/teletext.h
teletext-convert.o:	../libteletext/include/teletext.h

.PHONY: all clean

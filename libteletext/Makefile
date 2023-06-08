DESTDIR?=
PREFIX?=	/usr/local

LIB=	libteletext.so
OBJ=	page.o page_nos.o util.o

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-fPIC
CFLAGS+=	-I./include

all: ${LIB}

install: all
	install -d ${DESTDIR}${PREFIX}/lib
	install -m755 ${LIB} ${DESTDIR}${PREFIX}/lib/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/lib/${LIB}

clean:
	rm -f ${LIB} *.o

${LIB}: ${OBJ}
	${LINK.c} -shared -o ${LIB} ${OBJ}

page.o:		include/teletext.h private.h
page_nos.o:	include/teletext.h private.h
util.o:		include/teletext.h private.h

.PHONY: all install uninstall clean

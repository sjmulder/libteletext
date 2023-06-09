SRCROOT=	..

include ../common.mk

LIB?=		libteletext.so
OBJ=		page.o page_nos.o util.o cpp.o

CFLAGS+=	-fPIC
CXXFLAGS+=	-fPIC
LDLIBS+=	-ljson-c -lcurl

all: ${LIB}

install: all
	install -d ${DESTDIR}${PREFIX}/lib
	install -m755 ${LIB} ${DESTDIR}${PREFIX}/lib/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/lib/${LIB}

clean:
	rm -f ${LIB} *.o

${LIB}: ${OBJ}
	${LINK.cc} -shared -o ${LIB} ${OBJ} ${LDLIBS}

page.o:		include/teletext.h private.h
page_nos.o:	include/teletext.h private.h
util.o:		include/teletext.h private.h
cpp.o:		include/teletext.h

.PHONY: all install uninstall clean

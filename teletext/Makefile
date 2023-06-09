SRCROOT=	..

PROG?=		teletext
OBJ=		teletext.o

include ../common.mk

LDLIBS+=	-lteletext -ljson-c -lcurl

all: ${PROG}

install: all
	install -d ${DESTDIR}${PREFIX}/bin
	install -m755 ${PROG} ${DESTDIR}${PREFIX}/bin/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/${PROG}

clean:
	rm -f ${PROG} *.o

${PROG}: ${OBJ}
	${LINK.cc} -o ${PROG} ${OBJ} ${LDLIBS}

teletext.o:	../libteletext/include/teletext.h

.PHONY: all clean

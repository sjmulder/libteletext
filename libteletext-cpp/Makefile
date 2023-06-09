SRCROOT=	..

include ../common.mk

LIB?=		libteletext-cpp.a
OBJ=		libteletext-cpp.o

all: ${LIB}

install: all
	install -d ${DESTDIR}${PREFIX}/lib
	install -m755 ${LIB} ${DESTDIR}${PREFIX}/lib/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/lib/${LIB}

clean:
	rm -f ${LIB} *.o

${LIB}: ${OBJ}
	${AR} ${ARFLAGS} ${LIB} ${OBJ}

.PHONY: all install uninstall clean

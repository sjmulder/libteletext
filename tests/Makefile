SRCROOT=	..

include ../common.mk

PROGS=		test-cpp

LDLIBS+=	-lteletext

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

test-cpp: test-cpp.o
	${LINK.cc} -o test-cpp test-cpp.o ${LDLIBS}

.PHONY: all clean

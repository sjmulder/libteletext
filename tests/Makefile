PROGS=	test-cpp

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-I../libteletext/include

CXXFLAGS+=	-Wall -Wextra
CXXFLAGS+=	-I../libteletext/include

LDFLAGS+=	-L../libteletext
LDFLAGS+=	-Wl,-rpath=../libteletext
LDFLAGS+=	-Wl,-rpath=${PREFIX}/lib

LDLIBS+=	-lteletext -ljson-c -lcurl

all: ${PROGS}

clean:
	rm -f ${PROGS} *.o

test-cpp: test-cpp.o
	${LINK.cc} -o test-cpp test-cpp.o ${LDLIBS}

.PHONY: all clean

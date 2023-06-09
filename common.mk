DESTDIR?=
PREFIX?=	/usr/local

SRCROOT?=	.

CFLAGS+=	-Wall -Wextra
CFLAGS+=	-I${SRCROOT}/libteletext/include

CXXFLAGS+=	-Wall -Wextra
CXXFLAGS+=	-I${SRCROOT}/libteletext/include

LDFLAGS+=	-L${SRCROOT}/libteletext
LDFLAGS+=	-L${SRCROOT}/libteletext-cpp
LDFLAGS+=	-Wl,-rpath=${SRCROOT}/libteletext

LDFLAGS+=	-L${SRCROOT}/libteletext
LDFLAGS+=	-L${SRCROOT}/libteletext-cpp
LDFLAGS+=	-Wl,-rpath=${PREFIX}/lib
LDFLAGS+=	-Wl,-rpath=${SRCROOT}/libteletext
LDFLAGS+=	-Wl,-rpath=${SRCROOT}/libteletext-cpp

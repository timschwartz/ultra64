noinst_LIBRARIES = libultra64.a

libultra64_a_SOURCES = CRC.h N64.cpp ROM.cpp PIFrom.cpp MMU.cpp instruction.cpp \
	vr4300/vr4300.cpp vr4300/cp0.cpp vr4300/special.cpp vr4300/regimm.cpp \
	rcp/RSP.cpp
libultra64_a_CXXFLAGS = -std=c++17 -Wall -Werror ${JSONCPP_CFLAGS} -I.
libultra64_a_LIBADD = ${JSONCPP_LIBS}

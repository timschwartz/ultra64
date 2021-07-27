bin_PROGRAMS = ultra64
ultra64_SOURCES = ultra64.cpp MainWindow.cpp RegistersWindow.cpp DebuggerWindow.cpp \
	n64/ROM.cpp n64/PIFrom.cpp n64/MMU.cpp n64/instruction.cpp \
	n64/vr4300/vr4300.cpp n64/vr4300/cp0.cpp n64/vr4300/special.cpp \
	n64/vr4300/regimm.cpp n64/rcp/RSP.cpp
ultra64_CXXFLAGS = -std=c++17 -Wall -Werror ${JSONCPP_CFLAGS}
ultra64_LDADD = ${JSONCPP_LIBS}

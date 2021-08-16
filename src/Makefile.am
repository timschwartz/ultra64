bin_PROGRAMS = ultra64
ultra64_SOURCES = CRC.h ultra64.hpp ultra64.cpp MainWindow.hpp MainWindow.cpp \
	RegistersWindow.hpp RegistersWindow.cpp DebuggerWindow.hpp DebuggerWindow.cpp \
	n64/N64.hpp n64/N64.cpp n64/ROM.hpp n64/ROM.cpp n64/PIFrom.hpp n64/PIFrom.cpp \
	n64/MMU.hpp n64/MMU.cpp n64/instruction.hpp n64/instruction.cpp \
	n64/vr4300/vr4300.hpp n64/vr4300/vr4300.cpp n64/vr4300/cp0.cpp n64/vr4300/special.cpp \
	n64/vr4300/regimm.cpp n64/rcp/RSP.hpp n64/rcp/RSP.cpp \
	n64/instruction.hpp n64/opcodes.hpp
ultra64_CXXFLAGS = -std=c++17 -Wall -Werror ${JSONCPP_CFLAGS}
ultra64_LDADD = ${JSONCPP_LIBS}

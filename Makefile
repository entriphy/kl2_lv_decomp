EE_BIN = bin/kl2_lv.elf
EE_OBJS = src/kl2_lv.o src/hoshino/init.o src/common.o src/hato/rand.o
EE_LIBS = -lvux -ldebug -lc -lcdvd -lxcdvd

EE_OPTFLAGS = -O1
EE_WARNFLAGS = -Werror
EE_CXXFLAGS = -g3 -gdwarf-4 -gstrict-dwarf -Iinclude -Wno-strict-aliasing

all: $(EE_BIN)

clean:
	rm -f src/*.o
	rm -f src/**/*.o
	rm -rf bin

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal_cpp
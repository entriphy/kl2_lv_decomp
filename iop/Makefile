ifeq ($(wildcard PathDefs),)
PathDefs:
	/usr/local/sce/bin/iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
	$(MAKE) all
else
include PathDefs
endif

# Config
TOPDIR = /usr/local/sce
INCDIR = -I$(TOPDIR)/common/include
LIBDIR = $(TOPDIR)/iop/lib
MODDIR = $(TOPDIR)/iop/modules
 
# Flags
CFLAGS   = $(INCDIR) -Wall -Werror -G 0 -O0
ASFLAGS  = $(INCDIR)
LDLIBS   = -ilb=$(LIBDIR)/libsd.ilb

# Sources
PROGNAME = kl2.irx
OBJS	 = entry.o str.o jam.o sound.o main.o

# Rules
all: $(PROGNAME)

%.irx: $(OBJS)
	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	rm -f *.o $(PROGNAME) *.map


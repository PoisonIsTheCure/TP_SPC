### Do not modify: edit Makefile.project instead ###

include Makefile.project

#LDSCRIPT = link/stm32l073.ld
LDSCRIPT = link/stm32f446.ld

LDDEPS = $(LDSCRIPT) link/stm32_generic.ld

TOOLSDIR = devtools

SRCDIR = src
OBJDIR = obj
BINDIR = bin

ELF = $(BINDIR)/$(PROGNAME).elf
BIN = $(BINDIR)/$(PROGNAME).bin

OBJS = $(patsubst $(SRCDIR)/%.S,$(OBJDIR)/%.o,$(ASMS))
OBJS += $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPPSRCS))

CRCOBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CRCSRC))
CRCOBJOK = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_ok.o,$(CRCSRC))
CRCLSTOK = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_ok.lst,$(CRCSRC))
CRCOBJS = $(filter-out $(CRCOBJ),$(OBJS)) $(CRCOBJOK)

ARCHFLAGS = -mtune=cortex-m4 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16
FFLAGS = -fsingle-precision-constant
DIRFLAGS += -Isrc
CFLAGS = $(ARCHFLAGS) $(FFLAGS) -O0 -Os -Wall -Wdouble-promotion -std=gnu99 $(DIRFLAGS)

all: $(ELF)

bin: $(BIN)

clean :
	rm -rf $(OBJDIR) $(BINDIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(CDEPS) Makefile
	@mkdir -p $(dir $@)
	@echo "COMPILE $< => $@"
	@arm-none-eabi-gcc $(CFLAGS) -Wa,-ahlns=$(OBJDIR)/$*.lst -g -c $< -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(CPPDEPS) $(CDEPS) Makefile
	@mkdir -p $(dir $@)
	@echo "COMPILE $< => $@"
	@arm-none-eabi-g++ $(CFLAGS) -Wa,-ahlns=$(OBJDIR)/$*.lst -g -c $< -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.S $(ASMDEPS) Makefile
	@mkdir -p $(dir $@)
	@echo "COMPILE $< => $@"
	@arm-none-eabi-gcc $(ARCHFLAGS) -Wa,-ahlns=$(OBJDIR)/$*.lst -g -c $< -o $@

$(ELF).nocrc : $(OBJS) $(LDDEPS)
	@mkdir -p $(dir $@)
	@echo "LINK (without crc) $(OBJS) => $@"
	@arm-none-eabi-$(LINKER) $(ARCHFLAGS) $(OBJS) $(LINKFLAGS) -T$(LDSCRIPT) -o $@

$(ELF) : $(CRCOBJS)
	@mkdir -p $(dir $@)
	@echo -n "LINK $(CRCOBJS) => $@\n"
	@arm-none-eabi-$(LINKER) $(ARCHFLAGS) $(CRCOBJS) $(LINKFLAGS) -T$(LDSCRIPT) -o $@

$(BIN).nocrc : $(ELF).nocrc
	@mkdir -p $(dir $@)
	@echo "CREATE (without crc) $< => $@"
	@arm-none-eabi-objcopy -R .crc_info -O binary $< $@

$(BIN) : $(ELF)
	@mkdir -p $(dir $@)
	@echo "CREATE $< => $@"
	@arm-none-eabi-objcopy -O binary $< $@

$(CRCOBJOK) : $(BIN).nocrc $(CRCSRC)
	@chmod +x $(TOOLSDIR)/stm32-crc
	@echo -n "COMPUTE CRC : ";\
	 CRC_LINE="$(shell $(TOOLSDIR)/stm32-crc $<)";\
	 echo -n "$$CRC_LINE\n";\
	 echo -n "RECOMPILE (with crc) $(CRCSRC) => $(CRCOBJOK)\n";\
	 arm-none-eabi-gcc $(CFLAGS) -Wa,-ahlns=$(CRCLSTOK) -g -c $(CRCSRC) -o $(CRCOBJOK) \
	   -DFLASH_CRC=0x`echo "$$CRC_LINE" | awk '{print $$1}'` \
	   -DFLASH_SIZE=` echo "$$CRC_LINE" | awk '{print $$2}'`

dump : $(ELF)
	@echo "DUMP $<"
	@arm-none-eabi-objdump -h -t -d $(ELF)

load: $(ELF)
	@chmod +x $(TOOLSDIR)/stm32-load
	@echo "LOAD $<"
	@$(TOOLSDIR)/stm32-load $<

debug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@chmod +x $(TOOLSDIR)/stm32-debug
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-debug $<

rawdebug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-gdb $<


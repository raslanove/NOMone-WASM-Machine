
LINKER_FLAGS = 

CFLAGS = -I../../../Src/Includes/ -DDESKTOP -g -MMD# g=>generate debug info. MMD=>generate dependency files.

# WASM specific
CC = clang
CFLAGS += --target=wasm32 -O3 -nostdlib
LINKER_FLAGS += -Wl,--no-entry -Wl,--export-all -Wl,-z,stack-size=65536

# Optimization flags,
#CFLAGS += -s -O3 -fdata-sections -ffunction-sections 
#LINKER_FLAGS += -Wl,--gc-sections -Wl,--strip-all

SOURCES = \
	$(wildcard ../../../Src/*.c) \
	$(wildcard ../../../Src/Backends/Wasm/*.c) \
	$(wildcard ../Src/*.c) \

OBJECTS = $(SOURCES:.c=.o)

TARGET = HelloWasm.wasm

# Targets start here.
all: $(TARGET)

DEPENDENCIES = $(OBJECTS:.o=.d)
-include $(DEPENDENCIES)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(OBJECTS) $(LINKER_FLAGS)

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDENCIES)

depend:
	@$(CC) $(CFLAGS) -MM $(SOURCES)
        
# list targets that do not create files (but not all makes understand .PHONY)
.PHONY:        all clean depend

# Dependences (call 'make depend' to generate); do not delete:
# Build for these is implicit, no need to specify compiler command lines.

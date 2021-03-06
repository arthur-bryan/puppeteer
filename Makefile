# MIT License
#
# make all:	compile the executable for linux 64 bits by default
# make run:	compile (if not compiled yet) and run the executable for linux 64 bits
# make OUTPUT=linux32:	compile for linux 32 bits
# make OUTPUT=linux64:	compile for linux 64 bits
# make clean: removes the folder (bin) with generated executables


MKDIR	:= mkdir -p
RMDIR	:= rm -r
CC		:= gcc
BIN_PATH	:= ./bin
DATABASE_FILE_PATH	:= ./database/database.db
LD_LIBS	:= -lm

# FOR LINUX BOTS
LINUX_SRC_PATH	:= ./puppets/linux/src
LINUX_SRC_FILES	:= $(wildcard $(LINUX_SRC_PATH)/*.c)
TARGET_LINUX64	:= $(BIN_PATH)/puppet-linux64
TARGET_LINUX32	:= $(BIN_PATH)/puppet-linux32
LINUX_INCLUDE_PATH	:= ./puppets/linux/include
LINUX_OBJ_PATH	:= ./puppets/linux/obj
LINUX_OBJ_FILES	:= $(patsubst $(LINUX_SRC_PATH)/%.c, $(LINUX_OBJ_PATH)/%.o, $(LINUX_SRC_FILES))
LINUX64_CFLAGS	:= -m64 -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)
LINUX32_CFLAGS	:= -m32 -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)

# FOR WINDOWS BOTS
TARGET_WINDOWS64	:= $(BIN_PATH)/puppet-windows64
TARGET_WINDOWS32	:= $(BIN_PATH)/puppet-windows32
WIN32_CC := i686-w64-mingw32-gcc
WIN64_CC := X86_64-w64-mingw32-gcc
WINDOWS64_CFLAFS  := -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)
WINDOWS32_CFLAGS  := -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)

ifdef OUTPUT

ifeq ($(OUTPUT), linux32)
$(TARGET_LINUX32): $(LINUX_OBJ_FILES) | $(BIN_PATH)
	$(CC) -m32 $^ -o $@ $(LD_LIBS)
	$(RMDIR) $(LINUX_OBJ_PATH)
$(LINUX_OBJ_PATH)/%.o: $(LINUX_SRC_PATH)/%.c | $(LINUX_OBJ_PATH)
	$(CC) $(LINUX32_CFLAGS) $< -o $@
endif

ifeq ($(OUTPUT), linux64)
$(TARGET_LINUX64): $(LINUX_OBJ_FILES) | $(BIN_PATH)
	$(CC) $^ -o $@ $(LD_LIBS)
	$(RMDIR) $(LINUX_OBJ_PATH)
$(LINUX_OBJ_PATH)/%.o: $(LINUX_SRC_PATH)/%.c | $(LINUX_OBJ_PATH)
	$(CC) $(LINUX64_CFLAGS) $< -o $@
endif

ifeq ($(OUTPUT), windows32)
$(TARGET_WINDOWS32): $(LINUX_OBJ_FILES) | $(BIN_PATH)
	$(WIN32_CC) $^ -o $@ $(LD_LIBS)
	$(RMDIR) $(LINUX_OBJ_PATH)
$(LINUX_OBJ_PATH)/%.o: $(LINUX_SRC_PATH)/%.c | $(LINUX_OBJ_PATH)
	$(WIN32_CC) $(WINDOWS32_CFLAGS) $< -o $@
endif

else

.PHONY: all run clean

$(TARGET_LINUX64) $(TARGET_LINUX32): $(LINUX_OBJ_FILES) | $(BIN_PATH)
	$(CC) $^ -o $@ $(LD_LIBS)
	$(RMDIR) $(LINUX_OBJ_PATH)

$(LINUX_OBJ_PATH)/%.o: $(LINUX_SRC_PATH)/%.c | $(LINUX_OBJ_PATH)
	$(CC) $(LINUX64_CFLAGS) $< -o $@

endif

$(BIN_PATH) $(LINUX_OBJ_PATH) $(DATABASE_PATH):
	$(MKDIR) $@

all: $(TARGET_LINUX64)

run: $(TARGET_LINUX64)
	$<

clean:
	$(RMDIR) $(BIN_PATH) $(DATABASE_FILE_PATH)

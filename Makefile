# MIT License
#
# make all:	compile the executable for linux 64 bits by default
# make run:	compile (if not compiled yet) and run the executable for linux 64 bits
# make OUTPUT=linux32:	compile for linux 32 bits
# make OUTPUT=linux64:	compile for linux 64 bits
# make clean: removes the generated files

MKDIR	:= mkdir -p
RMDIR	:= rm -rf
CC		:= gcc
BIN_PATH	:= ./bin
DATABASE_FILE_PATH	:= ./database/database.db
LD_LIBS_LINUX	:= -lm
LD_LIBS_WINDOWS	:= -lm -lws2_32

# FOR LINUX BOTS
SRC_PATH	:= ./puppets/src
SRC_FILES	:= $(wildcard $(SRC_PATH)/*.c)
TARGET_LINUX64	:= $(BIN_PATH)/puppet-linux64
TARGET_LINUX32	:= $(BIN_PATH)/puppet-linux32
LINUX_INCLUDE_PATH	:= ./puppets/include
OBJ_PATH	:= ./puppets/obj
OBJ_FILES	:= $(patsubst $(SRC_PATH)/%.c, $(OBJ_PATH)/%.o, $(SRC_FILES))
LINUX64_CFLAGS	:= -m64 -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)
LINUX32_CFLAGS	:= -m32 -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)

# FOR WINDOWS BOTS
TARGET_WINDOWS64	:= $(BIN_PATH)/puppet-windows64
TARGET_WINDOWS32	:= $(BIN_PATH)/puppet-windows32
WIN32_CC := i686-w64-mingw32-gcc
WIN64_CC := x86_64-w64-mingw32-gcc
WINDOWS_CFLAGS  := -c -Wall -Wextra -I $(LINUX_INCLUDE_PATH)

ifdef output

ifeq ($(output), linux32)
$(TARGET_LINUX32): $(OBJ_FILES) | $(BIN_PATH)
	$(CC) -m32 $^ -o $@ $(LD_LIBS_LINUX)
	$(RMDIR) $(OBJ_PATH)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(LINUX32_CFLAGS) $< -o $@
endif

ifeq ($(output), linux64)
$(TARGET_LINUX64): $(OBJ_FILES) | $(BIN_PATH)
	$(CC) $^ -o $@ $(LD_LIBS_LINUX)
	$(RMDIR) $(OBJ_PATH)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(LINUX64_CFLAGS) $< -o $@
endif

ifeq ($(output), windows32)
$(TARGET_WINDOWS32): $(OBJ_FILES) | $(BIN_PATH)
	$(WIN32_CC) $^ -o $@ $(LD_LIBS_WINDOWS)
	$(RMDIR) $(OBJ_PATH)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(WIN32_CC) $(WINDOWS_CFLAGS) $< -o $@
endif

ifeq ($(output), windows64)
$(TARGET_WINDOWS64): $(OBJ_FILES) | $(BIN_PATH)
	$(WIN64_CC) $^ -o $@ $(LD_LIBS_WINDOWS)
	$(RMDIR) $(OBJ_PATH)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(WIN64_CC) $(WINDOWS_CFLAGS) $< -o $@
endif

else

.PHONY: all run clean

$(TARGET_LINUX64) $(TARGET_LINUX32): $(OBJ_FILES) | $(BIN_PATH)
	$(CC) $^ -o $@ $(LD_LIBS_LINUX)
	$(RMDIR) $(OBJ_PATH)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	$(CC) $(LINUX64_CFLAGS) $< -o $@

endif

$(BIN_PATH) $(OBJ_PATH) $(DATABASE_PATH):
	$(MKDIR) $@

all: $(TARGET_LINUX64)

run: $(TARGET_LINUX64)
	$<

clean:
	$(RMDIR) $(BIN_PATH) $(OBJ_PATH) $(DATABASE_FILE_PATH)

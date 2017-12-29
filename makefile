CC=gcc
EXE=run_solver
SRC_DIR=src
SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ_DIR=intermediate
OBJ=$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CPPFLAGS += -Iinclude
CFLAGS +=
LDLIBS += -lm
.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
		$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
		$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
		$(RM) $(OBJ)
		rm -f $(EXE)

clean_all:
		$(RM) $(OBJ)
		rm -f $(EXE)		
		rm -R results

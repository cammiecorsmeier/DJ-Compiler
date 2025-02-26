CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =
OBJ = dj.tab.o symtbl.o
SRC = ast.c codegen.c typecheck.c
EXEC = djc

all: $(EXEC)

$(EXEC): $(SRC) $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) $(OBJ)

clean:
	rm -f $(EXEC) *.o *.dism

BASE_NAME = egg
EXE_NAME = $(BASE_NAME)
LIB_NAME_BASE = $(BASE_NAME)
LIB_NAME = lib$(LIB_NAME_BASE).a
SRC_FILES = main.c parse.c
OBJ_FILES = $(SRC_FILES:.c=.o)
LIB_SRC_FILES = seconds.c timer.c
LIB_OBJ_FILES = $(LIB_SRC_FILES:.c=.o)
MAN_PAGE = $(BASE_NAME).1
MAN_PAGE_ZIP = $(MAN_PAGE).gz
CC = gcc
CFLAGS = -Wall -Wextra -g -O2
PREFIX=/usr/local

all: $(EXE_NAME)

$(EXE_NAME): $(OBJ_FILES) $(LIB_NAME)
	$(CC) $(CFLAGS) -L. -o $(EXE_NAME) $(OBJ_FILES) -l$(LIB_NAME_BASE)

$(OBJ_FILES): $(SRC_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES) 

$(LIB_NAME): $(LIB_OBJ_FILES)
	ar rcs $(LIB_NAME) $(LIB_OBJ_FILES)

$(LIB_OBJ_FILES): $(LIB_SRC_FILES)
	$(CC) $(CFLAGS) -c $(LIB_SRC_FILES)

test: test_parse
	./test_parse

test_parse: test_parse.o parse.o
	$(CC) $(CFLAGS) -o test_parse test_parse.o parse.o

test_parse.o: test_parse.c
	$(CC) $(CFLAGS) -c test_parse.c

$(MAN_PAGE_ZIP):
	gzip < $(MAN_PAGE) > $(MAN_PAGE_ZIP)

install: $(EXE_NAME) $(MAN_PAGE_ZIP)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp $(EXE_NAME) $(DESTDIR)$(PREFIX)/bin/$<
	cp $(MAN_PAGE) $(DESTDIR)$(PREFIX)/share/man/man1/$(MAN_PAGE_ZIP)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE_NAME)
	rm -f $(DESTDIR)$(PREFIX)/share/man/man1/$(MAN_PAGE_ZIP)

clean:
	rm -f *.o
	rm -f $(EXE_NAME)
	rm -f test_parse
	rm -f $(LIB_NAME)
	rm -f $(MAN_PAGE_ZIP)

.PHONY: all clean test install uninstall

OBJ_DIR = ./obj
LIB_DIR = ./lib
HEADER_DIR = ./include
LB_DIR = ./libbarcos
LB_OS_DIR = $(LB_DIR)
LG_DIR = ./libgame
LC_DIR = ./libcolors

CC = clang
CFLAGS := $(CFLAGS) -Wall -g -Wextra -pedantic -I $(HEADER_DIR) #--std=c99
LDFLAGS := $(LDFLAGS)
CMP = $(CC) $(CFLAGS)
COMPILE = $(CMP) -c

ifeq ($(OS),Windows_NT)
	LB_OS_DIR := $(LB_OS_DIR)/win
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		LB_OS_DIR := $(LB_OS_DIR)/linux
    endif
    ifeq ($(UNAME_S),Darwin)
		LB_OS_DIR := $(LB_OS_DIR)/mac
    endif
endif

.PHONY: all
all: objdir joc barcos
	$(CMP) $(OBJ_DIR)/juego.o -L $(LIB_DIR) -lgame -lBarcos2017 -lcolors -o game

joc: juego.c game
	$(COMPILE) juego.c -o $(OBJ_DIR)/juego.o

.PHONY: barcos
barcos: headerdir libdir $(LB_DIR)/libBarcos2017.h $(LB_OS_DIR)/libBarcos2017.a
	@\cp -rf $(LB_DIR)/libBarcos2017.h $(HEADER_DIR)/
	@\cp -rf $(LB_OS_DIR)/libBarcos2017.a $(LIB_DIR)/

game: proc tipus libdir $(LG_DIR)/libgame.c $(LG_DIR)/libgame.h
	$(COMPILE) $(LG_DIR)/libgame.c -o $(OBJ_DIR)/libgame.o
	@\cp -rf $(LG_DIR)/libgame.h $(HEADER_DIR)/
	@ar crD $(LIB_DIR)/libgame.a $(OBJ_DIR)/libgame.o $(OBJ_DIR)/proc.o

proc: objdir headerdir barcos colors $(LG_DIR)/proc.c $(LG_DIR)/proc.h
	@\cp -rf $(LG_DIR)/proc.h $(HEADER_DIR)/
	$(COMPILE) $(LG_DIR)/proc.c -o $(OBJ_DIR)/proc.o

tipus: headerdir $(LG_DIR)/tipus.h
	@\cp -rf $(LG_DIR)/tipus.h $(HEADER_DIR)/

colors: objdir headerdir libdir $(LC_DIR)/libcolors.c $(LC_DIR)/libcolors.h
	$(COMPILE) $(LC_DIR)/libcolors.c -o $(OBJ_DIR)/libcolors.o
	@\cp -rf $(LC_DIR)/libcolors.h $(HEADER_DIR)/
	@ar crD $(LIB_DIR)/libcolors.a $(OBJ_DIR)/libcolors.o

.PHONY: run
run: all
	./game

.PHONY: objdir
objdir:
	@if [ ! -d "$(OBJ_DIR)" ]; then mkdir $(OBJ_DIR); fi

.PHONY: libdir
libdir:
	@if [ ! -d "$(LIB_DIR)" ]; then mkdir $(LIB_DIR); fi

.PHONY: headerdir
headerdir:
	@if [ ! -d "$(HEADER_DIR)" ]; then mkdir $(HEADER_DIR); fi

.PHONY: dist
dist: all clean
	zip -r P2.zip . -x .editorconfig  -x .git\*

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	rm -rf $(HEADER_DIR)
	rm -f game records.txt juego.dat P2.zip

CC=clang
CFLAGS+=-Wall -g -Wextra -pedantic -I $(HEADER_DIR) #--std=c99
LDFLAGS+=
CMP=@$(CC) $(CFLAGS)
COMPILE=$(CMP) -c
OBJ_DIR=./obj
LIB_DIR=./lib
HEADER_DIR=./include
LG_DIR=./libgame

.PHONY: all
all: objdir joc
	$(CMP) $(OBJ_DIR)/*.o -L $(LIB_DIR) -lgame -lBarcos2017 -o game

joc: juego.c game
	$(COMPILE) juego.c -o $(OBJ_DIR)/juego.o

game: objdir proc tipus $(LG_DIR)/libgame.c $(LG_DIR)/libgame.h
	$(COMPILE) $(LG_DIR)/libgame.c -o $(OBJ_DIR)/libgame.o
	@\cp -rf $(LG_DIR)/libgame.h $(HEADER_DIR)/
	@ar crD $(LIB_DIR)/libgame.a $(OBJ_DIR)/libgame.o

proc: objdir $(LG_DIR)/proc.c $(LG_DIR)/proc.h
	@\cp -rf $(LG_DIR)/proc.h $(HEADER_DIR)/
	$(COMPILE) $(LG_DIR)/proc.c -o $(OBJ_DIR)/proc.o
	
tipus: $(LG_DIR)/tipus.h
	@\cp -rf $(LG_DIR)/tipus.h $(HEADER_DIR)/

.PHONY: objdir
objdir:
	@if [ ! -d "$(OBJ_DIR)" ]; then mkdir $(OBJ_DIR); fi

.PHONY: dist
dist: all clean
	zip -j P2.zip juego.c $(LG_DIR)/libgame.c $(LG_DIR)/libgame.h $(LG_DIR)/proc.c $(LG_DIR)/proc.h $(LG_DIR)/tipus.h

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_DIR)/libgame.a
	rm -f game
	rm -f records.txt
	rm -f P2.zip
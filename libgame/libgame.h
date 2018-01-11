#ifndef __LIBGAME_H__
#define __LIBGAME_H__

#include "tipus.h"
#include <stdbool.h>


extern void clear();
extern void flush_stdin();
extern void menu (bool complet);
extern void carregarPartida(partida_t *partida);
extern void novaPartida(bool *menu_extens, partida_t *partida);
extern void mostrarPodium();
extern void jugarPartida(partida_t *partida);
extern void guardarPartida(partida_t *partida);
extern void sortir(bool *s);

#endif

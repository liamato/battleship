/*----------------------------------------------------------------
 |  Autor1: nom.cognom@estudiants.urv.cat
 |  Autor2: nom.cognom@estudiants.urv.cat
 |  Data: Octubre 2017                 Versio: 2.0
 |-----------------------------------------------------------------|
 |	Nom projecte: .....
 |	Nom: juego.c
 |  Descripcio del Programa:   Programa principal.
 | ----------------------------------------------------------------*/

/* Incloure llibreries */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "libgame.h"

/* Programa principal */
int main ()
{
    /* Declaracio de les variables del programa */
    partida_t partida;
    bool fi = false, menu_extens = false;
    char opcio;

    /* Inicialitzacions generals */
    srand((int)time(NULL));

    while (!fi) {
        clear();
        menu(menu_extens);
        scanf("%c", &opcio);
        flush_stdin();
        switch (opcio) {
            case 'a': novaPartida(&menu_extens, &partida); break;
            case 'b': carregarPartida(&partida); break;
            case 'e': mostrarPodium(); break;
            case 'c': jugarPartida(&partida); break;
            case 'd': guardarPartida(&partida); break;
            case 'f': sortir(&fi); break;
            default: printf("Opcio incorrecte.\n"); break;
        }
    }

    return 0;
}

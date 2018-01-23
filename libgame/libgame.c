#include <string.h>
#include <unistd.h>
#include "libgame.h"
#include "proc.h"

void clear() {system(NETEJA_PANTALLA);}

void flush_stdin(){P_netejar_stdio();}

void menu (bool complet) {
    printf ("Tria opcio: \n") ;
    printf ("a. Iniciar partida nova\n") ;
    printf ("b. Carregar una partida\n") ;
    if (complet)
    {
        printf ("c. Jugar partida\n") ;
        printf ("d. Emmagatzemar la partida\n") ;
    }
    printf ("e. Veure podium\n") ;
    printf ("f. Sortir del joc\n") ;
}

void initTauler(tauler_t * tauler, bool aleatori) {
    P_inicializa_matriz(tauler->barcos, DIM_MAX, DIM_MAX, CASELLA_AIGUA);

    P_inicializa_matriz(tauler->disparos, DIM_MAX, DIM_MAX, CASELLA_BUIDA);
    //B_inicializa_barcos_fijo(tauler->barcos, tauler->dim);
    if (aleatori) {
        B_inicializa_barcos(tauler->barcos, tauler->dim);
    } else {
        P_coloca_barcos(tauler->barcos, tauler->dim);
    }
}

void initJugador(jugador_t *jugador, nom_t name, unsigned int dim, bool aleatori) {
    jugador->tauler.dim = dim;
    jugador->tauler.noEnfonsats = 10;
    initTauler(&jugador->tauler, aleatori);
    strcpy(jugador->record.nom, name);
    jugador->record.data = P_data_avui();
    jugador->record.punts = 0;
    jugador->efectivitatEnfonsats = 0;
    jugador->efectivitatDisparos = 0;
}

void initPartida(partida_t *partida, gamemode_t gamemode, nom_t name, unsigned int dim, bool aleatori) {
    initJugador(&partida->jugador1, name, dim, aleatori);
    initJugador(&partida->jugador2, "CPU", dim, true);

    partida->mode = gamemode;
    partida->turno = 0;
}

void novaPartida(bool *menu_extens, partida_t *partida) {
    unsigned int dim;
    int mode;
    nom_t nom = "CPU";
    char generacioBarcos = 'a';

    *menu_extens = true;
    clear();

    printf("Mode:\n\t0:Automatic\n\t1:Un jugador\n\t2:Contra la maquina\n");
    scanf("%i", &mode);
    while (mode < 0 || mode > 2) {
        printf("Mode invalid\n");
        printf("Mode:\n\t0:Automatic\n\t1:Un jugador\n\t2:Contra la maquina\n");
        scanf("%i", &mode);
    }
    clear();
    flush_stdin();

    if ((gamemode_t)mode != MAQUINA) {
        printf("Nom: ");
        P_leer_cadena(nom, NOM_MAX, '\n');
        clear();
    }

    printf("Mida del tauler (8, 9 o 10): ");
    scanf("%u", &dim);
    while (dim != 8 && dim != 9 && dim != 10) {
        printf("Dimensio incorrecte (8, 9 o 10)\n");
        printf("Mida del tauler: ");
        scanf("%u", &dim);
    }
    clear();
    flush_stdin();

    if ((gamemode_t)mode == JUGADOR_MAQUINA) {
        printf("Vols posar els vaixells manualment o automaticament (m/a): ");
        scanf("%c", &generacioBarcos);
        flush_stdin();
        while (generacioBarcos != 'm' && generacioBarcos != 'a') {
            printf("Resposta incorrecte\n");
            printf("Vols posar els vaixells manualment o automaticament (m/a): ");
            scanf("%c", &generacioBarcos);
            flush_stdin();
        }
    }

    initPartida(partida, (gamemode_t)mode, nom, dim, generacioBarcos == 'a');
}

void carregarPartida(bool *menu_extens, partida_t *partida) {
    int unsigned dimen;
    int m_joc, torn;
    jugador_t nom_jugadors[2];
    bool partidaCarregada;

    partidaCarregada = P_recupera_partida(FITXER_JOC, &dimen, &m_joc, &torn, nom_jugadors);

    if (partidaCarregada) {
        compose_partida(partida, dimen, m_joc, torn, nom_jugadors);
        *menu_extens = true;
        printf ("La partida s'ha carregat correctament.\n");
    } else {
        printf ("No s'ha pogut carregar cap partida, no existeix cap partida guardada.\n");
    }
}

void mostrarPodium() {
    int num_r;
    record_t records_g[RECORDS_TOP_COUNT];
    num_r = P_recupera_records(FITXER_RECORDS, records_g, RECORDS_TOP_COUNT);
    P_muestra_records(records_g, num_r);
    P_pausa();
}

void procesaDisparo(jugador_t *jugador, tauler_t *tauler, int *x, int *y) {
    int res,  midaVaixell;

    res = B_dispara('a'+*x, 1+*y, tauler->barcos, &midaVaixell);
    switch (res) {
        case RES_ERROR: printf("Error, casella fora de rang\n"); break;
        case RES_REPETIT: printf("Dispar repetit\n"); break;
        case RES_AIGUA: jugador->tauler.disparos[*x][*y] = CASELLA_AIGUA; break;
        case RES_TOCAT:
            jugador->tauler.disparos[*x][*y] = CASELLA_VAIXELL;
            jugador->efectivitatDisparos += 4-res;
            fill_water_diagonals(jugador->tauler.disparos, jugador->tauler.dim, *x, *y);
            break;
        case RES_ENFONSAT:
            jugador->tauler.disparos[*x][*y] = CASELLA_VAIXELL;
            jugador->efectivitatEnfonsats += ((2 * (float)midaVaixell)-1)/(jugador->efectivitatDisparos?jugador->efectivitatDisparos:4-res);
            jugador->efectivitatDisparos = 0;
            P_procesa_hundido(*x, *y, jugador->tauler.disparos, jugador->tauler.dim);
            jugador->tauler.noEnfonsats--;
            break;
    }
}

void jugarPartida(bool *menu_extens, partida_t *partida) {
    int x,y;
    char f;
    jugador_t *j = NULL;

    clear();

    if (partida->mode == MAQUINA) {
        while (partida->jugador1.tauler.noEnfonsats != 0) {
            partida->turno++;
            //counter++;
            clear();
            P_decide_disparo(&x, &y, partida->jugador1.tauler.disparos, (int) partida->jugador1.tauler.dim);
            procesaDisparo(&partida->jugador1, &partida->jugador1.tauler, &x, &y);
            P_muestra_dos_matrices (partida->jugador1.tauler.barcos, partida->jugador1.tauler.disparos, (int)partida->jugador1.tauler.dim, (int) partida->jugador1.tauler.dim);
            printf("\n");

            usleep(150000);
            //  P_pausa();
        }
    } else if (partida->mode == JUGADOR) {
        while (partida->jugador1.tauler.noEnfonsats != 0) {
            P_muestra_una_matriz(partida->jugador1.tauler.disparos, (int)partida->jugador1.tauler.dim, (int)partida->jugador1.tauler.dim);
            //P_muestra_dos_matrices (partida->jugador1.tauler.barcos, partida->jugador1.tauler.disparos, (int)partida->jugador1.tauler.dim, (int) partida->jugador1.tauler.dim);
            printf("\n");

            if (!P_nova_jugada(&f, &y, partida->jugador1.tauler.dim)) break;

            P_coordenadas(f, &y, &x);

            partida->turno++;
            //P_decide_disparo(&x, &y, partida->jugador1.tauler.disparos, (int) partida->jugador1.tauler.dim);
            clear();
            procesaDisparo(&partida->jugador1, &partida->jugador1.tauler, &x, &y);
            printf("\n");
        }
    } else if (partida->mode == JUGADOR_MAQUINA) {
        while (partida->jugador1.tauler.noEnfonsats != 0 && partida->jugador2.tauler.noEnfonsats != 0) {

            //P_muestra_dos_matrices (partida->jugador2.tauler.barcos, partida->jugador2.tauler.disparos, (int)partida->jugador1.tauler.dim, (int) partida->jugador1.tauler.dim);
            P_muestra_dos_matrices(partida->jugador1.tauler.barcos, partida->jugador1.tauler.disparos, (int)partida->jugador1.tauler.dim, (int) partida->jugador1.tauler.dim);
            printf("\n");

            if (!P_nova_jugada(&f, &y, partida->jugador1.tauler.dim)) break;

            P_coordenadas(f, &y, &x);

            partida->turno++;
            clear();

            //P_decide_disparo(&x, &y, partida->jugador1.tauler.disparos, (int) partida->jugador1.tauler.dim);
            procesaDisparo(&partida->jugador1, &partida->jugador2.tauler, &x, &y);
            printf("\n");

            P_decide_disparo(&x, &y, partida->jugador2.tauler.disparos, (int) partida->jugador2.tauler.dim);
            procesaDisparo(&partida->jugador2, &partida->jugador1.tauler, &x, &y);
        }
    }

    if (partida->jugador2.tauler.noEnfonsats == 0) j = &partida->jugador2;
    if (partida->jugador1.tauler.noEnfonsats == 0) j = &partida->jugador1;

    if (j != NULL) {
        j->record.punts = 100 * ((float)j->tauler.dim/(float)partida->turno)*j->efectivitatEnfonsats;
        printf("Disparos: %i\n", partida->turno);
        printf("Puntuacio: %i\n", j->record.punts);

        P_guarda_record(FITXER_RECORDS, j->record);

        *menu_extens = false;

        P_pausa();
    }

}

void guardarPartida(partida_t *partida) {
    unsigned int dimen;
    int  m_joc, torn;
    jugador_t nom_jugadors[2];
    bool p_guardada;

    decompose_partida (*partida, &dimen, &m_joc, &torn, nom_jugadors);
    p_guardada = P_guarda_partida(FITXER_JOC, dimen, m_joc, torn, nom_jugadors);
    if (p_guardada) {
        printf ("Partida guardada correctament.\n");
    } else {
        printf ("La partida no s'ha pogut guardar correctament.\n");
    }
}

void sortir(bool *s) {
    *s = true;
}

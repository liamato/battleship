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

void initTauler(tauler_t * tauler) {
    P_inicializa_matriz(tauler->barcos, DIM_MAX, DIM_MAX, CASELLA_AIGUA);
    
    P_inicializa_matriz(tauler->disparos, DIM_MAX, DIM_MAX, CASELLA_BUIDA);
    B_inicializa_barcos_fijo(tauler->barcos, tauler->dim);
    //B_inicializa_barcos(tauler->barcos, tauler->dim);
}

void novaPartida(bool *menu_extens, partida_t *partida) {
    
    *menu_extens = true;

    clear();
    
    printf("Mode:\n\t0:Automatic\n\t1:Un jugador\n\t2:Contra la maquina\n");
    scanf("%i", &partida->mode);
    while (partida->mode < 0 || partida->mode > 0) {
        if (partida->mode == JUGADOR || partida->mode == JUGADOR_MAQUINA) {
            printf("Mode no disponible actualment\n");
        } else {    
            printf("Mode invalid\n");
        }
        printf("Mode:\n\t0:Automatic\n\t1:Un jugador\n\t2:Contra la maquina\n");
        scanf("%i", &partida->mode);
    }
    clear();
    flush_stdin();
    
    strcpy(partida->jugador1.record.nom, "CPU");
    strcpy(partida->jugador2.record.nom, "CPU");
    if (partida->mode != MAQUINA) {
        printf("Nom: ");
        P_leer_cadena(partida->jugador1.record.nom, NOM_MAX, '\n');
        clear();
    }
    //flush_stdin();
    
    printf("Mida del tauler (8, 9 o 10): ");
    scanf("%hu", &partida->jugador1.tauler.dim);
    while (partida->jugador1.tauler.dim != 8 && partida->jugador1.tauler.dim != 9 && partida->jugador1.tauler.dim != 10) {
        printf("Dimensio incorrecte (8, 9 o 10)\n");
        printf("Mida del tauler: ");
        scanf("%hu", &partida->jugador1.tauler.dim);
    }
    clear();
    flush_stdin();
    
    partida->jugador2.tauler.dim = partida->jugador1.tauler.dim;
    
    initTauler(&partida->jugador1.tauler);
    initTauler(&partida->jugador2.tauler);
    
    partida->jugador1.record.data = P_data_avui();
    partida->jugador2.record.data = P_data_avui();
    
}

void carregarPartida(partida_t *partida) {
    
}

void mostrarPodium() {
    int num_r;
    record_t records_g[10];
    num_r = P_recupera_records(FITXER_RECORDS, records_g, 10);
    P_muestra_records(records_g, num_r);
    P_pausa();
}

void jugarPartida(partida_t *partida) {
        int x,y,res, lim = 10, counter = 0, midaVaixell;
        float efectivitatEnfnsats = 0, efectivitatDisparos = 0;
    
    if (partida->mode == MAQUINA) {
        while (lim != 0) {
            counter++;
            clear();
            P_decide_disparo(&x, &y, partida->jugador1.tauler.disparos, (int) partida->jugador1.tauler.dim);
            res = B_dispara('a'+x, 1+y, partida->jugador1.tauler.barcos, &midaVaixell);
            switch (res) {
                case RES_ERROR: printf("Error, casella fora de rang\n"); break;
                case RES_REPETIT: printf("Dispar repetit\n"); break;
                case RES_AIGUA: partida->jugador1.tauler.disparos[x][y] = CASELLA_AIGUA; break;
                case RES_TOCAT: 
                    partida->jugador1.tauler.disparos[x][y] = CASELLA_VAIXELL; 
                    efectivitatDisparos += 4-res;
                    fill_water_diagonals(partida->jugador1.tauler.disparos, partida->jugador1.tauler.dim, x, y);
                    break;
                case RES_ENFONSAT: 
                    partida->jugador1.tauler.disparos[x][y] = CASELLA_VAIXELL;
                    efectivitatEnfnsats += ((2 * (float)midaVaixell)-1)/(efectivitatDisparos?efectivitatDisparos:4-res);
                    efectivitatDisparos = 0;
                    P_procesa_hundido(x, y, partida->jugador1.tauler.disparos, partida->jugador1.tauler.dim);
                    lim--;
                    break;
            }
            
            P_muestra_dos_matrices (partida->jugador1.tauler.barcos, partida->jugador1.tauler.disparos, (int)partida->jugador1.tauler.dim, (int) partida->jugador1.tauler.dim);
            printf("\n");
            
            usleep(150000);
            //  P_pausa();
        }
        partida->jugador1.record.punts = 100 * ((float)partida->jugador1.tauler.dim/(float)counter)*efectivitatEnfnsats;
        printf("Disparos: %i\n", counter);
        printf("Puntuacio: %i\n", partida->jugador1.record.punts);
        
        P_guarda_record(FITXER_RECORDS, partida->jugador1.record);
        
    }
    //P_muestra_dos_matrices (partida->tablero_barcos, partida->tablero_disparos, (int)partida->dim, (int) partida->dim);
    P_pausa();
    
}

void guardarPartida(partida_t *partida) {
    
}

void sortir(bool *s) {
    *s = true;
}

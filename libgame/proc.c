/*----------------------------------------------------------------
 |  Autor1: nom.cognom@estudiants.urv.cat
 |  Autor2: nom.cognom@estudiants.urv.cat
 |  Data: Octubre 2017                 Versio: 3.0
 |-----------------------------------------------------------------|
 |	Nom projecte: ...
 |	Nom: proc.h
 |  Descripcio del Programa:   Implementacio dels procediments.
 | ----------------------------------------------------------------*/
#include "proc.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include "libcolors.h"

/** Procediments auxiliars */
/*
 accio P_netejar_stdio();

 "Netejar" l'entrada estandard (buidar el buffer de teclat)
*/
void P_netejar_stdio(void)
{
    int ch;
    while (((ch = getchar()) != EOF) && (ch != '\n'));
}

/*
 funcio P_data_avui() retorna data_tipus;

 Retorna la data actual.
 */
data_t P_data_avui(void)
{
    data_t avui;
    struct tm *data;
    time_t tiempo;

    tiempo = time(NULL);
    data = localtime(&tiempo);
    avui.dia = data->tm_mday;
    avui.mes = data->tm_mon+1;
    avui.any = data->tm_year+1900;

    return avui;
}

/*
 accio P_pausa();

 Pausa el programa fins que se pulse return/enter.
 */
void P_pausa(void)
{
    printf("\nPulsa return/enter per a continuar! ");
    P_netejar_stdio();
}

/** PROCEDIMENTS BASICS */
/*
 accio P_decide_disparo (var fila:enter, var col:enter, tablero_disparos: taula de caracters, dim:enter);
 */
extern void P_decide_disparo (int *f, int *c, char tablero_disparos[][COL_MAX], unsigned int dim)
{
    int x = 0, y = 0;

    *f = -1;
    *c = -1;

    while (x < (int)dim && *f == -1 && *c == -1) {
        y = 0;
        while (y < (int)dim && *f == -1 && *c == -1) {
            if (tablero_disparos[x][y] == CASELLA_VAIXELL) {
                if (x != 0 && tablero_disparos[x-1][y] == CASELLA_BUIDA) {
                    *f = x-1;
                    *c = y;
                } else if (y != 0 && tablero_disparos[x][y-1] == CASELLA_BUIDA) {
                    *f = x;
                    *c = y-1;
                } else if (x < (int)dim-1 && tablero_disparos[x+1][y] == CASELLA_BUIDA) {
                    *f = x+1;
                    *c = y;
                } else if (y < (int)dim-1 && tablero_disparos[x][y+1] == CASELLA_BUIDA) {
                    *f = x;
                    *c = y+1;
                }
            }
            y++;
        }
        x++;
    }

    while (*f == -1 && *c == -1) {
        x = P_aleatorio(0, (int)dim);
        y = P_aleatorio(0, (int)dim);
        if (tablero_disparos[x][y] == CASELLA_BUIDA) {
            *f = x;
            *c = y;
        }
    }
}

/*
 accio P_procesa_hundido (fila:enter, col:enter, var tablero_disparos: taula de caracters, dim:enter);
 */
extern void P_procesa_hundido (int f, int c, char tablero_disparos[][COL_MAX], unsigned int dim)
{
    char pos = tablero_disparos[f][c];
    fill_water(tablero_disparos, dim, f, c);
    find_first_position(tablero_disparos, dim, &f, &c);

    while (tablero_disparos[f][c] == pos) {
        fill_water(tablero_disparos, dim, f, c);
        if (es_vertical(tablero_disparos, dim, f, c)) {c++;}
        else {f++;}
    }
}

/*
funcio P_aleatorio ( min: enter, max: enter) retorna enter;
 */
int P_aleatorio (int min, int max) {
        return min + rand() % (max - min + 1);
}

/*
accio P_coordenadas ( fila: caracter, var col: enter, var fila_entero: enter );
*/
void P_coordenadas (char fila, int *col, int *fila_entero)
{
    *col = *col -1;
    *fila_entero = toupper(fila) - 'A';
}

/*
 funcio P_leer_cadena ( var cadena: taula de caracters, dim: enter, centinela: caracter) retorna enter;
 */
int P_leer_cadena (char cadena[], unsigned int dim, char centinela)
{
    unsigned int x = 0;
    scanf("%c", &cadena[x]);
    while (cadena[x] != centinela && x < dim) {
        scanf("%c", &cadena[++x]);
    }
    cadena[x] = 0;
    return (int)x;
}

/*
 accio P_inicializa_matriz ( var matriz: taula de caracters, nfilas: enter, int ncols, car: caracter) es;
 */
void P_inicializa_matriz (char matriz[][COL_MAX], int nfilas, int ncols, char car)
{
    int f, c;

    for (f=0 ; f<nfilas ; f++)
    {
        for (c=0; c<ncols ; c++)
        {
            matriz [f][c] = car ;
        }
    }
}

void colorPrint(char c) {
    if (c == CASELLA_BUIDA) {
        printcf(MAGENTA, " %c", c);
    }
    if (c == CASELLA_AIGUA) {
        printcf(BLUE, " %c", c);
    }
    if (c == CASELLA_VAIXELL) {
        printcf(GREEN, " %c", c);
    }
    if (c == CASELLA_VAIXELL_TOCAT) {
        printcf(RED, " %c", c);
    }
    if (c == CASELLA_AIGUA_TOCADA) {
        printcf(YELLOW, " %c", c);
    }
}

/*
 accio P_muestra_una_matriz ( matriz: taula de caracters, nfilas: enter, ncols: enter) es;
 */
void P_muestra_una_matriz (char matriz[][COL_MAX], int nfilas, int ncols)
{
     int f = 0, c;

     printf("  ");
     while (f < ncols) {printf(" %i", 1+f++);}
     printf("\n");

     for (f=0 ; f<nfilas ; f++)
     {
         printf(" %c", 'A' + f);
         for (c=0; c<ncols ; c++)
         {
             colorPrint(matriz[f][c]);
         }
         printf("\n");
     }
 }

/*
 accio P_muestra_dos_matrices ( matriz1: taula de caracters,matriz2: taula de caracters, nfilas: enter, ncols: enter) es;
 */
void P_muestra_dos_matrices (char matriz1[][COL_MAX], char matriz2[][COL_MAX], int nfilas, int ncols)
{
    int x = 0, y = 0;

    printf("  ");
    while (y < ncols) {printf(" %i", 1+y++);}
    printf("\t  ");
    y = 0;
    while (y < ncols) {printf(" %i", 1+y++);}

    printf("\n");

    while (x < nfilas) {
        y = 0;
        printf(" %c", 'A' + x);
        while (y < ncols) {colorPrint(matriz1[x][y++]);}
        printf("\t");

        y = 0;
        printf(" %c", 'A' + x);
        while (y < ncols) {colorPrint(matriz2[x][y++]);}
        printf("\n");
        x++;
    }
}

/*
 funcio P_guarda_record (fitxer_record:taula de caracters, record:record_t) retorna boolea;
 */
extern bool P_guarda_record (char fitxer_record[], record_t record) {
    FILE *file ;
    record_t records[RECORDS_TOP_COUNT+1];
    int count = 0,x=0;

    count = P_recupera_records(fitxer_record, records, RECORDS_TOP_COUNT);

    records[count++] = record;

    sort_records(records, count);

    file = fopen (fitxer_record,"w");

    if (file == NULL) {
        printf ("No existeix el fitxer\n") ;
        return false;
    }

    while (x < (count < RECORDS_TOP_COUNT ? count : RECORDS_TOP_COUNT)) {
        fprintf(file, RECORD_FORMAT, records[x].data.dia, records[x].data.mes, records[x].data.any, records[x].nom, records[x].punts);
        x++;
    }

    fclose(file);

    return true;
}

/*
 funcio P_recupera_records (fitxer_record:taula de caracters, var records:taula de record_tipus, dim:enter ) retorna enter;
 */
extern int P_recupera_records (char fitxer_record[], record_t records[], unsigned int dim) {
    FILE *file ;
    int x = 0;
    unsigned int c = 0;

    file = fopen(fitxer_record, "r");

    if (file == NULL) {
        return 0;
    }

    while (!feof(file) && x < (int)dim) {
        fscanf(file, "%d-%d-%d\t", &records[x].data.dia, &records[x].data.mes, &records[x].data.any);

        c = 0;
        fscanf(file, "%c", &records[x].nom[c]);
        while (records[x].nom[c] != '\t' && x < NOM_MAX) {
            fscanf(file, "%c", &records[x].nom[++c]);
        }
        records[x].nom[c] = 0;

        fscanf(file, "%d\n", &records[x].punts);
        x++;
    }

    fclose(file);

    return x;
}

void P_muestra_records (record_t records[], int dim) {
    int i, j;
    i = 0;
    printf("Data:\t\tNom:\t\t\tPunts:\n");
    while (i<dim)
    {
        printf ("%i", records[i].data.dia) ;
        printf ("-") ;
        printf ("%i", records[i].data.mes) ;
        printf ("-") ;
        printf ("%i", records[i].data.any) ;
        printf ("\t") ;
        j = 0;
        while ((j<NOM_MAX) && (!(records[i].nom[j]=='\0')))
        {
          printf ("%c", records[i].nom[j]) ;
          j = j + 1 ;
        }
        if (j<9) printf("\t");
        if (j<17) printf("\t");
        printf ("\t") ;
        printf ("%i\n", records[i].punts) ;
        i = i + 1 ;
    }
}

int compare_record(const void *record1, const void *record2) {
    return (((record_t *)record1)->punts > ((record_t *)record2)->punts) ? -1 : (((record_t *)record1)->punts < ((record_t *)record2)->punts);
}

void sort_records(record_t *records, unsigned int dim) {
    qsort(records, dim, sizeof(record_t), &compare_record);
}

/** -------------------------------------------------------
 PROCEDIMENTS ADDICIONALS A DESENVOLUPAR
 ------------------------------------------------------- */

 bool coor_correctes (char fila, int col, int dim)
 {
     bool crrecte;
     int fila_ent;

     P_coordenadas(fila, &col, &fila_ent);

     if (fila_ent < 0 || fila_ent > dim-1 || col < 0 || col > dim-1) {
         crrecte = false;
     } else {
         crrecte = true;
     }

     return crrecte;
 }

/*
funcio P_nuevo_disparo (var f:caracter, var c:enter, dim:enter) retorna boolea;
 */
bool P_nova_jugada (char *f, int *c, unsigned int dim)
{
    bool correctes, seguir;
    int num;

    printf ("Vols fer una pausa?\n");
    printf (" 0- Pausa\n 1- Continuar jugant\n");
    scanf ("%i", &num);
    P_netejar_stdio();

    while (num != 0 && num != 1) {
        printf ("\nOpcio incorrecta, torna a triar: ");
        scanf ("%i", &num);
        P_netejar_stdio();
    }

    if (num == 0) {
        seguir = false;
    }

    if (num == 1) {
        seguir = true;
        printf("Insereix coordenades pel seguent tret: ");
        scanf("%c", f);
        scanf("%i", c);

        P_netejar_stdio();

        correctes = coor_correctes(*f,*c, dim);
        while (!correctes) {
            printf("Coordenades fora del taulell, insereix noves coordenades pel tret (fila columna): ");
            scanf("%c", f);
            scanf("%i", c);
            P_netejar_stdio();
            correctes = coor_correctes(*f, *c, dim);
        }
    }

    return seguir;
}

/*
 funcio P_guarda_partida (nom_fitxer:taula de caracters, dim:enter, modo_juego:enter, turno:enter, jugadores:taula de jugador_t) retorna boolea;
 */
bool P_guarda_partida (char fitxer_record[], unsigned int dim, int modo_juego, int turno, jugador_t jugadores[]) {
    partida_t partida;
    FILE *partidaGuardada;
    bool correctament;

    correctament = true ;
    partidaGuardada = fopen(fitxer_record, "wb") ;

    if (partidaGuardada != NULL) {
        printf ("S'esta guardant la partida...\n") ;
        compose_partida(&partida, dim, modo_juego, turno, jugadores);

        fwrite(&partida, sizeof(partida_t), 1, partidaGuardada);

        fclose(partidaGuardada);
    } else {
        correctament = false ;
    }

    return correctament;
}

/*
 funcio P_recupera_partida (nom_fitxer:taula de caracters, var dim:enter, var modo_juego: enter,
 var turno:enter, var jugadores:taula de jugador_t) retorna boolea;
 */
bool P_recupera_partida (char fitxer_record[], unsigned int *dim, int *modo_juego, int *turno, jugador_t jugadores[]) {
    FILE *f_pguardada;
    partida_t partida;
    bool correcte;

    f_pguardada = fopen(fitxer_record, "rb");
    correcte = true;
    if (f_pguardada!=NULL) {
        fread(&partida, sizeof(partida_t), 1, f_pguardada);

        decompose_partida (partida, dim, modo_juego, turno, jugadores);
        fclose(f_pguardada);
    } else {
        correcte = false;
    }

    return correcte;
}

void askShip(int size, char board[][DIM_MAX], int dim) {
    char xc, pos;
    int x,y;
    bool vertical = true;

    system(NETEJA_PANTALLA);

    P_muestra_una_matriz(board, dim, dim);

    printf("Barco de %i espais:\n", size);

    if (size != 1) {
        printf("En vertical o horitzontal (v/h): ");
        scanf("%c", &pos);
        P_netejar_stdio();
        while (pos != 'v' && pos != 'h') {
            printf("Opcio incorrecte, torna a escriureu\n");
            printf("En vertical o horitzontal (v/h): ");
            scanf("%c", &pos);
            P_netejar_stdio();
        }

        vertical = (pos == 'v');
    }

    printf("Cordenades (fila columna): ");
    scanf("%c", &xc);
    scanf("%i", &y);
    P_netejar_stdio();

    P_coordenadas(xc, &y, &x);
    while (!checkSpace(x, y, size, vertical, CASELLA_AIGUA, board, dim)) {
        printf("Cordenades ocupades, incorrectes o masa aprop de altrs baixells, torna a escriureu\n");
        printf("Cordenades (fila columna): ");
        scanf("%c", &xc);
        scanf("%i", &y);
        P_netejar_stdio();

        P_coordenadas(xc, &y, &x);
    }
    fillSpace(x, y, size, vertical, board, CASELLA_VAIXELL);
}

/*
 accio P_coloca_barcos (var tablero_barcos:taula de caracters, dim: enter) es;
 */
void P_coloca_barcos (char tablero_barcos[][DIM_MAX], unsigned int dim) {
    int count4 = 1,
        count3 = 2,
        count2 = 3,
        count1 = 4;

    P_inicializa_matriz(tablero_barcos, DIM_MAX, DIM_MAX, CASELLA_AIGUA);

    // barco de 4 x1
    while (count4 > 0) {
        askShip(4, tablero_barcos, (int)dim);
        count4--;
    }

    // barco de 3 x2
    while (count3 > 0) {
        askShip(3, tablero_barcos, (int)dim);
        count3--;
    }

    // barco de 2 x3
    while (count2 > 0) {
        askShip(2, tablero_barcos, (int)dim);
        count2--;
    }

    // barco de 1 x4
    while (count1 > 0) {
        askShip(1, tablero_barcos, (int)dim);
        count1--;
    }
}

bool es_vertical(char taula[][DIM_MAX], unsigned int dim, int x, int y) {
    return ((x != 0 && taula[x-1][y] == taula[x][y]) || (x < (int)dim-1 && taula[x+1][y] == taula[x][y]));
}

void find_first_position(char taula[][DIM_MAX], unsigned int dim, int *x, int *y) {
    char c = taula[*x][*y];
    while (c == taula[*x][*y]) {
        if (es_vertical(taula, dim, *x, *y)){
            if (*x != 0){(*x)--;}
            else {break;}
        } else {
            if (*y != 0){(*y)--;}
            else {break;}
        }
    }
}

void fill_water_diagonals(char taula[][DIM_MAX], unsigned int dim, int x, int y) {
    if (x != 0 && y != 0) {
        taula[x-1][y-1] = CASELLA_AIGUA;
    }
    if (x != 0 && y < (int)dim-1) {
        taula[x-1][y+1] = CASELLA_AIGUA;
    }
    if (x < (int)dim-1 && y < (int)dim-1) {
        taula[x+1][y+1] = CASELLA_AIGUA;
    }
    if (x < (int)dim-1 && y != 0) {
        taula[x+1][y-1] = CASELLA_AIGUA;
    }
}

void fill_water(char taula[][DIM_MAX], unsigned int dim, int x, int y) {
    fill_water_diagonals(taula, dim, x, y);

    if (x != 0 && taula[x-1][y] != CASELLA_VAIXELL) {taula[x-1][y] = CASELLA_AIGUA;}
    if (y != 0 && taula[x][y-1] != CASELLA_VAIXELL) {taula[x][y-1] = CASELLA_AIGUA;}
    if (x < (int)dim-1 && taula[x+1][y] != CASELLA_VAIXELL) {taula[x+1][y] = CASELLA_AIGUA;}
    if (y < (int)dim-1 && taula[x][y+1] != CASELLA_VAIXELL) {taula[x][y+1] = CASELLA_AIGUA;}
}

/**
 * desmonta un partida_t en els seus components
 */
void decompose_partida(partida_t partida, unsigned int *dim, int *modo_juego, int *turno, jugador_t *jugadores) {
    *dim = partida.jugador1.tauler.dim;
    *modo_juego = (int)partida.mode;
    *turno = partida.turno;
    jugadores[0] = partida.jugador1;
    jugadores[1] = partida.jugador2;
}

/**
 * remonta un partida_t a partir dels seus components
 */
void compose_partida(partida_t * partida, unsigned int dim, int modo_juego, int turno, jugador_t *jugadores) {
    partida->turno = turno;
    partida->mode = (gamemode_t)modo_juego;
    partida->jugador1 = jugadores[0];
    partida->jugador2 = jugadores[1];
    partida->jugador1.tauler.dim = dim;
    partida->jugador2.tauler.dim = dim;
}

bool checkCross(int x, int y, char value, char tauler[][DIM_MAX], int dim) {
    if (tauler[x][y] != value) return false;
    if (x>0 && y>0 && tauler[x-1][y-1] != value) return false;
    if (x<dim-1 && y>0 && tauler[x+1][y-1] != value) return false;
    if (x>0 && y<dim-1 && tauler[x-1][y+1] != value) return false;
    if (x<dim-1 && y<dim-1 && tauler[x+1][y+1] != value) return false;

    return true;
}

bool checkVertical(int x, int y, char value, char tauler[][DIM_MAX], int dim) {
    if (tauler[x][y] != value) return false;
    if (x>0 && tauler[x-1][y] != value) return false;
    if (x<dim-1 && tauler[x+1][y] != value) return false;

    return true;
}

bool checkHorizontal(int x, int y, char value, char tauler[][DIM_MAX], int dim) {
    if (tauler[x][y] != value) return false;
    if (y>0 && tauler[x][y-1] != value) return false;
    if (y<dim-1 && tauler[x][y+1] != value) return false;

    return true;
}


bool checkSpace(int x, int y, int size, bool vertical, char value, char tauler[][DIM_MAX], int dim) {
    int toX =  (vertical ? x+size-1 : x),
        toY = (!vertical ? y+size-1 : y);

    if (!coor_correctes(x+'a',y+1, dim)) return false;
    if (!coor_correctes(toX+'a', toY+1, dim)) return false;

    if ((vertical || size == 1) && x>0 && tauler[x-1][y] != value) return false;
    if ((!vertical || size == 1) && y>0 && tauler[x][y-1] != value) return false;
    if (!checkCross(x, y, value, tauler, dim)) return false;

    while (x != toX || y != toY) {
        if ( vertical && !checkVertical(x++, y, value, tauler, dim)) return false;
        if (!vertical && !checkHorizontal(x, y++, value, tauler, dim)) return false;
    }

    if (!checkCross(x, y, value, tauler, dim)) return false;

    return true;
}

void fillSpace(int x, int y, int size, bool vertical, char tauler[][DIM_MAX], char value) {
    int toX =  (vertical ? x+size : x),
        toY = (!vertical ? y+size : y);

    while (x != toX || y != toY) {
        tauler[x][y] = value;
        if (vertical) x++;
        else y++;
    }
}

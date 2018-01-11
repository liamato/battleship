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
int P_aleatorio (int min, int max)
{
    return min + rand() % (max - min);
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
    if (c == '?') {
        printcf(MAGENTA, " %c", c);
    }
    if (c == '.') {
        printcf(BLUE, " %c", c);
    }
    if (c == '@') {
        printcf(GREEN, " %c", c);
    }
    if (c == 'X') {
        printcf(RED, " %c", c);
    }
    if (c == '-') {
        printcf(CYAN, " %c", c);
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
extern bool P_guarda_record (char fitxer_record[], record_t record)
{
    FILE *f_records ;
    bool operacio ;

    f_records = fopen (fitxer_record,"ab");
    if (f_records != NULL)
    {
        operacio = true ;
        fwrite(&record,sizeof(record_t), 1, f_records) ;

        fclose(f_records);
    }
    else
    {
        operacio = false ;
        printf ("No existeix el fitxer\n") ;
    }
    return operacio;
}

/*
 funcio P_recupera_records (fitxer_record:taula de caracters, var records:taula de record_tipus, dim:enter ) retorna enter;
 */
extern int P_recupera_records (char fitxer_record[], record_t records[], unsigned int dim)
{
    FILE *f_records ;
    unsigned int n_records = 0;

    f_records = fopen(fitxer_record, "rb");

    if (f_records != NULL) {
        while (!(feof(f_records)) && n_records < dim)
        {
            fread(records+n_records, sizeof(record_t), 1, f_records);
            n_records = n_records + 1 ;
        }
        fclose(f_records);
    } else {
        printf ("No existeix el fitxer\n") ;
    }


    return (int)n_records-1;
}

void P_muestra_records (record_t records[], int dim)
{
    int i, j;
    i = 0;
    printf("Data:\t\tNom:\t\tPunts:\n");
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
        if (j<4) printf("\t");
        printf ("\t") ;
        printf ("%i\n", records[i].punts) ;
        i = i + 1 ;
    }
}

/** -------------------------------------------------------
 PROCEDIMENTS ADDICIONALS A DESENVOLUPAR
 ------------------------------------------------------- */

/*
funcio P_nuevo_disparo (var f:caracter, var c:enter, dim:enter) retorna boolea;
 */
bool P_nova_jugada (char *f, int *c, unsigned int dim)
{
    printf("Procediment P_nova_jugada\n");
    return true;
}

/*
 funcio P_guarda_partida (nom_fitxer:taula de caracters, dim:enter, modo_juego:enter, turno:enter, jugadores:taula de jugador_t) retorna boolea;
 */
bool P_guarda_partida (char fitxer_record[], unsigned int dim, int modo_juego, int turno, jugador_t jugadores[])
{
    printf("Procediment P_guardar_partida\n");
    return true;
}

/*
 funcio P_recupera_partida (nom_fitxer:taula de caracters, var dim:enter, var modo_juego: enter,
 var turno:enter, var jugadores:taula de jugador_t) retorna boolea;
 */
bool P_recupera_partida (char fitxer_record[], unsigned int *dim, int *modo_juego, int *turno, jugador_t jugadores[])

{
    printf("Procediment P_recupera_partida\n");
    return true;
}

/*
 funcio P_coloca_barcos (var tablero_barcos:taula de caracters, dim: enter) retorna boolea;
 */
bool P_coloca_barcos (char tablero_barcos[][DIM_MAX], unsigned int dim)
{
    printf("Procediment P_coloca_barcos\n");
    return  true;
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

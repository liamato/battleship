/*----------------------------------------------------------------
 |  Autor1: nom.cognom@estudiants.urv.cat
 |  Autor2: nom.cognom@estudiants.urv.cat
 |  Data: Octubre 2017                 Versio: 3.0
 |-----------------------------------------------------------------|
 |	Nom projecte: ...
 |	Nom: proc.h
 |  Descripcio del Programa:   Definicio dels procediments.
 | ----------------------------------------------------------------*/


#ifndef FUNCIONS_H_INCLUDED
#define FUNCIONS_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "libBarcos2017.h"
#include "tipus.h"

/* Cap dels procediments */
/** -------------------------------------------------------
 Procediments auxiliars
 ------------------------------------------------------- */
/*
 accio P_netejar_stdio();

 "Netejar" l'entrada estandard (buidar el buffer de teclat)
 */
void P_netejar_stdio(void);

/*
 funcio P_data_avui() retorna data_tipus;

 Retorna la data actual.
 */
data_t P_data_avui(void);

/*
 accio P_pausa();

 Pausa el programa fins que se pulse return/enter.
 */
void P_pausa(void);

/** -------------------------------------------------------
    PROCEDIMENTS BASICS A DESENVOLUPAR
    ------------------------------------------------------- */
/*
accio P_decide_disparo (var fila:enter, var col:enter, tablero_disparos: taula de caracters, dim:enter);
*/
extern void P_decide_disparo (int *f, int *c, char tablero_disparos[][COL_MAX], unsigned int dim);

/*
 accio P_procesa_hundido (fila:enter, col:enter, var tablero_disparos: taula de caracters, dim:enter);
 */
extern void P_procesa_hundido (int f, int c, char tablero_disparos[][COL_MAX], unsigned int dim);

/*
 funcio P_aleatorio ( min: enter, max: enter) retorna enter;
 */
extern int P_aleatorio (int min, int max);

/*
 accio P_coordenadas ( fila: caracter, var col: enter, var fila_entero: enter );
 */
void P_coordenadas (char fila, int *col, int *fila_entero);

/*
funcio P_leer_cadena ( var cadena: taula de caracters, dim: enter, centinela: caracter) retorna enter;
 */
int P_leer_cadena (char cadena[], unsigned int dim, char centinela);

/*
accio P_inicializa_matriz ( var matriz: taula de caracters, nfilas: enter, int ncols, car: caracter) es;
 */
void P_inicializa_matriz (char matriz[][COL_MAX], int nfilas, int ncols, char car);

/*
accio P_muestra_una_matriz ( matriz: taula de caracters, nfilas: enter, ncols: enter) es;
 */
void P_muestra_una_matriz (char matriz[][COL_MAX], int nfilas, int ncols);


/*
accio P_muestra_dos_matrices ( matriz1: taula de caracters,matriz2: taula de caracters, nfilas: enter, ncols: enter) es;
 */
void P_muestra_dos_matrices (char matriz1[][COL_MAX], char matriz2[][COL_MAX], int nfilas, int ncols);

/*
funcio P_guarda_record (fitxer_record:taula de caracters, record:record_t) retorna boolea;
*/
extern bool P_guarda_record (char fitxer_record[], record_t record);


/*
funcio P_recupera_records (fitxer_record:taula de caracters, var records:taula de record_tipus, dim:enter ) retorna enter;
*/
extern int P_recupera_records (char fitxer_record[], record_t records[], unsigned int dim);

extern void sort_records(record_t *records, unsigned int dim);

/** -------------------------------------------------------
    PROCEDIMENTS ADDICIONALS A DESENVOLUPAR
    ------------------------------------------------------- */
/*
 funcio P_nuevo_disparo (var f:caracter, var c:enter, dim:enter) retorna boolea;
 */
bool P_nova_jugada (char *f, int *c, unsigned int dim);

/*
 funcio P_guarda_partida (nom_fitxer:taula de caracters, dim:enter, modo_juego:enter, turno:enter, jugadores:taula de jugador_t) retorna boolea;
 */
bool P_guarda_partida (char fitxer_record[], unsigned int dim, int modo_juego, int turno, jugador_t jugadores[]);

/*
 funcio P_recupera_partida (nom_fitxer:taula de caracters, var dim:enter, var modo_juego: enter,
 var turno:enter, var jugadores:taula de jugador_t) retorna boolea;
 */
bool P_recupera_partida (char fitxer_record[], unsigned int *dim, int *modo_juego, int *turno, jugador_t jugadores[]);

/*
 funcio P_coloca_barcos (var tablero_barcos:taula de caracters, dim: enter) retorna boolea;
 */
bool P_coloca_barcos (char tablero_barcos[][DIM_MAX], unsigned int dim);

extern void P_muestra_records (record_t records[], int dim);
extern bool es_vertical(char taula[][DIM_MAX], unsigned int dim, int x, int y);
extern void find_first_position(char taula[][DIM_MAX], unsigned int dim, int *x, int *y);
extern void fill_water_diagonals(char taula[][DIM_MAX], unsigned int dim, int x, int y);
extern void fill_water(char taula[][DIM_MAX], unsigned int dim, int x, int y);

#endif /* FUNCIONS_H_INCLUDED */

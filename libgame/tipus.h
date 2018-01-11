/*----------------------------------------------------------------
 |  Autor1: nom.cognom@estudiants.urv.cat
 |  Autor2: nom.cognom@estudiants.urv.cat
 |  Data: Octubre 2017                 Versio: 1.0
 |-----------------------------------------------------------------|
 |	Nom projecte: ...
 |	Nom: tipus.h
 |  Descripcio del Programa:   Tipus i constants del projecte.
 | ----------------------------------------------------------------*/

#ifndef TIPUS_H_INCLUDED
#define TIPUS_H_INCLUDED

/* Definicio de constants del projecte */
#define NOM_MAX             30      /* Mida maxima del nom */
#define NUM_JUGADORS_MIN    0       /* Nombre minim de jugadors */
#define NUM_JUGADORS_MAX    2       /* Nombre maxim de jugadors */

#define FITXER_RECORDS   "records.txt"      /* Doneu un nom al fitxer */
#define FITXER_JOC       "juego.dat"        /* Doneu un nom al fitxer */

#define __MAC__                     /* Comentar si SO es WIN */
/*#define __WIN32__                  Comentar si SO es MAC o LINUX */

/* Definim la comanda segons el SO */
#ifdef __WIN32__
#define NETEJA_PANTALLA "cls"
#else
#define NETEJA_PANTALLA "clear"
#endif

/** Espai reservat per a les vostres constants */
#ifndef DIM_MAX
#define DIM_MAX 10
#endif


/* Definicio de nous tipus del projecte */

/** -------------------------------------------------------
 TIPUS BASICS
 ------------------------------------------------------- */
typedef char nom_t [NOM_MAX];

typedef struct
{
    int dia, mes, any;
} data_t;

typedef struct
{
    char nom[NOM_MAX];
    int punts;
    data_t data;
} record_t;

/** -------------------------------------------------------
 TIPUS ADDICIONALS
 ------------------------------------------------------- */
 typedef struct {
     char barcos[DIM_MAX][DIM_MAX];
     char disparos[DIM_MAX][DIM_MAX];
     unsigned short dim;
 } tauler_t;
 
typedef struct {
    tauler_t tauler;
    record_t record;
} jugador_t;

typedef enum {
    MAQUINA = (int)0,
    JUGADOR = (int)1,
    JUGADOR_MAQUINA = (int)2
} gamemode_t;

typedef struct {
    jugador_t jugador1;
    jugador_t jugador2;
    gamemode_t mode;
    int turno;
} partida_t;

#endif /* TIPUS_H_INCLUDED */

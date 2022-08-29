#if !defined(__JUGADOR__)
#define __JUGADOR__

#include "casillas.h"

#define NUM_JUGADORES 4

#define DINERO_INICIAL 1500U
#define DINERO_SALIDA 200U
#define TURNOS_CARCEL 2
#define MAX_JUGADORES 4

#define PORCENTAJE_RENTA_CASILLA 20

typedef struct
{
    unsigned char casilla = 0;
    unsigned char pasosPorDar = 0;
    unsigned short dinero;
    char turnosCarcel = 0;
    unsigned char numeroDePosesiones = 0;
    unsigned char posesiones[NUM_CASILLAS];
} Jugador;

Jugador *crearJugadores(unsigned char numeroJugadores) {
    Jugador *jugadores = new Jugador[numeroJugadores];
    for (int i = 0; i < numeroJugadores; i++) {
        jugadores[i].dinero = DINERO_INICIAL;
        for (int j = 0; j < NUM_CASILLAS; j++) {
            jugadores[i].posesiones[j] = 255;
        }
    }

    return jugadores;
}

bool haCompradoCasilla(Jugador *jugador, unsigned char casilla) {
    for (int i = 0; i < NUM_CASILLAS; i++) {
        if (jugador->posesiones[i] == casilla) {
            return true;
        }
    }
    return false;
}

void agregarPropiedad(Jugador *jugador, unsigned char casilla) {
    jugador->numeroDePosesiones++;
    for (int i = 0; i < NUM_CASILLAS; i++) {
        if (jugador->posesiones[i] == 255) {
            jugador->posesiones[i] = casilla;
            return;
        }
    }
}

int hanCompradoCasilla(int numeroDeJugadores, Jugador *jugadores, unsigned char casilla) {
    for (int i = 0; i < numeroDeJugadores; i++) {
        Jugador jugador = jugadores[i];
        for (int j = 0; j < NUM_CASILLAS; j++) {
            if (jugador.posesiones[j] == casilla) {
                return i;
            }
        }
    }

    return -1;
}

#endif // __JUGADOR__

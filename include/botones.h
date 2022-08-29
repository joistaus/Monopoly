#if !defined(__BOTONES__)
#define __BOTONES__

#include "casillas.h"
#include "jugadores.h"
#include <Button.h>

static uint8_t pines_jugadores[NUM_JUGADORES] = {
    A15, A14, A13, A12};
static Button *botones_jugadores[NUM_JUGADORES];

Button botonCancelar(A11);

static uint8_t pines_casillas[NUM_CASILLAS] = {
    22, 24, 26, 28, 30, 32, 34, 36, 38, 40,
    42, 44, 46, 48, 50, 52, 31, 33, 35, 37,
    39, 41, 43, 45, 47, 49, 51, 53, A10, A9,
    A7, A6, A5, A4, A3, A2, A1, A0, 6, 7};
static Button *botones_casillas[NUM_CASILLAS];

void inicializarBotones(void) {

    for (int i = 0; i < NUM_JUGADORES; i++) {
        botones_jugadores[i] = new Button(pines_jugadores[i]);
        botones_jugadores[i]->begin();
    }
    botonCancelar.begin();

    for (int i = 0; i < NUM_CASILLAS; i++) {
        botones_casillas[i] = new Button(pines_casillas[i]);
        botones_casillas[i]->begin();
    }
}

#endif // __BOTONES__

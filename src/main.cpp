#include <Arduino.h>

#include "botones.h"
#include "estados.h"
#include "jugadores.h"
#include "pantalla.h"

#define BUTTON_DELAY 1500U

void mostrarNumeroDado(int numero) {
    switch (numero) {
    case 1:
        lcd.print("UNO");
        break;
    case 2:
        lcd.print("DOS");
        break;
    case 3:
        lcd.print("TRES");
        break;
    case 4:
        lcd.print("CUATRO");
        break;
    case 5:
        lcd.print("CINCO");
        break;
    case 6:
        lcd.print("SEIS");
        break;
    }
}

void setup() {

    randomSeed(analogRead(A8)); // Random Seed

    inicializarPantalla();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Bienvenido a:");
    lcd.setCursor(0, 2);
    lcd.print("    MONOPOLY");

    inicializarBotones();

    delay(3000);

    lcd.clear();
}

static Estado estado = elegirCantidadJugadores;
static uint8_t cantidadJugadores;
static uint8_t idJugador = 0;
static Jugador *jugadores;

static uint8_t numberOfClicks = 0;
static unsigned long referenceMillis = 0;
static unsigned long currentMillis = 0;
static uint8_t idPropiedad = 0;

void pasarAlSiguienteTurno(void) {
    lcd.clear();

    idJugador = (idJugador + 1) % cantidadJugadores;
    numberOfClicks = 0;
    estado = lanzarDado;
}

void loop() {
    currentMillis = millis();

    switch (estado) {
    case elegirCantidadJugadores:

        lcd.setCursor(0, 1);
        lcd.print("Cuantos jugaran?");
        for (int i = 1; i < NUM_JUGADORES; i++) {
            if (botones_jugadores[i]->released()) {
                lcd.clear();
                cantidadJugadores = i + 1;
                lcd.setCursor(0, 0);
                lcd.print("N. DE JUGADORES:");
                lcd.setCursor(0, 2);
                lcd.print("       ");
                lcd.print(cantidadJugadores);
                delay(2000);
                lcd.clear();
                estado = empiezaJuego;
                break;
            }
        }

        break;
    case empiezaJuego:

        lcd.setCursor(0, 0);
        lcd.print("EMPIEZA EL JUEGO");
        lcd.setCursor(0, 2);
        lcd.print("TODOS TIENEN:");
        jugadores = crearJugadores(cantidadJugadores);
        lcd.setCursor(0, 3);
        lcd.print("$ ");
        lcd.print(DINERO_INICIAL);
        lcd.print(" !!!");
        delay(5000);
        lcd.clear();
        estado = lanzarDado;
        numberOfClicks = 0;
        break;
    case lanzarDado:

        lcd.setCursor(0, 0);
        lcd.print("$");
        lcd.print(jugadores[idJugador].dinero);
        lcd.setCursor(7, 0);
        lcd.print("Jugador ");
        lcd.print(idJugador + 1);
        lcd.setCursor(0, 1);
        lcd.print("Estas en:");
        lcd.setCursor(0, 2);
        lcd.print(nombresCasillas[jugadores[idJugador].casilla]);
        lcd.setCursor(0, 3);
        lcd.print("Lanza los dados!");

        if (botones_jugadores[idJugador]->released()) {
            if (numberOfClicks == 0) {
                referenceMillis = currentMillis;
                numberOfClicks++;
            } else if (numberOfClicks == 1) {
                if (currentMillis - referenceMillis <= BUTTON_DELAY) {
                    numberOfClicks++;
                }
            }
        }

        if (numberOfClicks == 1 && currentMillis - referenceMillis > BUTTON_DELAY) {
            lcd.clear();
            numberOfClicks = 0;
            idPropiedad = 0;
            estado = mostrarPosesiones;
        } else if (numberOfClicks == 2) {
            numberOfClicks = 0;

            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("Dados");
            lcd.setCursor(7, 0);
            lcd.print("Jugador ");
            lcd.print(idJugador + 1);
            delay(1000);

            lcd.setCursor(0, 1);
            lcd.print("Lanzando dados");
            delay(1000);
            lcd.setCursor(1, 2);
            lcd.print("Lanzando dados");
            delay(1000);
            lcd.setCursor(2, 3);
            lcd.print("Lanzando dados");
            delay(1000);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Dados");
            lcd.setCursor(7, 0);
            lcd.print("Jugador ");
            lcd.print(idJugador + 1);
            int dado1 = random(1, 7);
            lcd.setCursor(0, 2);
            lcd.print("- ");
            mostrarNumeroDado(dado1);
            int dado2 = random(1, 7);
            lcd.setCursor(0, 3);
            lcd.print("- ");
            mostrarNumeroDado(dado2);

            jugadores[idJugador].pasosPorDar = dado1 + dado2;
            jugadores[idJugador].casilla += dado1 + dado2;

            estado = avanzar;
        }
        break;
    case mostrarPosesiones:
        lcd.setCursor(0, 0);
        lcd.print("$");
        lcd.print(jugadores[idJugador].dinero);
        lcd.setCursor(7, 0);
        lcd.print("Jugador ");
        lcd.print(idJugador + 1);

        if (jugadores[idJugador].numeroDePosesiones == 0) {
            lcd.setCursor(0, 2);
            lcd.print("Sin propiedades");
            if (botones_jugadores[idJugador]->released()) {
                lcd.clear();
                estado = lanzarDado;
            }
        } else {
            lcd.setCursor(0, 1);
            lcd.print("Propiedades:");

            lcd.setCursor(0, 2);
            lcd.print(nombresCasillas[jugadores[idJugador].posesiones[idPropiedad]]);

            if (idPropiedad + 1 < jugadores[idJugador].numeroDePosesiones) {

                lcd.setCursor(0, 3);
                lcd.print(nombresCasillas[jugadores[idJugador].posesiones[idPropiedad + 1]]);
            }

            if (botones_jugadores[idJugador]->released()) {
                lcd.clear();
                idPropiedad += 2;

                if (idPropiedad >= jugadores[idJugador].numeroDePosesiones) {

                    lcd.clear();
                    estado = lanzarDado;
                }
            }
        }

        break;
    case avanzar:
        // TODO esperar por usuario para avanzar

        if (botones_jugadores[idJugador]->released()) {

            lcd.clear();
            estado = verificarCasilla;
            numberOfClicks = 0;
        }
        break;
    case verificarCasilla:
        if (jugadores[idJugador].casilla >= NUM_CASILLAS) {
            lcd.setCursor(0, 0);
            lcd.print("RECIBES:");
            lcd.setCursor(0, 2);
            lcd.print("$ ");
            lcd.print(DINERO_SALIDA);
            jugadores[idJugador].dinero += DINERO_SALIDA;
            jugadores[idJugador].casilla %= NUM_CASILLAS;
            delay(3000);
            lcd.clear();
        }

        lcd.setCursor(0, 0);
        lcd.print("$");
        lcd.print(jugadores[idJugador].dinero);
        lcd.setCursor(7, 0);
        lcd.print("Jugador ");
        lcd.print(idJugador + 1);
        lcd.setCursor(0, 1);
        lcd.print("Estas en:");
        lcd.setCursor(0, 2);
        lcd.print(nombresCasillas[jugadores[idJugador].casilla]);

        int idJugadorAcreedor;

        lcd.setCursor(0, 3);
        if (haCompradoCasilla(jugadores + idJugador, jugadores[idJugador].casilla)) {
            lcd.print("Ya posees esto");
            if (botones_jugadores[idJugador]->released()) {
                pasarAlSiguienteTurno();
            }
        } else if ((idJugadorAcreedor = hanCompradoCasilla(cantidadJugadores, jugadores, jugadores[idJugador].casilla)) != -1) {
            short precio = (preciosCasillas[jugadores[idJugador].casilla] * PORCENTAJE_RENTA_CASILLA) / 100;
            lcd.print("Debes pagar $");
            lcd.print(precio);

            if (botones_jugadores[idJugador]->released()) {
                jugadores[idJugador].dinero -= precio;
                jugadores[idJugadorAcreedor].dinero += precio;

                lcd.setCursor(0, 0);
                lcd.print("     ");
                lcd.setCursor(0, 0);
                lcd.print("$ ");
                lcd.print(jugadores[idJugador].dinero);

                lcd.setCursor(0, 3);
                lcd.print("Pagaste $");
                lcd.print(precio);
                lcd.print("      ");
                delay(3000);

                pasarAlSiguienteTurno();
            }
        } else {
            short precio = preciosCasillas[jugadores[idJugador].casilla];
            if (precio < 0) {
                precio *= -1;
                lcd.print("Debes pagar $");
                lcd.print(precio);

                if (botones_jugadores[idJugador]->released()) {

                    jugadores[idJugador].dinero -= precio;

                    lcd.setCursor(0, 3);
                    lcd.print("Pagaste $");
                    lcd.print(precio);
                    lcd.print("    ");

                    lcd.setCursor(0, 0);
                    lcd.print("     ");
                    lcd.setCursor(0, 0);
                    lcd.print("$");
                    lcd.print(jugadores[idJugador].dinero);
                    delay(3000);

                    pasarAlSiguienteTurno();
                }
            } else if (precio == 0) {
                lcd.print("Descansa aqui");

                if (botones_jugadores[idJugador]->released()) {
                    pasarAlSiguienteTurno();
                }
            } else {
                lcd.print("Comprar x $");
                lcd.print(precio);
                lcd.print("?");

                if (botones_jugadores[idJugador]->released()) {
                    if (numberOfClicks == 0) {
                        referenceMillis = currentMillis;
                        numberOfClicks++;
                    } else if (numberOfClicks == 1) {
                        if (currentMillis - referenceMillis <= BUTTON_DELAY) {
                            numberOfClicks++;
                        }
                    }
                }

                if (numberOfClicks == 1 && currentMillis - referenceMillis > BUTTON_DELAY) {
                    lcd.setCursor(0, 3);
                    lcd.print("* No compraste *");
                    delay(3000);

                    pasarAlSiguienteTurno();
                } else if (numberOfClicks == 2) {
                    jugadores[idJugador].dinero -= precio;

                    agregarPropiedad(jugadores + idJugador, jugadores[idJugador].casilla);
                    lcd.setCursor(0, 3);
                    lcd.print("Compraste x $");
                    lcd.print(precio);

                    lcd.setCursor(0, 0);
                    lcd.print("     ");
                    lcd.setCursor(0, 0);
                    lcd.print("$");
                    lcd.print(jugadores[idJugador].dinero);
                    delay(3000);

                    pasarAlSiguienteTurno();
                }
            }

            break;
        }
    }
}
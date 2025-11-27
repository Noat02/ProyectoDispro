// SPACE/alien.c

#include "alien.h"
#include "aliens_draw.h" 
#include <time.h>  

// --- Inicialización ---
void inicializarAlien(Alien *a, int x, int y, int tipo) {
    a->x = x;
    a->y = y;
    a->estaVivo = true;
    a->tipoAlien = tipo;
    a->deltaX = 2;  // Movimiento hacia la derecha inicialmente
    a->bajar = false;
    a->delay = 0.5f; // Movimiento cada 0.5 segundos
    a->previous = (float)clock() / CLOCKS_PER_SEC;

    // Asignación de puntos según el tipo 
    switch (tipo) {
        case OCTOPUS_TYPE: a->puntos = 10; break;
        case CRAB_TYPE:    a->puntos = 20; break;
        case PHANTOM_TYPE: a->puntos = 40; break;
        case SQUID_TYPE:   a->puntos = 50; break;
        default: a->puntos = 10; 
    }
}

// --- Recibir Daño (Destrucción) ---//
int recibirDanio(Alien *a) {
    if (a->estaVivo) {
        a->estaVivo = false;
        return a->puntos;
    }
    return 0;
}

// --- Actualizar Posición ---//
int actualizarAlien(Alien *a) {
    int llego_al_borde = 0;

    if ((float)clock() / CLOCKS_PER_SEC - a->previous >= a->delay) {
        if (a->estaVivo) {
            if (a->bajar) {
                a->y += 6;  // Bajar una fila
                a->bajar = false;
                a->deltaX = -a->deltaX;  // Cambiar dirección
            } else {
                a->x += a->deltaX;  // Mover horizontalmente
            }

            // Verificar si llegó al borde
            if (a->x <= 5 || a->x >= 235) {
                llego_al_borde = 1;
            }

            a->previous = (float)clock() / CLOCKS_PER_SEC;
        }
    }

    return llego_al_borde;
}

// --- Conector de Dibujo ---
void dibujarAlienPorTipo(Alien *a, int frame) {
    if (a->estaVivo) {
        switch (a->tipoAlien) {
            case SQUID_TYPE:
                draw_squid(a->x, a->y, frame);
                break;
            case CRAB_TYPE:
                draw_crab(a->x, a->y, frame);
                break;
            case PHANTOM_TYPE:
                draw_phantom(a->x, a->y, frame);
                break;
            case OCTOPUS_TYPE:
                draw_octopus(a->x, a->y, frame);
                break;
        }
    }
}
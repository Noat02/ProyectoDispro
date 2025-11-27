// SPACE/ovni.c

#include <stdlib.h> 
#include "ovni.h"
#include "ovni_draw.h" 

// --- Inicialización ---
void inicializarOvni(Ovni *o) {
    o->x = OVNI_START_X; // Empieza fuera de la pantalla (izq)
    o->estaActivo = false;
    o->velocidad = 5; 
    o->direccion = 1; // Por defecto se mueve a la derecha
}



// --- Actualización de Movimiento ---
void actualizarOvni(Ovni *o) {
    if (o->estaActivo) {
        o->x += o->velocidad * o->direccion;

        // Comprueba si ha salido de la pantalla (240 columnas)
        if (o->x > 240 || o->x < OVNI_START_X) {
            o->estaActivo = false; // Desaparece
            o->x = OVNI_START_X;   // Reinicia posición
        }

    } else {
       
        if ((rand() % 200) == 0) {
            o->estaActivo = true;
            o->direccion = (rand() % 2) ? 1 : -1; // Dirección aleatoria
            // Asigna X inicial según la dirección
            o->x = (o->direccion == 1) ? OVNI_START_X : 240;
        }
    }
}

// --- Destrucción ---
int recibirDisparoOvni(Ovni *o) {
    if (o->estaActivo) {
        o->estaActivo = false;
        // La puntuación es aleatoria en el juego original (50, 100, 150 o 300)
        int puntos[] = {50, 100, 150, 300};
        return puntos[rand() % 4];
    }
    return 0;
}
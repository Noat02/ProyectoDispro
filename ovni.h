// SPACE/ovni.h

#ifndef OVNI_H
#define OVNI_H

#include <stdbool.h>

// Definición de las constantes de movimiento y posición
#define OVNI_START_X -10 // Empieza fuera de la pantalla
#define OVNI_Y 2         // Fila donde se dibuja (usada en main_completo.c)

// --- Estructura del OVNI (Atributos) ---
typedef struct {
    int x;             // Posición horizontal
    bool estaActivo;   // Si está en pantalla o no
    int velocidad;     // Velocidad de movimiento (siempre rápido)
    int direccion;     // 1 (derecha) o -1 (izquierda)
} Ovni;

// --- Prototipos de Funciones ---

// Inicializa el OVNI
void inicializarOvni(Ovni *o);

// Mueve el OVNI y comprueba si sale de la pantalla
void actualizarOvni(Ovni *o);

// Devuelve los puntos y lo marca como inactivo
int recibirDisparoOvni(Ovni *o);

#endif
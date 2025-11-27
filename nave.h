// SPACE/nave.h

#ifndef NAVE_H
#define NAVE_H

#include <stdbool.h>

// --- Estructura de la Nave del Jugador ---
typedef struct {
    int x;             // Posición horizontal (Y es fija)
    int vidas;         // Vidas restantes
    int velocidad;     // Velocidad de movimiento lateral
    bool estaViva;
} Nave;

// --- Prototipos de Métodos ---
void inicializarNave(Nave *n);
void moverNave(Nave *n, int r); 
void recibirDanoNave(Nave *n);

#endif
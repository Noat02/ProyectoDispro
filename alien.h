// SPACE/alien.h

#ifndef ALIEN_H
#define ALIEN_H

#include <stdbool.h>
#include <time.h>  // <-- AGREGAR esto

// --- 1. Definiciones de Tipos de Alien para Lógica ---
#define OCTOPUS_TYPE  1 // Rojo (10 Puntos)
#define CRAB_TYPE     2 // Verde (20 o 30 Puntos)
#define PHANTOM_TYPE  3 // Amarillo (40 Puntos)
#define SQUID_TYPE    4 // Azul/Púrpura (50 Puntos)

// --- 2. Estructura del Alien (Atributos) ---
typedef struct {
    int x;                   // Posición horizontal
    int y;                   // Posición vertical
    int puntos;              // Puntuación que otorga
    bool estaVivo;           // Estado actual
    int tipoAlien;           // Para saber qué sprite dibujar y qué puntos asignar
    int deltaX;              // Dirección y velocidad de movimiento (2 derecha, -2 izquierda)
    bool bajar;              // Indica si debe bajar en el próximo movimiento
    float delay;             // Tiempo entre movimientos
    float previous;          // Tiempo del último movimiento
} Alien;

// --- 3. Prototipos de Funciones (Métodos de la Entidad) ---

// Inicializa el alien con su posición y tipo
void inicializarAlien(Alien *a, int x, int y, int tipo);

// Devuelve los puntos y marca el alien como muerto
int recibirDanio(Alien *a);

// Conecta el atributo 'tipoAlien' con la función de dibujo correcta
void dibujarAlienPorTipo(Alien *a, int frame);

// Actualiza la posición del alien, retorna 1 si llegó al borde
int actualizarAlien(Alien *a);  // <-- CAMBIAR de void a int

#endif
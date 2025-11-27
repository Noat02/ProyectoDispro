// SPACE/bala.c

#include "bala.h"
#include "bala_draw.h"
#include "conio.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>

#define WHITE "\033[37m"
#define RED   "\033[31m"


Bala arregloBalas[10];

char *bala759[] = {
    "█"
};

void inicializarBalas(void) {
    for (int i = 0; i < 10; i++) {
        arregloBalas[i].estaActiva = 0;
    }
}

int iniciacionbala(int x,int y) {
    for(int i=0; i<10; i++) {
        if (arregloBalas[i].estaActiva == 0) {
            arregloBalas[i].x = x;
            arregloBalas[i].y = y;
            arregloBalas[i].estaActiva = 1;
            arregloBalas[i].direccion = 1;
            return i;
        }
    }
    return -1;
}


void updateBala(int tipo) {

    for (int i = 0; i < 10; i++) {
        if (arregloBalas[i].estaActiva == 0) continue;

        int x = arregloBalas[i].x;
        int oldY = arregloBalas[i].y;

        /* borrar la posición anterior completa */
        for (int j = 0; j < 4; j++) {
            gotoxy(x, oldY + j);
            printf("     ");
        }

        /* mover hacia arriba */  
        arregloBalas[i].y -= 4;

        /* si sale de la pantalla: desactivar */
        if (arregloBalas[i].y < 1) {
            arregloBalas[i].estaActiva = 0;
            continue;
        }

        /* dibujar en la nueva posición */
        if (tipo == BALA_JUGADOR) {
            draw_balapersona(arregloBalas[i].x, arregloBalas[i].y);
        }
        else if (tipo == BALA_ALIEN) {
            draw_bala_alien(arregloBalas[i].x, arregloBalas[i].y);
        }
        else if (tipo == BALA_OVNI) {
            draw_bala_ovni(arregloBalas[i].x, arregloBalas[i].y);
        }
        
    }

    fflush(stdout);
}

int balasDebenMoverse(float bala_delay, float *bala_previous) {
    float ahora = (float)clock() / CLOCKS_PER_SEC;
    if (ahora - *bala_previous >= bala_delay) {
        *bala_previous = ahora;
        return 1;
    }
    return 0;
}

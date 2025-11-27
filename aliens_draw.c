/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR ALIENS CON COLOR               */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "aliens_data.h"
#include "aliens_draw.h"
#include "conio.h"  

/******************************************************************************/
/*                           COLORES ANSI                                     */
/******************************************************************************/

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

/******************************************************************************/
/*                           FUNCIONES BÁSICAS                                */
/******************************************************************************/

/* Limpiar pantalla */
void clear_screen(void) {
    printf("\033[H\033[J");
}


/******************************************************************************/
/*                     FUNCIONES PARA DIBUJAR ALIENS                          */
/******************************************************************************/

/* Dibujar Squid (Azul) */
void draw_squid(int x, int y, int frame) {
    char **sprite = (frame == 0) ? squid1 : squid2;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", BLUE, sprite[i], RESET);
    }
}

/* Dibujar Crab (Verde) */
void draw_crab(int x, int y, int frame) {
    char **sprite = (frame == 0) ? crab1 : crab2;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", GREEN, sprite[i], RESET);
    }
}

/* Dibujar Octopus (Rojo) */
void draw_octopus(int x, int y, int frame) {
    char **sprite = (frame == 0) ? octopus1 : octopus2;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i); 
        printf("%s%s%s", RED, sprite[i], RESET);
    }
}

/* Dibujar Phantom (Amarillo) */
void draw_phantom(int x, int y, int frame) {
    char **sprite = (frame == 0) ? phantom1 : phantom2;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", YELLOW, sprite[i], RESET);
    }
}
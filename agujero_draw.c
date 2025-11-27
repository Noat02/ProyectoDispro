/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR BARRERAS EN NARANJA             */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "agujero_data.h"
#include "agujero_draw.h"
#include "conio.h" 

/******************************************************************************/
/*                           COLORES ANSI                                     */
/******************************************************************************/

#define RESET "\033[0m"
#define ORANGE "\x1B[38;5;208m"



/******************************************************************************/
/*                     FUNCIÓN PARA DIBUJAR BARRERAS                          */
/******************************************************************************/

/* Dibujar Barrera (Naranja) según fase (1-9) */
void draw_agujero(int x, int y, int fase) {
    char **sprite;
    int lineas;

    /* Seleccionar sprite según fase */
    switch (fase) {
        case 1:
            sprite = agujero_fase1;
            lineas = 8;
            break;
        case 2:
            sprite = agujero_fase2;
            lineas = 8;
            break;
        case 3:
            sprite = agujero_fase3;
            lineas = 8;
            break;
        case 4:
            sprite = agujero_fase4;
            lineas = 7;
            break;
        case 5:
            sprite = agujero_fase5;
            lineas = 6;
            break;
        case 6:
            sprite = agujero_fase6;
            lineas = 4;
            break;
        case 7:
            sprite = agujero_fase7;
            lineas = 3;
            break;
        case 8:
            sprite = agujero_fase8;
            lineas = 2;
            break;
        case 9:
            sprite = agujero_fase9;
            lineas = 0;
            break;
        default:
            sprite = agujero_fase1;
            lineas = 8;
            break;
    }

    /* Dibujar barrera en naranja */
    for (int i = 0; i < lineas; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", ORANGE, sprite[i], RESET);
    }
}
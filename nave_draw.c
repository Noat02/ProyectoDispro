/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR LA NAVE EN BLANCO              */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "nave_data.h"
#include "nave_draw.h"
#include "conio.h"  // <-- AGREGAR este include
#include "input.h"

/******************************************************************************/
/*                           COLORES ANSI                                     */
/******************************************************************************/

#define RESET "\033[0m"
#define WHITE "\033[37m"

/******************************************************************************/
/*                           FUNCIONES EXTERNAS                               */
/******************************************************************************/

// ELIMINAR esta línea: extern void gotoxy(int x, int y);

/******************************************************************************/
/*                     FUNCIÓN PARA DIBUJAR NAVE                              */
/******************************************************************************/

/* Dibujar Nave (Blanca) */
void draw_nave(int x, int y) {
    for (int i = 0; i < 8; i++) {
        gotoxy(x, y + i); 
        printf("%s%s%s", WHITE, navireal[i], RESET);
    }
}
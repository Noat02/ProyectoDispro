/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR ALIENS CON COLOR               */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "explocion_data.h"
#include "explosion_draw.h"
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
/*                     FUNCIONES PARA DIBUJAR EXPLOSIONES                      */
/******************************************************************************/

void draw_bigexplosion(int x, int y) {
    char **sprite = explosion_small;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", RED, explosion_big[i], RESET);
    }
}

void draw_smallexplosion(int x, int y) {
    char **sprite = explosion_small;
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", YELLOW, explosion_small[i], RESET);
    }
}
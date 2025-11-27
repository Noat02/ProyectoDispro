/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR ALIENS CON COLOR               */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "bala_data.h"
#include "bala_draw.h"
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
/*                     FUNCIONES PARA DIBUJAR ALIENS                          */
/******************************************************************************/


void draw_balapersona(int x, int y) {
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", BLUE, bala[i], RESET);
    }
}


void draw_bala_alien(int x, int y) {
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", GREEN, bala2[i], RESET);
    }
}

void draw_bala_ovni(int x, int y) {
    for (int i = 0; i < 4; i++) {
        gotoxy(x, y + i);  
        printf("%s%s%s", GREEN, bala3[i], RESET);
    }
}


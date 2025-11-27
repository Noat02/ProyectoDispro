/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE PANTALLAS DE INICIO Y GAME OVER                        */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include "pantallas.h"
#include "conio.h"
#include "simbolos_draw.h"
#include "aliens_draw.h"
#include "input.h"

/******************************************************************************/
/*                           COLORES ANSI                                     */
/******************************************************************************/

#define RED     "\033[31m"
#define WHITE   "\033[37m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

/******************************************************************************/
/*                     PANTALLA DE INICIO                                     */
/******************************************************************************/

int mostrarPantallaInicio(void) {
    Clear_Screen();
    Cursor(0);
    
    /* Título */
    draw_word("SPACE INVADERS", 60, 3, MAGENTA);
    
    /* Instrucción */
    draw_word("PRESS ENTER TO PLAY", 52, 10, WHITE);
    
    /* Tabla de puntuación */
    draw_word("SCORE ADVANCE TABLE", 56, 18, CYAN);
    
    /* Dibujar aliens con sus puntos */
    /* Squid - Azul */
    draw_squid(60, 25, 0);
    draw_word("50 POINTS", 75, 26, BLUE);
    
    /* Phantom - Amarillo */
    draw_phantom(60, 31, 0);
    draw_word("40 POINTS", 75, 32, YELLOW);
    
    /* Crab - Verde */
    draw_crab(60, 37, 0);
    draw_word("20 POINTS", 75, 38, GREEN);
    
    /* Octopus - Rojo */
    draw_octopus(60, 43, 0);
    draw_word("10 POINTS", 75, 44, RED);
    
    /* OVNI */
    draw_word("MYSTERY", 75, 50, MAGENTA);
    
    /* Instrucción para salir */
    draw_word("PRESS Q TO QUIT", 58, 58, WHITE);
    
    fflush(stdout);
    
    /* Esperar tecla */
    while (1) {
        int tecla = getch();
        
        if (tecla == '\n' || tecla == '\r') {
            return 1;  /* Enter - jugar */
        }
        if (tecla == 'q' || tecla == 'Q') {
            return 0;  /* Q - salir */
        }
    }
}

/******************************************************************************/
/*                     PANTALLA DE GAME OVER                                  */
/******************************************************************************/

int mostrarPantallaGameOver(void) {
    Clear_Screen();
    Cursor(0);
    
    /* Título GAME OVER */
    draw_word("GAME OVER", 70, 15, RED);
    
    /* Opciones */
    draw_word("PRESS ENTER TO PLAY AGAIN", 48, 30, WHITE);
    
    draw_word("OR", 90, 38, WHITE);
    
    draw_word("PRESS Q TO QUIT", 60, 46, WHITE);
    
    fflush(stdout);
    
    /* Esperar tecla */
    while (1) {
        int tecla = getch();
        
        if (tecla == '\n' || tecla == '\r') {
            return 1;  /* Enter - jugar de nuevo */
        }
        if (tecla == 'q' || tecla == 'Q') {
            return 0;  /* Q - salir */
        }
    }
}

/******************************************************************************/
/*                     PANTALLA DE GANASTE                                    */
/******************************************************************************/

int mostrarPantallaGanaste(void) {
    Clear_Screen();
    Cursor(0);
    
    /* Título */
    draw_word("YOU WIN", 75, 15, GREEN);
    
    /* Mensaje */
    draw_word("CONGRATULATIONS", 62, 25, YELLOW);
    
    /* Opciones */
    draw_word("PRESS ENTER TO PLAY AGAIN", 48, 38, WHITE);
    
    draw_word("OR", 90, 46, WHITE);
    
    draw_word("PRESS Q TO QUIT", 60, 54, WHITE);
    
    fflush(stdout);
    
    /* Esperar tecla */
    while (1) {
        int tecla = getch();
        
        if (tecla == '\n' || tecla == '\r') {
            return 1;  /* Enter - jugar de nuevo */
        }
        if (tecla == 'q' || tecla == 'Q') {
            return 0;  /* Q - salir */
        }
    }
}

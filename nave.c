// SPACE/nave.c
#include "conio.h"
#include "nave.h"
#include "nave_draw.h" 
#include "input.h"
#include <stdio.h>
#include <ctype.h>
#include "bala.h"



void inicializarNave(Nave *n) {
    n->x = 27;
    n->vidas = 3;
    n->velocidad = 2;
    n->estaViva = true;
}

void borrarNave(int x, int y) {
    for (int i = 0; i < 8; i++) {     
        gotoxy(x, y + i);
        printf("        ");        
    }
}


void moverNave(Nave *n, int r) {
    const int MIN_X = 1;
    const int MAX_X = 230;
    const int NAVE_Y = 70; 

    if (!kbhit()) return;
    int ch = getch();
    int mover = 0;
    int viejax = n->x;

    
    if (ch == 27) {
        int b1 = -1, b2 = -1;
        if (kbhit()) b1 = getch();
        if (kbhit()) b2 = getch();
        if (b1 == 91) {
            if (b2 == 67) {
                 n->x += 2; mover = 1; /* moverse a la derecha */
            } 
            else if (b2 == 68) { 
                n->x -= 2; mover = 1;  /* moverse a la izquierda */
            }else if (b2 == 65){
                iniciacionbala(n->x + 3, NAVE_Y-4);
            }

        }
    } else {
        
        if (ch == 'a' || ch == 'A' || ch == 97) {
             n->x -= 2; 
             mover = 1; 
        }
        else if (ch == 'd' || ch == 'D' || ch == 100) {
             n->x += 2; 
             mover = 1; 
        } else {
            if (ch == 'w' || ch == 'W'){
                iniciacionbala(n->x + 3, NAVE_Y-4);
            }
        }


    }


    if (mover) {
        if (n->x < MIN_X) {
            n->x = MIN_X; // mirar que la nave no se salga del limite izquierdo
        } 
        else if (n->x > MAX_X) {
            n->x = MAX_X; // mirar que la nave no se salga del limite derecho
        }

        
        borrarNave(viejax, NAVE_Y); //borrar la nave vieja
        draw_nave(n->x, NAVE_Y); //dibujar la nave en la nueva posicion
        fflush(stdout);
    }
}


void recibirDanoNave(Nave *n) {
    n->vidas--;
    if (n->vidas <= 0) {
        n->estaViva = false;
    }
}
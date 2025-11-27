/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   IMPLEMENTACIÓN DE FUNCIONES PARA DIBUJAR LETRAS                         */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "simbolos_data.h"
#include "simbolos_draw.h"
#include "conio.h"  // <-- AGREGAR este include

/******************************************************************************/
/*                           COLORES ANSI                                     */
/******************************************************************************/

#define RESET "\033[0m"
#define RED   "\033[31m"
#define WHITE "\033[37m"
#define ROWS 4

/******************************************************************************/
/*                           FUNCIONES EXTERNAS                               */
/******************************************************************************/



void draw_letter(char letter, int x, int y, const char *color) {
    char **sprite = NULL;
    
    /* Convertir a mayúscula */
    letter = toupper(letter);
    
    /* Seleccionar sprite según letra */
    switch (letter) {
        case 'A': sprite = A_letra; break;
        case 'B': sprite = B_letra; break;
        case 'C': sprite = C_letra; break;
        case 'D': sprite = D_letra; break;
        case 'E': sprite = E_letra; break;
        case 'F': sprite = F_letra; break;
        case 'G': sprite = G_letra; break;
        case 'H': sprite = H_letra; break;
        case 'I': sprite = I_letra; break;
        case 'J': sprite = J_letra; break;
        case 'K': sprite = K_letra; break;
        case 'L': sprite = L_letra; break;
        case 'M': sprite = M_letra; break;
        case 'N': sprite = N_letra; break;
        case 'O': sprite = O_letra; break;
        case 'P': sprite = P_letra; break;
        case 'Q': sprite = Q_letra; break;
        case 'R': sprite = R_letra; break;
        case 'S': sprite = S_letra; break;
        case 'T': sprite = T_letra; break;
        case 'U': sprite = U_letra; break;
        case 'V': sprite = V_letra; break;
        case 'W': sprite = W_letra; break;
        case 'X': sprite = X_letra; break;
        case 'Y': sprite = Y_letra; break;
        case 'Z': sprite = Z_letra; break;
        case '0': sprite = cero; break;
        case '1': sprite = uno; break;
        case '2': sprite = dos; break;
        case '3': sprite = tres; break;
        case '4': sprite = cuatro; break;
        case '5': sprite = cinco; break;
        case '6': sprite = seis; break;
        case '7': sprite = siete; break;
        case '8': sprite = ocho; break;
        case '9': sprite = nueve; break;
        case '!': sprite = excla1; break;
        case '.': sprite = excla2; break;
        case '?': sprite = pregunta; break;
        case '<': sprite = menor_que; break;
        case '>': sprite = mayor_que; break;
        case ':': sprite = dos_puntos; break;
        case '-': sprite = guion; break;

        case ' ': return;  /* Espacio - no dibujar nada */
        default: return;   /* Carácter no soportado */
    }

    
    /* Dibujar letra */
    if (sprite != NULL) {
        for (int i = 0; i < 4; i++) {
            gotoxy(x, y + i);
            printf("%s%s%s", color, sprite[i], RESET);
        }
    }
}

/******************************************************************************/
/*                   FUNCIÓN PARA DIBUJAR UNA PALABRA                         */
/******************************************************************************/

void draw_word(const char *word, int x, int y, const char *color) {
    int current_x = x;
    
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] == ' ') {
            current_x += 2;  /* Espacio entre palabras */
        } else {
            draw_letter(word[i], current_x, y, color);
            current_x += 4;  /* 3 chars de ancho + 1 de espacio */
        }
    }
}

void draw_sprite(int x, int y, char **sprite_data, int rows) {
    for (int i = 0; i < rows; i++) {
        gotoxy(x, y + i);
        printf("%s", sprite_data[i]);
    }
}

void draw_char(int x, int y, char char_code) {
        
    char **sprite = NULL; 
    
    // **PASO 1: Seleccionar el sprite basado en el carácter**
    switch (char_code) {
        // --- Símbolos ---
        case '?': sprite = pregunta; break;
        case '<': sprite = menor_que; break;
        case '>': sprite = mayor_que; break;
        case ':': sprite = dos_puntos; break;
        case '-': sprite = guion; break;

        // --- Números ---
        case '0': sprite = cero; break;
        case '1': sprite = uno; break;
        case '2': sprite = dos; break;
        case '3': sprite = tres; break;
        case '4': sprite = cuatro; break;
        case '5': sprite = cinco; break;
        case '6': sprite = seis; break;
        case '7': sprite = siete; break;
        case '8': sprite = ocho; break;
        case '9': sprite = nueve; break;
            
        // --- Letras (Solo las que uses en el HUD, ej. SCORE, LIVES) ---
        case 'S': sprite = S_letra; break;
        case 'C': sprite = C_letra; break;
        case 'O': sprite = O_letra; break;
        case 'R': sprite = R_letra; break;
        case 'E': sprite = E_letra; break;
        case 'L': sprite = L_letra; break;
        case 'I': sprite = I_letra; break;
        case 'V': sprite = V_letra; break;
        case 'H': sprite = H_letra; break;

        // Si el caracter no tiene un sprite definido, usa un espacio
        default: sprite = NULL; break; 
    }

    // **PASO 2: Dibujar el sprite seleccionado**
    if (sprite != NULL) {
        draw_sprite(x, y, sprite, ROWS); 
    }
}
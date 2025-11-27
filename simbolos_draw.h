/* Ruler 1         2         3         4         5         6         7        */

#ifndef SIMBOLOS_DRAW_H
#define SIMBOLOS_DRAW_H

/******************************************************************************/
/*                                                                            */
/*   PROTOTIPOS DE FUNCIONES PARA DIBUJAR SIMBOLOS                            */
/*                                                                            */
/******************************************************************************/

/* Función para dibujar una letra específica */
void draw_letter(char letter, int x, int y, const char *color);

/* Función para dibujar una palabra completa */
void draw_word(const char *word, int x, int y, const char *color);

void draw_char(int x, int y, char char_code);

void draw_sprite(int x, int y, char **sprite_data, int rows);

#endif /* LETRAS_DRAW_H */

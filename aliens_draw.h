/* Ruler 1         2         3         4         5         6         7        */

#ifndef ALIENS_DRAW_H
#define ALIENS_DRAW_H

/******************************************************************************/
/*                                                                            */
/*   PROTOTIPOS DE FUNCIONES PARA DIBUJAR ALIENS                             */
/*                                                                            */
/******************************************************************************/

/* Funciones básicas */
void clear_screen(void);


/* Funciones para dibujar cada tipo de alien */
void draw_squid(int x, int y, int frame);
void draw_crab(int x, int y, int frame);
void draw_octopus(int x, int y, int frame);
void draw_phantom(int x, int y, int frame);

#endif /* ALIENS_DRAW_H */
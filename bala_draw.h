#ifndef BALA_DRAW_H
#define BALA_DRAW_H

/******************************************************************************/
/*                                                                            */
/*   PROTOTIPOS DE FUNCIONES PARA DIBUJAR BALAS                             */
/*                                                                            */
/******************************************************************************/

/* Funciones básicas */
void clear_screen(void);


/* Funciones para dibujar cada tipo de alien */
void draw_balapersona(int x, int y);
void draw_bala_alien(int x, int y);
void draw_bala_ovni(int x, int y);
#endif /* BALA_DRAW_H */
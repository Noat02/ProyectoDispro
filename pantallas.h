/* Ruler 1         2         3         4         5         6         7        */

#ifndef PANTALLAS_H
#define PANTALLAS_H

/******************************************************************************/
/*                                                                            */
/*   PROTOTIPOS PARA PANTALLAS DE INICIO Y GAME OVER                          */
/*                                                                            */
/******************************************************************************/

/* Mostrar pantalla de inicio - retorna 1 si presiona ENTER, 0 si sale */
int mostrarPantallaInicio(void);

/* Mostrar pantalla de game over - retorna 1 si quiere jugar de nuevo */
int mostrarPantallaGameOver(void);

/* Mostrar pantalla de victoria */
int mostrarPantallaGanaste(void);

#endif /* PANTALLAS_H */

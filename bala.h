// SPACE/bala.h

#ifndef BALA_H
#define BALA_H

typedef struct {
    int x;
    int y;
    int estaActiva;
    int direccion;
} Bala;

#define BALA_JUGADOR 1
#define BALA_ALIEN 2
#define BALA_OVNI 3

/* prototipos públicos */
int iniciacionbala(int x,int y);
void updateBala(int tipo);
void inicializarBalas(void);
int balasDebenMoverse(float bala_delay, float *bala_previous);
Bala* obtenerBalas(void);

#endif /* BALA_H */


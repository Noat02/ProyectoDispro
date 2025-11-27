/* Ruler 1         2         3         4         5         6         7        */

/******************************************************************************/
/*                                                                            */
/*   SPACE INVADERS                       */
/*                                                                            */
/*   Flujo: Inicio -> Juego -> Game Over/Ganaste -> Repetir o Salir           */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "aliens_draw.h"
#include "nave_draw.h"
#include "ovni_draw.h"
#include "agujero_draw.h"
#include "simbolos_draw.h"
#include "conio.h"
#include "alien.h"
#include "ovni.h"
#include "nave.h"
#include "input.h"
#include "pantallas.h"
#include "bala.h"

/******************************************************************************/
/*                           CONFIGURACIÓN                                    */
/******************************************************************************/

#define ALIEN_HEIGHT 4

/* Aliens - 5 FILAS  */
#define ALIENS_PER_ROW 20
#define ALIEN_ROWS 5
#define ALIEN_START_X 10   
#define ALIEN_START_Y 15
#define ALIEN_SPACING_X 8 
#define ALIEN_SPACING_Y 6  
#define ALIEN_LIMITE_IZQ 5
#define ALIEN_LIMITE_DER 230
#define ALIEN_LIMITE_ABAJO 55
#define ALIEN_BAJAR_CANTIDAD 4

/* OVNI */
#define OVNI_POS_Y 9
#define OVNI_APARICION_PROB 800

/* Barreras - 5 Agujeros Negros */
#define NUM_BARRERAS 5
#define AGUJERO_START_X 20
#define AGUJERO_SPACING_X 50
#define AGUJERO_Y 48
#define AGUJERO_FASE_INICIAL 1

/* Nave del Jugador */
#define NAVE_X 100
#define NAVE_Y 60

/* Vidas (HUD inferior) */
#define LIVES_X 10
#define LIVES_Y 75
#define MAX_LIVES 3
#define LIFE_SPACING_X 10
#define WHITE "\033[37m"

/* Puntuación (HUD superior) */
#define SCORE_X 10
#define HISCORE_X 90
#define SCORE2_X 170

/******************************************************************************/
/*                           VARIABLES GLOBALES                               */
/******************************************************************************/

/* Array de aliens */
Alien aliens[ALIEN_ROWS][ALIENS_PER_ROW];

/* Variables de control de la FLOTA */
int flota_direccion;
int flota_frame;
int flota_total_vivos;
float flota_delay;
float flota_previous;

/* OVNI */
Ovni ovni;
int ovni_x_anterior;
float ovni_delay;
float ovni_previous;

/* BALAS */
float bala_delay;
float bala_previous;

/* Nave */
Nave nave;

/******************************************************************************/
/*                           FUNCIONES DE HUD                                 */
/******************************************************************************/

void draw_score_hud(int x, int y, const char *label) {
    int i = 0;
    while (label[i] != '\0') {
        draw_char(x + (i * 4), y, label[i]); 
        i++;
    }
}

void dibujarHUD(void) {
    draw_score_hud(SCORE_X, 2, "SCORE <1>:000000");
    draw_score_hud(HISCORE_X, 2, "HI-SCORE:000000");
    draw_score_hud(SCORE2_X, 2, "SCORE <2>:000000");
}

void dibujarBarreras(void) {
    for (int i = 0; i < NUM_BARRERAS; i++) {
        int x = AGUJERO_START_X + (i * AGUJERO_SPACING_X);
        draw_agujero(x, AGUJERO_Y, AGUJERO_FASE_INICIAL);
    }
}

void dibujarVidas(void) {
    draw_word("LIVES:", LIVES_X, LIVES_Y, WHITE);
    for (int i = 0; i < MAX_LIVES; i++) {
        draw_nave(LIVES_X + 27 + (i * LIFE_SPACING_X), LIVES_Y - 3); 
    }
}

/******************************************************************************/
/*                           FUNCIONES DE FLOTA                               */
/******************************************************************************/

int obtenerAnchoSprite(int tipo) {
    switch (tipo) {
        case SQUID_TYPE:   return 4;
        case PHANTOM_TYPE: return 5;
        case CRAB_TYPE:    return 6;
        case OCTOPUS_TYPE: return 6;
        default:           return 6;
    }
}

void inicializarAliens(void) {
    int tipos[] = {SQUID_TYPE, PHANTOM_TYPE, CRAB_TYPE, CRAB_TYPE, OCTOPUS_TYPE};

    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            int x = ALIEN_START_X + (col * ALIEN_SPACING_X);
            int y = ALIEN_START_Y + (fila * ALIEN_SPACING_Y);
            inicializarAlien(&aliens[fila][col], x, y, tipos[fila]);
        }
    }

    flota_direccion = 1;
    flota_frame = 0;
    flota_total_vivos = ALIEN_ROWS * ALIENS_PER_ROW;
    flota_delay = 0.25f;
    flota_previous = (float)clock() / CLOCKS_PER_SEC;
}

int encontrarExtremoIzquierdo(void) {
    for (int col = 0; col < ALIENS_PER_ROW; col++) {
        for (int fila = 0; fila < ALIEN_ROWS; fila++) {
            if (aliens[fila][col].estaVivo) {
                return col;
            }
        }
    }
    return -1;
}

int encontrarExtremoDerecho(void) {
    for (int col = ALIENS_PER_ROW - 1; col >= 0; col--) {
        for (int fila = 0; fila < ALIEN_ROWS; fila++) {
            if (aliens[fila][col].estaVivo) {
                return col;
            }
        }
    }
    return -1;
}

void borrarFlota(void) {
    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            if (aliens[fila][col].estaVivo) {
                int x = aliens[fila][col].x;
                int y = aliens[fila][col].y;
                int ancho = obtenerAnchoSprite(aliens[fila][col].tipoAlien);

                for (int i = 0; i < ALIEN_HEIGHT; i++) {
                    gotoxy(x, y + i);
                    for (int j = 0; j < ancho; j++) {
                        printf(" ");
                    }
                }
            }
        }
    }
}

void moverFlota(void) {
    int extremoIzq = encontrarExtremoIzquierdo();
    int extremoDer = encontrarExtremoDerecho();

    if (extremoIzq == -1) return;

    int posIzq = aliens[0][extremoIzq].x;
    int posDer = aliens[0][extremoDer].x + 6;

    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        if (aliens[fila][extremoIzq].estaVivo) {
            posIzq = aliens[fila][extremoIzq].x;
            break;
        }
    }
    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        if (aliens[fila][extremoDer].estaVivo) {
            posDer = aliens[fila][extremoDer].x + 6;
            break;
        }
    }

    int tocaBorde = 0;

    if (flota_direccion == 1 && posDer >= ALIEN_LIMITE_DER) {
        tocaBorde = 1;
    } else if (flota_direccion == -1 && posIzq <= ALIEN_LIMITE_IZQ) {
        tocaBorde = 1;
    }

    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            if (aliens[fila][col].estaVivo) {
                if (tocaBorde) {
                    aliens[fila][col].y += ALIEN_BAJAR_CANTIDAD;
                } else {
                    aliens[fila][col].x += flota_direccion * 2;
                }
            }
        }
    }

    if (tocaBorde) {
        flota_direccion *= -1;
    }

    flota_frame = 1 - flota_frame;

    int muertos = (ALIEN_ROWS * ALIENS_PER_ROW) - flota_total_vivos;
    flota_delay = 0.25f - (muertos * 0.002f);
    if (flota_delay < 0.03f) flota_delay = 0.03f;
}

void dibujarFlota(void) {
    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            if (aliens[fila][col].estaVivo) {
                dibujarAlienPorTipo(&aliens[fila][col], flota_frame);
            }
        }
    }
}

int flotaLlegoAbajo(void) {
    for (int fila = ALIEN_ROWS - 1; fila >= 0; fila--) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            if (aliens[fila][col].estaVivo) {
                if (aliens[fila][col].y >= ALIEN_LIMITE_ABAJO) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int flotaDebeMoverse(void) {
    float ahora = (float)clock() / CLOCKS_PER_SEC;
    if (ahora - flota_previous >= flota_delay) {
        flota_previous = ahora;
        return 1;
    }
    return 0;
}

/******************************************************************************/
/*                           FUNCIONES DE OVNI                                */
/******************************************************************************/

void borrarOvni(int x_anterior) {
    if (x_anterior > 0 && x_anterior < 250) {
        for (int i = 0; i < 4; i++) {
            gotoxy(x_anterior, OVNI_POS_Y + i);
            printf("        ");
        }
    }
}

int ovniDebeMoverse(void) {
    float ahora = (float)clock() / CLOCKS_PER_SEC;
    if (ahora - ovni_previous >= ovni_delay) {
        ovni_previous = ahora;
        return 1;
    }
    return 0;
}


void actualizarOvniConDibujo(void) {
    if (ovni.estaActivo) {
        if (ovniDebeMoverse()) {
            borrarOvni(ovni_x_anterior);

            ovni.x += ovni.direccion * 3;

            if (ovni.x > 240 || ovni.x < -10) {
                borrarOvni(ovni.x);
                ovni.estaActivo = false;
                ovni.x = OVNI_START_X;
                ovni_x_anterior = -100;
            } else {
                draw_ovni(ovni.x, OVNI_POS_Y);
                ovni_x_anterior = ovni.x;
            }
        }
    } else {
        if ((rand() % OVNI_APARICION_PROB) == 0) {
            ovni.estaActivo = true;
            ovni.direccion = (rand() % 2) ? 1 : -1;
            ovni.x = (ovni.direccion == 1) ? 5 : 235;
            ovni_x_anterior = ovni.x;
            ovni_previous = (float)clock() / CLOCKS_PER_SEC;
            draw_ovni(ovni.x, OVNI_POS_Y);
        }
    }
}

/******************************************************************************/
/*                           JUEGO PRINCIPAL                                  */
/******************************************************************************/

/* Retorna: 1 = ganó, 0 = perdió */
int ejecutarJuego() {
    /* Configurar consola */
    Clear_Screen();
    Cursor(0);

    /* Inicializar entidades */
    inicializarAliens();
    inicializarOvni(&ovni);
    inicializarNave(&nave);
    inicializarBalas();
    
    ovni_x_anterior = -100;
    ovni_delay = 0.05f;
    ovni_previous = (float)clock() / CLOCKS_PER_SEC;
    
    bala_delay = 0.08f;
    bala_previous = (float)clock() / CLOCKS_PER_SEC;

    /* Posición inicial de la nave */
    nave.x = NAVE_X;

    /* Dibujar elementos estáticos */
    dibujarHUD();
    dibujarBarreras();
    dibujarVidas();
    draw_nave(nave.x, NAVE_Y);

    /* Dibujar flota inicial */
    dibujarFlota();

    fflush(stdout);
    int r = 0;

    /* Bucle principal del juego */
    int game_running = 1;

    while (game_running) {
        /* 1. ACTUALIZAR FLOTA */
        if (flotaDebeMoverse()) {
            borrarFlota();
            moverFlota();
            dibujarFlota();
        }

        /* 2. ACTUALIZAR OVNI */
        actualizarOvniConDibujo();

        /* 3. MOVER NAVE SEGÚN INPUT */
        moverNave(&nave, r);
        
        /* 3b. ACTUALIZAR BALAS */
        if (balasDebenMoverse(bala_delay, &bala_previous)) {
            updateBala(BALA_JUGADOR);
        }

        /* 4. VERIFICAR GAME OVER */
        if (flotaLlegoAbajo()) {
            return 0;  /* Perdió */
        }

        /* 5. VERIFICAR SI GANÓ */
        if (flota_total_vivos <= 0) {
            return 1;  /* Ganó */
        }

        fflush(stdout);
    }

    return 0;
}

/******************************************************************************/
/*                           MAIN                                             */
/******************************************************************************/

int main(void) {
    setlocale(LC_ALL, "");
    srand(time(0));

    int salir = 0;
    while (!salir) {
        /* 1. PANTALLA DE INICIO */
        if (!mostrarPantallaInicio()) {
            salir = 1;
            break;
        }

        /* 2. EJECUTAR JUEGO */
        int resultado = ejecutarJuego();

        /* 3. PANTALLA DE FIN */
        int continuar;
        if (resultado == 1) {
            continuar = mostrarPantallaGanaste();
        } else {
            continuar = mostrarPantallaGameOver();
        }

        if (!continuar) {
            salir = 1;
        }
    }

    /* Restaurar terminal */
    Clear_Screen();
    Cursor(1);
    gotoxy(1, 1);

    return 0;
}

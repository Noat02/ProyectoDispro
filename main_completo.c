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
#include "explosion_draw.h"

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
bool ovni_fue_destruido = false;  /* Flag para saber si el ovni fue destruido */

/* BALAS */
float bala_delay;
float bala_previous;
float bala_alien_delay;
float bala_alien_previous;

/* AGUJEROS/BARRERAS */
int fases_agujeros[NUM_BARRERAS];

/* Nave */
Nave nave;

/* Score */
int score = 0;

/******************************************************************************/
/*                           FUNCIONES DE HUD                                 */
/******************************************************************************/

/* Prototipo */
void draw_score_hud(int x, int y, const char *label);

void actualizarScoreDisplay(void) {
    char scoreStr[30];
    sprintf(scoreStr, "SCORE <1>:%06d", score);
    draw_score_hud(SCORE_X, 2, scoreStr);
    fflush(stdout);
}

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
        fases_agujeros[i] = AGUJERO_FASE_INICIAL;
        int x = AGUJERO_START_X + (i * AGUJERO_SPACING_X);
        draw_agujero(x, AGUJERO_Y, AGUJERO_FASE_INICIAL);
    }
}

int obtenerAltoAgujero(int fase) {
    switch (fase) {
        case 1: case 2: case 3: return 8;
        case 4: return 7;
        case 5: return 6;
        case 6: return 4;
        case 7: return 3;
        case 8: return 2;
        default: return 0;
    }
}

void redibujarBarrerasDondeHayBalas(void) {
    Bala *balasJugador = obtenerBalas();
    
    for (int b = 0; b < 10; b++) {
        if (balasJugador[b].estaActiva == 0) continue;
        
        int balaX = balasJugador[b].x;
        int balaY = balasJugador[b].y;
        
        /* Verificar si la bala está sobre algún agujero */
        for (int i = 0; i < NUM_BARRERAS; i++) {
            if (fases_agujeros[i] >= 9) continue; /* Agujero destruido */
            
            int agujeroX = AGUJERO_START_X + (i * AGUJERO_SPACING_X);
            int agujeroY = AGUJERO_Y;
            int agujeroAncho = 11;
            int agujeroAlto = obtenerAltoAgujero(fases_agujeros[i]);
            
            if (agujeroAlto == 0) continue;
            
            /* Verificar si la bala pasó por el área del agujero o está en ella */
            /* La bala se mueve 4 líneas hacia arriba, así que verificamos un rango más amplio */
            int balaEnAgujero = 0;
            
            /* Verificar si la X de la bala está en el rango del agujero */
            if (balaX >= agujeroX && balaX < agujeroX + agujeroAncho) {
                /* Verificar si alguna parte de la bala (actual o anterior) está en el rango Y del agujero */
                /* La bala tiene 4 líneas de altura y se mueve 4 líneas hacia arriba */
                /* Verificamos desde balaY hasta balaY + 8 (posición actual + tamaño bala + movimiento) */
                int balaYMin = balaY;
                int balaYMax = balaY + 8;  /* 4 líneas de bala + 4 de movimiento */
                int agujeroYMin = agujeroY;
                int agujeroYMax = agujeroY + agujeroAlto;
                
                /* Verificar si hay solapamiento entre los rangos Y */
                if (!(balaYMax < agujeroYMin || balaYMin > agujeroYMax)) {
                    balaEnAgujero = 1;
                }
            }
            
            /* Si la bala pasó por el agujero, redibujar el agujero */
            if (balaEnAgujero) {
                draw_agujero(agujeroX, agujeroY, fases_agujeros[i]);
            }
        }
    }
    fflush(stdout);
}

void dibujarVidas(void) {
    draw_word("LIVES:", LIVES_X, LIVES_Y, WHITE);
    for (int i = 0; i < MAX_LIVES; i++) {
        draw_nave(LIVES_X + 27 + (i * LIFE_SPACING_X), LIVES_Y - 3); 
    }
}

void actualizarVidasDisplay(void) {
    /* Mostrar explosión en la vida que se acaba de perder */
    if (nave.vidas >= 0 && nave.vidas < MAX_LIVES) {
        int x_vida_perdida = LIVES_X + 27 + (nave.vidas * LIFE_SPACING_X);
        draw_nave_explosion(x_vida_perdida, LIVES_Y - 3);
        fflush(stdout);
        usleep(300000);  /* 300ms para ver la explosión roja */
    }
    
    /* Borrar el área de vidas */
    for (int i = 0; i < MAX_LIVES; i++) {
        int x = LIVES_X + 27 + (i * LIFE_SPACING_X);
        for (int j = 0; j < 8; j++) {
            gotoxy(x, LIVES_Y - 3 + j);
            printf("        ");
        }
    }
    
    /* Dibujar solo las vidas restantes */
    for (int i = 0; i < nave.vidas && i < MAX_LIVES; i++) {
        draw_nave(LIVES_X + 27 + (i * LIFE_SPACING_X), LIVES_Y - 3);
    }
    fflush(stdout);
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

void verificarColisionAliensAgujeros(void) {
    for (int fila = 0; fila < ALIEN_ROWS; fila++) {
        for (int col = 0; col < ALIENS_PER_ROW; col++) {
            if (!aliens[fila][col].estaVivo) continue;
            
            int alienX = aliens[fila][col].x;
            int alienY = aliens[fila][col].y;
            int alienAncho = 5;  /* Ancho aproximado del alien */
            
            /* Verificar colisión con cada agujero */
            for (int i = 0; i < NUM_BARRERAS; i++) {
                if (fases_agujeros[i] >= 9) continue; /* Agujero destruido */
                
                int agujeroX = AGUJERO_START_X + (i * AGUJERO_SPACING_X);
                int agujeroY = AGUJERO_Y;
                int agujeroAncho = 11;
                int agujeroAlto = obtenerAltoAgujero(fases_agujeros[i]);
                
                if (agujeroAlto == 0) continue;
                
                /* Verificar colisión con límites corregidos */
                int colisionDetectada = 0;
                
                /* Verificar si hay solapamiento en Y primero */
                int alienYMin = alienY;
                int alienYMax = alienY + ALIEN_HEIGHT;
                int agujeroYMin = agujeroY;
                int agujeroYMax = agujeroY + agujeroAlto;
                
                if (!(alienYMin >= agujeroYMax || alienYMax <= agujeroYMin)) {
                    /* Si hay solapamiento en Y, verificar X */
                    int alienXMin = alienX;
                    int alienXMax = alienX + alienAncho;
                    int agujeroXMin = agujeroX;
                    int agujeroXMax = agujeroX + agujeroAncho;
                    
                    if (!(alienXMin >= agujeroXMax || alienXMax <= agujeroXMin)) {
                        colisionDetectada = 1;
                    }
                }
                
                if (colisionDetectada) {
                    /* Colisión detectada */
                    
                    /* Eliminar el alien */
                    aliens[fila][col].estaVivo = false;
                    flota_total_vivos--;
                    
                    /* Borrar el alien de la pantalla */
                    for (int i = 0; i < ALIEN_HEIGHT; i++) {
                        gotoxy(alienX, alienY + i);
                        printf("        ");
                    }
                    
                    /* Borrar el agujero */
                    for (int j = 0; j < 8; j++) {
                        gotoxy(agujeroX, agujeroY + j);
                        printf("               ");
                    }
                    
                    /* Degradar el agujero una fase */
                    fases_agujeros[i]++;
                    
                    /* Redibujar el agujero en su nueva fase si no está destruido */
                    if (fases_agujeros[i] < 9) {
                        draw_agujero(agujeroX, agujeroY, fases_agujeros[i]);
                    }
                    
                    fflush(stdout);
                    return; /* Salir después de procesar una colisión */
                }
            }
        }
    }
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
    /* No hacer nada si el ovni fue destruido */
    if (ovni_fue_destruido) return;
    
    if (ovni.estaActivo) {
        if (ovniDebeMoverse()) {
            borrarOvni(ovni_x_anterior);

            ovni.x += ovni.direccion * 3;

            /* Ajustar límites para que no se salga de la pantalla */
            /* Pantalla termina en 240, ovni tiene 8 de ancho */
            if (ovni.x > 232 || ovni.x < 0) {
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
            /* Ajustar posiciones iniciales para que no se salga */
            ovni.x = (ovni.direccion == 1) ? 10 : 230;
            ovni_x_anterior = ovni.x;
            ovni_previous = (float)clock() / CLOCKS_PER_SEC;
            draw_ovni(ovni.x, OVNI_POS_Y);
        }
    }
}

/******************************************************************************/
/*                      DETECCIÓN DE COLISIONES                               */
/******************************************************************************/

void verificarColisionBalasAliens(void) {
    Bala *balas = obtenerBalas();
    
    for (int b = 0; b < 10; b++) {
        if (balas[b].estaActiva == 0) continue;
        
        int balaX = balas[b].x;
        int balaY = balas[b].y;
        
        for (int fila = 0; fila < ALIEN_ROWS; fila++) {
            for (int col = 0; col < ALIENS_PER_ROW; col++) {
                if (!aliens[fila][col].estaVivo) continue;
                
                int alienX = aliens[fila][col].x;
                int alienY = aliens[fila][col].y;
                int anchoAlien = obtenerAnchoSprite(aliens[fila][col].tipoAlien);
                
                /* Verificar si la bala está dentro del área del alien */
                if (balaX >= alienX && balaX < alienX + anchoAlien &&
                    balaY >= alienY && balaY < alienY + ALIEN_HEIGHT) {
                    
                    /* Colisión detectada */
                    balas[b].estaActiva = 0;
                    
                    /* Sumar puntos del alien al score */
                    score += aliens[fila][col].puntos;
                    actualizarScoreDisplay();
                    
                    aliens[fila][col].estaVivo = false;
                    flota_total_vivos--;

                    /* Borrar la bala primero */
                    for (int i = 0; i < 4; i++) {
                        gotoxy(balaX, balaY + i);
                        printf("     ");
                    }
                    
                    /* Animación de explosión: pequeña -> grande */
                    fflush(stdout);
                    draw_smallexplosion(alienX, alienY);
                    fflush(stdout);
                    usleep(150000);  /* 150ms para ver la explosión pequeña */
                    
                    draw_bigexplosion(alienX, alienY);
                    fflush(stdout);
                    usleep(150000);  /* 150ms para ver la explosión grande */
                    
                    /* Borrar el área del alien/explosión */
                    for (int i = 0; i < ALIEN_HEIGHT; i++) {
                        gotoxy(alienX, alienY + i);
                        for (int j = 0; j < anchoAlien; j++) {
                            printf(" ");
                        }
                    }
                    fflush(stdout);
                    
                    return; /* Salir después de la primera colisión */
                }
            }
        }
    }
}

void verificarColisionBalasOvni(void) {
    if (!ovni.estaActivo) return;  /* No hay ovni en pantalla */
    
    Bala *balas = obtenerBalas();
    
    for (int b = 0; b < 10; b++) {
        if (balas[b].estaActiva == 0) continue;
        
        int balaX = balas[b].x;
        int balaY = balas[b].y;
        int ovniX = ovni.x;
        int ovniY = OVNI_POS_Y;
        int ovniAncho = 8;  /* Ancho del sprite del ovni */
        int ovniAlto = 4;   /* Alto del sprite del ovni */
        
        /* Verificar si la bala está dentro del área del ovni */
        if (balaX >= ovniX && balaX < ovniX + ovniAncho &&
            balaY >= ovniY && balaY < ovniY + ovniAlto) {
            
            /* Colisión detectada */
            balas[b].estaActiva = 0;
            
            /* Sumar puntos del ovni al score (el ovni vale más, típicamente 100-300 puntos) */
            int puntosOvni = 100 + (rand() % 201);  /* Entre 100 y 300 puntos aleatorios */
            score += puntosOvni;
            actualizarScoreDisplay();
            
            /* Desactivar el ovni permanentemente */
            ovni.estaActivo = false;
            ovni_fue_destruido = true;
            
            /* Borrar la bala */
            for (int i = 0; i < 4; i++) {
                gotoxy(balaX, balaY + i);
                printf("     ");
            }
            
            /* Mostrar explosión en el ovni */
            draw_bigexplosion(ovniX, ovniY);
            fflush(stdout);
            usleep(200000);  /* 200ms para ver la explosión */
            
            /* Borrar el ovni y la explosión */
            for (int i = 0; i < 4; i++) {
                gotoxy(ovniX, ovniY + i);
                printf("          ");  /* 10 espacios para borrar completamente */
            }
            
            /* Mostrar los puntos obtenidos temporalmente */
            gotoxy(ovniX, ovniY + 1);
            printf("\033[33m%d\033[0m", puntosOvni);  /* Puntos en amarillo */
            fflush(stdout);
            usleep(500000);  /* 500ms para ver los puntos */
            
            /* Borrar los puntos */
            gotoxy(ovniX, ovniY + 1);
            printf("     ");
            fflush(stdout);
            
            return; /* Salir después de la colisión */
        }
    }
}

void verificarColisionBalasAliensNave(void) {
    Bala *balasAliens = obtenerBalasAliens();
    
    for (int b = 0; b < 20; b++) {
        if (balasAliens[b].estaActiva == 0) continue;
        
        int balaX = balasAliens[b].x;
        int balaY = balasAliens[b].y;
        
        /* Verificar si la bala está dentro del área de la nave */
        /* La nave tiene aproximadamente 8 unidades de ancho y 8 de alto */
        if (balaX >= nave.x && balaX < nave.x + 8 &&
            balaY >= NAVE_Y && balaY < NAVE_Y + 8) {
            
            /* Colisión detectada */
            balasAliens[b].estaActiva = 0;
            
            /* Borrar la bala */
            for (int i = 0; i < 4; i++) {
                gotoxy(balaX, balaY + i);
                printf("     ");
            }
            fflush(stdout);
            
            /* Quitar una vida a la nave */
            nave.vidas--;
            
            /* Mostrar explosión en la nave */
            draw_nave_explosion(nave.x, NAVE_Y);
            fflush(stdout);
            usleep(200000);  
            
            /* Actualizar display de vidas */
            actualizarVidasDisplay();
            
            /* Redibujar la nave si todavía tiene vidas */
            if (nave.vidas > 0) {
                draw_nave(nave.x, NAVE_Y);
                nave.estaViva = true;
            } else {
                /* Borrar la nave */
                for (int i = 0; i < 8; i++) {
                    gotoxy(nave.x, NAVE_Y + i);
                    printf("        ");
                }
                nave.estaViva = false;
            }
            
            fflush(stdout);
            return; /* Salir después de la primera colisión */
        }
    }
}

void verificarColisionBalasAliensAgujeros(void) {
    Bala *balasAliens = obtenerBalasAliens();
    
    for (int b = 0; b < 20; b++) {
        if (balasAliens[b].estaActiva == 0) continue;
        
        int balaX = balasAliens[b].x;
        int balaY = balasAliens[b].y;
        
        /* Verificar colisión con cada agujero */
        for (int i = 0; i < NUM_BARRERAS; i++) {
            int fase_actual = fases_agujeros[i];
            
            /* Solo verificar si el agujero no está completamente destruido */
            if (fase_actual >= 9) continue;
            
            int agujeroX = AGUJERO_START_X + (i * AGUJERO_SPACING_X);
            int agujeroY = AGUJERO_Y;
            int agujeroAncho = 11;  /* Ancho del sprite del agujero */
            int agujeroAlto = obtenerAltoAgujero(fase_actual);
            
            if (agujeroAlto == 0) continue;
            
            /* Verificar colisión con límites mejorados */
            /* La bala tiene 1 carácter de ancho y 4 líneas de altura */
            /* La bala se mueve 2 líneas hacia abajo por frame */
            int balaColisiona = 0;
            
            /* Primero verificar rango Y: la bala debe estar en el área vertical del agujero */
            int balaYMin = balaY;
            int balaYMax = balaY + 6;  /* 4 líneas de altura + 2 de movimiento */
            int agujeroYMin = agujeroY;
            int agujeroYMax = agujeroY + agujeroAlto;
            
            /* Si hay solapamiento en Y, verificar X */
            if (!(balaYMin > agujeroYMax || balaYMax < agujeroYMin)) {
                /* Verificar rango X: el agujero tiene formas irregulares en cada línea Y */
                /* Por lo tanto, usamos todo el ancho del sprite (11 caracteres) + margen amplio */
                /* Rango permisivo: desde X-2 hasta X+13 para capturar cualquier parte del agujero */
                if (balaX >= agujeroX - 2 && balaX <= agujeroX + agujeroAncho + 2) {
                    balaColisiona = 1;
                }
            }
            
            if (balaColisiona) {
                /* Colisión detectada */
                balasAliens[b].estaActiva = 0;
                
                /* Borrar la bala */
                for (int j = 0; j < 4; j++) {
                    gotoxy(balaX, balaY + j);
                    printf("     ");
                }
                
                /* Borrar el agujero actual completamente */
                for (int j = 0; j < 8; j++) {
                    gotoxy(agujeroX, agujeroY + j);
                    printf("               ");  /* 15 espacios */
                }
                
                /* Avanzar a la siguiente fase del agujero */
                fases_agujeros[i]++;
                
                /* Redibujar inmediatamente el agujero en su nueva fase */
                if (fases_agujeros[i] < 9) {
                    draw_agujero(agujeroX, agujeroY, fases_agujeros[i]);
                }
                
                fflush(stdout);
                return; /* Salir después de la primera colisión */
            }
        }
    }
}

void aliensDisparan(void) {
    /* Probabilidad de disparo: 1 en 100000 */
    if ((rand() % 100000) != 0) return;
    
    /* Encontrar una columna aleatoria que tenga aliens vivos */
    int columnas_disponibles[ALIENS_PER_ROW];
    int num_columnas = 0;
    
    for (int col = 0; col < ALIENS_PER_ROW; col++) {
        /* Buscar si hay al menos un alien vivo en esta columna */
        for (int fila = 0; fila < ALIEN_ROWS; fila++) {
            if (aliens[fila][col].estaVivo) {
                columnas_disponibles[num_columnas] = col;
                num_columnas++;
                break;
            }
        }
    }
    
    if (num_columnas == 0) return; /* No hay aliens vivos */
    
    /* Seleccionar una columna aleatoria */
    int col_seleccionada = columnas_disponibles[rand() % num_columnas];
    
    /* Encontrar el alien más abajo en esa columna */
    int fila_mas_abajo = -1;
    for (int fila = ALIEN_ROWS - 1; fila >= 0; fila--) {
        if (aliens[fila][col_seleccionada].estaVivo) {
            fila_mas_abajo = fila;
            break;
        }
    }
    
    if (fila_mas_abajo != -1) {
        /* Hacer que el alien dispare */
        int alienX = aliens[fila_mas_abajo][col_seleccionada].x;
        int alienY = aliens[fila_mas_abajo][col_seleccionada].y;
        iniciacionbalaAlien(alienX + 2, alienY + ALIEN_HEIGHT);
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
    inicializarBalasAliens();
    
    ovni_x_anterior = -100;
    ovni_delay = 0.05f;
    ovni_previous = (float)clock() / CLOCKS_PER_SEC;
    ovni_fue_destruido = false;  /* Resetear el flag al inicio de cada partida */
    
    bala_delay = 0.08f;
    bala_previous = (float)clock() / CLOCKS_PER_SEC;
    
    bala_alien_delay = 0.05f;
    bala_alien_previous = (float)clock() / CLOCKS_PER_SEC;

    /* Posición inicial de la nave */
    nave.x = NAVE_X;
    
    /* Inicializar score */
    score = 0;

    /* Dibujar elementos estáticos */
    dibujarHUD();
    actualizarScoreDisplay();
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

        /* 2d. VERIFICAR COLISIONES ALIENS-AGUJEROS */
        verificarColisionAliensAgujeros();

        /* 3. MOVER NAVE SEGÚN INPUT */
        moverNave(&nave, r);
        
        /* 3b. ACTUALIZAR BALAS DEL JUGADOR */
        if (balasDebenMoverse(bala_delay, &bala_previous)) {
            updateBala(BALA_JUGADOR);
            redibujarBarrerasDondeHayBalas(); /* Redibujar agujeros que fueron borrados por las balas */
        }
        
        /* 3c. VERIFICAR COLISIONES BALAS JUGADOR-ALIENS (las balas pasan a través de los agujeros) */
        verificarColisionBalasAliens();
        
        /* 3d. VERIFICAR COLISIONES BALAS JUGADOR-OVNI */
        verificarColisionBalasOvni();
        
        /* 3e. ALIENS DISPARAN */
        aliensDisparan();
        
        /* 3f. ACTUALIZAR BALAS DE ALIENS */
        if (balasDebenMoverse(bala_alien_delay, &bala_alien_previous)) {
            updateBalasAliens();
        }
        
        /* 3g. VERIFICAR COLISIONES BALAS ALIENS-AGUJEROS (con daño) */
        verificarColisionBalasAliensAgujeros();
        
        /* 3h. VERIFICAR COLISIONES BALAS ALIENS-NAVE */
        verificarColisionBalasAliensNave();

        /* 4. VERIFICAR GAME OVER */
        if (flotaLlegoAbajo()) {
            return 0;  /* Perdió */
        }
        
        /* Verificar si la nave se quedó sin vidas */
        if (nave.vidas <= 0 || !nave.estaViva) {
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

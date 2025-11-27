# Makefile para Space Invaders

CC = gcc
CFLAGS = -Wall -Wextra

# Archivos fuente
SRCS = main_completo.c \
       conio.c \
       aliens_draw.c \
       nave_draw.c \
       ovni_draw.c \
       agujero_draw.c \
       simbolos_draw.c \
       alien.c \
       ovni.c \
       nave.c \
       input.c \
       pantallas.c \
       bala.c

# Nombre del ejecutable
TARGET = space_invaders

# Regla principal
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Limpiar
clean:
	rm -f $(TARGET) *.o

# Ejecutar
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

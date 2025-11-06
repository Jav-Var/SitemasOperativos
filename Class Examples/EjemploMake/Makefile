# --- Variables de Compilación ---
# CC: El compilador que usaremos
CC = gcc
# CFLAGS: Flags para compilar.
# -Wall (Warning ALL) -> Muestra todos los warnings. Indispensable.
# -g          -> Incluye símbolos de debug (para usar gdb)
# -std=c11    -> (Opcional) Usar el estándar C11
CFLAGS = -Wall -g -std=c11

# LDFLAGS: Flags para enlazar (linkear), ej. librerías
# -lm (para la librería matemática, si la necesitas)
LDFLAGS = -lm

# --- Objetivos (Binarios finales) ---
# Los programas que queremos construir
TARGETS = servidor cliente

# --- Archivos Fuente (.c) y Objeto (.o) ---
# Fuentes del servidor
SRCS_SERV = servidor.c utils.c
OBJS_SERV = $(SRCS_SERV:.c=.o) # Sustitución mágica: servidor.c utils.c -> servidor.o utils.o

# Fuentes del cliente
SRCS_CLI = cliente.c utils.c
OBJS_CLI = $(SRCS_CLI:.c=.o) # cliente.c utils.c -> cliente.o utils.o


# --- Regla Principal ---
# 'all' es el objetivo por defecto (lo que se ejecuta si solo escribes 'make')
# .PHONY le dice a make que 'all' no es un archivo real.
.PHONY: all
all: $(TARGETS)


# --- Reglas de Enlazado (Linking) ---
# Para crear el binario 'servidor', necesitas sus .o
# $@ -> Variable automática: "El Objetivo" (servidor)
# $^ -> Variable automática: "Todas las Dependencias" (servidor.o utils.o)
servidor: $(OBJS_SERV)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Para crear el binario 'cliente', necesitas sus .o
cliente: $(OBJS_CLI)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)


# --- Regla Patrón de Compilación (Compila .c a .o) ---
# Esta es la regla "mágica" que evita repetir.
# "Para crear cualquier archivo .o (ej. utils.o),
#  busca su .c (utils.c) y ejecuta este comando"
# $< -> Variable automática: "La primera dependencia" (utils.c)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


# --- Regla de Limpieza ---
# Se usa con 'make clean' para borrar los compilados
.PHONY: clean
clean:
	rm -f *.o $(TARGETS)
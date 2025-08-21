# Diretórios
SRC_DIR = src
INC_DIR = include
OUT_DIR = output

# Compilador e flags
CC = gcc
CFLAGS = -Wall -I$(INC_DIR)

# Nome do executável
TARGET = $(OUT_DIR)/Loucacao.exe

# Fontes e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)

# Regra padrão
all: $(OUT_DIR) $(TARGET)

# Gera o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compila arquivos .c em .o
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cria o diretório de output se não existir
$(OUT_DIR):
	if not exist $(OUT_DIR) mkdir $(OUT_DIR)

# Limpa os objetos e o executável (compatível com cmd do Windows)
clean:
	del /Q $(OUT_DIR)\*.o $(OUT_DIR)\*.exe 2>nul || echo Nothing to clean

# Remove completamente a pasta de output (Windows)
reset:
	rmdir /S /Q $(OUT_DIR) 2>nul || echo Nothing to reset

.PHONY: all clean reset

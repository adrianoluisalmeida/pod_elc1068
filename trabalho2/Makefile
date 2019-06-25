# Compilador
CC = gcc

# Opções para o compilador e alers
CFLAGS = -Wall -g

# Comando para apagar itens no caso da execução do clean
RM = rm -f

# Arquivos necessários para a complicação
OBJS = main.c

# Nome final do executável
MAIN = trabalho2

# Compilação e ligação de dependências
$(MAIN): $(OBJS)
	@echo ""
	@echo "Compilando Programa .."
	@$(CC) $(CFLAGS) $(OBJS) -lm -o $(MAIN)
	@echo ""

%.o: %.c %.h
	@echo ""
	@echo "Compilando objeto \"$@\""
	@$(CC) $(CFLAGS) $< -c 

clean:
	$(RM) $(MAIN) *.o
	$(RM) output_*
	clear

run: $(MAIN)
	./$(MAIN)

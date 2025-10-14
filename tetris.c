#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5  // Tamanho fixo da fila

// Códigos ANSI para cores
#define AZUL    "\x1b[34m"
#define AMARELO "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define VERDE   "\x1b[32m"
#define RESET   "\x1b[0m"

// =======================================
// Estrutura que representa uma peça
// =======================================
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// =======================================
// Estrutura da fila circular
// =======================================
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// =======================================
// Protótipos das funções
// =======================================
void inicializarFila(Fila *fila);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
void enfileirar(Fila *fila, Peca peca);
Peca desenfileirar(Fila *fila);
void exibirFila(Fila *fila);
Peca gerarPeca(int id);
const char* corPeca(char tipo);

// =======================================
// Função principal
// =======================================
int main() {
    Fila fila;
    int opcao;
    int contadorID = 0; // Controla IDs únicos das peças

    srand(time(NULL)); // Semente para aleatoriedade

    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    printf("\n=== TETRIS STACK - SIMULAÇÃO DA FILA DE PEÇAS ===\n");

    do {
        exibirFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n➡️  Peça jogada: %s[%c %d]%s\n", 
                           corPeca(jogada.nome), jogada.nome, jogada.id, RESET);
                } else {
                    printf("\n⚠️  A fila está vazia! Nenhuma peça para jogar.\n");
                }
                break;

            case 2:
                if (!filaCheia(&fila)) {
                    Peca nova = gerarPeca(contadorID++);
                    enfileirar(&fila, nova);
                    printf("\n✅ Nova peça adicionada: %s[%c %d]%s\n",
                           corPeca(nova.nome), nova.nome, nova.id, RESET);
                } else {
                    printf("\n⚠️  A fila está cheia! Não é possível adicionar.\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\n❌ Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// =======================================
// Funções auxiliares
// =======================================

void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = -1;
    fila->tamanho = 0;
}

int filaVazia(Fila *fila) {
    return (fila->tamanho == 0);
}

int filaCheia(Fila *fila) {
    return (fila->tamanho == TAM_FILA);
}

void enfileirar(Fila *fila, Peca peca) {
    if (filaCheia(fila)) return;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->itens[fila->fim] = peca;
    fila->tamanho++;
}

Peca desenfileirar(Fila *fila) {
    Peca removida = {'?', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->tamanho--;
    return removida;
}

void exibirFila(Fila *fila) {
    printf("\nFila de peças: ");
    if (filaVazia(fila)) {
        printf("[vazia]\n");
        return;
    }
    int i = fila->inicio;
    for (int count = 0; count < fila->tamanho; count++) {
        Peca p = fila->itens[i];
        printf("%s[%c %d]%s ", corPeca(p.nome), p.nome, p.id, RESET);
        i = (i + 1) % TAM_FILA;
    }
    printf("\n");
}

Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// Retorna a cor ANSI correspondente à peça
const char* corPeca(char tipo) {
    switch(tipo) {
        case 'I': return AZUL;
        case 'O': return AMARELO;
        case 'T': return MAGENTA;
        case 'L': return VERDE;
        default: return RESET;
    }
}
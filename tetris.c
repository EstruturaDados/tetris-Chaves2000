#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===============================
// CONFIGURAÇÕES
// ===============================
#define TAM_FILA 5
#define TAM_PILHA 3

// Cores ANSI
#define AZUL    "\x1b[34m"
#define AMARELO "\x1b[33m"
#define MAGENTA "\x1b[35m"
#define VERDE   "\x1b[32m"
#define RESET   "\x1b[0m"

// ===============================
// ESTRUTURAS
// ===============================
typedef struct {
    char nome; // Tipo da peça (I, O, T, L)
    int id;    // Identificador único
} Peca;

typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ===============================
// PROTÓTIPOS
// ===============================
void inicializarFila(Fila *fila);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
void enfileirar(Fila *fila, Peca peca);
Peca desenfileirar(Fila *fila);

void inicializarPilha(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void empilhar(Pilha *pilha, Peca peca);
Peca desempilhar(Pilha *pilha);

Peca gerarPeca(int id);
const char* corPeca(char tipo);

void exibirEstado(Fila *fila, Pilha *pilha);

// ===============================
// FUNÇÃO PRINCIPAL
// ===============================
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(contadorID++));
    }

    printf("\n=== TETRIS STACK - GERENCIAMENTO DE PEÇAS ===\n");

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n➡️  Peça jogada: %s[%c %d]%s\n", 
                           corPeca(jogada.nome), jogada.nome, jogada.id, RESET);
                    // Gera nova peça e mantém fila cheia
                    enfileirar(&fila, gerarPeca(contadorID++));
                } else {
                    printf("\n⚠️  Fila vazia! Nenhuma peça para jogar.\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha cheia! Não é possível reservar mais peças.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça reservada: %s[%c %d]%s\n",
                           corPeca(reservada.nome), reservada.nome, reservada.id, RESET);
                    enfileirar(&fila, gerarPeca(contadorID++)); // repõe na fila
                }
                break;

            case 3: // Usar peça reservada
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("\n🧩 Peça usada da reserva: %s[%c %d]%s\n",
                           corPeca(usada.nome), usada.nome, usada.id, RESET);
                } else {
                    printf("\n⚠️  Pilha de reserva vazia!\n");
                }
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ===============================
// FUNÇÕES DE FILA
// ===============================
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

// ===============================
// FUNÇÕES DE PILHA
// ===============================
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaVazia(Pilha *pilha) {
    return (pilha->topo == -1);
}

int pilhaCheia(Pilha *pilha) {
    return (pilha->topo == TAM_PILHA - 1);
}

void empilhar(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) return;
    pilha->itens[++pilha->topo] = peca;
}

Peca desempilhar(Pilha *pilha) {
    Peca removida = {'?', -1};
    if (pilhaVazia(pilha)) return removida;
    removida = pilha->itens[pilha->topo--];
    return removida;
}

// ===============================
// GERAÇÃO E EXIBIÇÃO
// ===============================
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

const char* corPeca(char tipo) {
    switch(tipo) {
        case 'I': return AZUL;
        case 'O': return AMARELO;
        case 'T': return MAGENTA;
        case 'L': return VERDE;
        default: return RESET;
    }
}

// Exibe o estado completo do jogo
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("\n==================== ESTADO ATUAL ====================\n");

    // Exibe Fila
    printf("Fila de peças:\t");
    if (filaVazia(fila)) printf("[vazia]");
    else {
        int i = fila->inicio;
        for (int count = 0; count < fila->tamanho; count++) {
            Peca p = fila->itens[i];
            printf("%s[%c %d]%s ", corPeca(p.nome), p.nome, p.id, RESET);
            i = (i + 1) % TAM_FILA;
        }
    }
    printf("\n");

    // Exibe Pilha
    printf("Pilha de reserva\t(Topo -> Base): ");
    if (pilhaVazia(pilha)) printf("[vazia]");
    else {
        for (int i = pilha->topo; i >= 0; i--) {
            Peca p = pilha->itens[i];
            printf("%s[%c %d]%s ", corPeca(p.nome), p.nome, p.id, RESET);
        }
    }

    printf("\n======================================================\n");
}
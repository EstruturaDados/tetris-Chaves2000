#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// ==========================================
// Estrutura de uma peça
// ==========================================
typedef struct {
    char tipo;  // Tipo da peça (I, O, T, L)
    int id;     // Identificador único
} Peca;

// ==========================================
// Estruturas de Fila Circular e Pilha
// ==========================================
typedef struct {
    Peca elementos[TAM_FILA];
    int inicio, fim, tamanho;
} Fila;

typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// ==========================================
// Protótipos de Funções
// ==========================================
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca p);
Peca desempilhar(Pilha *p);
void gerarPeca(Peca *p, int *contador);
void exibirEstado(Fila *f, Pilha *p);
void trocarTopo(Fila *f, Pilha *p);
void trocaMultipla(Fila *f, Pilha *p);

// ==========================================
// Implementações
// ==========================================
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = -1;
    f->tamanho = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->elementos[f->fim] = p;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (filaVazia(f)) return p;
    p = f->elementos[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return p;
}

void empilhar(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->topo++;
    p->elementos[p->topo] = peca;
}

Peca desempilhar(Pilha *p) {
    Peca peca = {'-', -1};
    if (pilhaVazia(p)) return peca;
    peca = p->elementos[p->topo];
    p->topo--;
    return peca;
}

void gerarPeca(Peca *p, int *contador) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    p->tipo = tipos[rand() % 4];
    p->id = (*contador)++;
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n======================================\n");
    printf("Estado atual:\n");
    printf("Fila de peças\t: ");
    int i, idx;
    for (i = 0; i < f->tamanho; i++) {
        idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->elementos[idx].tipo, f->elementos[idx].id);
    }
    printf("\nPilha de reserva\t(Topo -> Base): ");
    for (i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->elementos[i].tipo, p->elementos[i].id);
    }
    printf("\n======================================\n");
}

// Troca a peça da frente da fila com o topo da pilha
void trocarTopo(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("❌ Não é possível trocar — fila ou pilha vazia.\n");
        return;
    }
    int idx = f->inicio;
    Peca temp = f->elementos[idx];
    f->elementos[idx] = p->elementos[p->topo];
    p->elementos[p->topo] = temp;
    printf("🔁 Troca entre topo da pilha e frente da fila realizada!\n");
}

// Troca múltipla: 3 primeiros da fila ↔ 3 da pilha
void trocaMultipla(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("❌ É necessário ter pelo menos 3 peças em cada estrutura.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        Peca temp = f->elementos[idx];
        f->elementos[idx] = p->elementos[p->topo - i];
        p->elementos[p->topo - i] = temp;
    }
    printf("🔄 Troca múltipla entre fila e pilha concluída!\n");
}

// ==========================================
// Função principal
// ==========================================
int main() {
    Fila fila;
    Pilha pilha;
    int contador = 0;
    int opcao;
    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova;
        gerarPeca(&nova, &contador);
        enfileirar(&fila, nova);
    }

    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Reservar (mover da fila para pilha)\n");
        printf("3 - Usar peça reservada (remover topo da pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiras da fila com 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca usada = desenfileirar(&fila);
                    printf("🧩 Peça [%c %d] jogada!\n", usada.tipo, usada.id);

                    // Gera nova peça para manter fila cheia
                    Peca nova;
                    gerarPeca(&nova, &contador);
                    enfileirar(&fila, nova);
                } else {
                    printf("❌ Fila vazia!\n");
                }
                break;
            }
            case 2: {
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("📦 Peça [%c %d] movida para a pilha!\n", reservada.tipo, reservada.id);

                    // Gera nova peça para manter fila cheia
                    Peca nova;
                    gerarPeca(&nova, &contador);
                    enfileirar(&fila, nova);
                } else {
                    printf("❌ Não foi possível reservar — fila vazia ou pilha cheia.\n");
                }
                break;
            }
            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("🧱 Peça [%c %d] usada da pilha!\n", usada.tipo, usada.id);
                } else {
                    printf("❌ Pilha vazia!\n");
                }
                break;
            }
            case 4:
                trocarTopo(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("🚪 Encerrando o jogo...\n");
                break;
            default:
                printf("❌ Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
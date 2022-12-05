#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINHAS 10000
#define MAX_COLUNAS 10000

/* Estrutura de um nó de lista */
typedef struct no_ NO;
struct no_ {
    double chave;       // chave do item
    int linha;          // linha do item na ME
    int coluna;         // coluna do item na ME
    NO* proximo;        // ponteiro para o próximo da lista
};

/* Estrutura de uma lista */
typedef struct {
    NO *inicio;         // ponteiro para o nó de início da lista
    int tamanho;        // quantidade de elementos da lista
} LISTA;

/* Estrutura da matriz esparsa */
typedef struct {
    LISTA **listas;     // arranjo de ponteiros para as listas da matriz esparsa
    int nLinhas;        // quantidade de linhas da matriz
    int nColunas;       // quantidade de colunas da matriz
    double valorPadrao;  // valor padrão (repetido pelo menos em 90% dos casos)
} ME;


/* Cria um novo nó para a matriz esparsa, dados sua chave, linha e coluna */
NO *lista_criar_no(double chave, int linha, int coluna) {
    
    // Alocação do novo nó:
    NO *novoNo = malloc(sizeof(NO));
    
    // Inicialização dos atributos se a alocação foi válida (não nula)
    if (novoNo != NULL) {
        novoNo->chave = chave;
        novoNo->linha = linha;
        novoNo->coluna = coluna;
        novoNo->proximo = NULL;
    }

    // Retorno do ponteiro do nó:
    return novoNo;
}


/* Cria uma nova lista (linha da matriz esparsa) */
LISTA *lista_criar(void) {

    // Alocação da nova lista:
    LISTA *novaLista = malloc(sizeof(LISTA));

    // Inicialização dos atributos se a alocação foi válida (não nula)
    if (novaLista != NULL) {
        novaLista->inicio = NULL;
        novaLista->tamanho = 0;
    }

    // Retorno do ponteiro da lista:
    return novaLista;
}


/* Cria um novo item com a chave passada por parâmetro e o insere na posição da coluna passada por parâmetro */
bool lista_inserir(LISTA *lista, double chave, int linha, int coluna) {

    // Prevenção de segmentation fault caso o ponteiro para a lista for inválido:
    if (lista == NULL)
        return false;
    
    // Inicialização dos nós atual e anterior da lista:
    NO *noAtual = lista->inicio;
    NO *noAnterior = NULL;
    
    // Percurso da lista até encontrar a posição correta de inserção:
    while (noAtual != NULL && noAtual->coluna < coluna) {   // enquanto for necessário colocar a nova chave mais à direita
        noAnterior = noAtual;                               // o nó anterior passa ser o atual
        noAtual = noAtual->proximo;                         // o nó atual recebe o próximo
    }

    // Inserção do elemento na lista (em algum dos if elses, dependendo do cenário):

    if (noAnterior == NULL) {                               // se a lista não tiver nenhum nó
        NO *noNovo = lista_criar_no(chave, linha, coluna);  // um novo nó é criado
        noNovo->proximo = lista->inicio;                    // o próximo nó recebe o inícial (nulo)
        lista->inicio = noNovo;                             // o início da lista é o novo nó criado
    }
    else {
        if (noAtual == NULL) {                              // se a chave a ser inserida for a última da lista
            noAtual = lista_criar_no(chave, linha, coluna); // um novo nó é criado
            noAnterior->proximo = noAtual;                  // o próximo nó do antigo último recebe o atual
        }
        else {                                                  // se a chave a ser inserida estiver no meio da lista
            NO *noNovo = lista_criar_no(chave, linha, coluna);  // um novo nó é criado
            noAnterior->proximo = noNovo;                       // o próximo nó do anterior é o novo
            noNovo->proximo = noAtual;                          // o próximo nó do novo é o atual 
        }
    }

    // Incremento no tamanho da lista
    lista->tamanho += 1;

    // Retorno verdadeiro (inserção com sucesso):
    return true;
}


/* Apaga todos os nós de uma lista recursivamente */
void lista_apagar_nos(NO **no) {

    if (*no != NULL) {                      // se o ponteiro para o nó for válido
        lista_apagar_nos(&(*no)->proximo);  // a função é chamada para o próximo nó
        if ((*no)->proximo == NULL) {       // se o próximo nó é nulo (se este é um nó final)
            free(*no);                      // o nó é liberado
            *no = NULL;                     // o nó passa a ser nulo
        } 
    }
}


/* Apaga uma lista (linha da matriz esparsa) */
bool lista_apagar(LISTA **lista) {
    
    if (*lista != NULL) {                       // se o ponteiro para a lista for válido
        lista_apagar_nos(&(*lista)->inicio);    // os nós são apagados pela função recursiva
        free(*lista);                           // a lista é liberada
        *lista = NULL;                          // a lista aponta para nulo
        return true;                            // é retornado true (deleção com sucesso)
    }

    return false;                               // se o ponteiro não for válido, é retornado false (deleção sem sucesso)
}


/* Imprime as chaves de uma lista*/
void lista_imprimir(LISTA *lista, FILE *fp) {

    // Abortagem da função caso o ponteiro para a lista seja inválido:
    if (lista == NULL)
        return;

    // Inicialização do nó atual (início da lista):
    NO *noAtual = lista->inicio;

    // Impressão das chaves;
    while (noAtual != NULL) {                  // enquanto não tiver chegado o fim da lista:
        fprintf(fp, "%.2lf %d %d ", noAtual->chave, noAtual->linha, noAtual->coluna);  // o nó atual é impresso
        noAtual = noAtual->proximo;            // o nó atual passa a ser o próximo
    }
}


/* Cria uma nova matriz esparsa */
ME *matriz_criar(int nLinhas, int nColunas, double valorPadrao) {

    // Abortagem da função caso sejam passadas dimensões inválidas:
    if(nLinhas < 0 || nColunas < 0) {
        fprintf(stderr, "Impossível criar matriz com dimensões negativas\n");
        return NULL;
    }

    // Alocação da nova matriz esparsa:
    ME *novaMatriz = malloc(sizeof(ME));

    // Inicialização dos atributos caso a alocação for válida:
    if (novaMatriz != NULL) {
        novaMatriz->listas = malloc(nLinhas * sizeof(LISTA *));    // alocação do conjunto de linhas
        for(int i = 0; i < nLinhas; i++)
            novaMatriz->listas[i] = lista_criar();                 // alocação das linhas individuais
        novaMatriz->nLinhas = nLinhas;
        novaMatriz->nColunas = nColunas;
        novaMatriz->valorPadrao = valorPadrao;
    }

    // Retorno da nova matriz criada:
    return novaMatriz;
}


/* Apaga uma matriz esparsa */
bool matriz_apagar(ME **matriz) {

    if (*matriz != NULL) {                                              // se o ponteiro para a matriz é válido
        for(int i = 0, nLinhas = (*matriz)->nLinhas; i < nLinhas; i++)  
            lista_apagar(&(*matriz)->listas[i]);                        // as linhas são liberadas
        free((*matriz)->listas);                                        // o conjunto de linhas é liberado
        free(*matriz);                                                  // a matriz é liberada
        *matriz = NULL;                                                 // a matriz aponta para nulo
        return true;                                                    // é retornado verdadeiro (deleção com sucesso)
    }

    return false;                                                       // se o ponteiro não for válido, é retornado false (deleção sem sucesso)  
}


/* Insere uma chave na matriz esparsa */
bool matriz_inserir(ME *matriz, int linha, int coluna, double chave) {

    // Abortagem dos casos em que a inserção é inválida:
    if(matriz == NULL || linha < 0 || coluna < 0 || linha >= matriz->nLinhas || coluna >= matriz->nColunas)
        return false;
    
    // Desconsideração caso a chave seja o valor padrão:
    if (chave == matriz->valorPadrao)
        return true;    // a inserção é válida, mas o valor não é inserido fisicamente

    // Inserção na lista correspondente caso contrário:
    return lista_inserir(matriz->listas[linha], chave, linha, coluna);
}


void matriz_imprimir(ME *matriz, FILE *fp) {

    // Abortagem caso a matriz tiver um ponteiro nulo:
    if (matriz == NULL)
        return;

    for (int i = 0; i < matriz->nLinhas; i++) {
        lista_imprimir(matriz->listas[i], fp);
    }

    fprintf(fp, "\n");
}


ME *matriz_somar(ME *a, ME *b) {

    // Abortagem caso alguma matriz tenha ponteiro inválido:
    if (a == NULL || b == NULL)
        return NULL;

    // Criação da matriz soma:
    ME *c = matriz_criar(MAX_LINHAS, MAX_COLUNAS, 0);

    // Realização da soma:
    for (int i = 0; i < a->nLinhas; i++) {
        LISTA *listaA = a->listas[i], *listaB = b->listas[i];
        NO *noA = listaA->inicio, *noB = listaB->inicio;
        while (noA != NULL && noB != NULL) {            // enquanto houver chaves a serem somadas em ambas as listas
            if (noA->coluna < noB->coluna) {                            // se a coluna do nó atual da lista A for menor que a da B
                matriz_inserir(c, noA->linha, noA->coluna, noA->chave); // uma cópia do nó da lista A é atribuída à matriz A C
                noA = noA->proximo;
            }
            else if (noB->coluna < noA->coluna) {                       // se a coluna do nó atual da matriz B for menor que a da A
                matriz_inserir(c, noB->linha, noB->coluna, noB->chave); // uma cópia do nó da lista B é atribuída à matriz A C
                noB = noB->proximo;
            }
            else {                                                                  // se as colunas forem iguais  
                matriz_inserir(c, noA->linha, noA->coluna, noA->chave + noB->chave);// é atribuída a soma das chaves de A e B 
                noA = noA->proximo;
                noB = noB->proximo;
            }
        }
        while (noA != NULL) { // enquanto houver chave a ser somada da lista A
            matriz_inserir(c, noA->linha, noA->coluna, noA->chave);     // a chave de A é inserida
            noA = noA->proximo;
        }
        while (noB != NULL) { // enquanto houver chave a ser somada da lista B
            matriz_inserir(c, noB->linha, noB->coluna, noB->chave);     // a chave de B é inserida
            noB = noB->proximo;
        }
    }

    return c;
}


int main(int argc, char *argv[]) {

    // Leitura da quantidade de elementos não nulos de A e B:
    int qtdA, qtdB;
    scanf("%d %d", &qtdA, &qtdB);

    // Criação e leitura da matriz A:
    ME *matrizA = matriz_criar(MAX_LINHAS, MAX_COLUNAS, 0);
    for (int i = 0; i < qtdA; i++) {
        double chave; int linha, coluna;
        scanf("%lf %d %d", &chave, &linha, &coluna);
        matriz_inserir(matrizA, linha, coluna, chave);
    }

    // Criação e leitura da matriz B:
    ME *matrizB = matriz_criar(MAX_LINHAS, MAX_COLUNAS, 0);
    for (int i = 0; i < qtdB; i++) {
        double chave; int linha, coluna;
        scanf("%lf %d %d", &chave, &linha, &coluna);
        matriz_inserir(matrizB, linha, coluna, chave);
    }

    // Realização da soma impressão do resultado:
    ME *matrizC = matriz_somar(matrizA, matrizB);

    // Abertura do arquivo de saída:
    FILE *fp = fopen("saida.txt", "w");

    // Impressão da saída no arquivo:
    matriz_imprimir(matrizC, fp);

    // Liberação de memória:
    matriz_apagar(&matrizA);
    matriz_apagar(&matrizB);
    matriz_apagar(&matrizC);

    printf("Cheque o arquivo \"saida.txt\" para verificar a saida do programa!\n");

    return 0;
}

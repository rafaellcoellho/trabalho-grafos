/*
 * Exercicio programa da disciplina de grafos 2019.1 IFCE
 * Alunos: Adauto Pinheiro, Gabriel Palhares,
 *         Mario Matheus, Rafael Coelho e Wagner Matheus
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_VERTICES 150

// Protótipo de funções
FILE *abre_arquivo(void);
void le_arquivo(FILE *arquivo, int n_vertices, int n_arcos, int *custos);
void dijkstra(int n_vertices, int origem, int destino, int *custos);

// Rotina principal
int main()
{
    // Quantidade de vertice e arcos
    int n_vertices = 0,n_arcos = 0;
    // Vertice origem e destino
    int v_origem = 0, v_destino = 0;
    // Lista de adjacências
    int *custos  = NULL;

    FILE *arquivo = abre_arquivo();

    //Lê o arquivo
    fscanf(arquivo, "%d %d %d %d", &n_vertices, &n_arcos, &v_origem, &v_destino);

    // Inicia os custos com -1
    custos = (int*) malloc(sizeof(int) * n_vertices * n_vertices);
    for(int i = 0; i <= n_vertices * n_vertices; i++) custos[i] = -1;
    
    // Preenche os custos de acordo com o arquivo
    le_arquivo(arquivo, n_vertices, n_arcos, custos);
    fclose(arquivo);

    dijkstra(n_vertices, v_origem, v_destino, custos);

    return 0;
}

/*
 * abre_arquivo() - Abre um arquivo com a descricao de um grafo definida no escopo do trabalho.
 *
 * Retorno: Uma referencia para o arquivo lido. 
 *          Quem chama a funcao eh que libera a memória do arquivo.
 */
FILE *abre_arquivo(void)
{
    FILE *arquivo = NULL;
    char caminho[50];

    while(arquivo == NULL) {
        printf("Nome do arquivo: ");
        fgets(caminho, sizeof(caminho), stdin);
        //Remove o último caractere do caminho, pois o fgets armazena a quebra de linha '\n'
        char *p_chr = strchr(caminho, '\n'); 
        if(p_chr != NULL) *p_chr = '\0';

        arquivo = fopen(caminho, "r");
    }

    return arquivo;
}

/*
 * le_arquivo() - Le um arquivo e preenche o vetor de custo;.
 *
 * @arquivo: Ponteiro para o arquivo aberto;
 * @n_vertices: Numero de vertices do grafo;
 * @n_arcos: Numero de arcos do grafo;
 * @custos: Lista de adjacência que vai ser preenchida com os custos de cada arco;
 */
void le_arquivo(FILE *arquivo, int n_vertices, int n_arcos, int *custos)
{
    int origem, destino;
    for(int i = 0; i < n_arcos; i++) {
        fscanf(arquivo, "%d %d", &origem, &destino);
        //Armazena os custos na lista de adjacências
        fscanf(arquivo, "%d", &custos[(origem-1)*n_vertices+destino-1]);
    }
}

/*
 * dijkstra() - Apresenta na tela o caminho de menor custo em um grafo.
 *
 * @n_vertices: Numero de vertices do grafo;
 * @origem: Vertice origem
 * @destino: Vertice destino
 * @custos: Lista de adjacência com os custos de cada arco;
 */
void dijkstra(int n_vertices, int origem, int destino, int *custos)
{   
    int anterior[MAX_VERTICES];
    int fronteira[MAX_VERTICES];
    double distancia[MAX_VERTICES];

    //Variaveis auxiliares
    int i = 0;
    int v = 0;
    double min;

    //Inicialização
    for(i = 0; i < n_vertices; i++) {
        if(custos[i] != -1) {
            anterior[i] = origem-1; 
            distancia[i] = custos[i];
        } else {
            anterior[i] = -1;
            distancia[i] = INFINITY;
        }
        fronteira[i] = 0;
    }
    fronteira[origem-1] = 1;
    distancia[origem-1] = 0;

    while(v != destino-1 && min != INFINITY){
        min = INFINITY; 

        for(i = 0; i < n_vertices; i++){
            if(fronteira[i] == 0){
                if(distancia[i] >= 0 && distancia[i] < min){
                    min = distancia[i];
                    v = i;
                }
            }
        }

        //Distâncias dos novos vizinhos de fronteira
        if(v != destino-1 && min != INFINITY) {
            fronteira[v] = 1;
            for(i = 0; i < n_vertices; i++)
                if(fronteira[i] == 0) {
                    if(custos[v*n_vertices+i] != -1 && distancia[v]+custos[v*n_vertices+i] < distancia[i]) {
                        distancia[i] = distancia[v]+custos[v*n_vertices+i];
                        anterior[i] = v;
                    }
                }
        }
    }

    //Imprime caminho mínimo de origem a destino
    if(min == INFINITY) {
        printf("\nNão existe caminho entre os vértices %d e %d.\n", origem, destino);
    } else {
        printf("\nCaminho mínimo do vértice %d para o vértice %d: \n", origem, destino);

        i = destino;
        printf("%d", i);
        i = anterior[i-1];

        while(i !=-1) {
            printf(" <- %d", i+1);
            i = anterior[i];
        }
        printf("\nCusto = %d\n", (int)distancia[destino-1]);
    }
}


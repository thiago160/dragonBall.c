#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#define LINHAS 57
#define COLUNAS 200

typedef struct {
    int linha;
    int coluna;
} Coordenadas;

typedef struct {
    Coordenadas pos;
    int custo;
    int prioridade;
} No;

typedef struct {
    No* nos;
    int tamanho;
    int capacidade;
} Heap;

Coordenadas encontrar_elemento(char mapa[LINHAS][COLUNAS], char elemento) {
    Coordenadas coordenadas = {-1, -1};
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == elemento) {
                coordenadas.linha = i;
                coordenadas.coluna = j;
                return coordenadas;
            }
        }
    }
    return coordenadas;
}

// Função para calcular a distância de Manhattan
int distancia_manhattan(Coordenadas de, Coordenadas para) {
    return abs(para.linha - de.linha) + abs(para.coluna - de.coluna);
}

// Função para inicializar a heap
Heap* criar_heap(int capacidade) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->nos = (No*)malloc(capacidade * sizeof(No));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

// Função para inserir na heap
void inserir_heap(Heap* heap, No no) {
    if (heap->tamanho >= heap->capacidade) return;
    heap->nos[heap->tamanho] = no;
    int i = heap->tamanho;
    heap->tamanho++;
    while (i != 0 && heap->nos[i].prioridade < heap->nos[(i - 1) / 2].prioridade) {
        No temp = heap->nos[i];
        heap->nos[i] = heap->nos[(i - 1) / 2];
        heap->nos[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

// Função para remover o nó de menor prioridade da heap
No remover_heap(Heap* heap) {
    if (heap->tamanho <= 0) {
        No no_vazio = {{-1, -1}, INT_MAX, INT_MAX};
        return no_vazio;
    }
    if (heap->tamanho == 1) {
        heap->tamanho--;
        return heap->nos[0];
    }
    No raiz = heap->nos[0];
    heap->nos[0] = heap->nos[heap->tamanho - 1];
    heap->tamanho--;
    int i = 0;
    while (i * 2 + 1 < heap->tamanho) {
        int menor = i * 2 + 1;
        if (i * 2 + 2 < heap->tamanho && heap->nos[i * 2 + 2].prioridade < heap->nos[menor].prioridade) {
            menor = i * 2 + 2;
        }
        if (heap->nos[i].prioridade < heap->nos[menor].prioridade) break;
        No temp = heap->nos[i];
        heap->nos[i] = heap->nos[menor];
        heap->nos[menor] = temp;
        i = menor;
    }
    return raiz;
}

// Função para verificar se a heap está vazia
bool heap_vazia(Heap* heap) {
    return heap->tamanho == 0;
}

// Função para liberar a memória da heap
void liberar_heap(Heap* heap) {
    free(heap->nos);
    free(heap);
}

void busca_a_estrela(char mapa[LINHAS][COLUNAS], Coordenadas inicio, Coordenadas fim) {
    int passos=1;
    int ponto=0,num_f=0,num_a=0,num_m=0,num_r=0;
    Heap* fronteira = criar_heap(LINHAS * COLUNAS);
    int custo[LINHAS][COLUNAS];
    Coordenadas caminho[LINHAS][COLUNAS];

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            custo[i][j] = INT_MAX;
        }
    }

    No inicio_no = {inicio, 0, distancia_manhattan(inicio, fim)};
    inserir_heap(fronteira, inicio_no);
    custo[inicio.linha][inicio.coluna] = 0;

    while (!heap_vazia(fronteira)) {
        No atual = remover_heap(fronteira);

        if (atual.pos.linha == fim.linha && atual.pos.coluna == fim.coluna) {
            printf("Caminho encontrado!\n");
            Coordenadas pos = fim;
            int custo_total = custo[pos.linha][pos.coluna];
            while (!(pos.linha == inicio.linha && pos.coluna == inicio.coluna)) {
                char a = mapa[pos.linha][pos.coluna];
                if(a=='.')
                    ponto++;
                else if(a=='A')
                    num_a++;
                else if(a=='F')
                    num_f++;
                else if(a=='M')
                    num_m++;

                else if(a=='R')

                    num_r++;
                

            
                printf("(%d, %d) %c<- ", pos.linha, pos.coluna,a);
                passos++;
                pos = caminho[pos.linha][pos.coluna];
            }

            printf("(%d, %d)\n", inicio.linha, inicio.coluna);
            printf("num pontos: %d:%d\nnum A: %d:%d\nnum F: %d:%d\nnum M: %d:%d\nnum R: %d:%d\n",ponto,ponto*1,num_a,num_a*30,num_f,num_f*15,num_m,num_m*200,num_r,num_r*5);
            printf("Custo total: %d\n", custo_total);
            printf("passos dados: %d\n",passos);
            liberar_heap(fronteira);
            return;
        }

        Coordenadas vizinhos[4] = {
            {atual.pos.linha + 1, atual.pos.coluna},
            {atual.pos.linha - 1, atual.pos.coluna},
            {atual.pos.linha, atual.pos.coluna + 1},
            {atual.pos.linha, atual.pos.coluna - 1}
        };

        for (int i = 0; i < 4; i++) {
            Coordenadas viz = vizinhos[i];

            if (viz.linha >= 0 && viz.linha < LINHAS && viz.coluna >= 0 && viz.coluna < COLUNAS) {
                int custo_novo = custo[atual.pos.linha][atual.pos.coluna] + (mapa[viz.linha][viz.coluna] == '.' ? 1 : 
                                    mapa[viz.linha][viz.coluna] == 'M' ? 200 : 
                                    mapa[viz.linha][viz.coluna] == 'A' ? 30 : 
                                    mapa[viz.linha][viz.coluna] == 'F' ? 15 :
                                    mapa[viz.linha][viz.coluna] == 'R' ? 5 :
                                    mapa[viz.linha][viz.coluna] == 'I' ? 1 : 
                                    mapa[viz.linha][viz.coluna] == 'E' ? 5 : 0);

                if (custo_novo < custo[viz.linha][viz.coluna]) {
                    custo[viz.linha][viz.coluna] = custo_novo;
                    int prioridade = custo_novo + distancia_manhattan(viz, fim);
                    No viz_no = {viz, custo_novo, prioridade};
                    inserir_heap(fronteira, viz_no);
                    caminho[viz.linha][viz.coluna] = atual.pos;
                }
            }
        }
    }

    printf("Caminho não encontrado.\n");
    liberar_heap(fronteira);
}
int main() {
    char mapa[LINHAS][COLUNAS] = {
        "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAARRRRRRRR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAA.AAAARRRRRRRRRRRRAAAAAAA.RRAAAAARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.RAARRAAAAAARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAA.AR.RRRRRRRRRRRRRRRRRRRRR.RRRRRRRRRRRRRRRRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARA.RRRRRRRRM.RR.RAARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.AAAAARRRRRRRRRRRR.RRRRRRAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAA.RRRRRRRRRRMRRMMMRMRMMMRRRRRRMMMRMMMMRRRRR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARRRRA.RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRA.AAA.RAAAAAAAAARAARRRRRRRR.RRRRRRRRRRRRRRRRRRRRRRAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAARRRRRMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMRRRRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARAAAAARRRRMMMMMR...R.R.RR.RRRRRRRMMMMMMMMMMRMMRRRMMRRRRRRR.R.A.RRRRRRRRRARRRRRRRRRRRRRRRRRRRRRRRRRR.AAAAAAAAAA",
"AAAAAAAAAAAAAAARR...RFRMMMMMMMMMMMMMMMMMMMMMRMFFFFF...RRRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARRRRAARRRRRMMMMMM.RA.A..R..9MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMRMMMMMMRRRRRRRAARRRRMMMRRRRRRRRRRRRRRRRRRRRRRAAAAAAAA",
"AAAAAAAAAAAAAAA.....RFFFRFFFFFFFFFFFFFFMMMMFFFFFFFR...R.AAAAAAAAAAAAAAAAAAAAAAAAAAAARRRRMMMMMM.ACRMMMMMMMMM.A..R....MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMRRRRMMMMRRRRRMMMMMMMMMMMMMRRRRRRRRRRRRRR.AAAAAAA",
"AAAAAAAAAAAAAAAARR......FFFFFFFFFFFFFFA..FRFFFFFRRR..AAAAAAAAAAAAAAAAAAAAAAAAARRRRRRRMMMMMMMMARR.MM.RMMRMMM.R...MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMRRRMMMMMMMMRRRMMMMMMMMMMM...MM.MMMMMMRRRRAAAAAA",
"AAAAAAAAAAAAAAAAA.R............RFFFFFFARR.FFFF.RRRAAAAAAAAAAAAAAAAAAAAAAAAAARRRMMMMMMMMMRMMMA.FFFRMMMMMMMMMMM....MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMFFFFFF.MMMMMMMMMMMMMMMM.FFFFFR..M.MRAAA",
"AAAAAAAAAAAAAAAAAAAAR...RR........RAAAAAAR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAARRRRMR..........M..R..FF.RMMM..RFFFFFFMMMMMMMMMMMMMMMMMMMMMMMMMMMM.....R.......MMMMMMMMMMM.FFFF.MMMMMMMMMM..FFFFFFFFFFFF....AAA",
"AAAAAAAAAAAAAAAAAAAAAAAA.A.RR.AR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.R...MMM..RMMRR.....R...RA..FFFFAAFFFFFFFFFFFFMMMMMMMMMMMMMMMMMMMMMMMM..........MMMMMMMMMMMM...R..R.R.R......FFFFFFFFFFFFFF...AAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARAAAAAAAAAAAAAAAAAARRAAAA.R...MRMMMMMFMRMR.R..RR..........A..RFFFFF...FFFFFFFFFFFFFFMRMMRM.A..........M.MMMMMMMMMMM..R.RRFFFRRFRFFF..AAAA.FFFFFFFFFF......AAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARAR.RRRFFFRAAAAAAAAAAA.A.R...RRR.R.RRFFFFFFFR.R................RA.RFFFA.RR......FFFFFFFFFFFFFFFFFAAA.....MMMMMMFFFFRFFFFFFFFFFFFFFFFFFFFFFFF.RFF.RFR......RRAR..AAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAARRRR....FFFFFFFFFFFR.....AR.....M.....R...FF.R........FFFFFFFFF......R..FFFRRR..........RFFFFFFFFFFFFFFFFFFR....MMMMMRFFFFFFFFFFR.FFFFFFFFFFFFFFFFFF.A.RFR.......RAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAA.R...FRFFFFFFFFFFFFFFFRR......MM..............R....R...R...RFFFFFFR........A.RR...FFFF..4.....FFFFFFFFFFFFFFFFFFF....................FFFFFFFFFFFFFFFFFFFFFARRRAR.AR.AAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAA....FFFFFFFFFFFFFFFFFFFFFF.R..RM........MR...............FFFR.FFFRR..............A...FFF.......FFFFFFFFFFFFFFFFFF.........R..........RFFFFFFFFFFFFFFFFFFRFFFFFRFFFFFRAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAA....FFFFFFFFFFFFFFFFFFFFFFFFRR.R..R.....R....RMMR..R...RFFF.......................R....RR....FFFFFFFFFFFFFFFFFFFFF........R...MMM...RFFFFFFFFFFFFFFFFFFFFFFFFF..MM.M.RAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAA..A...RFFFFFFFFFFFFR.....R.RRR..R.R...RR...M...R.......................R..R.......RAAR..FFF.RFFFFFFFFFFFFFFFFFFFFFFFFFRF......MM.MR....RFFFFFFFFFFFFFFFFFFFF.R.MMMMM.RAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAR..FFFFFFFFFFFFFFFRR...FR...MMM..M.......R.......FFFFFF..R..........M.R...R.RR......FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFR..................FF..FFFFFFFF.RF...RMMM...RAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAA.MRR.R....R..FFFFFFFFFFA.RFFFF...MMMMMMMMMMM.....RFFFFFFFFF......................A....RFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFRFF..............RFFFFRR.......RMM.MR.AAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAR.R..R.FFFFR.A.RFFFFFFARR.FFFF....MMMMMMMMMM.MFFFFFFFFFFFFFFFR.........................R.RFFFFFFFRR.................FFFFFFFFFFFFFFFFFF.........R..M.R.......R....MMMMMM..AAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAR..RFFFFFFFFFFRARFFFFFFFAR.FFFRAAA.FFFFFFRMMMMMFFFFFFFFFFFFFFFFF...R..R.........RR3..........FFFRF........R...............FFRAAAFRFF.........R..R.RRM.R.RR..R....RMMMMMMM...RAAAAAAAAAAAAAAA",
"AAAAAAAAAA...FFFFFFFFFFFFFF....RFFFFAR..FFRR.RFFFFFFFFMMMMMFFFFFR..FFFFFF......R.MM....R...........RR...............RR......R.........FFFFFFFFFF.........................MMRMMMMMMMM....AAAAAAAAAAAAAAAA",
"AAAAAAAAR...RFFFFFFFFFFFFFFFFRAA.FFFAR..FRFR....RAARFFFFFFFFFFFAAA..RFF.................................R.............R..R.R...........FFFFFFFFR...FF..........R...R.MR.....MMMMMMM....RAAAAAAAAAAAAAAAA",
"AAAAAA.R..FFFFFFFFFFFFFFFFFFFFFARFFAR.R.RRFFAAAAAAARR...F....R...RRR.............R.................R.R.....M...R...........MR.R..R...R..FFFFFFF..................MR......MMMMMMMMMMMR..RAAAAAAAAAAAAAAAA",
"AAAAA...FFFFFFFFFFFFFFRD....FRAAR.RAAAAAA..RAAAAAAAAA......................R.......................RR...............R.R.....MR.RMM...R...............................R...MMMMMMMMM...RAAAAAAAAAAAAAAAAAA",
"AAAA.R..FFFRFFFFFFR.FFFFF..R..RAAAAAAAAAAAAAAAAAAAAAAAA..............R.......6......R..RRRRR.RR...R...........R....R........MRR.RR.....FFFFFFFF....FFFFFFFFFFFFRR....R.......M.MMMM.M.AAAAAAAAAAAAAAAAAA",
"AAAAAA.R.FFFFFFFFF..FFFFF..AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.....RFFFFFFF.R..R..R........R...MMM..RR...RR.RMRR...............RR.R.......FFFFFFFF....FFFFFFFFFFFFFFFFFFFFFFR..MMMMMMMMMM.AAAAAAAAAAAAAAAAA",
"AAAARR.RFFFFFFFFFF.FFFFF..R.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.RARFFFFFFFR....R...MMMMMMMMMMMMMMMMM..RRARRRRR.RA.RR.......R.R.RR........FFFFFFFFR.......FFFFFFFFFFFFFFFFFFFFF....MMM..R..AAAAAAAAAAAAAAAA",
"AAAA.R.FFFFFFFFFFR..FFFF....AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR.A..R....R..RRRMMMMMMMMMMMMMMMMMMMMMM....A...R..RM...M.R................FFFFFFFFFF....R...FFFFFFFFFFFFFFFF....MMMRAAAAAAAAAAAAAAAAAAAAAAA",
"AAAA.R.FFFFFFFFFF.........R..AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA..AAA...R.ARR.R.....MMMMMMMMMMMMMMMMMMM....M.R...MRRR.R..............RFFFFFFFFFFFFFF.AR....RFFFFR....I......R.RRAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAA.R.RFFFFFFFF.........RAR...ARAAA.AARA.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARR...5.RMMM.MMMMMMMMMMMMMMMMMR....RR.RR...R.MMMMR..FFFFFFFFFFFFFFFFFFFFFFFF.A...FFFF.FFFF......R....AAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAA..RFFFFRR....R......RA......MMMRMMMMM....ARRAAAAAAAAAAAAAAAAAAAAAAAAARR......R.RMMMMMMMMMMMMMMMMMMM.......RR......RMMMRRR....RFFFFFFFFFFFFFFFR...R....FFFFFFFFFFFFFRR...AAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAR.FFFA....RR.....R..A.R...MMMMMMMMMMMMMM......AAAAAAAAAAAAAAAAAAAAAA.....MMM...R.MMRMM......MMMMMMMM.M.RRA.....MMMMMMMR...........FFFFF...R......ARFRRRFRRFRRFRRR.FFR...AAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAR..A...RR.R.R.RA.R..........MMMMMMMMMMMMM....AAAAAAAAAAAAAAAAAAAAAAARR.........RM...RMM.M...MMMMMMMMMR........MMMMMRR..MMM.R.....R.............RAAA.FFFFFFFFFFFAARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAA......R..AR..RRRRFFFFAA.AR..MMMMMMMMMMMMMMMM.RAAAAAAAAAAAAAAAAAAAAAAA.........MR..MM.RR.M..R..M.MM...RR.R.R.RMMM.R.M.MMMMMRMMR.R......R..RRR.RR...AAAAAARRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAARAR.......RFFFFFFFFFFR...A....RMMMMMMMMMMMMMMMM...RAAAAAAAAAAAAAAAAAAAAAAA...R.R...M.RMRR...R........ARRR..MMMMMMMMMMMMRMMMMMMMRMMMMRR....R.M...AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAA.......RR..FFFFFFFFFFRR..RR..RRRMMMMMMMMMMMMMMMMMM...AAAAAAAAAAAAAAAAAAAAAA..................MMR...A.....MMRR.MMMMMMM..R..MMMMMMMMMMMM...AAR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAR.....A.R...FFFFFFFFFFFFFRA.R...MMMMMMMMMMMMMMMMMM..AAAAAAAAAAAAAAAAAAAAAAAAR..R....RAAR..AAAAAAAAAAAAR.......RR.RRRR...RRMMMMMMMMM..AAAAAMMM.AAAAAAAAAAAAAAAAAAAAAA..RRAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAR...RRR....FRRFFFFFFFFFFRR..MMMMMMMMMMMMMRMM.....RAAAAAAAAAAAAAAAAAAAAAAAAAR....A.AAAAAAAAAAAAAAAAAAAAARR.R.AARM..MMMMMMMMMMMMMR.AR...RM.RAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAARR..RFFFFFF.FFFFFFFFAR..MMMMMMMMM......R.R.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA..RRR....RMMMMMMM.AA.R...MRAAAAAAAAAAAAA.AAAAAAAAAAAAAAAAAAAAARR.AAAAAAAAAAAAAA",
"AAAAAA.R.AAAAAARRR.FFFFFFFFRRRR....R.R..MMMMMM....A.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.R..............A.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA..E...AAAAAAAAAAAA",
"AAAAAAAA.AAAAAAR...FFFFFFFFFA.....RFF...MMMMMMMM.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.AAAAAAAAAAR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAA...R..A.R..RAAAAAAAAAAAA...AAAAAAAAAAAAAAAAAA.R.AARAAAAAAAAA.R.RRAAAAAAAAAAA",
"AAAAAAAAAAAAAAA.R...............RFFFFFFFMMMMMMMMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA....AA.AAAAAA....AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAARR...RAAAAAAAAAAAAA.....RAAAAAAAAAAAAAR..AAAAAAR.RAA",
"AAAAAAAAAAAAAAAA.R.AA....MMM.MM..RFFFFFFFMMMMMRMMR.RAAAAAAAAAAAAAAAAAAAAAAAAA.R..RR...R.R8M.MMRMMM...M.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.RRR.RRAAAAAAARRR..R..AAAAAAAAAAAAA..AAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAARRMMMMMM..FFFFFFMMMMMMMM.AAAAAAAAAAAAAAAAAAAAAAAAARRR.MM..MR..MR...R....MRMRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.RA.R0.R.AAAAAAAAAAAAA..AAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAA..RRMMMMMM.RFFFFFMMMMMMMMRAAAAAAAAAAAAAAAAAAAAAAA.R...M....MMM....R....MMMM.R.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.B....RAARAAAA....RAAAAAAAAA.R...AAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAR....MMMM.RRFFFF.MMMMMM.AAAAAAAAAAAAAAAAAAAAAAAR.....R.R..MMMMR.R...........RAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA..RAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA...RR.RAAR.AAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAA.R..MMMR..FFFFFMMMMMMAAAAAAAAAAAAAAAAAAAAAAAAR.....R..2RMMMR......MMMMMMR..MM..AAAAAAAAAAAAAAAAAAAAAAAAARR.........R.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.AAAR...RAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAARRR.MMMMM.RFFFF.AR..AAAAAAAAAAAAAAAAAAAAAAAARR......M.......M.....R.....RR.R...AAAA.R...ARRAAA......RA..RAAAAAAAR....AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAR.....MMMMMM....RA..RAAAAAAAAAAAAAAAAAAAAAAAAR........A..R....RR.RRRRRMM......MAAAAA.........R.ARAR....AAAAAAAAAAAAAAA...AR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAA.R..MMMMMM.M.1.FFFFAAAAAAAAAAAAAAAAAAAAAAAAARR...........AAAAAAA.R.........RAAAAAAA..RRR.R.AAAAAAAAAAAAAAAAAAAAAAAAAAARAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA...R..R.FFFFFFFFFAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.A..A..AAAAR...AAAAAAAAAAAAAAAAAAAAAA....AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.R.RFFFFFFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR..7..AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA..FFFFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" };
char caminhos_percorrer[15]= {'I','1','2','3','4','5','6','7','8','9','0','B','C','D','E'};
for(int i=0;i<14;i++){
    Coordenadas inicio = encontrar_elemento(mapa,caminhos_percorrer[i]);
    Coordenadas fim = encontrar_elemento(mapa,caminhos_percorrer[i+1]);
    int a = distancia_manhattan(inicio,fim);
    printf("indo de: %c para: %c\n",mapa[inicio.linha][inicio.coluna],mapa[fim.linha][fim.coluna]);
    printf("%d\n",a);
    busca_a_estrela(mapa,inicio,fim);


}

  
}

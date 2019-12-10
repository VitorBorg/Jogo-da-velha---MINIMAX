#include <stdio.h>
#include <stdlib.h>

//MAPA JOGO DA VELHA
//0 = NAO MARCADO
//1 = X
//2 = O

//MINIMAX
//0 EMPATE
//1 vitoria
//-1 derrota

typedef struct nodo{
    int map[9];
    int simbol;
    int size;
    struct nodo *nodos;
} nodo;

void init(nodo * nod);
void Tictactoe(nodo * r);
int lenght(int map[]);
nodo newNodo(nodo raiz, int pos);
int regras(int map[]);
int minimax(nodo * r, int depth);

int estados = 0;

int main(){
    int resultMinMax;
    nodo raiz;
    init(&raiz);

    Tictactoe(&raiz);
    resultMinMax = minimax(&raiz, 0);

    printf("\nNodos abertos: %d \nResultado MINIMAX: %d", estados, resultMinMax);

    //JESUS SABE QUE EU ME SINTO CULPADO POR NAO DAR FREE NOS OUTROS MALLOCS
    free(raiz.nodos);
    return 0;
}

void init(nodo * nod){
    //para preencher o map (vetor da raiz) com 0
    int i;

    for (i = 0; i < 9; i++)
        nod->map[i] = 0;

    //para setar o simbolo incial, no caso, será X = 1, pois, na criacao do nodo, o valor troca
    nod->simbol = 2;
}

void Tictactoe(nodo * r){
    estados++;

    int i, freepositions = lenght(r->map);

    //crio nodos filhos com base nas posicoes
    r->nodos = malloc(sizeof(nodo) * freepositions);

    //preencho as possibilidades
    for(i = 0; i < freepositions; i++){
        nodo *nn = r->nodos + i;
        *nn = newNodo(*r, i);

        Tictactoe(nn);
    }

    // para saber quantos nodos filhos o nodo tem
    r->size = i;
    return;
}

int lenght(int map[]){
    //para saber o numero de posicoes vazias
    int i, temp = 0;

    for (i = 0; i < 9; i++){
        if(map[i] == 0)
            temp+=1;
    }

    return temp;
}

nodo newNodo(nodo raiz, int pos){
    nodo new;

    //copiando o mapa do pai
    int i, temp = 0;

    for (i = 0; i < 9; i++)
        new.map[i] = raiz.map[i];

    //para saber qual simbolo jogar agora
    new.simbol = raiz.simbol == 1 ? 2 : 1;

    // marcando a posicao certa no mapa
    for (i = 0; i < 9; i++){
        if(new.map[i] == 0){
            if(temp == pos){
                new.map[i] = new.simbol;
                return new;
            }
            temp ++;
        }
    }

    //retornando a posicao preenchida
    return new;
}

int regras(int map[]){

    //combinacoes para vencer
    //0,3,6 - 1,4,7 - 2,5,8 - 0,1,2 - 3,4,5 - 6,7,8 - 0,4,8 - 2,4,6
    int zero = (((map[0] == map[3]) && map[0] == map[6]) || ((map[0] == map[1]) && map[0] == map[2])) || ((map[0] == map[4]) && map[0] == map[8]);
    int quatro = (((map[1] == map[4]) && map[4] == map[7]) || ((map[3] == map[4]) && map[4] == map[5])) || ((map[2] == map[4]) && map[4] == map[6]);
    int oito = ((map[2] == map[8]) && map[8] == map[5]) || ((map[6] == map[8]) && map[8] == map[7]);

    if(zero && (map[0] != 0)){
        if(map[0] == 1)
            return + 1;
        else
            return - 1;
    }
    else if(quatro && (map[4] != 0)){
        if(map[4] == 1)
            return + 1;
        else
            return - 1;
    }
    else if(oito && (map[8] != 0)){
        if(map[8] == 1)
            return + 1;
        else
            return - 1;
    }
    else
        return 0;
    
}

int minimax(nodo * r, int depth){
    //troca flag
    int result[r->size], i;

    if(r->size == 0)
        return regras(r->map);
    else {
        for (i = 0; i < r->size; i++)
            result[i] = minimax(r->nodos + i, depth + 1);
    }

    if(depth == 0 || depth % 2 == 0){
        for (i = 0; i < r->size; i++){
            if(result[i] == 1)
                return result[i];
        }
    }
     else {
        for (i = 0; i < r->size; i++){
            if(result[i] == -1)
                return result[i];
        }
     }

    //SE NAO HOUVER NENHUM RESULTADO DESEJADO, PEGUE O NEUTRO
     for (i = 0; i < r->size; i++){
        if(result[i] == 0)
            return result[i];
     }

    //SE NAO TIVER NEM O DESEJADO, E NEM O NEUTRO, PEGA O PRIMEIRO
     return result[0];
}

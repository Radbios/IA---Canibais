#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANNIBAL 0 
#define MISSIONARY 1  
#define NUMERO_POSSIBILIDADES 5   
#define CANNIBAL 0
#define MISSIONARY 1
#define LADO1 0
#define LADO2 1

#define TRUE 1
#define FALSE 0


typedef struct arvore{
    int deep;
    int m[4];
    int bote;
    struct arvore *pai; 
    struct arvore *filho;
    struct arvore *irmao;
} Arvore;

int possibilidades[5][2] = {{1, 0}, {0, 1}, {1, 1},{2, 0}, {0, 2}};

Arvore* iniciar_arvore(){
    Arvore *arvore = (Arvore*)malloc(sizeof(Arvore));
    arvore->m[0] = 3;
    arvore->m[1] = 3;
    arvore->m[2] = 0;
    arvore->m[3] = 0;
    arvore->bote = 0;
    arvore->deep = 0;
    arvore->pai = NULL;
    arvore->filho = NULL;
    arvore->irmao = NULL;

    return arvore;
}


int checarSolucao(Arvore *arvore){
    if(arvore->m[2] == 3 && arvore->m[3] == 3){
        return TRUE;
    }
    else return FALSE;
}

void printarSolucao(Arvore *arvore){
    printf("profundidade : %d\n", arvore->deep);
    printf("MARGEM 1: Nº de Canibais: %d\t\tNº de Missionários: %d\n", arvore->m[0], arvore->m[1]);
    printf("MARGEM 2: Nº de Canibais: %d\t\tNº de Missionários: %d\n", arvore->m[2], arvore->m[3]);

    if(arvore->pai != NULL){
        printarSolucao(arvore->pai);
    }
}
int checarCondicao(Arvore *arvore){
    if((arvore->m[CANNIBAL] > arvore->m[MISSIONARY] && arvore->m[MISSIONARY] > 0) || (arvore->m[CANNIBAL+2] > arvore->m[MISSIONARY+2] && arvore->m[MISSIONARY+2] > 0)){
        return FALSE;
    }
    return TRUE;
}

Arvore *criarFilhos(Arvore *arvore, Arvore *prev_node, int ind_possibilidades){
  
    if(ind_possibilidades < NUMERO_POSSIBILIDADES && checarCondicao(prev_node)){
        int n_cannibalM1 = 0;
        int n_cannibalM2 = 0;
        int n_missionaryM1 = 0;
        int n_missionaryM2 = 0;
        int bote = 0;
       
        if(prev_node->bote == LADO1){
            bote = LADO2;
            n_cannibalM1 = prev_node->m[CANNIBAL] - possibilidades[ind_possibilidades][CANNIBAL];
            n_cannibalM2 = prev_node->m[CANNIBAL+2] + possibilidades[ind_possibilidades][CANNIBAL];
            n_missionaryM1 = prev_node->m[MISSIONARY] - possibilidades[ind_possibilidades][MISSIONARY];
            n_missionaryM2 = prev_node->m[MISSIONARY+2] + possibilidades[ind_possibilidades][MISSIONARY];
        }
        else{
            bote = LADO1;
            n_cannibalM1 = prev_node->m[CANNIBAL] + possibilidades[ind_possibilidades][CANNIBAL];
            n_cannibalM2 = prev_node->m[CANNIBAL+2] - possibilidades[ind_possibilidades][CANNIBAL];
            n_missionaryM1 = prev_node->m[MISSIONARY] + possibilidades[ind_possibilidades][MISSIONARY];
            n_missionaryM2 = prev_node->m[MISSIONARY+2] - possibilidades[ind_possibilidades][MISSIONARY]; 
        }

      
        if(n_cannibalM1 >= 0 && n_cannibalM1 <= 3 && n_missionaryM1 >= 0 && n_missionaryM1 <= 3){
            if(arvore == NULL){
                
                arvore = (Arvore *)malloc(sizeof(Arvore));

                arvore->filho = NULL;
                arvore->irmao = NULL;
                arvore->pai = prev_node;
                arvore->deep = prev_node->deep + 1;
                
    
                arvore->bote = bote;
                arvore->m[CANNIBAL] = n_cannibalM1;
                arvore->m[MISSIONARY] = n_missionaryM1;
                arvore->m[CANNIBAL+2] = n_cannibalM2;
                arvore->m[MISSIONARY+2] = n_missionaryM2;
                
            }
           
            arvore->irmao = criarFilhos(arvore->irmao, prev_node, ++ind_possibilidades);
        }
        else arvore = criarFilhos(arvore, prev_node, ++ind_possibilidades);
        
    }

   
    return arvore;
}


Arvore* DSF(Arvore *arvore, int border_limit, Arvore *result){
    if(arvore->deep == border_limit){
        if(checarSolucao(arvore)){
            return arvore;
        }
    }
    else{
        arvore->filho = criarFilhos(arvore->filho, arvore, 0);
    }
    
    if(arvore->filho != NULL && (!checarSolucao(result))){

        result = DSF(arvore->filho, border_limit, result);
    }

    if(arvore->irmao != NULL && (!checarSolucao(result))){

        result = DSF(arvore->irmao, border_limit, result);
    }
    return result;
}

int main(){
    int border_limit = 0;

    while (TRUE){
    
        Arvore *arvore = iniciar_arvore();

        Arvore *result = DSF(arvore, border_limit, arvore);

        if(!checarSolucao(result)){
            border_limit = border_limit + 1;
        }
        else{
            printf("Solução encontrada\n");
            printarSolucao(result);
            break;
        }

        
        free(arvore);
    }
    return 0;
}
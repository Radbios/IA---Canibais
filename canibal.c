#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- DEFINES ---
#define CANNIBAL 0
#define MISSIONARY 1
#define VOID 3
#define SIDE_1 0
#define SIDE_2 1
#define N_CANNIBAL 3
#define N_MISSIONARY 3
#define MAX_CONFIG 5


#define TRUE 1
#define FALSE 0



// --- END DEFINES ---

typedef struct three{
    int n_missionary;
    int n_cannibal;    
    int boat_margin;
    struct three *father;
    struct three *next;
    struct three *right;
} Three;

// --- INIT THREE IN SIDE ONE (ORIGIN)---
Three* init(){
    Three *three = (Three*)malloc(sizeof(Three));
    three->n_cannibal = N_CANNIBAL;
    three->n_missionary = N_MISSIONARY;
    three->boat_margin = SIDE_1;
    three->father = NULL;
    three->next = NULL;
    three->right = NULL;

    return three;
}
// --- END INIT ---

// --- CHECK THREE CURRENT STATE ---
int response_check(Three *three){
    if(three->n_cannibal == 0 && three->n_missionary == 0){
        return TRUE;
    }
    else return FALSE;
}
// --- END CHECK ---

void insert_three(Three *three, Three *aux){
   
}

Three *children_generate(Three *three, Three *prev_node, int config[][2], int ind_config){
    // --- SE NAO ATINGIR TODAS AS CONFIGURAÇÕES ---
    if(ind_config < MAX_CONFIG){
        if(three == NULL){
            // --- CRIAR FILHO ---
            three = (Three *)malloc(sizeof(Three));

            three->next = NULL;
            three->right = NULL;

            // --- CHECAR SE O NÓ ANTERIOR TEM PAI (SE NÃO TIVER, ELE É A RAIZ) ---
            if(prev_node->father != NULL){
                three->father = prev_node->father;
            }
            else three->father = prev_node;
            

            // --- SE O BARCO ESTIVER NA MARGEM 1, SUBTRAÇÃO NO CÁLCULO ---
            if(prev_node->boat_margin == SIDE_1){
                three->boat_margin = SIDE_2;
                three->n_cannibal = prev_node->n_cannibal - config[ind_config][CANNIBAL];
                three->n_missionary = prev_node->n_missionary - config[ind_config][MISSIONARY];
            }
            // --- SE NÃO, ADIÇÃO NO CÁLCULO ---

            else{
                three->boat_margin = SIDE_1;
                three->n_cannibal = prev_node->n_cannibal + config[ind_config][CANNIBAL];
                three->n_missionary = prev_node->n_missionary + config[ind_config][MISSIONARY];
            }
        }
        // --- IR PARA O OUTRO FILHO COM OUTRA CONFIGURAÇÃO ---
        three->right = children_generate(three->right, three, config, ++ind_config);
        
    }
    
    // --- RETORNA O RAMO ---
    return three;
}

int main(){

    Three* three = init();

    // --- POSSIBLES CONFIG ---
    int config[5][2] = {{1, 0}, {0, 1}, {1, 1},{2, 0}, {0, 2}};
    three->next = children_generate(three->next, three, config, 0);

    // while(response_check(three)){
        
    // }
   
    free(three);
    return 0;
}
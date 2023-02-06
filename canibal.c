#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- DEFINES ---
#define CANNIBAL 0
#define MISSIONARY 1
#define VOID 3
#define SIDE_1 1
#define SIDE_2 2
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

void children_generate(Three *three, Three *father, int config[][2], int ind_config){
    if(ind_config < MAX_CONFIG){

        Three aux;

        if(father->boat_margin == SIDE_1){
            if(father->n_cannibal + config[ind_config][CANNIBAL] > N_CANNIBAL || father->n_missionary + config[ind_config][MISSIONARY] > N_MISSIONARY){
                children_generate(three, father, config, ind_config++);
            }
            aux.boat_margin = SIDE_2;
            aux.n_cannibal = father->n_cannibal + config[ind_config][CANNIBAL];
            aux.n_missionary = father->n_missionary + config[ind_config][MISSIONARY];

        }
        else{
            if(father->n_cannibal - config[ind_config][CANNIBAL] < 0 || father->n_missionary - config[ind_config][MISSIONARY] < 0){
                children_generate(three, father, config, ind_config++);
            }
            aux.boat_margin = SIDE_1;
            aux.n_cannibal = father->n_cannibal - config[ind_config][CANNIBAL];
            aux.n_missionary = father->n_missionary - config[ind_config][MISSIONARY];
        }

        if(aux.boat_margin != 0){
            
        }
    }
}

int main(){

    Three* three = init();

    // --- POSSIBLES CONFIG ---
    int config[5][2] = {{1, 0}, {0, 1}, {1, 1},{2, 0}, {0, 2}};

    // while(response_check(three)){
        
    // }
   
    free(three);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// --- DEFINES ---
#define CANNIBAL 0  // --- INDICE DOS CANIBAIS ---
#define MISSIONARY 1    // --- INDICE DOS MISSIONÁRIOS ---
#define SIDE_1 0    // --- MARGEM 1 DO RIO ---
#define SIDE_2 1    // --- MARGEM 2 DO RIO ---
#define N_CANNIBAL 3 // --- NUMERO DE CANIBAIS (INICIAL) ---
#define N_MISSIONARY 3 // --- NUMERO DE MISSIONARIOS (INICIAL) ---
#define MAX_CONFIG 5    // --- NUMERO MAXIMO DE CONFIGURAÇÕES DE TRAVESSIA ---
#define START_BORDER 0  // --- LIMITE DE BUSCA ---

#define TRUE 1
#define FALSE 0

// --- END DEFINES ---

// --- STRUCTS AND GLOBAL VARIABLES ---
typedef struct three{
    int id; // --- IDENTIFICADOR ---
    int line; // --- NUMERO DE TRAVESSIAS ---
    int n_missionary; // --- NUMERO DE MISSIONÁRIOS --- 
    int n_cannibal; // --- NUMERO DE CANIBAIS ---
    int boat_margin; // --- MARGEM Q O BOTE SE ENCONTRA ---
    struct three *father; // --- NÓ PAI ---
    struct three *next; // --- NÓ FILHO ---
    struct three *right; // --- NÓ IRMÃO ---
} Three;

int idCount = -1;
int config[5][2] = {{1, 0}, {0, 1}, {1, 1},{2, 0}, {0, 2}};


// --- END STRUCTS AND GLOBAL VARIABLES ---

// --- INCREMENT ID ---
int id(){
   idCount++;
   return idCount; 
}

// --- INIT THREE IN SIDE ONE (ORIGIN)---
Three* init(){
    Three *three = (Three*)malloc(sizeof(Three));
    three->n_cannibal = N_CANNIBAL;
    three->n_missionary = N_MISSIONARY;
    three->boat_margin = SIDE_1;
    three->father = NULL;
    three->next = NULL;
    three->right = NULL;
    three->id = id();
    three->line = 0;

    return three;
}
// --- END INIT ---

// --- CHECK THREE CURRENT STATE ---
int is_solution(Three *three){
    if(three->n_cannibal == 0 && three->n_missionary == 0){
        return TRUE;
    }
    else return FALSE;
}
// --- END CHECK ---

int condicion_is_valid(Three *three){
    if((three->n_cannibal > three->n_missionary && three->n_missionary > 0) || (N_CANNIBAL - three->n_cannibal > N_MISSIONARY - three->n_missionary && three->n_missionary < N_MISSIONARY)){
        return FALSE;
    }
    return TRUE;
}
// --- GERAR FILHOS ---
Three *children_generate(Three *three, Three *prev_node, int ind_config){
    // --- SE NAO ATINGIR TODAS AS CONFIGURAÇÕES ---
    if(ind_config < MAX_CONFIG && condicion_is_valid(prev_node)){

        // --- VARIAVEIS AUXILIARES ---
        int n_cannibalSUB = prev_node->n_cannibal - config[ind_config][CANNIBAL];
        int n_cannibalAD = prev_node->n_cannibal + config[ind_config][CANNIBAL];
        int n_missionarySUB = prev_node->n_missionary - config[ind_config][MISSIONARY];
        int n_missionaryAD = prev_node->n_missionary + config[ind_config][MISSIONARY];

        // --- CHECAR SE O NUMERO DE MISSIONARIOS E CANIBAIS ESTAO DENTRO DO LIMITE, 0 <= X <= N_MISSIONARY
        if((prev_node->boat_margin == SIDE_1 && n_cannibalSUB >= 0 && n_missionarySUB >=0) || (prev_node->boat_margin == SIDE_2 && n_cannibalAD <= N_CANNIBAL && n_missionaryAD <=N_MISSIONARY) ){
            if(three == NULL){
                // --- CRIAR FILHO ---
                three = (Three *)malloc(sizeof(Three));

                three->id = id();
                three->line = prev_node->line+1;
                three->next = NULL;
                three->right = NULL;
                three->father = prev_node;
                
                // --- SE O BARCO ESTIVER NA MARGEM 1, SUBTRAÇÃO NO CÁLCULO ---
                if(prev_node->boat_margin == SIDE_1){
                    three->boat_margin = SIDE_2;
                    three->n_cannibal = n_cannibalSUB;
                    three->n_missionary = n_missionarySUB;
                }
                // --- SE NÃO, ADIÇÃO NO CÁLCULO ---

                else{
                    three->boat_margin = SIDE_1;
                    three->n_cannibal = n_cannibalAD;
                    three->n_missionary = n_missionaryAD;
                }
            }
            // --- IR PARA O OUTRO FILHO COM OUTRA CONFIGURAÇÃO ---
            three->right = children_generate(three->right, prev_node, ++ind_config);
        }
        else three = children_generate(three, prev_node, ++ind_config);
        
    }

    // --- RETORNA O RAMO ---
    return three;
}
// --- PRINTAR SOLUÇÃO ---
void print_solucion(Three *three){
    printf("Margem : %d\n", three->boat_margin);
    if(three->boat_margin == SIDE_1){
        printf("Nº de Canibais: %d\t\tNº de Missionários: %d\n", three->n_cannibal, three->n_missionary);
    }
    else{
        printf("Nº de Canibais: %d\t\tNº de Missionários: %d\n", N_CANNIBAL - three->n_cannibal,N_MISSIONARY - three->n_missionary);
    }

    if(three->father != NULL){
        print_solucion(three->father);
    }
}
// --- BUSCA EM PROFUNDIDADE (INTERATIVA) ---
Three* deep_search(Three *three, int border_limit, Three *result){
    if(three->line == border_limit){
        if(is_solution(three)){
            return three;
        }
    }
    else{
        three->next = children_generate(three->next, three, 0);
    }
    
    if(three->next != NULL && (!is_solution(result))){
        result = deep_search(three->next, border_limit, result);
    }

    if(three->right != NULL && (!is_solution(result))){
        result = deep_search(three->right, border_limit, result);
    }

    return result;
}

int main(){

   int border_limit = START_BORDER;
    while (TRUE){
        idCount = 0;
        Three *three = init();

        Three *result = deep_search(three, border_limit, three);
        if(!is_solution(result)){

            border_limit++;
        }
        else{
            printf("SOLUÇÂO ENCONTRADA!!!\n");
            printf("Nº DE TRAVESSIAS : %d\n", border_limit);
            print_solucion(result);
            break;
        }
        free(three);
    }
    return 0;
}
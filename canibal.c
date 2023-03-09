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
typedef struct tree{
    int id; // --- IDENTIFICADOR ---
    int line; // --- NUMERO DE TRAVESSIAS ---
    int n_missionary; // --- NUMERO DE MISSIONÁRIOS --- 
    int n_cannibal; // --- NUMERO DE CANIBAIS ---
    int boat_margin; // --- MARGEM Q O BOTE SE ENCONTRA ---
    struct tree *father; // --- NÓ PAI ---
    struct tree *next; // --- NÓ FILHO ---
    struct tree *right; // --- NÓ IRMÃO ---
} Tree;

int idCount = -1;
int config[5][2] = {{1, 0}, {0, 1}, {1, 1},{2, 0}, {0, 2}};


// --- END STRUCTS AND GLOBAL VARIABLES ---

// --- INCREMENT ID ---
int id(){
   idCount++;
   return idCount; 
}

// --- INIT tree IN SIDE ONE (ORIGIN)---
Tree* init(){
    Tree *tree = (Tree*)malloc(sizeof(Tree));
    tree->n_cannibal = N_CANNIBAL;
    tree->n_missionary = N_MISSIONARY;
    tree->boat_margin = SIDE_1;
    tree->father = NULL;
    tree->next = NULL;
    tree->right = NULL;
    tree->id = id();
    tree->line = 0;

    return tree;
}
// --- END INIT ---

// --- CHECK tree CURRENT STATE ---
int is_solution(Tree *tree){
    if(tree->n_cannibal == 0 && tree->n_missionary == 0){
        return TRUE;
    }
    else return FALSE;
}
// --- END CHECK ---

int condicion_is_valid(Tree *tree){
    if((tree->n_cannibal > tree->n_missionary && tree->n_missionary > 0) || (N_CANNIBAL - tree->n_cannibal > N_MISSIONARY - tree->n_missionary && tree->n_missionary < N_MISSIONARY)){
        return FALSE;
    }
    return TRUE;
}
// --- GERAR FILHOS ---
Tree *children_generate(Tree *tree, Tree *prev_node, int ind_config){
    // --- SE NAO ATINGIR TODAS AS CONFIGURAÇÕES ---
    if(ind_config < MAX_CONFIG && condicion_is_valid(prev_node)){

        // --- VARIAVEIS AUXILIARES ---
        int n_cannibalSUB = prev_node->n_cannibal - config[ind_config][CANNIBAL];
        int n_cannibalAD = prev_node->n_cannibal + config[ind_config][CANNIBAL];
        int n_missionarySUB = prev_node->n_missionary - config[ind_config][MISSIONARY];
        int n_missionaryAD = prev_node->n_missionary + config[ind_config][MISSIONARY];

        // --- CHECAR SE O NUMERO DE MISSIONARIOS E CANIBAIS ESTAO DENTRO DO LIMITE, 0 <= X <= N_MISSIONARY
        if((prev_node->boat_margin == SIDE_1 && n_cannibalSUB >= 0 && n_missionarySUB >=0) || (prev_node->boat_margin == SIDE_2 && n_cannibalAD <= N_CANNIBAL && n_missionaryAD <=N_MISSIONARY) ){
            if(tree == NULL){
                // --- CRIAR FILHO ---
                tree = (Tree *)malloc(sizeof(Tree));

                tree->id = id();
                tree->line = prev_node->line+1;
                tree->next = NULL;
                tree->right = NULL;
                tree->father = prev_node;

                // --- SE O BARCO ESTIVER NA MARGEM 1, SUBTRAÇÃO NO CÁLCULO ---
                if(prev_node->boat_margin == SIDE_1){
                    tree->boat_margin = SIDE_2;
                    tree->n_cannibal = n_cannibalSUB;
                    tree->n_missionary = n_missionarySUB;
                }
                // --- SE NÃO, ADIÇÃO NO CÁLCULO ---

                else{
                    tree->boat_margin = SIDE_1;
                    tree->n_cannibal = n_cannibalAD;
                    tree->n_missionary = n_missionaryAD;
                }
            }
            // --- IR PARA O OUTRO FILHO COM OUTRA CONFIGURAÇÃO ---
            tree->right = children_generate(tree->right, prev_node, ++ind_config);
        }
        else tree = children_generate(tree, prev_node, ++ind_config);

    }

    // --- RETORNA O RAMO ---
    return tree;
}
// --- PRINTAR SOLUÇÃO ---
void print_solucion(Tree *tree){
    printf("Margem : %d\n", tree->boat_margin);
    if(tree->boat_margin == SIDE_1){
        printf("Nº de Canibais: %d\t\tNº de Missionários: %d\n", tree->n_cannibal, tree->n_missionary);
    }
    else{
        printf("Nº de Canibais: %d\t\tNº de Missionários: %d\n", N_CANNIBAL - tree->n_cannibal,N_MISSIONARY - tree->n_missionary);
    }

    if(tree->father != NULL){
        print_solucion(tree->father);
    }
}
// --- BUSCA EM PROFUNDIDADE (INTERATIVA) ---
Tree* deep_search(Tree *tree, int border_limit, Tree *result){
    if(tree->line == border_limit){
        if(is_solution(tree)){
            return tree;
        }
    }
    else{
        tree->next = children_generate(tree->next, tree, 0);
    }

    if(tree->next != NULL && (!is_solution(result))){
        result = deep_search(tree->next, border_limit, result);
    }

    if(tree->right != NULL && (!is_solution(result))){
        result = deep_search(tree->right, border_limit, result);
    }

    return result;
}

int main(){

   int border_limit = START_BORDER;
    while (TRUE){
        idCount = 0;
        Tree *tree = init();

        Tree *result = deep_search(tree, border_limit, tree);
        if(!is_solution(result)){
            border_limit++;
        }
        else{
            printf("SOLUÇÂO ENCONTRADA!!!\n");
            printf("Nº DE TRAVESSIAS : %d\n", border_limit);
            print_solucion(result);
            break;
        }
        free(tree);
    }
    return 0;
}
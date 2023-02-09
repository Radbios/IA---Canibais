#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct three{
    int id;
    struct three *next;
    struct three *right;
} Three;

Three* init(){
    Three *three = (Three*)malloc(sizeof(Three));
    three->id = 10;
    three->next = NULL;
    three->right = NULL;

    return three;
}

Three *children_generate(Three *three, int cont){
    if(cont <3){
            printf("1");
        // --- SE NAO ATINGIR TODAS AS CONFIGURAÇÕES ---
        
        if(three == NULL){
        printf("2");

            // --- CRIAR FILHO ---
            three = (Three *)malloc(sizeof(Three));

            three->next = NULL;
            three->right = NULL;
            three->id = 11;
        }
        printf("3");
        
        three->right = children_generate(three->right, ++cont);

    }

    return three;
    
        
}

int main(){
    Three *aux = init();

    aux->next = children_generate(aux->next, 0);

    if(aux->next == NULL){
        printf("ÈNULL\n");
    }
    else{
        printf("OK\n");
    }

    free(aux);
    return 0;
}
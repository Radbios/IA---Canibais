#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define TAM 14 // --- TAMANHO DA MATRIZ DE ADJACENCIA ---
#define VOID -1 // --- DEFINE VOID (VAZIO) ---
#define TRUE 1
#define FALSE 0


// --- STRUCT DA ARVORE ---
typedef struct node {
    int value;
    int realValue;
    int id;
    int visited;
    struct node* left;
    struct node* right;
    struct node* father;
} Node;

// --- MAPEAMENTO (HEURÍSTICA) ---
int heuristicMap[TAM][TAM] = {
    {VOID ,11, 20, 27, 40, 43, 39, 28, 18, 10, 18, 30, 30, 32},
    {11, VOID, 9, 16, 29, 32, 28, 19, 11, 4, 17, 23, 21, 24},
    {20, 9, VOID, 7, 20, 22, 19, 11, 4, 17, 23, 21, 24},
    {27, 16, 7, VOID, 13, 16, 12, 13, 13, 18, 26, 21, 11, 17},
    {40, 29, 20, 13, VOID, 3, 2, 21, 25, 31, 38, 27, 16, 20},
    {43, 32, 22, 16, 3, VOID, 4, 23, 28, 33, 41, 30, 17, 20},
    {39, 28, 19, 12, 2, 4, VOID, 22, 25, 29 ,38, 28, 13, 17},
    {28, 19, 15, 13, 21, 23, 22, VOID, 9, 22, 18, 7, 25, 30},
    {18, 11, 10, 13, 25, 28, 25, 9, VOID, 13, 12, 12, 23, 28},
    {10, 4, 11, 18, 31, 33, 29, 22, 13, VOID, 20, 27, 20, 23},
    {18, 17, 21, 26, 38, 41, 38, 18, 12, 20 VOID, 15, 35, 39},
    {30, 23, 21, 21, 27, 30, 28, 7, 12, 27, 15, VOID, 31, 37},
    {30, 21, 13, 11, 16, 17, 13, 25, 23, 20, 35, 31, VOID, 5},
    {32, 24, 18, 17, 20, 20, 17, 30, 28, 23, 39, 37, 5, VOID}
};

// --- MAPEAMENTO (REAL) ---
int adjacenceMap[TAM][TAM] = {
    {VOID, 11, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {11, VOID, 9, VOID, VOID, VOID, VOID, VOID, 11, 4, VOID, VOID, VOID, VOID},
    {VOID, 9, VOID, 7, VOID, VOID, VOID, VOID, 10, VOID, VOID, VOID, 17, VOID},
    {VOID, VOID, 7, VOID, 15, VOID, VOID, 16, VOID, VOID, VOID, VOID, 13, VOID},
    {VOID, VOID, VOID, 15, VOID, 3, 2, 34, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, 3, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, 2, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, 16, 34, VOID, VOID, VOID, 13, VOID, VOID, 7, VOID, VOID},
    {VOID, 11, 10, VOID, VOID, VOID, VOID, 13, VOID, VOID, 14, VOID, VOID, VOID},
    {VOID, 4, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, 14, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, 7, VOID, VOID, VOID, VOID, VOID, VOID},
    {VOID, VOID, 17, 13, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, 5},
    {VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, VOID, 5},
};

int visitedMap[TAM] = {0}; // --- ARRAY DE INDICES VISITADOS ---

// --- PROCURAR NÓ ---
Node* search(Node* node, int id, int value) {
    if (node == NULL || (node->id == id && node->value == value)) {
        return node;
    }
    if (value < node->value) {
        return search(node->left, id, value);
    } else {
        return search(node->right, id, value);
    }
    return node;
}

// --- MENOR VALOR NA ÁRVORE ---
Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

// --- REMOVER NÓ ---
Node* removeNode(Node* node, int id, int value) {
    if (node == NULL || (node->id == id && node->value == value)) {
        if(node == NULL){
            return node;
        }
        else {
            if (node->left == NULL) {
                Node* temp = node->right;
                free(node);
                return temp;
            }
            else if (node->right == NULL) {
                Node* temp = node->left;
                free(node);
                return temp;
            }
            Node* temp = minValueNode(node->right);
            node->id = temp->id;
            node->value = temp->value;
            node->realValue = temp->realValue;
            node->right = removeNode(node->right, temp->id, temp->value);
        }
    }
    if (value < node->value) {
        node->left = removeNode(node->left, id, value);
    } 
    else{
        node->right = removeNode(node->right, id, value);
    } 
    
    return node;
}

// --- CRIAR NÓ ---
Node* newNode(int id, int value, int realValue) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->id = id;
    node->value = value;
    node->realValue = realValue;
    node->visited = FALSE;
    node->left = NULL;
    node->right = NULL;
    node->father = NULL;
    return node;
}

// --- INSERIR NÓ (PROCURA NULL PARA INSERIR) ---
Node* insert(Node* node, int id, int value, int realValue) {
    if (node == NULL) {
        return newNode(id, value, realValue);
    }
    if (value < node->value) {
        node->left = insert(node->left, id, value, realValue);
    }
    else{
        node->right = insert(node->right, id, value, realValue);
    }
    return node;
}

// --- IMPRIMIR ARVORE ---
void print_root(Node* node) {
    if (node != NULL) {
        print_root(node->left);
        printf("%d(%d)(%d) ", node->id, node->value, node->realValue);
        print_root(node->right);
    }
}

// --- INSERIR PONTEIRO PAI NUM NÓ FILHO ---
void *insert_father(Node* node, Node* father){
    node->father = father;
}

// --- INSERIR FILHOS DE UM NÓ NA ARVORE ---
Node* insert_children(Node* root, Node* queue, Node* currentNode, int arrival){
    int i;
    for (i = 0; i < TAM; i++)
    {
        if(adjacenceMap[currentNode->id - 1][i] != VOID && !visitedMap[i]){
            int realDistance = adjacenceMap[currentNode->id - 1][i] + currentNode->realValue; // g(childrenNode)
            int distance = realDistance + heuristicMap[i][arrival-1]; // f(childrenNode) = g(childrenNode) + h(childrenNode)
            insert(root, i + 1, distance, realDistance); // --- INSERE O FILHO NA ARVORE ---
            insert(queue, i+1, distance, realDistance); // --- INSERE O FILHO NA FILA ---
            insert_father(search(root, i+1, distance), currentNode); // --- INSERE O PAI DO NÓ ---
        }
    }
    return root;
}

// --- PRINTAR ROTA(PELO PAI) ---
void print_route(Node *node){
    if(node->father != NULL){
        print_route(node->father);
        printf("- ");
    }
    printf("%d ", node->id);
}

// --- INICIAR BUSCA ---
void init_(Node *root, Node *queue, int arrival){
    while (queue != NULL)
    {
        Node *currentNodeQueue = minValueNode(queue); // --- NÓ ATUAL DA FILA ---
        Node *currentNode = search(root, currentNodeQueue->id, currentNodeQueue->value);  // --- NÓ ATUAL DA ARVORE ---
        if(currentNodeQueue->id == arrival){
            printf("Meno caminho: ");
            print_route(currentNode);
            printf("\n");
            break;
        }
        if(!visitedMap[currentNodeQueue->id-1]){
            int i;
            for (i = 0; i < TAM; i++)
            {
                if(adjacenceMap[currentNode->id - 1][i] != VOID && !visitedMap[i]){
                    int realDistance = adjacenceMap[currentNode->id - 1][i] + currentNode->realValue; // g(childrenNode)
                    int distance = realDistance + heuristicMap[i][arrival-1]; // f(childrenNode) = g(childrenNode) + h(childrenNode)
                    insert(root, i + 1, distance, realDistance); // --- INSERE O FILHO NA ARVORE ---
                    insert(queue, i+1, distance, realDistance); // --- INSERE O FILHO NA FILA ---
                    insert_father(search(root, i+1, distance), currentNode); // --- INSERE O PAI DO NÓ ---
                }
            }
            visitedMap[currentNodeQueue->id-1] = TRUE;
        }
        queue = removeNode(queue, currentNodeQueue->id, currentNodeQueue->value);
    }
}

int main(){

    int arrival, current; // --- VARIÁVEIS DE ORIGEM E DESTINO ---

    printf("Digite a origem e o destino, respectivamente:\n");
    scanf("%d%d", &current, &arrival);


    Node* queue = NULL; // --- FILA DE PRIORIDADE (ARVORE BINÁRIA) ---
    Node* root = NULL; // --- ARVORE DE ESTADOS ---

    queue = insert(queue, current, heuristicMap[current-1][arrival-1], 0); // --- INICIAR FILA DE PRIORIDADE ---

    root = insert(root, current, heuristicMap[current-1][arrival-1], 0); // --- INICIAR ARVORE ---

    init_(root, queue, arrival); // --- INICIAR ---

    free(root);
    free(queue);


    return 0;
}
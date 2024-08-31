#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct node{
    int num;
    struct node *left;
    struct node *right;
    struct node *father;
} No;

int fator_balanceamento(No *node);

No *rotacao_simples_esquerda(No *rot);

No *rotacao_simples_direita(No *rot);

No *rotacao_dupla_esq(No *rot);

No *rotacao_dupla_dir(No *rot);

No *balanceamento(No *node);

No *criar_no(){
    No *node = (No *)malloc(sizeof(No));

    printf("Digite o numero: ");
    scanf("%d", &node->num);
    
    node->left = NULL;
    node->right = NULL;
    node->father = NULL;

    return node;
}

void inserir_no(No **node, No *novo){

    if(node == NULL){
        return;
    }

    if((*node)->num == novo->num){
        return;
    }

    if((*node)->num > novo->num){
        if((*node)->left == NULL){
            (*node)->left = novo;
            novo->father = *node;
            return;
        }
        inserir_no(&(*node)->left, novo);
    } else {
        if((*node)->right == NULL){
            (*node)->right = novo;
            novo->father = *node;
            return;
        }
        inserir_no(&(*node)->right, novo);
    }
    (*node) = balanceamento((*node));
}

No *buscar_no(No *node, int num){
    if(node == NULL){
        printf("Error!");
        return NULL;
    }

    if(node->num == num){
        return node;
    }

    if(node->num > num){
        return buscar_no(node->left, num);
    }

    if(node->num < num){
        return buscar_no(node->right, num);
    }
}

void imprimir(No *node, int tab = 0){
    if(node == NULL){
        return;
    }

    int i;

    imprimir(node->right, tab + 1);
    for(i = 0; i < tab; i++){
        printf("\t");
    }
    printf("%d\n", node->num);
    imprimir(node->left, tab + 1);
}

void inserir_raiz(No **raiz){
    if(*raiz == NULL){
        *raiz = criar_no();
        return;
    }

    No *novo = criar_no();
    inserir_no(&(*raiz), novo);
}

int buscar_raiz(No *raiz){
    int num;
    printf("Digite o valor a buscar: ");
    scanf("%d", &num);

    No *res = buscar_no(raiz, num);

    if(res == NULL){
        printf("O valor não existe!");
    } else {
        imprimir(res, 0);
    }
}

No *avo_no(No *node){
    No *pai = node->father;

    return pai->father;
}

int altura(No *node){
    if(node == NULL){
        return 0;
    } 

    if((node->left == NULL) && (node->right == NULL)){
        return 1;
    }

    return 1 + fmax(altura(node->left), altura(node->right));
}

int altura_no(No *node, int num){
    No *buscado = buscar_no(node, num);

    if(buscado == NULL){
        printf("Error!");
        return 0;
    }

    return altura(buscado);
}

int fator_balanceamento(No *node){
    return altura(node->left) - altura(node->right);
}

No *rotacao_simples_esquerda(No *rot){ // Rot é o nó a ser rotacionado
    No *novo = rot->right; // Nó a direita do nó detectado
    No *esq_novo = novo->left; // Esquerda do novo

    novo->left = rot;
    rot->right = esq_novo;

    if(esq_novo != NULL){
        esq_novo->father = rot;
    }
    novo->father = rot->father;
    rot->father = novo;

    return novo;
}

No *rotacao_simples_direita(No *rot){
    No *novo = rot->left;
    No *dir_novo = novo->right;

    novo->right = rot;
    rot->left = dir_novo;

    if(dir_novo != NULL){
        dir_novo->father = rot;
    }
    novo->father = rot->father;
    rot->father = novo;

    return novo;
}

No *rotacao_dupla_esq(No *rot){
    rot->left = rotacao_simples_esquerda(rot->left); // Rotaciona o nó -1
    
    return rotacao_simples_direita(rot); // Porque aqui rotaciona a raiz
}

No *rotacao_dupla_dir(No *rot){
    rot->right = rotacao_simples_direita(rot->right);

    return rotacao_simples_esquerda(rot);
}

No *balanceamento(No *node){
    if(node == NULL){
        return NULL;
    }

    node->left = balanceamento(node->left); 
    node->right = balanceamento(node->right); // Recursive

    int fb = fator_balanceamento(node);

    if(fb > 1){
        if(fator_balanceamento(node->left) == -1){
            return rotacao_dupla_dir(node);
        } else {
            return rotacao_simples_direita(node);
        }
    } else {
        if(fb < -1){
            if(fator_balanceamento(node->right) == 1){
                return rotacao_dupla_esq(node);
            } else {
                return rotacao_simples_esquerda(node);
            }
        }
    }
    return node;
}

void balancear_raiz(No **raiz){
    *raiz = balanceamento(*raiz);
}

No *buscar_pai(No *node, int num){
    if(node == NULL) return NULL;

    if((node->left != NULL && node->left->num == num) || (node->right != NULL && node->right->num == num)){
        return node;
    }

    if(node->num > num){
        return buscar_pai(node->left, num);
    } else {
        return buscar_pai(node->right, num);
    }
}

No *maior(No *node){
    if(node == NULL) return NULL;

    if(node->right == NULL){
        return node;
    } else {
        return maior(node->right);
    }
}

bool folha(No *node){
    if(node->left == NULL && node->right == NULL){
        return true;
    }
    return false;
}

bool um_filho(No *node){
    if(node->left != NULL || node->right != NULL){
        return true;
    }
    return false;
}

No *remover_no(No *node, int num){
    if(node == NULL) return NULL;

    if(node->num == num){ // No encontrado
        if(folha(node) == true){ // Caso 1: é folha
            free(node);
            return NULL;
        } else  if(um_filho(node) == true){ // Caso 2: 1 filho
            No *remover = NULL;

            if(node->left != NULL){
                remover = node->left;
            } else {
                remover = node->right;
            }
            free(node);
            return remover;
        } else { // Caso 3: 2 filhos
            No *trocar = maior(node->left);

            remover_no(node, trocar->num);
            trocar->left = node->left;
            trocar->right = node->right;
            node->left = NULL;
            node->right = NULL;
            free(node);
            node = trocar;
            return node;
        }
    } else { // No ainda nao encontrado
        if(node->num > num){
            node->left = remover_no(node->left, num);
        } else {
            node->right = remover_no(node->right, num);
        }
    }
    node = balanceamento(node);
    return node;
}

void menu(){
    
    printf("##########################################\n");
    printf("################## MENU ##################\n");
    printf("# 1 - Inserir Elemento                   #\n");
    printf("# 2 - Altura da Arvore                   #\n");
    printf("# 3 - Imprimir Arvore                    #\n");
    printf("# 4 - Altura do No                       #\n");
    printf("# 5 - Remover                            #\n");
    printf("# 0 - Sair                               #\n");
    printf("##########################################\n");
    printf("Selecione: ");
}

int main(){

    No *node = NULL;

    int op;

    int num, toRemove;

    do{
        menu();
        scanf("%d", &op);
        system("cls || clear");

        switch(op){
            case 1: 
                inserir_raiz(&node); // Funcionando
                break;
            case 2:
                printf("Altura da arvore: %d", altura(node));
                break;
            case 3:
                imprimir(node, 1); // Funcionando
                break;
            case 4:
                printf("Numero a se buscar: "); // Funcionando
                scanf("%d", &num);
                printf("Altura do no: %d", altura_no(node, num));
                break;
            case 5:
                printf("Elemento a remover: "); // Funcionando
                scanf("%d", &toRemove);
                node = remover_no(node, toRemove);
                break;
            case 0:
                printf("Closing...\n");
                break;
            default:
                printf("Invalid option!\n");
                break;
        }

    } while (op != 0);

    return 0;
}
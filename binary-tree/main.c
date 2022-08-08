#include <stdio.h>
#include <stdlib.h>

enum TCompare {
    LESS_THAN = -1,
    GREATER_THAN = 1,
    EQUALS = 0
} typedef Compare;

enum TBalanceFactor {
    UNBALANCED_RIGHT = 1,
    SLIGHTLY_UNBALANCED_RIGHT,
    BALANCED,
    SLIGHTLY_UNBALANCED_LEFT,
    UNBALANCED_LEFT
};

struct TNode {
    int value;
    struct TNode * right;
    struct TNode * left;
} typedef Node;

struct TAvlTree {
    struct TNode * root;
} typedef AvlTree;

Node * createNode(int valueOfNode) {
    Node * node = (Node *)malloc(sizeof(Node));
    node->value = valueOfNode;
    node->left = NULL;
    node->right = NULL;
    return node;
}

AvlTree * createTree() {
    AvlTree * avlTree = (AvlTree *)malloc(sizeof(AvlTree));
    avlTree->root = NULL;
    return avlTree;
}

int compareValues(int valueOne, int valueTwo) {
    if(valueOne < valueTwo) return LESS_THAN;
    if(valueOne > valueTwo) return GREATER_THAN;
    return EQUALS;
}

int max(int valueOne, int valueTwo) {
    return valueOne > valueTwo ? valueOne : valueTwo;
}

Node * minNode(Node * node) {
    Node * current = node;
    while(current != NULL && current->left != NULL) current = current->left;
    return current;
}

Node * rotationLL(Node * node) {
    Node * tmp = node->left;
    node->left = tmp->right;
    tmp->right = node;
    return tmp;
}

Node * rotationRR(Node * node) {
    Node * tmp = node->right;
    node->right = tmp->left;
    tmp->left = node;
    return tmp;
}

Node * rotationLR(Node * node) {
    node->left = rotationRR(node->left);
    return rotationLL(node);
}

Node * rotationRL(Node * node) {
    node->right = rotationLL(node->right);
    return rotationRR(node);
}

Node * insertNode(Node * node, int element) {
    if(node == NULL) {
        return createNode(element);
    } else if(compareValues(element, node->value) == LESS_THAN) {
        node->left = insertNode(node->left, element);
    } else if(compareValues(element, node->value) == GREATER_THAN) {
        node->right = insertNode(node->right, element);
    } else {
        return node;
    }

    int balanceFactor = getBalanceFactor(node);

    if(balanceFactor == UNBALANCED_LEFT) {
        if(compareValues(element, node->left->value) == LESS_THAN) {
            node = rotationLL(node);
        } else {
            return rotationLR(node);
        }
    }

    if(balanceFactor == UNBALANCED_RIGHT) {
        if(compareValues(element, node->right->value) == GREATER_THAN) {
            node = rotationRR(node);
        } else {
            return rotationRL(node);
        }
    }

    return node;
}

Node * removeNode(Node * node, int element) {
    Node * tmp;

    if(node == NULL) return node;

    if(compareValues(element, node->value) == LESS_THAN) {
        node->left = removeNode(node->left, element);
        return node;
    } else if(compareValues(element, node->value) == GREATER_THAN) {
        node->right = removeNode(node->right, element);
        return node;
    } else {
        if(node->left == NULL && node->right == NULL) {
            node = NULL;
            return NULL;
        }

        if(node->left == NULL) {
            tmp = node;
            node = node->right;
            free(tmp);
            return node;
        } else if(node->right == NULL) {
            tmp = node;
            node = node->left;
            free(tmp);
            return node;
        }

        Node * aux = minNode(node->right);
        node->value = aux->value;
        node->right = removeNode(node->right, aux->value);

        return node;
    }
}

void insert(AvlTree * avlTree, int element) {
    avlTree->root = insertNode(avlTree->root, element);
}

Node * removeFn(Node * node, int element) {
    node = removeNode(node, element);

    if(node == NULL) return node;

    int balanceFactor = getBalanceFactor(node);

    if(balanceFactor == UNBALANCED_LEFT) {
        int balanceFactorLeft = getBalanceFactor(node->left);

        if(balanceFactorLeft == BALANCED || balanceFactorLeft == SLIGHTLY_UNBALANCED_LEFT) {
            return rotationLL(node);
        }

        if(balanceFactorLeft == SLIGHTLY_UNBALANCED_RIGHT) {
            return rotationLR(node->left);
        }
    }

    if(balanceFactor == UNBALANCED_RIGHT) {
        int balanceFactorRight = getBalanceFactor(node);

        if(balanceFactorRight == BALANCED || balanceFactorRight == SLIGHTLY_UNBALANCED_RIGHT) {
            return rotationRR(node);
        }

        if(balanceFactorRight == SLIGHTLY_UNBALANCED_LEFT) {
            return rotationRL(node->right);
        }
    }

    return node;
}

int getNodeHeight(Node * node) {
    if(node == NULL) return -1;
    return max(getNodeHeight(node->left), getNodeHeight(node->right)) + 1;
}

int getBalanceFactor(Node * node) {
    int heightDifference = getNodeHeight(node->left) - getNodeHeight(node->right);

    switch(heightDifference) {
        case -2:
            return UNBALANCED_RIGHT;
        case -1:
            return SLIGHTLY_UNBALANCED_RIGHT;
        case 1:
            return SLIGHTLY_UNBALANCED_LEFT;
        case 2:
            return UNBALANCED_LEFT;
        default:
            return BALANCED;
    }
}

void printTree(Node * root, int space) {
    if (root == NULL) return;
    space += 3;
    printTree(root->right, space);

    printf("\n");
    for (int i = 3; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->value);
    printTree(root->left, space);

}

void cleanAvlTree(Node * root) {
    if (root == NULL) return;

    cleanAvlTree(root->right);
    cleanAvlTree(root->left);
    free(root);
}

Node * searchNode(Node * node, int element) {
    if(node == NULL) return NULL;

    if(compareValues(element, node->value) == LESS_THAN) {
        return searchNode(node->left, element);
    } else if(compareValues(element, node->value) == GREATER_THAN) {
        return searchNode(node->right, element);
    }

    return node;
}

void preOrder(Node * node) {
    if(node != NULL) {
        printf("%d ", node->value);
        preOrder(node->left);
        preOrder(node->right);
    }
}

void posOrder(Node * node) {
    if(node != NULL) {
        preOrder(node->left);
        printf("%d ", node->value);
        preOrder(node->right);
    }
}

void simetricOrder(Node * node) {
    if(node != NULL) {
        preOrder(node->left);
        preOrder(node->right);
        printf("%d ", node->value);
    }
}

void menu(int option, AvlTree * avlTree) {
    int valueOfNode;
    Node * searchResult;

    switch(option) {
        case 0:
            printf("\nEncerrando programa!\n");
        break;
        case 1:
            printf("\nDigite o valor do no que deseja inserir: ");
            scanf("%d", &valueOfNode);

            insert(avlTree, valueOfNode);
            printTree(avlTree->root, 0);
        break;
        case 2:
            printf("\nDigite o valor do no que deseja remover: ");
            scanf("%d", &valueOfNode);

            removeFn(avlTree->root, valueOfNode);
            printTree(avlTree->root, 0);
        break;
        case 3:
            printf("\nDigite o valor que voce deseja pesquisar: ");
            scanf("%d", &valueOfNode);

            searchResult = searchNode(avlTree->root, valueOfNode);

            if(searchResult != NULL) {
                printf("\n Valor existe na arvore \n");
            } else {
                printf("\n Valor nao existe na arvore \n");
            }

        break;
        case 4:
            printf("\n");
                preOrder(avlTree->root);
            printf("\n");
        break;
        case 5:
            printf("\n");
                posOrder(avlTree->root);
            printf("\n");
        break;
        case 6:
            printf("\n");
                simetricOrder(avlTree->root);
            printf("\n");
        break;
        default:

            printf("\nOpcao invalida!\n");
        break;
    }

    printf("\n");
}

int main()
{
    int option;
    AvlTree * avlTree = createTree();

    do {
        printf("0 - Encerrar o programa\n1 - Inserir no na arvore\n2 - Remover no da arvore\n3 - Pesquisar valor\n4 - Percuso em Pre-order\n5 - Percuso em Pos-ordem\n6 - Percuso em ordem simetrica\n\n");
        scanf("%d", &option);

        menu(option, avlTree);
    } while(option != 0);

    cleanAvlTree(avlTree->root);
    free(avlTree);
}

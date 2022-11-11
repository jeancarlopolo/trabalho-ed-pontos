#include "cpt.h"
#include "math.h"
#include "stdbool.h"

struct circulo
{
    double x, y, raio;
};

struct node
{
    double x, y;
    Info info;
    struct circulo circ;
    struct node *esq;
    struct node *dir;
};

struct arvore
{
    struct node *raiz;
    double epsilon;
};

/** x1 < x2 = -1
 *  x1 = x2 = 0
 *  x1 > x2 = 1
 */
int comparar(double x1, double x2, double y1, double y2, double epsilon)
{
    if (fabs(x1 - x2) < epsilon && fabs(y1 - y2) < epsilon)
    {
        return 0;
    }
    else if (x1 < x2 || (x1 == x2 && y1 < y2))
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

CPTree createCPT(double epsilon)
{
    struct arvore *a = malloc(sizeof(struct arvore));
    a->raiz = NULL;
    a->epsilon = epsilon;
    return a;
}

bool insertCPT(CPTree b, double x, double y, Info info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra)
{
    struct arvore *a = b;
    bool inseriu;
    // se a arvore for vazia, cria um novo no e o insere na raiz
    if (a->raiz == NULL)
    {
        struct node *novo = malloc(sizeof(struct node));
        novo->x = x;
        novo->y = y;
        novo->info = info;
        novo->esq = NULL;
        novo->dir = NULL;
        a->raiz = novo;
        return true;
    }
    // se nao for vazia chama a funcao recursiva na raiz
    else
    {
        return insertCPTNode(a->raiz, x, y, info, vfindo, vfvoltando, extra, b);
    }
}

bool insertCPTNode(TreeNode a, double x, double y, Info info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra, CPTree b)
{
    struct node *atual = a;
    struct arvore *arv = b;
    bool inseriu;
    // Se o elemento for igual ao que eu quero inserir, retorna falso
    if (comparar(atual->x, x, atual->y, y, arv->epsilon) == 0)
    {
        return false;
    }
    else
    {
        // se for menor vai pra esquerda
        if (comparar(x, atual->x, y, atual->y, arv->epsilon) == -1)
        {
            // se o no da esquerda for vazio, cria um novo no e insere
            if (atual->esq == NULL)
            {
                struct node *novo = malloc(sizeof(struct node));
                novo->x = x;
                novo->y = y;
                novo->info = info;
                novo->esq = NULL;
                novo->dir = NULL;
                atual->esq = novo;
                return true;
            }
            // se nao for vazio chama a funcao recursiva
            else
            {
                if (vfindo != NULL)
                {
                    vfindo(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                inseriu = insertCPTNode(atual->esq, x, y, info, vfindo, vfvoltando, extra, b);
                if (vfvoltando != NULL)
                {
                    vfvoltando(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                return inseriu;
            }
        }
        // se for maior vai pra direita
        else
        {
            // se o no da direita for vazio, cria um novo no e insere
            if (atual->dir == NULL)
            {
                struct node *novo = malloc(sizeof(struct node));
                novo->x = x;
                novo->y = y;
                novo->info = info;
                novo->esq = NULL;
                novo->dir = NULL;
                atual->dir = novo;
                return true;
            }
            // se nao for vazio chama a funcao recursiva
            else
            {
                if (vfindo != NULL)
                {
                    vfindo(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                inseriu = insertCPTNode(atual->dir, x, y, info, vfindo, vfvoltando, extra, b);
                if (vfvoltando != NULL)
                {
                    vfvoltando(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                return inseriu;
            }
        }
    }
}

bool searchCPT(CPTree b, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra)
{
    struct arvore *a = b;
    if (a->raiz == NULL)
    {
        return false;
    }
    // se nao for vazia chama a funcao recursiva na raiz
    else
    {
        return searchCPTNode(a->raiz, x, y, info, vfindo, vfvoltando, extra, b);
    }
}

/** Função privada recursiva que busca um elemento na árvore. */
bool searchCPTNode(TreeNode a, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra, CPTree b)
{
    struct node *atual = a;
    struct arvore *arv = b;
    bool achou;
    // Se achou o elemento, retorna true e o info
    if (comparar(atual->x, x, atual->y, y, arv->epsilon) == 0)
    {
        *info = atual->info;
        return true;
    }
    else
    {
        // se for menor vai pra esquerda
        if (comparar(x, atual->x, y, atual->y, arv->epsilon) == -1)
        {
            // se o no da esquerda for vazio, retorna falso
            if (atual->esq == NULL)
            {
                return false;
            }
            // se nao for vazio chama a funcao recursiva
            else
            {
                if (vfindo != NULL)
                {
                    vfindo(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                achou = searchCPTNode(atual->esq, x, y, info, vfindo, vfvoltando, extra, b);
                if (vfvoltando != NULL)
                {
                    vfvoltando(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                return achou;
            }
        }
        // se for maior vai pra direita
        else
        {
            // se o no da direita for vazio, retorna falso
            if (atual->dir == NULL)
            {
                return false;
            }
            // se nao for vazio chama a funcao recursiva
            else
            {
                if (vfindo != NULL)
                {
                    vfindo(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                achou = searchCPTNode(atual->dir, x, y, info, vfindo, vfvoltando, extra, b);
                if (vfvoltando != NULL)
                {
                    vfvoltando(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                }
                return achou;
            }
        }
    }
}

bool removeCPT(CPTree b, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra)
{
    struct arvore *a = b;
    if (a->raiz == NULL)
    {
        return false;
    }
    // se nao for vazia chama a funcao recursiva na raiz
    else
    {
        return removeCPTNode(a->raiz, x, y, info, vfindo, vfvoltando, extra, b);
    }
}

/** Função privada recursiva que remove um elemento na árvore. */
bool removeCPTNode(TreeNode a, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra, CPTree b)
{
    struct node *atual = a;
    struct arvore *arv = b;
    struct node *aux;
    bool achou;
    if (comparar(atual->x, x, atual->y, y, arv->epsilon) == 0)
    {
        *info = atual->info;
        // Se nao tiver filhos, remove o no
        if (atual->esq == NULL && atual->dir == NULL)
        {
            aux = atual;
            a = NULL;
            free(aux);
            return true;
        }
        // Se tiver um filho, remove o no e o filho vai pra cima
        else if (atual->esq == NULL || atual->dir == NULL)
        {
            if (atual->esq == NULL)
            {
                aux = atual;
                a = atual->dir;
                free(aux);
                return true;
            }
            else
            {
                aux = atual;
                a = atual->esq;
                free(aux);
                return true;
            }
        }
        // Se tiver dois filhos, remove o no e o sucessor vai pra cima
        else
        {
            aux = sucessor(atual->dir);
            atual->x = aux->x;
            atual->y = aux->y;
            atual->info = aux->info;
            atual->circ = aux->circ;
            removeCPTNode(atual->dir, aux->x, aux->y, info, vfindo, vfvoltando, extra, b);
            return true;
        }
    }
    else
    {
        return false;
    }
}

TreeNode sucessor(TreeNode a)
{
    struct node *atual = a;
    while (atual->esq != NULL)
    {
        atual = atual->esq;
    }
    return atual;
}

struct circulo gerarCirculo(TreeNode a)
{
    struct node *atual = a;
    struct circulo circ;
    double maiorx, maiory, menorx, menory, centro, raio;
    // O círculo deve conter dentro dele todos os filhos do node
    // vai achar o maior ponto e o menor ponto dentre os filhos e o elemento
    // dai o centro do círculo vai ser metade da soma das coordenadas
    // raio vai ser metade da distância entre eles
    // toda vez que um elemento for inserido ou removido o círculo de todos os elementos ancestrais deve ser atualizado
    // 🤡🤡🤡
    return circ;
}


// findInRegionCPT, getInfoCPT, getCircCPT, percursoSimetrico, dfs, bfs

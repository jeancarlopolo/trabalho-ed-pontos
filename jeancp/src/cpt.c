#include "cpt.h"
#include "math.h"
#include "stdbool.h"

struct circulo
{
    double x, y, raio;
};

struct node
{
    double x, y, epsilon;
    Info info;
    struct circulo circ;
    struct node *esq;
    struct node *dir;
};

struct arvore
{
    struct node *raiz;
    double epsilon;
    bool isArvore;
};

CPTree createCPT(double epsilon)
{
    struct arvore *a = malloc(sizeof(struct arvore));
    a->raiz = NULL;
    a->epsilon = epsilon;
    a->isArvore = true;
    return a;
}

bool insertCPT(CPTree b, double x, double y, Info info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra)
{
    /**
     * Se a arvore for vazia, cria um novo no e o insere na raiz
     * Se nao for vazia, chama a funcao recursiva
     * ?Mas a função recebe uma arvore e nao um no?
     * ?Como a funcao recursiva vai saber onde ela está?
     * ?Como a funcao recursiva vai saber se o no ja existe?
     * ?Como a funcao recursiva vai saber o epsilon?
     */
    struct arvore *a = b;
    struct node *atual = b;
    bool inseriu;
    // b é um elemento
    if (a->isArvore != true)
    {
        // Se o elemento for igual ao que eu quero inserir, retorna falso
        if (fabs(atual->x - x) < atual->epsilon && fabs(atual->y - y) < atual->epsilon)
        {
            return false;
        }
        else
        {
            // se for menor vai pra esquerda
            if (atual->x > x || (atual->x == x && atual->y > y))
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
                    novo->epsilon = atual->epsilon;
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
                    inseriu = insertCPT(atual->esq, x, y, info, vfindo, vfvoltando, extra);
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
                    novo->epsilon = atual->epsilon;
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
                    inseriu = insertCPT(atual->dir, x, y, info, vfindo, vfvoltando, extra);
                    if (vfvoltando != NULL)
                    {
                        vfvoltando(atual->x, atual->y, atual->info, atual->circ.x, atual->circ.y, atual->circ.raio, extra);
                    }
                    return inseriu;
                }
            }
        }
    }
    // b é uma arvore
    else
    {
        // se a arvore for vazia, cria um novo no e o insere na raiz
        if (a->raiz == NULL)
        {
            struct node *novo = malloc(sizeof(struct node));
            novo->x = x;
            novo->y = y;
            novo->info = info;
            novo->esq = NULL;
            novo->dir = NULL;
            novo->epsilon = a->epsilon;
            a->raiz = novo;
            return true;
        }
        // se nao for vazia chama a funcao recursiva na raiz
        else
        {
            return insertCPT(a->raiz, x, y, info, vfindo, vfvoltando, extra);
        }
    }
}

//TODO: searchCPT, removeCPT, findInRegionCPT, getInfoCPT, getCircCPT, percursoSimetrico, dfs, bfs

#include "lista.h"

struct listanode
{
	Item info;
	struct listanode *ant;
	struct listanode *prox;
	double area;
};

struct lista
{
	struct listanode *l;
	int capacidade;
};

Lista createLista(int capacidade)
{
	struct lista *L = malloc(sizeof(struct lista));
	struct listanode *l = malloc(sizeof(struct listanode));
	L->l = l;
	l->prox = NIL;
	l->info = NIL;
	l->ant = NIL;
	if (capacidade < 0)
	{
		L->capacidade = CAPAC_ILIMITADA;
	}
	else
	{
		L->capacidade = capacidade;
	}
	return L;
}

int length(Lista L)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	int i = 0;
	if (pointernode != NIL)
	{
		if (pointernode->info != NIL)
		{
			i++;
		}
		while (pointernode->prox != NIL)
		{
			pointernode = pointernode->prox;
			i++;
		}
	}
	return i;
}

int maxLength(Lista L)
{
	struct lista *pointer = L;
	return pointer->capacidade;
}

bool isEmpty(Lista L)
{
	if (length(L) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isFull(Lista L)
{
	struct lista *pointer = L;
	if (pointer->capacidade != CAPAC_ILIMITADA && length(pointer) >= maxLength(pointer))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Posic insert(Lista L, Item info, double area)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (isFull(L))
	{
		return NIL;
	}
	else
	{
		while (pointernode->prox != NIL)
		{
			pointernode = pointernode->prox;
		}
		if (!isEmpty(L))
		{
			struct listanode *elemento = malloc(sizeof(struct listanode));
			pointernode->prox = elemento;
			elemento->prox = NIL;
			elemento->info = info;
			elemento->ant = pointernode;
			elemento->area = area;
			return elemento;
		}
		else
		{
			pointernode->info = info;
			return pointernode;
		}
	}
}

Item pop(Lista L, removerItem removedor)
{
	Item valor;
	struct listanode *elemento = getFirst(L);
	if (elemento == NIL)
	{
		return NIL;
	}
	else
	{
		valor = elemento->info;
		remover(L, elemento, removedor);
		return valor;
	}
}

void remover(Lista L, Posic p, removerItem removedor)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (!isEmpty(L))
	{
		if (p == getFirst(L))
		{
			pointer->l = pointernode->prox;
			if (pointernode->prox != NIL)
			{
				pointernode->prox->ant = NIL;
			}
			if (pointernode->info != NIL)
			{
				if (removedor != NIL)
				{
					removedor(pointernode->info);
				}
				else
				{
					pointernode->info = NIL;
				}
			}
			pointernode->prox = NIL;
			free(pointernode);
		}
		else
		{
			while (pointernode->prox != p)
			{
				pointernode = pointernode->prox;
			}
			if (pointernode->prox->info != NIL)
			{
				if (removedor != NIL)
				{
					removedor(pointernode->prox->info);
				}
				else
				{
					pointernode->prox->info = NIL;
				}
			}
			pointernode->prox = pointernode->prox->prox;
			if (pointernode->prox != NIL)
			{
				pointernode->prox->ant = pointernode;
			}
			free(p);
		}
	}
}

Item get(Lista L, Posic p)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (!isEmpty(L))
	{
		pointernode = p;
		return pointernode->info;
	}
	else
	{
		return NIL;
	}
}

Posic insertBefore(Lista L, Posic p, Item info)
{
	struct lista *pointer = L;
	struct listanode *pointernode = p;
	if (isFull(L))
	{
		return NIL;
	}
	else
	{
		if (p == getFirst(L))
		{
			struct listanode *elemento = malloc(sizeof(struct listanode));
			elemento->prox = p;
			elemento->info = info;
			elemento->ant = NIL;
			pointer->l = elemento;
			return elemento;
		}
		else
		{
			struct listanode *elemento = malloc(sizeof(struct listanode));
			elemento->prox = p;
			elemento->info = info;
			elemento->ant = pointernode->ant;
			pointernode->ant->prox = elemento;
			pointernode->ant = elemento;
			return elemento;
		}
	}
}

Posic insertAfter(Lista L, Posic p, Item info)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (isFull(L))
	{
		return NIL;
	}
	else
	{
		pointernode = p;
		struct listanode *elemento = malloc(sizeof(struct listanode));
		elemento->prox = pointernode->prox;
		pointernode->prox = elemento;
		elemento->info = info;
		elemento->ant = pointernode;
		return elemento;
	}
}

Posic getFirst(Lista L)
{
	struct lista *pointer = L;
	return pointer->l;
}

Posic getNext(Lista L, Posic p)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (!isEmpty(L))
	{
		pointernode = p;
		return pointernode->prox;
	}
	else
	{
		return NIL;
	}
}

Posic getLast(Lista L)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (!isEmpty(L))
	{
		while (pointernode->prox != NIL)
		{
			pointernode = pointernode->prox;
		}
		return pointernode;
	}
	else
	{
		return NIL;
	}
}

Posic getPrevious(Lista L, Posic p)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	if (!isEmpty(L))
	{
		pointernode = p;
		return pointernode->ant;
	}
	else
	{
		return NIL;
	}
}

void killLista(Lista L, removerItem removedor)
{
	while (!isEmpty(L))
	{
		remover(L, getFirst(L), removedor);
	}
	free(L);
}

/* A utility function to swap two elements */
void swap(struct listanode *a, struct listanode *b)
{
	struct listanode *temp = a;
	a = b;
	b = temp;
}
// função que cria um vetor com os elementos da lista, embaralha o vetor e depois recria a lista com os elementos do vetor
void shuffle(Lista L)
{
	struct lista *pointer = L;
	struct listanode *pointernode = pointer->l;
	int i = 0;
	int n = length(L);
	Item *vetor = malloc(n * sizeof(Item));
	while (pointernode != NIL)
	{
		vetor[i] = pointernode->info;
		pointernode = pointernode->prox;
		i++;
	}
	srand(time(NULL));
	for (int i = n - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		Item temp = vetor[i];
		vetor[i] = vetor[j];
		vetor[j] = temp;
	}
	pointernode = pointer->l;
	i = 0;
	while (pointernode != NIL)
	{
		pointernode->info = vetor[i];
		pointernode = pointernode->prox;
		i++;
	}
	free(vetor);
}

/* Considers last element as pivot, places the
pivot element at its correct position in sorted array,
and places all smaller (smaller than pivot) to left
of pivot and all greater elements to right of pivot */
struct listanode *partition(struct listanode *l, struct listanode *h)
{
	// set pivot as h element
	int area = h->area;

	// similar to i = l-1 for array implementation
	struct listanode *i = l->ant;

	// Similar to "for (int j = l; j <= h- 1; j++)"
	for (struct listanode *j = l; j != h; j = j->prox)
	{
		if (j->area <= area)
		{
			// Similar to i++ for array
			i = (i == NULL) ? l : i->prox;

			swap(i, j);
		}
	}
	i = (i == NULL) ? l : i->prox; // Similar to i++
	swap(i, h);
	return i;
}

/* A recursive implementation of quicksort for linked list */
void _quickSort(struct listanode *l, struct listanode *h)
{
	if (h != NULL && l != h && l != h->prox)
	{
		struct listanode *p = partition(l, h);
		_quickSort(l, p->ant);
		_quickSort(p->prox, h);
	}
}

// The main function to sort a linked list.
// It mainly calls _quickSort()
void quickSort(Lista *head)
{
	shuffle(head);
	// Find last node
	struct listanode *h = getLast(head);
	struct listanode *l = getFirst(head);

	// Call the recursive QuickSort
	_quickSort(l, h);
}


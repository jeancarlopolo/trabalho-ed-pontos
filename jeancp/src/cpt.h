#ifndef CPTREE__
#define CPTREE__

#include "lista.h"

/*
 * Uma Arvore Circulo-Planar (CPTree) e' uma arvore binaria de busca que associa uma informacao
 * 'a uma coordenada no plano. A chave de busca nesta arvore e', portanto, a coordenada.
 *
 * A arvore prove operacoes de insercao, busca e remocao de uma informacao associada a sua respectiva coordenada.
 * Alem da operacao de busca simples, a arvore prove uma operacao que retorna os nos cujas chaves estao dentro
 * de uma regiao circular (findInRegion).
 *
 * A fim de facilitar a busca, a arvore tambem mantem, para cada no', uma circunferencia para a qual todas as
 * coordenadas do referido no' e de suas sub-arvores sao internas. Esta circunferencia pode ser obtida por
 * meio da operacao (getCircCPT).
 *
 * Por fim, sao providas operacoes que visitam os nos da arvore. Tais operacoes recebem como parametro
 * uma funcao que e' invocada em cada no' visitado.
 *
 * Outras operacoes tambem podem receber funcoes que visitam os nos durante a execucao da
 * operacao. Tais parametros sao acrescentados 'as operacoes para fins didaticos. Por exemplo, a operacao
 * insertCPT pode receber duas funcoes diferentes. A primeira e' invocada quando o no' e' visitado
 * na 'descida' da arvore ('descida da recursao'). A segunda, na volta ('retorno da recursao').
 */

typedef void *CPTree;
typedef void *TreeNode;
typedef void *Info;

/* Tipo da funcao invocada quando um no' e' visitado. A funcao recebe os seguintes valores
      x,y     - coordenada (chave)
      info    - a informacao associada a coordenada
      xc,yc,r - a circunferencia que engloba as coordenadas do no' visitado e de suas sub-arvores
      extra   - apontador passado para operacao de percurso.
 */
typedef void (*VisitaNo)(double x, double y, Info info, double xc, double yc, double r, void *extra);

/*
  Retorna uma CPTree vazia.
  Comparar 2 numeros reais por igualdade pode gerar resultados incorretos devido
  a sua representacao em memoria. Assim, ao inves de verificar se x1 == x2,
  verifica-se | x1 - x2 | < epsilon.
*/
CPTree createCPT(double epsilon);

/*
  Insere na arvore b, a informacao info associada a coordenada  (x,y).
  A cada no vistado antes da insercao, invoca a funcao "vfindo" (se vfindo != NULL)
  Similarmente, na volta da recursao, apos a insercao, invoca a funcao vfvoltando (se, nao NULL).
  Retorna falso, se ja' chave (x,y) ja' existia armazenada na arvore; verdadeiro, caso contrario.
*/
bool insertCPT(CPTree b, double x, double y, Info info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra);

/*
  Procura a chave (x,y) na 'arvore b. Se encontrou, atribui a info a informacao associada
  a tal chave e retorna verdadeiro; caso contrario, retorna falso.
  As funcoes vfindo, vfvoltando, se diferentes de  NULL, sao invocadas durante a busca,
  como descrito acima.
 */
bool searchCPT(CPTree b, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra);

/*
  Procura a chave (x,y) na 'arvore. Se encontrar, atribui 'a info a informacao associada, remove o no'
  da 'arvore e retorna verdadeiro; caso contrario, retorna falso.
  As funcooes vfindo e vfvoltando (se != NULL) sao invocadas conforme descrito anteriormente.
 */
bool removeCPT(CPTree b, double x, double y, Info *info, VisitaNo vfindo, VisitaNo vfvoltando, void *extra);

/*
  Adiciona 'a lista lres os nos da arvore cujas coordenadas (chaves) estejam dentro
  da regiao circular com centro em (xc,yc) e de raio r.
  Retorna verdadeiro se, pelo menos, 1 no' foi encontrado e inserido em lres; falso,
  caso contrario.
 */
bool findInRegionCPT(CPTree b, double xc, double yc, double r, Lista lres);

/*
  Retorna a informacao associada ao no' n.
  O no' n deve ser um no' valido.
 */
Info getInfoCPT(CPTree b, TreeNode n);

/*
  Atribui a (x,y) e r os parametros do circulo envolvente do no' n.
  O no' n deve ser um no' valido.
 */
void getCircCPT(CPTree b, TreeNode n, double *x, double *y, double *r);

/*
  Faz o percurso simetrico na arvore. A cada no' visitado,
  invoca a funcao vf (se != NULL)
 */
void percursoSimetrico(CPTree b, VisitaNo vf, void *extra);

/*
  Percorre a arvore em profundidade. A cada no' visitado,
  invoca a funcao vf (se != NULL)
 */
void dfs(CPTree b, VisitaNo vf, void *extra);

/*
  Percorre a arvore em largura. A cada no' visitado,
  invoca a funcao vf (se != NULL)
 */

void bfs(CPTree b, VisitaNo vf, void *extra);
#endif

#include "qry.h"
#include "path.h"

struct dados
{
    double *pontosmax;
};

void checar_pontos(double x, double y, Info info, double xc, double yc, double r, void *extra)
{
    struct dados *dados = extra;
    Barco barco = info;
    double *pontos = dados->pontosmax;
    switch (barco_get_tipo(barco))
    {
    case 'r':
        if (barco_get_point_desat(barco) > barco_get_point_destr(barco))
        {
            *pontos += barco_get_point_desat(barco);
        }
        else
        {
            *pontos += barco_get_point_destr(barco);
        }
        break;
    case 'c':
        if (barco_get_point_desat(barco) > barco_get_point_destr(barco))
        {
            *pontos += barco_get_point_desat(barco);
        }
        else
        {
            *pontos += barco_get_point_destr(barco);
        }
        break;
    case 't':
        *pontos += barco_get_point_destr(barco);
        break;
    case 'l':
        *pontos += barco_get_point_destr(barco);
        break;
    default:
        break;
    }
}

double calcular_pontos(CPTree *arvore)
{
    double pontosmax = 0;
    struct dados *pontos = malloc(sizeof(struct dados));
    pontos->pontosmax = &pontosmax;
    dfs(arvore, checar_pontos, pontos);
    return pontosmax;
}

void qry_read(char *path, char *fileName, CPTree *arvore, FILE *svg, Lista *arvoreminas)
{
    CPTree listasSelec = createLista(-1);
    char type[100];
    double x, y, dx, dy, r, na = 1.0, pontos = 0.0, pontosmax = 0.0;
    int id, j, k, idc, cont = 0;
    char *fullPath = (char *)malloc(sizeof(char) * 100);
    Posic elemento;
    Barco *barco;
    joinFilePath(path, fileName, fullPath, 100);
    FILE *qry = fopen(fullPath, "r");
    joinFilePath(path, "relatorio.txt", fullPath, 100);
    FILE *textow = fopen(fullPath, "w");
    if (qry == NULL)
    {
        printf("Erro ao abrir o arquivo %s", fullPath);
        return;
    }
    elemento = getFirst(arvore);
    /* trocar isso pela funcao q percorre a arvore */
    pontosmax = calcular_pontos(arvore);
    while (true)
    {
        fscanf(qry, "%s", type);
        if (feof(qry))
        {
            break;
        }
        fprintf(textow, "[*] %s", type);
        if (type[0] == 'n' && type[1] == 'a')
        {
            fscanf(qry, "%lf", &na);
            fprintf(textow, " %lf\n", na);
        }
        else if (type[0] == 't')
        {
            if (type[1] == 'p')
            {
                fscanf(qry, "%lf %lf", &x, &y);
                fprintf(textow, " %lf %lf\n", x, y);
                pontos += torpedo(arvore, x, y, svg, textow);
            }
            else if (type[1] == 'r')
            {
                fscanf(qry, "%lf %lf %lf %lf %d", &x, &y, &dx, &dy, &id);
                fprintf(textow, " %lf %lf %lf %lf %d\n", x, y, dx, dy, id);
                torpedo_replicante(arvore, x, y, dx, dy, id, svg, textow);
            }
        }
        else if (type[0] == 'b' && type[1] == 'e')
        {
            fscanf(qry, "%lf %lf %lf", &x, &y, &r);
            fprintf(textow, " %lf %lf %lf\n", x, y, r);
            pontos += bomba_rad(arvore, x, y, r, na, svg, textow);
        }
        else if (type[0] == 'm' && type[1] == 'v')
        {
            if (type[2] == 'h')
            {
                fscanf(qry, "%d %d %lf", &j, &k, &x);
                fprintf(textow, " %d %d %lf\n", j, k, x);
                pontos += move_barco(listasSelec, x, 0, arvoreminas, arvore, svg, j, k, textow);
            }
            else if (type[2] == 'v')
            {
                fscanf(qry, "%d %d %lf", &j, &k, &y);
                fprintf(textow, " %d %d %lf\n", j, k, y);
                pontos += move_barco(listasSelec, 0, y, arvoreminas, arvore, svg, j, k, textow);
            }
        }
        else if (type[0] == 's' && type[1] == 'e')
        {
            if (type[2] == 'c')
            {
                fscanf(qry, "%d %d", &id, &idc);
                fprintf(textow, " %d %d\n", id, idc);
                if (idc <= 10)
                {
                    elemento = getFirst(arvore);
                    while (elemento != NULL)
                    {
                        barco = get(arvore, elemento);
                        switch (barco_get_tipo(barco))
                        {
                        case 'r':
                            if (id == retangulo_get_i(barco_get_info(barco)))
                            {
                                barco_set_capitaoid(barco, idc);
                                insert(listasSelec, barco, barco_get_area(barco));
                                fprintf(textow, "Ret??ngulo selecionado para nau capit?? \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \n??rea: %lf \nX: %lf \nY: %lf \n", retangulo_get_i(barco_get_info(barco)), retangulo_get_corb(barco_get_info(barco)), retangulo_get_corp(barco_get_info(barco)), retangulo_get_area(barco_get_info(barco)), retangulo_get_x(barco_get_info(barco)), retangulo_get_y(barco_get_info(barco)));
                            }
                            break;
                        case 'c':
                            if (id == circulo_get_i(barco_get_info(barco)))
                            {
                                barco_set_capitaoid(barco, idc);
                                insert(listasSelec, barco, barco_get_area(barco));
                                fprintf(textow, "c??rculo selecionado para nau capit?? \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \nraio: %lf \n??rea: %lf \nX: %lf \nY: %lf \n", circulo_get_i(barco_get_info(barco)), circulo_get_corb(barco_get_info(barco)), circulo_get_corp(barco_get_info(barco)), circulo_get_r(barco_get_info(barco)), circulo_get_area(barco_get_info(barco)), circulo_get_x(barco_get_info(barco)), circulo_get_y(barco_get_info(barco)));
                            }
                            break;
                        case 't':
                            if (id == texto_get_i(barco_get_info(barco)))
                            {
                                barco_set_capitaoid(barco, idc);
                                insert(listasSelec, barco, barco_get_area(barco));
                                fprintf(textow, "texto selecionado para nau capit?? \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \nX: %lf \nY: %lf \ntexto: %s\n", texto_get_i(barco_get_info(barco)), texto_get_corb(barco_get_info(barco)), texto_get_corp(barco_get_info(barco)), texto_get_x(barco_get_info(barco)), texto_get_y(barco_get_info(barco)), texto_get_conteudo(barco_get_info(barco)));
                            }
                            break;
                        case 'l':
                            if (id == linha_get_i(barco_get_info(barco)))
                            {
                                barco_set_capitaoid(barco, idc);
                                insert(listasSelec, barco, barco_get_area(barco));
                                fprintf(textow, "linha selecionada para nau capit?? \nid: %d \ncor: %s \nX1: %lf \nY1: %lf \nX2: %lf \nY2: %lf \n", linha_get_i(barco_get_info(barco)), linha_get_cor(barco_get_info(barco)), linha_get_x1(barco_get_info(barco)), linha_get_y1(barco_get_info(barco)), linha_get_x2(barco_get_info(barco)), linha_get_y2(barco_get_info(barco)));
                            }
                            break;
                        default:
                            break;
                        }
                        elemento = getNext(arvore, elemento);
                    }
                }
            }
            else
            {
                fscanf(qry, "%d", &id);
                fprintf(textow, " %d\n", id);
                elemento = getFirst(arvore);
                while (elemento != NULL)
                {
                    barco = get(arvore, elemento);
                    switch (barco_get_tipo(barco))
                    {
                    case 'r':
                        if (retangulo_get_i(barco_get_info(barco)) == id)
                        {
                            insert(listasSelec, barco, barco_get_area(barco));
                            fprintf(textow, "ret??ngulo selecionado \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \n??rea: %lf \nX: %lf \nY: %lf \n", retangulo_get_i(barco_get_info(barco)), retangulo_get_corb(barco_get_info(barco)), retangulo_get_corp(barco_get_info(barco)), retangulo_get_area(barco_get_info(barco)), retangulo_get_x(barco_get_info(barco)), retangulo_get_y(barco_get_info(barco)));
                            break;
                        }
                        break;
                    case 'c':
                        if (circulo_get_i(barco_get_info(barco)) == id)
                        {
                            insert(listasSelec, barco, barco_get_area(barco));
                            fprintf(textow, "c??rculo selecionado \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \nraio: %lf \n??rea: %lf \nX: %lf \nY: %lf \n", circulo_get_i(barco_get_info(barco)), circulo_get_corb(barco_get_info(barco)), circulo_get_corp(barco_get_info(barco)), circulo_get_r(barco_get_info(barco)), circulo_get_area(barco_get_info(barco)), circulo_get_x(barco_get_info(barco)), circulo_get_y(barco_get_info(barco)));
                            break;
                        }
                        break;
                    case 't':
                        if (texto_get_i(barco_get_info(barco)) == id)
                        {
                            insert(listasSelec, barco, barco_get_area(barco));
                            fprintf(textow, "texto selecionado \nid: %d \ncor da borda: %s \ncor do preenchimento: %s \nX: %lf \nY: %lf \ntexto: %s\n", texto_get_i(barco_get_info(barco)), texto_get_corb(barco_get_info(barco)), texto_get_corp(barco_get_info(barco)), texto_get_x(barco_get_info(barco)), texto_get_y(barco_get_info(barco)), texto_get_conteudo(barco_get_info(barco)));
                            break;
                        }
                        break;
                    case 'l':
                        if (linha_get_i(barco_get_info(barco)) == id)
                        {
                            insert(listasSelec, barco, barco_get_area(barco));
                            fprintf(textow, "Linha selecionada \nid: %d \ncor: %s \nX1: %lf \nY1: %lf \nX2: %lf \nY2: %lf \n", linha_get_i(barco_get_info(barco)), linha_get_cor(barco_get_info(barco)), linha_get_x1(barco_get_info(barco)), linha_get_y1(barco_get_info(barco)), linha_get_x2(barco_get_info(barco)), linha_get_y2(barco_get_info(barco)));
                            break;
                        }
                        break;
                    default:
                        break;
                    }
                    elemento = getNext(arvore, elemento);
                }
            }
        }
        else
        {
            continue;
        }
        cont++;
        fprintf(textow, "\n");
    }
    fprintf(textow, "Pontua????o total: %lf\n", pontos);
    fprintf(textow, "Propor????o em pontos obtidos e a pontua????o m??xima: %lf/%lf = %lf\n", pontos, pontosmax, pontos / pontosmax);
    fprintf(textow, "Propor????o entre pontos obtidos e o n??mero de agress??es contidas no arquivo .qry: %lf/%d = %lf\n", pontos, cont, pontos / cont);
    free(fullPath);
    fclose(textow);
    fclose(qry);
}

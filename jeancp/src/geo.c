#include <stdio.h>
#include <stdlib.h>

#include "path.h"
#include "geo.h"
#include "barco.h"
#include "mina.h"

void geo_read(char *path, char *fileName, CPTree *arvore, CPTree *arvoremina)
{
    char type[200];
    double x, y, w, h, r;
    char corb[200], corp[200], text[200];
    int id;
    char ancora[1];
    char fullPath[200];
    joinFilePath(path, fileName, fullPath, 200);
    FILE *geo = fopen(fullPath, "r");
    if (geo == NULL)
    {
        printf("Erro ao abrir o arquivo %s!", fullPath);
        exit(1);
    }
    while (true)
    {
        fscanf(geo, "%s", type);
        if (feof(geo))
        {
            break;
        }
        switch (type[0])
        {
        case 'c':
        {
            fscanf(geo, "%d %lf %lf %lf %s %s", &id, &x, &y, &r, corb, corp);
            Barco *circle = create_barco('c', create_circulo(id, x, y, r, corb, corp));
            insertCPT(arvore, circulo_get_x(circle), circulo_get_y(circle), circle, NULL, NULL, NULL);
            break;
        }
        case 'r':
        {
            fscanf(geo, "%d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, corb, corp);
            Barco *retan = create_barco('r', create_retangulo(id, x, y, w, h, corb, corp));
            insertCPT(arvore, retangulo_get_x(retan), retangulo_get_y(retan), retan, NULL, NULL, NULL);
            break;
        }
        case 't':
        {
            fscanf(geo, "%d %lf %lf %s %s %c", &id, &x, &y, corb, corp, ancora);
            fgets(text, 200, geo);
            Barco *txt;
            if (strcmp(text, "#") == 0)
            {
                txt = createMina(id, x, y, corb, corp, ancora);
                insertCPT(arvoremina, x, y, txt, NULL, NULL, NULL);
            }
            else
            {
                txt = create_barco('t', create_texto(id, x, y, corb, corp, text, ancora));
                insertCPT(arvore, texto_get_x(txt), texto_get_y(txt), txt, NULL, NULL, NULL);
            }
            break;
        }
        case 'l':
        {
            fscanf(geo, "%d %lf %lf %lf %lf %s", &id, &x, &y, &w, &h, corb);
            Barco *linha = create_barco('l', create_linha(id, x, y, w, h, corb));
            insertCPT(arvore, (linha_get_x1(linha) + linha_get_x2(linha)) / 2, (linha_get_y1(linha) + linha_get_y2(linha)) / 2, linha, NULL, NULL, NULL);
            break;
        };
        default:
            break;
        }
    }
    fclose(geo);
}

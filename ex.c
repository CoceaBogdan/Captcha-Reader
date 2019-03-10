#include <stdio.h>
#include <string.h>
#include "bmp_header.h"
#include <stdlib.h>
#include "get_numbers.h"

/*functie-nume fisiere*/
char* nameImg_task(char* name, char* name_task, 
    unsigned int t)
{
    unsigned int i;
    for (i = 0; i < 50; i++) {
        name_task[i] = 0;
    }
    for (i = 0; i < strlen(name) - 4; i++) {
        name_task[i] = name[i];        
    }
    if (t == 1) {
        strcat(name_task, "_task1.bmp");
    }
    if (t == 2) {
        strcat(name_task, "_task2.txt");
    }
    if (t == 3) {
        strcat(name_task, "_task3.bmp");
    }
    return name_task;
}

/*functie pentru padding*/
unsigned int padding(unsigned int width)
{
    unsigned int k = 0;
    while(width % 4 != 0) {
        k++;
        width++;
    }
    return k;
}

unsigned int compare(unsigned int i, unsigned int j, unsigned int* m, unsigned
        int** matrice)
{
    unsigned int n, k, z;
    n = j +5;
    k = 0;
    z = i;

    while (j < n) {
        for (i = z; i < z + 5; i++) {
            if (matrice[j][i] == m[k]) {
                k++;
            }
            else
                return 0;
        }
        if (matrice[j][z+5] == 2) {
            return 0;
        }
        j++;
    }
    return 1;
}

void get_number(unsigned int (*m)[30]);

struct imp_pixel {
    unsigned int c[100];
    unsigned int l[100];
};
struct color_pixel {
    unsigned int r[100];
    unsigned int g[100];
    unsigned int b[100];
};

void eliminate(unsigned int* numbers, struct color_pixel* Color_Pixel,
        unsigned int* elim_nr, unsigned int* w, unsigned int p)
{   
    unsigned int i, j, t;
    for (i = 0; i < *w; i++) {
        for (j = 0; j < p; j++) {
            if (numbers[i] == elim_nr[j])
            {
                for (t = i; t < *w; t++) {
                    numbers[t] = numbers[t + 1];
                    Color_Pixel->r[t] = Color_Pixel->r[t + 1];
                    Color_Pixel->g[t] = Color_Pixel->g[t + 1];
                    Color_Pixel->b[t] = Color_Pixel->b[t + 1];
                }
                *w = *w - 1;
                i--;
                break;
            }
        }
    }
}

int main()
{
    int i, j, f, h, x;
    ///x folosit pt a determina nr biti de padding pe linie
    char name[50], name_task[50]={0}, b[2], a[50] = {10};
    int color[3];
    unsigned char *bmp_Image;
    unsigned int **matrice,elim_nr[50] = {10}, p;//p =nr elemente elim_nr
    FILE *f1;
    FILE *inp;
    struct bmp_fileheader bmp_FileHeader;
    struct bmp_infoheader bmp_InfoHeader;
    struct imp_pixel Imp_Pixel;
    struct color_pixel Color_Pixel;
    unsigned int numbers[100];
    
    /*Citire*/
    inp = fopen("input.txt", "r");
    fgets(name, 50, inp);
    name[strlen(name) - 1] = 0;
    for (i = 0; i < 3; i++){
        fscanf(inp, "%d", &color[i]);
    }
    fgets(b , 2, inp);//citire \n
    fgets(a, 50, inp);

    for (i = 0; i < (int)strlen(a); i++) {
        if (a[i] == ' ') {
            for (j = i; j < (int)strlen(a) - 1; j++)
                a[j] = a[j + 1];
            a[strlen(a) - 1] = 0;
        }
    }
    p = strlen(a) - 1;
    for (i = 0; i < (int)p; i++) {
        elim_nr[i] = (unsigned int)(a[i] - '0');
    }

    /*Atribuire-structuri*/
    f1 = fopen(name, "rb");
    if (f1 == NULL) {
        fprintf(stderr,"ERROR: Can't open file");
        return -1;
    }
    fread(&bmp_FileHeader, sizeof(struct bmp_fileheader), 1, f1);
    fread(&bmp_InfoHeader, sizeof(struct bmp_infoheader), 1, f1);
    fseek(f1, bmp_FileHeader.imageDataOffset, SEEK_SET);
    x = padding(bmp_InfoHeader.width * 3);
    bmp_Image = (unsigned char*)malloc(bmp_InfoHeader.biSizeImage +
                x * bmp_InfoHeader.height);
    if (bmp_Image == NULL) {
        return -2;
    }
    fread(bmp_Image, bmp_InfoHeader.biSizeImage +
            x*bmp_InfoHeader.height, 1, f1);

    matrice = (unsigned int**)malloc(bmp_InfoHeader.height * sizeof(unsigned
       int*));
    if (matrice ==NULL) {
            return -2;
    }
    for (i = 0; i < bmp_InfoHeader.height; i++) {
        matrice[i] = calloc(bmp_InfoHeader.width, sizeof(unsigned int));
        if (matrice[i] == NULL) {
            for (j = 0; j< i; j++) {
                free(matrice[j]);
            }
            free(matrice);
            return -2;
        }
    }

    /*task1*/
    h = bmp_InfoHeader.width * 3 + x;
    for (f = 0; f < bmp_InfoHeader.height; f++) {
        for (i = 0; i < h - x; i = i + 3) {
            if ((bmp_Image[f * h + i] != 255) && (bmp_Image[f * h + i + 1] 
            != 255) && (bmp_Image[f * h + i + 2] != 255)) 
            {   
                matrice[f][i / 3] = 2;
                bmp_Image[f * h + i] = color[0];
                bmp_Image[f * h + i + 1] = color[1];
                bmp_Image[f * h + i + 2] = color[2];
            }
        }
    }
    /*scriere_task1*/
    FILE *f2;
    f2 = fopen(nameImg_task(name, name_task, 1), "wb");
    if (f2 == NULL) {
        fprintf(stderr,"ERROR: Can't open file");
        return -1;
    }
    fwrite(&bmp_FileHeader, sizeof(struct bmp_fileheader), 1, f2);
    fwrite(&bmp_InfoHeader, sizeof(struct bmp_infoheader), 1, f2);
    fwrite(bmp_Image, bmp_InfoHeader.biSizeImage +
            x * bmp_InfoHeader.height, 1, f2);
    fclose(f2);

    /*task2*/
    FILE *f3;
    f3 = fopen(nameImg_task(name, name_task, 2), "w");
    if (f3 == NULL) {
        fprintf(stderr,"ERROR: Can't open file");
        return -1;
    }
    unsigned int t, w=0;
    unsigned int m[10][30] = {0};
    get_number(m);
    for (i = 0; i < bmp_InfoHeader.width - 4; i++) {
        for (j = 0; j < bmp_InfoHeader.height - 4; j++) {
            for (t = 0; t < 10; t++) {
                if (compare(i, j, m[t], matrice) == 1) {
                    fprintf(f3, "%d", t);
                    Imp_Pixel.c[w] = i;
                    Imp_Pixel.l[w] = j;
                    numbers[w] = t;
                    w++;
                    break;
                }  
            }
        }
    }
    fclose(f3);

    /*task3*/
    free(bmp_Image);
    fseek(f1, bmp_FileHeader.imageDataOffset, SEEK_SET);
    bmp_Image = (unsigned char*)malloc(bmp_InfoHeader.biSizeImage + 
    x * bmp_InfoHeader.height);
    if (bmp_Image == NULL) {
        return -2;
    }
    fread(bmp_Image, bmp_InfoHeader.biSizeImage + 
            x * bmp_InfoHeader.height, 1, f1);
    /*determinare culoare cifra*/
    for (t = 0; t < w; t++) {    
        Color_Pixel.r[t] = bmp_Image[(Imp_Pixel.l[t] + 2) * h + 
        (Imp_Pixel.c[t] + 4) * 3];

        Color_Pixel.g[t] = bmp_Image[(Imp_Pixel.l[t] + 2) * h + 
        (Imp_Pixel.c[t] + 4) * 3 + 1];

        Color_Pixel.b[t] = bmp_Image[(Imp_Pixel.l[t] + 2) * h + 
        (Imp_Pixel.c[t] + 4) * 3 + 2];
    }
    eliminate(numbers, &Color_Pixel, elim_nr, &w, p);
    /*setez imaginea astfel incat sa fie alba*/
    for (f = 0; f < bmp_InfoHeader.height; f++) {
        for (i = 0; i < h - x; i = i + 3) {
            if ((bmp_Image[f * h + i] != 255) && (bmp_Image[f * h + i + 1] 
            != 255) && (bmp_Image[f * h + i + 2] != 255))
            {
                bmp_Image[f * h + i] = 255;
                bmp_Image[f * h + i + 1] = 255;
                bmp_Image[f * h + i + 2] = 255;
            }
        }
    }

    /*trecerea in imagine a cifrelor ramase dupa eliminate*/
    for (t = 0; t < w; t++) {
        for (f = 0; f < 25; f++) {
            if (m[numbers[t]][f] == 2) {
                bmp_Image[(Imp_Pixel.l[t]+f/5)*h+(Imp_Pixel.c[t]+f%5)*3]
                    =Color_Pixel.r[t];
                bmp_Image[(Imp_Pixel.l[t]+f/5)*h+(Imp_Pixel.c[t]+f%5)*3+1]
                    =Color_Pixel.g[t];
                bmp_Image[(Imp_Pixel.l[t]+f/5)*h+(Imp_Pixel.c[t]+f%5)*3+2]
                    =Color_Pixel.b[t];
            }
        }
    }
    FILE* f4;
    f4 = fopen(nameImg_task(name, name_task, 3), "wb");
    if (f4 == NULL) {
        fprintf(stderr,"ERROR: Can't open file");
        return -1;
    }
    fwrite(&bmp_FileHeader, sizeof(struct bmp_fileheader), 1, f4);
    fwrite(&bmp_InfoHeader, sizeof(struct bmp_infoheader), 1, f4);
    fwrite(bmp_Image, bmp_InfoHeader.biSizeImage + 
             x * bmp_InfoHeader.height, 1, f4);
    fclose(f4);
    fclose(f1);
    free(bmp_Image);
    for (i = 0; i < bmp_InfoHeader.height; i++) {
        free(matrice[i]);
    }
    free(matrice);
        return 0;
}

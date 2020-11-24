/*
=============================================================
Name        : sudoku_solver.c
Author      : BCPTe
Version     : 1.0 - March 2020
Copyright   : ++NONE++
Description : Sudoku Solver C
=============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define filename "sudoku.txt"
#define righe 9
#define colonne 9

enum{LOCKED=-99, EDITABLE=0};
typedef struct{
    int *vect;
}quadrante;

int **readfile();
int **BONDinit(int **table);
int **CHECKinitR(int **table);
int **CHECKinitC(int **table);
void SUDOKUsolve(int **table);
quadrante *QUADinit(quadrante *quad, int **table);
void SUDOKUsolveR(int r, int c, int **table , int **bond, int **checkR, int **checkC, quadrante quad[], int curr_quad);
int verifyquad(int **table, int quad, int tocheck);

int main(){
    int i, j;
    int **table=readfile();
    printf("\t\t  -GRIGLIA DI PARTENZA-\n");
    for(i=0 ; i<righe ; i++){
        if(i==0 || i==3 || i==6)
            printf("\t\t-------------------------\n");
        printf("\t\t");
        for(j=0 ; j<colonne ; j++){
            if(j==0 || j==3 || j==6)
                printf("| ");
            if(table[i][j]==-1){
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),                //VERDE
                    FOREGROUND_INTENSITY | FOREGROUND_GREEN);                           //VERDE
                printf("+ ");
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),                               //BIANCO CORPO TESTO
                    FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);       //BIANCO CORPO TESTO
            }
            else printf("%d ", table[i][j]);
        }
        printf("|\n");
    }
    printf("\t\t-------------------------\n\n");

    printf("\t     Sto calcolando la soluzione...\n\n");
    SUDOKUsolve(table);

    return EXIT_SUCCESS;
}

int **readfile(){
    char p;
    int i, j;
    int **tmp=malloc(righe*sizeof(int *));
    for(i=0 ; i<righe ; i++)
        tmp[i]=malloc(colonne*sizeof(int));
    FILE *fp=fopen(filename, "r");

    for(i=0 ; i<righe ; i++){
        for(j=0 ; j<colonne ; j++){
            fscanf(fp, "%c ", &p);
            if(p=='*')
                tmp[i][j]=-1;
            else tmp[i][j]=p-'0';
        }
        fscanf(fp, "\n");
    }

    fclose(fp);
    return tmp;
}

int **BONDinit(int **table){
    int i, j;
    int **bond=malloc(righe*sizeof(int *));
    for(i=0 ; i<righe ; i++)
        bond[i]=malloc(colonne*sizeof(int));

    for(i=0 ; i<righe ; i++){
        for(j=0 ; j<colonne ; j++){
            if(table[i][j]!=-1)
                bond[i][j]=LOCKED;
            else bond[i][j]=EDITABLE;
        }
    }


    return bond;
}

int **CHECKinitR(int **table){
    int i, j;
    int **tmp=calloc(righe, sizeof(int *));
    for(i=0 ; i<righe ; i++)
        tmp[i]=calloc(colonne, sizeof(int));

    for(i=0 ; i<righe ; i++){
        for(j=0 ; j<colonne ; j++){
            if(table[i][j]!=-1)
                tmp[i][table[i][j]-1]=1;
        }
    }

    return tmp;
}

int **CHECKinitC(int **table){
    int i, j;
    int **tmp=calloc(righe, sizeof(int *));
    for(i=0 ; i<righe ; i++)
        tmp[i]=calloc(colonne, sizeof(int));

    for(j=0 ; j<colonne ; j++){
        for(i=0 ; i<righe ; i++){
            if(table[i][j]!=-1)
                tmp[table[i][j]-1][j]=1;
        }
    }

    return tmp;
}

quadrante *QUADinit(quadrante *quad, int **table){
    int f=0;
    for(int x=0 ; x<righe ; x+=3){
        for(int y=0 ; y<colonne ; y+=3){
            for(int i=x ; i<3+x ; i++)
                for(int j=y ; j<3+y ; j++)
                    if(table[i][j]!=-1)
                        quad[f].vect[table[i][j]-1]=1;
            f++;
        }
    }

    return quad;
}

void SUDOKUsolve(int **table){
    int **bond=BONDinit(table);
    int **checkR=CHECKinitR(table);
    int **checkC=CHECKinitC(table);
    quadrante *quad=calloc(9, sizeof(quadrante));
    for(int i=0 ; i<9 ; i++)
        quad[i].vect=calloc(9, sizeof(int));
    quad=QUADinit(quad, table);

    SUDOKUsolveR(0, 0, table, bond, checkR, checkC, quad, 0);
}

int QUADcalc(int r, int c){
    if(r<3 && c<3)
        return 1;
    else if(r<3 && c<6)
        return 2;
    else if(r<3 && c<9)
        return 3;
    else if(r<6 && c<3)
        return 4;
    else if(r<6 && c<6)
        return 5;
    else if(r<6 && c<9)
        return 6;
    else if(r<9 && c<3)
        return 7;
    else if(r<9 && c<6)
        return 8;
    else if(r<9 && c<9)
        return 9;
    else return -1;
}

void SUDOKUsolveR(int r, int c, int **table , int **bond, int **checkR, int **checkC, quadrante quad[], int curr_quad){
    int i, j;

    if(r>=righe){
        printf("\t\t       -SOLUZIONE-\n");
        for(i=0 ; i<righe ; i++){
            if(i==0 || i==3 || i==6)
                printf("\t\t-------------------------\n");
            printf("\t\t");
            for(j=0 ; j<colonne ; j++){
                if(j==0 || j==3 || j==6)
                    printf("| ");
                printf("%d ", table[i][j]);
            }
            printf("|\n");
        }
        printf("\t\t-------------------------\n\n");
        return;
    }

    curr_quad=QUADcalc(r, c);

    if(c>=colonne)
        SUDOKUsolveR(r+1, 0, table, bond, checkR, checkC, quad, curr_quad);

    if(bond[r][c]==LOCKED)
        SUDOKUsolveR(r, c+1, table, bond, checkR, checkC, quad, curr_quad);

    for(i=1 ; i<=9 && bond[r][c]==EDITABLE ; i++){
        if(checkR[r][i-1]==1 || checkC[i-1][c]==1 || quad[curr_quad-1].vect[i-1]==1) continue;
        table[r][c]=i; checkR[r][i-1]=1; checkC[i-1][c]=1; quad[curr_quad-1].vect[i-1]=1;
        SUDOKUsolveR(r, c+1, table, bond, checkR, checkC, quad, curr_quad);
        table[r][c]=-1; checkR[r][i-1]=0; checkC[i-1][c]=0; quad[curr_quad-1].vect[i-1]=0;
    }

    return;
}

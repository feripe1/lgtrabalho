#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct
{
    int cod;
    char descr[20];
    float custo;
} registro; 

registro reg;
FILE *Arq;

int main()
{
    system("cls");
    void importMenu(void);
    void escolherOpcao(void);

    importMenu();

    getch();
    return 0;
}

void importMenu(void) {
    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");

    Arq = fopen("PRODUTOS.DAT", "r+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        exit(0);
    }
    while (!feof(Arq)) {
        fread(&reg, sizeof(reg), 1, Arq);
        if (!feof(Arq)) {
            printf("\n%3i %-20s R$ %.2f", reg.cod, reg.descr, reg.custo);
        }
    }
    printf("\n__________________________________\n");
    /* Fecha o arquivo*/
    fclose(Arq);
}

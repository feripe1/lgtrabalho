#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 99

struct produto
{
    int codProd;
    char descrProd[20];
    float custoProd;
} cardapio[MAX];

struct pagamento
{
    int nmPedido;
    char nomePessoa[10];
    char formaPagamento[10];
    float total;
} pagamento[MAX] = {0};

int main()
{
    void importPagamentos(void);
    void importCardapio(void);
    void opcoes(void);

    importPagamentos();
    importCardapio();
    opcoes();

    getch();
    return 0;
}

void opcoes(void) {
    system("cls");
    char opc;

    void adicionarItem(void);
    void verPagamentos(void);
    void retirarItem(void);

    printf("Selecione uma opcao: \n");
    printf("(1) Ver os últimos pagamentos\n(2) Adicionar item ao cardapio\n(3) Retirar item");
    opc = getch();

    switch (opc) {
    case '1':
        verPagamentos();
        break;
    case '2':
        adicionarItem();
        break;

    case '3':
        retirarItem();
        break;
    
    default:
        break;
    }
}

void importCardapio(void) {
    system("cls");

    FILE *Arq;
    Arq = fopen("PRODUTOS.DAT", "r+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        exit(0);
    }
    while (!feof(Arq)) {
        fread(&cardapio, sizeof(cardapio), 1, Arq);
    }
    fclose(Arq);
}

void importPagamentos(void) {
    system("cls");

    FILE *Arq;
    Arq = fopen("PAGAMENTOS.DAT", "r+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        exit(0);
    }
    while (!feof(Arq)) {
        fread(&pagamento, sizeof(pagamento), 1, Arq);
    }
    fclose(Arq);
}

void verPagamentos(void) {
    system("cls");
    int i;

    for (i = 0; i < MAX; i++)
    {
        if (pagamento[i].total != 0)
        {
            printf("%d %s %s %.2f \n", pagamento[i].nmPedido, pagamento[i].nomePessoa, pagamento[i].formaPagamento, pagamento[i].total);
        }
        
    }
    getch();
}

void adicionarItem(void) {
    char opc;
    int cod;
    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w+b");

    if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    do {
        fflush(stdin);

        printf("cod ");
        scanf("%d", &cod);

        cardapio[cod].codProd = cod;

        printf("nome ");
        fflush(stdin);
        gets(cardapio[cod].descrProd);

        printf("preco ");
        fflush(stdin);
        scanf("%f", &cardapio[cod].custoProd);

        printf("Deseja continuar? esq para não");
        opc = getch();
    } while (opc != 27);

    fwrite(&cardapio, sizeof(cardapio), 1, fp);
    fclose(fp);
}

void retirarItem(void) {
    int cod, i;
    system("cls");
    printf("Qual o código do item que deseja retirar?");
    scanf("%d", &cod);

    cardapio[cod].codProd = 0;
    cardapio[cod].custoProd = 0;

    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w+b");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    for (i = 0; i < MAX; i++)
    {
        if (cardapio[i].custoProd != 0)
        {
            fwrite(&cardapio, sizeof(cardapio), 1, fp);
            fclose(fp);
        }
    }

    getch();

}
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 20

int nmPedido;

struct produto
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int demoraMinutos;
} cardapio[MAX];

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    float total;
    bool entregue;
    int demoraTotal;
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

void verCardapio(void)
{
    int i;
    system("cls");

    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");
    for (i = 0; i < MAX; i++)
    {
        if (cardapio[i].codProd != 0)
        {
            printf("\n  %-7i %-15s R$ %.2f", cardapio[i].codProd, cardapio[i].descrProd, cardapio[i].custoProd);
        }
    }
    printf("\n__________________________________\n");
}

void importPagamentos(void) {
    system("cls");

    FILE *Arq;
    Arq = fopen("PAGAMENTOS.DAT", "rb");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        
    }
    while (!feof(Arq))
    {
        fread(&pagamento, sizeof(pagamento), 1, Arq);
    }
    
    fclose(Arq);
}

void verPagamentos(void) {
    system("cls");
    int i;

    for (i = 0; i < MAX; i++)
    {
        if (pagamento[i].total >0)
        {
            printf("%d %s %s %.2f \n", pagamento[i].nmPedido, pagamento[i].nomePessoa, pagamento[i].formaPagamento, pagamento[i].total);
        }
        
    }
    getch();
}

void adicionarItem(void) {
    char opc;
    int cod, aux;
    void verCardapio(void);
    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w");

    if (fp == NULL) {
        printf("Error opening file\n");
        
    }

    do {
        fflush(stdin);
        system("cls");
        verCardapio();
        printf("cod ");
        scanf("%d", &cod);

        cardapio[cod].codProd = cod;

        printf("nome ");
        fflush(stdin);
        gets(cardapio[cod].descrProd);

        printf("preco ");
        fflush(stdin);
        scanf("%f", &cardapio[cod].custoProd);

        printf("Demora? ");
        scanf("%d", &cardapio[cod].demoraMinutos);
        
        printf("Deseja continuar? esq para não");
        opc = getch();
    } while (opc != 27);

    fwrite(&cardapio, sizeof(cardapio), 1, fp);
    fclose(fp);
}

void retirarItem(void) {
    void verCardapio(void);
    int cod, i;
    system("cls");
    verCardapio();
    printf("Qual o código do item que deseja retirar?");
    scanf("%d", &cod);

    cardapio[cod].codProd = 0;
    cardapio[cod].custoProd = 0;

    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w+b");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        
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

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX 5
#define MAXPEDIDOS 99

int count = 0;

struct produto {
    int codProd;
    char descrProd[20];
    float custoProd;
} reg, cardapio[MAX] = {
    {0, "x-burguer", 7.34},
    {1, "x-picanha", 8.777},
    {2, "x-egg", 6.1},
    {3, "cocacola", 9.0},
    {4, "gasolina", 9999.9}};

struct pedido {
    char descrProd[20];
    int qntdProd;
    int nmPedido;
    int codProd;
    float custoProd;
    float subtotal;
    float total;
} pedido[MAXPEDIDOS] = {0};

int main()
{
    void bemvindo(void);
    void escolherOpcao(void);

    bemvindo();
    escolherOpcao();

    getch();
    return 0;
}

void bemvindo(void) {
    int opc;

    void verCardapio(void);
    printf("\n_______________________________________\n");
    printf("\n     Bem vindo ao restaurante x");
    printf("\n_______________________________________\n");
    printf("\n\nAperte qualquer tecla para ver o cardapio. \n");
    getch();

    verCardapio();
}

void verCardapio(void) {
    int i = 0;
    system("cls");
    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");
    
    FILE *Arq;
    Arq = fopen("PRODUTOS.DAT", "r+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        exit(0);
    }
    while (!feof(Arq)) {
        fread(&reg, sizeof(reg), 1, Arq);
        if (!feof(Arq)) {
            printf("\n%3i %-20s R$ %.2f", reg.codProd-1, reg.descrProd, reg.custoProd);
        }
        i++;
    }
    printf("\n__________________________________\n");
    fclose(Arq);
    getch();
}


void escolherOpcao(void) {
    int opc, i;
    void verCardapio(void);
    void opcoesPedido(void);

    do {
        system("cls");
        printf("Opções\n 1 - Ver cardápio\n 2 - Realizar pedido\n 3 - Sair");
        scanf("%d", &opc);
        switch (opc) {
        case 1:
            verCardapio();
            break;
        case 2:
            opcoesPedido();
            break;
        case 3:
            exit(0);
        default:
            printf("Opção inválida");
            break;
        }
    } while (opc != 1 || opc != 2 || opc != 3);
    
}

void opcoesPedido(void){
    int opc;
    void incluirItem(void);
    void removerItem(void);
    void finalizarPedido(void);
    do {
        system("cls");
        printf("Opções do pedido: ");
        printf("\n(1)- Incluir um item\n(2)- Remover um item\n(3)- Finalizar pedido\n(4)- Voltar ");
        scanf("%d", &opc);
        switch (opc) {
        case 1:
            incluirItem();
            break;
        case 2:
            removerItem();
            break;
        case 3:
            finalizarPedido();
            break;
        case 4:
            escolherOpcao();
            break;
        default:
            break;
        }
    } while (opc != 1 || opc != 2 || opc != 3);
    
}

void incluirItem(void) {
    char carrinho(int count, int cod, char* nome, float valorUnitario, int qntd);
    char subtotal(int cod, char* nome, float valorUnitario, int qntd);
    int i, cod, qntd;
    float total;
    char continuar;

    do {
        system("cls");
        verCardapio();

        carrinho(count, pedido[count].codProd, cardapio[count].descrProd, pedido[count].custoProd, pedido[count].qntdProd);

        printf("Qual o codigo do item desejado? ");
        scanf("%d", &cod);
        printf("Qual a quantidade do item desejado? ");
        scanf("%d", &pedido[count].qntdProd);

        pedido[count].codProd = cod;
        pedido[count].custoProd = cardapio[cod].custoProd;
        pedido[count].subtotal = cardapio[cod].custoProd * pedido[count].qntdProd;
        printf("Deu %.2f, mais alguma coisa?", pedido[count].subtotal);

        subtotal(cod, cardapio[cod].descrProd, cardapio[cod].custoProd, pedido[count].qntdProd);
        
        pedido[count].total += pedido[count].subtotal;
        count++;

        printf("%d", count);
        continuar = getch();        
    } while (continuar == 's' || continuar == 'S');
    
}

void removerItem(void) {
    system("cls");
}

void finalizarPedido(void) {
    system("cls");
}

char carrinho(int count, int cod, char *nome, float valorUnitario, int qntd) {
    int i;

    for (i = 0; i < MAXPEDIDOS; i++)
    {
        if (pedido[i].qntdProd > 0)
        {
            printf("--------------------------------\n");
            printf("Produto escolhido: (%d)- %s \n", pedido[i].codProd, pedido[i].descrProd);
            printf("Valor da unidade: %.2f\n", pedido[i].custoProd);
            printf("Quantidade: %d\n", pedido[i].qntdProd);
            printf("Subtotal: %.2f\n", (float)pedido[i].custoProd * pedido[i].qntdProd);
            printf("--------------------------------\n");
        }
        
    }
}
char subtotal(int cod, char* nome, float valorUnitario, int qntd)
{
    system("cls");
    printf("--------------------------------\n");
    printf("Produto escolhido: (%d)- %s \n", cod, nome);
    printf("Valor da unidade: %.2f\n", valorUnitario);
    printf("Quantidade: %d\n", qntd);
    printf("Subtotal: %.2f\n", (float)valorUnitario*qntd);
    printf("--------------------------------\n");
}

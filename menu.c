#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

struct produto {
    int codProd;
    char descrProd[20];
    float custoProd;
};

struct produto reg;

struct pedido {
    int codProd;
    char descrProd[20];
    float custoProd;
    int qntdProd;
    int nmPedido;
};

struct pedido pedido = {};

int main()
{
    system("cls");
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
    printf("\n   Seja bem vindo ao restaurante x");
    printf("\n_______________________________________\n");
    printf("Aperte qualquer tecla para ver o cardapio. \n");
    getch();

    verCardapio();
}

void verCardapio(void) {
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
            printf("\n%3i %-20s R$ %.2f", reg.codProd, reg.descrProd, reg.custoProd);
        }
    }
    printf("\n__________________________________\n");
    fclose(Arq);
    getch();
}


void escolherOpcao(void) {
    int opc;
    void verCardapio(void);
    void opcoesPedido(void);

    do {
        system("cls");
        printf("Opções\n 1 - Ver cardápio\n 2 - Realizar pedido\n 3 - Sair");
        scanf("%d", &opc);
        switch (opc)
        {
        case 1:
            verCardapio();
            break;
        case 2:
            opcoesPedido();
            break;
        case 3:
            exit(0);
        default:
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
        case 4:
            escolherOpcao();
        default:
            break;
        }
    } while (opc != 1 || opc != 2 || opc != 3);
    
}

void incluirItem(void) {
    system("cls");
    printf("Qual o codigo do item desejado? ");
    scanf("%d", pedido.codProd);
    
}

void removerItem(void) {
    system("cls");
}

void finalizarPedido(void) {
    system("cls");
}

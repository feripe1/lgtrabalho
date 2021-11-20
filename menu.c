#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define MAX 5
#define MAXPEDIDOS 99

int count = 0;
int primeiro = 0;
static int qntd = 0;

struct produto {
    int codProd;
    char descrProd[20];
    float custoProd;
} cardapio[MAX];

struct pedido {
    char descrProd[20];
    int qntdProd;
    int nmPedido;
    int codProd;
    float custoProd;
    float subtotal;
    float total;
    int cont;
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
        fread(&cardapio, sizeof(cardapio), 1, Arq);
        if (!feof(Arq)) {
            printf("\n%3i %-20s R$ %.2f", cardapio[i].codProd-1, cardapio[i].descrProd, cardapio[i].custoProd);
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
    void carrinho(void);
    char confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd);
    int i, cod, qntd;
    float total;
    char continuar;

    do {
        system("cls");
        verCardapio();

        carrinho();

        printf("Qual o codigo do item desejado? ");
        scanf("%d", &cod);
        printf("Qual a quantidade do item desejado? ");
        scanf("%d", &qntd);

        pedido[cod].codProd = cod;
        pedido[cod].custoProd = cardapio[cod].custoProd;
        pedido[cod].qntdProd += qntd;
        pedido[cod].subtotal = cardapio[cod].custoProd * pedido[cod].qntdProd;
        pedido[cod].total += pedido[cod].subtotal;

        confirmarEscolha(cod, cardapio[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd);
        continuar = getch();        
    } while (continuar == 's' || continuar == 'S');
    
}

void removerItem(void) {
    system("cls");

    int cod;
    void carrinho(void);
    carrinho();

    printf("Qual o código do item que deseja remover?");
    scanf("%d", &cod);

    pedido[cod].qntdProd = 0;
}

void carrinho(void) {
    int i = 0;

    for (i = 0; i < MAXPEDIDOS; i++) {
        if (pedido[i].qntdProd > 0) {
            primeiro = 1;
            if (i == 0)
            {
                printf("--------------------------CARRINHO----------------------------------\n");
            }
            printf("| Produto      Valor da unidade   Quantidade    Subtotal\n");
            printf("| (%d)- %s        %.2f            %d          %.2f\n",       pedido[i].codProd, 
                                                                                 cardapio[i].descrProd,
                                                                                 pedido[i].custoProd,
                                                                                 pedido[i].qntdProd,
                                                                                 pedido[i].subtotal);
            printf("--------------------------------------------------------------------\n");
        }
        
    }
    if (primeiro == 1)
    {
        printf("| TOTAL   =           %.2f\n", pedido[i].total);
        printf("--------------------------------------------------------------------\n");
    }
    
}

char confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd)
{
    system("cls");
    printf("--------------------------------\n");
    printf("Produto escolhido: (%d)- %s \n", cod, nome);
    printf("Valor da unidade: %.2f\n", valorUnitario);
    printf("Quantidade: %d\n", qntd);
    printf("Subtotal: %.2f\n", (float)valorUnitario*qntd);
    printf("--------------------------------\n");
}

void finalizarPedido(void)
{
    system("cls");
}

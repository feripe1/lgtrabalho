#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5
#define MAXPEDIDOS 10

static int qntd = 0;

struct produto {
    int codProd;
    char descrProd[20];
    float custoProd;
} cardapio[MAX];

struct pedido {
    int nmPedido;
    int codProd;
    char descrProd[20];
    float custoProd;
    int qntdProd;
    float subtotal;
    float total;
} pedido[MAXPEDIDOS] = {0};

struct pagamento {
    int nmPedido;
    char formaPagamento[20];
    float total;
}pagamento[MAXPEDIDOS] = {0};

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

    void importCardapio(void);
    void verCardapio(void);
    printf("\n_______________________________________\n");
    printf("\n     Bem vindo ao restaurante x");
    printf("\n_______________________________________\n");
    printf("\n\nAperte qualquer tecla para ver o cardapio. \n");
    getch();

    importCardapio();
    verCardapio();
}

void importCardapio(void) {
    int i = 0;
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

void verCardapio(void) {
    int i;
    system("cls");
    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");
    for (i = 0; i < MAX; i++)
    {
        printf("\n%3i %-20s R$ %.2f", cardapio[i].codProd - 1, cardapio[i].descrProd, cardapio[i].custoProd);
    }
    printf("\n__________________________________\n");
    getch();
}


void escolherOpcao(void) {
    int opc, i;
    void verCardapio(void);
    void opcoesPedido(void);

    do {
        system("cls");
        printf("Opções\n 1 - Ver cardápio\n 2 - Realizar pedido\n 3 - Sair");
        fflush(stdin);
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
            getch();
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
            escolherOpcao(); /*Voltar a tela anterior */
            break;
        default:
            break;
        }
    } while (opc != 1 || opc != 2 || opc != 3);
    
}

void incluirItem(void) {
    void carrinho(void);
    char confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd);
    int i, j, cod, qntd;
    float total;
    char continuar;

    do {
        system("cls");
        verCardapio();

        carrinho();

        printf("Qual o codigo do item desejado? ");
        fflush(stdin);
        scanf("%d", &cod);
        printf("Qual a quantidade do item desejado? ");
        fflush(stdin);
        scanf("%d", &qntd);

        pedido[cod].codProd = cod;
        pedido[cod].custoProd = cardapio[cod].custoProd;
        pedido[cod].qntdProd += qntd;
        strcpy(pedido[cod].descrProd, cardapio[cod].descrProd);
        pedido[cod].subtotal = cardapio[cod].custoProd * pedido[cod].qntdProd; 
        for ( i = 0; i < MAXPEDIDOS; i++)
        {
            pedido[MAXPEDIDOS].total += pedido[i].subtotal;
            if (pedido[i].qntdProd > 0) {
                pedido[MAXPEDIDOS].total = 0;
                for (j = 0; j < MAXPEDIDOS; j++) {
                    pedido[MAXPEDIDOS].total += pedido[j].subtotal;
                }
            }
        }
        
        confirmarEscolha(cod, cardapio[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd);
        continuar = getch();        
    } while (continuar == 's' || continuar == 'S');

}

void removerItem(void) {
    system("cls");

    int cod, opc, i;
    void carrinho(void);
    carrinho();

    
    if (pedido[MAXPEDIDOS].total > 0) {
        printf("(1)- Remover um item");
        printf("(2)- Remover tudo");
        scanf("%d", &opc);
        switch (opc) {
        case 1:
            printf("Qual o código do item que deseja remover?");
            fflush(stdin);
            scanf("%d", &cod);

            pedido[MAXPEDIDOS].total -= pedido[cod].subtotal;
            pedido[cod].qntdProd = 0;
            pedido[cod].subtotal = 0;
            break;
        case 2:
            for (cod = 0; cod < MAXPEDIDOS; cod++) {
                pedido[cod].qntdProd = 0;
                pedido[cod].subtotal = 0;
            }
            pedido[MAXPEDIDOS].total = 0;
            break;
        default:
            break;
        }
    }
    else {
        printf("Não há itens a remover.\nPressione qualquer tecla para voltar.");
        getch();
    }  
}

void carrinho(void) {
    int i = 0;
    for (i = 0; i < MAXPEDIDOS; i++) {
        if (pedido[i].qntdProd > 0) {
            if (i == 0)
            {
                printf("--------------------------CARRINHO----------------------------------\n");
            }
            printf("| Produto      Valor da unidade   Quantidade    Subtotal\n");
            printf("| (%d)- %s        %.2f            %d          %.2f\n",       pedido[i].codProd, 
                                                                                 pedido[i].descrProd,
                                                                                 pedido[i].custoProd,
                                                                                 pedido[i].qntdProd,
                                                                                 pedido[i].subtotal);
            printf("--------------------------------------------------------------------\n");
        }
        
    }
    if (pedido[MAXPEDIDOS].total > 1)
    {
        printf("| TOTAL   =           %.2f\n", pedido[MAXPEDIDOS].total);
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
    void pagamentoDinheiro(void);
    void pagamentoCartao(void);
    void pagamentoCheque(void);
    int opc;
    system("cls");
    printf("Qual o método de pagamento? \n");
    printf("(1)- Dinheiro  \n(2)-xerecard  \n(3)-Cheque");
    scanf("%d", &opc);
    fflush(stdin);
    switch (opc)
    {
    case 1:
        pagamentoDinheiro();
        break;
    case 2:
        pagamentoCartao();
        break;
    case 3:
        pagamentoCheque();
        break;
    default:
        printf("Opção inválida");
        break;
    }
}

void pagamentoDinheiro(void) {
    float cedula, troco, aux;

    printf("Digite o valor total das cedulas");
    scanf("%f", &cedula);

    troco = pedido[MAXPEDIDOS].total - cedula;

    if (pedido[MAXPEDIDOS].total < cedula)
    {
        printf("o troco é de %.2f ", troco * -1);
    }
    else {
        printf("Ainda faltam %.2f ", troco);
        printf("Adicione mais dinheiro");
        scanf("%.2f", &aux);
        cedula += aux;
    }
    if (pedido[MAXPEDIDOS].total - cedula == 0)
    {
        printf("Vá para a fila de espera");

        pagamento[MAXPEDIDOS].nmPedido = pedido[MAXPEDIDOS].nmPedido;
        strcpy(pagamento[MAXPEDIDOS].formaPagamento, "Dinheiro");
        pagamento[MAXPEDIDOS].total = pedido[MAXPEDIDOS].total;

        FILE *Arq;

        Arq = fopen("PAGAMENTOS.DAT", "a");

        fwrite(&pagamento[MAXPEDIDOS], sizeof(pagamento[MAXPEDIDOS]), 1, Arq);
        fclose(Arq);
    }
    getch();
}

void pagamentoCartao(void) {
    char numCartao[16+1];
    printf("Digite os números no verso do cartão");
    gets(numCartao);
    if (numCartao[17] != '\o')
    {
        printf("Cartão inválido");
    }
    
}

void pagamentoCheque(void) {
    pagamento[MAXPEDIDOS].nmPedido = pedido[MAXPEDIDOS].nmPedido;
    strcpy(pagamento[MAXPEDIDOS].formaPagamento, "Cheque");
    pagamento[MAXPEDIDOS].total = pedido[MAXPEDIDOS].total;

    FILE *Arq;

    Arq = fopen("PAGAMENTOS.DAT", "a");

    fwrite(&pagamento[MAXPEDIDOS], sizeof(pagamento[MAXPEDIDOS]), 1, Arq);
    fclose(Arq);
}

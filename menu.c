#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 99
#define MAXPEDIDOS 99
#define MAXQNTD 10

int qntd = 0;

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
    system("cls");
    void importCardapio(void);
    void verCardapio(void);
    printf("\n_______________________________________\n");
    printf("\n     Bem vindo ao restaurante x");
    printf("\n_______________________________________\n");
    printf("\n\nAperte qualquer tecla para ver o cardapio. \n");
    getch();

    importCardapio();
    verCardapio();
    getch();
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

void verCardapio(void) {
    int i;
    system("cls");
    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");
    for (i = 0; i < MAX; i++) {
        if (cardapio[i].codProd != 0) {
            printf("\n%3i %-20s R$ %.2f", cardapio[i].codProd, cardapio[i].descrProd, cardapio[i].custoProd);
        }
        
    }
    printf("\n__________________________________\n");
}


void escolherOpcao(void) {
    int opc, i;
    void verCardapio(void);
    void opcoesPedido(void);

    do {
        system("cls");
        printf("Opções\n(1)- Ver cardápio\n(2)- Realizar pedido\n(3)- Sair\n");
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
    void cobranca(void);
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
            cobranca();
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
    void confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd, int qntdAUX);
    void verificaValorTotal(void);
    void limparLancamento(int cod, int qntdAUX);
    void opcoesPedido(void);

    int i, j, cod, qntd;
    float total;
    char continuar;

    do {
        system("cls");
        verCardapio();

        carrinho();

        printf("Qual o codigo do item desejado? 0 para voltar.\n");
        fflush(stdin);
        scanf("%d", &cod);
        if (cod <= 0) {
            system("cls");
            printf("INCLUSÃO CANCELADA\n");
            printf("Retornando para as opções.\n");
            printf("...Pressione qualquer tecla...");
            getch();
            opcoesPedido();
        }
        else if (cardapio[cod].custoProd == 0) {
            printf("O código não é atribuido a um item");
            getch();
        }
        else {
            printf("Qual a quantidade do item desejado? 0 para voltar.\n");
            fflush(stdin);
            scanf("%d", &qntd);
            if (qntd <= 0) {
                system("cls");
                printf("INCLUSÃO CANCELADA\n");
                printf("Retornando para as opções.\n");
                printf("...Pressione qualquer tecla...");
                opcoesPedido();
            }
            else{
                pedido[cod].qntdProd += qntd;
                if (pedido[cod].qntdProd > MAXQNTD) {
                    printf("Quantidade de itens por código não pode ultrapassar 10");
                    getch();
                    pedido[cod].qntdProd -= qntd;
                    opcoesPedido();
                }
                else {
                    pedido[cod].codProd = cod;
                    pedido[cod].custoProd = cardapio[cod].custoProd;
                    strcpy(pedido[cod].descrProd, cardapio[cod].descrProd);
                    pedido[cod].subtotal = cardapio[cod].custoProd * pedido[cod].qntdProd;
                    verificaValorTotal();

                    confirmarEscolha(cod, cardapio[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd, qntd);
                }
            }
        }    
    } while (continuar == 's' || continuar == 'S' || cod != 0 || qntd != 0);
}

void verificaValorTotal(void) {
    int i;
    pedido[MAXPEDIDOS].total = 0;
    for (i = 0; i < MAXPEDIDOS; i++) {
        pedido[MAXPEDIDOS].total += pedido[i].subtotal;
    }
}

void removerItem(void) {
    system("cls");

    int cod, opc, i;
    void carrinho(void);
    carrinho();

    if (pedido[MAXPEDIDOS].total > 0) {
        printf("(1)- Remover um item\n");
        printf("(2)- Remover tudo\n");
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
    void verificaValorTotal(void);
    verificaValorTotal();

    for (i = 0; i < MAXPEDIDOS; i++) {
        if (pedido[i].qntdProd > 0) {
            if (i == 0) {
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
    if (pedido[MAXPEDIDOS].total > 1) {
        printf("| TOTAL   =           %.2f\n", pedido[MAXPEDIDOS].total);
        printf("--------------------------------------------------------------------\n");
    }
}

void confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd, int qntdAUX) {
    char opc;
    void limparLancamento(int cod, int qntd);
    void opcoesPedido(void);
    
    system("cls");
    printf("--------------------------------\n");
    printf("Produto escolhido: (%d)- %s \n", cod, nome);
    printf("Valor da unidade: %.2f\n", valorUnitario);
    printf("Quantidade: %d\n", qntd);
    printf("Subtotal: %.2f\n", (float)valorUnitario*qntd);
    printf("--------------------------------\n");
    printf ("Confirma a escolha? aperte esq para não\n");
    opc = getch();

    if (opc == 27) {
        limparLancamento(cod, qntdAUX);
        opcoesPedido();
    }
}

void limparLancamento(int cod, int qntdAUX) {
    int i;
    pedido[cod].qntdProd -= qntdAUX;
    pedido[cod].subtotal -= qntdAUX*cardapio[cod].custoProd;
}

void cobranca(void) {
    void formasPagamento(void);

    if (pedido[MAXPEDIDOS].total > 0) {
        system("cls");
        printf("Entrando em formas de pagamento\n");
        printf("...Pressione qualquer tecla para continuar...");
        getch();
        formasPagamento();
    }
    else {
        printf("Não há nenhum pedido lançado.");
        printf("...Pressione qualquer tecla para continuar...");
        getch();
    }
}

void formasPagamento(void) {
    void pagamentoDinheiro(void);
    void pagamentoCartao(void);
    void pagamentoCheque(void);
    int opc;

    system("cls");
  
    printf("Qual o método de pagamento? \n");
    printf("(1)- Dinheiro  \n(2)-xerecard  \n(3)-Cheque\n");
    fflush(stdin);
    scanf("%d", &opc);
    switch (opc) {
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
    system("cls");

    printf("\nO valor total do pedido foi de %.2f", pedido[MAXPEDIDOS].total);
    printf("\nDigite o valor total das cedulas\n");
    scanf("%f", &cedula);

    troco = pedido[MAXPEDIDOS].total - cedula;

    if (pedido[MAXPEDIDOS].total < cedula) {
        system("cls");
        printf("\no troco é de %.2f \n", troco * -1);

        pagamento[MAXPEDIDOS].nmPedido = pedido[MAXPEDIDOS].nmPedido;
        strcpy(pagamento[MAXPEDIDOS].formaPagamento, "Dinheiro");
        pagamento[MAXPEDIDOS].total = pedido[MAXPEDIDOS].total;

        FILE *Arq;

        Arq = fopen("PAGAMENTOS.DAT", "a+b");

        fwrite(&pagamento[MAXPEDIDOS], sizeof(pagamento[MAXPEDIDOS]), 1, Arq);
        fclose(Arq);
        printf("\nVá para a fila de espera.");
        exit(0);
    }
    else {
        printf("Valor insuficiente. ");
        getch();
        pagamentoDinheiro();
    }
    getch();
}

void pagamentoCartao(void) {
    /*char numCartao[16+1];
    printf("Digite os números no verso do cartão");
    gets(numCartao);
    if (numCartao[17] != '\0'){
        printf("Cartão inválido");
    }
    */
}

void pagamentoCheque(void) {
    system("cls");

    printf("Pagamento em cheque conclúido.\n");
    printf("Vá para a fila de espera");
    pagamento[MAXPEDIDOS].nmPedido = pedido[MAXPEDIDOS].nmPedido;
    strcpy(pagamento[MAXPEDIDOS].formaPagamento, "Cheque");
    pagamento[MAXPEDIDOS].total = pedido[MAXPEDIDOS].total;

    FILE *Arq;

    Arq = fopen("PAGAMENTOS.DAT", "a+b");

    fwrite(&pagamento[MAXPEDIDOS], sizeof(pagamento[MAXPEDIDOS]), 1, Arq);
    fclose(Arq);
}

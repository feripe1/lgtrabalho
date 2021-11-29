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

int main() {
    int opc;
    system("cls");
    void importCardapio(void);
    void verCardapio(void);
    void opcoesPedido(void);

    importCardapio();
    opcoesPedido();
    getch();

    getch();
    return 0;
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

void opcoesPedido(void){
    char opc;
    void incluirItem(void);
    void removerItem(void);
    void cobranca(void);
    void menuLancamento(void);
    void carrinho(void);
    do {
        system("cls");
        carrinho();
        printf("\nOpções do pedido: ");
        printf("\n(1)- Incluir um item\n(2)- Remover um item\n(3)- Finalizar pedido\n(ESQ)- Sair ");
        opc = getch();
        switch (opc) {
        case '1':
            incluirItem();
            break;
        case '2':
            removerItem();
            break;
        case '3':
            system("cls");
            printf("Entrando em FINALIZAR PEDIDO\n");
            printf("...Aperte qualquer tecla para continuar...");
            getch();
            cobranca();
            break;
        case 27:
            exit(0);
            break;
        default:
            system("cls");
            printf("Opção inválida\n");
            printf("...Aperte qualquer tecla para continuar...");
            getch();
            menuLancamento();
            break;
        }
    } while (opc != 27);
}

void menuLancamento(void) {
    void verCardapio(void);
    void opcoesPedido(void);

    verCardapio();
    opcoesPedido();
}

void incluirItem(void) {
    void carrinho(void);
    void confirmarEscolha(int cod, char* nome, float valorUnitario, int qntd, int qntdAUX);
    void verificaValorTotal(void);
    void limparLancamento(int cod, int qntdAUX);
    void opcoesPedido(void);

    int i, j, cod, qntd;
    float total;
    char opc, continuar;

    do {
        system("cls");

        verCardapio();
        carrinho();

        printf("\nQual o codigo do item desejado? 0 para voltar.\n");
        fflush(stdin);
        scanf("%d", &cod);
        if (cod <= 0) {
            system("cls");
            printf("INCLUSÃO CANCELADA\n");
            printf("Retornando para as opções.\n");
            printf("...Pressione qualquer tecla...");
            getch();
        }
        else if (cardapio[cod].custoProd == 0) {
            printf("O código não é atribuido a um item");
            getch();
        }
        else {
            printf("\nQual a quantidade do item desejado? 0 para voltar.\n");
            fflush(stdin);
            scanf("%d", &qntd);
            if (qntd <= 0) {
                system("cls");
                printf("INCLUSÃO CANCELADA\n");
                printf("Retornando para as opções.\n");
                printf("...Pressione qualquer tecla...");
                getch();
            }
            else{
                pedido[cod].qntdProd += qntd;
                if (pedido[cod].qntdProd > MAXQNTD) {
                    printf("\nQuantidade de itens por código não pode ultrapassar 10\n");
                    pedido[cod].qntdProd -= qntd;
                    getch();
                }
                else {
                    pedido[cod].codProd = cod;
                    pedido[cod].custoProd = cardapio[cod].custoProd;
                    strcpy(pedido[cod].descrProd, cardapio[cod].descrProd);
                    pedido[cod].subtotal = cardapio[cod].custoProd * pedido[cod].qntdProd;
                    verificaValorTotal();

                    confirmarEscolha(cod, cardapio[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd, qntd);
                    printf("Confirma a escolha? aperte esq para não\n");
                    opc = getch();
                    do {
                        switch (opc) {
                        case 13:
                            system("cls");
                            printf("Lancamento confirmado com sucesso.\n");
                            printf("...Aperte qualquer tecla para continuar...\n");
                            getch();
                            break;
                        case 27:
                            system("cls");
                            printf("Lancamento cancelado com sucesso.\n");
                            printf("...Aperte qualquer tecla para continuar...\n");
                            getch();
                            limparLancamento(cod, qntd);
                            break;
                        default:
                            break;
                        }
                    } while (opc != 27 && opc != 13);
                }
            }
        }
    } while (cod != 0 && qntd != 0);
}

void verificaValorTotal(void) {
    int i;
    pedido[MAXPEDIDOS].total = 0;
    for (i = 0; i < MAXPEDIDOS; i++) {
        pedido[MAXPEDIDOS].total += pedido[i].subtotal;
    }
}

void removerItem(void) {
    int cod, i;
    char opc;
    void carrinho(void);

    if (pedido[MAXPEDIDOS].total > 0) {
        do {
            system("cls");
            carrinho();
            printf("\nQual o código do item que deseja remover?\n");
            printf("(-1) Remover tudo\n");
            printf("(0)  Voltar\n");
            scanf("%d", &cod);
            if (cod == 0) {
                system("cls");
                printf("Retornando para o menu de opções\n");
                printf("...Aperte qualquer tecla para voltar");
                getch();
            }

            else if (cardapio[cod].custoProd == 0) {
                system("cls");
                printf("Código inválido");
                getch();
            }

            else if (pedido[cod].qntdProd == 0)
            {
                system("cls");
                printf("Esse item tem 0 de quantidade");
                getch();
            }

            else if (cod < 0) {
                do {
                    printf("\nDeseja mesmo remover tudo?");
                    opc = getch();
                    switch (opc) {
                    case 13:
                        system("cls");
                        printf("Itens removidos com sucesso\n");
                        printf("...Aperte qualquer tecla para voltar");

                        for (cod = 0; cod < MAXPEDIDOS; cod++) {
                            pedido[cod].qntdProd = 0;
                            pedido[cod].subtotal = 0;
                        }
                        pedido[MAXPEDIDOS].total = 0;
                        getch();
                        break;

                    case 27:
                        system("cls");
                        printf("Remoção cancelada.");
                        printf("...Aperte qualquer tecla para voltar");
                        getch();
                    default:
                        system("cls");
                        printf("Opção inválida");
                        getch();
                        break;
                    }
                } while (opc != 13 && opc != 27);
            }
            else if (cod > 0) {
                do {
                    printf("\nDeseja mesmo remover o item?\n");
                    opc = getch();

                    switch (opc)
                    {
                    case 13:
                        system("cls");
                        printf("O item foi removido com sucesso.\n");
                        printf("...Aperte qualquer tecla para voltar");
                        pedido[MAXPEDIDOS].total -= pedido[cod].subtotal;
                        pedido[cod].qntdProd = 0;
                        pedido[cod].subtotal = 0;
                        getch();
                        break;

                    case 27:
                        system("cls");
                        printf("Remoção de item cancelada.\n");
                        printf("...Aperte qualquer tecla para voltar");
                        getch();
                        break;

                    default:
                        printf("Opção inválida");
                        getch();
                        break;
                    }

                } while (opc != 13 && opc != 27);
            }
        }
        while (cod != 0 && opc != 13 && opc != 27);
    }
    else {
        printf("Não há itens a remover.\nPressione qualquer tecla para voltar.");
        getch();
    }  
}

void carrinho(void) {
    int i = 0;
    int primeiro = 0;
    void verificaValorTotal(void);
    verificaValorTotal();

    for (i = 0; i < MAXPEDIDOS; i++) {
        if (pedido[i].qntdProd > 0) {
            if (primeiro == 0) {
                printf("--------------------------CARRINHO----------------------------\n");
            }
            primeiro = 1;
            printf("| Produto      Valor da unidade   Quantidade    Subtotal     |\n");
            printf("| (%d)- %s        %.2f            %d          %.2f   |\n",       pedido[i].codProd, 
                                                                                 pedido[i].descrProd,
                                                                                 pedido[i].custoProd,
                                                                                 pedido[i].qntdProd,
                                                                                 pedido[i].subtotal);
            printf("--------------------------------------------------------------\n");
        }
    }
    if (pedido[MAXPEDIDOS].total > 1) {
        printf("| TOTAL   =           %.2f                                 |\n", pedido[MAXPEDIDOS].total);
        printf("--------------------------------------------------------------\n");
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
    printf("--------------------------------\n\n");
}

void limparLancamento(int cod, int qntdAUX) {
    int i;
    pedido[cod].qntdProd -= qntdAUX;
    pedido[cod].subtotal -= qntdAUX*cardapio[cod].custoProd;
}

void cobranca(void) {
    void formasPagamento(void);

    if (pedido[MAXPEDIDOS].total > 0) {
        printf("Entrando em formas de pagamento");
        printf("...Aperte alguma tecla para continuar...");
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
    carrinho();
    printf("\nQual o método de pagamento? \n");
    printf("(1)- Dinheiro  \n(2)- xerecard  \n(3)- Cheque\n");
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

    carrinho();
    printf("\nDigite o valor total das cedulas\n");
    scanf("%f", &cedula);

    troco = cedula - pedido[MAXPEDIDOS].total;

    if (pedido[MAXPEDIDOS].total < cedula) {
        system("cls");
        printf("\no troco é de %.2f \n", troco);

        pagamento[MAXPEDIDOS].nmPedido = pedido[MAXPEDIDOS].nmPedido;
        strcpy(pagamento[MAXPEDIDOS].formaPagamento, "Dinheiro");
        pagamento[MAXPEDIDOS].total = pedido[MAXPEDIDOS].total;

        FILE *Arq;

        Arq = fopen("PAGAMENTOS.DAT", "a");

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

    Arq = fopen("PAGAMENTOS.DAT", "a");

    fwrite(&pagamento[MAXPEDIDOS], sizeof(pagamento[MAXPEDIDOS]), 1, Arq);
    fclose(Arq);
    getch();
    exit(1);
}

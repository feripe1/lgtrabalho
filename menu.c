#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 20
#define MAXQNTD 10
#define MAXNPEDIDOS 199


int qntd, i = 0;
int nmPedido;

struct produto
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int demoraMinutos;
} cardapio[MAX];

struct pedido
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int qntdProd;
    float subtotal;
    int demoraMinutos;
} pedido[MAX] = {};

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    float total;
    bool entregue;
    int demoraTotal;
} pagamento = {};

struct fila
{
    struct pedido pedido[MAX];
    struct pagamento pagamento;
    int posicaoFila;
} fila[MAXNPEDIDOS];

int main() {
    int opc;
    system("cls");
    void importCardapio(void);
    void verCardapio(void);
    void opcoesPedido(void);
    void importNPedido(void);

    importCardapio();
    importNPedido();
    verCardapio();
    opcoesPedido();

    getch();
    return 0;
}

void importNPedido(void) {
    FILE *Arq;
    void exportNPedido(void);

    Arq = fopen("NMPEDIDO.DAT", "r+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir NMPEDIDO.DAT");
        getch();
        exit(0);
    }
    else {
        fread(&nmPedido, sizeof(nmPedido), 1, Arq);
        nmPedido++;
    }
    fclose(Arq);
    exportNPedido();
}

void exportNPedido(void) {
    FILE *Arq;
    Arq = fopen("NMPEDIDO.DAT", "w+b");
    if (Arq == NULL) {
        printf("\nErro ao abrir NMPEDIDO.DAT");
        getch();
        exit(0);
    }
    else {
        fwrite(&nmPedido, sizeof(nmPedido), 1, Arq);
    }
    fclose(Arq);
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

void exportPagamento(void) {
    FILE *Arq;

    Arq = fopen("PAGAMENTOS.DAT", "ab");

    fwrite(&pagamento, sizeof(pagamento), 1, Arq);
    fclose(Arq);
}

void verCardapio(void) {
    int i;
    system("cls");

    printf("nm do pedido: %d\n", nmPedido);
    printf("\n___________________________MENU________________________\n");
    printf("\n  COD     DESCRICAO      VALOR (R$)   DEMORA EM MINUTOS");
    printf("\n_______________________________________________________\n");
    for (i = 0; i < MAX; i++) {
        if (cardapio[i].codProd != 0) {
            printf("\n  %-7i %-15s R$ %.2f %10d", cardapio[i].codProd, cardapio[i].descrProd, cardapio[i].custoProd, cardapio[i].demoraMinutos);
        }
        
    }
    printf("\n______________________________________________________\n");
}

void opcoesPedido(void){
    char opc;
    void incluirItem(void);
    void removerItem(void);
    void cobranca(void);
    void menuLancamento(void);
    void carrinho(void);
    void verCardapio(void);
    do {
        verCardapio();
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
                    pedido[cod].demoraMinutos = cardapio[cod].demoraMinutos;
                    verificaValorTotal();

                    confirmarEscolha(cod, pedido[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd, qntd);
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

void limparLancamento(int cod, int qntdAUX)
{
    int i;
    pedido[cod].qntdProd -= qntdAUX;
    pedido[cod].subtotal -= qntdAUX * cardapio[cod].custoProd;
}

void verificaValorTotal(void) {
    int i;
    pagamento.total = 0;
    for (i = 0; i < MAX; i++) {
        pagamento.total += pedido[i].subtotal;
    }
}

void removerItem(void) {
    int cod, i;
    char opc;
    void carrinho(void);

    if (pagamento.total > 0) {
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
            
            else if (cod < 0) {
                do {
                    printf("\nDeseja mesmo remover tudo?");
                    opc = getch();

                    switch (opc) {
                    case 13:
                        system("cls");
                        printf("Itens removidos com sucesso\n");
                        printf("...Aperte qualquer tecla para voltar");

                        for (cod = 0; cod < MAX; cod++) {
                            pedido[cod].qntdProd = 0;
                            pedido[cod].subtotal = 0;
                        }
                        break;
                    case 27:
                        system("cls");
                        printf("Remoção cancelada.\n");
                        printf("...Aperte qualquer tecla para voltar\n");
                        getch();
                    default:
                        system("cls");
                        printf("Opção inválida");
                        getch();
                        break;
                    }
                } while (opc != 13 && opc != 27);
            }
            else if (cardapio[cod].custoProd == 0) {
                system("cls");
                printf("Código inválido");
                getch();
            }

            else if (cod > 0) {
                if (pedido[cod].qntdProd == 0) {
                    system("cls");
                    printf("Esse item tem 0 de quantidade");
                    getch();
                }
                else {
                    do {
                        printf("\nDeseja mesmo remover o item?\n");
                        opc = getch();

                        switch (opc) {
                        case 13:
                            system("cls");
                            printf("O item foi removido com sucesso.\n");
                            printf("...Aperte qualquer tecla para voltar\n");
                            pagamento.total -= pedido[cod].subtotal;
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
        }
        while (cod != 0 && opc != 13 && opc != 27);
    }
    else {
        system("cls");
        printf("Não há itens a remover.\nPressione qualquer tecla para voltar.\n");
        getch();
    }  
}

void carrinho(void) {
    int i = 0;
    int primeiro = 0;
    void verificaValorTotal(void);
    verificaValorTotal();

    for (i = 0; i < MAX; i++) {
        if (pedido[i].qntdProd > 0) {
            if (primeiro == 0) {
                printf("--------------------------CARRINHO-------------------------------\n");
            }
            primeiro = 1;
            printf("| Produto         Valor da unidade   Quantidade    Subtotal     |\n");
            printf("| (%d)- %s        %.2f            %d          %.2f      |\n",       pedido[i].codProd, 
                                                                                        pedido[i].descrProd,
                                                                                        pedido[i].custoProd,
                                                                                        pedido[i].qntdProd,
                                                                                        pedido[i].subtotal);
            printf("-----------------------------------------------------------------\n");
        }
    }
    if (pagamento.total > 1) {
        printf("| TOTAL   =           %.2f                                    |\n", pagamento.total);
        printf("-----------------------------------------------------------------\n");
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

void cobranca(void) {
    void formasPagamento(void);

    if (pagamento.total > 0) {
        system("cls");
        printf("Entrando em formas de pagamento\n");
        printf("...Aperte alguma tecla para continuar...");
        getch();
        formasPagamento();
    }
    else {
        system("cls");
        printf("Não há nenhum pedido lançado.\n");
        printf("...Pressione qualquer tecla para continuar...");
        getch();
    }
}

void formasPagamento(void) {
    void pagamentoDinheiro(void);
    void pagamentoCartao(void);
    void pagamentoCheque(void);
    void mostrarComprovante(void);
    int opc, i;

    system("cls");
    carrinho();

    printf("Digite seu nome, para chamarmos após a preparação do pedido\n");
    fflush(stdin);
    gets(pagamento.nomePessoa);
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
    void armazenarFila(void);
    void exportPagamento(void);
    float cedula, troco, aux;
    int i;
    system("cls");

    carrinho();
    printf("\nDigite o valor total das cedulas\n");
    scanf("%f", &cedula);

    troco = cedula - pagamento.total;

    if (pagamento.total < cedula) {
        system("cls");
        printf("\no troco é de %.2f \n", troco);

        pagamento.nmPedido = nmPedido;
        strcpy(pagamento.formaPagamento, "Dinheiro");

        exportPagamento();
        armazenarFila();

        getch();
    }
    else {
        printf("Valor insuficiente. ");
        getch();
        pagamentoDinheiro();
    }
    exit(0);
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
    void mostrarComprovante(void);
    void exportPagamento(void);
    int i;

    system("cls");

    printf("Pagamento em cheque conclúido.\n");
    printf("Vá para a fila de espera");

    strcpy(pagamento.formaPagamento, "Cheque");
    pagamento.nmPedido = nmPedido;

    exportPagamento();
    mostrarComprovante();

    exit(0);
}

void exportPedidos(void) {
    FILE *Arq;
    int i;
    Arq = fopen ("PEDIDOS.DAT", "w");

    fwrite(&pedido, sizeof(pedido), 1, Arq);
    fclose(Arq);
}

void armazenarFila(void) {
    void exportFila(void);
    void mostrarComprovante(void);

    pagamento.entregue == true;

    for (i = 0; i < 20; i++) {
        pagamento.demoraTotal += pedido[i].demoraMinutos * pedido[i].qntdProd;
        printf("%d", pagamento.demoraTotal);
        getch();
    }

    if (pagamento.demoraTotal > 4) {
        pagamento.entregue = false;
        for (i = 0; i < 20; i++)
        {
            fila[nmPedido].pedido[i] = pedido[i];
        }
        fila[nmPedido].pagamento = pagamento;
        exportFila();
    }
    mostrarComprovante();
}

void exportFila(void) {
    FILE *Arq;
    Arq = fopen("FILA.DAT", "ab");

    fwrite(&fila[nmPedido], sizeof(fila[nmPedido]), 1, Arq);
    fclose(Arq);
}

void mostrarComprovante(void) {
    void exportPedidos(void);
    void importPedidos(void);
    void armazenarFila(void);

    printf("\n-------------------------COMPROVANTE--------------------\n");
    printf("| Nm do pedido                Cliente                   |\n");
    printf("| %-15d %20s             |\n", nmPedido, pagamento.nomePessoa);
    printf("| Pedido pago com %-20s |\n", pagamento.formaPagamento);
    printf("--------------------------------------------------------\n");
    printf("| Cod    Nome do Produto    Quantidade    Subtotal     |\n");
     for (i = 0; i < 20; i++) {
        if (pedido[i].qntdProd > 0) {
            printf("| %d %15s %12d %13.2f        |\n", pedido[i].codProd, pedido[i].descrProd, pedido[i].qntdProd, pedido[i].subtotal);
        }
    }
    printf("|\n|                                   Total: %.2f       |\n", pagamento.total);
    printf("-------------------------------------------------------\n");
    getch();
}

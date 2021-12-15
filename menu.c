#include <stdio.h>
#include <conio.h> /* Bibliotecas utilizadas */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define MAX 20
#define MAXQNTD 10 /* Defines usados para determinar o tamanho dos vetores */

int qntd, i = 0;
int nmPedido; /* Variaveis globais */

struct produto
{
    int codProd;
    char descrProd[20]; /* Estrutura do cardapio onde o codigo do produto é o valor do vetor */
    float custoProd;
    int demoraMinutos;
} cardapio[MAX];

struct pedido
{
    int codProd;
    char descrProd[20];
    float custoProd; /* Estrutura do pedido do usuario onde cada valor do vetor é o codigo do item no cardapio */
    int qntdProd;
    float subtotal;
    int demoraMinutos;
} pedido[MAX] = {};

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    char nmCartao[17];
    float total; /* Estrutura de pagamento onde tem informações adicionais */
    bool entregue;
    int demoraTotal;
} pagamento = {};

struct fila
{
    struct pedido pedido[MAX];
    struct pagamento pagamento; /* Estrutura fila que engloba pagamento e pedido, serve para organiza-los  */
    int posicaoFila;            /*  para cada número do pedido, por exemplo, */
} fila;                         /* a fila com número do pedido 10 tem seus dados de pagamento e pedido, e a fila com nm 12 tem outros dados */

int main()
{
    int opc;
    system("cls");
    void importCardapio(void);
    void opcoesPedido(void);
    void importNPedido(void);

    importCardapio(); /* main com os imports de dados importantes como nm do pedido e cardapio */
    importNPedido();  /* mostra o cardapio e as opcoes do usuario */
    opcoesPedido();

    return 0;
}

void importNPedido(void)
{
    FILE *Arq;
    void exportNPedido(void);

    Arq = fopen("NMPEDIDO.DAT", "r+b");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir NMPEDIDO.DAT");
        getch();
        exit(0); /* import no ultimo nm do pedido e adiciona +1 */
    }
    else
    {
        fread(&nmPedido, sizeof(nmPedido), 1, Arq);
        nmPedido++;
    }
    fclose(Arq);
    exportNPedido();
}

void exportNPedido(void)
{
    FILE *Arq;
    Arq = fopen("NMPEDIDO.DAT", "w+b");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir NMPEDIDO.DAT");
        getch();
        exit(0); /* exporta o ultimo nm do pedido */
    }
    else
    {
        fwrite(&nmPedido, sizeof(nmPedido), 1, Arq);
    }
    fclose(Arq);
}

void importCardapio(void)
{
    system("cls");

    FILE *Arq;
    Arq = fopen("PRODUTOS.DAT", "r+b");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch(); /* simples import no cardapio */
        exit(0);
    }
    while (!feof(Arq))
    {
        fread(&cardapio, sizeof(cardapio), 1, Arq);
    }
    fclose(Arq);
}

void exportPagamento(void)
{
    FILE *Arq;

    Arq = fopen("PAGAMENTOS.DAT", "ab");

    fwrite(&pagamento, sizeof(pagamento), 1, Arq);
    fclose(Arq);
}

void verCardapio(void)
{
    int i;
    system("cls");

    printf("nm do pedido: %d\n", nmPedido);
    printf("\n___________________________MENU________________________\n");
    printf("\n  COD     DESCRICAO      VALOR (R$)   DEMORA EM MINUTOS");
    printf("\n_______________________________________________________\n");
    for (i = 0; i < MAX; i++)
    {
        if (cardapio[i].codProd != 0)
        { /* mostra o cardapio */
            printf("\n  %-7i %-15s R$ %.2f %10d", cardapio[i].codProd, cardapio[i].descrProd, cardapio[i].custoProd, cardapio[i].demoraMinutos);
        }
    }
    printf("\n______________________________________________________\n");
}

void opcoesPedido(void)
{
    char opc;
    void incluirItem(void);
    void removerItem(void);
    void cobranca(void);
    void carrinho(void);
    void verCardapio(void);
    do
    {
        verCardapio();
        carrinho();

        printf("\nOpções do pedido: ");
        printf("\n(1)- Incluir um item\n(2)- Remover um item\n(3)- Finalizar pedido\n(ESQ)- Sair ");
        opc = getch();
        switch (opc)
        {
        case '1':
            incluirItem();
            break;
        case '2':
            removerItem();
            break; /* opcoes do usuario */
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

void incluirItem(void)
{
    void carrinho(void);
    void confirmarEscolha(int cod, char *nome, float valorUnitario, int qntd, int qntdAUX);
    void verificaValorTotal(void);
    void limparLancamento(int cod, int qntdAUX);
    void opcoesPedido(void);

    int i, j, cod, qntd;
    float total;
    char opc, continuar;

    do
    {
        system("cls");

        verCardapio();
        carrinho();

        printf("\nQual o codigo do item desejado? 0 para voltar.\n");
        fflush(stdin);
        scanf("%d", &cod);
        if (cod <= 0)
        { /* se o codigo for 0 ou menor que 0 voltará para a tela inicial */
            system("cls");
            printf("INCLUSÃO CANCELADA\n");
            printf("Retornando para as opções.\n");
            printf("...Pressione qualquer tecla...");
            getch();
        }
        else if (cardapio[cod].custoProd == 0)
        {
            printf("O código não é atribuido a um item");
            getch(); /* se o item do codigo selecionado nao tiver valor, ele não foi listado */
        }
        else
        {
            printf("\nQual a quantidade do item desejado? 0 para voltar.\n");
            fflush(stdin);
            scanf("%d", &qntd);
            if (qntd <= 0)
            { /* quantidade 0 = voltar */
                system("cls");
                printf("INCLUSÃO CANCELADA\n");
                printf("Retornando para as opções.\n");
                printf("...Pressione qualquer tecla...");
                getch();
            }
            else
            {
                pedido[cod].qntdProd += qntd; /* adiciona a quantidade selecionada na struct pedido */
                if (pedido[cod].qntdProd > MAXQNTD)
                { /* a quantidade máxima não pode passar de 10*/
                    printf("\nQuantidade de itens por código não pode ultrapassar 10\n");
                    pedido[cod].qntdProd -= qntd; /* e é retirado a quantidade selecionada */
                    getch();
                }
                else
                {
                    pedido[cod].codProd = cod; /* itens da struct cardapio são copiadas para a struct pedido */
                    pedido[cod].custoProd = cardapio[cod].custoProd;
                    strcpy(pedido[cod].descrProd, cardapio[cod].descrProd);
                    pedido[cod].subtotal = cardapio[cod].custoProd * pedido[cod].qntdProd;
                    pedido[cod].demoraMinutos = cardapio[cod].demoraMinutos;
                    verificaValorTotal(); /* verifica o valor total, fiz uma funcao para usar em diversas partes a fim de
                  /* evitar bugs com valores totais aleatorios */

                    confirmarEscolha(cod, pedido[cod].descrProd, pedido[cod].custoProd, pedido[cod].qntdProd, qntd);
                    /* os itens selecionados são jogados na funcao confirmar escolha para mostrar e confirmar o lancamento */
                    printf("Confirma a escolha? (ENTER = Sim || ESQ = Não)\n");
                    fflush(stdin);
                    opc = getch();
                    do
                    {
                        switch (opc)
                        {
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
                            limparLancamento(cod, qntd); /* caso o lancamento seja cancelado é necessario limpar as variaveis modificadas */
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

    pedido[cod].qntdProd -= qntdAUX; /* funcao para limpar as variaveis em caso de rejeicao na confirmacao do lancamento */
    pedido[cod].subtotal -= qntdAUX * cardapio[cod].custoProd;
}

void verificaValorTotal(void)
{
    int i;
    pagamento.total = 0;

    for (i = 0; i < MAX; i++) /* verifica o valor total e joga na struct pagamento */
        pagamento.total += pedido[i].subtotal;
}

void removerItem(void)
{
    void carrinho(void);
    int cod, i;
    char opc;
    bool voltar;

    if (pagamento.total > 0)
    {
        do
        {
            system("cls");
            carrinho();

            printf("\nQual o código do item que deseja remover?\n");
            printf("(-1) Remover tudo\n");
            printf("(0)  Voltar\n");
            scanf("%d", &cod); /* selecao do codigo do pedido que deseja remover */
            if (cod == 0)
            {
                system("cls"); /* cod == 0 para sair */
                printf("Retornando para o menu de opções\n");
                printf("...Aperte qualquer tecla para voltar");
                voltar = true;
                getch();
            }

            else if (cod < 0)
            {
                do
                {
                    printf("\nDeseja mesmo remover tudo?");
                    opc = getch(); /* ccaso queira remover todos os lancamentos entao use um valor menor que 0 */

                    switch (opc)
                    {
                    case 13:
                        system("cls");
                        printf("Itens removidos com sucesso\n");
                        printf("...Aperte qualquer tecla para voltar");

                        for (cod = 0; cod < MAX; cod++)
                        {
                            pedido[cod].qntdProd = 0; /* limpa o pedido retirado */
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
            else if (cardapio[cod].custoProd == 0)
            {
                system("cls"); /* verifica se o codigo selecionado tem valor estabelecido */
                printf("Código inválido");
                getch();
            }

            else if (cod > 0)
            {
                if (pedido[cod].qntdProd == 0)
                {
                    system("cls"); /* se o codigo selecionado pertencer a um item, mas ele não foi selecionado previamente */
                    printf("Esse item tem 0 de quantidade");
                    getch();
                }
                else
                {
                    do
                    {
                        printf("\nDeseja mesmo remover o item? (ENTER = Sim || ESQ = Não)\n");
                        opc = getch();

                        switch (opc)
                        {
                        case 13:
                            system("cls");
                            printf("O item foi removido com sucesso.\n");
                            printf("...Aperte qualquer tecla para voltar\n");
                            pagamento.total -= pedido[cod].subtotal; /* zera o total e tambem o subtotal de cada pedido */
                            pedido[cod].qntdProd = 0;                /* e tambem a quantidade */
                            pedido[cod].subtotal = 0;
                            getch();
                            break;

                        case 27:
                            system("cls");
                            printf("Remoção de item cancelada.\n");
                            printf("...Aperte qualquer tecla para voltar");
                            voltar = true;
                            getch();
                            break;

                        default:
                            printf("Opção inválida");
                            getch();
                            break;
                        }

                    } while (voltar == false);
                }
            }
        } while (voltar == false);
    }
    else
    {
        system("cls");
        printf("Não há itens a remover.\nPressione qualquer tecla para voltar.\n");
        getch();
    }
}

void carrinho(void)
{
    void verificaValorTotal(void);
    int i = 0;
    int primeiro = 0;
    
    verificaValorTotal();

    for (i = 0; i < MAX; i++)
    { /* carrinho que é mostrado praticamente em todo o decorrer do app */
        if (pedido[i].qntdProd > 0)
        {
            if (primeiro == 0)
            { /* verifica se há algum item selecionado, para não mostrar o topo do carrinho
sem necessidade */
                printf("--------------------------CARRINHO-------------------------------\n");
            }
            primeiro = 1;
            printf("| Produto         Valor da unidade   Quantidade    Subtotal     |\n");
            printf("| (%d)- %s        %.2f            %d          %.2f      |\n", pedido[i].codProd,
                   pedido[i].descrProd,
                   pedido[i].custoProd,
                   pedido[i].qntdProd,
                   pedido[i].subtotal);
            printf("-----------------------------------------------------------------\n");
        }
    }
    if (pagamento.total > 1)
    { /* tambem é verificado se há itens lancados para não mostrar o total sem motivo */
        printf("| TOTAL   =           %.2f                                    |\n", pagamento.total);
        printf("-----------------------------------------------------------------\n");
    }
}

void confirmarEscolha(int cod, char *nome, float valorUnitario, int qntd, int qntdAUX)
{
    char opc;

    system("cls"); /* funcao usada no incluirItem para confirmacao da selecao do item */
    printf("--------------------------------\n");
    printf("Produto escolhido: (%d)- %s \n", cod, nome);
    printf("Valor da unidade: %.2f\n", valorUnitario);
    printf("Quantidade: %d\n", qntd);
    printf("Subtotal: %.2f\n", (float)valorUnitario * qntd);
    printf("--------------------------------\n\n");
}

void cobranca(void)
{
    void formasPagamento(void);

    if (pagamento.total > 0)
    { /* verifica se há pedidos listados para permitir finalizar o pedido */
        system("cls");
        printf("Entrando em formas de pagamento\n");
        printf("...Aperte alguma tecla para continuar...");
        getch();
        formasPagamento();
    }
    else
    {
        system("cls");
        printf("Não há nenhum pedido lançado.\n");
        printf("...Pressione qualquer tecla para continuar...");
        getch();
    }
}

void formasPagamento(void)
{
    void pagamentoDinheiro(void);
    void pagamentoCartao(void);
    void pagamentoCheque(void);
    void mostrarComprovante(void);
    bool finalizarPedido(void);
    char opc;
    bool voltar = false;
    bool pago = false;

    do
    {
        system("cls");
        carrinho();
        printf("Qual o método de pagamento? \n");
        printf("(1)- Dinheiro  \n(2)- xerecard  \n(3)- Cheque\n(ESQ)- Voltar\n"); /* selecao do metodo de pagamento */
        fflush(stdin);
        opc = getch();

        switch (opc)
        {
        case '1':
            pagamentoDinheiro();
            break;
        case '2':
            pagamentoCartao();
            break;
        case '3':
            pagamentoCheque();
            break;
        case 27:
            system("cls");
            printf("Voltando ao menu principal\n");
            printf("...Aperte qualquer tecla para continuar...\n");
            getch();
            voltar = true;
            break;
        default:
            printf("Opção inválida");
            getch();
            break;
        }
        if (finalizarPedido)
            pago = true;
        
    } while (voltar == false && pago == false);
}

void pagamentoDinheiro(void)
{
    bool finalizarPedido(void);
    float cedula, troco, aux;
    int i;
    bool voltar = false;
    char opc;
    
    do
    {
        system("cls");
        carrinho();
        printf("\nDigite o valor total das cedulas: ( 0 - Voltar )\n\n");
        fflush(stdin);
        scanf("%f", &cedula); /* entrada do valor das cedulas */

        troco = cedula - pagamento.total; /* calculo do troco */
        if (cedula == 0)                  /* caso o valor seja 0 == voltar */
        {
            printf("Voltando para formas de pagamento\n");
            printf("...Aperte qualquer tecla para continuar...");
            getch();
            voltar = true;
        }   

        else if (cedula >= pagamento.total)
        {  
            system("cls");
            printf("Valor total: %.2f\n\n", pagamento.total);
            printf("Valor entregue: %.2f\n\n", cedula); 
            if (troco > 0)
                printf("O troco é de %.2f\n\n", troco); /* caso entre um valor valido então mostra o troco e exporta a struct fila */
            printf("Confirmar pedido? (ENTER = Sim || ESQ = Não)");
            opc = getch(); /* e tambem mostra o comprovante */
            switch (opc)
            {
            case 13:
                pagamento.nmPedido = nmPedido;
                strcpy(pagamento.formaPagamento, "Dinheiro"); /* copia os itens selecionados para a struct pagamento */

                finalizarPedido();
                voltar = true;
                break;
            case 27:
                system("cls");
                printf("Voltando para formas de pagamento\n");
                printf("...Aperte qualquer tecla para continuar");
                voltar = true;
                getch();
                break;

            default:
                system("cls");
                printf("Opção inválida");
                getch();
                break;
            }
        }
        else
        {
            printf("\nValor insuficiente. \n");
            getch();
        }
    } while (voltar == false);
}

void pagamentoCartao(void)
{
    char nmCartaoDigitado[17];
    char opc;
    bool validarCartao(char nmCartao[]);
    bool voltar = false;
    void mascaraCartao(char nmCartao[]);
    bool finalizarPedido(void);

    do
    {
        system("cls");
        printf("Digite os números no verso do cartão\n");
        gets(pagamento.nmCartao);

        if (pagamento.nmCartao == 0)
        {
            system("cls");
            printf("Retornando aos métodos de pagamento\n");
            printf("...Aperte qualquer tecla para continuar\n");
            voltar = true;
            getch();
        }

        else if (validarCartao(pagamento.nmCartao))
        {
            system("cls");
            mascaraCartao(pagamento.nmCartao);
            printf("Valor total: %.2f\n\n", pagamento.total);
            printf("Cartão: %s \n\n", pagamento.nmCartao);
            printf("Confirmar pedido? (ENTER = Sim || ESQ = Não)\n\n");
            opc = getch();

            switch (opc)
            {
            case 13:
                pagamento.nmPedido = nmPedido;
                strcpy(pagamento.formaPagamento, "Cartão"); /* copia os itens selecionados para a struct pagamento */
                finalizarPedido();
                voltar = true;
                break;
            case 27:
                system("cls");
                printf("Voltando para formas de pagamento\n");
                printf("...Aperte qualquer tecla para continuar...\n");
                voltar = true;
                getch();
                break;
            default:
                system("cls");
                printf("Opção inválida");
                getch();
                break;
            }
            getch();
        }
        else
        {
            system("cls");
            printf("Número do cartão inválido.\n\n");
            printf("Cheque se há 16 números\n\n");
            printf("...Aperte qualquer tecla para tentar novamente...\n");
            getch();
        }
    } while (voltar == false);
}

bool validarCartao(char nmCartao[]) {
    int nmValido = 0;

    for (i = 0; i <= 15; i++)
        if (nmCartao[i] <= 57 && nmCartao[i] >= 48)
            nmValido++;

    if (nmValido == 16 && nmCartao[16] == '\0')
        return true;
    else
        return false;
}

void mascaraCartao(char nmCartao[]) {
    for (i = 4; i < 12; i++)
    {
        nmCartao[i] = '*';
    }
}

void pagamentoCheque(void)
{
    bool finalizarPedido(void);
    int i, valorCheque;
    char opc;

    bool voltar = false;
    system("cls");

    do
    {
        carrinho();
        printf("\nDigite o valor total do cheque: ( 0 - Voltar )\n\n");
        scanf("%f", &valorCheque); /* exatamente a mesma logica do pagamento em dinheiro */

        if (valorCheque == 0)
        {
            system("cls");
            printf("Voltando para formas de pagamento\n");
            printf("...Aperte qualquer tecla para continuar...");
            voltar = true;
            getch();
        }

        else if (valorCheque >= pagamento.total)
        {
            system("cls");
            printf("Valor total: %.2f\n\n", pagamento.total);
            printf("Confirmar pedido? (ENTER = Sim || ESQ = Não)");
            opc = getch(); /* e tambem mostra o comprovante */
            switch (opc)
            {
            case 13:
                pagamento.nmPedido = nmPedido;
                strcpy(pagamento.formaPagamento, "Cheque"); /* copia os itens selecionados para a struct pagamento */
                finalizarPedido();
                voltar = true;
                break;
            case 27:
                system("cls");
                printf("Voltando para formas de pagamento\n");
                printf("...Aperte qualquer tecla para continuar");
                voltar = true;
                getch();
                break;
            default:
                system("cls");
                printf("Opção inválida");
                getch();
                break;
            }
        }
        else
        {
            printf("\nValor insuficiente. ");
            getch();
        }
    } while (voltar == false);
}

bool finalizarPedido(void)
{
    void exportPagamento(void);
    void armazenarFila(void);
    void limparVariaveis(void);
    void importNPedido(void);
    void mostrarComprovante(void);
    bool validarNome(char nome[]);

    do
    {
        system("cls");
        printf("Digite seu nome, para chamarmos após o preparo do pedido\n");
        fflush(stdin); /* entrada do nome do usuario na struct pagamento */
        gets(pagamento.nomePessoa);
    } while (!validarNome(pagamento.nomePessoa));
    
    exportPagamento();
    armazenarFila();
    mostrarComprovante();

    system("cls");
    printf("Pedido realizado com sucesso!\n\n");
    if (pagamento.demoraTotal > 4)
    {
        printf("O tempo de preparo é estimado em %d minutos\n", pagamento.demoraTotal);
        printf("Por favor ir para a fila de espera\n");
        getch();
    }
    else
    {
        printf("Não é necessario ir à fila de espera\n");
        printf("Em instantes seu pedido será entregue\n");
        getch();
    }
    
    for (i = 5; i > 0; i--)
    {
        system("cls");
        printf("Redirecionando ao menu principal em %d", i);
        Sleep(1000);
    }
    importNPedido();
    limparVariaveis();
    return true;
} 

bool validarNome(char nome[]) {
    int caractereValido = 0;

    for (i = 0; nome[i] != '\0'; i++)
        if (nome[i] >= 65 && nome[i] <= 90 || nome[i] >= 97 && nome[i] <= 122)
            caractereValido++;

    if (caractereValido != 0)
        return true;
    else {
        printf("\nNome inválido\n");
        printf("Usar letras minúsculas ou maíusculas\n");
        printf("Não usar números nem caracteres especiais.\n");
        getch();
        return false;
    }
}

void armazenarFila(void)
{
    void exportFila(void);
    void mostrarComprovante(void);

    pagamento.entregue == true;

    for (i = 0; i < 20; i++) /* aqui é calculado a demora total para preparar os pedidos */
        pagamento.demoraTotal += pedido[i].demoraMinutos * pedido[i].qntdProd;

    if (pagamento.demoraTotal > 4)
    {
        pagamento.entregue = false; /* caso a demora passe de 4 minutos, o pedido é jogado na fila de espera */
        for (i = 0; i < 20; i++)
            fila.pedido[i] = pedido[i]; /* copia das variaveis na struct fila com base no nm do pedido */

        fila.pagamento = pagamento;
        exportFila();
    }
}

void exportFila(void)
{
    FILE *Arq;
    Arq = fopen("FILA.DAT", "ab"); /* export na struct fila */

    fwrite(&fila, sizeof(fila), 1, Arq);
    fclose(Arq);
}

void mostrarComprovante(void)
{
    FILE *REL;
    REL = fopen("COMPROVANTE.DAT", "w"); /* o comprovante é gravado no arquivo comprovante.dat e é aberto para vizualicao
    após o pagamento ser efetuado */
    fprintf(REL, "----------------------COMPROVANTE-----------------------\n");
    fprintf(REL, "| Nm do pedido                Cliente                  |\n");
    fprintf(REL, "| %-15d %20s                 |\n", nmPedido, pagamento.nomePessoa);
    fprintf(REL, "| Pedido pago com %-20s                 |\n", pagamento.formaPagamento);
    fprintf(REL, "--------------------------------------------------------\n");
    fprintf(REL, "| Cod    Nome do Produto    Quantidade    Subtotal     |\n");

    for (i = 0; i < 20; i++)
        if (pedido[i].qntdProd > 0)
        {
            fprintf(REL, "| %d %15s %12d %13.2f         |\n", pedido[i].codProd, pedido[i].descrProd, pedido[i].qntdProd, pedido[i].subtotal);
        }

    fprintf(REL, "|\n|                                   Total: %.2f       |\n", pagamento.total);
    fprintf(REL, "--------------------------------------------------------\n");
    fclose(REL);
    system("notepad COMPROVANTE.DAT");
}

void limparVariaveis(void)
{
    for (i = 0; i < 20; i++)
    {
        pedido[i].codProd = '\0';
        pedido[i].custoProd = '\0';
        pedido[i].demoraMinutos = '\0';
        pedido[i].qntdProd = '\0';
        pedido[i].subtotal = '\0';
    }
    pagamento.demoraTotal = '\0';
    pagamento.entregue = '\0';
    pagamento.nmPedido = '\0';
    pagamento.total = '\0';

    for (i = 0; i <= 26; i++)
        pagamento.nomePessoa[i] = '\0';

    for (i = 0; i <= 10; i++)
        pagamento.formaPagamento[i] = '\0';

    for (i = 0; i <= 20; i++)
        for (int j = 0; j <= 20; j++)
            pedido[i].descrProd[j] = '\0';

    for (i = 0; i < 17; i++)
        pagamento.nmCartao[i] = '\0';
}

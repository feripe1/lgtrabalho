#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 20
#define MAXNPEDIDOS 199

int nmPedido;
int i = 0;

struct produto
{
    int codProd;
    char descrProd[20];
    float custoProd;
    bool demora;
} cardapio[MAX];

struct pedido
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int qntdProd;
    float subtotal;
    bool demora;
} pedido[MAX] = { };

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    float total;
    bool entregue;
    int demoraTotal;
} pagamento = { };

struct fila
{
    struct pedido pedido[MAX];
    struct pagamento pagamento;
    int posicaoFila;
} fila[MAXNPEDIDOS], aux;

int main()
{
    void importNPedido(void);
    void importPagamentos(void);
    void importCardapio(void);
    void importFila(void);
    void verPagamentos(void);

    importNPedido();
    importPagamentos();
    importCardapio();
    importFila();
    verPagamentos();

    getch();
    return 0;
}

void importNPedido(void)
{
    FILE *Arq;

    Arq = fopen("NMPEDIDO.DAT", "r+b");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir NMPEDIDO.DAT");
        getch();
        exit(0);
    }
    else
    {
        fread(&nmPedido, sizeof(nmPedido), 1, Arq);
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
        getch();
        exit(0);
    }
    while (!feof(Arq))
    {
        fread(&cardapio, sizeof(cardapio), 1, Arq);
    }
    fclose(Arq);
}

void importPedidos(void) {
    system("cls");

    FILE *Arq;
    Arq = fopen("PEDIDOS.DAT", "r");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
    }
    while (!feof(Arq))
    {
        fread(&pedido, sizeof(pedido), 1, Arq);
    }

    fclose(Arq);
}


void importPagamentos(void)
{
    system("cls");

    FILE *Arq;
    Arq = fopen("PAGAMENTOS.DAT", "r");
    if (Arq == NULL)
    {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
    }
    while (!feof(Arq))
    {
        fread(&pagamento, sizeof(pagamento), 1, Arq);
    }

    fclose(Arq);
}

void importFila(void) {
    int cont = 1;

    FILE *Arq;
    Arq = fopen("FILA.DAT", "rb");
    fread(&fila, sizeof(fila), 1, Arq);

    for (i = 0; i <= nmPedido; i++) {
        for ( int j = 0; j <= nmPedido; j++) {
            if (i != fila[i].pagamento.nmPedido) {
                aux = fila[fila[i].pagamento.nmPedido];
                fila[fila[i].pagamento.nmPedido] = fila[i];
                fila[i] = aux;
            }
        }
    }

    for (i = 0; i <= nmPedido; i++) {
        if (fila[i].pagamento.nmPedido != 0) {
            fila[i].posicaoFila = cont;
            cont++;
        }
    }
    fclose(Arq);
}

void verPagamentos(void) {
    void verdetalhes(int nmpedido);
    system("cls");
    int i, j;
    int opc;
    printf("--------------------PEDIDOS NA FILA-------------------\n");
    printf("     NM DO PEDIDO      NOME        POSICAO NA FILA\n");
    for (i = 0; i <= nmPedido; i++) {        
        if (fila[i].pagamento.entregue == false && fila[i].pagamento.nmPedido != 0)
        {
            printf("%10d %18s %16d\n", fila[i].pagamento.nmPedido, fila[i].pagamento.nomePessoa, fila[i].posicaoFila);
        }
            
    }
    printf("------------------------------------------------------\n");
    printf("\nDigite o número do pedido para ver mais detalhes: ");
    scanf("%d", &opc);
    verdetalhes(opc);
    
}

void verdetalhes(int nmpedido){
    if (fila[nmpedido].pagamento.total == 0) {
        printf("Número inválido.");
    }

    else {
        printf("\n\n----------------Lançamentos do pedido----------------\n");
        printf("     COD  NOME DO PRODUTO    QUANTIDADE    SUBTOTAL\n");
        for (i = 0; i < 20; i++)
        {
            if (fila[nmpedido].pedido[i].qntdProd > 0)
            {
                printf("%7d %14s %12d %13.2f\n", fila[nmpedido].pedido[i].codProd, fila[nmpedido].pedido[i].descrProd,
                                                fila[nmpedido].pedido[i].qntdProd, fila[nmpedido].pedido[i].subtotal);
            }
        }
        printf("------------------------------------------------------\n");
        printf("                                     Total: %.2f\n", fila[nmpedido].pagamento.total);
        printf("               Tempo de preparo total: %d minutos\n", fila[nmpedido].pagamento.demoraTotal);
        printf("------------------------------------------------------");
    }
}

void prepararPedido(void) {

}

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#define MAX 20
#define MAXNPEDIDOS 199

int nmPedido;
int i = 0;
int cont = 0;
bool sair;

struct pedido
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int qntdProd;
    float subtotal;
    int demoraMinutos;
    int tipoProduto;
} pedido[MAX] = { };

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    char nmCartao[17];
    float total;
    bool entregue;
    int demoraTotal;
} pagamento = { };

struct FILA
{
    struct pedido pedido[MAX];
    struct pagamento pagamento;
    int posicaoFila;
} fila[MAXNPEDIDOS], aux;


int main()
{
    void importNPedido(void);
    void verFila(void);
    void importFila(void);
    void atualizarPosicaoFila(void);

    importNPedido();
    importFila();
    atualizarPosicaoFila();
    verFila();

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

void importFila(void) {
	int j;
    FILE *Arq;
    Arq = fopen("FILA.DAT", "rb");
    fread(&fila, sizeof(fila), 1, Arq);

    for (i = 0; i < nmPedido; i++) {
        for (j = 0; j < nmPedido; j++)
        {
            aux = fila[fila[i].pagamento.nmPedido];
            fila[fila[i].pagamento.nmPedido] = fila[i];
            fila[i] = aux;
        }
    }
    fclose(Arq);
}

void atualizarPosicaoFila(void) {
    cont = 0;
    for (i = 0; i <= nmPedido; i++)
    {
        if (fila[i].pagamento.nmPedido != 0 && fila[i].pagamento.entregue == false)
        {
            cont++;
            fila[i].posicaoFila = cont;
        }
    }
}

void verFila(void) {
    void verDetalhes(int nmpedido);
    void importFila(void);
    system("cls");
    int i, j;
    int opc;
    
    do {
    	system("mode 65, 33");
    	system("color f0");
        if (cont > 0)
        {
            system("cls");
            printf("\n--------------------------PEDIDOS NA FILA------------------------\n");
            printf("     NM DO PEDIDO        NOME         POSICAO NA FILA\n");
            for (i = 0; i <= nmPedido; i++)
            {
                if (fila[i].pagamento.entregue == false && fila[i].pagamento.nmPedido != 0)
                {
                    printf("%10d %18s %16d\n", fila[i].pagamento.nmPedido, fila[i].pagamento.nomePessoa, fila[i].posicaoFila);
                }
            }
            printf("-----------------------------------------------------------------\n");
            printf("\n Digite o número do pedido para ver mais detalhes: ");
            scanf("%d", &opc);

            if (opc == 0)
            {
                system("cls");
                printf("\n Fechando o sistema");
                getch();
                sair = true;
            }

            else if (fila[opc].pagamento.total == 0)
            {
                system("cls");
                system("color c7");
                printf("\n Número inválido.\n");
                printf(" ...Aperte qualquer tecla para continuar...");
                getch();
            }
            else
            {
                verDetalhes(opc);
            }
        }
        else {
            system("cls");
            system("color c7");
            printf("\n Não há pedidos na fila");
            sair = true;
        }
    } while (sair == false);
}

void verDetalhes(int x){
    char opc;
    void prepararPedido(int x);
    void verFila(void);

    printf("\n\n----------------------Lançamentos do pedido---------------------\n");
    printf("     COD  NOME DO PRODUTO    QUANTIDADE    SUBTOTAL\n");
    for (i = 0; i < 20; i++)
    {
        if (fila[x].pedido[i].qntdProd > 0)
        {
            printf("%7d %14s %12d %13.2f\n", fila[x].pedido[i].codProd, fila[x].pedido[i].descrProd,
                   fila[x].pedido[i].qntdProd, fila[x].pedido[i].subtotal);
        }
    }
    printf("                                    Total: %.2f\n", fila[x].pagamento.total);
    printf("            Tempo de preparo previsto: %d minutos\n", fila[x].pagamento.demoraTotal);
    printf("-----------------------------------------------------------------\n");
    printf("\n Deseja preparar o pedido? ( ENTER = Sim || ESQ = N�o )\n ");
    do {
    	
    opc = getch();
    switch (opc)
    {
	    case 27:
	        break;
	    case 13:
	        prepararPedido(x);
	        break;
	    default:
	        break;
	    }
	} while (opc != 27 && opc != 13);
    
}

void prepararPedido(int x) {
    int j;
    void exportFila(void);
    void atualizarPosicaoFila(void);
    if (fila[x].posicaoFila == 1) {
        fila[x].pagamento.entregue = true;
        printf("Preparando pedido (20x acelerado)\n\n");
        for (i = 0; i < 20; i++)
        {
            if (fila[x].pedido[i].qntdProd > 0)
            {
                
                for (j = 0; j < fila[x].pedido[i].qntdProd; j++)
                {
                    if (fila[x].pedido[i].tipoProduto == 1 || fila[x].pedido[i].tipoProduto == 4)
                    {
                        printf(" Separando ingredientes...\n");
                        Sleep(666);
                        printf(" Preparando %s.\n", fila[x].pedido[i].descrProd);
                        Sleep(666);
                        printf(" %s ficou pronto!!\n\n", fila[x].pedido[i].descrProd);
                        Sleep(666);
                    }
                    if (fila[x].pedido[i].tipoProduto == 2)
                    {
                        printf(" Pegando %s na geladeira.\n\n", fila[x].pedido[i].descrProd);
                        Sleep(1000);
                    }
                    if (fila[x].pedido[i].tipoProduto == 3)
                    {
                        printf(" Separando ingredientes...\n");
                        Sleep(500);
                        printf(" Batendo %s.\n\n", fila[x].pedido[i].descrProd);
                        Sleep(500);
                    }
                }
            } 
        }
        printf(" Aperte alguma tecla para entregar o pedido ");
        getch();
        for (i = 0; i < 3; i++)
        {
            printf(". ");
            Sleep(1000);
        }
        
        printf("\n\n Pedido número %d entregue com sucesso!\n", x);
        exportFila();
        atualizarPosicaoFila();
        getch();
    }
    else {
        printf("\n O pedido número %d não é o primeiro na lista de espera.", x);
        getch();
    }
}

void exportFila(void) {
    FILE *Arq;
    Arq = fopen("FILA.DAT", "wb");

    for (i = 0; i <= nmPedido; i++)
    {
        if (fila[i].pagamento.entregue == false && fila[i].pagamento.total != 0)
        {
            fwrite(&fila[i], sizeof(fila[i]), 1, Arq);
        }
    }
    fclose(Arq);
}

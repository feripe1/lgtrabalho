#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 20

int nmPedido;

struct produto
{
    int codProd;
    char descrProd[20];
    float custoProd;
    int demoraMinutos;
    int tipoProduto;
} cardapio[MAX];

struct pagamento
{
    int nmPedido;
    char nomePessoa[26];
    char formaPagamento[10];
    char nmCartao[17];
    float total;
    bool entregue;
    int demoraTotal;
} pagamento[199];

int main()
{
    void importPagamentos(void);
    void importCardapio(void);
    void opcoes(void);
    importPagamentos();
    importCardapio();
    opcoes();

    getch();
    return 0;
}

void opcoes(void) {
    system("cls");
    char opc;

    void adicionarItem(void);
    void verPagamentos(void);
    void retirarItem(void);
    void verCartoes(void);
    
    do {
    system("mode 45, 33");
    system("color f0");
    printf("\n Selecione uma opcao: \n");
    printf(" 1 Ver os últimos pagamentos\n 2 Ver cartões\n 3 Adicionar item ao cardapio\n 4 Retirar item\n ESQ = Sair\n");
    
    opc = getch();
    switch (opc) {
	    case '1':
	        verPagamentos();
	        break;
	    case '2':
	        verCartoes();
	        break;
	    case '3':
	        adicionarItem();
	        break;
	    case '4':
	        retirarItem();
	        break;
	    case 27:
	    	exit(0);
	    	break;
	    default:
	        printf("Opção inválida");
	        getch();
	        break;
	    }
	} while(opc != 27);
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

void verCardapio(void)
{
    int i;
    system("cls");

    printf("\n_______________MENU_______________\n");
    printf("\n  Cod     Descricao      Valor (R$)");
    printf("\n__________________________________\n");
    for (i = 0; i < MAX; i++)
    {
        if (cardapio[i].codProd != 0)
        {
            printf("\n  %-7i %-15s R$ %.2f", cardapio[i].codProd, cardapio[i].descrProd, cardapio[i].custoProd);
        }
    }
    printf("\n__________________________________\n");
}

void importPagamentos(void) {
    system("cls");

    FILE *Arq;
    Arq = fopen("PAGAMENTOS.DAT", "rb");
    if (Arq == NULL) {
        printf("\nErro ao abrir PRODUTOS.DAT");
        getch();
        
    }
    while (!feof(Arq))
    {
        fread(&pagamento, sizeof(pagamento), 1, Arq);
    }
    
    fclose(Arq);
}

void verPagamentos(void) {
    system("cls");
    int i;
    printf("\n---------------------------------------------\n");
    printf(" COD       FORMA DE PAGAMENTO       TOTAL\n");
    printf("---------------------------------------------\n");

    for (i = 0; i < 199; i++)
    {
        if (pagamento[i].total >0)
        {
            printf("% 3d %19s %17.2f \n", pagamento[i].nmPedido,  pagamento[i].formaPagamento, pagamento[i].total);
        }
        
    }
    getch();
}

void verCartoes(void)
{
    system("cls");
    int i;
    printf("\n---------------------------------------------\n");
    printf("       NMPEDIDO           CARTAO\n");
    printf("---------------------------------------------\n");

    for (i = 0; i < 199; i++)
    {
        if (pagamento[i].total > 0 && pagamento[i].nmCartao[i] != '\0')
        {
            printf("%10d %25s \n", pagamento[i].nmPedido, pagamento[i].nmCartao);
        }
    }
    getch();
}

void adicionarItem(void) {
	void limparItemRemovido(int cod);
    char opcContinuar, opcConfirmar;
    int cod, aux;
    void verCardapio(void);
    bool voltar = false;
    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w+b");

    if (fp == NULL) {
        printf("Erro ao tentar abrir PRODUTOS.DAT\n");
        
    }

    do {
        fflush(stdin);
        system("cls");
        verCardapio();
        printf(" cod ( 0 = Voltar ) ");
        scanf("%d", &cod);
        if (cod == 0){
        	voltar = true;
		}
		else {
			cardapio[cod].codProd = cod;
	        printf(" nome ");
	        fflush(stdin);
	        gets(cardapio[cod].descrProd);
	
	        printf(" preco ");
	        fflush(stdin);
	        scanf("%f", &cardapio[cod].custoProd);
	
	        printf(" Demora? ");
	        scanf("%d", &cardapio[cod].demoraMinutos);
	
	        printf(" Tipo de produto\n 1 = Lanche\n 2 = Liquido em lata\n 3 = Liquido com preparo\n 4 = Acompanhamento\n ");
	        scanf("%d", &cardapio[cod].tipoProduto);
	        
	        printf("\n Confirmar adi��o ao card�pio? ESQ = N�o)");
	        printf("\n ");
	        opcConfirmar = getch();
	        
	        if(opcConfirmar == 27) {
	        	limparItemRemovido(cod);
	        	voltar = true;
			}
			else {
				printf("\n\n Deseja continuar? ESQ = N�o");
	        	opcContinuar = getch();
			}
		}
    } while (opcContinuar != 27 && voltar == false);
    fwrite(&cardapio, sizeof(cardapio), 1, fp);
    fclose(fp);
}

void limparItemRemovido(int cod){
	int i;
	cardapio[cod].codProd = '\0';
	for(i = 0; i <20; i++)
    	cardapio[cod].descrProd[i] = '\0';
    	
    cardapio[cod].custoProd = '\0';
    cardapio[cod].demoraMinutos = '\0';
    cardapio[cod].tipoProduto = '\0';
}

void retirarItem(void) {
	void limparItemRemovido(int cod);
    void verCardapio(void);
    int cod, i;
    char opc;
    FILE *fp;

    fp = fopen("PRODUTOS.DAT", "w+b");

    if (fp == NULL)
    {
        printf("Erro ao tentar abrir PRODUTOS.DAT\n");
        
    }
    do {
    system("cls");
    verCardapio();
    printf(" Qual o código do item que deseja retirar?\n\n");
    scanf("%d", &cod);

    limparItemRemovido(cod);
    
    printf(" Deseja continuar? ESQ = N�o\n ");
    opc = getch();
	} while(opc != 27);

    fwrite(&cardapio, sizeof(cardapio), 1, fp);
    fclose(fp);

    getch();

}

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define MAX 100
#define MAXPALAVRAS 100

typedef struct Node { // estrutura da lista
    int linha;
    int contaPalavras;
    struct Node* next;
}Node;

Node *InicializaNode(int linha) // cria nó
{
    struct Node* novoNode = (struct Node*)malloc(sizeof(struct Node));
    novoNode->linha = linha;
    novoNode->contaPalavras = 1;
    novoNode->next = NULL;
    return novoNode;
}

// aqui vai ser criado uma estrutura para ajudar na hora de mexer com dicionario. 
typedef struct 
{
    char palavra[MAX];
    Node *nOcorrencia;
}DicTipo;

//Agora a logica para analisar se a palavra está no dicionario ou não.
int procura_Palavra(const char *palavra, DicTipo *dicionario, int tamanho)
{
    for(int indice = 0; indice< tamanho; indice++)
    {
        if(strcmp(dicionario[indice].palavra, palavra) == 0)//serve pra comparar sequencia
        {
            return indice;
        }
    }
    return -1;
}

//Agora vai ser criado uma função para adicionar ou atualizar o acaso das palavras na lista
void verificaAcaso(int linha, Node **head)
{
    Node *acaso = *head;
    Node *ultimoAcaso = NULL;

    while(acaso)
    {
        if(acaso ->linha == linha)
        {
            acaso->contaPalavras++;
            return;
        }
        ultimoAcaso = acaso;
        acaso = acaso->next;
    }
    Node *novoNode = InicializaNode(linha);
    if(ultimoAcaso == NULL)
    {
        *head = novoNode;
    }else
    {
        ultimoAcaso->next = novoNode;
    }
}
//Importante tirar caracteres que podem atrapalhar a analise 
void limpeza(char *palavra)
{
    char aux[MAX];
    int k = 0;
    
    for(int i = 0;palavra[i] != '\0'; i++)
    {
        if(isalnum(palavra[i]))
        {
            aux[k++] = tolower(palavra[i]);
        }
    }
    aux[k] = '\0';
    strcpy(palavra,aux);
}
int main() {

    FILE *fp;
    fp = fopen("TESTE.txt", "r");

    char palavra[MAX];
    DicTipo dicionario[MAXPALAVRAS];
    int numeroLinhas = 0;
    
    int tamanhoDicionario =0;
    while(fgets(palavra, sizeof(palavra),fp))
    {
        char *auxPalavra = strtok(palavra, " \t\n"); // separa a string com base em "marcador".
        while (auxPalavra)
        {
            limpeza(auxPalavra);
            int tamanhoPalavra = strlen(auxPalavra);
            if(tamanhoPalavra > 0)
            {
                int indice = procura_Palavra(auxPalavra,dicionario,tamanhoDicionario);
                if(indice == -1) // Caso a palavra não existir no dicionario, copia usando strcpy e att a lista de ocorrencias.
                {
                    strcpy(dicionario[tamanhoDicionario].palavra,auxPalavra); // pega auxPalavra e copia ela no campo palavra do dicionario
                    verificaAcaso(numeroLinhas,&dicionario[tamanhoDicionario].nOcorrencia); 
                    tamanhoDicionario++;
                }
                else //att lista de ocorrencia se a palavra ja estiver no dicionario
                {
                    verificaAcaso(numeroLinhas,&dicionario[indice].nOcorrencia);

                }
            }
            auxPalavra = strtok(NULL, " \t\n"); // busca proxima palavra separando a string.
        }
        numeroLinhas++;
    }
    //for para imprimir o dicionario.
    printf("Indice Invertido: \n");
    for(int i = 0; i<tamanhoDicionario;i++)
    {
        printf("%s: ", dicionario[i].palavra);
        Node *acaso = dicionario[i].nOcorrencia;

        while (acaso)
        {
            printf("(%d, %d) ", acaso->linha,acaso->contaPalavras);
            acaso = acaso->next;
        }
        printf("\n");
    }

    for(int i = 0; i< tamanhoDicionario; i++)
    {
        Node *acaso = dicionario[i].nOcorrencia;
        while(acaso)
        {
            Node *next = acaso->next;
            free(acaso);
            acaso = next;
        }
    }
    fclose(fp);
    return 0;
}
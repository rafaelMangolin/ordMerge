#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINT 9999999999

int qntdArqUso = 0;

struct registro{
    int id;
    char desc[55];
}registro;

int main(){
    int tamanhoEntrada,tamanhoSaida;
    char *nome;

    printf("Digite o nome do arquivo:");
    gets(nome);

    printf("Digite o valor do buffer de entrada:");
    scanf("%i",&tamanhoEntrada);

    printf("Digite o valor do buffer de saida:");
    scanf("%i",&tamanhoSaida);

    merge(tamanhoEntrada,tamanhoSaida,nome);
}

void merge(int tamanhoBufferEntrada,int tamanhoBufferSaida,char *nome){
    //Declarações
    int qntdRegBufferSaida = 0;
    int regCountBreak = 0;
    int nomeArqIncr = 0;
    int regcont = 0;
    int arr_rrn[2000];

    struct registro buffer[tamanhoBufferEntrada];
    struct registro bufferSaida[tamanhoBufferSaida];

    FILE *file,*fileWriteFinal;

    if ((file = fopen(nome, "r")) == NULL) {
       printf("Tentativa de abrir o arquivo invalida\n");
       exit(1);
    }

    fseek(file, SEEK_SET, 0);

    fread(&regcont,1,sizeof(int),file);

    while(regcont > regCountBreak){
        fread(buffer,tamanhoBufferEntrada,sizeof(registro),file);
        regCountBreak += tamanhoBufferEntrada;
        quickSort(buffer,0,tamanhoBufferEntrada - 1);

        FILE *fileWrite;
        char nomeCorrida[50];

        sprintf(nomeCorrida, "corrida_%i.txt", nomeArqIncr++);

        if ((fileWrite = fopen(nomeCorrida, "w")) == NULL) {
           printf("Tentativa de abrir o arquivo invalida\n");
           exit(1);
        }
        printf("\n\nCorrida numero %i \n", nomeArqIncr-1);
        int i;
        for(i = 0 ;i < tamanhoBufferEntrada;i++){
            printf("Chave: %i, texto: %s \n",buffer[i].id,buffer[i].desc);
        }

        fwrite(buffer,tamanhoBufferEntrada,sizeof(registro),fileWrite);
        fclose(fileWrite);
    }
    qntdArqUso = nomeArqIncr;
    printf("Quantidade de corridas geradas %i, reads %i, writes %i",qntdArqUso,qntdArqUso,qntdArqUso);

    if(tamanhoBufferEntrada < qntdArqUso){
        printf("\n\n\nNão é possivel executar para essa especificação dos tamanhos dos buffers\n");
        exit(1);
    }

    if((fileWriteFinal = fopen("arquivo_saida.txt", "w")) == NULL) {
       printf("Tentativa de abrir o arquivo invalida\n");
       exit(1);
    }
    fclose(fileWriteFinal);

    int i;
    for(i = 0 ;i < nomeArqIncr ; i++){
        arr_rrn[i] = 0;
        fromBufToFile(i,arr_rrn,buffer);
    }

    while (qntdArqUso > 0 ){
        fromBufferToOutBuffer(buffer,bufferSaida,arr_rrn,tamanhoBufferEntrada,tamanhoBufferSaida,&qntdRegBufferSaida);

    }

    fclose(file);
}

int encontraMenor(struct registro *buffer,int length){
    int i = 0,menor = 0;
    for (i = 0 ;i< length-1; i++){
        if(buffer[menor].id > buffer[i+1].id){
            menor = i+1;
        }
    }
    return menor;
}

void fromBufferToOutBuffer(struct registro *buffer, struct registro *bufferSaida,int arr_rrn[],int tambuffer,int tambufferSaida,int *contSaida){
    int menor = encontraMenor(buffer,tambuffer);

    bufferSaida[*contSaida] = buffer[menor];
    *contSaida += 1;

    fromBufToFile(menor,arr_rrn,buffer);

    if(tambufferSaida == *contSaida){
        FILE *fileWriteOutput;
        if ((fileWriteOutput = fopen("arquivo_saida.txt", "r+")) == NULL) {
           printf("Tentativa de abrir o arquivo invalida\n");
           exit(1);
        }
        fseek(fileWriteOutput,0,SEEK_END);

        fwrite(bufferSaida,tambufferSaida,sizeof(registro),fileWriteOutput);
        *contSaida = 0;
        fclose(fileWriteOutput);
    }
}


void fromBufToFile(int index, int arr_rrn[],struct registro *buffer ){
        FILE *fileRead;
        char nomeCorrida[50];
        struct registro str_aux;
        int retornoQuantidade = -1;

        sprintf(nomeCorrida, "corrida_%i.txt", index);

        if ((fileRead = fopen(nomeCorrida, "r+")) == NULL) {
           printf("Tentativa de abrir o arquivo invalida\n");
           exit(1);
        }

        fseek(fileRead,arr_rrn[index]++ * sizeof(registro),SEEK_SET);
        //Passar o tamanho do buffer no arquivo
        retornoQuantidade = fread(&str_aux,1,sizeof(registro),fileRead);

        if(retornoQuantidade == 0 ){
            buffer[index].id = 9999999999;
            qntdArqUso--;
        }else{
            buffer[index] = str_aux;
        }
}


void trocaValores(struct registro* a, struct registro* b){
    struct registro aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int divide(struct registro vec[], int esquerdo, int direito){
    int i, j;

    i = esquerdo;
    for (j = esquerdo + 1; j <= direito; ++j){
        if (vec[j].id < vec[esquerdo].id){
            ++i;
            trocaValores(&vec[i], &vec[j]);
        }
    }
    trocaValores(&vec[esquerdo], &vec[i]);

    return i;
}

void quickSort(struct registro vec[], int esquerdo, int direito){
    int r;
    if (direito > esquerdo){
        r = divide(vec, esquerdo, direito);
        quickSort(vec, esquerdo, r - 1);
        quickSort(vec, r + 1, direito);
    }
}

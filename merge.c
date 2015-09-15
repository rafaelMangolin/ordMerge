#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct registro{
    int id;
    char descricao[55];
}registro;

int main(){
    int tamanhoEntrada,tamanhoSaida;
    char nome[300];

    printf("Digite o nome do arquivo:");
    scanf("%s[^\\n]",nome);

    printf("Digite o valor do buffer de entrada:");
    scanf("%i",&tamanhoEntrada);

    printf("Digite o valor do buffer de saida:");
    scanf("%i",&tamanhoSaida);

    merge(tamanhoEntrada,tamanhoSaida,nome);
}

void merge(int tamanhoBufferEntrada,int tamanhoBufferSaida,char *nome){
    int qntdRegBufferSaida = 0;
    int regCountBreak = 0;
    int nomeArqIncr = 0;
    int regcont = 0;
    int qntdArqUso = 0;
    FILE *arrPonteiro[2000];

    struct registro buffer[tamanhoBufferEntrada];
    struct registro bufferSaida[tamanhoBufferSaida];

    FILE *file,*ponteiroSaida;

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

        sprintf(nomeCorrida, "corrida%i.txt", nomeArqIncr++);

        if ((fileWrite = fopen(nomeCorrida, "w")) == NULL) {
           printf("Tentativa de abrir o arquivo invalida\n");
           exit(1);
        }
        printf("\n\nCorrida numero %i \n", nomeArqIncr-1);
        int i;
        fwrite(buffer,tamanhoBufferEntrada,sizeof(registro),fileWrite);
        fclose(fileWrite);
        for(i = 0 ;i < tamanhoBufferEntrada;i++){
            printf("Chave: %i, texto: %s \n",buffer[i].id,buffer[i].descricao);
        }
    }



    qntdArqUso = nomeArqIncr;
    printf("Quantidade de corridas geradas %i, reads %i, writes %i",qntdArqUso,qntdArqUso,qntdArqUso);

    if(tamanhoBufferEntrada < qntdArqUso){
        printf("\n\n\nNão é possivel executar para essa especificação dos tamanhos dos buffers\n");
        exit(1);
    }

    if((ponteiroSaida = fopen("arquivo_saida.txt", "w")) == NULL) {
       printf("Tentativa de abrir o arquivo invalida\n");
       exit(1);
    }

    int i;
    for(i = 0 ;i < nomeArqIncr ; i++){
        char nomeCorrida[50];
        sprintf(nomeCorrida, "corrida%i.txt", i);
        if ((arrPonteiro[i] = fopen(nomeCorrida, "r+")) == NULL) {
           printf("Tentativa de abrir o arquivo invalida\n");
           exit(1);
        }
        fromBufToFile(i,arrPonteiro,buffer,&qntdArqUso);
    }

    while (qntdArqUso > 0 ){
        fromBufferToOutBuffer(buffer,bufferSaida,arrPonteiro,tamanhoBufferEntrada,tamanhoBufferSaida,&qntdRegBufferSaida,ponteiroSaida,&qntdArqUso);
    }

    fclose(ponteiroSaida);
    fclose(file);

    FILE *fp;
    if ((fp = fopen("arquivo_saida.txt", "r")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado\n");
           exit(1);
        }
        int h =0,x = 0;
        struct registro cx;
        fseek(fp,0,0);
         h = fread(&cx,1,sizeof(registro),fp);
    while(h!= 0){
        printf("ARQ ID: %i contador : %i \n" , cx.id , x++) ;
        h = fread(&cx,1,sizeof(registro),fp);
    }

    fclose(fp);
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

void fromBufferToOutBuffer(struct registro *buffer, struct registro *bufferSaida,FILE* arrPonteiro[],int tambuffer,int tambufferSaida,int *contSaida,FILE *ponteiroSaida, int *qntdArqUso){
    int menor = encontraMenor(buffer,tambuffer);

    bufferSaida[*contSaida] = buffer[menor];
    *contSaida += 1;

    fromBufToFile(menor,arrPonteiro,buffer,qntdArqUso);

    if(tambufferSaida == *contSaida){
        fwrite(bufferSaida,tambufferSaida,sizeof(registro),ponteiroSaida);
        *contSaida = 0;
    }
}


void fromBufToFile(int indice, FILE* arrRegistros[],struct registro *buffer ,int *qntdArqUso){
        FILE *fileRead;
        char nomeCorrida[50];
        struct registro str_aux;
        int retornoQuantidade = -1;



        retornoQuantidade = fread(&str_aux,1,sizeof(registro),arrRegistros[indice]);

        if(retornoQuantidade == 0 ){
            fclose(arrRegistros[indice]);
            buffer[indice].id = 9999999999;
            *qntdArqUso-=1;
        }else{
            buffer[indice] = str_aux;
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

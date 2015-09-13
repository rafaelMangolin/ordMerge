#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITO = 999999

int regcont = 0, contArquivosEmUso = 0;

struct str_reg{
    int id;
    char desc[55];
}str_reg;

int main(){
    mostrar();
}

void mostrar(){
    //Declarações
    int tambuffer = 10,totalRegistros = 0, contsaida = 0,
        contRegRead = 0,contNomeArq = 0,contCorrida = 0,index = 0;

    int arr_rrn[2000],tambufferSaida = 5;

    char *status = "Preenchendo Buffer";
    struct str_reg buffer[tambuffer];
    struct str_reg bufferSaida[tambufferSaida];

    FILE *file,*fileWriteFinal;

    if ((file = fopen("arqdados-100.dat", "r")) == NULL) {
       printf("Erro na abertura do arquivo --- programa abortado\n");
       exit(1);
    }
    fseek(file, SEEK_SET, 0);
    //load(status);

    //fread(&totalRegistros ,1,sizeof(int),file);
    fread(&regcont,1,sizeof(int),file);
    //Leitura Escrita Ordenação
    while(regcont > contRegRead){
        fread(buffer,tambuffer,sizeof(str_reg),file);
        contRegRead += tambuffer;
        quickSort(buffer,0,tambuffer - 1);

        FILE *fileWrite;
        char nomeCorrida[50];

        sprintf(nomeCorrida, "corrida_%i.txt", contCorrida++);

        if ((fileWrite = fopen(nomeCorrida, "w")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado\n");
           exit(1);
        }

        fwrite(buffer,tambuffer,sizeof(str_reg),fileWrite);
        fclose(fileWrite);
    }

    contArquivosEmUso = contCorrida;

     if ((fileWriteFinal = fopen("arquivo_saida.txt", "w")) == NULL) {
       printf("Erro na abertura do arquivo --- programa abortado\n");
       exit(1);
    }

    fclose(fileWriteFinal);

    for(index = 0 ;index < contCorrida ; index++){
        arr_rrn[index] = 0;
        fromBufToFile(index,arr_rrn,buffer);

    }

    while (contArquivosEmUso > 0 ){
        fromBufferToOutBuffer(buffer,bufferSaida,arr_rrn,tambuffer,tambufferSaida,&contsaida);

    }

    //Implementação primeira etapa do merging
    fclose(file);
    FILE *fp;
    if ((fp = fopen("arquivo_saida.txt", "r")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado\n");
           exit(1);
        }
        int h =0,x = 0;
        struct str_reg cx;
        fseek(fp,0,0);
         h = fread(&cx,1,sizeof(str_reg),fp);
    while(h!= 0){
        printf("ARQ ID: %i contador : %i \n" , cx.id , x++) ;
        h = fread(&cx,1,sizeof(str_reg),fp);
    }

    fclose(fp);
}
int gc = 0;
void fromBufferToOutBuffer(struct str_reg *buffer, struct str_reg *bufferSaida,int arr_rrn[],int tambuffer,int tambufferSaida,int *contSaida){
    int indiceI = 0, menorIndice = 0 ;
    for (indiceI = 0 ; indiceI < tambuffer - 1; indiceI++){
        if(buffer[menorIndice].id > buffer[indiceI + 1].id ){
            menorIndice = indiceI  + 1;
        }
    }

    bufferSaida[*contSaida] = buffer[menorIndice];
    *contSaida += 1;

    fromBufToFile(menorIndice,arr_rrn,buffer);

    if(tambufferSaida == *contSaida){

        FILE *fileWriteOutput;
        if ((fileWriteOutput = fopen("arquivo_saida.txt", "r+")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado\n");
           exit(1);
        }
        fseek(fileWriteOutput,0,SEEK_END);

        fwrite(bufferSaida,tambufferSaida,sizeof(str_reg),fileWriteOutput);
        *contSaida = 0;
        fclose(fileWriteOutput);
    }
}


void fromBufToFile(int index, int arr_rrn[],struct str_reg *buffer ){
        FILE *fileRead;
        char nomeCorrida[50];
        struct str_reg str_aux;
        int retornoQuantidade = -1;

        sprintf(nomeCorrida, "corrida_%i.txt", index);

        if ((fileRead = fopen(nomeCorrida, "r+")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado\n");
           exit(1);
        }

        fseek(fileRead,arr_rrn[index]++ * sizeof(str_reg),SEEK_SET);
        //Passar o tamanho do buffer no arquivo
        retornoQuantidade = fread(&str_aux,1,sizeof(str_reg),fileRead);

        if(retornoQuantidade == 0 ){
            buffer[index].id = 999999999;
            contArquivosEmUso--;
        }else{
            buffer[index] = str_aux;
        }
}


void trocaValores(struct str_reg* a, struct str_reg* b){
    struct str_reg aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

int divide(struct str_reg vec[], int esquerdo, int direito){
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

void quickSort(struct str_reg vec[], int esquerdo, int direito){
    int r;
    if (direito > esquerdo){
        r = divide(vec, esquerdo, direito);
        quickSort(vec, esquerdo, r - 1);
        quickSort(vec, r + 1, direito);
    }
}

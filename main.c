/******************************************************************************
* author: José Henrique Medeiros Felipetto
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

/*
	The structure below defines virtual pages
*/
typedef struct virtualPages{
    int valor;
    unsigned int R;
    unsigned int counter;
    struct virtualPages *prox;
} stPagVirtual;

/* Struct used to measure time*/
static struct timeval start_time;
typedef long long int64;

stPagVirtual *frameHead;

/* PROTOTYPES */
stPagVirtual *readFile(const char *arq);
void execute(stPagVirtual *entrada,int frames,int arq);
int searchOnFrame(stPagVirtual *frame, int value, int frames,int bitNumber);
void replaceOnFrame(stPagVirtual *frame, int from, int to, int frames);
void addOnFrame(stPagVirtual *frame, int value);
void showStruct(stPagVirtual *list);
void envelhece(stPagVirtual *frameHead, int bitNumber);
void replacePage(stPagVirtual *frameHead, int pageToAdd,int count, int frames, int arq);
/* END PROTOTYPES */


void init_time() {
    gettimeofday(&start_time, NULL);
}

int64 get_time() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return (int64) (t.tv_sec - start_time.tv_sec) * 1000000
    + (t.tv_usec - start_time.tv_usec);
}


/*
	main function
*/
int main(){
 
    return 0;
}

int test(){
	   stPagVirtual *entradasHead;   /* Guarda um ponteiro para o inicio da lista. Utilizado para economizar processamento. */
    int64 inicio, fim;

    // Arq 1
    stPagVirtual *entradas = readFile((const char *)"arquivo1.txt");

    inicio = get_time();
    execute(entradas,4,1);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    //exit(-1);

    // Arq2
    entradas = readFile((const char *)"arquivo2.txt");
    inicio = get_time();
    execute(entradas,4,2);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    // Arq3
    entradas = readFile((const char *)"arquivo3.txt");

    entradasHead = entradas;
    inicio = get_time();
    execute(entradas,64,3);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,256,3);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,1024,3);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,2048,3);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    // Arq 4
    entradas = readFile((const char *)"arquivo4.txt");

    entradasHead = entradas;
    inicio = get_time();
    execute(entradas,64,4);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,256,4);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,1024,4);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));

    entradas = entradasHead;
    inicio = get_time();
    execute(entradas,2048,4);
    fim = get_time();
    printf("Tempo corrido: %.2f us \n\n ", (double)(fim-inicio));
     
    return 0;
}

/*
	Open a file and return a linked list with the values
*/
stPagVirtual *readFile(const char *arq)
{


    stPagVirtual *inicio, *pagina, *pag_ant;
	FILE *arquivo;
	int valor;
	char req[10];

	arquivo = fopen (arq, "r");
    pagina = malloc(sizeof(struct virtualPages));
    inicio = pagina;
    pag_ant = pagina;
    inicio->valor=valor;
    fgets(req,9,arquivo);       			/* Le o arquivo */
	valor = atoi(req);						/* Converte o string lido para inteiro */
    inicio->valor=valor;
    inicio->prox = NULL;

	while(!feof(arquivo) )
	{
		fgets(req,9,arquivo);				/* Le o arquivo */
		if (!feof(arquivo))					/* Verifica se Ã© fim de arquivo */
		{
			valor = atoi(req);
            pagina = malloc(sizeof(stPagVirtual)); 		/* Aloca nova pÃ¡gina na memÃ³ria  */
            pagina->valor=valor;
            pagina->R = 0;
            pagina->counter = 0;

            pag_ant->prox = pagina;						/* Inclui a pÃ¡gina na estrutura */
            pag_ant = pagina;
            pagina->prox = NULL;
		}
	}

    return inicio;
}
/*
	execute the algorithm
*/
void execute(stPagVirtual *entrada,int frames, int arq){
    int page_faults = 0, a, req = 0,ret = -1, frame_count = 1;
    int ciclo = 0, bitNumber=0;
    stPagVirtual *frame = malloc(sizeof(stPagVirtual)), *entradaHead;
    frame->valor = entrada->valor; frame->prox = NULL, frame->counter = (unsigned int)(arq <= 2 ? 8 : 128), frame->R = 0;
    frameHead = frame;
    entradaHead = entrada;
    entrada = entrada->prox;
    req++;

    while(entrada != NULL){
        if(arq > 2){
            /* Como o envelhecimento, independente do numero de frames, vai ocorrer com no minimo 20 ciclos, coloquei esse
               if para agilizar o código de verificações desnecessárias
            */
            //if(ciclo >= 20){
                if(frames == 64){
                    if(ciclo == 20){
                        ciclo = 0;
                        envelhece(frameHead,bitNumber);
                    }
                }else if(frames == 256){
                    if(ciclo == 100){
                        ciclo = 0;
                        envelhece(frameHead,bitNumber);
                    }
                }
                else if(frames == 1024){
                    if(ciclo == 400){
                        ciclo = 0;
                        envelhece(frameHead,bitNumber);
                    }
                }else if(frames == 2048){
                    if(ciclo == 800){
                        ciclo = 0;
                        envelhece(frameHead,bitNumber);
                    }
                }
            //}

        }else{
            // cada leitura é um ciclo.
            envelhece(frameHead,bitNumber);
        }



        /* Procura a pagina na memoria.
            Se ele achar, ele vai setar o bit R da página como 1(página foi referenciada)
        */
        if(arq <= 2)
            bitNumber = 4;
        else
            bitNumber = 8;
        ret = searchOnFrame(frameHead,entrada->valor,frames,bitNumber);
        if(!ret){ // page fault ocorreu
            if(frame_count >= frames){
                //printf("PageFault\n");
                page_faults++;
            }

            //printf("Frame_count: %d\n",frame_count);
            replacePage(frameHead,entrada->valor,frame_count,frames,arq);
            frame_count++;
        }
        //showStruct(frameHead);
        entrada = entrada->prox;
        req++;
        ciclo++;
    }
    printf ("Arquivo %d, %5d páginas, %7d requisições: NFU: %5d Page Faults ||| ",arq, frames, req, page_faults) ;
    free(frameHead);
}


void envelhece(stPagVirtual *frameHead, int bitNumber){
    stPagVirtual *aux = frameHead;
    while(aux != NULL){
        // Desloca o counter um bit a direita
        //aux->counter >>= 1;
        aux->counter = (aux->counter >> 1) | ((unsigned int)aux->R << (bitNumber - 1));
        aux = aux->prox;
    }
    return;
}


void replacePage(stPagVirtual *frameHead, int pageToAdd,int count, int frames, int arq){
    stPagVirtual *aux = frameHead;
    // Se a memória ainda tem espaço vazio, apenas adiciona ao final da memória
    int auxCount = 0;
    if(count < frames){
        while(aux->prox != NULL){
            aux = aux->prox;
            auxCount++;
        }

        stPagVirtual *newNode = malloc(sizeof(stPagVirtual));
        newNode->valor    = pageToAdd;
        newNode->counter = (unsigned int)(arq <= 2 ? 8 : 128);
        newNode->R        = 0;
        newNode->prox     = NULL;
        aux->prox         = newNode;
    }else{
        // Procura a página com menor número de referências
        stPagVirtual *lessR = frameHead;
        unsigned int lessCounter = frameHead->counter;
        while(aux != NULL){
            if(aux->counter == 0){
                lessR = aux;
                break;
            }
            if(aux->counter < lessCounter){
                lessR = aux;
                lessCounter = aux->counter;
            }
            aux = aux->prox;
            auxCount++;
        }

        // Agora que achou a menor, substitui
        lessR->valor = pageToAdd;
        lessR->counter = ((unsigned int)(arq <= 2 ? 8 : 128));
    }

}


int searchOnFrame(stPagVirtual *frame, int value, int frames, int bitNumber){
    int count = 0;
    stPagVirtual *aux = frame;
    unsigned int ou = 0;
    while(aux!=NULL && count < frames){
        if(aux->valor == value){

            // Se achou, a página foi referenciada(lida). Atualiza no frame o bit R da página e adiciona no counter
            aux->R = 1;
            aux->counter = (aux->counter >> 1) | ((unsigned int)1 << (bitNumber - 1));
            aux->R = 0;
            return 1;
        }
        aux = aux->prox;
        count++;
    }
    return 0;
}



void showStruct(stPagVirtual *list){
    printf(" Struct \n");
    stPagVirtual *aux = list;
    while(aux != NULL){
        printf("counter: %d | ",aux->counter);
        printf("R: %d | ",aux->R);
        printf("Valor:%d | \n",aux->valor);
        aux = aux->prox;
    }
    printf("End Show Struct \n");
    printf(" =========================================\n");
}

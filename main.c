#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>
#include "list.h"

#define TRUE 1
#define FALSE 0
#define LENGTH 5000 // Ordem da matriz
#define THREADS 2    // Quantidade de threads
#define SEED 7       // Semente para geração de números aleatórios
#define NUMBER_OF_ELEMENTS (LENGTH * LENGTH)

// Constantes de controle
#define single_thread 0
#define multi_thread 1

// Para guardar as coordenadas dos números primos encontrados
typedef struct _number_
{
    int line;
    int column;
} Number;

// Argumentos que serão passados para as threads
typedef struct _thread_arguments_
{
    int initial_index;
    int final_index;
    SLList *list;
} thread_arguments;

// Variáveis globais
pthread_t thread[THREADS];
thread_arguments th_arguments[THREADS];
SLList *main_list;
int *matrix;

int main(void)
{
    // Inicializando as variáveis que irão guardar o tempo em microssegundos
    struct timeval inicio, fim;
    float tempo;

    // Alocando memória para a lista
    main_list = sllCreate();

    // Alocando memória para a estrutura vetor em que serão guardados os números
    allocateMemory();

    // Inserindo valres aleatórios na matriz
    insertRandomValues();

    printf("+ Ordem da matriz: %i\n", LENGTH);

    // Single-thread
    printf("[Identificando os números primos em single-thread...]\n");

    gettimeofday(&inicio, 0);
    identifyPrimeNumbers();
    gettimeofday(&fim, 0);

    tempo = (fim.tv_sec + fim.tv_usec / 1000000.0) - (inicio.tv_sec + inicio.tv_usec / 1000000.0);

    printf("[Tempo de processamento: %fs]\n\n", tempo);

    printf("Números primos encontrados: %i\n\n", main_list->number_of_elements);

    main_list->number_of_elements = 0;
    // Multi-threads
    printf("[Identificando os números primos com %i threads...]\n", THREADS);

    gettimeofday(&inicio, 0);
    separateBlocks();
    gettimeofday(&fim, 0);

    tempo = (fim.tv_sec + fim.tv_usec / 1000000.0) - (inicio.tv_sec + inicio.tv_usec / 1000000.0);

    printf("[Tempo de processamento: %fs]\n\n", tempo);

    printf("Números primos encontrados: %i\n", main_list->number_of_elements);

    free(matrix);

    return 0;
}

// Identificar números primos single-thread
void identifyPrimeNumbers()
{
    Number *element;
    int index_x, index_y;

    for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
    {
        index_x = i / LENGTH;
        index_y = i % LENGTH;

        if (isPrimeSqrt(matrix[i]))
        {
            element = (Number *)malloc(sizeof(Number));
            element->line = index_x;
            element->column = index_y;

            sllInsertAsLast(main_list, (void *)element);
        }
    }
}

// Identificar números primos multi-thread
void *identifyPrimeNumbersMT(void *id_thread)
{
    Number *element;
    int id = (int)id_thread;
    int initial_index = th_arguments[id].initial_index;
    int final_index = th_arguments[id].final_index;

    th_arguments[id].list = sllCreate(); // Criando lista auxiliar

    for (int i = initial_index; i < final_index + 1; i++)
    {
        if (isPrimeSqrt(matrix[i]))
        {
            element = (Number *)malloc(sizeof(Number));

            element->line = i / LENGTH;
            element->column = i % LENGTH;
            sllInsertAsLast(th_arguments[id].list, (void *)element);
        }
    }

    pthread_exit(NULL);
}

// Separar os blocos em que será executada a função de identificar os números primos
void separateBlocks()
{
    int elements_per_thread = NUMBER_OF_ELEMENTS / THREADS;
    Number *element;

    for (int i = 0; i < THREADS; i++)
    {
        int initial_index = i * elements_per_thread;
        int final_index = initial_index + elements_per_thread - 1;

        th_arguments[i].initial_index = initial_index;
        th_arguments[i].final_index = final_index;

        pthread_create(&(thread[i]), NULL, identifyPrimeNumbersMT, (void *)i);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    printf("Iniciando a inserção na lista principal...\n");
    clock_t inicio = clock();

    for (int i = 0; i < THREADS; i++)
    {
        element = sllGetFirst(th_arguments[i].list);
        while (element != NULL)
        {
            sllInsertAsLast(main_list, element);
            element = sllGetNext(th_arguments[i].list);
        }
    }

    clock_t fim = clock();
    printf("Tempo que levou para inserir todos os números na lista principal: %f\n", (float)(fim - inicio) / CLOCKS_PER_SEC);
}

int isPrimeSqrt(int n)
{
    if (n < 3 || (n % 2 == 0 && n > 2))
    {
        return FALSE;
    }

    for (int i = 3; i <= sqrt(n) + 1; i++)
    {
        if (n % i == 0)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void printValues()
{
    for (int i = 0; i < LENGTH * LENGTH; i++)
    {
        printf("[%i] ", matrix[i]);
    }
}

void printList()
{
    Number *element;
    int counter = 0;

    element = sllGetFirst(main_list);

    while (element != NULL)
    {
        printf("Coordenadas %i { Linha: %i, Coluna: %i }\n", ++counter, element->line, element->column);
        element = sllGetNext(main_list);
    }
}

void allocateMemory()
{
    matrix = (int *)malloc(sizeof(int *) * LENGTH * LENGTH);
}

void insertRandomValues()
{
    int count = 0;
    srandom(SEED);
    for (int i = 0; i < LENGTH * LENGTH; i++)
    {
        matrix[i] = random() % 30000;
    }
}
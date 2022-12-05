#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct biggo
{
    int high;
    int low;
} BigInt;

void copy(BigInt *destination, BigInt *source, int size)
{
    int i;
    for (i = 0; i < size; ++i)
    {
        destination[i].high = source[i].high;
        destination[i].low = source[i].low;
    }
}

void insertData(BigInt *array, char *line, int i)
{
    // Função para inserir dado da linha para a i-ésima posição do array BigInt
    int j = 0;
    int k = 0;
    int high, low;
    char temp[256] = {0};
    
    // Extrair o valor high
    while (line[j] != ' ')
    {
        temp[j] = line[j];
        j++;
    }
    high = atoi(temp);
    
    // Resetar a variável temp
    memset(temp, 0, 256);
    
    // Extrair o valor low
    j++;
    while (line[j] != '\n')
    {
        temp[k] = line[j];
        j++;
        k++;
    }
    low = atoi(temp);
    
    // Inserir os dados (high e low) na i-ésima posição do array
    array[i].high = high;
    array[i].low = low;
}

int lt(BigInt *smaller, BigInt *bigger)
{
    // Função para verificar se smaller é menor que bigger
    if (smaller->high < bigger->high)
    {
        return 1;
    }
    else if (smaller->high == bigger->high)
    {
        if (smaller->low < bigger->low)
        {
            return 1;
        }
        else {
            return 0;
        } 
    }
    else
    {
        return 0;
    }
}

int le(BigInt *smaller, BigInt *bigger)
{
    // Função para verificar se smaller é menor ou igual que bigger
    if (smaller->high < bigger->high)
    {
        return 1;
    }
    else if (smaller->high == bigger->high)
    {
        if (smaller->low <= bigger->low)
        {
            return 1;
        }
        else {
            return 0;
        } 
    }
    else
    {
        return 0;
    }
}

void shellSort(BigInt *array, int size)
{
    int i, j;
    BigInt temp;
    
    int k = 1;
    while (k < size)
    {
        k = 3*k + 1;
    }
    while (k > 0)
    {
        for (i = k; i < size; ++i)
        {
            temp.high = array[i].high;
            temp.low = array[i].low;
            j = i;
            while (j > k-1 && le(&temp, &array[j-k]))
            {
                array[j].high = array[j-k].high;
                array[j].low = array[j-k].low;
                j = j - k;
            }
            array[j].high = temp.high;
            array[j].low = temp.low;
        }
        k = k/3;
    }
}

void selectionSort(BigInt* array, int size)
{
    int i, j, k;
    BigInt min;
    BigInt temp;
    for (i = 0; i < size; ++i)
    {
        k = i;
        min.high = array[i].high;
        min.low = array[i].low;
        for (j = i+1; j < size; ++j)
        {
            if (lt(&array[j], &min))
            {
                k = j;
                min.high = array[j].high;
                min.low = array[j].low;
            }
        }
        temp.high = array[i].high;
        temp.low = array[i].low;
        array[i].high = min.high;
        array[i].low = min.low;
        array[k].high = temp.high;
        array[k].low = temp.low;
    }
}

int main(int argc, char **argv)
{
    int i = 0;
    char c;
    char *line = NULL;
    size_t len;
    FILE *file1 = NULL;
    FILE *file2 = NULL;
    int size = 0;
    BigInt *original = NULL;
    BigInt *sorted = NULL;
    clock_t begin, end;
    double time;
    
    // Alocar memória para ler o conteúdo de cada linha
    line = (char*)calloc(256, sizeof(char));
    if (line == NULL)
    {
        printf("Não foi possível alocar memória!\n");
        return -1;
    }
    
    // Abrir o arquivo bigint.dat para ler
    file1 = fopen("bigint.dat", "r");
    if (file1 == NULL)
    {
        printf("Não foi possível abrir o arquivo bigint.dat!\n");
        free(line);
        return -1;
    }
    
    // Contar quantas linhas tem no arquivo bigint.dat
    for (c = getc(file1); c != EOF; c = getc(file1))
    {
        if (c == '\n')
        {
            size++;
        }
    }
    
    // Alocar memória para os dados do tipo BigInt
    original = (BigInt*)malloc(size*sizeof(BigInt));
    sorted = (BigInt*)malloc(size*sizeof(BigInt));
    if (original == NULL || sorted == NULL)
    {
        printf("Não foi possível alocar memória!\n");
        free(line);
        free(original);
        free(sorted);
        fclose(file1);
        return -1;
    }
    
    // Resetar o ponteiro do file1 para o início do arquivo
    rewind(file1);
    
    // Extrair os valores do arquivo e colocar em um array de BigInt
    while (getline(&line, &len, file1) != -1)
    {
        insertData(original, line, i);
        i++;
    }
    
    // Fechar o arquivo bigint.dat
    fclose(file1);
    
    // Medir o tempo de execução do shell sort
    printf("Shell Sort:\n");
    for (i = 0; i < 5; ++i)
    {
        // Resetar o array sorted
        copy(sorted, original, size);
        
        // Medir o tempo
        begin = clock();
        shellSort(sorted, size);
        end = clock();
        
        // Calcular o tempo
        time = (double)(end - begin)/CLOCKS_PER_SEC;
        
        printf("%d: %.6lf s\n", i+1, time);
    }
    
    // Medir o tempo de execução do selection sort
    printf("\nSelection Sort:\n");
    for (i = 0; i < 5; ++i)
    {
        // Resetar o array sorted
        copy(sorted, original, size);
        
        // Medir o tempo
        begin = clock();
        selectionSort(sorted, size);
        end = clock();
        
        // Calcular o tempo
        time = (double)(end - begin)/CLOCKS_PER_SEC;
        
        printf("%d: %.6lf s\n", i+1, time);
    }
    
    // Copiar os dados
    copy(sorted, original, size);
        
    // Ordenal pelo shell sort
    shellSort(sorted, size);
    
    // Criar o arquivo shell.dat para escrever os resultados nele
    file2 = fopen("shell.dat", "w");
    if (file2 == NULL)
    {
        printf("Não foi possível criar o arquivo shell.dat!\n");
        free(line);
        free(original);
        free(sorted);
        return -1;
    }
    
    // Escrever os resultados ordenados no arquivo shell.dat
    for (i = 0; i < size; ++i)
    {
        fflush(file2);
        fprintf(file2, "%d %d\n", sorted[i].high, sorted[i].low);
    }
    
    // Fechar o arquivo shell.dat
    fclose(file2);
    
    // Liberar as memórias alocadas
    free(line);
    free(original);
    free(sorted);
    
    return 0;
}

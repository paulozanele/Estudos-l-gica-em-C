#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int sign(void)
{
    // Gerador de sinal pseudoaleatório (1 ou -1)
    int s = rand() % 2;
    if (s == 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int generate(void)
{
    // Gerador de número pseudoaleatórios entre 0 a 999.999.999
    int num;
    do
    {
        num = rand();
    }
    while (num > 999999999);
    return num;
}

int main(void)
{
    int i;
    int n = 200000;
    int high, low;
    FILE *file = NULL;
    
    // Criar o arquivo bigint.dat
    file = fopen("bigint.dat", "w");
    if (file == NULL)
    {
        printf("Não foi possível criar o arquivo bigint.dat!\n");
        return -1;
    }
    
    // Gerar a semente do gerador de números pseudoaleatórios
    srand(time(NULL));
    
    // Gerar n bigint e escrever no arquivo bigint.dat
    for (i = 0; i <= n; ++i)
    {
        high = sign() * generate();
        low = generate();
        
        fflush(file);
        fprintf(file, "%d %d\n", high, low);
    }
    
    // Fechar o arquivo bigint.dat
    free(file);
    
    return 0;
}

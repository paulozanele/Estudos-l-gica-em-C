#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isOnlyOne(char *s){
    // Função para verificar se só existe '1' na linha
    int i = 0;
    while (s[i] != '\n'){
    	if (s[i] != '1'){
            return 0;
        }
        i++;
    }
    return 1;
}

int baseWidth(char *s){
    // Função para calcular a largura base
    int i = 0;
    int n = 0;
    while (s[i] != '\n'){
        if (s[i] == '0'){
            while (s[i] == '0'){
                n++;
                i++;
            }
            break;
        }
        i++;
    }
    return n;
}

void trimString(char *trimmed, char *original){
    // Função para limpar o texto original da linha e deixar somente '0' e '1'
    int i = 0;
    while (original[i] != '\0'){
        if (original[i] == '0' || original[i] == '1'){
            trimmed[i] = original[i];
        }
        else{
            trimmed[i] = '\0';
            break;
        }
        i++;
    }
}

void barcode2Numbers(char *original, char *numbers, int base_width, int buffer_size){
    int i;
    int n = 0; // Contador da largura da barra/espaço em unidade de base_width
    int m = 0; // Contador de barra/espaço
    int k = 0; // Contador da quantidade de números (2 barras + 2 espaços)
    char code[4] = {0}; // Variável para armazenar as larguras do código
    char c = '1'; // Símbolo atual (barra ou espaço)
    
    int len = 0;
    char *trimmed = (char*)calloc(buffer_size, sizeof(char));
    
    // Limpar os dados da linha para só conter '0' e '1'
    trimString(trimmed, original);
    
    // Calcular o tamanho da nova string trimmed
    while (trimmed[len] != '\0')
    {
        len++;
    }
    
    // Esvaziar a string numbers
    memset(numbers, 0, buffer_size); 
    
    // Converter as barras em números
    for (i = 5*base_width; i < len-4*base_width; i+=base_width){  // Pular os primeiro 5 e últimos 5 blocos
        if (trimmed[i] == c){// Caso símbolo igual
        	n++;
        }
        else{ // Caso símbolo diferente
            c = trimmed[i];
            code[m] = n + 48;
            n = 1;
            m++;
        }
        if (m == 4){ // Caso completar 2 barras + 2 espaços
            // Calcular o número correspondente
            if (strncmp(code, "3211", 4) == 0){
                strcat(numbers, "0 ");
            }
            else if (strncmp(code, "2221", 4) == 0){
                strcat(numbers, "1 ");
            }
            else if (strncmp(code, "2122", 4) == 0){
                strcat(numbers, "2 ");
            }
            else if (strncmp(code, "1411", 4) == 0){
                strcat(numbers, "3 ");
            }
            else if (strncmp(code, "1132", 4) == 0){
                strcat(numbers, "4 ");
            }
            else if (strncmp(code, "1231", 4) == 0){
                strcat(numbers, "5 ");
            }
            else if (strncmp(code, "1114", 4) == 0){
                strcat(numbers, "6 ");
            }
            else if (strncmp(code, "1312", 4) == 0){
                strcat(numbers, "7 ");
            }
            else if (strncmp(code, "1213", 4) == 0){
                strcat(numbers, "8 ");
            }
            else if (strncmp(code, "3112", 4) == 0){
                strcat(numbers, "9 ");
            }
            
            m = 0;
            k++;
            if (k == 6){ // Caso completar 6 números
                // Trocar de símbolo
                if (c == '1'){
                    c = '0';
                }
                else{
                    c = '1';
                }
                
                // Pular as 2 barras + 3 espaços do meio
                i += 5*base_width;
                k = 0;
            }
        }
    }
    
    // Retirar o último espaço do numbers
    i = 0;
    while (numbers[i] != '\0'){
        i++;
    }
    numbers[i-1] = '\0';
    
    // Liberar a memória
    free(trimmed);
}

int main(void){
    int base_width;
    int buffer_size = 1024;
    char *numbers = NULL;
    size_t len = 0;
    char *line = NULL;
    char *buffer = NULL;
    FILE *file1 = NULL;
    FILE *file2 = NULL;
    
    // Alocar memória
    numbers = (char*)calloc(buffer_size, sizeof(char));
    line = (char*)calloc(buffer_size, sizeof(char));
    buffer = (char*)calloc(buffer_size, sizeof(char));
    if (numbers == NULL || line == NULL || buffer == NULL)
    {
        printf("Não foi possível alocar memória!\n");
        free(numbers);
        free(line);
        free(buffer);
        return -1;
    }
    
    // Abrir o arquivo para ler
    file1 = fopen("barras.dat", "r");
    if (file1 == NULL){
        printf("Não foi possível abrir o arquivo barras.dat!\n");
        free(numbers);
        free(line);
        free(buffer);
        return -1;
    }
    
    // Abrir o arquivo para escrever
    file2 = fopen("codigos.dat", "w");
    if (file2 == NULL){
        printf("Não foi possível abrir o arquivo codigos.dat!\n");
        free(numbers);
        free(line);
        free(buffer);
        fclose(file1);
    }
    
    // Ler o arquivo linha por linha
    while (getline(&line, &len, file1) != -1){
        // Pular as linhas duplicadas
        if (strncmp(buffer, line, buffer_size) == 0){
            continue;
        }
        
        // Pular as linhas que contêm somente '1'
        if (isOnlyOne(line)){
            continue;
        }
        
        // Calcular a largura base da barra
        base_width = baseWidth(line);
        
        // Converter as barras em números
        barcode2Numbers(line, numbers, base_width, buffer_size);
        
        // Escrever os número no arquivo codigos.dat
        fprintf(file2, "%s\n", numbers);
        
        // Mostrar os números na tela
        printf("%s\n", numbers);
        
        // Copiar o conteúdo da linha em um buffer
        strncpy(buffer, line, buffer_size);
    }
    
    // Liberar as memórias alocadas
    free(numbers);
    free(line);
    free(buffer);
    
    // Fechar os arquivos
    fclose(file1);
    fclose(file2);
    
    return 0;
}

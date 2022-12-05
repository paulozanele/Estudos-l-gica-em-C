#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct temperatura
{
	int ano;
	int mes;
	char tempMinimaMes[10];
	char tempMediaMes[10];
	char tempMaximaMes[10];
}temperatura;
temperatura temperaturas[2500]; //Struct para definir as temperaturas conforme cada linha do arquivo

FILE* abreArq(char modo, char caminho[50]){
	FILE *arquivo;
	switch (modo){
		case 'g':
			arquivo=fopen(caminho,"wt");
			break;
		case 'l':
			arquivo=fopen(caminho,"rt");
			break;
		case 'a':
			arquivo=fopen(caminho,"a");
			break;		

	}
	if(arquivo==NULL){
		printf("Não foi possivel abrir o arquivo.");
		return 0;
	}
	return arquivo;
}

void fechaArq(FILE *arquivo){
		fclose(arquivo);
	}

int leituraArq(FILE *arquivo, struct temperatura *dadosTemperatura){
	int i;
	i = 0;
	arquivo = abreArq('l',"temps.dat");
	while(!feof(arquivo)){
		fscanf(arquivo,"%d %d %s %s %s", &dadosTemperatura[i].ano, &dadosTemperatura[i].mes, dadosTemperatura[i].tempMinimaMes, dadosTemperatura[i].tempMediaMes, dadosTemperatura[i].tempMaximaMes );
		i++;
	}
	fechaArq(arquivo);
	printf ("Leitura realizada com sucesso.\n");
	return i;
}

void funcaoCalcula(struct temperatura *dadosTemperatura, int contador, int mes){
	FILE *arquivoF;
	int i, j, aux;
	aux = 0;
	
	float totalTempMinMes;
	float totalTempMaxMes;
	float totalTempMediaMes;
	float menorTempMinMes;
	float maiorTempMaxMes;
	
	maiorTempMaxMes=-500; // Inicializei como se -500 e 500 fosse o minimo e maximo absoluto de temperatura, para que nao de problema nos if
	menorTempMinMes=500; 

	float mediaTempMinMes;
	float mediaTempMaxMes;
	float mediaTempMediaMes;
	
	for (i=0;i<=contador;i++){		
		if (dadosTemperatura[i].mes==mes){ // Médias, Maximas e MinimasCorretas.
			//printf("%f\n",strtod(dadosTemperatura[i].tempMinimaMes,NULL));
			if (strtod(dadosTemperatura[i].tempMinimaMes,NULL)<menorTempMinMes)
				menorTempMinMes = strtod(dadosTemperatura[i].tempMinimaMes,NULL);// Pegando a menor temperatura minima do mes
				//printf ("%f\n",menorTempMinMes);
        	if (strtod(dadosTemperatura[i].tempMaximaMes,NULL)>maiorTempMaxMes)
        		maiorTempMaxMes = strtod(dadosTemperatura[i].tempMaximaMes,NULL);// Pegando a maior temperatura maxima do mes
		
			totalTempMinMes = totalTempMinMes+strtod(dadosTemperatura[i].tempMinimaMes,NULL);//Somando o total de temperatura minima naquele mês
			totalTempMaxMes = totalTempMaxMes+strtod(dadosTemperatura[i].tempMaximaMes,NULL);//Somando o total de temperatura maxima naquele mês
			totalTempMediaMes = totalTempMediaMes+strtod(dadosTemperatura[i].tempMediaMes,NULL);//Somando o total de temperatura maxima naquele mês
			aux++;
		}
	}
	
	mediaTempMinMes=totalTempMinMes/aux;
	mediaTempMaxMes=totalTempMaxMes/aux;
	mediaTempMediaMes=totalTempMediaMes/aux;
	printf("%d %.3f (%.3f) %.3f %.3f (%.3f)",mes , mediaTempMinMes ,menorTempMinMes ,mediaTempMediaMes ,mediaTempMaxMes, maiorTempMaxMes);
	arquivoF = abreArq('a',"media.dat");
	fprintf(arquivoF,"%d %.3f (%.3f) %.3f %.3f (%.3f)\n",mes , mediaTempMinMes ,menorTempMinMes ,mediaTempMediaMes ,mediaTempMaxMes, maiorTempMaxMes);
	fclose(arquivoF);
}


int main(){

	FILE *arquivo;
	int aux, i, j;
	aux = leituraArq(arquivo,temperaturas);
	for (j=1; j<=12; j++){
	funcaoCalcula(temperaturas,aux,j);
	printf ("\n");
	}
	return 0;
}

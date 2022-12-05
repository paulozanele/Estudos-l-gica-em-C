#include<stdio.h>

//Essa função calcula os valores de enteresse, explicarei linha por linha 

/*
valor_combinado: 
	É valor que queremos combinar, no caso teste (148 206 14), no entanto mandaremos um por 
um em for no programa principal. 

	soma: tive que utilizar essa variável devido a recursividade da função. Cada vez que "afundo"
dentro da recursividade ela vai aumentando, se ficar igual à valor_combinado encontramos uma combinação.
	
	M[]: usada para receber o vetor que possue os números do conjunto que vamos usar para a 
combinação, no nosso caso é ( 1 10 11 21 27 33 34 46 49 61 62 67 73 77 79)

	contador: Armazena a quantidade de combinações encontradas

	inicio: foi preciso devido a recursividade, é usado para dizer onde o lup ira começar
conforme explicarei posteriormente 
	final: basecamente é o tamanho do vetor M[], é utilizado para ditar o final do for da função
*/
int recursiva(int valor_combinado, int soma, int M[], int contador, int inicio, int final)
	{int l,i, son; // inteiros para auxiliar no código 
		son = soma;// precisamos fazer isso para reiniar o valor da soma corretamente, depois de somar erradas
	//	printf("\n\n--------------------------------------------------\n\n");
	//	printf("\n\n soma inicial: %d\n\n", soma);
		if(soma >= valor_combinado)	// se já tivermos utrapassado ou igualado ao procurado, não vamos mais incrementar soma
			{
				if(soma == valor_combinado) { // se for igual, encontramos uma combinação 
					//printf("\n\nEu nao deveria estar aqui!!!!!!!!!\n\n", contador); 
					//system ("pause");
					return contador + 1; //como encontramos uma combinação, incrementamos o contador
				}else
				{
					//printf("\n\nEstou dentro!!!!!!!\n\n");
					return contador; // se tivermos passado o procurado, matemos o valor do contador
					
				}
				
			}
		else // nesse caso, a soma ainda é menor que o procurado (148 206 14)
			{
				//printf(" passou o else - improvável");
				for(i = inicio; i<final; i++) // nesse for, estamos variando o enezimo elemento da soma
					{soma = son; //essa e a próxima linha, corrigem a variavel soma, depois de mudarmos o enezimo elemento da soma
					soma = soma + M[i];
					//printf("\nElemento do conjunto: %d \n soma = %d ",M[i], soma);
					//system ("pause"); 
					
					contador = recursiva(valor_combinado, soma, M, contador ,( i+1 ) , final); // aqui, chamos a propria função que estamos, para gerar o próximo elemento da soma
				//se a soma com esse próximo elemento for igual ao valor_combinado o primeiro if desse função ira incrementar o contador
						
					}
				
			return contador; // quanto terminar todas a posibilidade ele retornar a valor do contador que buscamos	
			}			
	}

int main()
	{
	int M, N; // variavéis de entrada 
	int i,j,ax;// auxiliares 

	
	scanf("%d", &N);
	
	scanf("%d", &M);

	/*
	Conjunto[M]: vetor que recebe os elementos do conjunto que usaremos para combinar 
	
	Descomposicao[N]: Os valores que queremos decompor são recebidos por esse vetor 
	
	resultado[N]:
	*/
	
	//Recebendo os vetore
		int Conjunto[M], Descomposicao[N], resultado[N];		
	for(i=0;i<M;i++)
		{
			scanf("%d", &Conjunto[i]);
		}	
		
		for(i=0;i<N;i++)
		{
			scanf("%d", &Descomposicao[i]);
		}
		
//organizando o vetor conjunto em ordem crescente
	for(i=0;i<M;i++)
		{
		for(j=i+1;i<M;i++)
		{
			if(Conjunto[i] > Conjunto[i])
			{
				ax = Conjunto[i];
				Conjunto[i] = Conjunto[j];
				Conjunto[j] = ax;
			}
		}	
		}
		
	// chamaos a função para cada um dos elementos que queremos decompor
	for(i=0;i<N; i++)
	{
	resultado[i] = recursiva(Descomposicao[i], 0, Conjunto, 0, 0, M);	
	printf("\n%d",resultado[i] );
	}
	
	

	
	return 0;
	
}




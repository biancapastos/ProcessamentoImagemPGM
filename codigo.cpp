#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

// ----------------------- prototipos ----------------------------------
void cabecalho(FILE *fp, char *tipo_arq, char *num_col, char *num_lin, char *cinza);
int ** preenche_mat(int **mat, int col, int lin, FILE *fp);

int * limiarizacao(int **mat, int *limiar, int col, int lin);
void img_limiar(int *limiar, char *tipo_arq, char *num_col, int col, char *num_lin, int lin, char *cinza);

int * negativo(int **mat, int *neg, int col, int lin);
void img_negativo(int *neg, char *tipo_arq, char *num_col, int col, char *num_lin, int lin, char *cinza);

void histograma(int **mat, int col, int lin, int max_cinza);
void escreve_histograma(int *vet, int pos);
// ---------------------------------------------------------------------

int main(){
	
	FILE *fp;
	char tipo_arq[10], num_col[10], num_lin[10], cinza[10];
	int j;
	int **mat;
	int *limiar;
	int *neg;
	
	// abre o arquivo da imagem para leitura
	fp = fopen("exemplo.pgm", "r");
	if(fp == NULL){
		printf("Nao foi possivel abrir o arquivo!");
	}

	cabecalho(fp, tipo_arq, num_col, num_lin, cinza);						// chama funcao p/ ler o cabecalho do arquivo
	
	// converte strings char para inteiros tipo long int na base 10
	int max_cinza = strtol(cinza, NULL, 10); 
	int col = strtol(num_col, NULL, 10);
	int lin = strtol(num_lin, NULL, 10);
	
	mat = preenche_mat(mat, col, lin, fp);									// cria mat e armazena os pixels da imagem
	
	// limiarizacao 
	limiar = limiarizacao(mat, limiar, col, lin);							// cria matriz com valores trocados de acordo com limiar
	img_limiar(limiar, tipo_arq, num_col, col, num_lin, lin, cinza);		// cria arq da nova imagem com os novos valores
	
	// negativo	
	neg = negativo(mat, neg, col, lin);										// cria matriz com valores trocados para negativo
	img_negativo(neg, tipo_arq, num_col, col, num_lin, lin, cinza);			// cria arq da nova imagem com os novos valores

	// histograma
	histograma(mat, col, lin, max_cinza);
	
	fclose(fp);
	free(mat);
	free(limiar);
	free(neg);
}

// ------------------------- funcoes -----------------------------------

void cabecalho(FILE *fp, char *tipo_arq, char *num_col, char *num_lin, char *cinza){

	fgets(tipo_arq, 100, fp);		// le o tipo do arquivo
	fscanf(fp, "%s", num_col);		// le o num de colunas
	fscanf(fp, "%s", num_lin);		// le o num de linhas
	fscanf(fp, "%s", cinza);		// le o max de cinza
}

int ** preenche_mat(int **mat, int col, int lin, FILE *fp){

	int i=0, j=0;
	char *aux = (char *) malloc (sizeof(char));			// aux que le os numeros no arquivo para inserir na mat
	int num;
	
	// aloca mat dinamicamente na memoria do tam da imagem
	mat = (int **) malloc (lin * sizeof(int *));
	for(j=0; j<col; j++){
		mat[j] = (int *) malloc (col * sizeof(int));
	}
	j=0;
	
	while( (i != lin) && (j != col) ){			// enquanto nao for o fim do arquivo
		
		fscanf(fp, "%s", aux);					// le os numeros 
		num = strtol(aux, NULL, 10);			// converte a string para inteiro na base 10
		
		mat[i][j] = num;						// armazena na mat o valor do pixel da imagem e incrementa a coluna
		j++;
		
		if(j == col){						// quando preencher toda a primeira linha, incrementa para a proxima e zera a coluna
			i++;
			j=0;
		}
	}
	return mat;		// retorna a matriz com os pixels da img original
}

// processo p/ imagem limiarizada

int * limiarizacao(int **mat, int *limiar, int col, int lin){

	int i, j;

	// aloca mat dinamicamente na memoria - vet que armazena todos os elementos da linha 1 seguido pelos da linha 2 e assim continua...
	limiar = (int *) malloc (lin * col * sizeof(int));
	
	// cria matriz limiar com os novos valores
	for(i=0; i<lin; i++){
		for(j=0; j<col; j++){
			
			if(mat[i][j] < 150){			// se valor original for menor que 150 troca por 0
				
				limiar[(i*col)+j] = 0;
			}else{							// se valor original for maior que 150 troca por 255
				limiar[(i*col)+j] = 255;
			}
		}
	}
	
	return limiar;		// retorna matriz limiar com os valores trocados
}

void img_limiar(int *limiar, char *tipo_arq, char *num_col, int col, char *num_lin, int lin, char *cinza){

	int i, j;
	FILE *fpl;
	char str[5];		// string aux para receber o valor inteiro na matriz limiar
	
	// abre novo arquivo pgm 
	fpl = fopen("img_limiarizada.pgm", "w");
	if(fpl == NULL){
		printf("Nao foi possivel abrir o arquivo!");
	}
	
	// escreve o cabecalho pgm no arquivo
	fputs(tipo_arq, fpl);
	fputs(num_col, fpl);
	fputc(32, fpl);
	fputs(num_lin, fpl);
	fputc(10, fpl);	
	fputs(cinza, fpl);
	fputc(10, fpl);	
	
	for(i=0; i<lin; i++){
		for(j=0; j<col; j++){
			
			sprintf(str, "%d", limiar[(i*col)+j]);		// Converte os valores inteiros na matriz para string e armazena em str
			fputs(str, fpl);							// escreve no arquivo o numero como um texto
			fputc(32, fpl);								// coloca um espaco entre os numeros
		}
		fputc(10, fpl);							// quando acaba uma linha acrescenta pula
	}
	
	fclose(fpl);
}

// processo p/ imagem negativa

int * negativo(int **mat, int *neg, int col, int lin){

	int i, j;

	// aloca mat dinamicamente na memoria - vet que armazena todos os elementos da linha 1 seguido pelos da linha 2 e assim continua...
	neg = (int *) malloc (lin * col * sizeof(int));
	
	// cria matriz negativo com os novos valores
	for(i=0; i<lin; i++){
		for(j=0; j<col; j++){
			
			neg[(i*col)+j] = 255 - mat[i][j];
		}
	}

	return neg;		// retorna matriz neg com os valores trocados
}

void img_negativo(int *neg, char *tipo_arq, char *num_col, int col, char *num_lin, int lin, char *cinza){

	int i, j;
	FILE *fpn;
	char str[5];			// string aux para receber o valor inteiro na matriz limiar
	
	// abre novo arquivo pgm 
	fpn = fopen("img_negativo.pgm", "w");
	if(fpn == NULL){
		printf("Nao foi possivel abrir o arquivo!");
	}
	
	// escreve o cabecalho pgm no arquivo
	fputs(tipo_arq, fpn);
	fputs(num_col, fpn);
	fputc(32, fpn);
	fputs(num_lin, fpn);
	fputc(10, fpn);
	fputs(cinza, fpn);
	fputc(10, fpn);
	
	for(i=0; i<lin; i++){
		for(j=0; j<col; j++){
			
			sprintf(str, "%d", neg[(i*col)+j]);		// Converte os valores inteiros na matriz para uma string
			fputs(str , fpn);						// escreve no arquivo o numero como um texto
			fputc(32, fpn);							// coloca um espaco entre os numeros
		}
		fputc(10, fpn);	
	}
	
	fclose(fpn);	
}

// processo p/ histograma

void histograma(int **mat, int col, int lin, int max_cinza){

	int i, j;
	int *vet = (int *) malloc (sizeof(int));
	int cont=0, pos=0;
	
	while(pos != max_cinza){				// enquanto pos do vet nao for igual a quant de tonalidades de cinza
		
		for(i=0; i<lin; i++){
			for(j=0; j<col; j++){
				
				if(mat[i][j] == (pos+1) ){		//conta quantos pixels no valor (de 1 a 255) tem - (pos+1 do vet pq pos comeca no 0)
					cont++;
				}
			}
		}
		
		vet[pos] = cont;					// cada pos do vet armazena a quant de vezes que aquela tonalidade aparece
		pos++;
		cont = 0;
	}
	
	escreve_histograma(vet, pos);			// chama funcao para escrever o arquivo txt com o histograma
}

void escreve_histograma(int *vet, int pos){

	int i;
	FILE *fph;
	
	// abre arquivo
	fph = fopen("histograma.txt", "w");
	if(fph == NULL){
		printf("Nao foi possivel abrir o arquivo!\n");
	}
	
	fputs("Histograma: \n", fph);
	// roda as posicoes do vet e vai escrevendo no arquivo que abriu
	for(i=0; i<pos; i++){
		fprintf(fph, " pixel %d: aparece %d vezes\n", i+1, vet[i]);
	}
	
	fclose(fph);
}
// -------------------------------------------------------------------

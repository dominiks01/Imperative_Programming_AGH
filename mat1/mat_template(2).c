#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A){
	return A[cols*row + col];
}

void set(int cols, int row, int col, double *A, double value){
	A[cols*row + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB){
	for(int i = 0; i < rowsA; ++i)
		for(int j = 0; j < colsB; ++j)
			for(int p = 0; p < colsA; ++p)
				set(rowsA, i, j, AB, get(colsA, i, p, A) * get(colsB, p, j, B) + get(rowsA, i, j, AB));	
}

void read_mat(int rows, int cols, double *t){
	for(int i = 0; i < rows * cols; ++i)
		if(!scanf("%lf", &t[i]))
			break;
}

void print_mat(int rows, int cols, double *t){
	for(int i = 0; i < rows * cols; ++i){
		printf("%lf ", t[i]);
		if(i%cols==1)
			printf("\n");
	}
}

int read_char_lines(char *tab[]){
	return 0;
};

void write_char_line(char *tab[], int n){
}

void delete_lines(char *tab[], int line_count){
	return;
}

int read_dbl_lines_v1(double *tab[]){
	char *bufer;
	size_t characters;
	size_t bufsize = 1000;

	char ditig[BUF_SIZE];
	double digits[BUF_SIZE][BUF_SIZE];

	int iterator = 0;
	int i = 0;

	char * pch;
	char * last;

	char find[] = " ";

	while(getline(&bufer, &bufsize, stdin) != EOF){
		last = bufer;
		pch = strpbrk(last, find);
		int i = 0;
		while(pch != NULL){
			char ditig[BUF_SIZE];
			memcpy(ditig, last+1, pch-last);
			//printf("%lf ", atof(ditig));
			digits[iterator][i] = atof(ditig);
			printf("%lf ",atof(ditig));

			last = pch+1;
			pch = strpbrk(last, find);
		}

		pch = last;
		while (*pch){
			pch++;
		}
		
		memcpy(ditig, last, pch-last);
		printf("%lf\n", atof(ditig));
		digits[iterator][i] = atof(ditig);
		tab[iterator] = &digits[iterator++][0];
	}

	//for(int i = 0; i<5; ++i)
	//	printf("%lf ", digits[i]);

	//printf("%d", iterator);

	return iterator;
	
}

void write_dbl_line_v1(double *tab[], int n){
	for(double * pch = tab[n+1]; *pch; ++pch)
		printf("%lf ", *pch);
}


int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(ptr_table);
			write_dbl_line_v1(ptr_table,n);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table);
			write_char_line(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

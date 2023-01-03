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
		printf("%.2f ", t[i]);
		if(i%rows==(rows-1))
			printf("\n");
	}
}


int read_char_lines(char *tab[], char* text){
	char c;
	int iterator = 0;
	int i =0;
	while((c = getchar()) != EOF){
		text[i++] = c;
		if(c == '\n')
			tab[iterator++] = &text[i];
	}
	return iterator;
}

void write_char_line(char *tab[], int n, char* txt){
	for(char *d = tab[n-1]; d < tab[n]; ++d){
		printf("%c", *d);
	}
}

void delete_lines(char *tab[], int line_count){
	//for(int i = 0; i < line_count; ++i)
		//free(tab[i]);
		return;
}

int read_dbl_lines_v1(double *ptr_tab[], double *digits){
	char *buffer;
	char find[] = " ";
	
	int iterator = 0;
	int i = 0;

	size_t bufsize = 1000;

	while (getline(&buffer, &bufsize, stdin) != EOF){
		char* start = buffer;
		char* end = strpbrk(buffer, find);
		
		while(end!=NULL){	
			char digit[BUF_SIZE] = {};
			memcpy(digit, start, end-start);
			digits[i++] = atof(digit);
			start = end+1;
			end = strpbrk(start+1, find);
		}

		char *p = start;

		while (*p){
			p++;
		}
		
		char digit[BUF_SIZE] = {};
		memcpy(digit, start, p-start);
		digits[i++]= atof(digit);
		ptr_tab[iterator++] = &digits[i];

	}
	return iterator-1;
}

void write_dbl_line_v1(double *ptr_tab[], int n, double* digits){
	for(double *d = ptr_tab[n-1]; d < ptr_tab[n]; ++d){
		printf("%.2f ", (double)(*d));
	}
}

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	char text[TAB_SIZE*20];
	double series_table[TAB_SIZE];
	double digits[TAB_SIZE];
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
			lines_counter = read_dbl_lines_v1(ptr_table, digits);
			write_dbl_line_v1(ptr_table,n, digits);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table, text);
			write_char_line(char_lines_table,n, text);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}

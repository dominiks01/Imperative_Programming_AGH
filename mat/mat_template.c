#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]){}

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p], B[p][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n){
	//m wiersze A, p wiersze A, n kolumny B
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){ 
			for (int k = 0; k < p; k++){
				AB[j][i] += A[j][k] * B[k][i];
			}
		}
	}
	return;
}


// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n){}

void backward_substit(double A[][SIZE], double x[], int n){}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n){}
// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n){
	for (int i = 0; i < n; i++){ // kolumny
		for (int j = i+1; j < n; j++){ // wiersze
			double il = A[j][i] / A[i][i];
			for (int m = 0; m < n; m++){
				A[j][m] -= il * A[i][m];
			}
		}
	}
	
	int i = 0;
	double wzn = 1;
	while (i < n){
		wzn = wzn*A[i][i];
		i += 1;
	}
	//printf("%d", wzn);
	if (wzn == 0){
		return NAN;
	}
	return wzn;
}


int is_valid(int arr[], int x, int r){
	for(int i = 0; i <= r; ++i)
		if(arr[i] == x)
			return 0;
	return 1;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps){
	double result = 1;
	int index_vector[n];
	for(int i = 0; i < n; ++i)
		index_vector[i] = -1;

	double b_cpy[n];

	for(int i = 0; i < n; ++i)
		b_cpy[i] = b[i];

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			if(index_vector[i] == -1)
				index_vector[i] = j;
			else{
				if((A[j][i] > A[index_vector[i]][i] || (-1)*A[j][i] > A[index_vector[i]][i]) && is_valid(index_vector, j, i))
					index_vector[i] = j;
				}
		}

		for(int j = 0; j < n; ++j){
			if(is_valid(index_vector, j, i)){
				double value = 0;
				value = A[j][i]/A[index_vector[i]][i];
				for(int ij = 0; ij < n; ++ij)
					A[j][ij] -= (A[index_vector[i]][ij] * value);
				b_cpy[j] -= (b_cpy[index_vector[i]] * value);
			}
		}
	}

	int count_c = 0;
	for(int i = 0; i < n; ++i){
		result *= A[index_vector[i]][i];
		if(index_vector[i] != i)
			count_c += 1;
	}
	if(count_c % 2 == 0)
		result *= (-1);

	
	for(int j = n; j >=0; j--){
		x[j-1] = b_cpy[n-j]/A[n-j][j-1];
	}

	int i_v = n-1;

	for(int i = n-1; i>=0; --i){
		int act = index_vector[i];

		x[i] = b_cpy[act]/A[act][i_v];

		for(int j = 0; j < n; ++j){
			if(j != act)
				b_cpy[j] -= (x[i]*A[j][i_v]);
		}
		i_v -= 1;
	}
	
	return result;
	
}
	

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps){
	double result = 1;
	int index_vector[n];
	for(int i = 0; i < n; ++i)
		index_vector[i] = -1;

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j)
			B[i][j] = 0;
		B[i][i] = 1;
	}

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			if(index_vector[i] == -1)
				index_vector[i] = j;
			else{
				if((A[j][i] > A[index_vector[i]][i] || (-1)*A[j][i] > A[index_vector[i]][i]) && is_valid(index_vector, j, i))
					index_vector[i] = j;
				}
		}
		for(int j = 0; j < n; ++j){
			if(is_valid(index_vector, j, i)){
				double value = 0;
				value = A[j][i]/A[index_vector[i]][i];
				for(int ij = 0; ij < n; ++ij){
					A[j][ij] -= (A[index_vector[i]][ij] * value);
					B[j][ij] -= (B[index_vector[i]][ij] * value);
				}
			}
		}
	}

	int count_c = 0;

	for(int i = 0; i < n; ++i){
		result *= A[index_vector[i]][i];
		if(index_vector[i] != i)
			count_c += 1;
	}

	if(count_c % 2 == 0)
		result *= (-1);

	for(int i = 0; i < n; ++i){
		int act = index_vector[i];

		//printf("%d, %d\n", act, i);

		if(A[act][i] != 1){
			double value = 1/A[act][i];
			for(int ij = i; ij < n; ++ij){
				A[act][ij] *= value;
				//B[act][ij] *= value;
			}
			for(int ij = 0; ij<n; ++ij)
				B[act][ij] *= value;
		}

		for(int j = i+1; j < n; ++j){
			double value = A[act][j]/A[index_vector[j]][j];

			for(int ij = 0; ij < n; ++ij){
				A[act][ij] -= value*A[index_vector[j]][ij];
				B[act][ij] -= value*B[index_vector[j]][ij];
			}
		} 
	}

	double vect_cpy[n];
	double vec_cpy2[n];

	for(int i = 0; i < n; ++i){
		int act = index_vector[i];

		if(i < act){
			for(int j = 0; j < n; ++j){
				vect_cpy[j] = B[act][j];
				vec_cpy2[j] = B[i][j];
			}

			for(int j = 0; j < n; ++j){
				B[i][j]= vect_cpy[j];
				B[act][j] = vec_cpy2[j];
			}}
	}



	
	//printf("\n");
	//print_mat(A, n, n);
	//print_mat(B, n, n);


	return result;
	
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break; 
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}
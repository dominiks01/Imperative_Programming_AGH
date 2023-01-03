#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

int cmp(pair, pair);
int get_domain(pair*, int, int*);


// Add pair to existing relation if not already there
int add_relation (pair* relation, int size, pair p2){
	relation[size] = p2;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int size){
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	for(int i = 0; i < n; ++i){
		int flag = 0;
		for(int j = 0; j < size; ++j){
			if(relation[j].first == relation[j].second && relation[j].first == domain[i])
				flag = 1;
		}
		if(!flag)
			return 0;
	}
	return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size){
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	for(int i = 0; i < n; ++i){
		int flag = 0;
		for(int j = 0; j < size; ++j){
			if(relation[j].first == relation[j].second && relation[j].first == domain[i])
				flag = 1;
		}
		if(flag)
			return 0;
	}
	return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size){
	for(int i = 0; i < size; ++i){
		int flag = 0;
		for(int j = 0; j < size; ++j)
			if(relation[i].second == relation[j].first && relation[j].second == relation[i].first)
				flag = 1;

		if(!flag)
			return 0;
	}
	return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size){
	for(int i = 0; i < size; ++i){
		int flag = 1;
		for(int j = 0; j < size; ++j)
			if(relation[i].second == relation[j].first && relation[j].second == relation[i].first && !(relation[i].first = relation[i].second))
				flag = 0;

		if(!flag)
			return 0;
	}
	return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int size){
	return !is_symmetric(relation, size);
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int size){
	for(int i = 0; i < size; ++i){
		int flag = 0;
		int flag2 = 0;

		for(int j = 0; j < size; ++j)
			if(relation[i].second == relation[j].first){
				flag = 1;
				
				for(int p = 0; p < size; ++p)
					if(relation[p].first == relation[i].first && relation[j].second == relation[p].second)
						flag2 = 1;
			}
				
		if(flag && flag2 == 0)
			return 0;
	}
	return 1;
}

int is_connected(pair*, int);

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* relation, int size){
	return is_reflexive(relation, size) && is_antisymmetric(relation, size) && is_transitive(relation, size);
};
// A total order relation is a partial order relation that is connected
int is_total_order(pair* relation, int size){
	return is_partial_order(relation, size) && is_connected(relation, size);
}

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* relation, int size){
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			int flag = 0;

			for(int o = 0; o<size; ++o){
				//printf("%d - %d or %d - %d\n", relation[o].first, domain[i], relation[o].second, domain[j]);
				if((relation[o].first == domain[i] && relation[o].second == domain[j]) || 
				(relation[o].first == domain[j] &&  relation[o].second == domain[i]))
					flag = 1;
			}

			if(!flag)
				return 0;
		}
	}
	return 1;
}

void swap(int *a, int*b){
	int t = *a;
	*a = *b;
	*b = t;
}

int partition(int* arr, int p, int r){
	int x = arr[r];
	int i = p-1;

	for(int j = p; j <= r - 1; ++j)
		if(arr[j] < x) {
			i ++;
			swap(&arr[i], &arr[j]);
		}
	swap(&arr[i + 1], &arr[r]);
    return (i + 1);

}

void quick_sort(int* arr, int p, int r){
	if(p<r){
		int pi = partition(arr, p, r);
		quick_sort(arr, p, pi-1);
		quick_sort(arr, pi +1, r);
	}
}


int find_max_elements(pair* relation, int size, int* max_elements){
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	int iterator = 0;

	for(int i = 0; i < n; ++i){
		int flag = 1;

		for (int j = 0; j<size; ++j){
			if(domain[i] == relation[j].first && relation[j].first != relation[j].second)
				flag = 0;
		}

		if(flag)
			max_elements[iterator++] = domain[i];
	}

	return iterator;
}

int find_min_elements(pair* relation, int size, int* min_elements){
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	int iterator = 0;

	for(int i = 0; i < n; ++i){
		int flag = 1;

		for (int j = 0; j<size; ++j){
			if(domain[i] == relation[j].second && relation[j].first != relation[j].second)
				flag = 0;
		}

		if(flag)
			min_elements[iterator++] = domain[i];
	}

	return iterator;
}

int get_domain(pair* relation, int size, int* domain){
	int iterator = 0;
	for(int i = 0; i<size; ++i){
		int flag = 1;
		for(int j = 0; j < iterator; ++j)
			if(domain[j] == relation[i].first)
				flag = 0;
		if(flag)
			domain[iterator++] = relation[i].first;
		
		flag = 1;
		for(int j = 0; j < iterator; ++j)
			if(domain[j] == relation[i].second)
				flag = 0;
		if(flag)
			domain[i++] = relation[i].second;
	}

	quick_sort(domain, 0, iterator);
	return iterator;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

// Case 3:
int composition (pair* relation, int size1, pair* relation_2, int size2, pair* comp_relation){
	int iteration = 0;
	for(int i = 0; i < size2; ++i){
		for(int j = 0; j<size1; ++j){
			if(relation_2[j].second == relation[i].first){
				comp_relation[iteration].first = relation_2[i].first;
				comp_relation[iteration].second = relation[j].second;
				iteration++;
			}
		}
	}
	return iteration-1;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation){
	int n = 0;
	scanf("%d", &n);
	pair p;

	for(int i = 0; i < n; ++i){
		scanf("%d %d", &p.first, &p.second);
		add_relation(relation, i, p);
	}

	return n;
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size)); 
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


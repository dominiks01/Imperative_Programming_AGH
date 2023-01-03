#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  -1
#define LIST_ERROR               -2
#define PROGRAM_ERROR            -3

typedef union data_union {
	int int_data;
	char char_data;
	void *ptr_data;
} data_union;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef  int (*CompareDataFp)(data_union, data_union);
typedef  int (*HashFp)(data_union, int);

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef struct hash_table {
	int size;
	int no_elements;
	ht_element **ht;
	DataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if(ptr) return ptr;
	exit(MEMORY_ALLOCATION_ERROR);
}

void insert_element(hash_table *, data_union *);
void remove_element(hash_table *, data_union );

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {

	p_table->size = size;
	p_table->dump_data = dump_data;
	p_table->free_data = free_data;
	p_table->compare_data = compare_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;
	p_table->no_elements = 0;
	p_table->ht = (ht_element **)safe_malloc(size*sizeof(ht_element));

}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
	ht_element *start = p_table->ht[n];
	for (; start; start = start->next)
		p_table->dump_data(start->data);
	printf("\n");
	
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
	free_data(to_delete->data);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
	
	for(int i = 0; i < p_table->size; ++i){
		ht_element * head = p_table->ht[i];

		while (head != NULL) {
			ht_element *next = head->next;
			free(head);
			head = next;
		}
	}

	free(p_table->ht);
}


// calculate hash function for integer k
int hash_base(int k, int size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (int)floor(size * (tmp - floor(tmp)));
}

hash_table *rehash(hash_table *p_table) {
	int size_cpy = p_table->size;
	ht_element **table_copy = p_table->ht;
	p_table->size *= 2;
	p_table->no_elements = 0;
	p_table->ht = (ht_element **)safe_malloc(p_table->size*sizeof(ht_element));
	
	for(int i = 0; i<size_cpy; ++i){
		ht_element * start = table_copy[i];

		while (start != NULL){
			ht_element *next_node = start->next;
			insert_element(p_table, start);
			start = next_node;
		}
	}
	free(table_copy);
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {
	int index = p_table->hash_function(data, p_table->size);
	ht_element* start = p_table->ht[index];

	if(p_table->compare_data(start->data, data) == 0)
		first = 1;

	if(start->next == 0)
		return start;

	while (p_table->compare_data(start->next->data, data) != 0)
		if(start->next == 0)
			return NULL;
		start = start->next;
	
	return start;
}

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
	int index = p_table->hash_function(*data, p_table->size);
	ht_element* start = p_table->ht[index];

	while (start != NULL){
	if (p_table->compare_data(start->data, *data) == 0)
		return start;
	else 
		start = start->next;
	}
	
	return NULL;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
	ht_element *new_element = malloc(sizeof(ht_element));

	new_element->data.char_data = data->char_data;
	new_element->data.int_data = data->int_data;
	new_element->data.ptr_data = data->ptr_data;
	

	new_element->next = NULL;

	int index = p_table->hash_function(*data, p_table->size);

	if( p_table->ht[index] == 0) {
		p_table->ht[index] = new_element;
	} else {
		ht_element* start = p_table->ht[index];
		p_table->ht[index] = new_element;
		p_table->ht[index]->next = start;
	}

	p_table->no_elements++;
	
	if((double)(p_table->no_elements/p_table->size) > MAX_RATE){
		rehash(p_table);
	}

}

// remove element
void remove_element(hash_table *p_table, data_union data) {
	p_table->no_elements--;
	int index = p_table->hash_function(data, p_table->size);

	if(p_table->ht[index] != 0){
		ht_element* start = p_table->ht[index];

		if(p_table->compare_data(start->data, data) == 0){
			p_table->ht[index] = start->next;
			return;
		}

		if(start->next == 0)
			return;

		while (p_table->compare_data(start->next->data, data) != 0){
			start = start->next;

			if(start->next == 0)
				break;
		}

		if(start->next == 0){
			start = NULL;
		} else {
			start->next = start->next->next;
		}		
	}
}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
	int a1 = a.int_data;
	int b1 = b.int_data;
	return (a1 - b1);
}

// read int value and insert to the union
data_union create_data_int() {
	data_union new_element;
	int data;
	scanf("%d", &data);
	new_element.int_data = data;
	return new_element;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
	const char first = a.char_data;
 	const char second = b.char_data;
  	return first - second;
}

// read char value and insert to the union
data_union create_data_char() {
	data_union new_element;
	char data;
	scanf(" %c", &data);
	new_element.char_data = data;
	return new_element;
}

// Word element

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

void dump_word(data_union data) {
		printf("%s %d",((DataWord*)(data.ptr_data))->word,((DataWord*)(data.ptr_data))->counter);
}

void free_word(data_union data) {
	free(data.ptr_data);
}

int cmp_word(data_union a, data_union b) {
	DataWord* a1 = (DataWord*)(a.ptr_data);
	DataWord* b1 = (DataWord*)(b.ptr_data);

	return strcmp(a1->word, b1->word);
}

int hash_word(data_union data, int size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}

// increase the counter
void modify_word(data_union *data) {
	((DataWord*)(data->ptr_data))->counter +=1;
}

// allocate DataWord structure and insert to the union
data_union create_data_word(char *value) {
	data_union elem;
	DataWord *ptr = malloc(sizeof(DataWord));
	ptr->counter = 1;

	void *w = malloc(strlen(value));
	w = strdup(value);
	ptr->word = w;
	elem.ptr_data = ptr;
	return elem;
}

// read text, parse it to words, and insert those words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
	char buffer[BUFFER_SIZE];
	char delimiters[] = " 	(){}[]<>\"\'*.,?!:;-.\r\n\t";
	void *w;

	fgets(buffer, BUFFER_SIZE, stream);
	char *word;
	for (word = strtok(buffer, delimiters); word; word = strtok(NULL, delimiters))
	{
		if (strlen(word) >= 1)
		{
			DataWord *x = malloc(sizeof(DataWord));
			w = strdup(word);

			for(;w; ++w)
				w = tolower(w);

			printf("%s XD", (char*)w);

			data_union data = create_data_word(w);
			ht_element *q = get_element(p_table, &data);
			if(q){
				modify_word(&(q->data));
			}
			else{
				insert_element(p_table, &data);
			}
		}
	}
}

// test primitive type list
void table_test(hash_table *p_table, int n, data_union (*create_data)()) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		data = create_data();
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n, index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: // test integer table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_int,NULL,
					cmp_int, hash_int, NULL);
			table_test(&table, n, create_data_int);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 2: // test char table
			scanf("%d %d",&n, &index);
			init_ht(&table, 4, dump_char,NULL,
			        cmp_char, hash_char, NULL);
			table_test(&table, n, create_data_char);
			printf ("%d\n", table.size);
			dump_list(&table, index);
			free_table(&table);
			break;
		case 3: // read words from text, insert into table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, free_word,
			        cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%d\n", table.size);
			data = create_data_word(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

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

// ---------------------- functions to implement

// initialize table fields
void init_ht(hash_table *p_table, int size, DataFp dump_data, DataFp free_data,
		 CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
	p_table->size = size;
	p_table->compare_data = compare_data;
	p_table->dump_data = dump_data;
	p_table->free_data=free_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;
	p_table->no_elements = 0;
	p_table->ht = calloc(size,sizeof(ht_element));
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, int n) {
	ht_element *ptr = p_table->ht[n];
	while (ptr){
		p_table->dump_data(ptr->data);
		printf(" ");
		ptr = ptr->next;
	}
	
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
	if(free_data){
		free_data(to_delete->data);
	}
	free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
	ht_element *ptr,*erase;
	for(int i = 0;i<p_table->size;i++){
		ptr = p_table->ht[i];
		while (ptr){
			erase = ptr;
			ptr = ptr->next;
			free_element(p_table->free_data,erase);
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

void rehash(hash_table *p_table) {
	hash_table *new_table = safe_malloc(sizeof(hash_table));
	init_ht(new_table, p_table->size*2, p_table->dump_data, p_table->free_data, p_table->compare_data, p_table->hash_function, p_table->modify_data);
	new_table->no_elements = p_table->no_elements;
	ht_element *ptr, *next;
	int  ind;
	for(int i = 0;i<p_table->size;i++){
		ptr = p_table->ht[i];
		while (ptr){
			next = ptr->next;
			ind = new_table->hash_function(ptr->data,new_table->size);
			ptr->next = new_table->ht[ind];
			new_table->ht[ind] = ptr;
			ptr = next;
		}
	}
	free(p_table->ht);
	memcpy(p_table,new_table,sizeof(hash_table));
	free(new_table);

}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data, int *first) {
	int i = p_table->hash_function(data,p_table->size);
	ht_element *ptr = p_table->ht[i];
	if(ptr == NULL || !p_table->compare_data(ptr->data,data)){
		*first = 1;
	}
	else{
		while (ptr->next){
			if (p_table->compare_data(ptr->next->data,data)==0)return ptr;
			ptr = ptr->next;
		}
	}
	return NULL;
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
  if(get_element(p_table,data) != NULL)return;
	int i = p_table->hash_function(*data,p_table->size);
	ht_element *elem = malloc(sizeof(ht_element));
	elem->data = *data;
	elem->next = NULL;
	elem->next = p_table->ht[i];
	p_table->ht[i] = elem;
	p_table->no_elements += 1;
	//printf(": %d : %d\n",p_table->no_elements,p_table->size);
	if (p_table->no_elements>MAX_RATE*p_table->size){
		rehash(p_table);
		/////////////////////////////////////////////////////
		//printf("\n$$$$$$\n%d\n$$$$$\n",p_table->size);
	}

}

// remove element
void remove_element(hash_table *p_table, data_union data) {
	ht_element *ptr, *tmp;
	int first = 0;
	ptr = find_previous(p_table, data, &first);
	if (first){
		int i = p_table->hash_function(data,p_table->size);
		tmp = p_table->ht[i];
		p_table->ht[i] = tmp->next;
		free_element(p_table->free_data,tmp);
	}
	else if (ptr!=NULL){
		tmp = ptr->next;
		ptr->next = tmp->next;
		free_element(p_table->free_data,tmp);
	}
	p_table->no_elements -= 1;

}

// type-specific definitions

// int element

int hash_int(data_union data, int size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
	printf("%d",data.int_data);
}

int cmp_int(data_union a, data_union b) {
	return a.int_data - b.int_data;
}

// read int value and insert to the union
data_union create_data_int() {
	data_union elem;
	scanf("%d",&(elem.int_data));
	return elem;
}

// char element

int hash_char(data_union data, int size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
	printf("%c",data.char_data);
}

int cmp_char(data_union a, data_union b) {
	return strcmp(&(a.char_data),&(b.char_data));
}
// read char value and insert to the union
data_union create_data_char() {
	data_union elem;
	scanf(" %c",&(elem.char_data));
	return elem;
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
  //printf("1");
	free(((DataWord*)(data.ptr_data))->word);
	//printf(" 2");
	free(data.ptr_data);
	//printf(" 3\n");
}

int cmp_word(data_union a, data_union b) {
	return strcmp(((DataWord*)(a.ptr_data))->word,((DataWord*)(b.ptr_data))->word);
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
	char line[BUFFER_SIZE];
	void *w;
	while ((fgets(line,BUFFER_SIZE,stream))!= NULL){
		for(char*ptr=strtok(line,".,?!:;- \n\t\r");ptr;ptr=strtok(NULL,".,?!:;- \n\t\r")){
			if (strlen(ptr)<1)continue;
			w = strdup(ptr);
			char* p = w;
			for(int i = 0; i<strlen((char*)w);++i){
				*p = tolower(*p);
				p+=1;
			}
			data_union data = create_data_word(w);
			ht_element *q = get_element(p_table,&data);
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
			//printf("test 1\n");///////////
			ht_element *e = get_element(&table, &data);
			//printf("test 2\n");//////////////
			if (e) table.dump_data(e->data);
			//printf("test 3\n");////////////
			if (table.free_data) table.free_data(data); 
			//printf("test free");
			free_table(&table);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64

typedef struct Vector {
  void *data;
  size_t element_size;
  size_t size;
  size_t capacity;
} Vector;

typedef struct Person {
  int age;
  char first_name[MAX_STR_LEN];
  char last_name[MAX_STR_LEN];
} Person;

typedef struct {
  void *data;
  size_t len;
} Vector_elem;

// Allocate vector to initial capacity (block_size elements),
// Set element_size, size (to 0), capacity
void init_vector(Vector *vector, size_t block_size, size_t element_size) {
  vector->data = malloc(block_size * element_size);
  vector->element_size = element_size;
  vector->size = 0;
  vector->capacity = block_size;
}

// If new_capacity is greater than the current capacity,
// new storage is allocated, otherwise the function does nothing.
void reserve(Vector *vector, size_t new_capacity) {
  if(vector->capacity < new_capacity){
		vector->data = realloc(vector->data, new_capacity*vector->element_size);
		vector->capacity = new_capacity;
	}
}

size_t get_addres(Vector *vector, size_t i){
    return (size_t)(vector->data + i * vector->element_size);
}

// Resizes the vector to contain new_size elements.
// If the current size is greater than new_size, the container is
// reduced to its first new_size elements.

// If the current size is less than new_size,
// additional zero-initialized elements are appended
void resize(Vector *vector, size_t new_size) {
    reserve(vector, new_size);
    if (vector->size < new_size) {
        memset((void *)get_addres(vector, vector->size), 0, get_addres(vector, new_size) - get_addres(vector, vector->size));
    }
    vector->size = new_size;
}

// Add element to the end of the vector
void push_back(Vector *vector, void *value) {
    char *c = (char *)get_addres(vector, vector->size);

	if(vector->size == vector->capacity){
		reserve(vector, vector->capacity*2);
		c = (char *)vector->data + vector->element_size*vector->size;
	} 
	
	memcpy(c, value, vector->element_size);
	vector->size++;
}

// Remove all elements from the vector
void clear(Vector *vector) {
    vector->size = 0;
}

// Remove the last element from the vector
void pop_back(Vector *vector) {
    if(vector->size > 0)
        vector->size--;
}

// Insert new element at index (0 <= index <= size) position
void insert(Vector *vector, int index, void *value) {
  if(vector->size+1 > vector->capacity)
		reserve(vector, vector->capacity*2);
	
	char* dest = vector->data + vector->element_size*index;
	memmove(dest+vector->element_size, dest, (vector->size-index)*vector->element_size);
	memcpy(dest, value, vector->element_size);
	vector->size+=1;
}

// Erase element at position index
void erase(Vector *vector, int index) {
    if (index >= vector->size)
        return;
    void *del_elem = (void*)get_addres(vector, index);
    memmove(del_elem, del_elem + vector->element_size,
        (vector->size - 1 - index) * vector->element_size);

  vector->size--;
}
// Erase all elements that compare equal to value from the container
void erase_value(Vector *vector, void *value, int (*cmp)(const void *, const void *)) {
	for (int i = vector->size - 1; i >= 0; i--) {
    if (cmp((void *)get_addres(vector, (size_t)i), value)==0) {
        erase(vector, i);
    }
  }
}

// Erase all elements that satisfy the predicate from the vector
void erase_if(Vector *vector, int (*predicate)(void *)) {
    for (int i = vector->size - 1; i >= 0; i--) {
	  	char * first_element = (char *)get_addres(vector, (size_t)i);
    	if (predicate(&first_element)) {
        	erase(vector, i);
    	}
  	}
}

// Request the removal of unused capacity
void shrink_to_fit(Vector *vector){
	vector->capacity = vector->size;
	vector->data = realloc(vector->data, vector->capacity * vector->element_size);
};

// Print integer vector
void print_vector_int(Vector *vector){
	printf("%d\n", (int)vector->size);
	int *c = (int *)vector->data;
	for(int i = 0; i < vector->size; ++i){
		printf("%d ", *c);
		c += 1;
	}
	printf("\n");
};

// Print char vector
void print_vector_char(Vector *vector){
	printf("%d\n", (int)vector->capacity);
	char *c = (char *)vector->data;
	for(int i = 0; i < vector->size; ++i){
		printf("%c ", *c);
		c += 1;
	}
	printf("\n");
};

// Print vector of Person
void print_vector_person(Vector *vector){
	printf("%d\n", (int)vector->capacity);
	Person *c = (Person *)vector->data;
	for(int i = 0; i < vector->size; ++i){
		printf("%d %s %s\n", c->age, c->first_name, c->last_name);
		c++;
	}
	printf("\n");
}

// integer comparator - increasing order
int int_cmp(const void *v1, const void *v2){
	const int *first = (int *)v1;
  	const int *second = (int *)v2;
  	return *first - *second;
};

// char comparator - lexicographical order (case sensitive)
int char_cmp(const void *v1, const void *v2){
	const char *first = (char *)v1;
 	const char *second = (char *)v2;
  	return *first - *second;
};

// Person comparator:
// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int person_cmp(const void *p1, const void *p2){
	const Person *first = (const Person *)p1;
    const Person *second = (const Person *)p2;
    if (first->age == second->age) {
    int first_res = strcmp(first->first_name, second->first_name);
    if (first_res == 0)
        return strcmp(first->last_name, second->last_name);
    return first_res;
  }
  return second->age - first->age;
};

// predicate: check if number is even
int is_even(void *value){
	return (*(int*)value+1)%2;
};

// predicate: check if char is a vowel
int is_vowel(void *value){
	char vowels[] = {"yaeiouAEIOUY"};
	char *p = strchr(vowels, *(char *)value);
	if (p == NULL)
		return 0;
  	return 1;
}

// predicate: check if person is older than 25
int is_older_than_25(void *person){
	Person* p = (Person*) person;
	return p->age > 25;
};

// -------------------------------------------------------------
void read_int(void* value) {
	scanf("%d", (int*)value);
}

void read_char(void* value) {
	char c[2];
	scanf("%s", c);
	*(char*)value = c[0];
};

void read_person(void* value) {
	Person *person = (Person*)value;
	scanf("%d %s %s", &person->age, person->first_name, person->last_name);
};

void vector_test(Vector *vector, int n, void(*read)(void*),
		 int (*cmp)(const void*, const void*), int(*predicate)(void*)) {
	char op[2];
	int index;
	size_t size;
	void *v = malloc(vector->element_size);
	for (int i = 0; i < n; ++i) {
		scanf("%s", op);
		switch (op[0]) {
			case 'p': // push_back
				read(v);
				push_back(vector, v);
				break;
			case 'i': // insert
				scanf("%d", &index);
				read(v);
				insert(vector, index, v);
				break;
			case 'e': // erase
				scanf("%d", &index);
				read(v);
				erase(vector, index);
				erase_value(vector, v, cmp);
				break;
			case 'd': // erase (predicate)
				erase_if(vector, predicate);
				break;
			case 'r': // resize
				scanf("%zu", &size);
				resize(vector, size);
				break;
			case 'c': // clear
				clear(vector);
				break;
			case 'f': // shrink
				shrink_to_fit(vector);
				break;
			case 's': // sort
				qsort(vector->data, vector->size,
				      vector->element_size, cmp);
				break;
			default:
				printf("No such operation: %s\n", op);
				break;
		}
	}
	free(v);
}

int main(void) {
	int to_do, n;
	Vector vector_int, vector_char, vector_person;

	scanf("%d%d", &to_do, &n);

	switch (to_do) {
		case 1:
			init_vector(&vector_int, 4, sizeof(int));
			vector_test(&vector_int, n, read_int, int_cmp, is_even);
			print_vector_int(&vector_int);
			free(vector_int.data);
			break;
		case 2:
			init_vector(&vector_char, 2, sizeof(char));
			vector_test(&vector_char, n, read_char, char_cmp, is_vowel);
			print_vector_char(&vector_char);
			free(vector_char.data);
			break;
		case 3:
			init_vector(&vector_person, 2, sizeof(Person));
			vector_test(&vector_person, n, read_person, person_cmp, is_older_than_25);
			print_vector_person(&vector_person);
			free(vector_person.data);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}

	return 0;
}


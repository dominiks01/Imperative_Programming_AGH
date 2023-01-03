#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MEMORY_ALLOCATION_ERROR -1
#define LIST_ERROR -2
#define PROGRAM_ERROR -3

struct tagList;
typedef void (*ConstDataFp)(const void *);
typedef void (*DataFp)(void *);
typedef int (*CompareDataFp)(const void *, const void *);
typedef void (*InsertInOrder)(struct tagList *, void *);

typedef struct tagListElement
{
	struct tagListElement *next;
	void *data;
} ListElement;

typedef struct tagList
{
	ListElement *head;
	ListElement *tail;
	ConstDataFp dump_data;
	DataFp free_data;
	CompareDataFp compare_data;
	InsertInOrder insert_sorted;
} List;

// -----------------------------------------------------------------
// generic functions - they are common for all instances of the list
// (independent of the data type)
// -----------------------------------------------------------------

void init_list(List *p_list, ConstDataFp dump_data, DataFp free_data,
			   CompareDataFp compare_data, InsertInOrder insert_sorted)
{
	p_list->compare_data = compare_data;
	p_list->free_data = free_data;
	p_list->insert_sorted = insert_sorted;
	p_list->dump_data = dump_data;
	p_list->head = NULL;
	p_list->tail = NULL;
}

// Print all elements of the list
void dump_list(const List *p_list)
{
	ListElement *start;
	for (start = p_list->head; start != 0; start = start->next)
		p_list->dump_data(start->data);
	printf("\n");
}

// Print elements of the list if comparable to data
void dump_list_if(List *p_list, void *data)
{
	ListElement *start = p_list->head;
	while (start != NULL)
	{
		if (p_list->compare_data(data, start->data)==0){
			p_list->dump_data(start->data);
		}
		start = start->next;
	}
}

// Free all elements of the list
void free_list(List *p_list)
{
	ListElement *start = 0;
	start = p_list->head;
	while (start != 0)
	{
		ListElement *toDelete = start;
		start = start->next;
		free(toDelete);
	}
}

// Push element at the beginning of the list
void push_front(List *p_list, void *data)
{
	ListElement *element = malloc(sizeof(ListElement));
	if (element == NULL)
		exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	element->next = p_list->head;

	if (p_list->head != 0)
		p_list->head = element;
	else
		p_list->head = p_list->tail = element;

	if (!p_list->tail)
		p_list->tail = p_list->head;
}

// Push element at the end of the list
void push_back(List *p_list, void *data)
{
	ListElement *element = malloc(sizeof(ListElement));
	if (element == NULL)
		exit(MEMORY_ALLOCATION_ERROR);
	element->data = data;
	element->next = NULL;
	if (p_list->tail)
		p_list->tail->next = element;
	p_list->tail = element;
	if (!p_list->head)
		p_list->head = p_list->tail;
}

// Remove the first element
void pop_front(List *p_list)
{
	ListElement *toDelete;
	if (p_list->head == 0)
		return;
	toDelete = p_list->head;
	p_list->head = p_list->head->next;
	if (p_list->head == 0)
		p_list->tail = 0;
	free(toDelete);
}

// Reverse the list
void reverse(List *p_list)
{
	if (p_list->head == NULL)
		return;
	
	if (p_list->head->next == NULL)
		return;

	ListElement *p, *q, *act;
	p = NULL;
	q = p_list->head;
	act = q->next;

	ListElement *start = p_list->head;

	while (q)
	{
		q->next = p;
		p = q;
		q = act;
		if (act)
			act = act->next;
	}

	p_list->head = p;
	p_list->tail = start;
}

// insert element preserving the ordering (defined by insert_sorted function)
void insert_in_order(List *p_list, void *data)
{
	p_list->insert_sorted(p_list, data);
}

// find element in sorted list after which to insert given element
ListElement *find_insertion_point(const List *p_list, ListElement *p_element)
{
	ListElement *insertionPoint = 0;
	ListElement *current = 0;
	for (current = p_list->head; current != 0; current = current->next)
	{
		if (p_list->compare_data(current->data, p_element->data) <= 0)
			insertionPoint = current;
	}
	return insertionPoint;
}

// Insert element after 'previous'
void push_after(List *p_list, void *data, ListElement *previous)
{
	ListElement *element = (ListElement *)malloc(sizeof(ListElement));
	element->next = 0;
	element->data = data;

	element->next = previous->next;
	previous->next = element;
}

// Insert element preserving order (no counter)
void insert_elem(List *p_list, void *p_data)
{
	ListElement *element = (ListElement *)malloc(sizeof(ListElement));
	element->next = 0;
	element->data = p_data;

	ListElement *current = 0;
	for (current = p_list->head; current != 0; current = current->next)
	{
		if (p_list->compare_data(element->data, current->data) == 0)
		{
			return;
		}
	}

	ListElement *insertionPt = find_insertion_point(p_list, element);

	if (insertionPt == 0)
	{
		element->next = p_list->head;
		p_list->head = element;
		if (p_list->tail == 0)
			p_list->tail = p_list->head;
	}
	else
		push_after(p_list, p_data, insertionPt);
}

// ---------------------------------------------------------------
// type-specific definitions
// ---------------------------------------------------------------

// int element

typedef struct DataInt
{
	int id;
} DataInt;

void dump_int(const void *d)
{
	DataInt *x = (DataInt *)d;
	printf("%d ", x->id);
}

void free_int(void *d)
{
	free(d);
}

int cmp_int(const void *a, const void *b)
{
	const int *a1 = (int *)a;
	const int *b1 = (int *)b;
	return (*a1 - *b1);
}

DataInt *create_data_int(int v)
{
	DataInt *new_int = malloc(sizeof(int));
	new_int->id = v;
	return (void *)new_int;
}

// Word element

typedef struct DataWord
{
	char *word;
	int counter;
} DataWord;

void dump_word(const void *d)
{
	DataWord *new_word = (DataWord *)d;
	printf("%s\n", new_word->word);
}

void dump_word_lowercase(const void *d)
{
	DataWord *new_word = (DataWord *)d;
	printf("%s\n", new_word->word);
}

void free_word(void *d)
{
	free(d);
}

// conpare words case insensitive
int cmp_word_alphabet(const void *a, const void *b)
{
	DataWord *new_word1 = (DataWord *)a;
	DataWord *new_word2 = (DataWord *)b;

	char *s1 = new_word1->word;
	char *s2 = new_word2->word;

	for (int i = 0; i < strlen(s1); ++i)
		s1[i] = tolower(s1[i]);

	for (int i = 0; i < strlen(s2); ++i)
		s2[i] = tolower(s2[i]);

	return strcmp(s1, s2);
}

int cmp_word_counter(const void *a, const void *b)
{
	DataWord *new_word1 = (DataWord *)a;
	DataWord *new_word2 = (DataWord *)b;

	return new_word1->counter - new_word2->counter;
}

// insert element; if present increase counter
void insert_elem_counter(List *p_list, void *data)
{
	ListElement *element = (ListElement *)malloc(sizeof(ListElement));
	element->next = 0;
	element->data = data;

	ListElement *current = 0;
	for (current = p_list->head; current != 0; current = current->next)
	{
		if (p_list->compare_data(element->data, current->data) == 0)
		{
			DataWord *d = (DataWord *)current->data;
			d->counter += 1;
			current->data = d;
			p_list->free_data(element);
			return;
		}
	}

	ListElement *insertionPt = find_insertion_point(p_list, element);

	if (insertionPt == 0)
	{
		element->next = p_list->head;
		p_list->head = element;
		if (p_list->tail == 0)
			p_list->tail = p_list->head;
	}
	else
		push_after(p_list, data, insertionPt);
}

// read text, parse it to words, and insert those words to the list
// in order given by the last parameter (0 - read order,
// 1 - alphabetical order)
void stream_to_list(List *p_list, FILE *stream, int order)
{
	char buffer[BUFFER_SIZE];
	char delimiters[] = " 	(){}[]<>\"\'*.,?!:;-.\r\n\t";
	void *w;

	while (fgets(buffer, BUFFER_SIZE, stream))
	{
		char *word;
		for (word = strtok(buffer, delimiters); word; word = strtok(NULL, delimiters))
		{
			if (strlen(word) >= 1)
			{
				DataWord *x = malloc(sizeof(DataWord));
				w = strdup(word);
				x->word = w;
				x->counter = 1;

				if (order == 1)
					p_list->insert_sorted(p_list, x);
				else
					push_back(p_list, x);
			}
		}
	}
}

// test integer list
void list_test(List *p_list, int n)
{
	char op[2];
	int v;
	for (int i = 0; i < n; ++i)
	{
		scanf("%s", op);
		switch (op[0])
		{
		case 'f':
			scanf("%d", &v);
			push_front(p_list, create_data_int(v));
			break;
		case 'b':
			scanf("%d", &v);
			push_back(p_list, create_data_int(v));
			break;
		case 'd':
			pop_front(p_list);
			break;
		case 'r':
			reverse(p_list);
			break;
		case 'i':
			scanf("%d", &v);
			insert_in_order(p_list, create_data_int(v));
			break;
		default:
			printf("No such operation: %s\n", op);
			break;
		}
	}
}

int main(void)
{
	int to_do, n;
	List list;

	scanf("%d", &to_do);
	switch (to_do)
	{
	case 1: // test integer list
		scanf("%d", &n);
		init_list(&list, dump_int, free_int,
				  cmp_int, insert_elem);
		list_test(&list, n);
		dump_list(&list);
		free_list(&list);
		break;
	case 2: // read words from text, insert into list, and print
		init_list(&list, dump_word, free_word,
				  cmp_word_alphabet, insert_elem_counter);
		stream_to_list(&list, stdin, 0);
		dump_list(&list);
		free_list(&list);
		break;
	case 3: // read words, insert into list alphabetically, print words encountered n times
		scanf("%d", &n);
		init_list(&list, dump_word_lowercase, free_word,
				  cmp_word_alphabet, insert_elem_counter);
		stream_to_list(&list, stdin, 1);
		list.compare_data = cmp_word_counter;
		DataWord data = {NULL, n};
		list.dump_data = dump_word_lowercase;
		dump_list_if(&list, &data);
		printf("\n");
		free_list(&list);
		break;
	default:
		printf("NOTHING TO DO FOR %d\n", to_do);
		break;
	}
	return 0;
}

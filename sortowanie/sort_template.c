// Dominik Szot

// Zadanie rozwiązane trochę innym sposobem
// 2 dni męczyłem się z błędem występującym przy zapisywaniu nowych lini w zadaniu 2
// Z braku czasu poszedłem na skróty i rozwiązałem je podobnie do pierwszego 
// :(  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Convert line into person
Person strtop(const void *p1){
	char *str = (char*)p1;
	Person p;

	sscanf(str,"%d %s %s", &p.age, p.first_name, p.last_name);
	return p;
}

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *pp1, const void *pp2){

	Person *p1 = (Person*)pp1;
	Person *p2 = (Person*)pp2;

	if((p1->age) == (p2->age)){
		if((p1->first_name) == (p2->first_name)){
			return strcmp(p1->last_name, p2->last_name);
		} else {
			return strcmp(p1->first_name, p2->first_name);
		}
	}
	
	return (p1->age) < (p2->age);
}

// Read data to Person array (till EOF)
int read_person_array(Person *persons){
	int person_counter = 0;
	char *str = (char *)malloc(MAX_STR_LEN);

	while (fgets(str, MAX_STR_LEN, stdin) != NULL){
		if(strlen(str) == 1)
			break;
		Person p = strtop(str);
      	persons[person_counter++] = p;
	};
	free(str);
	return person_counter;
}

// Print Person array
void print_person_array(Person *persons, int n){	
	for(int i = 0; i < n; ++i)
		printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
	
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (int that order)
int cmp_lines(const void *l1, const void *l2){

	Person *p1 = (Person*)l1;
	Person *p2 = (Person*)l2;

	if(p1->first_name[strlen(p1->first_name) - 1] == 'a' && p2->first_name[strlen(p2->first_name) - 1] != 'a')
		return 0;

	if(p1->first_name[strlen(p1->first_name) - 1] != 'a' && p2->first_name[strlen(p2->first_name) - 1] == 'a')
		return 1;

	if(p1->first_name[strlen(p1->first_name) - 1] != 'a'){
		return strcmp(p1->last_name, p2->last_name);
	}

	if(p1->first_name[strlen(p1->first_name) - 1] == 'a')
		return (p1->age) > (p2->age);
		
	return 0;
}

// Read lines with students' data *(as text)
// *(as text and convert it into persons array)
int read_lines(Person *persons){
	int person_counter = 0;
	char *str = (char *)malloc(MAX_STR_LEN);

	while (fgets(str, MAX_STR_LEN, stdin) != NULL){
		if(strlen(str) == 1)
			break;
		Person p = strtop(str);
      	persons[person_counter++] = p;
	};
	free(str);
	return person_counter;
}

// Print sorted lines
void print_lines(Person *persons, int n){
	for(int i = 0; i < n; ++i)
		printf("%d %s %s\n", persons[i].age, persons[i].first_name, persons[i].last_name);
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_lines);
			print_lines(persons, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}


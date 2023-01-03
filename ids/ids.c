#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

void print_tab( char arr[][MAX_ID_LEN]){
	for(int i = 0; i < MAX_IDS; ++i)
		if(arr[i][0] != '\0')
			printf("%s\n", arr[i]);
}


int find_idents(){
	char ch;
	char word[MAX_ID_LEN];
	char tab_2[MAX_IDS][MAX_ID_LEN];
	int index_word = 0;
	word[0] = '\0';

	while ((ch = getchar()) != EOF){
		switch (ch){
		case '/':{
			ch = getchar();
			switch (ch){
				case '*':{
					while ((ch=getchar()) != EOF && ch != '*'){
						if(ch=='\\')
							ch = getchar();
						ch = getchar();
						if(ch=='/')
							break;
						ungetc(ch, stdin);
					}
					break;
				}
				case '/':{
					while ((ch=getchar()) != EOF && ch != '\n'){
						if(ch=='\\')
							ch = getchar();
					};
					break;	
				}
				default:
					break;
				}
			break;
		}
		case '"':{
			while ((ch=getchar()) != EOF && ch != '"');
			break;
		}
		case '\'':{
			while ((ch=getchar()) != EOF && ch !='\'');
			break;
		}
		case '#':{
			while ((ch=getchar()) != EOF && ch !=' ');
			word[0] = '\0';
			break;
		}
		case '<':{
			while ((ch=getchar()) != EOF && ch != ' ');
			word[0] = '\0';
			break;
		}
		default:{
			if(!(isalnum(ch) || ch=='_' || isdigit(ch))){
				strcpy(tab[index_word++], word);
				word[0] = '\0';
			}
			else strncat(word, &ch, 1);
			}
			break;
		}
	}

	int j = 0;
	int result = 0;

	for(int i = 0; i<MAX_IDS; ++i){
		if(!strcmp(tab[i], "char"))
			strcpy(tab_2[j++], tab[i+1]);
	}

	result = j;

	for(int i = 0; i<MAX_IDS; ++i)
		for(int o = 0; o <=j; ++o)
			if(!strcmp(tab[i], tab_2[o]))
				tab[i][0] = '\0';

	for(int i = 0; i<MAX_IDS; ++i){
		for(int j = 0; j <32;++j){
			if(!strcmp(tab[i], keywords[j]))
				tab[i][0] = '\0';
		}
	}

	for(int i = 0; i<MAX_IDS-1; ++i){
		for(int j = i+1; j<MAX_IDS;++j){
			if(!strcmp(tab[i], tab[j]))
				tab[j][0] = '\0';
		}
	}

	//print_tab(tab);

	for(int i = 0; i < MAX_IDS; ++i)
		if(tab[i][0] != '\0' && !isdigit(tab[i][0]))
			result += 1;

	return result;
}


int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}


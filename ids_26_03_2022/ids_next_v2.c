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

char *remove_spaces(char *str){
	int i =0, j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
          str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return str;
}


int find_idents(){
	char ch;
	char word[100];
	int word_index = 0;
	word[0] = '\0';

	while ((ch = getchar()) != EOF){
		switch (ch){
		case '/':{
			char next_char = getchar();

			switch (next_char){
			    case '/':{
				    while((ch = getchar()) != EOF && ch != '\n');
					    //if(ch == '\n') break;
				    break;
			    }
			    case '*':{
				    while((ch = getchar()) != EOF && ch != '*'){
					   // if(ch == '*'){
						    //next_char = getchar();
						//if(next_char == '/')
						//	break;
					    //}
						ch = getchar();
						if(ch == '/')
							break;
						ungetc(ch, stdin);
						break;
				    }
				break;
			    }
			    default:{
				    if(isalpha(next_char) || isalnum(next_char) || ch =='_')
					    strncat(word, &next_char, 1);
				    break;
			    }
		    }break;
		}
		case '"':{
			while ((ch = getchar()) != EOF){ 
				if(ch == '"')
					break;
			}
			break;
		}	
		case '\'':{
			while ((ch = getchar()) != EOF){
				if(ch == '\\')
                    ch = getchar();   
                if(ch == EOF)
                    break;
				if(ch == '\'')
					break;
            }
			break;
		}
		case ' ':{
			strcpy(tab[word_index++], word);
			word[0] = '\0';
			break;
		}
		case '	':{
			strcpy(tab[word_index++], word);
			word[0] = '\0';
			break;
		}
		case '\n':{
			strcpy(tab[word_index++], word);
			word[0] = '\0';
			break;
		}
		case 92:{
			ch = getchar();
			break;
		}
		case '<':{
			while ((ch = getchar()) != EOF)
				if(ch == ' ' || ch == '	' || ch == '\n'){
					ungetc(ch, stdin);
					break;
				}
			break;
		} 
		case '#':{
			while ((ch = getchar()) != EOF)
				if(ch == ' ')
					break;
			break;
		}
		default:{
			if(isalpha(ch) || isalnum(ch) || ch =='_'){
				strncat(word, &ch, 1);
			} else {
				strcpy(tab[word_index++], word);
				word[0] = '\0';
			}
			break;
		}
	}
	}
	
    char s_z[MAX_IDS][MAX_ID_LEN];
    int s_z_index = 0;
    int result = 0;

	for(int i = 0; i < MAX_IDS; ++i){
		if(isdigit(tab[i][0])){
			tab[i][0] = '\0';
		}else{
			int flag = 0;
            
            if(!strcmp(tab[i], "char")){
                int jest = 1;
                for(int xd = 0; xd < 1024; ++xd)
                    if(strcmp(s_z[xd], tab[i+1]))  
                        jest = 0;

                if(jest){
				    strcpy(s_z[s_z_index], tab[i+1]);
                    tab[i+1][0] = '\0';
                    result += 1;
                }
            }

			for(int j = 0; j < 32; j++)
				if(!strcmp(keywords[j], tab[i]))
					flag = 1;
                
			if(flag)
				tab[i][0] = '\0';
			}
		}
	
	for(int i = 0; i < MAX_IDS-1; ++i){
		for(int j = i+1; j < MAX_IDS; ++j)
			if(!strcmp(tab[i], tab[j]))
				tab[j][0] = '\0';
	}

    for(int i = 0; i < MAX_IDS-1; ++i){
		for(int j = i+1; j < MAX_IDS; ++j)
			if(!strcmp(tab[i], s_z[j]))
				tab[i][0] = '\0';
	}
    
	for(int i = 0; i < MAX_IDS; ++i){
		if(tab[i][0] != '\0'){
			//printf("%s\n", tab[i]);
			result += 1;
		}
	}
		
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

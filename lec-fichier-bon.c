#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _TsudokuData {
    char* type;  // le type/taille du sudoku
	char* solu;  // le sudoku solutionné
	char* init;  // le sudoku initial
} TsudokuData;

int main (int argc, char* argv[]) {
	char *tableau[1000];
	const char chaine;
	int i = 0;
	int j;
	TsudokuData sudoku_t[] = {{}};
 
	FILE *fichier = fopen("sudoku-db.txt","r");
	if (fichier != NULL){
while (fscanf(fichier, "%s", &chaine)==1)
printf(" %s\n" ,&chaine);
 		
}
	tableau[i] = strdup(&chaine);
			i=*++tableau[i];
printf(" pointeur %d\n" ,i);

fclose(fichier);
/*int k = 0;
	int l = 0;
	char test[100] ; test[0] = 0;
for(i=0; i<tableau[i]; i++){
	for (j = 0; j < i; j++)
	{
		if (strcmp(tableau[j], "4x4") == 0) {
			sudoku_t[l].type = "4x4";
			k = 4;
			l++;
		}else if(strcmp(tableau[j], "6x6") == 0) {
			sudoku_t[l].type = "6x6";
			k = 6;
			l++;
		}else if(strcmp(tableau[j], "8x8") == 0) {
			sudoku_t[l].type = "8x8";
			k = 8;
			l++;
		}else if(strcmp(tableau[j], "9x9") == 0) {
			sudoku_t[l].type = "9x9";
			k = 9;
			l++;
		}else{
			strcat(test, tableau[j]);
			k--;
			if (k==0) {
				sudoku_t[l-1].solu = strdup(test);
				test[0] = 0;
}
}
}
		
		//
	}
}
printf("%s", tableau[i]);	
fclose (fichier);
}/*
	int k = 0;
	int l = 0;
	char test[100] ; test[0] = 0;
	for (j = 0; j < i; j++)
	{
		if (strcmp(tableau[j], "4x4") == 0) {
			sudoku_t[l].type = "4x4";
			k = 4;
			l++;
		}else if(strcmp(tableau[j], "6x6") == 0) {
			sudoku_t[l].type = "6x6";
			k = 6;
			l++;
		}else if(strcmp(tableau[j], "8x8") == 0) {
			sudoku_t[l].type = "8x8";
			k = 8;
			l++;
		}else if(strcmp(tableau[j], "9x9") == 0) {
			sudoku_t[l].type = "9x9";
			k = 9;
			l++;
		}else{
			strcat(test, tableau[j]);
			k--;
			if (k==0) {
				sudoku_t[l-1].solu = strdup(test);
				test[0] = 0;
			}
		}
	

	// Juste pour tester, à supprimer
	printf("0 type : %s\n", sudoku_t[0].type);
	printf("0 solu : %s\n", sudoku_t[0].solu);
	printf("1 type : %s\n", sudoku_t[1].type);
	printf("1 solu : %s\n", sudoku_t[1].solu);
	printf("2 type : %s\n", sudoku_t[2].type);
	printf("2 solu : %s\n", sudoku_t[2].solu);
	printf("3 type : %s\n", sudoku_t[3].type);
	printf("3 solu : %s\n", sudoku_t[3].solu);
	printf("4 type : %s\n", sudoku_t[4].type);
	printf("4 solu : %s\n", sudoku_t[4].solu);
	printf("5 type : %s\n", sudoku_t[5].type);
	printf("5 solu : %s\n", sudoku_t[5].solu);
	printf("6 type : %s\n", sudoku_t[6].type);
	printf("6 solu : %s\n", sudoku_t[6].solu);
	printf("7 type : %s\n", sudoku_t[7].type);
	printf("7 solu : %s\n", sudoku_t[7].solu);
	printf("8 type : %s\n", sudoku_t[8].type);
	printf("8 solu : %s\n", sudoku_t[8].solu);
*/	return 0;
}

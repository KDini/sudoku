#include <stdio.h>
#include <stdlib.h>
#include "pile.h"

Pile *initialiser()
{
    Pile *pile = malloc(sizeof(*pile));
    pile->premier = NULL;
}

void empiler(Pile *pile, char x,int y,  char c)
{
    Element *nouveau = malloc(sizeof(*nouveau)*2);

    if (pile == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE);
    }

    nouveau->nombre = x,y,c;
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;

}

char depiler(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }

   char nombreDepile ;
    Element *elementDepile = pile->premier;


    if (pile != NULL && pile->premier!= NULL)
    {
        nombreDepile = elementDepile->nombre;
        pile->premier== elementDepile->suivant;
        free(elementDepile);
    }

    return nombreDepile;
}

void afficher(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actuel = pile->premier;

    while (actuel != NULL)
    {
        printf(" PILE: %c  %c\n", actuel->nombre);
        actuel = actuel->suivant;
    }

    printf("\n");
}
/*

int main()
{
    Pile *maPile = initialiser();

 /*  empiler(maPile, 'A');
    empiler(maPile, 'B');
    empiler(maPile, 'C');
    empiler(maPile, 'D');
    empiler(maPile,'E');
    empiler(maPile, 'F');
    printf("\nEtat de la pile :\n");
    afficherPile(maPile);

    printf("Je depile %c\n", depiler(maPile));
    printf("Je depile %c\n", depiler(maPile));


 char valeur;
printf(" affciher historique");
 scanf("%c",&valeur);

empiler(maPile, valeur);
    printf("\nEtat de la pile :\n");
    afficherPile(maPile);
//printf("vous avez tapez :  MA PILE %c\n  ",nvNombre);
  printf("Je depile %c\n", depiler(maPile));
printf("\nEtat de la pile :\n");
    afficherPile(maPile);

    return 0;
}
*/

#ifndef H_PILE
#define H_PILE

#define SUD_MAX_XY 20
#define SUD_VIDE   -1



typedef struct Element Element;
struct Element
{
   char nombre;
    Element *suivant;
};

typedef struct Pile Pile;
struct Pile
{
    Element *premier;
};

Pile *initialiser();
void empiler(Pile *pile, char x,int y, char c);
char depiler(Pile *pile);
void afficher(Pile *pile);

#endif

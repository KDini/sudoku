/**
 * Jeu du sudoku en mode console
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "pile.c"
/***********************************************************************
 * Module Sudoku
**/
typedef enum _TsudType {
  SUD_4x4, SUD_6x6, SUD_8x8, SUD_9x9, SUD_TypeNb
} TsudType;

typedef struct _TsudCase {
    int sud_init; // 1:case initiale  0:case non initiale
    int sud_val;  // la valeur courante de la case
                  // SUD_VIDE si non initialisée
    int sud_sol;  // la valeur solution
                  // SUD_VIDE si non initialisée
} TsudCase;

typedef struct _Tsud {
    int sud_dx,  sud_dy;  // taille de la grille
    int sud_rdx, sud_rdy; // taille des regions
    int sud_ncv;          // nombre de cases vides

    // les sud_dx*sud_dy cases de la grille de sudoku
    TsudCase sud_cases[SUD_MAX_XY][SUD_MAX_XY];
} Tsud;

// structure d'échange
typedef struct _TsudCoup {
  int x,y;   // coordonnées du coup sur la grille
  int l;     // valeur de la case
} TsudCoup;



#define SUD_MAX_XY 20
#define SUD_VIDE   -1



// prédéclaration
void sud_coupJoue(Tsud*,const TsudCoup*);
void sud_coupJoueIII(Tsud*,int,int,int);

/**
 * sud_init initialise le damier sud à la taille type.
 * Toutes les cases sont mises à VIDE.
**/
void sud_init(Tsud*sud, TsudType type)
{
  memset(sud,0,sizeof(*sud));
  switch ( type ) {
     case SUD_4x4:
       sud->sud_dx=4;  sud->sud_dy=4;
       sud->sud_rdx=2; sud->sud_rdy=2;
       break;
     case SUD_6x6:
       sud->sud_dx=6;  sud->sud_dy=6;
       sud->sud_rdx=3; sud->sud_rdy=2;
       break;
     case SUD_8x8:
       sud->sud_dx=8;  sud->sud_dy=8;
       sud->sud_rdx=4; sud->sud_rdy=2;
       break;
     case SUD_9x9:
       sud->sud_dx=9;  sud->sud_dy=9;
       sud->sud_rdx=3; sud->sud_rdy=3;
       break;
  }
  sud->sud_ncv = sud->sud_dx * sud->sud_dy;
  int x,y;
  for ( x=0; x<sud->sud_dx ; x+=1) {
    for ( y=0; y<sud->sud_dy ; y+=1) {
      // sud->sud_cases[x][y].sud_init=0; fiat dans le memset plus haut
      sud->sud_cases[x][y].sud_val=SUD_VIDE;
      sud->sud_cases[x][y].sud_sol=SUD_VIDE;
    }
  }
}

/**
 * sud_initCase initialise la case (x,y) du damier sud à la
 * valeur v.
 * si ic==0, la case est marquée VIDE, sud_caseValeur(sud,x,y)
 * renvoie SUD_VIDE.
 * si ic!=0, la case est non marquée VIDE. sud_caseValeur(sud,x,y)
 * renvoie v.
 * Dans tous les cas sud_caseSolut(sud,x,y) renvoie v.
 * Parametre:
 *   Tsud sud: la grille
 *   int  x,y: les coordonnées de la case
 *   int  v:   la valeur de la case, doit être > 0.
 *   int  ic:  1:case initiale, 0:case non initiale
**/
void sud_initCase(Tsud*sud, int x, int y, int v, int ic)
{
  sud->sud_cases[x][y].sud_sol = v;
  if ( ic ) {
    sud->sud_cases[x][y].sud_init = 1;
    sud_coupJoueIII(sud,x,y,v);
  }
}

/**
 * Retour:
 *   sud_nbCases:       le nombre de cases du jeux.
 *   sud_nbCasesVides:  le nombre de cases vides (non encore jouées).
 *   sud_grilleRemplie: 1 si toutes les cases du jeux sud sont remplies sinon 0.
**/
int sud_nbCases       (const Tsud*sud) { return sud->sud_dx*sud->sud_dy; }
int sud_nbCasesVides  (const Tsud*sud) { return sud->sud_ncv; }
int sud_grilleRemplie (const Tsud*sud) { return sud_nbCasesVides(sud)==0; }

/**
 * sud_coupJoue joue la valeur c->l sur la case (c->x,c->y) de la grille sud.
 * sud_coupJoueIII joue la valeur l sur la case (x,y) de la grille sud.
 * Pré-condition:
 *    La case jouée ne doit pas être marquée "init"
 *    (sud->sud_cases[x][y].sud_init!=0)
**/
void sud_coupJoue(Tsud* sud, const TsudCoup* cp)
{
  int x=cp->x, y=cp->y, l=cp->l;
  TsudCase* c = &sud->sud_cases[x][y];
  if ( l==SUD_VIDE && c->sud_val==SUD_VIDE ) {
    // on dejoue un coup non joué --> rien à faire
    return;
  } else if ( l==SUD_VIDE && c->sud_val!=SUD_VIDE ) {
    // on dejoue un coup
    c->sud_val  = l;
    sud->sud_ncv += 1;
    return;
  } else if ( l!=SUD_VIDE && c->sud_val!=SUD_VIDE ) {
    // on dejoue un coup pour en jouer un autre
    c->sud_val  = l;
    return;
  } else { // l!=SUD_VIDE && c->sud_val==SUD_VIDE
    // on joue un coup non encore joué
    c->sud_val  = l;
    sud->sud_ncv -= 1;
    return;
  }
}

void sud_coupJoueIII(Tsud* sud, int x, int y, int l)
{
  TsudCoup cp = {x,y,l};
  sud_coupJoue(sud,&cp);
}

/**
 * sud_coupVerif vérifie si jouer la valeur cp->l sur la case
 * (cp->x,cp->y) de la grille sud est valide.
 * C'est à dire que la règle 1 seule valeur par région est respectée.
 * sud_coupVerifIII est équivalent à sud_coupVerif(sud, {x,y,l}).
 * Retour: 1 jouer ce coup est valide 0 sinon.
 * Pre-condition: l!=SUD_VIDE
 * Note: aucune vérification sur le nombre de valeurs présente dans
 *       la grille n'est faite.
**/
int sud_coupVerifIII(const Tsud* sud, int x, int y, int l)
{
  int xi,yi,nb;
  /* vérification de la région ligne */
  for ( nb=0,xi=0; xi<sud->sud_dx ; xi+=1) {
    if (xi==x) continue;
    if ( sud->sud_cases[xi][y].sud_val==l ) nb+=1;
  }
  if (nb!=0) return 0;
  /* vérification de la région colonne */
  for ( nb=0,yi=0; yi<sud->sud_dy ; yi+=1) {
    if (yi==y) continue;
    if ( sud->sud_cases[x][yi].sud_val==l ) nb+=1;
  }
  if (nb!=0) return 0;
  /* vérification de la 3ieme région */
  int xii,yii;
  for ( nb=0,xii=0; xii<sud->sud_rdx ; xii+=1) {
    for ( yii=0; yii<sud->sud_rdy ; yii+=1) {
      xi = xii + (x/sud->sud_rdx)*sud->sud_rdx;
      yi = yii + (y/sud->sud_rdy)*sud->sud_rdy;
      if (xi==x && yi==y) continue;
      if ( sud->sud_cases[xi][yi].sud_val==l ) nb+=1;
    }
  }
  return nb==0;
}

int sud_coupVerif(const Tsud* sud, const TsudCoup* cp)
{ return sud_coupVerifIII(sud,cp->x,cp->y,cp->l); }

/***********************************************************************
 * Programme principal: initialisation de sudoku
**/

#define DB_TypeTyp    TsudType
#define DB_TypeVal(v) SUD_##v
#include "sudoku-db.c"

/**
 * main_init_grilleParData initialise la grille sud avec la grille de taille
 * type décrite par sudoku.
**/
void main_init_grilleParData(Tsud*sud, const TsudokuData* data)
{
  int x,y,c=0;
  sud_init(sud,data->type);
  for (y=0 ; y<sud->sud_dy ; y+=1) {
    for (x=0 ; x<sud->sud_dx ; x+=1) {
      sud_initCase( sud, x, y, data->solu[c]-'0'-1, data->init[c]!='.' );
      c += 1;
    }
  }
}

/**
 * main_init_grilleAlea initialise la grille sud avec une grille de taille
 * type choisie aléatoirement dans la DB statique.
 * Pré-condition:
 *   la DB statique doit au moins avoir une grille de chaque type.
**/
void main_init_grilleAlea(Tsud*sud, TsudType type)
{
  int i,nb;
  // nb= nombre de sudoku du type demandé dans la DB
  for ( nb=0,i=0 ; i<(sizeof(sudoku_t)/sizeof(sudoku_t[0])) ; i+=1) {
    if ( sudoku_t[i].type == type )
      nb++;
  }

  // ind= l'indice du sudoku  (ind<nb)
  int ind = random()%nb;  // nb!=0 voir pré-condition

  // recherche le ind ieme sudoku du type demandé.
  for ( nb=0,i=0 ; i<(sizeof(sudoku_t)/sizeof(sudoku_t[0])) ; i+=1) {
    if ( sudoku_t[i].type == type ) {
      if ( nb==ind )
        break;
      else
        nb +=1;
    }
  }

  // initialise la grille
  main_init_grilleParData(sud,sudoku_t+i);
}

/***********************************************************************
 * Programme principal: quelques definitions
**/

const char* progname;

#define NB_PROPOSITION_MAX(x) ((x)+(x)/3)

/***********************************************************************
 * Programme principal: partie jeu
**/

/**
 * jeu_lireProposition gère le dialogue pour lire une proposition
 * "X Y L" sur stdin.
 * Le résultat est renvoyé dans cp vérifie:
 *   0 <= cp->x < cp_max->x, 0 <= cp->y < cp_max->y,
 *   0 <= cp->l < cp_max->l
**/
void jeu_lireProposition(TsudCoup*cp, const TsudCoup* cp_max,Pile *maPile)
{
  int x_max=cp_max->x, y_max=cp_max->y, l_max=cp_max->l;
  char buffer[1000], x_lu[1000], l_lu[1000], extra[1000];
  int y_lu,c,c2;
 Tsud grille;
//Pile *maPile = initialiser();

retry:
  printf("Votre proposition (? aide): ");
  if ( fgets(buffer,sizeof(buffer),stdin)==0 ) {
    fprintf(stderr,"%s:FATAL: fin inattendue du flux stdin\n",progname);
    exit(1);
  }
  int len=strlen(buffer);
  if ( len==(sizeof(buffer)-1) && buffer[len]!='\n' ) {
    // read stdin until EOL
    int c; do c=fgetc(stdin); while ( c!='\n' && c!=EOF );
  }
  c  = toupper(buffer[0]);
  c2 = toupper(buffer[1]);

 
if(c=='<'){
printf("afficher historique\n");

 depiler(maPile);

printf("\nEtat de la pile :\n");
  
main(cp,maPile);
goto retry; }


 if ( c=='?' || (c=='A' && c2=='I') || (c=='H' && c2=='E') )
{
    printf("Aide sur l'affichage:\n"
           "  les valeurs en minuscule sont les valeurs initiales\n"
           "  vous ne pouvez les modifier\n"
           "\n");
    printf("Aide sur la saisie:\n"
           "  ?      : aide en ligne\n"
           "  ai[de] : aide en ligne\n"
           "  he[lp] : aide en ligne\n"
           "  XYV    : saisie un abscisse, une ordonnée, une valeur\n"
           "  X Y V  : saisie un abscisse, une ordonnée, une valeur\n"
           "\n"
           "  - insensible aux minuscules/majuscules\n");
    printf("  - X une lettre entre A et %c inclue\n",'A'+x_max-1);
    printf("  - Y un  nombre entre 1 et %d inclu\n",y_max);
    printf("  - V une lettre entre A et %c inclue\n",'A'+l_max-1);
    goto retry;
  }
  if ( sscanf(buffer,"%s%d%s%s",x_lu,&y_lu,l_lu,extra)!=3 ) {
    if ( buffer[0]==0 || sscanf(buffer+1,"%d%s%s",&y_lu,l_lu,extra)!=2 ) {
      printf("Erreur saisie: trop ou pas assez d'arguments\n");
      goto retry;
    } else {
      x_lu[0] = buffer[0]; x_lu[1] = 0;
    }
  }
  // vérifie x
  c=toupper(x_lu[0]);
  if ( x_lu[1]!=0 || c<'A' || ('A'+x_max)<=c ) {
    printf("Erreur saisie: \"%s\" n'est pas un abscisse valide",x_lu);
    goto retry;
  }
  cp->x = toupper(x_lu[0]) -'A';
  // vérifie y
  if ( y_lu<1 || (x_max)<y_lu) {
    printf("Erreur saisie: \"%d\" n'est pas une ordonnée valide.\n",y_lu);
    goto retry;
  }
  cp->y = y_max - y_lu;
  // vérifie l
  c=toupper(l_lu[0]);
  if ( l_lu[1]!=0 || c<'A' || ('A'+l_max)<=c ) {
    printf("Erreur saisie: \"%s\" n'est pas une wvaleur valide",l_lu);
    goto retry;
  }
  cp->l = c-'A';




  printf("X : %c\n",x_lu[0]);
  printf("Y : %d\n",y_lu);
  printf("Valeur : %c\n",c);

  empiler (maPile,x_lu[0],y_lu,c);
  afficher(maPile);
printf(" pile %d",maPile);
}

/**
 * jeu_printGrille affiche la grille sud sur stdout.
 * Si resolue est différent de zéro la solution est
 * affichée.
**/
void jeu_printGrilleHL(int dx, int rdx, int line)
{
  int x;
  printf("%3s  ","");
  for (x=0; x<dx ; x+=1) {
    if ( (x%rdx)==0 ) printf(line?"--":"  ");
    if (line)
      printf("--");
    else
      printf("%c ",'A'+x);
  }
  printf("\n");
}

void jeu_printGrille(const Tsud* sud, int resolue)
{
  int rdx=sud->sud_rdx;
  int rdy=sud->sud_rdy;
  int x,y;
  jeu_printGrilleHL(sud->sud_dx,rdx,0);
  for (y=0; y<sud->sud_dy ; y+=1) {
    if ( (y%rdy)==0) jeu_printGrilleHL(sud->sud_dx,rdx,1);
    printf("%3d  ",sud->sud_dy-y);
    for (x=0; x<sud->sud_dx ; x+=1) {
      int val=resolue ? sud->sud_cases[x][y].sud_sol
                      : sud->sud_cases[x][y].sud_val;
      if ( (x%rdx)==0 ) printf("| ");
      printf("%c ", val==SUD_VIDE                 ? '.'
                  : sud->sud_cases[x][y].sud_init ? 'a'+val
                                                  :'A'+val);
    }
    printf("|  %d\n",sud->sud_dy-y);
  }
  jeu_printGrilleHL(sud->sud_dx,rdx,1);
  jeu_printGrilleHL(sud->sud_dx,rdx,0);
}

/**
 * Vérifie si le coup cp est jouable sur la grille sud.
 * si il n'est pas jouable il écrit sur stdout un message explicatif.
 * Est vérifie:
 *    - la case cible n'est pas une case initiale.
 *    - jouer ce coup n'enfreint pas les règles du sudoku.
 * Retour: 0 si le coup est jouable sinon
 *        -1 dans le cas de l'erreur "case initiale."
 *        -2 dans le cas de l'erreur "règle enfreinte."
 * Pré-condition: x,y est une case du jeu et l une couleur valide
**/
int jeu_coupValide(const Tsud* sud, const TsudCoup* cp)
{
  int x=cp->x, y=cp->y, l=cp->l;

  // on ne peut pas modifier une case initiale
  if ( sud->sud_cases[x][y].sud_init!=0 ) {
    printf("Erreur saisie: (%c %d) est une case initiale non modifiable.\n",
        'A'+x,sud->sud_dy-y);
    return -1;
  }
  if ( sud_coupVerif(sud,cp)==0 ) {
    printf("Erreur inattention: %c ne peut pas être joué sur (%c %d).\n",
        'A'+l,'A'+x,sud->sud_dy-y);
    printf("Erreur inattention: elle vous coûte 2 poins.\n");
    return -2;
  }
  return 0;
}

/***********************************************************************
 * Programme principal: partie visualisation de la DB
**/

/**
 * Affiche sur stdout les grilles de la data base de taille type.
 * Si resolue vaut 0, elle affiche les grilles initiales sinon
 * les grilles complètes.
**/
void main_afficheGrillesT(int resolue, TsudType type)
{
  Tsud grille;
  int i;
  // nb= nombre de sudoku du type demandé dans la DB
  for ( i=0 ; i<(sizeof(sudoku_t)/sizeof(sudoku_t[0])) ; i+=1) {
    if ( sudoku_t[i].type == type ) {
      main_init_grilleParData(&grille,sudoku_t+i);
      printf("\n");
      jeu_printGrille(&grille,resolue);
    }
  }
}

/**
 * Affiche sur stdout les grilles de la data base.
 * Si resolue vaut 0, elle affiche les grilles initiales sinon
 * les grilles complètes.
**/
void main_afficheGrilles(int resolue)
{
  main_afficheGrillesT(resolue,SUD_4x4);
  main_afficheGrillesT(resolue,SUD_6x6);
  main_afficheGrillesT(resolue,SUD_8x8);
  main_afficheGrillesT(resolue,SUD_9x9);
}

/***********************************************************************
 * Programme principal: gestion de la ligne de commande
**/

typedef struct _Tparam {
  int      travail;   // 0:affiche-type 1:affiche-all 2:jeu
  int      resolue;   // affiche les grilles résolues
  TsudType type;
} Tparam;

/**
 * main_cmdline analyse la ligne de commande donnée par argc et
 * argv.
 * Retour:
 *  main_cmdline renvoie l'analyse de la ligne de commande si
 *  celle-ci est correcte sinon elle écrit un message d'erreur
 *  sur stderr et termine le programme avec un statut de 1.
**/
Tparam main_cmdline(int argc, char *argv[])
{
  int i;
  // initialisation des options par défaut.
  Tparam ret;

  memset(&ret,0,sizeof(ret));
  ret.travail = 2;
  ret.type = SUD_9x9;

  // traite le help
  for (i=0 ; i<argc ; i+=1) {
    if ( strcmp("-h",argv[i])==0 ) {
      printf("usage: %s [OPTIONS]\n",progname);
      printf("\n");
      printf("jeu de sudoku en mode console\n");
      printf("\n");
      printf("OPTIONS\n"
        "  -h   : cette aide.\n"
        "  -g   : Écrit sur stdout les grilles et se termine.\n"
        "  -s   : Écrit sur stdout les grilles solutionnées et se termine.\n"
        "  -t # : sélectionne la taille de la grille à #. Les valeurs de #\n"
        "         sont: 4x4, 6x6, 8x8, 9x9, rand.\n"
        "\n"
        "les options -g et -s sont exclusives.\n");
      exit(0);
    }
  }

  // analyse de argv
  int opt_g=0, opt_s=0, opt_t=0, opt_rand=0;
  for (i=0 ; i<argc ; i+=1) {
    char*opt = argv[i];
    if ( strcmp("-g",opt)==0 ) {
      if ( opt_g==1 ) {
        fprintf(stderr,"%s:Warning: l'option %s est présente 2 fois.\n",
          progname,opt);
        continue;
      }
      if ( opt_s==1 ) {
        fprintf(stderr,"%s:ERREUR: les options -g et -s sont exclusives.\n",
          progname);
        exit(1);
      }
      opt_g = 1;
    }
    if ( strcmp("-s",opt)==0 ) {
      if ( opt_s==1 ) {
        fprintf(stderr,"%s:Warning: l'option %s est présente 2 fois.\n",
          progname,opt);
        continue;
      }
      if ( opt_g==1 ) {
        fprintf(stderr,"%s:ERREUR: les options -g et -s sont exclusives.\n",
          progname);
        exit(1);
      }
      opt_s = 1;
    }
    if ( strcmp("-t",opt)==0 ) {
      if ( opt_t==1 ) {
        fprintf(stderr,"%s:ERREUR: l'option %s est présente 2 fois.\n",
          progname,opt);
        exit(1);
      }
      const char* val = argv[i+1];
      if ( val==0 ) {
        fprintf(stderr,"%s:ERREUR: l'option %s est mal formatée.\n",
          progname,opt);
        exit(1);
      }
      i += 1;
      if ( strcmp(val,"rand")==0 ) {
        opt_rand = 1;
      } else if ( strcmp(val,"4x4")==0 ) { ret.type = SUD_4x4;
      } else if ( strcmp(val,"6x6")==0 ) { ret.type = SUD_6x6;
      } else if ( strcmp(val,"8x8")==0 ) { ret.type = SUD_8x8;
      } else if ( strcmp(val,"9x9")==0 ) { ret.type = SUD_9x9;
      } else {
        fprintf(stderr,
          "%s:ERREUR: %s est un type non reconnu,\n",progname,val);
        fprintf(stderr,
          "%s:ERREUR: essayez -h pour avoir les types supportés.\n",progname);
        exit(1);
      }
    }
  }

  // mise à jour de ret
  if ( opt_g || opt_s ) {
    ret.travail = opt_rand!=0 ;
    if ( opt_s ) ret.resolue = 1;
  } else { // jeu
    if ( opt_rand ) {
      ret.type = random()%SUD_TypeNb;
    }
  }

  return ret;
}

/***********************************************************************
 * Programme principal
**/

int main(int argc, char*argv[])
{
  progname = argv[0];
  srandom(time(NULL));

  // analyse de la ligne de commande
  Tparam param = main_cmdline(argc,argv);

  // travail
  if ( param.travail==0 ) {
    main_afficheGrillesT(param.resolue, param.type);
    return 0;
  } if ( param.travail==1 ) {
    main_afficheGrilles(param.resolue);
    return 0;
  }

  /* faire une partie */

  // initialisation
  Tsud grille;           // la grille de jeu
  main_init_grilleAlea(&grille,param.type);

  int nb_proposition=0;      // compte les coups joués
  int nb_proposition_min=sud_nbCasesVides(&grille);
  int nb_proposition_max=NB_PROPOSITION_MAX(nb_proposition_min);
  TsudCoup cp_max = { // les valeurs maximales pour un coup
    grille.sud_dx,    // les minimales sont zéro
    grille.sud_dy,
    grille.sud_rdx*grille.sud_rdy};
#define SCORE(nbp,nbp_min) (20.*((double)(nbp_min))/((double)(nbp)))

  // boucle de jeu
  do {

  Pile *maPile = initialiser();
    int coupJouable;
    TsudCoup cp;

    printf("\n\n");
    jeu_printGrille(&grille,0);
    printf("coup restant:%-3d    score courant:%4.1f\n",
      nb_proposition_max-nb_proposition,
      SCORE(nb_proposition,nb_proposition_min));
    do {
      jeu_lireProposition(&cp, &cp_max,maPile);
      coupJouable=jeu_coupValide(&grille,&cp);
      // si le joueur n'a pas respecté les règles --> pénalité
      if ( coupJouable==-2 ) nb_proposition += 2;
    } while ( coupJouable!=0 );
sud_coupJoue(&grille, &cp);



    nb_proposition += 1;

  } while ( nb_proposition < nb_proposition_max
            && sud_grilleRemplie(&grille)==0 );

  // afficher le résultat du jeu
  printf("\n\n");
  if ( sud_grilleRemplie(&grille)!=0 ) {
    jeu_printGrille(&grille,1);
    printf("Bravo vous avez gagnez, votre score est de %4.1f/20\n",
        SCORE(nb_proposition,nb_proposition_min));
  } else {
    printf("Vous avez perdu. La solution était:\n");
    jeu_printGrille(&grille,1);
  }
  return 0;


}

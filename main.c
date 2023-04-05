#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "affiche_tas.h"
#include "tas_funct.h"

int main()
{
    char *p1, *p2, *p3, *p4, *p5;
    char*(*tabStrategie[])(int) = {worst_fit, best_fit, first_fit}; //Tableau de pointeur de fonction
    int choix;

    tas_init();

    printf("Choix de la strategie (w : worst_fit, b : best_fit, f : first_fit) : ");
    fscanf(stdin, "%c", &choix);

    int strat = 0;

    switch(choix)
    {
        case 'w':
            strat = 0;
            break;
        case 'b':
            strat = 1;
            break;
        default:
            strat = 2;
            break;
    }
    Libre.libCount = 0; //Initialisation du compteur de libre pour la partie tas_free

    p1 = tas_malloc(10, tabStrategie[strat]);
    p2 = tas_malloc(9, tabStrategie[strat]);
    p3 = tas_malloc(5, tabStrategie[strat]);

    strcopy(p1, "tp 1" );
    strcopy(p2, "tp 2" );
    strcopy(p3, "tp 3" );

    tas_free(p2);
    p4 = tas_malloc(8, tabStrategie[strat]);
    strcopy(p4, "systeme");

    p5 = tas_malloc(8, tabStrategie[strat]);
    strcopy(p5, "victoire");

    tas_free(p2);
    tas_free(p1);

    afficher_tas();

    return 0;
}

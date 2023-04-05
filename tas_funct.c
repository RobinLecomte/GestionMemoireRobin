#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "affiche_tas.h"
#include "tas_funct.h"

char* best_fit(int taille) // Le plus petit bloc suffisamment grand pour contenir notre processus est choisi.
{
    char* ptr = tas + libre; //Adresse du premier element à parcourir
    char* tab_adress[TAILTAS]; //Tableau de pointeurs contenant les adresses des zones disponibles
    int tab_taille[TAILTAS]; //Tableau contenant la taille de la zone de l'adresse correspondante
    int c = 0;
    int found = 0; //Booleen indiquant si une zone a été trouvée

    while (ptr < tas + TAILTAS)
    {
        if (*ptr >= taille && *(ptr + 1) == FIN) //Vérifie si l'allocation est libre et si la taille est suffisante
        {
            tab_adress[c] = ptr; //Récupère l'adresse dans le tableau
            tab_taille[c] = *ptr; //Récupère la taille de la zone
            found = 1;
            c++;
        }
        ptr++;
    }

    if (found == 0)
    {
        return NULL; //Pas d'espace libre trouvé
    }

    int taille_min = TAILTAS; //Taille min de la zone à allouer
    char* adress_min = tas; //Adresse de la zone à allouer

    for (int i = 0 ; i < c ; i++)
    {
        if (tab_taille[i] < taille_min) //Si la taille est inférieure à la taille max
        {
            taille_min = tab_taille[i]; //Nouveau taille_max
            adress_min = tab_adress[i]; //Stockage de l'adresse à renvoyer
        }
    }
    return adress_min;
}

char* worst_fit(int taille) //Le bloc qui nous laisse le plus grand morceau de mémoire libre est choisi (le plus grand bloc).
{
    char* ptr = tas + libre; //Adresse du premier element à parcourir
    char* tab_adress[TAILTAS]; //Tableau de pointeurs contenant les adresses des zones disponibles
    int tab_taille[TAILTAS]; //Tableau contenant la taille de la zone de l'adresse correspondante
    int c = 0;
    int found = 0; //Booleen indiquant si une zone a été trouvée

    while (ptr < tas + TAILTAS)
    {
        if (*ptr >= taille && *(ptr + 1) == FIN) //Vérifie si l'allocation est libre et si la taille est suffisante
        {
            tab_adress[c] = ptr; //Récupère l'adresse dans le tableau
            tab_taille[c] = *ptr; //Récupère la taille de la zone
            found = 1;
            c++;
        }
        ptr++;
    }

    if (found == 0)
    {
        return NULL; //Pas d'espace libre trouvé
    }

    int taille_max = 0; //Taille max de la zone à allouer
    char* adress_max = tas; //Adresse de la zone à allouer

    for (int i = 0 ; i < c ; i++)
    {
        if (tab_taille[i] > taille_max) //Si la taille est inférieure à la taille max
        {
            taille_max = tab_taille[i]; //Nouveau taille_max
            adress_max = tab_adress[i]; //Stockage de l'adresse à renvoyer
        }
    }
    return adress_max;
}

char* first_fit(int taille) // Le premier bloc suffisamment grand pour contenir notre processus est choisi.
{
    char* ptr = tas + libre; //Adresse du premier element à parcourir

    while (ptr < tas + TAILTAS)
    {
        if (*ptr >= taille && *(ptr + 1) == FIN) //Vérifie si l'allocation est libre et si la taille est suffisante
        {
            return ptr;
        }
        ptr++;
    }
    return NULL; //Aucun espace libre suffisant trouvé
}

char* tas_malloc(int taille, char* (*strat)(int))
{
    if (strat(taille) == NULL) //Récupération de la stratégie demandée
    {
        return NULL;
    }
    libre = strat(taille) - tas; //Index de libre dans le tas
    char *adress_return = tas + libre + 1; //Adresse du début de l'allocation retournée à la fin

    int found = 0;
    for (int i = 0 ; i < Libre.libCount ; i++)
    {
        if (Libre.indexLibreTab[Libre.libCount] == libre) //Si ce libre est déja stocké (free --> alloc)
        {
            found = 1;
        }
    }
    if (found == 0) //Si le libre n'est pas deja stocké
    {
        Libre.indexLibreTab[Libre.libCount] = libre; //Stockage de l'index de libre
        Libre.libCount++; //Compteur
    }

    Libre.libTab[libre] = adress_return; //Stockage de l'adresse sur l'index de libre

    *(adress_return) = DEFAUT; //Remplace le -1 par 0
    int old_taille = *(tas + libre); //Ancienne taille utilisée

    if (*(tas + libre + old_taille + 2) == DEFAUT) //Cas où il n'y a pas d'allocations effectuées à droite
    {
        *(tas + libre) = taille; //Taille de l'espace occupée sur le bloc libre
        *(tas + libre + taille + 1) = TAILTAS - (libre + taille + 1); //Nombre de cases libres
        *(tas + libre + taille + 2) = FIN; //-1 après l'indication des cases libres

        libre = libre + taille + 1; //Nouvelle index de la case libre

    }
    else
    {
        libre = libre + old_taille + 1; //Cas avec une allocation à droite
    }

    return adress_return; //Renvoie l'adresse de la première case à remplir

}

void tas_free(char* alloc)
{
    *(alloc) = FIN; //Marque la zone d'un -1 pour dire qu'elle n'est plus allouée

    int c = 0;
    while (c < TAILTAS)
    {
        if (Libre.libTab[c] == alloc)
        {
            Libre.libTab[c] = FIN; //Remplace le libre par la valeur par défaut si l'adresse est trouvée
        }
        c++;
    }

    if (alloc - tas - 1 < libre)
    {
        libre = alloc - tas - 1; //Nouveau libre si on a pas déja un libre plus faible
    }

    char* mergeTab[TAILTAS]; //Stockage sans les zeros de libTab

    c = 0;
    int c2 = 0;
    while (c < TAILTAS)
    {
        if (Libre.libTab[c] != 0)
        {
            mergeTab[c2] = Libre.libTab[c]; //On se débarasse des zero pour avoir les adresses de libres cote a cote
            c2++;
        }
        c++;
    }
    for (int i = 0 ; i < c2 ; i++)
    {
        if (mergeTab[i] == FIN && mergeTab[i + 1] == FIN) //Cas de deux allocation vides cote à cote
        {
            *(tas + Libre.indexLibreTab[i + 1] + 1) = DEFAUT; //Remplace le -1 de droite par la valeur par défaut
            int taille_droite = *(tas + Libre.indexLibreTab[i + 1]); //Taille de l'allocation de droite
            *(tas + Libre.indexLibreTab[i]) += taille_droite + 1; //Ajout de la taille de droite à celle de gauche (en prenant en compte la case indiquant le nombre de cases libres)
            *(tas + Libre.indexLibreTab[i + 1]) = DEFAUT; //Ramplace la taille par la valeur par defaut
        }
    }
}

void strcopy(char* alloc, char* chaine)
{
    if (*(alloc - 1) >= strlen(chaine) + 1) //Vérifie si l'espace alloué est suffisemment grand pour la chaine (prend en compte le \0)
    {
        strcpy(alloc, chaine);
    }
    else
    {
        printf("Copie de (%s) a l'adresse %d impossible car espace insuffisante\n", chaine, alloc);
    }
}

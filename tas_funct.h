#pragma once

char* best_fit(int taille);
char* worst_fit(int taille);
char* first_fit(int taille);
char* tas_malloc(int taille, char* (*strat)(int));
void tas_free(char* alloc);
void strcopy(char* alloc, char* chaine);

struct Libre
{
    int libCount;
    int indexLibreTab[TAILTAS];
    char* libTab[TAILTAS];
}Libre;

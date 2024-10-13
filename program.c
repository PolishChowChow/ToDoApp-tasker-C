#include<stdio.h>
#include <stdlib.h>
#include<string.h>

typedef struct
{
    char nazwa[50];
    char opis[200];
    char data_wykonania[10];
} Zadanie;

Zadanie stworz_zadanie(char nazwa[50], char opis[200], char data_wykonania[10]){
    Zadanie nowe_zadanie;
    strcpy(nowe_zadanie.nazwa, nazwa);
    strcpy(nowe_zadanie.opis, opis);
    strcpy(nowe_zadanie.data_wykonania, data_wykonania);
    return nowe_zadanie;
};
void dodaj_zadanie_do_listy(Zadanie **lista_zadan, int *rozmiar, char nazwa[50], char opis[200], char data_wykonania[10]){
    *lista_zadan = realloc(*lista_zadan, (*rozmiar + 1) * sizeof(Zadanie));
    if(*lista_zadan == NULL){
        printf("błąd w alokacji listy \n");
        exit(0);
    }
    Zadanie nowe_zadanie = stworz_zadanie(nazwa, opis, data_wykonania);
    (*lista_zadan)[*rozmiar] = nowe_zadanie;
    (*rozmiar)++;
}

void wyswietl_liste_zadan(Zadanie *lista_zadan, int rozmiar){
    printf("=========================================================================== \n");
    for(int i = 0; i < rozmiar; i++){
        printf("|%s|%s|%s| \n", lista_zadan[i].nazwa, lista_zadan[i].opis, lista_zadan[i].data_wykonania);
    }
    printf("=========================================================================== \n");
}
int main(){
    Zadanie *lista_zadan = NULL;
    int rozmiar_listy = 0;
    char nazwa[50];
    char opis[200];
    char data_wykonania[10];
    printf("Podaj nazwe \n");
    scanf("%49s", nazwa);
    printf("Podaj opis \n");
    scanf("%199s", opis);
    printf("Podaj date wykonania \n");
    scanf("%9s", data_wykonania);
    dodaj_zadanie_do_listy(&lista_zadan, &rozmiar_listy, nazwa, opis, data_wykonania);
    wyswietl_liste_zadan(lista_zadan, rozmiar_listy);
    
    free(lista_zadan);
    getchar();
}
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<locale.h>

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
void dodaj_zadanie_do_listy(Zadanie **lista_zadan, int *rozmiar){
    char nazwa[50];
    char opis[200];
    char data_wykonania[10];
    printf("Podaj nazwe \n");
    scanf("%49s", nazwa);
    printf("Podaj opis \n");
    scanf("%199s", opis);
    printf("Podaj date wykonania \n");
    scanf("%9s", data_wykonania);
    *lista_zadan = realloc(*lista_zadan, (*rozmiar + 1) * sizeof(Zadanie));
    if(*lista_zadan == NULL){
        printf("błąd w alokacji listy \n");
        exit(0);
    }
    Zadanie nowe_zadanie = stworz_zadanie(nazwa, opis, data_wykonania);
    (*lista_zadan)[*rozmiar] = nowe_zadanie;
    (*rozmiar)++;
    printf("utworzono zadanie \n");
    getchar();
}

void wyswietl_liste_zadan(Zadanie *lista_zadan, int rozmiar){
    printf("=========================================================================== \n");
    for(int i = 0; i < rozmiar; i++){
        printf("|%d|%s|%s|%s| \n", i+1, lista_zadan[i].nazwa, lista_zadan[i].opis, lista_zadan[i].data_wykonania);
    }
    printf("=========================================================================== \n");
}

void usun_zadanie(Zadanie **lista_zadan, int *rozmiar){
    int index;
    Zadanie *nowa_lista_zadan = malloc((*rozmiar - 1) * sizeof(Zadanie));
        if(nowa_lista_zadan == NULL){
        printf("problem z alokacja nowej listy \n");
        exit(1);
    }
    printf("Podaj numer zadanie, ktory chcesz usunac: \n");
    scanf(" %d", &index);

    index -= 1;
    int j = 0;
    for(int  i = 0;  i < *rozmiar; i++){
        if(i != index){
            nowa_lista_zadan[j] = (*lista_zadan)[i];
            j++;
        }
    }

    free(*lista_zadan);
    *lista_zadan = nowa_lista_zadan;
    (*rozmiar)--;
    printf("zadanie zostalo usuniete \n");
}

void usun_wszystkie_zadania(Zadanie **lista_zadan, int *rozmiar){
    if (*lista_zadan != NULL){
        free(*lista_zadan);  
        *lista_zadan = NULL;
    }
    (*rozmiar) = 0;
    printf("usunieto wszystkie zadania \n");
}

int main(){
    Zadanie *lista_zadan = NULL;
    int rozmiar_listy = 0;
    char wybor;
    while(1){
        printf("Co chcesz zrobić? \n");
        printf("1 - dodaj nowe zadanie \n");
        printf("2 - wyswietl wszystkie zadania \n");
        printf("3 - usun zadanie \n");
        printf("4 - usun wszystkie zadania \n");
        printf("5 - wyjdz \n");
        scanf(" %c", &wybor);
        getchar();
        switch (wybor)
        {
        case '5':
            free(lista_zadan);
            exit(0);
            break;
        case '4':
            usun_wszystkie_zadania(&lista_zadan, &rozmiar_listy);
            break;
        case '3':
            usun_zadanie(&lista_zadan, &rozmiar_listy);
            break;
        case '2':
            wyswietl_liste_zadan(lista_zadan, rozmiar_listy);
            break;
        case '1':
            dodaj_zadanie_do_listy(&lista_zadan, &rozmiar_listy);
            break;
        default:
            // system("clear");
            printf("Blad: nie ma takiego wyboru, sprobuj ponownie \n \n");
            // getchar();
        }
    }
}
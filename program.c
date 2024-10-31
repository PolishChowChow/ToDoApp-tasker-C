#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#define ROZMIAR_NAZWA 30
#define ROZMIAR_OPIS 100
#define ROZMIAR_DATA_WYKONANIA 12
#define ROZMIARY_BUFOROW [ROZMIAR_NAZWA, ROZMIAR_OPIS, ROZMIAR_DATA_WYKONANIA]

const char* wyczyszcz_ekran;

typedef struct
{
    char nazwa[ROZMIAR_NAZWA];
    char opis[ROZMIAR_OPIS];
    char data_wykonania[ROZMIAR_DATA_WYKONANIA];
} Zadanie;

void ustaw_komende_do_czyszczenia(){    
    #ifdef _WIN32
        wyczyszcz_ekran = "cls";
    #else
        wyczyszcz_ekran = "clear";
    #endif
}

Zadanie stworz_zadanie(char nazwa[ROZMIAR_NAZWA], char opis[ROZMIAR_OPIS], char data_wykonania[ROZMIAR_DATA_WYKONANIA]){
    Zadanie nowe_zadanie;
    strcpy(nowe_zadanie.nazwa, nazwa);
    strcpy(nowe_zadanie.opis, opis);
    strcpy(nowe_zadanie.data_wykonania, data_wykonania);
    return nowe_zadanie;
};

void wstaw_linie(){
    for(int i = 0; i < 150; i++){
        switch (i)
        {
        case 0:
        case 4:
        case 35:
        case 136:
        case 149:
            printf("+");
            break;
        default:
            printf("-");
        }
    }
    printf("\n");
}

void wyczysc_bufor(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

void sprawdz_input(char *bufor, int rozmiar, char *polecenie){
    while(1){
        printf("%s", polecenie);
        fgets(bufor, rozmiar, stdin);
        int dlugosc = strlen(bufor);
        if(strchr(bufor, '\n') == NULL) {
            wyczysc_bufor();
            printf("Wprowadzony tekst jest za dlugi, sprobuj ponownie \n");
            continue;
        }
        else{
            bufor[strcspn(bufor, "\n")] = '\0';
        }
        if(dlugosc <= 1){
            printf("Blad: nie wprowadzono niczego, sprobuj ponownie \n");
            continue;
        }
        break;
    }
}

void sprawdz_input_z_data(char *bufor, int rozmiar, char *polecenie){
    sprawdz_input(bufor, rozmiar, polecenie);
}
void dodaj_zadanie_do_listy(Zadanie **lista_zadan, int *rozmiar){
    char nazwa[ROZMIAR_NAZWA];
    char opis[ROZMIAR_OPIS];
    char data_wykonania[ROZMIAR_DATA_WYKONANIA];
    sprawdz_input(nazwa, ROZMIAR_NAZWA, "Podaj nazwę \n");
    sprawdz_input(opis, ROZMIAR_OPIS, "Podaj opis \n");
    sprawdz_input(data_wykonania, ROZMIAR_DATA_WYKONANIA, "Podaj datę wykonania \n");
    *lista_zadan = (Zadanie*)realloc(*lista_zadan, (*rozmiar + 1) * sizeof(Zadanie));
    if(*lista_zadan == NULL){
        printf("błąd w alokacji listy \n");
        exit(0);
    }
    Zadanie nowe_zadanie = stworz_zadanie(nazwa, opis, data_wykonania);
    (*lista_zadan)[*rozmiar] = nowe_zadanie;
    (*rozmiar)++;
    system(wyczyszcz_ekran);
    printf("Utworzono nowe zadanie. \n\n");
}

void wyswietl_liste_zadan(Zadanie *lista_zadan, int rozmiar){
    char naglowki[4][200] = {
        "Lp.",
        "Nazwa",
        "Opis",
        "Data"
    };
    if(rozmiar == 0){
        printf("Brak zapisanych zadań.");
    }
    else{
        printf("\n\n");
        wstaw_linie();    
        printf("|%s|%-30s|%-100s|%-12s| \n", naglowki[0], naglowki[1], naglowki[2], naglowki[3]);
        wstaw_linie();
        for(int i = 0; i < rozmiar; i++){
            printf("|%-3d|%-30s|%-100s|%-12s| \n", i+1, lista_zadan[i].nazwa, lista_zadan[i].opis, lista_zadan[i].data_wykonania);
            wstaw_linie();
        }
    }
}

void usun_zadanie(Zadanie **lista_zadan, int *rozmiar){
    int index;
    Zadanie *nowa_lista_zadan = (Zadanie*)malloc((*rozmiar - 1) * sizeof(Zadanie));
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
    system(wyczyszcz_ekran);
    printf("Zadanie zostało usunięte. \n\n");
}

void usun_wszystkie_zadania(Zadanie **lista_zadan, int *rozmiar){
    if (*lista_zadan != NULL){
        free(*lista_zadan);  
        *lista_zadan = NULL;
    }
    (*rozmiar) = 0;
    system(wyczyszcz_ekran);
    printf("Usunięto wszystkie zadania. \n\n");
}

int main(){
    ustaw_komende_do_czyszczenia();
    setlocale(LC_ALL, "pl-PL");
    Zadanie *lista_zadan = NULL;
    int rozmiar_listy = 0;
    char wybor;
    while(1){
        printf("Co chcesz zrobić? \n");
        printf("1 - Dodaj nowe zadanie. \n");
        printf("2 - Wyświetl wszystkie zadania. \n");
        printf("3 - Usuń zadanie. \n");
        printf("4 - Usuń wszystkie zadania. \n");
        printf("5 - Wyjdź. \n");
        scanf(" %c", &wybor);
        system(wyczyszcz_ekran);
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
            system(wyczyszcz_ekran);
            printf("Błąd: nie ma takiego wyboru, spróbuj ponownie ponownie \n\n");
        }
    }
}
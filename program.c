#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#define ROZMIAR_NAZWA 30
#define ROZMIAR_OPIS 100
#define ROZMIAR_DATA_WYKONANIA 12
#define ROZMIARY_BUFOROW [ROZMIAR_NAZWA, ROZMIAR_OPIS, ROZMIAR_DATA_WYKONANIA]

const char* wyczysc_ekran;

typedef struct
{
    char nazwa[ROZMIAR_NAZWA];
    char opis[ROZMIAR_OPIS];
    char data_wykonania[ROZMIAR_DATA_WYKONANIA];
} Zadanie;

void ustaw_komende_do_czyszczenia(){    
    #ifdef _WIN32
        wyczysc_ekran = "cls";
    #else
        wyczysc_ekran = "clear";
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
    for(int i = 0; i < 152; i++){
        switch (i)
        {
        case 0:
        case 4:
        case 35:
        case 136:
        case 151:
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

bool walidator_daty(char *data){
    int rok;
    int miesiac;
    int dzien;
    int is_not_rok_przestepny = 2;
    for(int i = 0; i < ROZMIAR_DATA_WYKONANIA; i++){
        switch (i)
        {
        case 2:
        case 5:
            if(data[i] != '-'){
                return false;
            }
            break;
        case 9:
        case 10:
        case 11:
            break;
        default:
            if(!isdigit(data[i])){
                return false;
            }
            break;
        }
    }
    dzien = (data[0] - '0') * 10 + (data[1] - '0');
    miesiac = (data[3] - '0') * 10 + (data[4] - '0');
    rok = (data[6] - '0') * 1000 + (data[7] - '0') * 100 + (data[8] - '0') * 10 + (data[9] - '0');
    if(miesiac > 12 || miesiac < 0 || rok < 2024 || dzien < 0){
        return false;
    }
    if(rok % 400 == 0 || (rok % 4 == 0 && rok % 100 != 0 )){
        is_not_rok_przestepny = 0;
    }
    if(miesiac == 2 && dzien + is_not_rok_przestepny > 29){
        return false;
    }
    if(miesiac < 7 ^  miesiac % 2 == 0){
        if(dzien > 31){
            return false;
        }
    }
    else{
        if(dzien > 30){
            return false;
        }
    }
    return true;
}

void sprawdz_input(char *bufor, int rozmiar, char *polecenie, bool (*walidator)(char* data)){
    bool poprawny_input;
    do{
	poprawny_input = true;
        printf("%s", polecenie);
        fgets(bufor, rozmiar, stdin);
        int dlugosc = strlen(bufor);
        if(strchr(bufor, '\n') == NULL) {
            wyczysc_bufor();
	    system(wyczysc_ekran);
            printf("Wprowadzony tekst jest za długi, spróbuj ponownie. \n\n");
            poprawny_input = false;
        }
        else{
            bufor[strcspn(bufor, "\n")] = '\0';
        }
        if(dlugosc <= 1){
	    system(wyczysc_ekran);
            printf("Błąd: nie wprowadzono niczego, spróbuj ponownie. \n\n");
            poprawny_input = false;
        }
        if(walidator != NULL && !walidator(bufor)){
		system(wyczysc_ekran);
                printf("Podano nieprawidłowy format danych, spróbuj ponownie.  \n\n");
                poprawny_input = false;
        }
    }while(!poprawny_input);
}


void dodaj_zadanie_do_listy(Zadanie **lista_zadan, int *rozmiar){
    char nazwa[ROZMIAR_NAZWA];
    char opis[ROZMIAR_OPIS];
    char data_wykonania[ROZMIAR_DATA_WYKONANIA];
    sprawdz_input(nazwa, ROZMIAR_NAZWA, "Podaj nazwę \n", NULL);
    sprawdz_input(opis, ROZMIAR_OPIS, "Podaj opis \n", NULL);
    sprawdz_input(data_wykonania, ROZMIAR_DATA_WYKONANIA, "Podaj datę wykonania \n", walidator_daty);
    *lista_zadan = (Zadanie*)realloc(*lista_zadan, (*rozmiar + 1) * sizeof(Zadanie));
    if(*lista_zadan == NULL){
        printf("Błąd w alokacji listy. \n");
        exit(0);
    }
    Zadanie nowe_zadanie = stworz_zadanie(nazwa, opis, data_wykonania);
    (*lista_zadan)[*rozmiar] = nowe_zadanie;
    (*rozmiar)++;
    system(wyczysc_ekran);
    printf("Utworzono nowe zadanie. \n\n");
}

void wyswietl_liste_zadan(Zadanie *lista_zadan, int rozmiar){
    char naglowki[4][200] = {
        "Lp.",
        "Nazwa",
        "Opis",
        "Data wykonania"
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
            printf("|%-3d|%-30s|%-100s|%-14s| \n", i+1, lista_zadan[i].nazwa, lista_zadan[i].opis, lista_zadan[i].data_wykonania);
            wstaw_linie();
        }
    }
}

void usun_zadanie(Zadanie **lista_zadan, int *rozmiar){
    int index;
    Zadanie *nowa_lista_zadan;
    if(*rozmiar == 0){
        printf("Nie można usunąć zadania z pustej listy zadań. \n\n");
        return;
    }
    printf("Podaj numer zadania, które chcesz usunąć: \n");
    scanf(" %d", &index);
    if(index > (*rozmiar) || index < 0){
	printf("Nie można usunąć zadania o nieistniejącym numerze. \n\n");
        return;
    }
    nowa_lista_zadan = (Zadanie*)malloc((*rozmiar - 1) * sizeof(Zadanie));
    if(nowa_lista_zadan == NULL){
        printf("Problem z alokacja nowej listy. \n");
        exit(1);
    }
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
    system(wyczysc_ekran);
    printf("Zadanie zostało usunięte. \n\n");
}

void usun_wszystkie_zadania(Zadanie **lista_zadan, int *rozmiar){
    if (*lista_zadan != NULL){
        free(*lista_zadan);  
        *lista_zadan = NULL;
    }
    (*rozmiar) = 0;
    system(wyczysc_ekran);
    printf("Usunięto wszystkie zadania. \n\n");
}

int main(){
    ustaw_komende_do_czyszczenia();
    setlocale(LC_ALL, "pl-PL");
    Zadanie *lista_zadan = NULL;
    int rozmiar_listy = 0;
    char wybor;
    while(wybor != '5'){
        printf("Co chcesz zrobić? \n");
        printf("1 - Dodaj nowe zadanie. \n");
        printf("2 - Wyświetl wszystkie zadania. \n");
        printf("3 - Usuń zadanie. \n");
        printf("4 - Usuń wszystkie zadania. \n");
        printf("5 - Wyjdź. \n");
        scanf(" %c", &wybor);
        system(wyczysc_ekran);
        getchar();
        switch (wybor)
        {
        case '5':
            free(lista_zadan);
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
            system(wyczysc_ekran);
            printf("Błąd: nie ma takiego wyboru, spróbuj ponownie. \n\n");
        }
    }
}

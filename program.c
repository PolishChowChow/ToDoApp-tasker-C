//Biblioteki
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <wchar.h>

// Stałe
#define ROZMIAR_NAZWA 30
#define ROZMIAR_OPIS 100
#define ROZMIAR_DATA_WYKONANIA 12
const char *wyczysc_ekran;

// Struktura do przechowywania zadania
typedef struct {
    wchar_t nazwa[ROZMIAR_NAZWA];
    wchar_t opis[ROZMIAR_OPIS];
    wchar_t data_wykonania[ROZMIAR_DATA_WYKONANIA];
} Zadanie;

// Funkcja do ustawiania komendy czyszczenia ekranu w zależności od systemu
void ustaw_komende_do_czyszczenia() {    
    #ifdef _WIN32
        wyczysc_ekran = "cls";
    #else
        wyczysc_ekran = "clear";
    #endif
}

// Funkcja tworzy zadanie na podstawie podanej nazwy, opisu oraz daty
Zadanie stworz_zadanie(wchar_t nazwa[ROZMIAR_NAZWA], wchar_t opis[ROZMIAR_OPIS], wchar_t data_wykonania[ROZMIAR_DATA_WYKONANIA]) {
    Zadanie nowe_zadanie;
    wcscpy(nowe_zadanie.nazwa, nazwa);
    wcscpy(nowe_zadanie.opis, opis);
    wcscpy(nowe_zadanie.data_wykonania, data_wykonania);
    return nowe_zadanie;
}

// Funkcja do wyświetlania linii w terminalu (oddzielanie wierszy tabeli)
void wstaw_linie() {
    for (int i = 0; i < 152; i++) {
        switch (i) {
            case 0:
            case 4:
            case 35:
            case 136:
            case 151:
                wprintf(L"+");
                break;
            default:
                wprintf(L"-");
        }
    }
    wprintf(L"\n");
}

// Funkcja do czyszczenia buforu (ułatwia obsługę wprowadzania danych)
void wyczysc_bufor() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funkcja walidująca format daty "DD-MM-YYYY"
bool walidator_daty(wchar_t *data) {
    int rok, miesiac, dzien;
    int is_not_rok_przestepny = 2;
    for (int i = 0; i < ROZMIAR_DATA_WYKONANIA; i++) {
        switch (i) {
            case 2:
            case 5:
                if (data[i] != '-') {
                    return false;
                }
                break;
            case 9:
            case 10:
            case 11:
                break;
            default:
                if (!iswdigit(data[i])) {
                    return false;
                }
                break;
        }
    }
    dzien = (data[0] - L'0') * 10 + (data[1] - L'0');
    miesiac = (data[3] - L'0') * 10 + (data[4] - L'0');
    rok = (data[6] - L'0') * 1000 + (data[7] - L'0') * 100 + (data[8] - L'0') * 10 + (data[9] - L'0');
    
    if (miesiac > 12 || miesiac < 1 || rok < 2024 || dzien < 1) {
        return false;
    }
    if (rok % 400 == 0 || (rok % 4 == 0 && rok % 100 != 0)) {
        is_not_rok_przestepny = 0;
    }
    if (miesiac == 2 && dzien + is_not_rok_przestepny > 29) {
        return false;
    }
    if (miesiac < 7 ^ miesiac % 2 == 0) {
        if (dzien > 31) {
            return false;
        }
    } else {
        if (dzien > 30) {
            return false;
        }
    }
    return true;
}

// Funkcja do sprawdzania i walidacji wprowadzanych danych
void sprawdz_input(wchar_t *bufor, int rozmiar, wchar_t *polecenie, bool (*walidator)(wchar_t* data)) {
    bool poprawny_input;
    do {
        poprawny_input = true;
        wprintf(L"%ls", polecenie);
        fgetws(bufor, rozmiar, stdin);
        int dlugosc = wcslen(bufor);
        if (wcschr(bufor, L'\n') == NULL) {
            wyczysc_bufor();
            wprintf(L"Wprowadzony tekst jest za długi, spróbuj ponownie. \n\n");
            poprawny_input = false;
        } else {
            bufor[wcslen(bufor) - 1] = L'\0';
        }
        if (dlugosc <= 1) {
            wprintf(L"Błąd: nie wprowadzono niczego, spróbuj ponownie. \n\n");
            poprawny_input = false;
        }
        if (walidator != NULL && !walidator(bufor)) {
            wprintf(L"Podano nieprawidłowy format danych, spróbuj ponownie.  \n\n");
            poprawny_input = false;
        }
    } while (!poprawny_input);
}

// Funkcja do dodawania zadania do listy
void dodaj_zadanie_do_listy(Zadanie **lista_zadan, int *rozmiar) {
    wchar_t nazwa[ROZMIAR_NAZWA];
    wchar_t opis[ROZMIAR_OPIS];
    wchar_t data_wykonania[ROZMIAR_DATA_WYKONANIA];
    sprawdz_input(nazwa, ROZMIAR_NAZWA, L"Podaj nazwę \n", NULL);
    sprawdz_input(opis, ROZMIAR_OPIS, L"Podaj opis \n", NULL);
    sprawdz_input(data_wykonania, ROZMIAR_DATA_WYKONANIA, L"Podaj datę wykonania \n", walidator_daty);
    *lista_zadan = (Zadanie*)realloc(*lista_zadan, (*rozmiar + 1) * sizeof(Zadanie));
    if (*lista_zadan == NULL) {
        wprintf(L"Błąd w alokacji listy. \n");
        exit(0);
    }
    Zadanie nowe_zadanie = stworz_zadanie(nazwa, opis, data_wykonania);
    (*lista_zadan)[*rozmiar] = nowe_zadanie;
    (*rozmiar)++;
    system(wyczysc_ekran);
    wprintf(L"Utworzono nowe zadanie. \n\n");
}

// Funkcja do wyświetlania listy zadań w formie tabeli
void wyswietl_liste_zadan(Zadanie *lista_zadan, int rozmiar) {
    system(wyczysc_ekran);
    wchar_t naglowki[4][200] = {
        L"Lp.",
        L"Nazwa",
        L"Opis",
        L"Data wykonania"
    };
    if (rozmiar == 0) {
        wprintf(L"Brak zapisanych zadań.\n\n");
    } else {
        wstaw_linie();    
        wprintf(L"|%-3ls|%-30ls|%-100ls|%-14ls| \n", naglowki[0], naglowki[1], naglowki[2], naglowki[3]);
        wstaw_linie();
        for (int i = 0; i < rozmiar; i++) {
            wprintf(L"|%-3d|%-30ls|%-100ls|%-14ls| \n", i + 1, lista_zadan[i].nazwa, lista_zadan[i].opis, lista_zadan[i].data_wykonania);
            wstaw_linie();
        }
        printf("\n");
    }
}

// Funkcja do usuwania zadania z listy
void usun_zadanie(Zadanie **lista_zadan, int *rozmiar) {
    int index;
    Zadanie *nowa_lista_zadan;
    if (*rozmiar == 0) {
        system(wyczysc_ekran);
        wprintf(L"Nie można usunąć zadania z pustej listy zadań. \n\n");
        return;
    }
    wprintf(L"Podaj numer zadania, które chcesz usunąć: \n");
    scanf("%d", &index);
    if (index > (*rozmiar) || index < 0) {
        system(wyczysc_ekran);
        wprintf(L"Nie można usunąć zadania o nieistniejącym numerze. \n\n");
        return;
    }
    nowa_lista_zadan = (Zadanie*)malloc((*rozmiar - 1) * sizeof(Zadanie));
    if (nowa_lista_zadan == NULL) {
        wprintf(L"Problem z alokacją nowej listy. \n");
        exit(1);
    }
    index -= 1;
    int j = 0;
    for (int i = 0; i < *rozmiar; i++) {
        if (i != index) {
            nowa_lista_zadan[j] = (*lista_zadan)[i];
            j++;
        }
    }
    free(*lista_zadan);
    *lista_zadan = nowa_lista_zadan;
    (*rozmiar)--;
    system(wyczysc_ekran);
    wprintf(L"Zadanie zostało usunięte. \n\n");
}

// Funkcja do usuwania wszystkich zadań
void usun_wszystkie_zadania(Zadanie **lista_zadan, int *rozmiar) {
    system(wyczysc_ekran);
    if (*lista_zadan != NULL) {
        free(*lista_zadan);
        *lista_zadan = NULL;
    }
    (*rozmiar) = 0;
    wprintf(L"Usunięto wszystkie zadania. \n");
}

// Funkcja główna
int main() {
    ustaw_komende_do_czyszczenia();
    setlocale(LC_CTYPE, "pl_PL.UTF-8"); 
    system(wyczysc_ekran);
    Zadanie *lista_zadan = NULL;
    int rozmiar = 0;
    int wybor;

    wprintf(L"ToDoApp(Tasker)\n\n");
    do {
        wprintf(L"MENU\n");
        wprintf(L"1. Dodaj zadanie\n");
        wprintf(L"2. Wyświetl listę zadań\n");
        wprintf(L"3. Usuń zadanie\n");
        wprintf(L"4. Usuń wszystkie zadania\n");
        wprintf(L"5. Zakończ\n\n");
        wprintf(L"Wybierz opcję: ");
        scanf("%d", &wybor); 
        wyczysc_bufor();
        
        switch (wybor) {
            case 1:
                dodaj_zadanie_do_listy(&lista_zadan, &rozmiar);
                break;
            case 2:
                wyswietl_liste_zadan(lista_zadan, rozmiar);
                break;
            case 3:
                usun_zadanie(&lista_zadan, &rozmiar);
                break;
            case 4:
                usun_wszystkie_zadania(&lista_zadan, &rozmiar);
                break;
            case 5:
                break;
            default:
                wprintf(L"Nieznana opcja! \n");
                break;
        }
    } while (wybor != 5);
    return 0;
}
#include<stdio.h>
#include<string.h>

struct Zadanie
{
    double id;
    char nazwa[50];
    char opis[200];
    char data_wykonania[10];
};
struct Zadanie create_zadanie(char nazwa[50], char opis[200], char data_wykonania[10]){
    struct Zadanie nowe_zadanie;
    strcpy(nowe_zadanie.nazwa, nazwa);
    strcpy(nowe_zadanie.opis, opis);
    strcpy(nowe_zadanie.data_wykonania, data_wykonania);
    return nowe_zadanie;
};

int main(){
    struct Zadanie lista_zadan[10];
    struct Zadanie zad = create_zadanie("trying", "opis", "data");
    printf(zad.nazwa);
    printf("hello world");
    getchar();
}
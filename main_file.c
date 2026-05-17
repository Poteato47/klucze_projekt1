#include <stdio.h>
#include <stdbool.h>
#include <unistd.h> // function for including delays, making the program "sleep" for a period of time!
#include <curl/curl.h> // library for connecting with apis/sites

// URL FROM MOCK.API - constant definiton (link)
#define API_URL "https://6a09e244e7e3f433d4838ad5.mockapi.io/api/base/status/1"

void send_status_to_cloud(bool status) {
    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL); // gdzie wyslac - miejsce docelowe (link)
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); // metoda - put (aktualizacja)

        char json_data[64]; // prealokacja pamieci na 64 miejsca
        snprintf(json_data, sizeof(json_data), "{\"is_green\": %s}", status ? "true" : "false");
        // snprintf - funkcja do sklejania tekstu
        // odwolanie do zmiennej, ustawienie limitu, wybór danej z api => predykat
        
        struct curl_slist *headers = NULL; // prealokacja pointera
        headers = curl_slist_append(headers, "Content-Type: application/json"); // definicja typu pliku json
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); // dodanie typu json do curl_easy_setopt()
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data); // dodaje dane do curl_easy_setopt() (uchwyt)

        CURLcode res = curl_easy_perform(curl); // wyslanie danej do api (mockapi) res => odpowiedz serwera
        if(res != CURLE_OK) {
            fprintf(stderr, "Błąd wysyłania: %s\n", curl_easy_strerror(res));
        } else {
            printf("[LOG] Wysłano do chmury: %s\n", status ? "TRUE" : "FALSE");
        }

        curl_slist_free_all(headers); // sprzatanie pamieci, kasowanie naglowkow 
        curl_easy_cleanup(curl); // i obiekt polaczenia
    }
}

int main(void) {
    curl_global_init(CURL_GLOBAL_ALL); // preparation of net library
    bool is_green = false;

    printf("Start programu. Symulacja zmiany przycisku co 5 sekund...\n");

    while(1) {
        is_green = !is_green; // Boolean value inversion (veto)
        send_status_to_cloud(is_green); // data send
        sleep(5); // Czekaj 5 sekund
    }

    curl_global_cleanup();
    return 0;
}

// #include <stdio.h>
// #include <stdbool.h>
// //#include "simple_site.c"

// extern bool is_green;
// int site();

// int main(void)
// {
//     printf("Hello World!\n");

//     return site();
// }

// // unnecessary comment
// // unnecessary comment 2
// // unnecessary comment 3
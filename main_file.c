#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <curl/curl.h> 

// !!! TUTAJ WPISZESZ SWÓJ URL Z MOCKAPI !!!
#define API_URL "https://6a09e244e7e3f433d4838ad5.mockapi.io/api/base/status/1"

void send_status_to_cloud(bool status) {
    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, API_URL);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

        char json_data[64];
        snprintf(json_data, sizeof(json_data), "{\"is_green\": %s}", status ? "true" : "false");
        
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        CURLcode res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "Błąd wysyłania: %s\n", curl_easy_strerror(res));
        } else {
            printf("[LOG] Wysłano do chmury: %s\n", status ? "TRUE" : "FALSE");
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

int main(void) {
    curl_global_init(CURL_GLOBAL_ALL);
    bool is_green = false;

    printf("Start programu. Symulacja zmiany przycisku co 5 sekund...\n");

    while(1) {
        is_green = !is_green; // Odwracamy wartość logiczną
        send_status_to_cloud(is_green);
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
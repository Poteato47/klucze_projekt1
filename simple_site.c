#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>

// --- ZMIENNA GLOBALNA ---
// Zmień na 'false', aby strona stała się czerwona
bool is_green = false; 
// ------------------------

int site() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = 8080;

    // 1. Tworzenie gniazda (socket)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Błąd tworzenia gniazda");
        exit(EXIT_FAILURE);
    }

    // 2. Konfiguracja opcji gniazda, aby port się nie blokował
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. Bindowanie gniazda do portu 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Błąd bindowania (bind failed)");
        exit(EXIT_FAILURE);
    }

    // 4. Rozpoczęcie nasłuchiwania
    if (listen(server_fd, 3) < 0) {
        perror("Błąd nasłuchiwania (listen failed)");
        exit(EXIT_FAILURE);
    }

    printf("Serwer uruchomiony! Wejdz w przegladarce na adres: http://localhost:%d\n", port);
    printf("Aktualny kolor strony: %s\n", is_green ? "ZIELONY" : "CZERWONY");
    printf("Nacisnij Ctrl+C, aby wylaczyc serwer.\n\n");

    // Nieskończona pętla serwera - czeka na wejścia z przeglądarki
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Błąd akceptacji polaczenia");
            continue;
        }

        // Odczyt zapytania z przeglądarki (ignorujemy treść, by uprościć kod)
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024);

        // --- LOGIKA KOLORU ---
        // Na podstawie zmiennej globalnej ustalamy kolor w kodzie CSS
        const char* bgcolor = is_green ? "green" : "red";
        const char* bool_text = is_green ? "TRUE" : "FALSE";

        // Budowanie surowej odpowiedzi HTTP oraz kodu HTML
        char response[2048];
        snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n\r\n" // Dwie puste linie oznaczają koniec nagłówków HTTP
            "<!DOCTYPE html>"
            "<html>"
            "<head><title>Strona w C</title></head>"
            "<body style=\"background-color: %s; color: white; text-align: center; font-family: sans-serif; padding-top: 20vh;\">"
            "<h1>Witaj!</h1>"
            "<h2>Ta strona jest serwowana prosto z programu napisanego w jezyku C!</h2>"
            "<p>Zmienna globalna 'is_green' wynosi: <strong>%s</strong></p>"
            "</body>"
            "</html>",
            bgcolor, bool_text);

        // Wysłanie odpowiedzi do przeglądarki i zamknięcie połączenia
        send(new_socket, response, strlen(response), 0);
        close(new_socket);
    }

    return 0;
}
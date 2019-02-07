#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "jsmn/jsmn.h"
#include "main.h"

#define URL "https://data.culture.gouv.fr/api/datasets/1.0/search/?q=Paris"
#define PATH "../data.json"

int main() {
    curlWrite();
    jsonParser();
    return 0;
}

void curlWrite(void) {
    CURL *curl;
    FILE *ptr = fopen(PATH, "w");
    CURLcode res;

    curl = curl_easy_init(); //Initialization

    if (ptr == NULL) { //Check if the file is created
        printf("Error, creating file failed");
        exit(0);
    }
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL); //Get the data from the API
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, ptr); //Write the data in a file
        res = curl_easy_perform(curl); //Return OK(0) if there is no error
        if (res != CURLE_OK) { //Check errors
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl); //Close the connection
    }
    fclose(ptr);
}

//WIP
void jsonParser(void) {
    jsmn_parser parser;
    jsmntok_t tokens[10];
    jsmn_init(&parser);
    FILE *ptr = fopen(PATH, "r");
    fseek(ptr, 0, SEEK_SET);

    if (ptr == NULL) {
        printf("Error, reading file failed");
        exit(0);
    }

    jsmn_parse(&parser, ptr, strlen(ptr), tokens, 10);

}


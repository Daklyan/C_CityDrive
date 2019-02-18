#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "cJSON/cJSON.c"
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
        fprintf(stderr, "Error, creating file failed");
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
    string fileData;
    cJSON *dataJson = NULL;
    cJSON *title = NULL;
    int i = 0;

    FILE *ptr = fopen(PATH, "r");

    if (ptr == NULL) {
        fprintf(stderr, "Error, reading file failed");
        exit(0);
    }

    fileData = reader(ptr);

    dataJson = cJSON_Parse(fileData);

    char *errorPtr = cJSON_GetErrorPtr();
    if (errorPtr != NULL) {
        fprintf(stderr, "Error: %s:\n", errorPtr);
        exit(0);
    }
    /////////////////////TEST FIELD///////////////////////////
    title = cJSON_GetObjectItemCaseSensitive(dataJson,"title");
    for(;i<strlen(fileData);i++){
        if(cJSON_IsString(title) && (title->valuestring != NULL)){
            printf("Title : %s\n",title->valuestring);
        }
    }
    ///////////////////////////////////////////////////////////
    free(fileData);
    fclose(ptr);
}

string reader(FILE *ptr) { //Read a file and put in a char * var, needs a free() after using it
    long size = sizer(ptr);

    if (ptr == NULL) {
        fprintf(stderr, "Error, non valid pointer");
        exit(0);
    }
    string fileData = malloc(size + 1);
    fread(fileData, size, 1, ptr);
    return fileData;
}

long sizer(FILE *ptr) { //Get the size of a FILE
    long size;
    fseek(ptr, 0, SEEK_END);
    size = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    return size;
}


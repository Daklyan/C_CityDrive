#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "cJSON/cJSON.c"
#include "main.h"

#define URL "https://data.culture.gouv.fr/api/datasets/1.0/search/?q=Paris"
#define PATH "../data.json"

int main(int argc, char **argv[]) {
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

    FILE *ptr = fopen(PATH, "r");

    if (ptr == NULL) {
        fprintf(stderr, "Error, reading file failed");
        exit(0);
    }

    fileData = reader(ptr);

    dataJson = cJSON_Parse(fileData);

    const char *errorPtr = cJSON_GetErrorPtr();
    if (errorPtr != NULL) {
        fprintf(stderr, "Error: %s:\n", errorPtr);
        exit(0);
    }
    /////////////////////TEST FIELD///////////////////////////
    parseObject(dataJson);
    ///////////////////////////////////////////////////////////
    cJSON_Delete(dataJson);
    free(fileData);
    fclose(ptr);
}

void parseObject(cJSON *object) {
    {
        cJSON *title = NULL;
        cJSON *description = NULL;
        cJSON *domain = NULL;

        int i = 0;
        int j = 0;

        cJSON *item = cJSON_GetObjectItem(object, "datasets");
        for (; i < cJSON_GetArraySize(item); i++) {
            cJSON *test = cJSON_GetArrayItem(item, i);
            for (; j < cJSON_GetArraySize(test); j++) {
                cJSON *subitem = cJSON_GetArrayItem(test, j);
                title = cJSON_GetObjectItem(subitem, "title");
                description = cJSON_GetObjectItem(subitem, "description");
                domain = cJSON_GetObjectItem(subitem, "domain");
                if (title != NULL) printf("%s\n", title->valuestring);
                if (description != NULL) printf("%s\n", description->valuestring);
                if (domain != NULL) printf("%s\n", domain->valuestring);
            }
        }

    }
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

void doit(string text) {
    string out = NULL;
    cJSON *json = NULL;

    json = cJSON_Parse(text);
    if (!json) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    } else {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        printf("%s\n", out);
        free(out);
    }
}

long sizer(FILE *ptr) { //Get the size of a FILE
    long size;
    fseek(ptr, 0, SEEK_END);
    size = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    return size;
}


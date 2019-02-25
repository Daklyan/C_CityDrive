#ifdef _WIN64
#include <windows.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <gtk-2.0/gtk/gtk.h>
#include "cJSON/cJSON.h"
#include "cJSON/cJSON.c"
#include "main.h"

#define URL "https://data.culture.gouv.fr/api/datasets/1.0/search/?q=Paris"
#define PATH "../data.json"
#define LOGO "../citydrive.png"

const GtkWidget *mainwin;

int main(int argc, char **argv[]) {
    GtkWidget *button;
    GtkWidget *halign;
    GdkPixbuf *icon;

    gtk_init(&argc, &argv);
    mainwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(mainwin), "City Drive");
    gtk_window_set_default_size(GTK_WINDOW(mainwin), 800, 600);
    gtk_window_set_position(GTK_WINDOW(mainwin), GTK_WIN_POS_CENTER);

    jsonParser();

    button = gtk_button_new_with_label("Refresh Informations");
    gtk_widget_set_tooltip_text(button, "Refresh the informations of the API");


    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK(buttonClicked), NULL);

    halign = gtk_alignment_new(0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(halign), button);
    gtk_container_add(GTK_CONTAINER(mainwin), halign);

    icon = createPixbuf(LOGO);
    gtk_window_set_icon(GTK_WINDOW(mainwin), icon);

    gtk_widget_show_all(mainwin);
    g_object_unref(icon);
    gtk_main();
    jsonParser();
    return 0;
}

void buttonClicked(void) {
    g_print("click");
    curlWrite();
}

GdkPixbuf *createPixbuf(const gchar *logoname) {
    GdkPixbuf *pixbuf;
    GError *error = NULL;
    pixbuf = gdk_pixbuf_new_from_file(logoname, &error);

    if (!pixbuf) {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }
    return pixbuf;
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

    const string errorPtr = cJSON_GetErrorPtr();
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

    cJSON *title = NULL;
    cJSON *description = NULL;
    cJSON *domain = NULL;
    cJSON *publisher = NULL;
    int i;
    int j;

    cJSON *item = cJSON_GetObjectItem(object, "datasets"); //Get in the object datasets in the JSON file
    printf("Size: %d\n", cJSON_GetArraySize(item));
    for (i = 0; i < cJSON_GetArraySize(item); i++) {
        cJSON *metas = cJSON_GetArrayItem(item, i);
        for (j = 0; j < cJSON_GetArraySize(metas); j++) { //Seek the informations we need
            cJSON *subitem = cJSON_GetArrayItem(metas, j);
            title = cJSON_GetObjectItem(subitem, "title");
            publisher = cJSON_GetObjectItem(subitem, "publisher");
            domain = cJSON_GetObjectItem(subitem, "domain");
            description = cJSON_GetObjectItem(subitem, "description");
            //Verify if null before print
            if (title != NULL) {
                printf("Title : %s\n", title->valuestring);
                //display(title->valuestring);
            }
            if (publisher != NULL) {
                printf("Publisher : %s\n", publisher->valuestring);
                //display(publisher->valuestring);
            }
            if (domain != NULL) {
                printf("Domain : %s\n", domain->valuestring);
                //display(domain->valuestring);
            }
            if (description != NULL) {
                printf("Description : %s\n", description->valuestring);
                //display(description->valuestring);
            }
        }
    }
}

void display(string text) {
    gchar *convertText = NULL;
    GtkWidget *label = NULL;
    GtkWidget *halignOth = NULL;
    convertText = g_locale_from_utf8(text, -1, NULL, NULL, NULL);
    label = gtk_label_new(convertText);
    g_free(convertText);
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    halignOth = gtk_alignment_new(0, 0, 0, 0);
    gtk_container_add(GTK_CONTAINER(halignOth), label);
    gtk_container_add(GTK_CONTAINER(mainwin), halignOth);
    //gtk_container_add(GTK_CONTAINER(mainwin),label);
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


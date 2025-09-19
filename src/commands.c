#include "commands.h"
#include "config.h"
#include <cjson/cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

// Trim leading/trailing whitespace
char* trim(char* str) {
    char* end;
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;
    return str;
}

// Lookup movie in JSON catalog
char* get_url_from_catalog(const char* movie) {
    FILE* f = fopen("movies_catalog.json", "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* data = malloc(len + 1);
    if (!data) {
        fclose(f);
        return NULL;
    }

    fread(data, 1, len, f);
    data[len] = 0;
    fclose(f);

    cJSON* root = cJSON_Parse(data);
    if (!root) {
        free(data);
        return NULL;
    }

    cJSON* movie_item = cJSON_GetObjectItem(root, movie);
    if (!movie_item) {
        cJSON_Delete(root);
        free(data);
        return NULL;
    }

    cJSON* url_item = cJSON_GetObjectItem(movie_item, "url");
    if (!url_item || !cJSON_IsString(url_item)) {
        cJSON_Delete(root);
        free(data);
        return NULL;
    }

    static char url[8192];
    strcpy(url, url_item->valuestring);

    cJSON_Delete(root);
    free(data);
    return url;
}

// Create folder if it doesn't exist
void ensure_folder_exists(const char* folder) {
    struct stat st = {0};
    if (stat(folder, &st) == -1) {
        mkdir(folder, 0755);
        printf("Created folder: %s\n", folder);
    }
}

// Install movie
void install_movie(const char* input) {
    char folder[256];
    strcpy(folder, get_folder());
    ensure_folder_exists(folder);

    const char* url = input;

    // If input is not a URL or magnet link, look it up in JSON catalog
    if (strncmp(input, "http", 4) != 0 && strncmp(input, "magnet:", 7) != 0) {
        url = get_url_from_catalog(input);
        if (!url) {
            printf("Movie not found in catalog. Provide a full URL or magnet link.\n");
            return;
        }
    }

    char command[8192];
    sprintf(command, "aria2c -d \"%s\" \"%s\"", folder, url);
    printf("Downloading %s to folder %s...\n", input, folder);
    system(command);
}

// List downloaded movies
void list_movies() {
    char folder[256];
    strcpy(folder, get_folder());
    ensure_folder_exists(folder);

    DIR* dir = opendir(folder);
    if (!dir) {
        printf("Download folder '%s' does not exist.\n", folder);
        return;
    }

    struct dirent* entry;
    int found = 0;
    printf("Movies in %s:\n", folder);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            printf(" - %s\n", entry->d_name);
            found = 1;
        }
    }
    if (!found) printf(" (No movies found)\n");
    closedir(dir);
}

// Remove a downloaded movie
void remove_movie(const char* movie) {
    char folder[256];
    strcpy(folder, get_folder());

    char path[512];
    sprintf(path, "%s/%s", folder, movie);

    if (remove(path) == 0) {
        printf("Removed %s\n", movie);
    } else {
        printf("Failed to remove %s. Make sure the filename matches exactly.\n", movie);
    }
}

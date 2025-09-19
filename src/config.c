#include "config.h"
#include <stdio.h>
#include <string.h>

#define CONFIG_FILE "./config.txt"

void set_folder(const char* path) {
    FILE* f = fopen(CONFIG_FILE, "w");
    if (f) {
        fprintf(f, "%s\n", path);
        fclose(f);
        printf("Folder set to %s\n", path);
    } else {
        printf("Failed to set folder\n");
    }
}

char* get_folder() {
    static char folder[256];
    FILE* f = fopen(CONFIG_FILE, "r");
    if (f && fgets(folder, sizeof(folder), f)) {
        folder[strcspn(folder, "\n")] = 0; // remove newline
        fclose(f);
        return folder;
    }
    return "movies"; // default folder
}

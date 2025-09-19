#include "config.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: moviebrew <command> [args]\n");
        printf("Commands:\n");
        printf("  set-folder <path>\n");
        printf("  install <movie_name_or_url>\n");
        printf("  list\n");
        printf("  remove <movie_file>\n");
        return 0;
    }

    if (strcmp(argv[1], "set-folder") == 0 && argc >= 3) {
        set_folder(argv[2]);
    } else if (strcmp(argv[1], "install") == 0 && argc >= 3) {
        install_movie(argv[2]);
    } else if (strcmp(argv[1], "list") == 0) {
        list_movies();
    } else if (strcmp(argv[1], "remove") == 0 && argc >= 3) {
        remove_movie(argv[2]);
    } else {
        printf("Unknown command or missing argument.\n");
    }

    return 0;
}

/*
 * main.c  –  Mini Student Shell
 *
 * TODO: Implement admin_shell and client_shell.
 *
 * Build:
 *   make admin   →  admin_shell  (compiled with -DADMIN_MODE)
 *   make client  →  client_shell (compiled with -DCLIENT_MODE)
 *
 * Usage:
 *   ./admin_shell [students.csv]
 *   ./admin_shell -f commands.txt [students.csv]
 *   ./client_shell [students.csv]
 *   ./client_shell -f commands.txt [students.csv]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add your own header includes here */
#include "student.h"
#include "file_io.h"
#include "command.h"


/* ---------------------------------------------------------------
 * TODO: Implement the interactive shell loop.
 *   - Print a prompt and read a line from stdin.
 *   - Parse the line into a command and arguments.
 *   - Dispatch to the appropriate handler function.
 *   - Loop until the user types "exit" or EOF.
 * --------------------------------------------------------------- */
void run_shell(const char *csv_path) {
    /* TODO */
    Student* head = load_students(csv_path);
    char line[256];

    while(1){
        #ifdef ADMIN_MODE
            printf("admin> ");
        #else
            printf("client> ");
        #endif

        if(fgets(line, sizeof(line), stdin) == NULL) break;

        line[strcspn(line, "\n")] = '\0';

        if(strlen(line) == 0) continue;

        char* cmd = strtok(line, " ");
        char* args = strtok(NULL, "");

        int found = 0;

        for(int i = 0; i < get_cmd_count(); i++){
            if(strcmp(cmd, commands[i].name) == 0){
                ShellResult result = commands[i].handler(args, &head);
                found = 1;

                if(result == SHELL_EXIT){
                    free_students(head);
                    return;
                }

                break;;
            }
        }
        if(found != 1){
            printf("Error: unknown command\n");
        }
    }
    free_students(head);
}

/* ---------------------------------------------------------------
 * TODO: Implement batch mode – read commands from a file.
 *   - Open cmd_file for reading.
 *   - Execute each line as a command (same logic as run_shell).
 *   - Close the file when done.
 * --------------------------------------------------------------- */
void run_command_file(const char *cmd_file, const char *csv_path) {
    /* TODO */ 
    FILE* fp = fopen(cmd_file, "r");

    if(fp == NULL){
        printf("Error: cannot open command file\n");
        return;
    }

    Student* head = load_students(csv_path);
    char* line[256];

    while(fgets(line, sizeof(line), fp)){
        line[strcspn(line, "\n")] = '\0';

        if(strlen(line) == 0) continue;

        char* cmd = strtok(line, " ");
        char* args = strtok(NULL, "");

        int found = 0;

        for(int i = 0; i < get_cmd_count(); i++){
            if(strcmp(cmd, commands[i].name) == 0){
                ShellResult result = commands[i].handler(args, &head);
                found = 1;

                if(result == SHELL_EXIT){
                    free_students(head);
                    fclose(fp);
                    return;
                }

                break;;
            }
        }
        if(found != 1){
            printf("Error: unknown command\n");
        }
    }
    free_students(head);
    fclose(fp);
}

int main(int argc, char *argv[]) {
    const char *csv_path  = "students.csv"; /* default CSV file */
    const char *cmd_file  = NULL;           /* -f <file> argument */

    /* TODO: Parse command-line arguments.
     *   Supported flags:
     *     -f <file>   run commands from <file> instead of stdin
     *   Remaining positional argument (if any): path to students CSV.
     *
     *   Example parsing skeleton:
     *
     *   for (int i = 1; i < argc; i++) {
     *       if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
     *           cmd_file = argv[++i];
     *       } else {
     *           csv_path = argv[i];
     *       }
     *   }
     */
    (void)argc;
    (void)argv;

#ifdef ADMIN_MODE
    /* Admin shell: supports add, delete, update, save, load, sort, list, find, help, exit */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#elif defined(CLIENT_MODE)
    /* Client shell: supports find, list, help, exit  (read-only) */
    if (cmd_file) {
        run_command_file(cmd_file, csv_path);
    } else {
        run_shell(csv_path);
    }

#else
//#error "Define either -DADMIN_MODE or -DCLIENT_MODE when compiling."
#endif

    return 0;
}

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
    #ifdef ADMIN_MODE
        printf("[Admin Program]\n");
    #else
        printf("[Client Program]\n");
    #endif
    Student* head = load_students(csv_path);
    printf("Loaded %d students from %s.\n\n", num_students(head), csv_path);
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
            if(strcmp(cmd, commands[i].name) == 0){//명령어 비교
                ShellResult result = commands[i].handler(args, &head);
                found = 1;//커맨드 수행 확인

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
    #ifdef ADMIN_MODE
        printf("[Admin Program]\n");
    #else
        printf("[Client Program]\n");
    #endif
    FILE* fp = fopen(cmd_file, "r");

    //비어있는지 확인
    if(fp == NULL){
        printf("Error: cannot open command file\n");
        return;
    }

    Student* head = load_students(csv_path);
    printf("Loaded %d students from %s.\n\n", num_students(head), csv_path);
    char line[256];
    char index[256];
    int fileCount = 1;

    while(fgets(line, sizeof(line), fp)){
        
        line[strcspn(line, "\n")] = '\0';
        strcpy(index, line); //출력용으로 복사
        if(strlen(line) == 0) continue; //빈줄 무시

        char* cmd = strtok(line, " ");
        char* args = strtok(NULL, "");
        if(cmd[0] == '#') continue; //#으로 시작하면 주석으로 처리

        int found = 0;
        for(int i = 0; i < get_cmd_count(); i++){
            if(strcmp(cmd, commands[i].name) == 0){
                printf("[Command file: %d] %s\n", fileCount, index);
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
            printf("[Command file: %d]\nError: unknown command", fileCount);
            printf("Skipped line %d\n", fileCount);
        }
        fileCount++;
        printf("\n");
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
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-f") == 0 && i + 1 < argc){
            cmd_file = argv[++i];
        }else{
        csv_path = argv[i];
        }
    }

    if(csv_path == NULL){
        #ifdef ADMIN_MODE
            printf("Usage: ./admin_shell <csv_file> [-f command_file]\n");
        #else
            printf("Usage: ./client_shell <csv_file> [-f command_file]\n");
        #endif
    }

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

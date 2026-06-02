/* 명령어 파싱과 command handler 구현 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "file_io.h"
#include "student.h"

//command: name handler usage description
#ifdef ADMIN_MODE
Command commands[] = {
    {"save",   handle_save,   "save",                    "Save students to CSV"},
    {"reload", handle_reload, "reload",                  "Reload students from CSV"},
    {"add",    handle_add,    "add <id> <name> <score>", "Add a studnet"},
    {"delete", handle_delete, "delete <id>",             "Delete a student"},
    {"update", handle_update, "update <id> <score>",     "Update studnet score"},
    {"find",   handle_find,   "find <id>",               "Find student"},
    {"list",   handle_list,   "list",                    "List students"},
    {"stats",  handle_stats,  "stats",                   "Show statistics"},
    {"help",   handle_help,   "help",                    "Show help"},
    {"clear",  handle_clear,  "clear",                   "Clear screen"},
    {"exit",   handle_exit,   "exit",                    "Exit shell"}
};
#endif

#ifdef CLIENT_MODE
Command commands[] = {
    {"reload", handle_reload, "reload",      "Reload students from CSV"},
    {"find",   handle_find,   "find <id>",   "Find student"},
    {"list",   handle_list,   "list",        "List students"},
    {"stats",  handle_stats,  "stats",       "Show statistics"},
    {"help",   handle_help,   "help",        "Show help"},
    {"clear",  handle_clear,  "clear",       "Clear screen"},
    {"exit",   handle_exit,   "exit",        "Exit shell"}
};
#endif

int get_cmd_count(){
    return sizeof(commands) / sizeof(commands[0]);
}


ShellResult handle_save(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}
ShellResult handle_reload(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

ShellResult handle_add(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}
ShellResult handle_delete(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}
ShellResult handle_update(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

ShellResult handle_find(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}
ShellResult handle_list(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}
ShellResult handle_stats(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

ShellResult handle_help(char* args, Student** head){
    (void)args;
    (void)head;

    printf("Commands\n");

    int count = get_cmd_count();

    for(int i = 0; i < count; i++){
        printf("%-25s %5s\n", commands[i].usage, commands[i].description);
    }

    return SHELL_OK;
}
ShellResult handle_clear(char* args, Student** head){
    (void)args;
    (void)head;
    printf("\033[2J\033[H");
    return SHELL_OK;
}
ShellResult handle_exit(char* args, Student** head){
    (void)args;
    (void)head;
    printf("GoodBye\n");
    return SHELL_EXIT;
}

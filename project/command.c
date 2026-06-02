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

//현재 linked list에 저장된 학생정보를 원래 CSV파일에 저장한다.
ShellResult handle_save(char* args, Student** head){
    (void)args;
    
    Student* cur = head;
    if(save_students("students.csv", cur)==0){
        printf("No student file");
        return SHELL_ERR_INVALID_ARGUMENT;
    }else{
        int count  = save_students("studendts.csv", cur);
        printf("Saved %d students to students.csv.", count);
        return SHELL_OK;
    }

}

//원래 CSV파일에서 학생정보를 다시 불러온다.
ShellResult handle_reload(char* args, Student** head){
    (void)args;
    
    if(load_students("students.csv")==NULL){
        printf("No students argument");
        return SHELL_ERR_INVALID_ARGUMENT;
    }else{
        head = load_students("students.csv");
        Student* cur = head;

        int count = 0;
        while(cur != NULL){
            cur = cur->next;
            count++;
        }
    
        printf("Reloaded %d students from students.csv.", count);
        return SHELL_OK;
    }
}

//새 학생정보를 추가한다.
ShellResult handle_add(char* args, Student** head){
    int id, score;
    char name[32];

    if(!args || sscanf(args, "%d %s %d", &id, name, &score) != 3){
        printf("Error: missing or invalid arguments\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if(score<0 || score>100){
        printf("Error: invalid score\n");
        return SHELL_ERR_INVALID_SCORE;
    }

    if(find_student(*head, id)){
        printf("Error: duplicated ID\n");
        return SHELL_ERR_DUPLICATE_STUDENT;
    }

    Student* newST = create_student(id, name, score);
    insert_student(head, newST);
    printf("Student added\n");
    return SHELL_OK;
}

//ID기준으로 학생정보를 삭제한다.
ShellResult handle_delete(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

//ID를 기준으로 학생의 점수를 수정한다.
ShellResult handle_update(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

//ID를 기준으로 학생정보를 검색한다.
ShellResult handle_find(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

//linked list에 저장된 모든 학생을 출력한다.
ShellResult handle_list(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

//간단한 통계를 출력한다.
ShellResult handle_stats(char* args, Student** head){
    (void)args;
    (void)head;
    return SHELL_OK;
}

//사용 가능한 명령어와 사용법을 출력한다.
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

//터미널 화면을 지운다
ShellResult handle_clear(char* args, Student** head){
    (void)args;
    (void)head;
    printf("\033[2J\033[H");
    return SHELL_OK;
}

//프로그램을 종료한다.
ShellResult handle_exit(char* args, Student** head){
    (void)args;
    (void)head;
    printf("GoodBye.\n");
    return SHELL_EXIT;
}

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
    #ifdef ADMIN_MODE
        return 11;
    #else
        return 7;
    #endif
}

char csvName[256] = "students.csv";
void reset_csvName(const char* name){
    strcpy(csvName, name);
}

//현재 linked list에 저장된 학생정보를 원래 CSV파일에 저장한다.
ShellResult handle_save(char* args, Student** head){
    (void)args;
    
    Student* cur = *head;
    int count  = save_students(csvName, cur);

    if(count<0){
        printf("No student file\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }else{
        printf("Saved %d students to %s.\n", count, csvName);
        return SHELL_OK;
    }

}

//원래 CSV파일에서 학생정보를 다시 불러온다.
ShellResult handle_reload(char* args, Student** head){
    (void)args;

    if(load_students(csvName)==NULL){
        printf("No students argument\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }else{
        free_students(*head);
        *head = load_students(csvName);
        Student* cur = *head;

        int count = 0;
        while(cur != NULL){
            cur = cur->next;
            count++;
        }
    
        printf("Reloaded %d students from %s.\n", count, csvName);
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

    if(id <= 0){
        printf("Error: invalid ID\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student* newST = create_student(id, name, score);
    insert_student(head, newST);
    printf("Student added\n");
    return SHELL_OK;
}

//ID기준으로 학생정보를 삭제한다.
ShellResult handle_delete(char* args, Student** head){
    int id;

    if(!args || sscanf(args, "%d", &id) != 1){
        printf("Error: invalid argument\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if(delete_student(head, id)){
        printf("Student deleted.\n");
        return SHELL_OK;
    }
    printf("Error: student not found.\n");
    return SHELL_ERR_STUDENT_NOT_FOUND;
    
}

//ID를 기준으로 학생의 점수를 수정한다.
ShellResult handle_update(char* args, Student** head){
    int id; 
    int score;

    if(!args || sscanf(args, "%d %d", &id, &score) != 2){
        printf("Error: invalid argument\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    if(score < 0 || score > 100){
        printf("Error: invalid score\n");
        return SHELL_ERR_INVALID_SCORE;
    }

    Student* s = find_student(*head, id);
    if(s == NULL){
        printf("Error: student not found\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }

    s->score = score;
    printf("Student updated.\n");
    return SHELL_OK;
}

//ID를 기준으로 학생정보를 검색한다.
ShellResult handle_find(char* args, Student** head){
    int id;

    if(!args || sscanf(args, "%d", &id) != 1){
        printf("Error: invalid argument\n");
        return SHELL_ERR_INVALID_ARGUMENT;
    }

    Student* s = find_student(*head, id);

    if(s == NULL){
        printf("Error: student not found\n");
        return SHELL_ERR_STUDENT_NOT_FOUND;
    }

    printf("ID: %d\n", s->id);
    printf("Name: %s\n", s->name);
    printf("Score: %d\n", s->score);

    return SHELL_OK;
}

//linked list에 저장된 모든 학생을 출력한다.
ShellResult handle_list(char* args, Student** head){
    (void)args;
    
    if(*head == NULL){
        printf("No students found.\n");
        return SHELL_OK;
    }

    Student* cur = *head;
    printf("ID       Name     Score\n");

    while(cur != NULL){
        printf("%-8d %-8s %-8d\n", cur->id, cur->name, cur->score);
        cur = cur->next;
    }
    return SHELL_OK;
}

//간단한 통계를 출력한다.
ShellResult handle_stats(char* args, Student** head){
    (void)args;
    int sum = 0;
    int count = 0;
    int max = -1;
    int min = 101;

    if(*head == NULL){
        printf("No student data available.\n");
        return SHELL_OK;
    }

    Student* cur = *head;
    while (cur != NULL){
        sum += cur->score;

        if(max < cur->score) max = cur->score;

        if(min > cur->score) min = cur->score;

        cur = cur->next;
        count++;
    }
    printf("Count: %d\nAverage: %.1f\nMax: %d\nMin: %d\n", count, (double)sum/count, max, min);
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

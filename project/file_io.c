/* CSV 파일 입출력 구현 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "student.h"

Student* load_students(const char* filename){
    FILE* fp = fopen(filename, "r");

    if(fp == NULL){
        printf("Error: cannot open file\n");
        return NULL;
    }

    Student* head = NULL;
    char line[100];
    fgets(line,sizeof(line), fp);

    while(fgets(line,sizeof(line), fp) != NULL){

        //strtok 문자열을 토큰화 해서 나누어 주는 메서드, CSV파일에 id,name,score로 저장됨
        char* id_s = strtok(line, ",");
        char* name = strtok(NULL, ",");
        char* score_s = strtok(NULL, ",");

        int id = atoi(id_s);
        int score = atoi(score_s);

        //한줄씩 읽어가면서 객체 생성하고 저장
        Student* new_student = create_student(id, name, score);
        insert_student(&head, new_student);//head주소 기준으로 학생 삽입
    }

    fclose(fp);
    return head;
}

int save_students(const char* filename, Student* head){
    FILE* fp = fopen(filename, "w");

    if(fp == NULL){
        printf("Error: cannot open file\n");
        return 0;
    }

    fprintf(fp, "id,name,score\n");//가장 윗줄에 적어줘야 할 것들

    //구조체에 있는 학생들을 저장
    Student* cur = head;
    while(cur != NULL){
        fprintf(fp, "%d,%s,%d", cur->id, cur->name, cur->score);
        cur = cur->next;
    }

    fclose(fp);
    return 1;
}

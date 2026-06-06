/*linked list 구현*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

Student* create_student(int id, const char* name, int score){
    Student* s = malloc(sizeof(Student));

    if(s == NULL) return NULL;

    s->id = id;
    strcpy(s->name, name);
    s->score = score;
    s->next = NULL;

    return s;
}

void insert_student(Student** head, Student* new_studnet){

    //리스트가 비어있는 경우
    if(*head == NULL){
        *head = new_studnet;
        return;
    }

    //새로운 포인터로 리스트 끝을 가리킬때 까지 이동시켜줌
    Student* cur = *head; //current
    while (cur->next != NULL)
    {   
        cur = cur->next;
    }

    cur->next = new_studnet; //마지막 뒤에 삽입
    
}

Student* find_student(Student* head, int id){
    Student* cur = head;

    //head부터 나아가면서 id를 찾음
    while (cur != NULL)
    {
        if(cur->id == id) return cur;
        else cur = cur->next;
    }
    
    return NULL;
}

int delete_student(Student** head, int id){
    Student* cur = *head;
    Student* prev = NULL;//삭제를 위한 이전노드 저장용 변수

    while(cur != NULL){
        //삭제할 노드를 찾은 경우
        if(cur->id == id){
            if(prev==NULL){
                *head = cur->next;
            }else{
                prev->next = cur->next;
            }

            free(cur);//삭제한 노드의 메모리를 날려줌
            return 1;//삭제 성공하면 1 반환
        }

        //삭제할 노드 찾기위해 계속 나아가기
        prev = cur;
        cur = cur->next;
    }
    return 0;//삭제 실패하면 0 반환
    
}

void free_students(Student* head){
    Student* cur = head;

    while(cur != NULL){
        Student* temp = cur;
        cur = cur->next;

        free(temp);
    }
}

int num_students(Student* head){
    Student* cur = head;
    int count = 0;

    while (cur != NULL){
        count++;
        cur = cur->next;
    }
    return count;
}
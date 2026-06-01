/*Student 구조체와 linked list 함수 선언*/
#ifndef STUDENT_H
#define STUDENT_H

typedef struct Student{
    int id;
    char name[32];
    int score;
    struct Student* next;
}Student;

//학생 구조체 생성
Student* create_student(int id, const char* name, int score);

//제일 뒤에 학생 삽입, head를 수정할 수도 있으니까 이중 포인터 사용
void insert_student(Student** head, Student* new_student);

//학생 찾기
Student* find_student(Student* head, int id);

//학생 삭제
int delete_student(Student** head, int id);

//전부 없애기
void free_students(Student* head);
#endif
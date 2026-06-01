/* CSV load/save 함수 선언 */
#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

//파일 읽어오기, 첫 번째 줄 제외하고 학생 구조체에 저장하기
Student* load_students(const char* filename);

//파일 저장하기
int save_students(const char* filename, Student* head);

#endif
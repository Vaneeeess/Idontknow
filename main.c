#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MAX_LINE 64
#define MAX 100
typedef struct _student {
  char* name;
  int id;
  struct _student* next;
} Student;

typedef struct _dorm {

  char* name;
  int dormNum;
  int numStudents;

  Student *studentList;
  struct _dorm* next;
} Dorm;


Dorm *dormListHead = NULL;

void addDorm(char* name, int dormNum);
void addStudent(char* name, int id);
void graduate(int id);
void move(char* name, int id, int dormNum);


void printStudentList(Student* listHead);
void printDorms();

void usage();
int readDormFile(char* dormFileName);
int readStudentFile(char* studentFileName);
void releaseMemory();

int main(int argc, char* argv[]){

  if(argc != 3) {
    usage();
    return -1;
  }

  readDormFile(argv[1]);
  readStudentFile(argv[2]);

  printDorms();

  releaseMemory();
  return 0;
}

void addDorm(char* name, int dormNum){
  Dorm* newDorm = (Dorm*)malloc(sizeof(Dorm));
  newDorm->name = strdup(name);
  newDorm->dormNum = dormNum;
  newDorm->numStudents = 0;
  newDorm->studentList = NULL; 
  

  newDorm->next = dormListHead;
  dormListHead = newDorm;
}

void addStudent(char* name, int id){

  Dorm* leastPopulatedDorm = NULL;
  int minStudents = MAX;
  
  for (Dorm* dorm = dormListHead; dorm != NULL; dorm = dorm->next) {
    if (dorm->numStudents < minStudents) {
      leastPopulatedDorm = dorm;
      minStudents = dorm->numStudents;
    }
  }
  

  Student* newStudent = (Student*)malloc(sizeof(Student));
  newStudent->name = strdup(name);
  newStudent->id = id;
  newStudent->next = NULL;
 
  if (leastPopulatedDorm->studentList == NULL) {
    leastPopulatedDorm->studentList = newStudent;
  } else {
    Student* currentStudent = leastPopulatedDorm->studentList;
    while (currentStudent->next != NULL) {
      currentStudent = currentStudent->next;
    }
    currentStudent->next = newStudent;
  }
  

  leastPopulatedDorm->numStudents++;
}

void graduate(int id){

  Dorm* currentDorm = NULL;
  Student* prevStudent = NULL;
  
  for (Dorm* dorm = dormListHead; dorm != NULL; dorm = dorm->next) {
    Student* currentStudent = dorm->studentList;
    while (currentStudent != NULL) {
      if (currentStudent->id == id) {
        currentDorm = dorm;
        break;
      }
      prevStudent = currentStudent;
      currentStudent = currentStudent->next;
    }
    if (currentDorm != NULL) break;
  }
  
  if (currentDorm == NULL) {
    printf("Student with ID %d not found\n", id);
    return;
  }
  
 
  Student* studentToRemove = currentDorm->studentList;
  
  if (prevStudent != NULL) {
    prevStudent->next = studentToRemove->next;
  } else {
    currentDorm->studentList = studentToRemove->next;
  }
  
  free(studentToRemove->name);
  free(studentToRemove);
  

  currentDorm->numStudents--;
}

void move(char* name, int id, int dormNum){

  Dorm* currentDorm = NULL;
  Student* prevStudent = NULL;
  
  for (Dorm* dorm = dormListHead; dorm != NULL; dorm = dorm->next) {
    Student* currentStudent = dorm->studentList;
    while (currentStudent != NULL) {
      if (currentStudent->id == id) {
        currentDorm = dorm;
        break;
      }
      prevStudent = currentStudent;
      currentStudent = currentStudent->next;
    }
    if (currentDorm != NULL) break;
  }
  
  if (currentDorm == NULL) {
    printf("Student with ID %d not found\n", id);
    return;
  }
  

  Student* studentToMove = currentDorm->studentList;
  
  if (prevStudent != NULL) {
    prevStudent->next = studentToMove->next;
  } else {
    currentDorm->studentList = studentToMove->next;
  }
  
  
  currentDorm->numStudents--;

  Dorm* newDorm = NULL;
  
  for (Dorm* dorm = dormListHead; dorm != NULL; dorm = dorm->next) {
    if (dorm->dormNum == dormNum) {
      newDorm = dorm;
      break;
    }
  }
  
  if (newDorm == NULL) {
    printf("Dorm with number %d not found\n", dormNum);
    return;
  }
  
  studentToMove->next = NULL;
  Student* tempStudent = newDorm->studentList;
  
  if (tempStudent == NULL) {
    newDorm->studentList = studentToMove;
  } else {
    while (tempStudent->next != NULL) {
      tempStudent = tempStudent->next;
    }
    tempStudent->next = studentToMove;
  }

  newDorm->numStudents++;
}

void usage(){

  fprintf(stderr, "Usage: dorm <dormFile> <studentFile>\n");
}


int readDormFile(char* dormFileName){

  char line[MAX_LINE];
  char name[MAX_LINE];
  char* str;
  int id;
  FILE* f;

  if(dormFileName == NULL) {
    printf("Dorm file name is invalid\n");
    return -1;
  }

  f = fopen(dormFileName, "r");
  if(f == NULL) {
    printf("Dorm file (%s) is not present\n", dormFileName);
    return -1;
  }

  while(( str = fgets(line, MAX_LINE, f)) != NULL) {

    int len = strlen(line);
    if(len > 1 && line[len-1] == '\n') {
      line[len-1] = '\0';
      len = strlen(line);
      if(len > 0) {
        sscanf(line, "%d %s",&id, name);
        addDorm(name, id);
      }
    }
  }
  fclose(f);
  return 0;
}

int readStudentFile(char* studentFileName){

  char line[MAX_LINE];
  char name[MAX_LINE];
  char* str;
  int id;
  FILE* f;

  if(studentFileName == NULL) {
    printf("Student file name is invalid\n");
    return -1;
  }

  f = fopen(studentFileName, "r");
  if(f == NULL) {
    printf("Student file (%s) is not present\n", studentFileName);
    return -1;
  }

  while(( str = fgets(line, MAX_LINE, f)) != NULL) {

    int len = strlen(line);
    if(len > 1 && line[len-1] == '\n') {
      line[len-1] = '\0';
      len = strlen(line);
      if(len > 0) {
        sscanf(line, "%d %s",&id, name);
        addStudent(name, id);
      }
    }
  }
  fclose(f);
  return 0;
}


void printStudentList(Student* listHead) {

  Student* stu;
  int i=0;
  for(stu = listHead; stu != NULL; stu = stu->next) {
    printf("\tStudent [%d]: %s (%d)\n", i, stu->name, stu->id);
    i++;
  }
}

void printDorms() {

  Dorm* dorm;
  for(dorm = dormListHead; dorm != NULL; dorm = dorm->next) {

    printf("Dorm [%d]: %s (%d)\n", dorm->dormNum, dorm->name, dorm->numStudents);
    printStudentList(dorm->studentList);
    printf("---------------------------------------------------------------------\n");
  }
}

void releaseStudentList(Student* studentList) {
  Student * stu = studentList;
  Student * tmp = stu;

  while(stu){
    free(stu->name);
    tmp = stu->next;
    free(stu);
    stu=tmp;
  }
}

void releaseMemory() {

  Dorm * dorm = dormListHead;
  Dorm * tmp = dorm;
  while(dorm) {

    releaseStudentList(dorm->studentList);
    free(dorm->name);
    tmp = dorm->next;
    free(dorm);
    dorm=tmp;
  }

}

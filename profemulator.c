#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h> 
#include <ctype.h>
#include <unistd.h>

struct student{
    int studentNum;
    int totalQuestions;
    int answered;
};

int totalStudents;
int officeLimits;
int inOffice = 0; 
sem_t readyForQuestion, questionWait, answerWait;
pthread_t professor;
pthread_t closeOffice;
pthread_mutex_t question_lock, office_lock; 
int currentStudent; 


void Professor();
void *StartProfessor();
void AnswerStart();
void AnswerDone();
void Student(int x);
void *StartStudent(void * student);
void QuestionStart(void * student);
void QuestionDone(void * student);
void EnterOffice(void * student);
void LeaveOffice(void * student);
bool isNumber(char* input);

void *CloseOfficeCheck() //P_Thread to check if the lab has been empty for a time, and closes the office if it has.
{
    int closeTimer = 0;

    while(true)
    {
        if(inOffice == 0)
        {
            closeTimer++;
            usleep(400000);
        }
        else
        {
            closeTimer = 0;
        }

        if(closeTimer >= 10)
        {
            printf("No more students left\nClosing Office.....\n");
            exit(0);
        }

    }
}

void Professor() {
    pthread_mutex_init(&question_lock, NULL);

    sem_init(&readyForQuestion,1,0);
    sem_init(&questionWait,1,0);
    sem_init(&answerWait,1,0);

    pthread_mutex_init(&office_lock, NULL); //Office Lock mutex

    pthread_create(&professor, NULL, StartProfessor, NULL);

}

void * StartProfessor() {
    while(1) {
      
        sem_post(&readyForQuestion); //signals students that the professor is ready for a question
        sem_wait(&questionWait);

        AnswerStart(); //Answers the question
        AnswerDone();
        sem_post(&answerWait); //informs the student that the question was answerd

    }
}


void AnswerStart() {
  
    printf("Professor starts to answer question for student %d.\n", currentStudent);
    usleep(rand() % 20000 + 10001);
}


void AnswerDone() {
  
    printf("Professor is done with answer for student %d.\n", currentStudent);
}



void Student(int x) {

    struct student * newStd = malloc(sizeof(struct student));
    newStd->studentNum = x;
    newStd->totalQuestions = (x % 4) + 1;
    newStd->answered = 0;
    
    pthread_t stack;

    
    pthread_create(&stack, NULL, (void *) StartStudent, (void *) newStd);
    
}

void *StartStudent(void * student) {
  
    struct student * std = student;

    
    pthread_mutex_lock(&office_lock); // Mutex locks the office
    while(inOffice >= officeLimits); //One student thread waits for another student to leave beffore enteringe

    EnterOffice(std);
    inOffice++;
    pthread_mutex_unlock(&office_lock); //Mutex unlocks letting another student enter position 1 of the queue

    printf("Students in office currently: %d\n", inOffice);
    
    while(std->totalQuestions > std->answered) {

        sem_wait(&readyForQuestion); //Waits to receive the signal to ask a question
        pthread_mutex_lock(&question_lock); //locks other students so they can't ask a question at the same time

	    currentStudent = std->studentNum;
        QuestionStart(std);

	    sem_post(&questionWait); //signals the professer that the question was asked
	    sem_wait(&answerWait); //waits to receive the answer


	    QuestionDone(std); 
	    pthread_mutex_unlock(&question_lock);//unlocks the question lock when done
	    std->answered++;


	    if(std->totalQuestions == std->answered) { //if the student is done asking questions, leaves the office

	        LeaveOffice(std);
	        inOffice--;
	        printf("Students in office currently: %d\n", inOffice);

       }
    }

    return 0;
}

void QuestionStart(void * student) {
  
    struct student * std = student;

    printf("Student %d asks a question.\n", std->studentNum);
    usleep(rand() % 20000 + 10001);
}


void QuestionDone(void * student) {
  
    struct student * std = student;
    
    printf("Student %d is satisfied.\n", std->studentNum);
}



void EnterOffice(void * student) {
  
   struct student * std = student;
  
   printf("Student %d enters the office.\n", std->studentNum);
}


void LeaveOffice(void * student) {
  
   struct student * std = student;
  
   printf("Student %d leaves the office.\n", std-> studentNum);
}


bool isNumber(char* input) { //Function to check if the arguments entered are numbers
  while(*input != 0) {
    if(isdigit(*input)) {
        return true;
     } else {
        return false;
     }
  }
  return false;
}
  
int main(int argc, char **argv) //First argument is the number of students. The second argument is the office size limit
{
    int totalStudents, i;

    if(argc <= 2)
    {
        printf("Invalid Number of Arguments (./profemulator <Num Students> <Office Limit>)\n");
        exit(0);
    }
    else
    {
        if(isNumber(argv[1]) && isNumber(argv[2]))
        {
            totalStudents = atoi(argv[1]);
            officeLimits = atoi(argv[2]);
            if(totalStudents <= 0 || officeLimits <= 0)
            {
                printf("Invalid Number of Students or Office Limit(Number must be > 0)\n");
                exit(0);
            }

        }
        else
        {
            printf("Invalid Entry Found (./profemulator <Num Students> <Office Limit>)\n");
            exit(0);
        }
    }

    Professor(); //starts the professor helper function

    pthread_create(&closeOffice, NULL, CloseOfficeCheck, NULL); //starts the CloseOfficeCheck Thread

    for(i = 0 ; i < totalStudents; i++) 
    { //Calls the Student helper function totalStudents times
       
        Student(i);
        
    }
    pthread_exit(NULL);
    


}


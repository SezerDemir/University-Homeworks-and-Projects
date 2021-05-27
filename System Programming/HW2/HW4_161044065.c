#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

// unnamed semaphores
sem_t sem_queue, sem_empty, sem_printf, sem_available, sem_look, sem_money, sem_students, sem_wait_main;

//-----------------------------------------//
//Queue functions
int front = 0;
int rear = -1;
int itemCount = 0;
int max_item = 0;
int max_size = 0;
int homeworks_done = 0;
int student_num = 0;
char input1_name[1000], input2_name[1000];
int *available;
int game_finished = 0;
char *queue_arr;
sig_atomic_t sig_f = 0;


static void SIGINT_Handler(int signum)
{
    sig_f = 1;
}

// returns 1 if queue is full
int isFull() {
    if(itemCount == max_size)
        return 1;
    return 0;
}

// returns 1 if queue is empty
int isEmpty() {
    if(itemCount <= 0)
        return 1;
    return 0;
}

int peek() {
   return queue_arr[front];
}

void insert(char data) {      
    queue_arr[++rear] = data;
    itemCount++;
}

char removeData() {
   char item = queue_arr[front++];
   if(front == max_size) {
      front = 0;
   }
   itemCount--;
   return item;  
}

struct Student
{
    char name[10000];
    int Q;
    int C;
    int S;
    long int money;
    long int total_m;
    int total_homework;
    int got_h;
    char homework;
    sem_t sem;
};
struct Student *students;

//-----------------------------------------//

long int total_money = 0;

void controlArguments(int, char **);
void createQueue(char **);
void createStudents(char **);
void *H_thread_function();
void clearStruct();
void *Solve_homework(void *num);

int main(int argc, char * argv[])
{
    // money assignment
    total_money = atoi(argv[3]);

    int money_over = 0, total_money_t = 0;
    pthread_t thread_h;
    controlArguments(argc, argv);
    createQueue(argv);
    createStudents(argv);
    total_money_t = total_money;
    available = (int*)calloc(student_num, sizeof(int));
    int s_sorted[student_num], s_sorted_index[student_num], c_sorted[student_num], c_sorted_index[student_num], q_sorted[student_num], q_sorted_index[student_num];

    // sigint handler init
    struct sigaction sa;
    sa.sa_handler = SIGINT_Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
    }

    // creates sorted arrays
    int min = students[0].C;    
    for(int i = 0; i < student_num; i++)
    {
        c_sorted_index[i] = i;
        c_sorted[i] = students[i].C;
    }
    for (int i = 0; i < student_num; i++) {     
        for (int j = i+1; j < student_num; j++) {     
           if(c_sorted[i] > c_sorted[j]) {    
               min = c_sorted[i];  
               c_sorted_index[i] = j;
               c_sorted_index[j] = i;
               c_sorted[i] = c_sorted[j];    
               c_sorted[j] = min;    
           }     
        }     
    } 

    // creates sorted arrays
    min = students[0].S;    
    for(int i = 0; i < student_num; i++)
    {
        s_sorted_index[i] = i;
        s_sorted[i] = students[i].S;
    }
    for (int i = 0; i < student_num; i++) {     
        for (int j = i+1; j < student_num; j++) {     
           if(s_sorted[i] < s_sorted[j]) {    
               min = s_sorted[i];  
               s_sorted_index[i] = j;
               s_sorted_index[j] = i;
               s_sorted[i] = s_sorted[j];    
               s_sorted[j] = min;    
           }     
        }     
    }

    // creates sorted arrays
    min = students[0].Q;    
    for(int i = 0; i < student_num; i++)
    {
        q_sorted_index[i] = i;
        q_sorted[i] = students[i].Q;
    }
    for (int i = 0; i < student_num; i++) {     
        for (int j = i+1; j < student_num; j++) {     
           if(q_sorted[i] < q_sorted[j]) {    
               min = q_sorted[i];  
               q_sorted_index[i] = j;
               q_sorted_index[j] = i;
               q_sorted[i] = q_sorted[j];    
               q_sorted[j] = min;    
           }     
        }     
    }            

    for(int i = 0; i < student_num; i++)
    {
        available[i] = 1;
    }

    // copies file names
    strcpy(input1_name, argv[1]);
    strcpy(input2_name, argv[2]);


    // semaphore initialization
    sem_init(&sem_queue, 0, 1);
    sem_init(&sem_printf, 0, 1);
    sem_init(&sem_empty, 0, 0);
    sem_init(&sem_available, 0, 1);
    sem_init(&sem_look, 0, 0);
    sem_init(&sem_money, 0, 1);
    sem_init(&sem_students, 0, 1);
    sem_init(&sem_wait_main, 0, 0);
    // creates hire-student threads
    pthread_t student_th[student_num];
    int id[student_num];
    for(int i = 0 ; i < student_num; i++)
    {
        id[i] = i;
        if(pthread_create(&student_th[i], NULL, Solve_homework, (void*)(id + i)) != 0)
            perror("Failed to create thread");
    }
    sem_wait(&sem_printf);
    printf("%d students-for-hire threads have been created.\n", student_num);
    fflush(stdout);
    sem_post(&sem_printf);
    for(int i = 0; i < student_num; i++)
    {
        sem_wait(&sem_printf);
        printf("%s %d %d %d\n", students[i].name, students[i].Q, students[i].S, students[i].C);
        fflush(stdout);
        sem_post(&sem_printf);        
    }

    // creates H thread
    if(pthread_create(&thread_h, NULL, H_thread_function, NULL) != 0)
        perror("Failed to create thread");
    if(pthread_detach(thread_h) != 0)
        perror("Error at detach segment");

    for(int i = 0 ; i < student_num * 2; i++)
    {
        sem_post(&sem_wait_main);
    }
    
    for(;student_num != 0;)
    {
        if(sig_f == 1)
            break;
        if( ((homeworks_done == 1 && isEmpty() == 1) || game_finished == 1))
        {
            sem_post(&sem_queue);
            game_finished = 1;
            break;
        }

        sem_wait(&sem_empty);
        sem_wait(&sem_queue);
        if((homeworks_done == 1 && isEmpty() == 1) || game_finished == 1)
        {
            sem_post(&sem_queue);
            game_finished = 1;
            break;
        }
        int enough = 0; 
        for(int i = 0; i < student_num; i++)
        {
            if(students[i].C <= total_money)
                enough = 1;
        }
        if(enough == 0)
        {
            money_over = 1;
            game_finished = 1;
            break;
        }
        char hw = '.';
        hw = removeData();
        if(sig_f == 1)
            break;
        sem_post(&sem_queue);     

        // distributes the homework to available student
        for(;;)
        {
            if(sig_f == 1)
                break;
            if(((homeworks_done == 1 && isEmpty() == 1 && hw == '.') || game_finished == 1))
            {
                sem_post(&sem_queue);
                game_finished = 1;
                break;
            }  
            int found = 0;
            // if homework has a 'C' priority
            if(hw == 'C')
            {
                enough = 0; 
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    if(students[i].C <= total_money)
                        enough = 1;
                }
                if(enough == 0)
                {
                    money_over = 1;
                    game_finished = 1;
                    break;
                }
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    sem_wait(&sem_available);
                    if(sig_f == 1)
                        break;
                    if(available[c_sorted_index[i]] == 1 && students[c_sorted_index[i]].C <= total_money)
                    {
                        available[c_sorted_index[i]] = 0;
                        sem_wait(&sem_students);
                        students[c_sorted_index[i]].homework = 'C';
                        // removes mones from pocket
                        sem_wait(&sem_money);
                        total_money -= students[c_sorted_index[i]].C;
                        students[c_sorted_index[i]].money = total_money;
                        students[c_sorted_index[i]].got_h = 1;
                        sem_post(&sem_students);
                        sem_post(&sem_money);
                        // awakes hired student
                        sem_post(&students[c_sorted_index[i]].sem);
                        found = 1;
                        sem_post(&sem_available);
                        break;
                    }
                    sem_post(&sem_available);
                }
                if(found == 0)
                {
                    sem_wait(&sem_look);
                }
            }
            // if homework has a 'S' priority
            else if(hw == 'S')
            {
                enough = 0; 
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    if(students[i].C <= total_money)
                        enough = 1;
                }
                if(enough == 0)
                {
                    money_over = 1;
                    game_finished = 1;
                    break;
                }
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    sem_wait(&sem_available);
                    if(available[s_sorted_index[i]] == 1 && students[s_sorted_index[i]].C <= total_money)
                    {
                        available[s_sorted_index[i]] = 0;
                        sem_wait(&sem_students);
                        students[s_sorted_index[i]].homework = 'S';
                        // removes mones from pocket
                        sem_wait(&sem_money);
                        total_money -= students[s_sorted_index[i]].C;
                        students[s_sorted_index[i]].money = total_money;
                        students[s_sorted_index[i]].got_h = 1;
                        sem_post(&sem_students);
                        sem_post(&sem_money);
                        // awakes hired student
                        sem_post(&students[s_sorted_index[i]].sem);
                        found = 1;
                        sem_post(&sem_available);
                        break;
                    }
                    sem_post(&sem_available);
                }
                if(found == 0)
                {
                    sem_wait(&sem_look);
                }
            }
            // if homework has a 'Q' priority
            else if(hw == 'Q')
            {
                enough = 0; 
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    if(students[i].C <= total_money)
                        enough = 1;
                }
                if(enough == 0)
                {
                    money_over = 1;
                    game_finished = 1;
                    break;
                }
                for(int i = 0; i < student_num; i++)
                {
                    if(sig_f == 1)
                        break;
                    sem_wait(&sem_available);
                    if(available[q_sorted_index[i]] == 1 && students[q_sorted_index[i]].C <= total_money)
                    {
                        available[q_sorted_index[i]] = 0;
                        sem_wait(&sem_students);
                        students[q_sorted_index[i]].homework = 'Q';
                        // removes mones from pocket
                        sem_wait(&sem_money);
                        total_money -= students[q_sorted_index[i]].C;
                        students[q_sorted_index[i]].money = total_money;
                        students[q_sorted_index[i]].got_h = 1;
                        sem_post(&sem_students);
                        sem_post(&sem_money);
                        // awakes hired student
                        sem_post(&students[q_sorted_index[i]].sem);
                        found = 1;
                        sem_post(&sem_available);
                        break;
                    }
                    sem_post(&sem_available);
                }
                if(found == 0 && sig_f != 1)
                {
                    sem_wait(&sem_look);
                }
            }
            if(found == 1 || sig_f == 1)
                break;
        }
        // if game is finished awakes all the hired students
        if(game_finished == 1 || sig_f == 1)
        {
            break;
        }
    } 
    if(sig_f == 1)
    {
        pthread_cancel(thread_h);
        for(int i = 0; i < student_num; i++)
        {
            pthread_cancel(student_th[i]);
        }
    }
    for(int i = 0; i < student_num; i++)
    {
        for(int j = 0; j < student_num; j++)
        {
            sem_post(&students[j].sem);
            sem_post(&sem_money);
        }
    }
    for(int i = 0; i < student_num; i++)
    {
        pthread_join(student_th[i], NULL);
    }

    if(money_over == 1 && sig_f != 1)
    {
        printf("Money is over, closing.\n");
        fflush(stdout);
    }
    else if(sig_f == 1)
    {
        printf("\nTermination signal received, closing.\n");
        fflush(stdout);
    }
    else if(student_num != 0)
    {
        printf("No more homeworks left or coming in, closing.\n");
        fflush(stdout);
    }
    else if( student_num == 0)
    {
        printf("There is no student in the input file, so no homework is solved.\n");
        fflush(stdout);
    }
    if(student_num != 0)
    {
        printf("Homeworks solved and money made by the students:\n");
        fflush(stdout);
    }

    for(int i=0 ; i < student_num; i++)
    {
        printf("%s %d %ld\n", students[i].name, students[i].total_homework, students[i].total_m);
        fflush(stdout);
    }
    int total_hom = 0; 
    int total_cost = 0;

    for(int i = 0 ; i < student_num; i++)
    {
        total_hom+= students[i].total_homework;
        total_cost+= students[i].total_homework * students[i].C;
    }

    printf("Total cost for %d homeworks %dTL\n", total_hom, total_cost);
    fflush(stdout);
    printf("Money left at G’s account: %dTL\n", total_money_t - total_cost);
    fflush(stdout);

    sem_destroy(&sem_queue);
    sem_destroy(&sem_printf);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_available);
    sem_destroy(&sem_look);
    sem_destroy(&sem_money);
    sem_destroy(&sem_students);
    sem_destroy(&sem_wait_main);
    free(queue_arr);
    free(available);
    clearStruct();

    exit(EXIT_SUCCESS);
}

void *Solve_homework(void *num)
{
    sem_wait(&sem_wait_main);
    int* id = (int*)num;
    int speed = 6 - students[*id].S;
    for(;;)
    {
        sem_wait(&sem_printf);
        printf("%s is waiting for a homework\n", students[*id].name);
        fflush(stdout);
        sem_post(&sem_printf);
        sem_wait(&students[*id].sem);
        if(game_finished == 1 && students[*id].got_h != 1)
            return 0;
        sem_wait(&sem_printf);
        printf("%s is solving homework %c for %d, H has %ldTL left.\n", students[*id].name, students[*id].homework, students[*id].C, students[*id].money);
        fflush(stdout);
        sem_post(&sem_printf);
        sleep(speed);

        sem_wait(&sem_students);
        students[*id].got_h = 3;
        students[*id].total_homework++;
        students[*id].total_m += students[*id].C;
        sem_post(&sem_students);

        sem_wait(&sem_available);
        available[*id] = 1;
        sem_post(&sem_available);
        sem_post(&sem_look);
        
    }
    return 0;
}

// function that h-thread executes
void *H_thread_function()
{
    sem_wait(&sem_wait_main);
    FILE * file;
    file = fopen(input1_name, "r");
    char c;
    int counter = 0;
    // runs until reach the end of the file or run out of money
    while (1) 
    {
        counter++;
        sem_wait(&sem_money);

        int enough = 0; 
        for(int i = 0; i < student_num; i++)
        {
            if(students[i].C <= total_money)
                enough = 1;
        }
        if(enough == 0)
        {
            sem_wait(&sem_printf);
            printf("H has no more money for homeworks, terminating.\n");
            fflush(stdout);
            sem_post(&sem_printf);
            sem_post(&sem_money);
            sem_post(&sem_empty);
            fclose(file);
            return NULL;
        }
        sem_post(&sem_money);
        sem_wait(&sem_queue);
        c = fgetc(file);
        while(c != 'C' && c != 'S' && c != 'Q' && c != EOF)
            c = fgetc(file);
        if(c == EOF)
        {
            if(counter == 1)
                sem_post(&sem_empty);
            homeworks_done = 1;
            sem_post(&sem_queue);
            break;
        }
        // prints current amount of money
        sem_wait(&sem_money);
        sem_wait(&sem_printf);
        printf("H has a new homework %c; remaining money is %ldTL\n", c, total_money);
        fflush(stdout);
        sem_post(&sem_printf);
        sem_post(&sem_money);
        // inserts next homework into queue
        insert(c);
        sem_post(&sem_empty);
        sem_post(&sem_queue);
    }
    sem_wait(&sem_printf);
    printf("H has no other homeworks, terminating.\n");
    fflush(stdout);
    sem_post(&sem_printf);
    fclose(file);
    return NULL;   
}

void clearStruct()
{
    for(int i = 0; i < student_num; i++)
    {
        sem_destroy(&students[i].sem);
    }
    free(students);
}

// creates students struct and assigns associated values to each student
void createStudents(char ** argv)
{
    FILE * file;
    file = fopen(argv[2], "r");
    char line[10000];
    int counter = 0;
    while (fgets(line, 10000, file))
    {
        if(strcmp(line, "\n") != 0)
        {
            student_num++;
            counter++;
        }
    }    
    fclose(file);
    students = calloc(counter, sizeof(struct Student));
    file = fopen(argv[2], "r");
    counter = 0;
    while (fgets(line, 10000, file))
    {
        if(strcmp(line, "\n") !=0 )
        {
            char *ptr = strtok(line, " ");
            strcpy(students[counter].name, ptr);
            ptr = strtok(NULL, " ");
            students[counter].Q = atoi(ptr);
            ptr = strtok(NULL, " ");
            students[counter].S = atoi(ptr);
            ptr = strtok(NULL, " ");
            students[counter].C = atoi(ptr);
            sem_init(&students[counter++].sem, 0, 0);
        }
    }    
    fclose(file);
}

void createQueue(char ** argv)
{
    FILE * file;
    file = fopen(argv[1], "r");
    int counter = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if(c == 'S' || c == 'C' || c == 'Q')
            counter++;
    }    
    fclose(file);
    queue_arr =  (char *)calloc(counter + 50, sizeof(char));
    max_size = counter + 50;
}

// checks arguments are proper or not
void controlArguments(int n, char **arg)
{

    // checks number of arguments of the program is correct
    if( n != 4)
    {
        printf("You must enter exactly 3 argument(input file 1, input file 2, amount of money)\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < strlen(arg[3]); i++ )
    {
        if(isdigit(arg[3][i]) == 0)
        {
            printf("Amount of total money must consist of digits and be a positive number!\n");
            fflush(stdout);
            exit(EXIT_SUCCESS);
        }    
    }
    // checks input file exist or not. If it is not terminates the program.
    FILE * file;
    file = fopen(arg[1], "r");
    if (file)
    {
        fclose(file);
    }
    else{
        fprintf(stderr, "File named as \"%s\" does not exist!\n", arg[1]);
        exit(EXIT_FAILURE);
    }    
    file = fopen(arg[2], "r");
    if (file)
    {
        fclose(file);
    }
    else{
        fprintf(stderr, "File named as \"%s\" does not exist!\n", arg[2]);
        exit(EXIT_FAILURE);
    }    
}
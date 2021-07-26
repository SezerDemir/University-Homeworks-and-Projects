#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h> 
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>      
#include <fcntl.h> 
#include <semaphore.h>
#define MESSAGESIZE 10000



void create_deamon();
void singleton_check();
void argument_check(int , char **);
void *Solve_query(void *);
void create_csv_list();

// logging file
FILE *fp_log = NULL, *fp_csv = NULL;
// reader-writer counts
int reader = 0, writer = 0, reader_waiting = 0, writer_waiting = 0;
int socket_fd, server_socket, client_socket, valread;
socklen_t client_size;
struct sockaddr_in address, client_address;
int addrlen, column_number = 0, row_number = 0;
double dataset_load_time = 0.0;
int port_number = 0, pool_size = 0, available_thread = 0;
int *available, *client_sockets;
char **columns;
char *** values;
pthread_mutex_t lock_log, main_thread_lock, available_lock, query_lock, read_write_lock;
pthread_mutex_t *thread_locks;
pthread_cond_t *thread_conds;
pthread_cond_t main_thread_cond, canread, canwrite;
sem_t * my_semaphore;

sig_atomic_t sig_int = 0;
char client_message[MESSAGESIZE], csv_file_name[2000], log_file_name[2000];
static void SIGINT_Handler(int signum)
{
    sig_int = 1;
}

int main(int argc, char * argv[])
{
    // checks arguments in correct format

    if ( (my_semaphore = sem_open("/aaaaaaaaaddd",O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 10 )) == SEM_FAILED)
    {
        printf("Server still running...\n");
        exit(EXIT_FAILURE);
    }
    
    argument_check(argc, argv);
    // entry message to display
    //creates deamon and applies singleton pattern
    // creates logging file
    fp_log = fopen(log_file_name, "w+");
    fprintf(fp_log, "Executing with parameters\n-p %d\n-o %s\n-l %d\n-d %s\n", port_number, log_file_name, pool_size, csv_file_name);
    fflush(fp_log);

    create_csv_list();
    create_deamon();
    singleton_check();
    client_size = sizeof(struct sockaddr_in);
    // sigint handler init
    struct sigaction sa;
    sa.sa_handler = SIGINT_Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        fprintf(fp_log, "sigaction error\n");
        fflush(fp_log);
    }


    // mutex init 
    if (pthread_mutex_init(&lock_log, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&available_lock, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&read_write_lock, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&query_lock, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_mutex_init(&main_thread_lock, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&main_thread_cond, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&canread, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_cond_init(&canwrite, NULL) != 0) {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, " mutex init has failed\n");
        exit(EXIT_FAILURE);
    }
    thread_locks = (pthread_mutex_t *)calloc(pool_size, sizeof(pthread_mutex_t));
    for(int i = 0; i < pool_size; i++)
    {
        if (pthread_mutex_init(&thread_locks[i], NULL) != 0) {
            sem_close(my_semaphore);
            sem_unlink("/aaaaaaaaaddd");
            fprintf(fp_log, " mutex init has failed\n");
            exit(EXIT_FAILURE);
        }        
    }
    thread_conds = (pthread_cond_t *)calloc(pool_size, sizeof(pthread_cond_t));
    for(int i = 0; i < pool_size; i++)
    {
        if (pthread_cond_init(&thread_conds[i], NULL) != 0) {
            sem_close(my_semaphore);
            sem_unlink("/aaaaaaaaaddd");
            fprintf(fp_log, " condtion variable init has failed\n");
            exit(EXIT_FAILURE);
        }        
    }

    // creates arrays those are required
    available = (int*)calloc(pool_size, sizeof(int));
    client_sockets = (int*)calloc(pool_size, sizeof(int));

    for(int i = 0; i < pool_size; i++)
    {
        available[i] = 1;
    }

    // creates pool threads
    pthread_t pool_th[pool_size];
    int id[pool_size];
    for(int i = 0 ; i < pool_size; i++)
    {
        id[i] = i;
        if(pthread_create(&pool_th[i], NULL, Solve_query, (void*)(id + i)) != 0)
        {
            sem_close(my_semaphore);
            sem_unlink("/aaaaaaaaaddd");
            fprintf(fp_log, "pthread_create error\n");
            fflush(fp_log);
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_lock(&lock_log);
    fprintf(fp_log,"A pool of %d threads has been created\n", pool_size);
    fflush(fp_log);
    pthread_mutex_unlock(&lock_log);

    available_thread = pool_size;
    // gets messages from socket 
    while(sig_int == 0)
    {
        if (sig_int == 1)
            break;
        // Accept an incoming connection:
        client_socket = accept(socket_fd, (struct sockaddr*)&client_address, &client_size);
        if (sig_int == 1)
            break;
        if (client_socket < 0){
            fprintf(fp_log, "accept() error\n");
            fflush(fp_log);
            return -1;
        }

        // checks if there is an available thread at the moment
        pthread_mutex_lock(&main_thread_lock);
        while(available_thread < 1)
        {
            pthread_mutex_lock(&lock_log);
            fprintf(fp_log, "No thread is available! Waiting...\n");
            fflush(fp_log);
            pthread_mutex_unlock(&lock_log);
            pthread_cond_wait(&main_thread_cond,&main_thread_lock);
        }
        pthread_mutex_unlock(&main_thread_lock);

        int index = 0;

        for(int i = 0; i < pool_size; i++)
        {
            pthread_mutex_lock(&available_lock);
            if(available[i] == 1)
            {
                index = i;
                pthread_mutex_unlock(&available_lock);
                break;
            }
            pthread_mutex_unlock(&available_lock);
        }

        // writes query into associated thread's query string
        pthread_mutex_lock(&query_lock);
        client_sockets[index] = client_socket;
        pthread_mutex_unlock(&query_lock);


        pthread_mutex_lock(&lock_log);
        fprintf(fp_log, "A connection has been delegated to thread id #%d\n", index + 1);
        fflush(fp_log);
        pthread_mutex_unlock(&lock_log);

        pthread_mutex_lock(&available_lock);
        available_thread--;
        pthread_mutex_unlock(&available_lock);


        // wakes associated thread up
        pthread_mutex_lock(&thread_locks[index]);
        available[index] = 0;
        pthread_mutex_unlock(&thread_locks[index]);    
        pthread_cond_signal(&thread_conds[index]); 

    }

    pthread_mutex_lock(&lock_log);
    fprintf(fp_log,"Termination signal received, waiting for ongoing threads to complete.\n");
    fflush(fp_log);
    pthread_mutex_unlock(&lock_log);
    // waits until all threads finish their works
    for(int i = 0; i < pool_size; i++)
    {
        for(int i = 0; i < pool_size; i++)
        {
            pthread_cond_broadcast(&thread_conds[i]);
            pthread_mutex_unlock(&main_thread_lock);                  
        }
        pthread_join(pool_th[i], NULL);
    }

    fprintf(fp_log,"All threads have terminated, server shutting down\n");
    fflush(fp_log);


    fprintf(fp_log, "terminated...\n");
    fflush(fp_log);
    sem_close(my_semaphore);
    sem_unlink("/aaaaaaaaaddd");
    pthread_mutex_destroy(&lock_log);
    pthread_mutex_destroy(&available_lock);
    pthread_mutex_destroy(&main_thread_lock);
    pthread_mutex_destroy(&query_lock);
    pthread_mutex_destroy(&read_write_lock);

    pthread_cond_destroy(&main_thread_cond);
    pthread_cond_destroy(&canread);
    pthread_cond_destroy(&canwrite);
    for(int i = 0; i < pool_size; i++)
    {
        pthread_mutex_destroy(&thread_locks[i]);
    }
    for(int i = 0; i < pool_size; i++)
    {
        pthread_cond_destroy(&thread_conds[i]);
    }
    free(thread_conds);
    free(thread_locks);
    free(available);
    free(client_sockets);

    for(int i = 0; i < column_number; i++)
    {
        for(int j = 0; j < row_number - 1; j++)
        {
            if(values[i][j] != NULL)
                free(values[i][j]);
        }
        if(values[i] != NULL)
            free(values[i]);
    }
    free(values);

    for(int i = 0; i < column_number; i++)
    {
        if(columns[i] != NULL)
            free(columns[i]);
    }
    free(columns);
    fclose(fp_csv);
    fclose(fp_log);
    exit(EXIT_SUCCESS);
}

void *Solve_query(void *num)
{
    int* id = (int*)num;
    int finished = 0;
    pthread_mutex_lock(&lock_log);
    fprintf(fp_log, "Thread #%d: waiting for connection\n", *id + 1);
    fflush(fp_log);
    pthread_mutex_unlock(&lock_log);
    while (1)
    {  

        pthread_mutex_lock(&available_lock);
        available[*id] = 1;
        pthread_mutex_unlock(&available_lock);
        pthread_mutex_lock(&main_thread_lock);
        pthread_mutex_lock(&available_lock);
        // waits until signal comes
        while(available[*id] == 1)
        {
            pthread_mutex_unlock(&available_lock);
            available_thread++;
            if(sig_int == 1)
            {
                for(int i = 0; i < pool_size; i++)
                {
                    pthread_cond_broadcast(&thread_conds[i]);
                    pthread_mutex_unlock(&main_thread_lock);                 
                }
                return 0;   
            }
            pthread_cond_signal(&main_thread_cond);
            pthread_cond_wait(&thread_conds[*id] ,&main_thread_lock);
            if(sig_int == 1)
            {
                for(int i = 0; i < pool_size; i++)
                {
                    pthread_cond_broadcast(&thread_conds[i]);
                    pthread_mutex_unlock(&main_thread_lock);     
                    return 0;               
                }

            }
            // to check available array
            pthread_mutex_lock(&available_lock);
        }
        pthread_mutex_unlock(&available_lock);
        pthread_mutex_unlock(&main_thread_lock);
    
        while(finished == 0)
        {
            int query_counter = 0;
            int select_flag = 0, select_dist_flag = 0, update_flag = 0;
            char *thread_query, response[1500000] = {'0'};
            char tmp_query[15000] = {'0'}, keyword[150] = {'0'}, where_condition[10000] = {'0'}, select_condition[100000] = {'0'};
            char *token;
            char *response_message = (char*)malloc((column_number * row_number * 2000) * sizeof(char));
            int *column_index = (int*)malloc(column_number*sizeof(int));
            for(int i = 0; i < column_number; i++)
            {
                column_index[i] = 0;
            }

            strcpy(response_message,"");


            select_flag = 0;
            select_dist_flag = 0;
            update_flag = 0;

            int data_size = 0, read_size = 0;
            char query_size[500] = {'0'};
            char message_read[100000] = {'0'};
            strcpy(query_size, "");
            strcpy(message_read, "");
            int message_finished = 0;
            // reads until gets size of the message from client
            while(message_finished == 0)
            {
                if ( (read_size = recv(client_sockets[*id], message_read, 1, 0) ) < 0)
                {
                    fprintf(fp_log, "recv() error\n");
                    fflush(fp_log);
                }
                message_read[1] = '\0';
                if(message_read[strlen(message_read) - 1] == 'y')
                {
                    message_read[strlen(message_read) - 1] = '\0';
                    message_finished = 1;
                }
                strcat(query_size, message_read);
            }
            read_size = atoi(query_size);


            // sends respose to client
            sprintf(response, "*");
            if (send(client_sockets[*id] , response , 1, 0) == -1)
            {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(fp_log, "send() error\n");
                    fflush(fp_log);      
                    exit(EXIT_FAILURE);      
            }
            strcpy(response, "");

            thread_query = (char*)malloc((read_size + 100) * sizeof(char));
            strcpy(thread_query, "");
            int current_size = 0;
            data_size = 0;
            // reads until get the whole message
            while(data_size != read_size)
            {
                if ( (current_size = recv(client_sockets[*id], message_read, read_size - data_size , 0)) < 0)
                {
                    if(current_size != 0)
                    {
                        fprintf(fp_log, "recv() error\n");
                        fflush(fp_log);
                    }

                }
                strcat(thread_query, message_read);
                data_size += current_size;
            }
            thread_query[data_size] = '\0';


            // sends respose to client
            sprintf(response, "*");
            if( send(client_sockets[*id] , response , 1, 0) < 0)
            {
                sem_close(my_semaphore);
                sem_unlink("/aaaaaaaaaddd");
                fprintf(fp_log, "write1() error\n");
                fflush(fp_log);     
                exit(EXIT_FAILURE);       
            }
            strcpy(response, "");

            int table_statement_flag = 0;
            strcpy(tmp_query, thread_query);
            strcpy(keyword, "");
            char *context = NULL;
            token = strtok_r(tmp_query, " ", &context);
            strcpy(where_condition, "");
            strcpy(select_condition, "");
            
            // parses statement
            while (token != NULL)
            {
                if(table_statement_flag == 0 && (strcasecmp(token, "UPDATE") == 0 || strcasecmp(token, "SELECT") == 0 || strcasecmp(token, "SET") == 0 || strcasecmp(token, "DISTINCT") == 0 || strcasecmp(token, "TABLE") == 0))
                {
                    if(strcasecmp(token, "SELECT") == 0)
                        select_flag = 1;
                    if(strcasecmp(token, "DISTINCT") == 0)
                    {
                        select_dist_flag = 1;
                        break;
                    }           
                    if(strcasecmp(token, "UPDATE") == 0)
                    {
                        update_flag = 1;
                        break;
                    }      
                    strcat(keyword, token);
                    strcat(keyword, " ");
                }
                token = strtok_r(NULL, " ", &context);
            }
            keyword[strlen(keyword) - 1] = '\0';
            char query_word[20000] = {'0'};
            strcpy(query_word,"");
            int start = 0;
            int counter = 0, star_cond = 0;
            if(select_dist_flag == 1 || select_flag == 1)
            {
                int single_q = 0, double_q = 0;
                for(int i = 0; i < strlen(thread_query); i++)
                {
                    if(thread_query[i] == ' ' && start == 0)
                    {
                        strcpy(query_word, "");
                    }
                    else if(start == 0)
                    {
                        char x[3] = {'0'};
                        x[0] = thread_query[i];
                        x[1] = '\0';
                        strcat(query_word, x);
                    }
                    if(start== 0 && ((select_dist_flag == 1 && strcasecmp(query_word, "DISTINCT") == 0) || (select_flag == 1 && select_dist_flag == 0 && strcasecmp(query_word, "SELECT") == 0)) )
                    {
                        start = 1;
                        strcpy(query_word, "");
                        continue;
                    }
                    if(start == 1)
                    {
                        if(single_q == 0 && double_q == 0)
                        {
                            if(thread_query[i] == '\'')
                            {   
                                single_q = 1;
                                continue;
                            }
                            else if(thread_query[i] == '"')
                            {
                                double_q = 1;
                                continue;
                            }
                        }
                        if((single_q == 0 && double_q == 0) && (thread_query[i] == ',' || (strlen(query_word) != 0 && thread_query[i] == ' ' && single_q == 0 && double_q == 0)))
                        {
                            if(strcasecmp(query_word, "FROM") == 0)
                                break;
                            
                            for(int i = 0; i < column_number; i++)
                            {
                                if(strcmp(query_word, columns[i]) == 0)
                                {
                                    column_index[i] = 1;
                                    counter++;
                                }
                            }
                            strcpy(query_word, "");
                            continue;
                        }
                        else if((single_q == 0 && double_q == 0) && thread_query[i] != ',' && thread_query[i] != ' ')
                        {
                            char c[3] = {'0'};
                            strcpy(c, "");
                            c[0] = thread_query[i];
                            c[1] = '\0';
                            strcat(query_word, c);
                            if(c[0] == '*')
                            {
                                star_cond = 1;
                                break;
                            }
                        }
                        else if(single_q == 1 && thread_query[i] == '\'')
                        {
                            single_q = 0;
                            continue;
                        }
                        else if(double_q == 1 && thread_query[i] == '"')
                        {   
                            double_q = 0;
                            continue;
                        }
                        if((double_q == 1 && thread_query[i] != '"') || (single_q == 1 && thread_query[i] != '\'')) // "Mag nitude"
                        {
                            char c[3] = {'0'};
                            strcpy(c, "");
                            c[0] = thread_query[i];
                            c[1] = '\0';
                            strcat(query_word, c);
                        }
                    }
                }
            }


            // reader segment
            if(select_dist_flag == 1 || select_flag == 1)
            {
                pthread_mutex_lock(&read_write_lock);
                // if there is at least 1 writer at the waiting state or the writing state
                while(writer > 0 || writer_waiting > 0)
                {
                    reader_waiting++;
                    pthread_cond_wait(&canread, &read_write_lock);
                    reader_waiting--;
                }
                reader++;
                pthread_mutex_unlock(&read_write_lock);
                pthread_cond_broadcast(&canread);        

                //prints message
                pthread_mutex_lock(&lock_log);
                fprintf(fp_log, "Thread #%d: received query '%s'\n", *id + 1, thread_query);
                fflush(fp_log);
                pthread_mutex_unlock(&lock_log);
  
                if(select_flag == 1)
                {
                    strcpy(response_message, "");
                    char ***tmp_data;
                    int tmp_counter = 0;
                    if(select_dist_flag == 1)
                    {
                        // creates temp array that stores used values   
                        tmp_data = (char ***)malloc(column_number * sizeof(char **));
                        if (tmp_data == NULL)
                        {
                            sem_close(my_semaphore);
                            sem_unlink("/aaaaaaaaaddd");
                            fprintf(fp_log, "tmp_values array: out of memory\n");
                            fflush(fp_log);
                            exit(EXIT_FAILURE);
                        }
                        for (int i=0; i < column_number; i++)
                        {
                            tmp_data[i] = (char **)malloc((row_number - 1)* sizeof(char *));
                            if (tmp_data[i] == NULL)
                            {
                                sem_close(my_semaphore);
                                sem_unlink("/aaaaaaaaaddd");
                                fprintf(fp_log, "tmp_values array: out of memory\n");
                                fflush(fp_log);
                                exit(EXIT_FAILURE);
                            }
                            for (int j = 0; j < row_number - 1; j++)
                            {
                                tmp_data[i][j] = (char*)malloc(2000 * sizeof(char));
                                if (tmp_data[i][j] == NULL)
                                {
                                    sem_close(my_semaphore);
                                    sem_unlink("/aaaaaaaaaddd");
                                    fprintf(fp_log, "tmp_values array: out of memory\n");
                                    fflush(fp_log);
                                    exit(EXIT_FAILURE);
                                }
                            }
                        }                
                    }

                    // if SELECT * query has come
                    if(star_cond == 1)
                    {    
                        // adds column names at the beginning of the response message
                        for(int i = 0 ; i < column_number; i++)
                        {
                            strcat(response_message, columns[i]);
                            if(i != column_number - 1)
                                strcat(response_message, ",");
                            else
                                strcat(response_message, "\n");
                        }
                        int flag = 0;
                        for(int i = 0; i < row_number - 1; i++)
                        {
                            if(select_dist_flag == 1)
                            {
                                flag = 0;
                                for(int m = 0; m < tmp_counter; m++)
                                {
                                    flag = 0;
                                    for(int z = 0; z < column_number; z++)
                                    {
                                        if(strcmp(tmp_data[z][m], values[z][i]) != 0)
                                        {
                                            flag = 1;
                                        }
                                    }
                                    //found 1 same sample in dataset that is added before
                                    if(flag == 0)
                                        break;
                                }
                            }
                            if(select_dist_flag == 0 || tmp_counter == 0 || flag == 1)
                            {
                                for(int j = 0 ; j < column_number; j++)
                                {         
                                    if(select_dist_flag == 1)
                                    {
                                        strcpy(tmp_data[j][tmp_counter], values[j][i]);
                                    }
                                    strcat(response_message, values[j][i]);
                                    if(j != column_number - 1)
                                        strcat(response_message, ",");
                                    else
                                        strcat(response_message, "\n");
                                }
                                query_counter++;
                                if(select_dist_flag == 1)
                                    tmp_counter++;
                            }
                        }
                    }
                    else
                    {
                        int counter_n = 0;

                        // adds column names at the beginning of the response message
                        for(int i = 0 ; i < column_number; i++)
                        {
                            if(column_index[i] == 1)
                            {
                                counter_n++;
                                strcat(response_message, columns[i]);
                                if(counter_n != counter)
                                    strcat(response_message, ",");
                                else
                                {
                                    strcat(response_message, "\n");
                                    break;
                                }
                            }
                            
                        }
                        counter_n = 0;
                        int flag = 0;
                        for(int i = 0; i < row_number - 1; i++)
                        {
                            if(select_dist_flag == 1)
                            {
                                flag = 0;
                                for(int m = 0; m < tmp_counter; m++)
                                {
                                    flag = 0;
                                    for(int z = 0; z < column_number; z++)
                                    {
                                        if(column_index[z] == 1)
                                        {
                                            if(strcmp(tmp_data[z][m], values[z][i]) != 0)
                                            {
                                                flag = 1;
                                            }
                                        }
                                    }
                                    //found 1 same sample in dataset that is added before
                                    if(flag == 0)
                                        break;
                                }
                            }    
                            if(select_dist_flag == 0 || tmp_counter == 0 || flag == 1)
                            {
                                for(int j = 0 ; j < column_number; j++)
                                {
                                    if(column_index[j] == 1)
                                    {
                                        if(select_dist_flag == 1)
                                        {
                                            strcpy(tmp_data[j][tmp_counter], values[j][i]);
                                        }
                                        counter_n++;
                                        strcat(response_message, values[j][i]);
                                        if(counter_n != counter)
                                            strcat(response_message, ",");
                                        else
                                        {
                                            strcat(response_message, "\n");
                                            break;
                                        }
                                    }
                                }
                                query_counter++;
                                if(select_dist_flag == 1)
                                    tmp_counter++;
                            }
                            
                            counter_n = 0;
                        }
                    }
                    if(select_dist_flag == 1)
                    {
                        for(int i = 0; i < column_number; i++)
                        {
                            for(int j = 0; j < row_number - 1; j++)
                            {
                                if(tmp_data[i][j] != NULL)
                                    free(tmp_data[i][j]);
                            }
                        }
                        for(int i = 0; i < column_number; i++)
                        {
                            if(tmp_data[i] != NULL)
                                free(tmp_data[i]);
                        }
                        if(tmp_data != NULL)
                            free(tmp_data);
                    }
                }      

                pthread_mutex_lock(&lock_log);
                fprintf(fp_log, "Thread #%d: query completed, %d records have been returned.\n", *id + 1, query_counter);
                fflush(fp_log);
                pthread_mutex_unlock(&lock_log);
                
                //finishes reading process
                pthread_mutex_lock(&read_write_lock);
                reader--;
                if (reader == 0)
                    pthread_cond_signal(&canwrite);
                pthread_mutex_unlock(&read_write_lock);


            }
            else if(update_flag == 1)        // writer segment
            {
                pthread_mutex_lock(&read_write_lock);
                // if there is a writer or reader in process, waits
                while(writer > 0 || reader > 0) {
                    writer_waiting++;
                    pthread_cond_wait(&canwrite, &read_write_lock);
                    writer_waiting--;
                }
                writer = 1;
                pthread_mutex_unlock(&read_write_lock);

                //prints message
                pthread_mutex_lock(&lock_log);
                fprintf(fp_log, "Thread #%d: received query '%s'\n", *id + 1, thread_query);
                fflush(fp_log);
                pthread_mutex_unlock(&lock_log);

                // UPDATING SECTION //
                strcpy(response_message, "");
                int counter = 0, counter_n = 0;
                strcat(select_condition,",");
                int start = 0, single_q = 0, double_q = 0, where_last_index = 0;


                char **cond_vals = (char**)malloc(column_number * sizeof(char*));
                for(int i = 0 ; i < column_number; i++)
                {
                    cond_vals[i] = (char*)malloc(10000 * sizeof(char));
                }

                for(int i = 0 ; i < column_number; i ++)
                {
                    column_index[i] = -1;
                }

                char query_word[20000] = {'0'};
                strcpy(query_word,"");
                int num = 0;
                int eq_flag = 0;
                for(int i = 0; i < strlen(thread_query); i++)
                {
                    if(thread_query[i] == ' ' && start == 0)
                    {
                        strcpy(query_word, "");
                    }
                    else if(start == 0)
                    {
                        char x[3] = {'0'};
                        x[0] = thread_query[i];
                        x[1] = '\0';
                        strcat(query_word, x);
                    } //2 UPDATE TABLE SET columnName1=’value1’, columnName2=’value2’ WHERE columnName=’valueX’
                    if(start== 0 && strcasecmp(query_word, "SET") == 0)
                    {
                        start = 1;
                        strcpy(query_word, "");
                        continue;
                    }
                    if(start == 1)
                    {
                        if(single_q == 0 && double_q == 0)
                        {
                            if(thread_query[i] == '\'')
                            {   
                                single_q = 1;
                                continue;
                            }
                            else if(thread_query[i] == '"')
                            {
                                double_q = 1;
                                continue;
                            }
                        }
                        if( eq_flag == 0 && ( ((single_q == 0 && double_q == 0) && thread_query[i] == '=') || (strlen(query_word) != 0 && thread_query[i] == ' ' && single_q == 0 && double_q == 0)))
                        {

                            if(strcasecmp(query_word, "WHERE") == 0)
                            {
                                where_last_index = i + 1;
                                break;
                            }

                            for(int j = 0; j < column_number; j++)
                            {
                                if(strcmp(query_word, columns[j]) == 0)
                                {
                                    column_index[counter] = j;
                                    counter++;
                                }
                            }
                            eq_flag = 1;
                            strcpy(query_word, "");
                            continue;
                        }
                        else if(eq_flag == 1 && ( ((single_q == 0 && double_q == 0) && thread_query[i] == ',') || (strlen(query_word) != 0 && thread_query[i] == ' ' && single_q == 0 && double_q == 0)))
                        {
                            if(strcasecmp(query_word, "WHERE") == 0)
                            {
                                where_last_index = i + 1;
                                break;
                            }
                            for(int j = num ; j < column_number; j++)
                            {
                                if(column_index[j] != -1)
                                {
                                    strcpy(cond_vals[column_index[j]], query_word);
                                    num = j + 1;
                                    break;
                                }
                            }
                            eq_flag = 0;
                            strcpy(query_word, "");  //2 UPDATE TABLE SET columnName1=’value1’, columnName2=’value2’ WHERE columnName=’valueX’
                            continue;
                        }
                        else if((single_q == 0 && double_q == 0) && ( (thread_query[i] != ',' && eq_flag == 1) || eq_flag == 0 ) && thread_query[i] != ' ')
                        {
                            char c[3] = {'0'};
                            strcpy(c, "");
                            c[0] = thread_query[i];
                            c[1] = '\0';
                            strcat(query_word, c);

                        }
                        else if(single_q == 1 && thread_query[i] == '\'')
                        {
                            single_q = 0;
                            continue;
                        }
                        else if(double_q == 1 && thread_query[i] == '"')
                        {   
                            double_q = 0;
                            continue;
                        }
                        if((double_q == 1 && thread_query[i] != '"') || (single_q == 1 && thread_query[i] != '\'')) // "Mag nitude"
                        {
                            char c[3] = {'0'};
                            strcpy(c, "");
                            c[0] = thread_query[i];
                            c[1] = '\0';
                            strcat(query_word, c);
                        }
                    }
                }

                char where_column_name[10000] = {'0'}, where_column_value[10000] = {'0'};
                strcpy(query_word, "");
                strcpy(where_column_value, "");
                single_q = 0, double_q = 0;
                int where_flag = 0;
                for(int i = where_last_index; i < strlen(thread_query); i++)
                {
                    if(double_q == 0 && single_q == 0)
                    {
                        if(i == strlen(thread_query) - 1 && (thread_query[i] == '\'' || thread_query[i] == '"' || thread_query[i] == ';') )
                        {
                            strcpy(where_column_value, query_word);
                            break;
                        }
                        if(thread_query[i] == '\'')
                        {
                            single_q = 1;
                            continue;
                        }
                        if(thread_query[i] == '"')
                        {
                            double_q = 1;
                            continue;
                        }
                    }
                    if(single_q == 0 && double_q == 0 && thread_query[i] != ' ' && thread_query[i] != '=')
                    {
                        char c[3] = {'0'};
                        strcpy(c, "");
                        c[0] = thread_query[i];
                        c[1] = '\0';
                        strcat(query_word, c);
                        if(i == strlen(thread_query) - 1)
                        {
                            strcpy(where_column_value, query_word);
                            break;
                        }
                        continue;
                    }
                    else if(where_flag == 0 && single_q == 0 && double_q == 0 && thread_query[i] == '=')
                    {
                        strcpy(where_column_name, query_word);
                        strcpy(query_word, "");
                        where_flag = 1;
                        continue;
                    }
                    if(where_flag == 1 && single_q == 0 && double_q == 0 && (thread_query[i] == ';' || i == strlen(thread_query) - 1))
                    {
                        strcpy(where_column_value, query_word);
                        break;
                    }
                    else if(single_q == 1 && thread_query[i] == '\'')
                    {
                        single_q = 0;
                        if(i == strlen(thread_query) - 1)
                        {
                            strcpy(where_column_value, query_word);
                            break;
                        }
                        continue;
                    }
                    else if(double_q == 1 && thread_query[i] == '"')
                    {   
                        double_q = 0;
                        if(i == strlen(thread_query) - 1)
                        {
                            strcpy(where_column_value, query_word);
                            break;
                        }
                        continue;
                    }
                    if((double_q == 1 && thread_query[i] != '"') || (single_q == 1 && thread_query[i] != '\'')) // "Mag nitude"
                    {
                        char c[3] = {'0'};
                        strcpy(c, "");
                        c[0] = thread_query[i];
                        c[1] = '\0';
                        strcat(query_word, c);
                    }
                }

                

                int where_index = -1;
                for(int i = 0 ; i < column_number; i++)
                {
                    if(strcmp(columns[i], where_column_name) == 0)
                    {
                        where_index = i;
                        break;
                    }
                }
                // creates response message
                if(where_index == -1)
                {
                    strcpy(response_message, "NOTHING FOUND!!!\n");
                }
                else
                {
                    strcpy(response_message,"");
                    int flag = 0;
                    for(int i = 0; i < row_number - 1 ; i++)
                    {
                        flag = 0;
                        if(strcmp(values[where_index][i], where_column_value) == 0)
                        {
                            for(int j = 0; j < column_number; j++)
                            {
                                if(column_index[j] != -1)
                                {
                                    strcpy(values[column_index[j]][i], cond_vals[column_index[j]]);
                                    flag = 1;
                                }
                            }
                        }
                        if(flag == 1)
                        {
                            for(int j = 0; j < column_number; j++)
                            {
                                counter_n++;
                                strcat(response_message, values[j][i]);
                                if(j != column_number - 1)
                                    strcat(response_message, ",");
                                else
                                {
                                    strcat(response_message, "\n");
                                    break;
                                }
                            }
                            query_counter++;
                        }
                    }
                }


                pthread_mutex_lock(&lock_log);
                fprintf(fp_log, "Thread #%d: query completed, %d records have been returned.\n", *id + 1, query_counter);
                fflush(fp_log);
                pthread_mutex_unlock(&lock_log);
  

                pthread_mutex_lock(&read_write_lock);
                // writer's work is done
                writer = 0;
                // if any readers are waiting, threads are unblocked
                if (writer_waiting > 0)
                    pthread_cond_signal(&canwrite);
                else
                    pthread_cond_signal(&canread);
                pthread_mutex_unlock(&read_write_lock);

                for(int i = 0;  i < column_number; i++)
                {
                    free(cond_vals[i]);
                }
                free(cond_vals);
            }
            else
            {
                strcpy(response_message, "UNVALID QUERY!" );
                query_counter++;
            }

            sleep(0.5);

            // SENDING THE RESULT PART //

            
            data_size = strlen(response_message);
            char *size_send_message = (char *)malloc((data_size + 100) * sizeof(char));
            char *message_send = (char *)malloc((data_size + 150) * sizeof(char));
            sprintf(size_send_message, "%ldy", strlen(response_message));
            data_size = strlen(size_send_message);
            int send_size = 0 ,total_send = 0;
            int index_c = 0;
            while(data_size != total_send)
            {
                // creates string of remaining message
                for(int i = total_send ; i < strlen(size_send_message); i++)
                {
                    message_send[index_c++] = size_send_message[i];
                }
                message_send[index_c] = '\0';
                if( (send_size = send(client_sockets[*id] , message_send , data_size - total_send, 0) ) < 0)
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(fp_log, "write2() error\n");
                    fflush(fp_log);   
                    exit(EXIT_FAILURE);         
                }          

                index_c = 0;
                total_send += send_size;
            }

            // waits until a response comes from client
            if (recv(client_sockets[*id], message_read, 1, 0) < 0)
            {
                sem_close(my_semaphore);
                sem_unlink("/aaaaaaaaaddd");
                fprintf(fp_log, "reacv() error\n");
                fflush(fp_log);
                exit(EXIT_FAILURE);
            }       

            // sends message itself
            strcpy(message_send, "");
            total_send = 0;
            index_c = 0;
            strcpy(size_send_message,response_message);
            size_send_message[strlen(size_send_message) - 1] = '\0';
            long int new_data_size = strlen(response_message);

            while(new_data_size != total_send)
            {
                // creates string of remaining message
                for(int i = total_send ; i < strlen(size_send_message); i++)
                {
                    message_send[index_c++] = size_send_message[i];
                }
                message_send[index_c] = '\0';
                if( (send_size = send(client_sockets[*id] , message_send , new_data_size - total_send, 0) ) < 0)
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(fp_log, "send() error\n");
                    fflush(fp_log);   
                    exit(EXIT_FAILURE);         
                }          

                index_c = 0;
                total_send += send_size;
            }
            new_data_size = 0;
            strcpy(message_read, "");
            // waits until a response comes from client
            if (recv(client_sockets[*id], message_read, 1, 0) < 0)
            {
                fprintf(fp_log, "recv() error: %s \n", strerror(errno));
                fflush(fp_log);
            }     


            strcpy(keyword, "");
            message_read[1] = '\0';
            if(message_read[0] == 'n')
                finished = 1;
        
            free(message_send);
            free(size_send_message);
            free(thread_query);
            free(response_message);
            free(column_index);
            query_counter++;
        }
        finished = 0;

        pthread_mutex_lock(&query_lock);
        close(client_sockets[*id]);
        pthread_mutex_unlock(&query_lock);

    }
    
    return 0;
}

void create_deamon()
{
    pid_t pid;
    if( (pid = fork()) < 0 )
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        perror("fork() error");
        exit(EXIT_FAILURE);
    }
    // parent exits
    if (pid > 0)
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        exit(EXIT_SUCCESS);
    }
        
    umask(0);

    // makes child session leader
    if (setsid() < 0)
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        perror("setsid() error");
        exit(EXIT_FAILURE);    
    }

    // makes working directory root
    if(chdir("/") == -1)
    {   
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        perror("chdir() error");
        exit(EXIT_FAILURE);
    }
    // closes standard output, input, and error streams
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

}

// checks is there any server process that is running at the moment by creating a socket on port of 2000.
// if that port is binded before it throws an error and exits
void singleton_check()
{
    socket_fd = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_fd == -1)
	{
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
		fprintf(fp_log, " Could not create socket\n");
        fflush(fp_log);
        exit(EXIT_FAILURE);
	}

    address.sin_family = AF_INET;
    address.sin_port = htons(port_number);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(socket_fd, (struct sockaddr*)&address, sizeof(address))<0){
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, "Error while listening\n");
        exit(EXIT_FAILURE);
    }

    // listens for clients:
    if(listen(socket_fd, 100) < 0){
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, "listen() error\n");
        exit(EXIT_FAILURE);
    }
}

void argument_check(int argc_cpy, char **argv_cpy)
{
    char c;
    int p_flag = 0, o_flag = 0, l_flag= 0, d_flag = 0;
    // gets arguments and checks if they're in correct format
    while( (c = getopt(argc_cpy, argv_cpy, "p:o:l:d:")) != -1 )
    {
        switch (c)
        {
            case 'p':
                p_flag = 1;
                port_number = atoi(optarg);
                if(port_number < 1000)
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(stderr,"port number must be greater than 1000\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                o_flag = 1;
                strcpy(log_file_name, optarg);
                break;
            case 'l':
                l_flag = 1;
                pool_size = atoi(optarg);
                if(pool_size < 2)
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(stderr,"pool size must be greater than 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'd':
                d_flag = 1;
                strcpy(csv_file_name, optarg);
                break;
            //handles invalid flags and no-argument flags
            case '?':
                if(optopt == 'p' || optopt == 'o' || optopt == 'l' || optopt == 'd')
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(stderr, "Flag needs an argument!\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    sem_close(my_semaphore);
                    sem_unlink("/aaaaaaaaaddd");
                    fprintf(stderr, "Unknown flag !\nValid flags = ('-p', '-o', '-l', '-d')\n");
                    exit(EXIT_FAILURE);                
                }
                break;
            default:
            {
                sem_close(my_semaphore);
                sem_unlink("/aaaaaaaaaddd");
                fprintf(stderr,"getopt() caused an error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    if(p_flag == 0 || l_flag == 0 || o_flag == 0 || d_flag == 0)
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(stderr, "You must enter the all necessary flags ('-p', '-o', '-l', '-d')!\n");
        exit(EXIT_FAILURE);
    }    

    fp_csv = fopen(csv_file_name, "r");
    if(fp_csv == NULL)
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(stderr, "There is no such a query file at given location!\n");
        exit(EXIT_FAILURE);
    }    
}

// loads csv file into 3d array
void create_csv_list()
{
    fprintf(fp_log, "Loading dataset...\n");
    fflush(fp_log);
    clock_t begin = clock();
    char columnline[MESSAGESIZE]={'0'};

    FILE *fp_csv = fopen(csv_file_name, "r");
    while(fgets(columnline, MESSAGESIZE, fp_csv))
    {
        if(strcmp(columnline,"\n") != 0)
        {
            if(columnline[strlen(columnline) - 1 ] == '\n' || columnline[strlen(columnline) - 1 ] == '\r')
                columnline[strlen(columnline) - 1 ] = '\0';
            if(columnline[strlen(columnline) - 1 ] == '\n' || columnline[strlen(columnline) - 1 ] == '\r')
                columnline[strlen(columnline) - 1 ] = '\0';
            break;
        }
    }
    if(columnline[strlen(columnline) - 1] != ',')
        strcat(columnline, ",");

    int sin_q = 0, double_q = 0, start = 0;

    for(int i = 0; i < strlen(columnline); i++)
    {
        if(start == 0)
        {
            start = 1;
            if(columnline[i] == '\'')
            {
                sin_q = 1;
            }
            else if(columnline[i] == '"')
            {
                double_q = 1;
            }
            continue;
        }
        if(sin_q == 1 && double_q == 0)
        {
            if(columnline[i] != '\'')
            {
                continue;
            }
            else
            {
                sin_q = 0;
                continue;
            }
        }
        if(sin_q == 0 && double_q == 1)
        {
            if(columnline[i] != '"')
            {
                continue;
            }
            else
            {
                double_q = 0;
                continue;
            }
        }
        if(double_q == 0 && sin_q == 0 && columnline[i] == ',')
        {
            column_number++;
            start = 0;
        }
    }
    sin_q = 0, double_q = 0, start = 0;

    // creates array that stores column names
    columns = (char **)malloc(column_number * sizeof(char *));
    for (int i=0; i < column_number; i++)
         columns[i] = (char *)malloc(1000 * sizeof(char));

    char word[1000] = {'0'};
    strcpy(word,"");
    int counter = 0;
    int c_w = 0;
    start = 0;
    for(int i = 0; i < strlen(columnline); i++)
    {
        if(start == 0)
        {
            start = 1;
            if(columnline[i] == '\'')
            {
                sin_q = 1;              
            }
            else if(columnline[i] == '"')
            {
                double_q = 1;
            }
            if(sin_q == 1 || double_q == 1)
                continue;
        }
        if(sin_q == 1 && double_q == 0)
        {
            if(columnline[i] == '\'')
            {
                sin_q = 0;
                continue;
            }
        }
        if(sin_q == 0 && double_q == 1)
        {
            if(columnline[i] == '"')
            {
                double_q = 0;
                continue;
            }
        }
        if(double_q == 0 && sin_q == 0 && columnline[i] == ',')
        {
            word[c_w] = '\0';
            strcpy(columns[counter++], word);
            strcpy(word, "");
            c_w = 0;
            start = 0;
            continue;
        }
        word[c_w++] = columnline[i];
    }

    fseek(fp_csv, 0, SEEK_SET);
    counter = 0;
    while(fgets(columnline, MESSAGESIZE, fp_csv) != NULL)
        counter++;
    
    // creates array that stores values   
    values = (char ***)malloc(column_number * sizeof(char **));
    if (values == NULL)
    {
        sem_close(my_semaphore);
        sem_unlink("/aaaaaaaaaddd");
        fprintf(fp_log, "values array: out of memory");
        exit(EXIT_FAILURE);
    }
    for (int i=0; i < column_number; i++)
    {
        values[i] = (char **)malloc((counter - 1)* sizeof(char *));
        if (values[i] == NULL)
        {
            sem_close(my_semaphore);
            sem_unlink("/aaaaaaaaaddd");
            fprintf(fp_log, "values array: out of memory");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < counter - 1; j++)
        {
            values[i][j] = (char*)malloc(2000 * sizeof(char));
            if (values[i][j] == NULL)
            {
                sem_close(my_semaphore);
                sem_unlink("/aaaaaaaaaddd");
                fprintf(fp_log, "values array: out of memory");
                exit(0);
            }
        }
    }

    row_number = counter;
    fseek(fp_csv, 0, SEEK_SET);
    fgets(columnline, MESSAGESIZE, fp_csv);

    char value[2000] = {'0'};
    counter = 0;
    int line = 0;    
    int flag = 0, flag1 = 0;
    // stores values into values 3d array
    while(fgets(columnline, MESSAGESIZE, fp_csv) != NULL)
    {
        if(strcmp(columnline,"\n") == 0)
            continue;
        if(columnline[strlen(columnline) - 1] == '\n' || columnline[strlen(columnline) - 1] == '\r')
            columnline[strlen(columnline) - 1] = '\0';
        if(columnline[strlen(columnline) - 1] == '\n' || columnline[strlen(columnline) - 1] == '\r')
            columnline[strlen(columnline) - 1] = '\0';
        if(columnline[strlen(columnline) - 1] !=',')
            strcat(columnline,",");
        strcpy(value, "");
        for(int i = 0 ; i < strlen(columnline); i++)
        {
            if(columnline[i] == '"' && flag1 % 2 == 0)
            {
                flag++;
            }
            if(columnline[i] == '\'' && flag % 2 == 0)
            {
                flag1++;
            }
            if(columnline[i] == ',' && (flag % 2 == 0 && flag1 % 2 == 0))
            {
                if(value[0] == '\'' || value[0] == '"')
                {
                    for(int i = 0 ; i < strlen(value) - 1; i++)
                    {
                        value[i] = value[i+1];
                    }
                    value[strlen(value) - 2] = '\0';
                }
                if((i > 0 && columnline[i-1] == ',') || i == 0)
                    strcpy(values[line][counter], "NULL");
                else
                    strcpy(values[line][counter], value);
                ++line;
                strcpy(value,"");  
                continue;
            }
            char tmp[2];
            tmp[0] = columnline[i];
            tmp[1] = '\0';
            strcat(value, tmp);
        }
        flag = 0;
        ++counter;
        line = 0;
    }

    clock_t end = clock();
    dataset_load_time += (double)(end - begin) / CLOCKS_PER_SEC;

    fprintf(fp_log, "Dataset loaded in %f seconds with %d records\n", dataset_load_time, row_number - 1);
    fflush(fp_log);
    fclose(fp_csv);
}

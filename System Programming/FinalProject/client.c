#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>
#define MESSAGELENGTH 5000

void create_socket();
void send_message(char *);
void argument_check(int, char **);

double dataset_load_time = 0.0;
int socket_fd, server_socket, client_socket, valread;
socklen_t client_size;
struct sockaddr_in address, client_address;
int addrlen, finished = 0, query_number = 0;
int id, port_number, first = 1;;
char server_message[MESSAGELENGTH], client_message[MESSAGELENGTH], ip_adress[200], file_name[1000];
FILE * query_fp, * tmp_fp;

int main(int argc, char * argv[])
{
    int total_query = 0;

    memset(server_message,'\0',sizeof(server_message));
    memset(client_message,'\0',sizeof(client_message));

    argument_check(argc, argv);
    create_socket();


    // sends message to server
    while((fgets(client_message, MESSAGELENGTH, query_fp)) != NULL) {
        char tmp[MESSAGELENGTH] = {'0'};
        strcpy(tmp, client_message);
        char *context = NULL;
        char *ptr = strtok_r(tmp, " ",&context);
        if(client_message[strlen(client_message) - 1] == '\n')
            client_message[strlen(client_message) - 1] = '\0';
        if(atoi(ptr) == id)
        {
            if(first == 1)
            {
                printf("Client-%d connecting to %s:%d\n", id, ip_adress, port_number);
                fflush(stdout);
                if(connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
                    perror("Connect:");
                    exit(EXIT_FAILURE);
                }
            }
            first = 0;
            query_number++;
            send_message(client_message);
            total_query++;
        }
        strcpy(client_message, "");
    }



    printf("Client ID:%d A total of %d queries were executed, client is terminating.\n",id, total_query);
    fflush(stdout);
    

    fclose(query_fp);
    fclose(tmp_fp);
    exit(EXIT_SUCCESS);
}
void argument_check(int argc_cpy, char **argv_cpy)
{
    char c;
    int i_flag = 0, a_flag = 0, p_flag= 0, o_flag = 0;
    // gets arguments and checks if they're in correct format
    while( (c = getopt(argc_cpy, argv_cpy, "i:a:p:o:")) != -1 )
    {
        switch (c)
        {

            case 'i':
                i_flag = 1;
                id = atoi(optarg);
                if(id < 1)
                {
                    fprintf(stderr,"id must be greater than or equal to 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'a':
                a_flag = 1;
                strcpy(ip_adress, optarg);
                // argument format check
                break;
            case 'p':
                p_flag = 1;
                port_number = atoi(optarg);
                if(port_number < 1000)
                {
                    fprintf(stderr,"port number must be greater than 1000\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                o_flag = 1;
                strcpy(file_name, optarg);
                break;

            //handles invalid flags and no-argument flags
            case '?':
                if(optopt == 'i' || optopt == 'a' || optopt == 'p' || optopt == 'o')
                {
                    fprintf(stderr, "Flag needs an argument!\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    fprintf(stderr, "Unknown flag !\nValid flags = ('-i', '-a', '-p', '-o')\n");
                    exit(EXIT_FAILURE);                
                }
                break;
            default:
            {
                fprintf(stderr,"getopt() caused an error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    if(i_flag == 0 || a_flag == 0 || p_flag == 0 || o_flag == 0)
    {
       fprintf(stderr, "You must enter the all necessary flags ('-i', '-a', '-p', '-o')!\n");
        exit(EXIT_FAILURE);
    }    

    query_fp = fopen(file_name, "r");
    if(query_fp == NULL)
    {
        fprintf(stderr, "There is no such a query file at given location!\n");
        exit(EXIT_FAILURE);
    }    
    tmp_fp = fopen(file_name, "r");
}
void send_message(char *message)
{
    
    char *size_message = (char*)malloc((strlen(message)+1)*sizeof(char));
    char *left_message = (char*)malloc((strlen(message)+1)*sizeof(char));
    sprintf(size_message, "%ldy", strlen(message));
    long int message_length = strlen(size_message);
    long int total_send = 0, current_send = 0;
    int counter = 0;

    clock_t begin = clock();

    printf("Client-%d connected and sending query ‘%s’\n",id, message);
    fflush(stdout);

    // Sends the message of how many bytes will be sent to the server
    while(total_send != message_length)
    {
        for(int i = total_send; i < strlen(size_message); i++)
        {
            left_message[counter++] = size_message[i];
        }
        left_message[counter] = '\0';
        if( (current_send = send(socket_fd, left_message, message_length - total_send, 0) ) < 0){
            perror("send() error");
            exit(EXIT_FAILURE);
        }
        total_send += current_send;
        counter = 0;
    }


    char dumb_message[100] = {'0'};
    strcpy(dumb_message, "");

    // waits until a response comes from server
    if (recv(socket_fd, dumb_message, 1, 0) < 0)
    {
        perror("recv():");
    }      
    dumb_message[1]='\0';


    strcpy(left_message, "");

    message_length = strlen(message);
    total_send = 0;
    counter = 0;
    // Sends the query to the server
    while(total_send != message_length)
    {
        for(int i = total_send; i < strlen(message); i++)
        {
            left_message[counter++] = message[i];
        }
        left_message[counter] = '\0';
        if( (current_send = send(socket_fd, left_message, message_length - total_send, 0) ) < 0){
            perror("send() error");
            exit(EXIT_FAILURE);
        }
        total_send += current_send;
        counter = 0;
    }    

    // waits until a response comes from server
    if (recv(socket_fd, dumb_message, 1, 0) < 0)
    {
    }      
    dumb_message[1] = '\0';

    char message_size[50000] = {'0'};
    char message_read[1000000] = {'0'};
    strcpy(message_size, "");
    strcpy(message_read, "");
    int read_size = 0;
    int message_finished = 0;
    // reads until gets size of the message from server
    while(message_finished == 0)
    {
        if ( (read_size = recv(socket_fd, message_read, 1, 0) ) < 0)
        {
            perror("recv() error");
            exit(EXIT_FAILURE);
        }
        if(read_size == 0)
            return ;
        message_read[1] = '\0';
        if(message_read[strlen(message_read) - 1] == 'y')
        {
            message_read[strlen(message_read) - 1] = '\0';
            message_finished = 1;
        }
        strcat(message_size, message_read);
    }
    read_size = atoi(message_size);

    char response[20] = {'0'};
    // sends respose to server
    sprintf(response, "*");
    if( send(socket_fd , response , 1, 0) < 0)
    {
        perror("send() error");
        exit(EXIT_FAILURE);            
    }
 

    char *result_message = (char*)malloc((read_size + 1000000)* sizeof(char)); 
    char *read_message = (char*)malloc((read_size + 1000000)* sizeof(char)); 
    strcpy(result_message, "");
    int data_size = 0, current_size = 0;
    // reads until get the whole message
    while(data_size != read_size)
    {
        if ( (current_size = recv(socket_fd, read_message, read_size - data_size, 0)) < 0)
        {
            perror("recv() error");
            exit(EXIT_FAILURE);           
        }
        strcat(result_message, read_message);
        data_size += current_size;
        strcpy(read_message, "");
    }
    result_message[data_size] = '\0';

    int cn = 0;
    char *context = NULL;
    char tmp[10000] = {'0'};
    
    while((fgets(tmp, MESSAGELENGTH, tmp_fp)) != NULL)
    {
        context = NULL;
        char *ptr = strtok_r(tmp, " ",&context);
        if(atoi(ptr) == id)
            cn++;
    }
    fseek(tmp_fp, 0, SEEK_SET);

    if(cn == query_number)
    {
        strcpy(response,"n");
        finished = 1;
    }
    else
    {
        strcpy(response,"*");
    }
    // sends response to the server
    if( send(socket_fd , response , 1, 0) < 0)
    {
        perror("send() error");
        exit(EXIT_FAILURE);            
    }

    clock_t end = clock();
    int record_n = 0;
    char *tmp_res = (char*)malloc((strlen(result_message) + 1000) * sizeof(char));
    strcpy(tmp_res, result_message);
    char *ct = NULL;
    char *ptr = strtok_r(tmp_res, "\n", &ct);
    // counts records
    while(ptr != NULL)
    {
        ++record_n;
        ptr = strtok_r(NULL, "\n", &ct);
    }
    record_n--;
    free(tmp_res);


    dataset_load_time += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Server’s response to Client-%d is %d records, and arrived in %f seconds\n", id, record_n, dataset_load_time);
    fflush(stdout);


    printf("%s\n", result_message);
    fflush(stdout);

    free(result_message);
    free(read_message);
    free(size_message);
    free(left_message);
}
void create_socket()
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        perror("Cannot create socket");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port_number);
    address.sin_addr.s_addr = inet_addr(ip_adress);


}
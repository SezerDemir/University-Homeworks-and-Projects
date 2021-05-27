#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <wait.h>

#define SEMAPHORE_UNDERFLOW "underflow1535878"
#define SEMAPHORE_OVERFLOW "overflow1535878"
#define SEMAPHORE_BUFFER "buffer1535878"
#define SEMAPHORE_PARENT "parent1535878"
#define SEMAPHORE_CITIZEN "citizen1535878"
#define SEMAPHORE_SENDING "sending1535878"
#define SEMAPHORE_READ "reding1535878"
#define SEMAPHORE_VAC "vac1535878"
#define SEMAPHORE_OLDER "older1535878"
#define SHARED_MEMORY_CLINIC "/clinic_buf1535878"
#define FILELOCATIONSHARED "/shared_file_loc155878"
#define FIFONAMESSHARED "/shared_fifo1535878"
#define VACINATOR_COUNTER_MEMORY_NAME "/shared_vacinator1535878"
#define CITIZEN_COUNTER_MEMORY_NAME "/shared_citizen1535878"
#define SIGINT_COUNTER_MEMORY_NAME "/shared_sigint1535878"
#define FILE_MEMORY_SIZE 1000000 // size for shared memory
#define VACINATOR_COUNTER_MEMORY_SIZE 100000 // size for shared memory
#define CITIZEN_COUNTER_MEMORY_SIZE 10000 // size for shared memory
#define SIGINT_COUNTER_MEMORY_SIZE 100


volatile sig_atomic_t sig_flag = 0;
static void SIGINT_Handler(int signum)
{
    sig_flag = 1;
}




void SIGUSR1_handler(int sig)
{

}


int main(int argc, char * argv[])
{
    int nurse_num = 0, vac_num = 0, cit_num = 0, size_buf = 5, shot_num = 0;
    short int n_flag = 0, v_flag = 0, c_flag = 0, t_flag = 0, i_flag = 0, b_flag = 0;
    char *file_path, c;
    int pid = 1;
    int nurse_number = 0, vacinator_number = 0, citizen_number = 0, total_shot = 0, number_of_vacineted = 0;
    char citizen_fifo_name[100] = {'0'};
    long int fifomemorysize = 0;
    int sigint1 = 0;

    // indicates the type of process "citizen", "nurse", or "vaccinator"
    char type;


    // signal handler init
    struct sigaction sig_USR1;
    memset (&sig_USR1, '\0', sizeof(sig_USR1));
    sig_USR1.sa_handler = &SIGUSR1_handler;
    sigemptyset(&sig_USR1.sa_mask);
    if (sigaction(SIGUSR1, &sig_USR1, NULL) < 0) {
		perror ("sigaction");
		exit(EXIT_FAILURE);
	}

    // blocks SIGUSR1 
    sigset_t blockMask;
    sigemptyset (&blockMask);
	sigaddset (&blockMask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) < 0) {
		perror ("sigprocmask");
		return 1;
	}

    // sigint handler init
    struct sigaction sa;
    sa.sa_handler = SIGINT_Handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction");
    }


    // gets arguments and checks if they're in correct format
    while( (c = getopt(argc, argv, "n:v:c:b:t:i:")) != -1 )
    {
        switch (c)
        {
            case 'n':
                n_flag = 1;
                nurse_num = atoi(optarg);
                if(nurse_num < 2)
                {
                    printf("Number of nurses must be greater than 1!\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'v':
                v_flag = 1;
                vac_num = atoi(optarg);
                if(vac_num < 2)
                {
                    printf("Number of vaccinators must be greater than 1!\n");
                    exit(EXIT_FAILURE);
                }
                // argument format check
                break;

            case 'c':
                c_flag = 1;
                cit_num = atoi(optarg);
                fifomemorysize = cit_num * 30;
                if(cit_num < 3)
                {
                    printf("Number of citizens must be greater than 2!\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'b':
                b_flag = 1;
                size_buf = atoi(optarg);
                if(size_buf < 1)
                {
                    printf("size of buffer cannot be  less than 3!\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 't':
                t_flag = 1;
                shot_num = atoi(optarg);
                total_shot = shot_num;
                if(shot_num < 1)
                {
                    printf("Number of vaccine shots must be greater than 1!\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'i':
                i_flag = 1;
                file_path = optarg;
                if(shot_num < 1)
                {
                    printf("Number of vaccine shots must be greater than 1!\n");
                    exit(EXIT_FAILURE);
                }
                break;

            //handles invalid flags and no-argument flags
            case '?':
                if(optopt == 'n' || optopt == 'v' || optopt == 'c' || optopt == 't' || optopt == 'i')
                {
                    printf("Flag needs an argument!\n");
                    exit(EXIT_FAILURE);
                }
                else
                {
                    printf("Unknown flag !\nValid flags = ('-n', '-v', '-c', '-t', 'i')\n");
                    exit(EXIT_FAILURE);                
                }
                break;
            default:
            {
                perror("getopt() caused an error:\n");
            }
        }
    }   
    // to store the citizen's pid
    int citizen_pid[cit_num], vacinator_pid[vac_num];

    // check buffer size is a valid one
    if(size_buf < shot_num * cit_num + 1)
    {
        printf("Buffer size must be greater than tc+1!\n");
        exit(EXIT_FAILURE);
    }


    // checks all necessary flags are typed or not
    if(n_flag == 0 || v_flag == 0 || c_flag == 0 || b_flag == 0 || t_flag == 0 || i_flag == 0)
    {
        fprintf(stderr, "You must enter the all necessary flags ('-n', '-v', '-c', '-b', '-t', '-i')!\n");
        exit(EXIT_FAILURE);
    }    

    // creates clinic buffer as a named shared memory
    int shm = shm_open(SHARED_MEMORY_CLINIC , O_RDWR | O_CREAT, S_IRWXU);
	if (shm == -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm, size_buf)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr = (char*) mmap(NULL, size_buf, PROT_READ | PROT_WRITE , MAP_SHARED, shm, 0);
	if (addr == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
    strcpy(addr, "");

    // creates file buffer as a named shared memory to store the info of where the last nurse was at the file when it get the vacine
    int shm1 = shm_open(FILELOCATIONSHARED , O_RDWR | O_CREAT, S_IRWXU);
	if (shm1 == -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm1, FILE_MEMORY_SIZE)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr_file = (char*) mmap(NULL, FILE_MEMORY_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, shm1, 0);
	if (addr_file == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
    strcpy(addr_file, "0");

    // creates file buffer as a named shared memory to store the info of where the last nurse was at the file when it get the vacine
    int shm2 = shm_open(FIFONAMESSHARED , O_RDWR | O_CREAT, S_IRWXU);
	if (shm2 == -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm2, fifomemorysize)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr_fifo = (char*) mmap(NULL, fifomemorysize, PROT_READ | PROT_WRITE , MAP_SHARED, shm2, 0);
	if (addr_fifo == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
    strcpy(addr_fifo, "");

    // creates vacinator buffer as a named shared memory to store the info of which vacinator how many vacines applied
    int shm3 = shm_open(VACINATOR_COUNTER_MEMORY_NAME , O_RDWR | O_CREAT, S_IRWXU);
	if (shm3== -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm3, VACINATOR_COUNTER_MEMORY_SIZE)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr_vacinator = (char*) mmap(NULL, VACINATOR_COUNTER_MEMORY_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, shm3, 0);
	if (addr_fifo == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
    strcpy(addr_fifo, "");

    // creates vacinator buffer as a named shared memory to store the info of which vacinator how many vacines applied
    int shm4 = shm_open(CITIZEN_COUNTER_MEMORY_NAME , O_RDWR | O_CREAT, S_IRWXU);
	if (shm4== -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm4, CITIZEN_COUNTER_MEMORY_SIZE)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr_citizen = (char*) mmap(NULL, CITIZEN_COUNTER_MEMORY_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, shm4, 0);
	if (addr_citizen == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
    strcpy(addr_citizen, "");
    {
        char num_citi[1000] = {'0'};
        sprintf(num_citi, "%d", cit_num);
        strcpy(addr_citizen, num_citi);
    }

    // creates sigint buffer as a named shared memory to store the info of pid of sigint process
    int shm5 = shm_open(SIGINT_COUNTER_MEMORY_NAME , O_RDWR | O_CREAT, S_IRWXU);
	if (shm5== -1)
	{
		perror("shm_open");
		exit(EXIT_FAILURE);
	}
    if((ftruncate(shm5, SIGINT_COUNTER_MEMORY_SIZE)) == -1){  
          perror("ftruncate failure");
          exit(EXIT_FAILURE);
    }
	// map shared memory to process address space
	char *addr_sigint = (char*) mmap(NULL, SIGINT_COUNTER_MEMORY_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, shm5, 0);
	if (addr_sigint == ((void *) -1))
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}

    // checks input file exist or not. If it is not terminates the program.
    FILE * file;
    file = fopen(file_path, "r");
    if (file)
    {
        fclose(file);
    }
    else{
        fprintf(stderr, "File does not exist!\n");
        exit(EXIT_FAILURE);
    }


    // semaphore for underflow check
    sem_t *sem_underflow = sem_open(SEMAPHORE_UNDERFLOW, O_CREAT | O_EXCL , 0644, 0);
    if(sem_underflow == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // semaphore for overflow check
    sem_t *sem_overflow = sem_open(SEMAPHORE_OVERFLOW, O_CREAT | O_EXCL, 0644, size_buf);
    if(sem_overflow == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // semaphore for buffer control
    sem_t *sem_buffer = sem_open(SEMAPHORE_BUFFER, O_CREAT | O_EXCL, 0644, 1);
    if(sem_overflow == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // semaphore for citizen message control
    sem_t *sem_citizen = sem_open(SEMAPHORE_CITIZEN, O_CREAT | O_EXCL, 0644, 1);
    if(sem_overflow == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // semaphore for vacinator message control
    sem_t *vac_sem = sem_open(SEMAPHORE_VAC , O_CREAT | O_EXCL, 0644, 1);
    if(vac_sem == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // semaphore to make children wait the parent until it finish it's job
    sem_t *sem_parent = sem_open(SEMAPHORE_PARENT, O_CREAT | O_EXCL, 0644, 0);
    if(sem_parent == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // semaphore for citizen message sending control
    sem_t *sem_sending = sem_open(SEMAPHORE_SENDING, O_CREAT | O_EXCL, 0644, 1);
    if(sem_sending == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    // semaphore for citizen message read control
    sem_t *sem_read = sem_open(SEMAPHORE_READ, O_CREAT | O_EXCL , 0644, 1);
    if(sem_read == SEM_FAILED)
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }


    printf("Welcome to the GTU344 clinic. Number of citizen to vaccinate c=%d with t=%d doses\n", cit_num, shot_num);
    fflush(stdout);

    if(pid > 0 && sig_flag != 1)
    {
        // creates citizen processes
        for(int i = 0;  i < cit_num && sig_flag != 1; i++)
        {
            type = 'c';
            citizen_number++;
            pid = fork();
            // parent adds citizen's pid into array
            if(pid > 0)
                citizen_pid[i] = pid;
            if(pid == 0)
            {
                sprintf(citizen_fifo_name, "./F%d", getpid());
                break;
            }
        }
    }
    // creates vaccinators processes
    if(pid > 0 && sig_flag != 1)
    {
        for(int i = 0;  i < vac_num && sig_flag != 1; i++)
        {
            type = 'v';
            vacinator_number++;
            pid = fork();
            if(pid > 0)
                vacinator_pid[i] = pid;
            if(pid == 0)
                break;
        }
    }
    if(pid > 0 && sig_flag != 1)
    {
        // creates nurse processes
        for(int i = 0;  i < nurse_num && sig_flag != 1; i++)
        {
            type = 'n';
            nurse_number++;
            pid = fork();
            if(pid == 0)
                break;
        }
    }

    if(pid > 0)
    {
        // creates sigint1 process
        pid = fork();
        sigint1 = pid;
        if(pid > 0)
        {
            strcpy(addr_sigint, "");
            {
                char addr_sig[1000] = {'0'};
                sprintf(addr_sig, "%d", sigint1);
                strcpy(addr_sigint, addr_sig);
            }
        }
        else
            type = 's';
    }
    if(pid > 0)
        type = 'p';

    

    // children will wait until parent finish it's shared memory arrengment job
    if(pid == 0)
    {
        sem_wait(sem_parent);
        if(sig_flag == 1)
            sem_wait(sem_parent);
    }
    else
    {
        char path[100] = { '0' };

        // sorts pids in ascending order
        int min = -1;

        for (int i = 0; i < cit_num; i++) 
        {     
            for (int j = i+1; j < cit_num; j++) 
            {     
                if(citizen_pid[i] > citizen_pid[j]) 
                {    
                    min = citizen_pid[i];    
                    citizen_pid[i] = citizen_pid[j];    
                    citizen_pid[j] = min;    
                }     
            }     
        }  

        // creates fifos for citizen
        for(int i = 0; i < cit_num && sig_flag != 1; i++)
        {
            sprintf(path, "./F%d", citizen_pid[i]);
            mkfifo(path, 0666);
        }
    
        // sets up fifoname shared memory
        char fifoname[250] = {'0'};
        char tmp_addr[fifomemorysize];
        strcpy(tmp_addr, "");
        for(int i = 0; i < cit_num; i++)
        {
            sprintf(fifoname, "./F%d*%d,", citizen_pid[i], shot_num);
            strcat(tmp_addr, fifoname);
        }
        strcpy(addr_fifo, tmp_addr);
        // release the children
        for(int i = 0; i < (nurse_num + vac_num + cit_num + 1) * 2; i++)
        {
            sem_post(sem_parent);
        }
    }


    //------------------------------------------------//
    //-------------------NURSE PART-------------------//
    if(type == 'n' && sig_flag != 1)
    {
        while(sig_flag != 1)
        {
            int type_1_count = 0, type_2_count = 0;
            char buff[size_buf];

            if(sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_overflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                break;
            }

            sem_wait(sem_overflow);
            sem_wait(sem_buffer);

            // checks that putting that new vaccine will make buffer full and if it is checks what type we need.
            // lets we have 8 byte buffer so 8 vaccine can be there, so if first 7 vaccine are type 1 last one must be type 2. Otherwise system will be locked
            // since vaccinator cannot give any vaccine to citizen since it would not has one of the types
            strcpy(buff, addr);

            // to determine how many vaccine 1 and 2 pairs are in the buffer before adding the new vaccine
            for(int i = 0 ; i < strlen(buff); i++)
            {
                if(sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_overflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    break;
                }
                if(buff[i] == '1')
                    type_1_count++;
                else
                    type_2_count++;
            }
            if(sig_flag == 1)
            {
                break;
            }

            // gets the vaccine from file
            FILE *fp = fopen(file_path, "r");
            char ch = '0';
            long int loc = 0, last_loc = 0;
            char locaddr[5000] = {'0'};
            char vac[2] = {0};

            // sets location of the file where last nurse got the vacine
            strcpy(locaddr, addr_file);
            loc = atoi(locaddr);
            fseek(fp , 0l, SEEK_END);
            last_loc = ftell(fp);
            // if all file is read then nurses has no job anymore, so they will leave the loop
            if(loc == last_loc || sig_flag == 1)
            {
                fclose(fp);
                sem_post(sem_buffer);
                break;
            }
            fseek(fp, loc, SEEK_SET);
            // gets vacine and removes it from file
            while ((ch = fgetc(fp)) != EOF)
            {
                if(ch == '1' || ch == '2')
                {
                    vac[0] = ch;
                    vac[1] = '\0';
                    break;
                }
            }
            loc = ftell(fp);
            sprintf(locaddr, "%ld", loc);
            strcpy(addr_file, locaddr);
            fclose(fp);
            if(sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_overflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                break;
            }
            // prints the nurse message
            if(strcmp(vac, "1") == 0)
            {
                printf("Nurse %d (pid=%d) has brought vaccine %s: the clinic has %d vaccine1 and %d vaccine2.\n", nurse_number, getpid(), vac, type_1_count + 1, type_2_count);
                fflush(stdout);
            }
            if(strcmp(vac, "2") == 0)
            {
                printf("Nurse %d (pid=%d) has brought vaccine %s: the clinic has %d vaccine1 and %d vaccine2.\n", nurse_number, getpid(), vac, type_1_count, type_2_count + 1);
                fflush(stdout);
            }

            // adds vaccine to clinic buffer
            char tmp_addr[size_buf];
            strcpy(tmp_addr, addr);
            strcat(tmp_addr, vac);
            strcpy(addr, tmp_addr);

            // if a new pair of vaccine (1-2) is in the buffer after adding new vaccine, increases the underflow semaphore to to notify vecinators
            if( (type_1_count < type_2_count && vac[0] == '1') || (type_2_count < type_1_count && vac[0] == '2'))
            {
                sem_post(sem_underflow);
            }
            sem_post(sem_buffer);
        }
    }
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);

    //-----------------------------------------------------//
    //-------------------VACCINATOR PART-------------------//
    if(type == 'v' && sig_flag != 1)
    {
        int finished = 0;
        char send_fifo[200] = {'0'};
        // loops until all vaccines are applied
        while(sig_flag != 1)
        {
            int before_vac = 0;
            int type_1_count = 0, type_2_count = 0;
            char buff[size_buf];
            int vac_left = 0;
            // check if game finished
            if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                sem_post(sem_read);
                sem_post(sem_citizen);
                kill(vacinator_pid[vac_num - 1], SIGUSR1);
                break;
            }

            while(sig_flag != 1)
            {
                before_vac = 0;
                if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_read);
                    sem_post(sem_citizen);
                    break;
                }
                
                // finds if there is citizen that is available
                char tmp1_addr[100000] = {'0'}, tmp2_addr[1000] = {'0'}, tmp3_addr[100000] = {'0'}, new_str[500] = {'0'};

                sem_wait(sem_read);
                
                if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_read);
                    sem_post(sem_citizen);
                    break;
                }

                strcpy(tmp3_addr, "");
                strcpy(tmp1_addr, addr_fifo);
                char *ptr1 = strtok(tmp1_addr, ",");
                int counter = 0;
                int found = 0;
                vac_left = 0;

                while(ptr1 != NULL)
                {           
                    if(sig_flag == 1)
                    {
                        sem_post(sem_underflow);
                        sem_post(sem_buffer);     
                        sem_post(sem_sending);
                        sem_post(sem_read);
                        sem_post(sem_citizen);
                        break;
                    }
                    counter++;
                    if(found == 0)
                    {
                        before_vac = vac_left;
                        strcpy(tmp2_addr, ptr1);
                        char *ptr2 = strtok(tmp2_addr, "*");
                        strcpy(send_fifo, ptr2);
                        ptr2 = strtok(NULL, "*");
                        vac_left = atoi(ptr2);
                        ptr2 = strtok(NULL, "*");
                        // There is no unavailable sign('+') so it can send message to it
                        if(ptr2 == NULL && vac_left > before_vac)
                        {
                            //marks it
                            sprintf(new_str, "%s*%d*+,", send_fifo, vac_left);
                            strcat(tmp3_addr, new_str);
                            found = 1;
                        }   
                        if(found == 1)
                        {
                            strcpy(tmp1_addr, addr_fifo);
                            ptr1 = strtok(tmp1_addr, ",");
                            for(int i = 0; i < counter; i++)
                            {
                                ptr1 = strtok(NULL, ",");
                            }   
                            continue;
                        }
                        else
                        {
                            strcpy(tmp1_addr, addr_fifo);
                            ptr1 = strtok(tmp1_addr, ",");
                            for(int i = 0; i < counter - 1; i++)
                            {
                                ptr1 = strtok(NULL, ",");
                            }   
                            strcat(tmp3_addr, ptr1);
                            strcat(tmp3_addr, ",");
                            ptr1 = strtok(NULL, ",");
                            continue;
                        }
                    }
                    strcat(tmp3_addr, ptr1);
                    strcat(tmp3_addr, ",");
                    ptr1 = strtok(NULL, ",");
                }
                // if sıgint signal received
                if(sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_read);
                    break;
                }
                if(found == 0)
                {
                    // waits until one of the citizens is available
                    sem_post(sem_read);
                    sem_wait(sem_sending);
                }        
                else
                {
                    strcpy(addr_fifo, tmp3_addr);
                    sem_post(sem_read);
                    break;
                }
            }
            if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                sem_post(sem_read);
                sem_post(sem_citizen);
                break;
            }            
            // gets vacines from clinic
            sem_wait(sem_underflow);
            sem_wait(sem_buffer);
            char tmp_addr[size_buf], tmp1_addr[size_buf];
            strcpy(tmp_addr, addr);
            strcpy(tmp1_addr, "");
            
            int first_vac1 = 0, first_vac2 = 0;
            char new_char[2] = {'0'};
            // removes 1 pair of vaccines from clinic buffer
            for(int i = 0; i < strlen(tmp_addr); i++)
            {
                if(sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_citizen);
                    sem_post(sem_read);
                    break;
                }

                if(first_vac1 == 0 && tmp_addr[i] == '1')
                {
                    first_vac1 = 1;
                    continue;
                }
                if(first_vac2 == 0 && tmp_addr[i] == '2')
                {
                    first_vac2 = 1;
                    continue;
                }
                new_char[0] = tmp_addr[i];
                new_char[1] = '\0';
                strcat(tmp1_addr, new_char);
            }

            strcpy(addr, tmp1_addr);

            strcpy(buff, addr);
            // to determine how many vaccine 1 and 2 pairs are in the buffer before adding the new vaccine
            for(int i = 0 ; i < strlen(buff); i++)
            {
                if(sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_read);
                    break;
                }
                if(buff[i] == '1')
                    type_1_count++;
                else
                    type_2_count++;
            }
            if(sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                sem_post(sem_read);
                sem_post(sem_citizen);
                break;
            }
            // releases the clinic buffer for nurses
            sem_post(sem_overflow);
            sem_post(sem_overflow);
            sem_post(sem_buffer);

            char send_pid[50] = {'0'};
            for(int i = 3; i < strlen(send_fifo); i++)
            {   
                if(sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_citizen);
                    sem_post(sem_read);
                    break;
                }
                send_pid[i - 3] = send_fifo[i];
                if(send_fifo[i+1] == '\0')
                    send_pid[i - 2] = '\0';
            }
            
            if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
            {
                sem_post(sem_underflow);
                sem_post(sem_buffer);     
                sem_post(sem_sending);
                sem_post(sem_citizen);
                sem_post(sem_read);
                break;
            }

            char send_message[200] = {'0'};
            sprintf(send_message, "VAC1:%d,VAC2:%d,PID:%d,", type_1_count, type_2_count, getpid());
            printf("Vaccinator %d (pid=%d) is inviting citizen pid=%s to the clinic\n", vacinator_number, getpid(), send_pid);
            fflush(stdout);
            int fd_send; 
            if(sig_flag == 1)
                break;
            if((fd_send = open(send_fifo, O_WRONLY)) == -1 && sig_flag != 1)
                perror("open:");
            if(write(fd_send, send_message, strlen(send_message)) == -1 && sig_flag != 1)
                perror("write:");
            close(fd_send);
            int reciever_pid = atoi(send_pid);
            kill(reciever_pid, SIGUSR1);
            // increases the vaccine number that vacinator applied
            number_of_vacineted++;


            // waits until citizen sends response
            sigsuspend(&mask);

            // removes mark of the citizen to mark it as availebla
            sem_wait(sem_read);
            {
                if(strcmp(addr_fifo, "GAMEFINISHED") == 0 || sig_flag == 1)
                {
                    sem_post(sem_underflow);
                    sem_post(sem_buffer);     
                    sem_post(sem_sending);
                    sem_post(sem_citizen);
                    break;
                }
                
                char tmp1_addr[100000] = {'0'}, tmp3_addr[100000] = {'0'}, new_str[500] = {'0'};
                strcpy(tmp3_addr, "");
                strcpy(tmp1_addr, addr_fifo);
                vac_left--;
                sprintf(new_str, "%s*%d,", send_fifo, vac_left);
                char *ptr1 = strtok(tmp1_addr, ",");
                if(vac_left == 0)
                {
                    ptr1 = strtok(NULL, ",");
                    // game finished
                    if(ptr1 == NULL)
                        finished = 1;
                }
                strcpy(tmp1_addr, addr_fifo);
                ptr1 = strtok(tmp1_addr, ",");
                while(ptr1 != NULL)
                {
                    if(sig_flag == 1)
                        break;
                    if(strstr(ptr1, send_fifo) != NULL)
                    {
                        // if there is no left veccine that citizzen should get, removes it from fifo shared memory, otherwise just removes the available sign('+')
                        if(vac_left != 0)
                            strcat(tmp3_addr, new_str);
                        ptr1 = strtok(NULL, ",");
                        continue;
                    }
                    strcat(tmp3_addr, ptr1);
                    strcat(tmp3_addr, ",");
                    ptr1 = strtok(NULL, ",");
                }
                strcpy(addr_fifo, tmp3_addr);
                if(finished == 1)
                    strcpy(addr_fifo, "GAMEFINISHED");
            }
            sem_post(sem_sending);
            sem_post(sem_read);
            if(sig_flag == 1)
                break;
            if(finished == 1)
            {
                for(int i = 0; i < (vac_num + nurse_num + cit_num) * 2; i++)
                {
                    sem_post(sem_read);
                    sem_post(sem_sending);
                    sem_post(sem_buffer);
                    sem_post(sem_underflow);
                    sem_post(sem_overflow);
                }
                break;
            }
        }
    }

    //---------------------------------------------------//
    //-------------------CITIZEN PART-------------------//
    if(type == 'c' && sig_flag != 1)
    {
        
        int sender_pid = 0;
        int fd_read = open(citizen_fifo_name, O_RDWR, O_NONBLOCK);
        if(fd_read == -1 && sig_flag != 1)
        {
            perror("fd_read");
        }
        char buffer_message[200] = {'0'};
        while(1 && sig_flag != 1)
        {
            strcpy(buffer_message, "");
            sigsuspend(&mask);
            if(sig_flag == 1)
                break;
            if (read(fd_read, buffer_message, 200) != -1 );
            else if(sig_flag != 1)
            {
                perror("read:");
            }
            if(sig_flag == 1)
                break;
            // parsing info from message
            int vac1 =0, vac2=0;
            char tmp_addr[1000] = {'0'};
            strcpy(tmp_addr, buffer_message);
            char *ptr = strtok(tmp_addr, ",");
            char *ptr1 = strtok(ptr, ":");
            ptr1 = strtok(NULL, ":");
            vac1 = atoi(ptr1);

            strcpy(tmp_addr, buffer_message);
            ptr = strtok(tmp_addr, ",");
            ptr = strtok(NULL, ",");
            ptr1 = strtok(ptr, ":");
            ptr1 = strtok(NULL, ":");
            vac2 = atoi(ptr1);

            strcpy(tmp_addr, buffer_message);
            ptr = strtok(tmp_addr, ",");
            ptr = strtok(NULL, ",");
            ptr = strtok(NULL, ",");
            ptr1 = strtok(ptr, ":");
            ptr1 = strtok(NULL, ":");
            sender_pid = atoi(ptr1);

            
            shot_num--;
            if(shot_num == 0 || sig_flag == 1)
            {
                sem_wait(sem_citizen);
                char tmp_addr[500] = {'0'};
                int num = 0;
                strcpy(tmp_addr, addr_citizen);
                num = atoi(tmp_addr);
                num--;
                sprintf(tmp_addr, "%d", num);
                strcpy(addr_citizen, tmp_addr);
                printf("Citizen %d (pid=%d) is vaccinated for the %d times: the clinic has %d vaccine1 and %d vaccine2. The citizen is leaving. Remaining citizens to vaccinate: %d\n", citizen_number, getpid(), total_shot - shot_num, vac1, vac2, num);
                fflush(stdout);
                kill(sender_pid, SIGUSR1);
                sem_post(sem_citizen);
                break;
            }
            else
            {
                printf("Citizen %d (pid=%d) is vaccinated for the %d times: the clinic has %d vaccine1 and %d vaccine2\n", citizen_number, getpid(), total_shot - shot_num, vac1, vac2);
                kill(sender_pid, SIGUSR1);
                fflush(stdout);
            }
        }
    }

    if(type == 'p')
    {
        pid_t wpid;
        int status = 0;
        while ((wpid = wait(&status)) > 0);
    }
    if(type == 'p' && sig_flag == 1)
    {
        pid_t wpid;
        int status = 0;
        while ((wpid = wait(&status)) > 0);
    }


    // vacinators writes their info into vacinator shared memory so parent can print all of their info at the end of the program
    if(type == 'v' && sig_flag == 0)
    {
        sem_wait(vac_sem);
        char tmp_add[1000] = {'0'}, new_str[250] = {'0'};
        strcpy(tmp_add, addr_vacinator);
        sprintf(new_str, "%d,%d,%d,", vacinator_number, getpid(),number_of_vacineted);
        strcat(tmp_add, new_str);
        strcpy(addr_vacinator, tmp_add);
        sem_post(vac_sem);
    }

    if(pid > 0 && sig_flag == 0)
    {
        // parent prints all the info about vaccinators from vaccinator shared memory
        printf("All citizens have been vaccinated.\n");
        fflush(stdout);
        char tmp_addr[VACINATOR_COUNTER_MEMORY_SIZE];
        strcpy(tmp_addr, "");
        strcpy(tmp_addr, addr_vacinator);
        char *ptr = strtok(tmp_addr, ",");
        int vac_number = 0, vac_cit = 0, vac_pid = 0;
        while (ptr != NULL)
        {
            vac_number = atoi(ptr);
            ptr = strtok(NULL, ",");
            vac_pid = atoi(ptr);
            ptr = strtok(NULL, ",");
            vac_cit = atoi(ptr);
            printf("Vaccinator %d (pid=%d) vaccinated %d doses. ", vac_number, vac_pid, vac_cit);
            fflush(stdout);
            ptr = strtok(NULL, ",");
        }
        printf("The clinic is now closed. Stay healthy.\n");
        fflush(stdout);

    }

    if(sig_flag == 0 && type == 'v' && (vacinator_number == 1 || vacinator_number == vac_num))
    {
        int sig = atoi(addr_sigint);
        kill(sig, SIGUSR1);
    }
   
    // closing shared memory    
    if( (munmap(addr, size_buf) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    // closing shared memory    
    if( (munmap(addr_fifo, fifomemorysize) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    // closing shared memory    
    if( (munmap(addr_file, FILE_MEMORY_SIZE) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    // closing shared memory    
    if( (munmap(addr_vacinator, VACINATOR_COUNTER_MEMORY_SIZE) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    // closing shared memory    
    if( (munmap(addr_citizen, CITIZEN_COUNTER_MEMORY_SIZE) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    // closing shared memory    
    if( (munmap(addr_sigint, SIGINT_COUNTER_MEMORY_SIZE) == -1))
    {
        perror("munmap:");
        exit(EXIT_FAILURE);
    }
    close(shm);
    close(shm1);
    close(shm2);
    close(shm3);
    close(shm4);
    close(shm5);

    if(type == 's') 
    {
        sigset_t mask1;
        sigfillset(&mask1);
        sigdelset(&mask1, SIGUSR1);
        sigdelset(&mask1, SIGINT);
        if(sig_flag == 0)
            sigsuspend(&mask1);
        if(sig_flag == 1)
        {
            for(int i = 0 ; i < vac_num;i++)
            {
                kill(vacinator_pid[i], SIGUSR1);
            }
            for(int i = 0 ; i < cit_num;i++)
            {
                kill(citizen_pid[i], SIGUSR1);
            }
            for(int i = 0; i < (vac_num + nurse_num + cit_num) * 2; i++)
            {
                sem_post(sem_read);
                sem_post(sem_sending);
                sem_post(sem_buffer);
                sem_post(sem_underflow);
                sem_post(sem_overflow);
                sem_post(sem_citizen);
                sem_post(sem_parent);
            }
            char send_fifo[250] = {'0'};
            for(int i = 0; i < cit_num; i++)
            {
                sprintf(send_fifo, "./F%d", citizen_pid[i]);
                // dont check errors because that just checks is there any blocked process because of reading to a fifo after SIGINT
                int fd_send = open(send_fifo, O_WRONLY);
                if(fd_send != -1)
                {
                    write(fd_send, "xxxxx", strlen("xxxxx"));
                    close(fd_send);
                }
            }
            for(int i = 0 ; i < vac_num;i++)
            {
                kill(vacinator_pid[i], SIGUSR1);
            }
            for(int i = 0 ; i < cit_num;i++)
            {
                kill(citizen_pid[i], SIGUSR1);
            }
        }
    }

  
    
    if(pid > 0)
    {
        if( shm_unlink(SHARED_MEMORY_CLINIC) == -1)
            perror("shm_unlink:");
        if( shm_unlink(FILELOCATIONSHARED) == -1)
            perror("shm_unlink:");
        if( shm_unlink(FIFONAMESSHARED) == -1)
            perror("shm_unlink:");
        if( shm_unlink(VACINATOR_COUNTER_MEMORY_NAME) == -1)
            perror("shm_unlink:");
        if( shm_unlink(CITIZEN_COUNTER_MEMORY_NAME) == -1)
            perror("shm_unlink:");
        if( shm_unlink(SIGINT_COUNTER_MEMORY_NAME) == -1)
            perror("shm_unlink:");
    }
    // removes citizen fifos
    if(type == 'c')
    {
        if(unlink(citizen_fifo_name) == -1)
        {
            perror("unlink fifo:");
        }
    }

    if(type == 'n' && nurse_number == 1 && sig_flag == 0)
    {
        printf("Nurses have carried all vaccines to the buffer, terminating.\n");
        fflush(stdout);
    }

    sem_close(sem_underflow);
    sem_close(sem_overflow);
    sem_close(sem_buffer);
    sem_close(sem_parent);
    sem_close(sem_sending);
    sem_close(sem_read);
    sem_close(sem_citizen);
    sem_close(vac_sem);
    
    // PARENT CLOSES THE SEMAHORES
    if(pid>0)
    {
        sem_unlink(SEMAPHORE_BUFFER);
        sem_unlink(SEMAPHORE_CITIZEN);
        sem_unlink(SEMAPHORE_OVERFLOW);
        sem_unlink(SEMAPHORE_PARENT);
        sem_unlink(SEMAPHORE_READ);
        sem_unlink(SEMAPHORE_SENDING);
        sem_unlink(SEMAPHORE_UNDERFLOW);
        sem_unlink(SEMAPHORE_VAC);
    }
    exit(EXIT_SUCCESS);

}

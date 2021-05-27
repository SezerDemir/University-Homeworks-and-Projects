#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

struct Node 
{
    struct Node *next;
    char *fileName, *fullPath;
    int found, depth;
    char type;
};
// linked list to store all the pointers those are dynamically allocated. 
struct clean
{
    struct clean *next;
    void *p;
    char t;
    int d_flag;
};

static struct clean *c_head;

int numberCheck(char *string);
int stringComparator(const char *string1, const char *string2);
char * stringCopy(char *stringDest, const char *stringSrc);
int my_strlen(const char *string);
int p_flag_stringCheck(const char *string);
int t_flag_stringCheck(const char *string);
int f_flag_stringCheck(const char *string);
int compareFileNames(const char *file1, const char *file2);
char * convertPermission(const mode_t perm);
char convertType(const mode_t mode);
int findDepth(const char *string);
int findFile(char *path, const char *fileName, const long long int size, const char *fileType, const char *permission, const short int link, short int depth, const short int *flags, struct Node *head);
void displayPath(char * string, const short int w_depth, struct Node *head);
char * stringSlashAdder(char *string1);
void displayTree(struct Node *head, const int w_depth, char *w_arg);
char * stringAppend(char *string1, char *string2);
char * stringAppend2(char *string1, char *string2);
int toUpper(int c);
int toLower(int c);
void myWrite(char *error, int n);
struct clean *getNode();
void freeResource();

void signalHandler(int s)
{
    char *error = "\nPressed CTRL-C! Program terminated and resources freed!\n";
    if(write(1, error, my_strlen(error)) == -1)
    {
        write(2, "Something has gone wrong in writing process\n", 46);
    }
    freeResource();
    exit(EXIT_SUCCESS);
}

int main(int argc, char * argv[]){
    // In case of CTRL-C pressed, it handles with signal by signalHandler function
    struct sigaction sig;
    sig.sa_handler = &signalHandler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    if(sigaction(SIGINT, &sig, NULL) == -1)
    {
        char *message = "Something has gone wrong in sigaction method!";
        myWrite(message, my_strlen(message));
        freeResource();
        exit(EXIT_FAILURE);        
    }

    signal(2, signalHandler);
    c_head = (struct clean*)calloc(1, sizeof(struct clean));
    c_head->next = NULL;
    c_head->p = NULL;
    // variable initialization
    short int c = 0;
    short int f_flag = 0, b_flag = 0, t_flag = 0, p_flag = 0, l_flag = 0, w_flag = 0;
    short int flags[5];
    // those are to store argument values
    char * f_arg = NULL, * p_arg = NULL, * w_arg = NULL , *t_arg = NULL;
    int l_arg = -1;
    long long int b_arg;

    // gets arguments and checks if they're in correct format
    while( (c = getopt(argc, argv, "f:b:t:p:l:w:")) != -1 )
    {
        switch (c)
        {
            case 'w':
                w_flag = 1;
                w_arg = optarg;
                break;

            case 'f':
                f_flag = 1;
                f_arg = optarg;
                // argument format check
                if(f_flag_stringCheck(optarg) == -1)
                {
                    char *error = "-f flag must take a file name as an argument that doesn't includes invalid character of '/' in the POSIX!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                break;

            case 'b':
                b_flag = 1;
                b_arg = atoi(optarg);
                // argument format check
                if(numberCheck(optarg) == -1 || b_arg < 0)
                {
                    char *error = "-b flag must take a positive integer value as argument!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                break;

            case 't':
                t_flag = 1;
                t_arg = optarg;
                // argument format check
                if(t_flag_stringCheck(optarg) == -1)
                {
                    char *error = "-t flag must take a character as an argument that is one of the"
                            " 'd', 's', 'b', 'c', 'f', 'p', 'l' characters!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                p_flag = 1;
                p_arg = optarg;
                // argument format check
                if(p_flag_stringCheck(optarg) == -1)
                {
                    char *error = "Invalid permission form! -p flag must take an string value as an argument that contains 9 characters and each character must be one of the"
                            " 'w', 'r', 'x', '-' characters!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                break;       

            case 'l':
                l_flag = 1;
                l_arg = atoi(optarg);
                // argument format check
                if(numberCheck(optarg) == -1 || l_arg < 0)
                {
                    char *error = "-l flag must take an positive integer value as argument!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                break;     
            //handles invalid flags and no-argument flags
            case '?':
                if(optopt == 'w' || optopt == 'f' || optopt == 'b' || optopt == 't' || optopt == 'p' || optopt == 'l')
                {
                    char *error = "Flag needs an argument!\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
                else
                {
                    char *error = "Unknown flag !\nValid flags = ('-w', '-l', '-p', '-b', '-f', '-t')\n";
                    myWrite(error, my_strlen(error));
                    freeResource();
                    exit(EXIT_FAILURE);                
                }
                break;

            default:
            {
                char *error = "getopt() caused an error\n";
                myWrite(error, my_strlen(error));
                freeResource();
                exit(EXIT_FAILURE);
            }
        }
    }
    flags[0] = f_flag;
    flags[1] = b_flag;
    flags[2] = t_flag;
    flags[3] = p_flag;
    flags[4] = l_flag;
    // -w and at least one of the other flags are mandotory so if they're not entered error will be printed
    if(w_flag == 0 || (f_flag == 0 && b_flag == 0 && t_flag == 0 && p_flag == 0 && l_flag == 0))
    {
        char *error = "You must enter the -w and at least one of the other flags ('-l', '-p', '-b', '-f', '-t')!\n";
        myWrite(error, my_strlen(error));
        freeResource();
        exit(EXIT_FAILURE);
    }
    int w_depth = findDepth(w_arg);
    // creates head for linked list
    struct Node *head = (struct Node *)malloc(sizeof(struct Node));
    head->fullPath = NULL;
    head->fileName = NULL;
    head->next = NULL;    

    struct clean *p0 = getNode();
    p0->p = head;
    p0->t = 'n';

    char *w_arg_copy = NULL;
    w_arg_copy = stringCopy(w_arg_copy, w_arg);

    // checks dir is a valid one
    DIR *dir = opendir(w_arg);

    if(dir == NULL)
    {
        char *message = "Can't open directory. No such a directory to search or No permission can cause it\n";
        if(write(2, message, my_strlen(message)) == -1)
        {
            message = "Something has gone wrong in writing process!";
            write(2, message, my_strlen(message));
        }        
        freeResource();
        exit(EXIT_FAILURE);
    }
    closedir(dir);

    findFile(w_arg, f_arg, b_arg, t_arg, p_arg, l_arg, 0, flags, head);  

    displayPath(w_arg_copy, w_depth, head);
    freeResource();
    return 0;
}
// creates new node to cleaning linked list and returns it
struct clean *getNode()
{
    struct clean *newNode = (struct clean *)calloc(1, sizeof(struct clean));
    newNode->next = NULL;
    newNode->p = NULL;
    struct clean *tmp = c_head;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = newNode;
    return newNode;
}

void freeResource()
{
    struct clean *tmp = c_head;
    if(c_head->next == NULL)
    {
        free(c_head);
        return;
    }
    while(tmp->next != NULL)
    {
        struct clean *tmp2 = tmp;
        if(tmp->next->p != NULL && tmp->next->t == 's')
        {
            free((char*)tmp->next->p);
        }
        else if(tmp->next->p != NULL && tmp->next->t == 'n')
        {
            free((struct Node*)tmp->next->p);
        }
        else if(tmp->next->p != NULL && tmp->next->t == 'd')
        {
            free((DIR*)tmp->next->p);
        }
        tmp = tmp->next;
        free(tmp2);
    }
    free(tmp);

}

// Checks whether strings are equal or not, if they're it returns 0 otherwise -1
int stringComparator(const char *string1, const char *string2)
{
    if(my_strlen(string1) != my_strlen(string2))
        return -1;
    while(*string1 != '\0' && *string2 != '\0' )
    {
        if(*string1 != *string2)
            return -1;
        ++string1;
        ++string2;
    }       
    return 0;
}

void myWrite(char *error, int n)
{
    if((write(2, error, n ) == -1))
    {
        write(2, "Something has gone wrong at writing function\n", 46);
    }
}
// copies the context of an array into another one
char * stringCopy(char *stringDest, const char *stringSrc)
{
    char *s = (char*)calloc((my_strlen(stringSrc) + 1) , sizeof(char));
    struct clean *p0 = getNode();
    p0->p = s;
    p0->t = 's';
    stringDest = s;
    while(*stringSrc != '\0')
    {
        *stringDest = *stringSrc;
        stringDest++;
        stringSrc++;
    }
    *stringDest = '\0';
    return s;
}

// checks a string contains of full of numbers, if not then it return -1 otherwise 0
int numberCheck(char *string)
{
    for(int i = 0 ; i < my_strlen(string) ; i++)
    {     
        if(string[i] < '0' || string[i] > '9' )
            return -1;
    }
    return 0;
}

// returns the length of the string
int my_strlen(const char *string)
{
    int l = 0;
    if(string != NULL)
        while (string[l++] != '\0');
    else
        return 0;
    return l - 1;
}

// checks -p flag's argument is in correct format, if not then it return -1 otherwise 0
int p_flag_stringCheck(const char *string)
{
    int len = my_strlen(string);
    if(len != 9)
        return -1;
    for(int i = 0; i < len; i++)
    {
        if(string[i] != '-' && string[i] != 'x' && string[i] != 'r' && string[i] != 'w')
            return -1;
    }
    return 0;
}

// checks -t flag's argument is in correct format, if not then it return -1 otherwise 0
int t_flag_stringCheck(const char *string)
{
    if(my_strlen(string) != 1)
        return -1;
    if (string[0] != 'd' && string[0] != 's' && string[0] != 'b' && string[0] != 'c' && string[0] != 'f' && string[0] != 'p' && string[0] != 'l' ) 
        return -1;
    return 0;
}

// checks -f flag's argument is in correct format, if not then it return -1 otherwise 
// characters of '/' cannot be used as file names in the POSIX 
int f_flag_stringCheck(const char *string)
{
    int len = my_strlen(string);
    for(int i = 0; i < len; i++)
    {
        if(string[i] == '/')
            return -1;
    }
    return 0;
}   

int toUpper(int c)
{
    if(c >= 'a' && c <= 'z')  //Only if it's a lower letter
        return c - 32;
    return c;
}

int toLower(int c)
{
    if(c >= 'A' && c <= 'Z')  //Only if it's a lower letter
        return c + 32;
    return c;
}

// compares file name ,that's entered as argumenet, to another name with respect to regular expression of '+'   
int compareFileNames(const char *file1, const char *file2)
{
    int len1 = my_strlen(file1), len2 = my_strlen(file2);
    int flag_exp = 0, flag_last = 0;
    int counter = 0, counter2 = 0, counter3 = 0;

    for(int i = 0, j = 0; i < len1 && j < len2; i++, j++)
    {
        if(file1[i] != toUpper(file2[j]) && file1[i] != toLower(file2[j]))
            return -1;
        // if next character of argument file-name is '+' flag will be 1 
        if( (i + 1) < len1 && file1[i + 1] == '+')
        {
            flag_exp = 1;
            if(i + 1 == len1 - 1)
                flag_last = 1;
        }
        if(flag_exp)
        {
            counter = 0;
            counter2 = 0;
            for(int w = i + 2; w < len1; w++)
            {
                if(file1[w] == toUpper(file1[i]) || file1[w] == toLower(file1[i]))
                    counter++;
                else
                    break;
            }
            for(int w = j + 1; w < len2; w++)
            {
                if(file2[w] == toUpper(file1[i]) || file2[w] == toLower(file1[i]))
                    counter2++;
                else
                    break;
            }
            for(int w = j + 1; w < len2; w++)
            {
                if(file2[w] != toUpper(file1[i]) || file2[w] != toLower(file1[i]))
                    counter3++;
            }
            if(flag_last == 1 && counter3 == 0)
                return 0;
            if(counter > counter2)
                return -1;
            else
            {
                for(int z = j + 1; z < len2; z++)
                {
                    if(file2[z] != toUpper(file1[i]) &&  file2[z] != toLower(file1[i]) )
                    {
                        j = z - 1;
                        break;
                    }
                }
                for(int z = i + 2; z < len1; z++)
                {
                    if(file1[z] != toUpper(file1[i]) &&  file1[z] != toLower(file1[i]) )
                    {
                        i = z - 1;
                        break;
                    }
                }               
            }
            counter = 0;
            counter2 = 0;
            counter3 = 0;
            flag_exp = 0;
            flag_last = 0;
        }
        if( (i < (len1 - 1) && j == (len2 - 1)) || (i == (len1 - 1) && j < (len2 - 1) ) )
        {
            return -1;
        }
    }
    return 0;
}

// to convert permission from octav values to string form
char* convertPermission(const mode_t perm)
{
    char *mode_string = calloc(12, sizeof(char));
    struct clean *p0 = getNode();
    p0->p = mode_string;
    p0->t = 's';
    if(perm & S_IRUSR)
        mode_string[0] = 'r';
    else
        mode_string[0] = '-';
    if(perm & S_IWUSR)
        mode_string[1] = 'w';
    else
        mode_string[1] = '-';
    if(perm & S_IXUSR)
        mode_string[2] = 'x';
    else
        mode_string[2] = '-';
    if(perm & S_IRGRP)
        mode_string[3] = 'r';
    else
        mode_string[3] = '-';
    if(perm & S_IWGRP)
        mode_string[4] = 'w';
    else
        mode_string[4] = '-';
    if(perm & S_IXGRP)
        mode_string[5] = 'x';
    else
        mode_string[5] = '-';
    if(perm & S_IROTH)
        mode_string[6] = 'r';
    else
        mode_string[6] = '-';
    if(perm & S_IWOTH)
        mode_string[7] = 'w';
    else
        mode_string[7] = '-';
    if(perm & S_IXOTH)
        mode_string[8] = 'x';
    else
        mode_string[8] = '-';
    mode_string[9] = '\0';
    return mode_string;
}
// checks type of the file and returns associated character to it 
char convertType(const mode_t mode)
{
    char c;
    if((mode & S_IFMT) == S_IFBLK)
        c = 'b';
    else if((mode & S_IFMT) == S_IFCHR)
        c = 'c';
    else if((mode & S_IFMT) == S_IFDIR)
        c = 'd';
    else if((mode & S_IFMT) == S_IFIFO)
        c = 'p';
    else if((mode & S_IFMT) == S_IFLNK)
        c = 'l';
    else if((mode & S_IFMT) == S_IFREG)
        c = 'f';
    else if((mode & S_IFMT) == S_IFSOCK)
        c = 's';

    return c;
}

int FileEqualityCheck(const char *fileName, const short int size, const char *fileType, const char *permission, const short int link, const short int *flags)
{
    int x = 0;
    return x;
}

// finds how many / sign in there, to find how many folder are there until reach the file
int findDepth(const char *string)
{
    int counter = 0;
    for(int i = 0; i < my_strlen(string); i++)
    {
        if(string[i] == '/')
            counter++;
    }
    return counter;
}

// returns proper presign
char *preSign(int depth)
{
    int counter = 1;
    int i = 1;
    if(depth == 1)
    {
        char *sign = (char *)malloc(4 * sizeof(char));
        struct clean *p0 = getNode();
        p0->p = sign;
        p0->t = 's';
        sign[0] = '|';
        sign[1] = '-';
        sign[2] = '-';
        sign[3] = '\0';
        return sign;
    }
    char *sign = (char *)calloc(depth * 2 + 2, sizeof(char));
    struct clean *p0 = getNode();
    p0->p = sign;
    p0->t = 's';

    sign[0] = '|';
    while(counter < depth + 1)
    {
        sign[i++] = '-';
        sign[i++] = '-';
        counter++;
    }
    sign[i] = '\0';
    return sign;
}
// adds new node to Node linkedList
void addItem(struct Node* head,char *fileName, int depth)
{
    struct Node *newNode = (struct Node *)calloc(1, sizeof(struct Node));    
    newNode->fileName = stringCopy(newNode->fileName, fileName);
    newNode->fullPath = NULL;
    newNode->next = NULL;
    newNode->depth = depth;

    struct clean *p0 = getNode();
    p0->p = newNode;
    p0->t = 'n';

    struct Node *iter = head;
    while(iter->next != NULL)
    {
        iter = iter->next;
    }

    iter->next = newNode;
}

int checkItem(struct Node *head, const char *string, int depth)
{
    struct Node *iter = head;
    while(iter->next != NULL)
    {
        if(stringComparator(iter->next->fileName, string) == 0 && iter->next->depth == depth)
            return 0;
        iter = iter->next;
    }
    return -1;
}

char *messageCreator(char *message)
{
    char *myMessage = (char *)calloc((my_strlen(message) + 2), sizeof(char));
    struct clean *p0 = getNode();
    p0->p = myMessage;
    p0->t = 's';

    char *tmp = myMessage, *tmp2 = message;
    while(*tmp2 != '\0')
    {
        *tmp = *tmp2;
        tmp++;
        tmp2++;
    }
    *tmp++ = '\n';
    tmp = '\0';
    return myMessage;

}

// prints out all the files those are found in a well designed tree structure
void displayPath(char * w_arg, const short int w_depth, struct Node *head)
{
    int w_slash_number = findDepth(w_arg);
    int foundNumber = 0; // number of founded file
    char * presign;
    struct Node *checkList = (struct Node *)calloc(1, sizeof(struct Node));
    checkList->next = NULL;
    checkList->fileName = NULL;
    checkList->fullPath = NULL;
    struct clean *p0 = getNode();
    p0->p = checkList;
    p0->t = 'n';

    struct Node *iter = head;
    while(iter->next != NULL)
    {
        foundNumber++;
        iter = iter->next;
    }
    if(foundNumber == 0)
    {
        char *message = "No file found\n";
        if(write(1, message, my_strlen(message)) == -1)
        {
            char *mes = "Something has gone wron in writing process\n";
            write(2, mes, my_strlen(mes));
        }
        return;
    } 
    {
        char* message = messageCreator(w_arg);
        if(write(1, message, my_strlen(message)) == -1)
        {
            char *mes = "Something has gone wron in writing process\n";
            write(2, mes, my_strlen(mes));            
        }
    }


    iter = head;
    struct Node *iter_check = checkList;
    char *basePath = stringSlashAdder(w_arg);
    char *current_path;

    while(iter->next != NULL)
    {
        int slash_counter = 0, flag_word = 0, s_index = 0, e_index = 0;
        current_path = stringCopy(current_path, basePath);
        
        // if file printed before it skips it
        if(iter->next->found == 1)
        {
            iter = iter->next;
            continue;
        }
        // if file has same path with -w argumant then it's printed immediatily
        if(stringComparator(iter->next->fullPath, basePath) == 0)
        {
            iter->next->found = 1;
            presign = preSign(1);
            char *message = stringAppend2(presign,  iter->next->fileName);
            message = messageCreator(message);
            if(write(1, message, my_strlen(message)) == -1)
            {
                char *mes = "Something has gone wron in writing process\n";
                write(2, mes, my_strlen(mes));                            
            }     
            if(iter->next->type == 'd')
                addItem(iter_check, iter->next->fileName, findDepth(iter->next->fullPath));
            continue;
        }
        
        for(int i = 0; i < my_strlen(iter->next->fullPath); i++)
        {
            if(iter->next->fullPath[i] == '/')
            {
                slash_counter++;
                flag_word = 1;
            }
            if(slash_counter > w_depth && flag_word == 1)
            {
                s_index = i + 1;
                if(iter->next->fullPath[s_index] != '\0')
                {
                    for(int j = i + 1; j < my_strlen(iter->next->fullPath); j++)
                    {
                        if(iter->next->fullPath[j] == '/')
                        {
                            i = j - 1;
                            e_index = j;
                            flag_word = 0;
                            break;
                        }
                    }
                }
                else
                {
                    break;
                }
                // gets folder name
                char *c = (char *)calloc(e_index - s_index + 2, sizeof(char));
                struct clean *p0 = getNode();
                p0->p = c;
                p0->t = 's';

                int c_index = 0;
                for(int j = s_index; j < e_index; j++)
                {
                    c[c_index++] = iter->next->fullPath[j];
                }
                c[c_index] = '\0';
                current_path = stringAppend(current_path, c);
                if(checkItem(iter_check, c, findDepth(current_path) - 1) == -1)
                {
                    addItem(iter_check, c, findDepth(current_path) - 1);
                    presign = preSign(slash_counter - w_slash_number);
                    char *message = stringAppend2(presign,  c);
                    message = messageCreator(message);
                    if(write(1, message, my_strlen(message)) == -1)
                    {
                        char *mes = "Something has gone wron in writing process\n";
                        write(2, mes, my_strlen(mes));                            
                    }     
                }
                struct Node *tmp_iter = head;
                while (tmp_iter->next != NULL)
                {
                    if(tmp_iter->next->type == 'd')
                    {
                        if(checkItem(iter_check, tmp_iter->next->fileName, findDepth(tmp_iter->next->fullPath)) == 0)
                        {
                            tmp_iter->next->found = 1;
                            tmp_iter = tmp_iter->next;
                            continue;
                        }
                    }
                    if(stringComparator(tmp_iter->next->fullPath, current_path) == 0 && tmp_iter->next->found == 0)
                    { 
                        tmp_iter->next->found = 1;
                        if(tmp_iter->next->type == 'd')
                            addItem(iter_check, tmp_iter->next->fileName, findDepth(tmp_iter->next->fullPath));
                        presign = preSign(findDepth(tmp_iter->next->fullPath) - w_slash_number);
                        char *message = stringAppend2(presign,  tmp_iter->next->fileName);
                        message = messageCreator(message);
                        if(write(1, message, my_strlen(message)) == -1)
                        {
                            char *mes = "Something has gone wron in writing process\n";
                            write(2, mes, my_strlen(mes));                            
                        }     ;
                    }
                    tmp_iter = tmp_iter->next;
                }
            }
        }
        iter = iter->next;
    }
}

// adds / sign at the end of the string
char * stringSlashAdder(char *string1)
{
    char *dname = (char*)calloc((my_strlen(string1) + 3), sizeof(char));
    struct clean *p0 = getNode();
    p0->p = dname;
    p0->t = 's';
    char *tmp1 = string1, *tmp3 = dname;
    while (*tmp1 != '\0') {
        *tmp3 = *tmp1;
        tmp3++;
        tmp1++;
    }
    *tmp3++ = '/';
    *tmp3 = '\0';
    return dname;
}

// appends a string to at the end of another one and adds / character at the end the final string
char * stringAppend(char *string1, char *string2)
{
    char *dname = (char*)calloc((my_strlen(string1) + my_strlen(string2) + 3), sizeof(char));
    struct clean *p0 = getNode();
    p0->p = dname;
    p0->t = 's';
    char *tmp1 = string1, *tmp3 = dname;
    char *tmp2 = string2;
    while (*tmp1 != '\0') {
        *tmp3 = *tmp1;
        tmp3++;
        tmp1++;
    }
    while (*tmp2 != '\0') {
        *tmp3 = *tmp2;
        tmp3++;
        tmp2++;
    }
    *tmp3 = '/';
    tmp3++;
    *tmp3 = '\0';
    return dname;
}

// appends a string to at the end of another one 
char * stringAppend2(char *string1, char *string2)
{
    char *dname = (char*)calloc((my_strlen(string1) + my_strlen(string2) + 3), sizeof(char));
    struct clean *p0 = getNode();
    p0->p = dname;
    p0->t = 's';
    char *tmp1 = string1, *tmp3 = dname;
    char *tmp2 = string2;
    while (*tmp1 != '\0') {
        *tmp3 = *tmp1;
        tmp3++;
        tmp1++;
    }
    while (*tmp2 != '\0') {
        *tmp3 = *tmp2;
        tmp3++;
        tmp2++;
    }
    *tmp3 = '\0';
    return dname;
}
// appends / character at the end of the string1 and adds string2 after that
char * stringAppend3(char *string1, char *string2)
{
    char *dname = (char*)calloc((my_strlen(string1) + my_strlen(string2) + 3), sizeof(char));
    struct clean *p0 = getNode();
    p0->p = dname;
    p0->t = 's';
    char *tmp1 = string1, *tmp3 = dname;
    char *tmp2 = string2;
    while (*tmp1 != '\0') {
        *tmp3 = *tmp1;
        tmp3++;
        tmp1++;
    }
    *tmp3++ = '/';
    while (*tmp2 != '\0') {
        *tmp3 = *tmp2;
        tmp3++;
        tmp2++;
    }

    *tmp3 = '\0';
    return dname;
}

// finds all files those satisfies conditions and store their info in Node linked list
int findFile(char *path, const char *fileName, const long long int size, const char *fileType, const char *permission, const short int link, short int depth, const short int *flags, struct Node *head)
{
    off_t fileSize;
    char *fullPathFileName;
    char currentFileType = 'x';
    char *currentFileName;
    char *perm;
    int err = 0;
    int found = 1;
    short int currentFileLinks;
    struct dirent *dp;
    DIR *dir = opendir(path);
    struct clean *p0 = getNode();
    p0->p = dir;
    p0->t = 'd';
    p0->d_flag = 1;
    //since this method tries open each file in current path sometimes it tries to open regular files with open dir. Because of that it ignores some errors like EACCES, EBADF, ENOTDIR.
    if(dir == NULL && (errno == EMFILE || errno == ENFILE || errno == ENOMEM) )
    {
        char *message = "Seacrhing cannot continue because of EMFILE, ENFILE, or ENOMEM errors.\n";
        if(write(2, message, my_strlen(message)) == -1)
        {
            message = "Something has gone wrong in writing process!";
            write(2, message, my_strlen(message));
            freeResource();
            exit(EXIT_FAILURE);
        }
        freeResource();
        exit(EXIT_FAILURE);
    }    
    if(!dir)
    {
        return 0;
    }
    // since the only error can be occured because of the readdir is EBADF, we don't need to print it because this algorithm tries to open each file in directory
    // so it's normal to get EBADF error in this process (EBADF = Invalid directory stream descriptor)
    while( (dp = readdir(dir)) != NULL )
    {
        struct stat buf;
        if(stringComparator(dp->d_name, ".") != 0 && stringComparator(dp->d_name, "..") != 0)
        {
            fullPathFileName = stringAppend3(path, dp->d_name);
            // comparasion of file meta-data
            if( (err = lstat(fullPathFileName, &buf)) != -1)
            {
                found = 1;
                if(dp->d_name != NULL)
                {
                    currentFileName = stringCopy(currentFileName, dp->d_name);
                }
                fileSize = buf.st_size;
                currentFileType = convertType(buf.st_mode);
                perm = convertPermission(buf.st_mode);
                currentFileLinks = buf.st_nlink;      
                if(currentFileType == 'd')         
                    depth = depth + 1;
                if(flags[0] == 1 && compareFileNames(fileName, currentFileName) == -1)
                    found = 0;
                if(flags[1] == 1 && (long long int)fileSize != size)
                {
                    found = 0;
                }
                if(flags[2] == 1 && currentFileType != fileType[0])
                    found = 0;
                if(flags[3] == 1 && stringComparator(perm, permission) != 0)
                    found = 0;
                if(flags[4] == 1 && link != currentFileLinks)
                    found = 0;
                
                if(found)
                {
                    struct Node *tmp = (struct Node*) malloc(sizeof(struct Node));
                    struct clean *p0 = getNode();
                    p0->p = tmp;
                    p0->t = 'n'; 

                    char *n = stringCopy(n, currentFileName);
                    tmp->fileName = n;
                    if(path != NULL)
                    {
                        char *name = stringSlashAdder(path);   
                        tmp->fullPath = name;
                    }
                    tmp->found = 0;
                    if(currentFileType == 'd')
                    {
                        tmp->depth = findDepth(tmp->fullPath);
                        tmp->type = 'd';
                    }
                    else
                    {
                        tmp->type = 'o';
                        tmp->depth = depth;
                    }
                    tmp->next = NULL;
                    struct Node *prev = head;
                    while (prev->next != NULL) 
                    {
                        prev = prev->next;
                    }
                    prev->next = tmp;
                }
                      
            }           
//since this method tries open each file in current path sometimes it tries to open regular files with open dir. Because of that it ignores some errors like EACCES, EBADF, ENOTDIR.    
            if(err == -1 && (errno == ENOMEM || errno == EINVAL))
            {
                char *message = "Out of memory\n";
                if(write(2, message, my_strlen(message)) == -1) 
                {
                    message = "Something has gone wrong in write function!";
                    write(2, message, my_strlen(message));
                    freeResource();
                    exit(EXIT_FAILURE);
                }
            }    
            findFile(fullPathFileName, fileName, size, fileType, permission, link, depth, flags, head);
        }
    }
    p0->p = NULL;
    closedir(dir);
    return 0;
}
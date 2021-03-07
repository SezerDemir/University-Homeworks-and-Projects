#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct prime_number_list{
	int number;
	struct prime_number_list *next;
}prime_number_list;

int read_file(FILE *);
void init_link_list(prime_number_list *);
float list_prime(prime_number_list *,FILE*,clock_t*,clock_t*,clock_t*,clock_t*);
void init_array(int *);
float list_prime_array(int *,FILE *,clock_t*,clock_t*,clock_t*,clock_t*);

int main(){
	FILE *file_write_linked_list=fopen("output_prime_LiknedList.txt","w" );
	FILE *file_write_array=fopen("output_prime_dynamic_array.txt","w" );
	float writing_time_linked_list,writing_time_array;
	clock_t time_linked_list_1,time_linked_list_2,time_linked_list_3,time_array1,time_array2,time_array3,beginning_time;
	prime_number_list *head=(prime_number_list*)malloc(sizeof(prime_number_list));/*declares the first node of list */
	prime_number_list *starting_ptr=head; /* declares a second pointer to hold first node's adress on it */
	int *arr=(int*)malloc(1000000*sizeof(int));
	
	/*Linked list is initialized and lists prime numbers in linked list  */	
	init_link_list(head);
	writing_time_linked_list=list_prime(starting_ptr,file_write_linked_list,&time_linked_list_1,&time_linked_list_2,&time_linked_list_3,&beginning_time);
	
	/*array is initialized and lists prime numbers in array  */
	init_array(arr); 
	writing_time_array=list_prime_array(arr,file_write_array,&time_array1,&time_array2,&time_array3,&beginning_time);
	free(arr);
	
	/*Process time is written on file */
	fprintf(file_write_linked_list,"%f miliseconds for linked list in 250.000 bound\n",(float)time_linked_list_1);
	fprintf(file_write_linked_list,"%f miliseconds for linked list in 500.000 bound\n",(float)time_linked_list_2);
	fprintf(file_write_linked_list,"%f miliseconds for linked list in 1.000.000 bound\n",(float)time_linked_list_3);
	fprintf(file_write_linked_list,"%f miliseconds to write file of linked list\n",(float)writing_time_linked_list);
	fprintf(file_write_array,"%f miliseconds for array in 250.000 bound\n",(float)time_array1);
	fprintf(file_write_array,"%f miliseconds for array in 500.000 bound\n",(float)time_array2);
	fprintf(file_write_array,"%f miliseconds for array in 1.000.000 bound\n",(float)time_array3);
	fprintf(file_write_array,"%f miliseconds to write file of array\n",(float)writing_time_array);
	printf("%f miliseconds for linked list in 250.000 bound\n",(float)time_linked_list_1);
	printf("%f miliseconds for linked list in 500.000 bound\n",(float)time_linked_list_2);
	printf("%f miliseconds for linked list in 1.000.000 bound\n",(float)time_linked_list_3);
	printf("%f miliseconds to write file of linked list\n",(float)writing_time_linked_list);
	printf("%f miliseconds for array in 250.000 bound\n",(float)time_array1);
	printf("%f miliseconds for array in 500.000 bound\n",(float)time_array2);
	printf("%f miliseconds for array in 1.000.000 bound\n",(float)time_array3);
	printf("%f miliseconds to write file of array\n",(float)writing_time_array);
	fclose(file_write_linked_list);
	fclose(file_write_array);
	return 0;
}
/*this function reads each line of file */
int read_file(FILE *file){
	int data;
	if(fscanf(file,"%d,00",&data)!=EOF){
		return data;
	}
	else{
		return -1;
	}
}
/*adds all numbers into linked list from file */
void init_link_list(prime_number_list *head){
	FILE *file=fopen("data.txt","r");
	int data=0,i,counter=0;	
	while(data!=1000000){
		data=read_file(file);
		prime_number_list *new_node=(prime_number_list *)malloc(sizeof(prime_number_list));
		head->number=data;
		head->next=new_node;
		head=new_node;
		
	}
	head->next=NULL;	
	fclose(file);
}

float list_prime(prime_number_list * starting_ptr,FILE *file,clock_t *time1,clock_t *time2,clock_t *time3,clock_t *time4){
	*time4=clock()/CLOCKS_PER_SEC;
	clock_t time_file;
	float time_delta=0.0;
	int num,flag=1,i,prime_flag=1;
	prime_number_list * temp_ptr; 
	while(flag){
		if(starting_ptr->next==NULL){ /*If there is no more node in list , loop is over */
			break;
		}
		
		num=starting_ptr->number;
		if(num==250000){
			*time1=clock()/CLOCKS_PER_SEC;
			*time1=(*time1-*time4)*1000.0; /*determines time of first 250000 numbers */
		}else if(num==500000){
			*time2=clock()/CLOCKS_PER_SEC;
			*time2=(*time2-*time4)*1000.0; /*determines time of first 500000 numbers */
		}else if(num==1000000){
			*time3=clock()/CLOCKS_PER_SEC;
			*time3=(*time3-*time4)*1000.0; /*determines time of first 1000000 numbers */
		}
		for(i=2;i<num/2;i++){
			if(num%i==0){
				prime_flag=0;
				i=num;
			}
		}
		if(!(num<2)){
			if(prime_flag==1){ /* If num is a prime number, that number is displayed on screen and writes on file */
				printf("%d\n",num);
				time_file=clock();
				fprintf(file,"%d\n",num);
				time_file=(clock()-(float)time_file)/CLOCKS_PER_SEC*1000.0;
				time_delta+=(float)time_file;
			}			
		}
		prime_flag=1;
		temp_ptr=starting_ptr;
		starting_ptr=starting_ptr->next;
		free(temp_ptr); /* Frees node that won't be used again */
	}
	return time_delta;
}
/*adds all numbers into array from file */
void init_array(int *arr){
	FILE *file=fopen("data.txt","r");
	int k=0,i,counter,data=0;
	while(data!=1000000){
		data=read_file(file);
		arr[k]=data;
		k++;	
	}
	arr[k]=-101;
	fclose(file);
}
/*Determines prime numbers in array and displayes these numbers on screen and writes on file */
float list_prime_array(int *arr,FILE *file,clock_t *time1,clock_t *time2,clock_t *time3,clock_t *time4){
	*time4=clock()/CLOCKS_PER_SEC;
	clock_t time_file;
	float time_delta=0.0;
	int num,k=0,flag=1,i,prime_flag=1;
	while(flag){
		if(arr[k]==-101){
			break;
		}
		num=arr[k];
		if(num==250000){ /*determines time for first 250000 numbers*/
			*time1=clock()/CLOCKS_PER_SEC;
			*time1=(*time1-*time4)*1000.0;
		}else if(num==500000){/*determines time for first 500000 numbers*/
			*time2=clock()/CLOCKS_PER_SEC;
			*time2=(*time2-*time4)*1000.0;
		}else if(num==1000000){/*determines time for first 10000000 numbers*/
			*time3=clock()/CLOCKS_PER_SEC;
			*time3=(*time3-*time4)*1000.0;
		}
		for(i=2;i<num/2;i++){
			if(num%i==0){
				prime_flag=0;
				i=num;
			}
		}
		if(!(num<2)){
			if(prime_flag==1){
				printf("%d\n",num);
				time_file=clock();
				fprintf(file,"%d\n",num);
				time_file=(clock()-(float)time_file)/CLOCKS_PER_SEC*1000.0;/*determines how much time passed while writes a line into file*/
				time_delta+=(float)time_file;/*holds and add time for each line*/
			}			
		}
		prime_flag=1;
		k++;
	}	
	return time_delta;
}

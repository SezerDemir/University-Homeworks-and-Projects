#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*creates customers structure*/
typedef struct customers{
	char name[20];
	char surname[20];
	char gender[20];
	char age[20];
	struct customers *next;
}customers;

/*creates rooms structure*/
typedef struct rooms{
	int number;
	int capacity;
	int floor;
	struct customers *customer;
	struct rooms *next;
}rooms;

/*defines prototypes of functions*/
rooms * init_room_list(rooms *, rooms*);
rooms * add_room_to_end(rooms *, int, int, int);
void add_room_after_room_number(rooms *, int, int, int);
void print_list(rooms *);
void init_customer_arr(customers *,int *);
int find_line_count();
void add_customer_to_end(customers *, int *, int);
void link_customers_to_room(rooms *,customers *, int *);
void show_vacancy(rooms *);
void get_by_floor(rooms *, int);
rooms * remove_room(rooms *, int);
void remove_customers_of_room(rooms *, customers *);

int main(){
	int customer_file_line_count=find_line_count(); /*determines number of lines of customer.txt file */
	int customer_count=customer_file_line_count;  /*assignes number of lines to customer_count */
	int selection,flag=1,floor_number,room_number;
	rooms *head=NULL;
	rooms *beginning_ptr=NULL; /*holds adress of first node of the room list*/
	customers *customer_arr=(customers*)malloc(customer_file_line_count*sizeof(customers)); /*defines an array that holds customers' information*/
	int *room_number_arr=(int*)malloc(customer_file_line_count*sizeof(int)); /*defines an array that holds room numbers */
	beginning_ptr=init_room_list(head,beginning_ptr); /*creates room list*/
	init_customer_arr(customer_arr,room_number_arr); /*assignes customer information to customer_arr*/
	add_customer_to_end(customer_arr,room_number_arr,customer_count); /*establishes links that among customers*/ 
	link_customers_to_room(beginning_ptr,customer_arr,room_number_arr); 
	/*Menu*/
	while(flag){
		printf("\n------------------Process List------------------");
		printf("\n1-)List rooms that have customers\n2-)Show vacancy\n3-)List rooms that are at a specific floor\n4-)Remove a room              (-1 to exit)\nSelection:");
		scanf("%d",&selection);
		switch(selection){
			case 1:{
				print_list(beginning_ptr);
				break;
			}
			case 2:{
				show_vacancy(beginning_ptr);
				break;
			}
			case 3:{
				printf("\nEnter the floor number:");
				scanf("%d",&floor_number);
				get_by_floor(beginning_ptr,floor_number);
				break;
			}
			case 4:{
				printf("\nEnter the number of the room that you want to remove:");
				scanf("%d",&room_number);
				beginning_ptr=remove_room(beginning_ptr,room_number);
				break;
			}
			case -1:{
				flag=0;
				break;
			}
			default:{
				printf("\nEnter a valid process number!!!");
				break;
			}
		}
	}
	free(customer_arr);
	free(room_number_arr);
	return 0;
}

rooms * init_room_list(rooms *head,rooms *beginning_ptr){
	int number,capacity,floor;
	rooms *temp_ptr;
	FILE *file=fopen("rooms.txt","r");
	while(fscanf(file,"%d,%d,%d",&number,&floor,&capacity)!=EOF){ /*gets values of room*/
		if(head==NULL){/*if list is empty, creates first node and node is assigned to head of list*/
			head=add_room_to_end(head,number,floor,capacity);
			beginning_ptr=head;
			temp_ptr=head;
		}
		else{
			temp_ptr=beginning_ptr;
			while(1){
				if(number+1==(temp_ptr->number) && temp_ptr==beginning_ptr){
					rooms *new_node=(rooms*)malloc(sizeof(rooms));
					new_node->number=number;
					new_node->capacity=capacity;
					new_node->floor=floor;
					new_node->next=beginning_ptr;
					new_node->customer=NULL;
					beginning_ptr=new_node;	
					break;				
				}
				else if(temp_ptr->next==NULL && number+1!=(temp_ptr->number)){/*if there is not a room that has a number that 1 more than new room's number*/ 
					head=add_room_to_end(head,number,floor,capacity); /*adds it to end of the list*/
					break;
				}
				else if(temp_ptr!=beginning_ptr && number+1==(temp_ptr->number)){			
					add_room_after_room_number(beginning_ptr, number, floor, capacity);
					break;
				}
				if(temp_ptr->next!=NULL){
					temp_ptr=temp_ptr->next;
				}
			}
		}				
	}
	fclose(file);
	return beginning_ptr; /*return the new beginning adress of the room list*/
}

rooms * add_room_to_end(rooms *head, int number, int floor, int capacity){
	/*creates new node and values is assigned*/
	rooms *new_node = (rooms*)malloc(sizeof(rooms));
	new_node->number=number;
	new_node->capacity=capacity;
	new_node->floor=floor;
	new_node->customer=NULL;
	new_node->next=NULL;
	if(head!=NULL){/*if list is not empty*/
		head->next=new_node;
	}	
	head=new_node;
	return head;
}

void add_room_after_room_number(rooms *beginning_ptr, int number, int floor, int capacity){
	rooms *previous_ptr; /*that holds address of previoys node*/
	rooms *temp_ptr=beginning_ptr;
	/*creates new node and values is assigned*/
	rooms *new_node=(rooms*)malloc(sizeof(rooms));
	new_node->number=number;
	new_node->capacity=capacity;
	new_node->floor=floor;	
	new_node->customer=NULL;
	while(1){/*adds node to between two different node*/
		previous_ptr=temp_ptr;
		temp_ptr=temp_ptr->next;
		if(number+1==temp_ptr->number){			
			previous_ptr->next=new_node;
			new_node->next=temp_ptr;	
			break;		
		}
	}
}

void init_customer_arr(customers *customers_arr, int *room_number_arr){
	FILE *file=fopen("customers.txt","r");
	int room_number,i,temp_i,customer_counter=0;
	char *name,*surname,*others,*age,*gender;
	int line_count=find_line_count();
	/*creates new arrays to hold information of customer*/
	name=(char*)malloc(50*sizeof(char));
	surname=(char*)malloc(50*sizeof(char));
	others=(char*)malloc(50*sizeof(char));
	age=(char*)malloc(50*sizeof(char));
	gender=(char*)malloc(50*sizeof(char));
	while(line_count>0){
		fscanf(file,"%d,%s %s",&room_number,name,others);
		/*others holds surname,age and gender information, that part seperates them into pieces and assign them to their arrays*/
		for(i=0;others[i]!=',';i++){
			surname[i]=others[i];		
		}
		surname[i]='\0';
		for(i=i+1,temp_i=0;others[i]!=',';i++,temp_i++){
			age[temp_i]=others[i];
		}
		age[temp_i]='\0';
		for(i=i+1,temp_i=0;others[i]!='\0';i++,temp_i++){
			gender[temp_i]=others[i];		
		}
		gender[temp_i]='\0';	
		strcpy(customers_arr[customer_counter].name,name);
		strcpy(customers_arr[customer_counter].surname,surname);
		strcpy(customers_arr[customer_counter].age,age);
		strcpy(customers_arr[customer_counter].gender,gender);
		customers_arr[customer_counter].next=NULL;
		room_number_arr[customer_counter]=room_number;
		customer_counter++;
		line_count--;		
	}
	free(name);
	free(surname);
	free(age);
	free(gender);
	free(others);
	fclose(file);
}
/*links customers which have same room number*/
void add_customer_to_end(customers *customers_arr, int *room_number_arr, int customer_count){
	int i,j,flag=1;
	for(i=0;i<customer_count;i++){
		flag=1;
		for(j=i+1;j<customer_count;j++){
			if(room_number_arr[i]==room_number_arr[j]){ 
				customers_arr[i].next=&customers_arr[j];
				break;				
			}
		}			
	}		
}
/*links customer partial lists to room*/
void link_customers_to_room(rooms *beginning_ptr,customers *customer_arr, int *room_number_arr){
	int condition_arr[100];/*holds room number of customers that should not match up with room again.One room must be linked with just a customer and if there are customers more then 1, others must be linked with each other*/
	int condition_counter=0,i=0,k,flag=1;
	while(1){		
		flag=1;
		for(i=0;i<find_line_count();i++){
			for(k=0;k<condition_counter;k++){ /*checks room number of customer is in condition_arr or not*/
				if(room_number_arr[i]==condition_arr[k]){
					flag=0;
				}
			}
			if(flag){/*if it is not in condition_arr*/
				if(beginning_ptr->number==room_number_arr[i]){/*if it's room number matches up with room number of the room,assign it to room*/
					beginning_ptr->customer= &customer_arr[i];
					condition_arr[condition_counter++]=room_number_arr[i];
					break;
				}				
			}
			flag=1;
		}
		if(beginning_ptr->next!=NULL){
			beginning_ptr=beginning_ptr->next;
		}else{
			break;
		}			
	}
}
	
rooms * remove_room(rooms *beginning_ptr, int room_number){
	rooms *temp_ptr=beginning_ptr;
	rooms *new_beginning_ptr=beginning_ptr;
	rooms *previous_room_ptr=beginning_ptr;
	customers *customer;
	while(1){
		if(temp_ptr->number==room_number){
			customer=temp_ptr->customer;
			if(temp_ptr==beginning_ptr){/*if it is first room of the list*/
				if(temp_ptr->customer!=NULL){/*if there is at least one customer in the room*/					
					remove_customers_of_room(temp_ptr,customer);/*removes customers*/
				}		
				new_beginning_ptr=temp_ptr->next;/*changes beginning address of the room list*/
				temp_ptr->next=NULL;
				return new_beginning_ptr;				
			}
			else{
				if(temp_ptr->customer!=NULL){
					remove_customers_of_room(temp_ptr,customer);
				}			
				if(temp_ptr->next!=NULL){
					previous_room_ptr->next=temp_ptr->next;
					return new_beginning_ptr;
				}
				else{
					previous_room_ptr->next=NULL;
					return new_beginning_ptr;
				}
			}
		}
		else{
			previous_room_ptr=temp_ptr;
			temp_ptr=temp_ptr->next;
		}
	}
}	
/*removes customers of a room*/
void remove_customers_of_room(rooms *room_ptr, customers *cus_ptr){
	customers *temp_ptr=cus_ptr;
	if(temp_ptr->next!=NULL){
		if(temp_ptr->next->next!=NULL){
		temp_ptr=temp_ptr->next;
		remove_customers_of_room(room_ptr,temp_ptr);
		}
	}
	if(temp_ptr->next!=NULL){
		if(temp_ptr->next->next==NULL){
			temp_ptr->next=NULL;
			return;
		}		
	}
	if(cus_ptr->next==NULL){/*if there is just a customer in list of customer of room*/
		room_ptr->customer=NULL;
		return;
	}
}
/*displays empty rooms*/
void show_vacancy(rooms *beginning_ptr){
	rooms *temp_ptr=beginning_ptr;
	printf("---------------Empty Rooms---------------\n");
	while(1){
		if(temp_ptr!=NULL){
			if(temp_ptr->customer==NULL){
				printf("Room %d(Floor %d-capacity %dx):\n",temp_ptr->number,temp_ptr->floor,temp_ptr->capacity);
			}
			if(temp_ptr->next!=NULL){
				temp_ptr=temp_ptr->next;
			}
			else{
				break;
			}
		}		
	}
}
/*displays room list of a specific floor*/
void get_by_floor(rooms *beginning_room_ptr, int floor){
	rooms *temp_ptr=beginning_room_ptr;
	int flag_room_exist=1;
	printf("\n-------------Rooms at floor %d-------------\n",floor);
	while(1){ 
		if(temp_ptr!=NULL){
			if(temp_ptr->floor==floor){
				printf("Room %d(Floor %d-capacity %dx):\n",temp_ptr->number,temp_ptr->floor,temp_ptr->capacity);
				flag_room_exist=0;
			}
			if(temp_ptr->next!=NULL){
				temp_ptr=temp_ptr->next;
			}
			else{
				break;
			}
		}		
	}
	if(flag_room_exist){
		printf("There is not any room at this floor\n");
	}	
}

void print_list(rooms *beginning_room_ptr){
	rooms *temp_ptr=beginning_room_ptr;
	customers *cus_ptr;
	while(1){
		if(temp_ptr->customer!=NULL){/*if there is customer in the room*/
			printf("Room %d(Floor %d-capacity %dx):\n",temp_ptr->number,temp_ptr->floor,temp_ptr->capacity);
			cus_ptr=temp_ptr->customer;
			while(1){/*finds customer of the room and displays it*/
				printf("->%s %s (%s - %s)\n",cus_ptr->name,cus_ptr->surname,cus_ptr->age,cus_ptr->gender);
				if(cus_ptr->next!=NULL){
					cus_ptr=cus_ptr->next;
				}
				else{
					break;
				}				
			}
			printf("\n");
		}			
		if(temp_ptr->next!=NULL){
			temp_ptr=temp_ptr->next;
		}
		else{
			break;
		}		
	}
}
/*returns what is the line count of customer file*/
int find_line_count(){
	int count=0;
	FILE *file=fopen("customers.txt","r");
	char *arr=(char*)malloc(20*sizeof(char));
	char *arr1=(char*)malloc(30*sizeof(char));
	while(fscanf(file,"%s%s",arr,arr1)!=EOF){
		count++;
	}
	fclose(file);
	free(arr);
	free(arr1);
	return count;	
}

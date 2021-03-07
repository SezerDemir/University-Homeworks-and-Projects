#include <stdio.h>
#include <time.h>

typedef struct{ /* declares forost structure */
	char **map;
	int width,height;
	int flower_x;
	int flower_y;
}forest;

typedef struct{ /* declares botanist structure */
	int coord_x;
	int coord_y;
	int water_bottle_size;
}botanist;

typedef enum{ right=0,left=1,up=2,down=3 }direction; /* declares enum for directions */

void init_game(forest*,botanist*);
void print_map(forest*);
void search(forest*,botanist*);

int main(){
	int k;
	srand(time(NULL));
	forest *ptr_game,game; /* declares a pointer and a veriable of forest structer */
	botanist *ptr_man,man; /* declares a pointer and a veriable of botanist structer */
	ptr_game=&game; /* veriable's adress assigns to pointer to use pointer in functions */
	ptr_man=&man;
	init_game(ptr_game,ptr_man);
	print_map(ptr_game);
	printf("\n\n");
	search(ptr_game,ptr_man);
	for(k=0;k<ptr_game->height;k++){ /*Frees memory that be taken by map array */
		free(ptr_game->map[k]);
	}
	free(ptr_game->map);
	return 0;
}

void init_game(forest *ptr_game,botanist *ptr_man){ /* Assigns values of veriables and array of structeres from init file */
	FILE *file;	
	int i=0,j=0,k,m,z;
	file= fopen("init.csv","r");
	fscanf(file,"%d\n",&ptr_man->water_bottle_size);
	fscanf(file,"%d,%d\n",&ptr_game->height,&ptr_game->width);
	ptr_game->map=(char **)malloc(ptr_game->width*sizeof(char *));
	for(z=0;z<ptr_game->height;z++){ /* Allocates memory for array of game structure */
		*(ptr_game->map+z)=(char *)malloc(ptr_game->height*sizeof(char));
	}
	while(fscanf(file,"%c,",&ptr_game->map[i][j])!=EOF){ /* Creates map according to excel file */
		if(ptr_game->map[i][j]=='\n'){
			i++;
			j=0;
		}else{
			j++;
		}
	}
	for(i=0;i<ptr_game->height;i++){ /*assigns man's and flower's coordinates */
		for(j=0;j<ptr_game->width;j++){
			if(ptr_game->map[i][j]=='B'){
				ptr_man->coord_x=i;
				ptr_man->coord_y=j;
			}
			if(ptr_game->map[i][j]=='F'){
				ptr_game->flower_x=i;
				ptr_game->flower_y=j;
			}
			
		}
	}
	fclose(file);
}

void print_map(forest *ptr_game){/* Displays map of forest in this function */
	int k,m;
	for(k=0;k<ptr_game->width;k++){
		for(m=0;m<ptr_game->height;m++){
			printf("%c ",ptr_game->map[k][m]);
		}
		printf("\n");
	} 
}

void search(forest *ptr_game,botanist *ptr_man){ 
	int flag=1,flag_finish=0;
	direction direct; /* declares a veriable of direction enum */
	while(flag){
		direct=rand()%4; /* determines a random number between 0-3 */
		/*Movement part*/
		if(direct==right && ptr_man->coord_y < ptr_game->width-1 && ptr_game->map[ptr_man->coord_x][ptr_man->coord_y+1]!='#'){
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]=' ';
			ptr_man->coord_y++;
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]='B';
			ptr_man->water_bottle_size--;			
			flag=0;
		}else if(direct==left && ptr_man->coord_y>0 && ptr_game->map[ptr_man->coord_x][ptr_man->coord_y-1]!='#'){
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]=' ';
			ptr_man->coord_y--;
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]='B';
			ptr_man->water_bottle_size--;
			flag=0;
		}else if(direct==up && ptr_man->coord_x>0 && ptr_game->map[ptr_man->coord_x-1][ptr_man->coord_y]!='#'){
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]=' ';
			ptr_man->coord_x--;
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]='B';
			ptr_man->water_bottle_size--;
			flag=0;
		}else if(direct==down && ptr_man->coord_x < ptr_game->height-1 && ptr_game->map[ptr_man->coord_x+1][ptr_man->coord_y]!='#'){
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]=' ';
			ptr_man->coord_x++;
			ptr_game->map[ptr_man->coord_x][ptr_man->coord_y]='B';
			ptr_man->water_bottle_size--;
			flag=0;
		}
	}
	/*If botanist reaches to flower game ends and map be displayed on the screen */ 
	if(ptr_man->coord_x==ptr_game->flower_x && ptr_man->coord_y == ptr_game->flower_y){
		print_map(ptr_game);
		printf("I have found it on(%d,%d)!",ptr_man->coord_x,ptr_man->coord_y);	
		flag_finish=1;
	}
	
	if(flag_finish==0){
		if(ptr_man->water_bottle_size==0){ /*If water runs out game ends and a specific message be displayed on the screen */
			print_map(ptr_game);
			printf("Help! I am on (%d,%d)",ptr_man->coord_x,ptr_man->coord_y);			
		}else{
			search(ptr_game,ptr_man);
		}		
	}
}

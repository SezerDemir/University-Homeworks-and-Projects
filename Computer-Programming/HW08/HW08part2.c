#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum{ N,P,S,M,B,T,Finish,Start}type_block;
typedef struct{
	char text[15];
	int data;
	type_block type;
	int pos_x,pos_y;
	int jump_x,jump_y;
}block;

void init_board(block[][10]);
void show_board(block[][10]);
void single_game(block[][10],int,int);
void compatative_game();

int main(){
	int play_counter,flag=1,selection;
	block board[10][10];
	init_board(board);
	srand(time(NULL));
	while(flag){
		printf("\n\n-----------Please select the mode of the game-----------\n1-Single Player\n2-Compatative Mode\n\nSelection:");
		scanf("%d",&selection);
		printf("\n\n\n");
		switch(selection){
			case 1:{
				play_counter=single_player(board,0,1); // Second parameter represents the beginning of how many times passed turn in the game. So it must be entered as 0 for true result
				printf("\nTimes Played:%d",play_counter);
				break;
			}
			case 2: {
				compatative_game();
				break;
			}
			case 0:{
				flag=0;
				break;
			}
			default:
				printf("Please enter a valid selection(1 or 2)(0 to exit)");
				
		}

	}

	return 0;
}

void init_board(block board[][10]){
	int i,j,counter=1,sign=2;
	/*-------------Data Declaration-------------*/
	for(i=9;i>=0;i--){ 
		for(j=9;j>=0;j--){
			board[i][j].data=counter;
			if(sign%2==0){
			counter++;
			}
			else{
			counter--;
			}

		}
		if(sign%2==0){
			counter--;
		}
		else{
			counter++;
		}
		counter+=10;
		sign++;
	}
	/*-------------Type declaration-------------*/
	for(i=0;i<10;i++){ // öncelikle bütün kutularýn type ý normal olarak atanýyor
		for(j=0;j<10;j++){
			board[i][j].type=N;
		}
	}
	board[0][2].type=T,board[0][7].type=S,board[0][8].type=S,board[1][1].type=M;
	board[1][5].type=P,board[1][8].type=B,board[2][2].type=S,board[2][5].type=M;
	board[2][8].type=T,board[3][1].type=M,board[3][4].type=B,board[3][7].type=S;
	board[4][3].type=T,board[4][6].type=M,board[4][9].type=S,board[5][0].type=B;
	board[5][2].type=P,board[5][5].type=S,board[6][1].type=M,board[6][5].type=B;
	board[6][9].type=T,board[7][1].type=B,board[7][5].type=S,board[8][2].type=P;
	board[8][6].type=M,board[9][3].type=M,board[9][4].type=S,board[9][9].type=Start;
	board[0][9].type=Finish;
	
	/*-------------Position Declaration-------------*/
	for(i=9;i>=0;i--){
		for(j=9;j>=0;j--){
			board[i][j].pos_x=i;
			board[i][j].pos_y=j;
		}
	}
	
	/*-------------Jump Declaration-------------*/
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			board[i][j].jump_x=0;	/*Firstly,Declares jump values as 0 for all blocks */
			board[i][j].jump_y=0;
		}
	}
	
	board[0][7].jump_x=2,board[0][7].jump_y=1;
	board[0][8].jump_x=4,board[0][8].jump_y=5;
	board[2][2].jump_x=3,board[2][2].jump_y=2;
	board[3][7].jump_x=4,board[3][7].jump_y=1;
	board[4][9].jump_x=5,board[4][9].jump_y=6;	/*Jump declaration of Snakes*/
	board[5][5].jump_x=7,board[5][5].jump_y=8;
	board[7][5].jump_x=8,board[7][5].jump_y=1;
	board[9][4].jump_x=9,board[9][4].jump_y=9;
	
	board[1][1].jump_x=0,board[1][1].jump_y=3;
	board[2][5].jump_x=1,board[2][5].jump_y=7;
	board[3][1].jump_x=2,board[3][1].jump_y=6;
	board[4][6].jump_x=3,board[4][6].jump_y=1;	/*Jump declaration of Stairs*/
	board[6][1].jump_x=4,board[6][1].jump_y=1;
	board[8][6].jump_x=7,board[8][6].jump_y=2;
	board[9][3].jump_x=8,board[9][3].jump_y=0;
	
	/*-------------Text Declaration-------------*/
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(board[i][j].type==N){
				sprintf(board[i][j].text,"%d",board[i][j].data);				
			}else if(board[i][j].type==S){
				sprintf(board[i][j].text,"S {%d}",board[board[i][j].jump_x][board[i][j].jump_y].data);
			}else if(board[i][j].type==P){
				sprintf(board[i][j].text,"P");
			}else if(board[i][j].type==M){
				sprintf(board[i][j].text,"M {%d}",board[board[i][j].jump_x][board[i][j].jump_y].data);
			}else if(board[i][j].type==B){
				sprintf(board[i][j].text,"B");
			}else if(board[i][j].type==T){
				sprintf(board[i][j].text,"T");
			}else if(board[i][j].type==Start){
				sprintf(board[i][j].text,"%d (Start)",board[i][j].data);
			}else if(board[i][j].type==Finish){
				sprintf(board[i][j].text,"%d (Finish)",board[i][j].data);
			}
		}
	}
}

void show_board(block board[][10]){
	int i,j;
	/*Board is displated on the screen*/
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			printf("%-10s ",board[i][j].text); 
		}
		printf("\n");
	}
	printf("\n");
}

int single_player(block board[][10],int play_counter,int data){
	int dice_value,flag=1,coordinate_x,coourdinate_y,temp;
	int i,j,k,m;
	while(flag){
		dice_value=rand()%6+1;
		data+=dice_value;
		if(data>100){
			data-=dice_value;
		}
		else{
			flag=0;
			play_counter++;
		}
	}
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(board[i][j].data==data){
				if(board[i][j].type==S || board[i][j].type==M){ /*If new blocks type is M or S, it jumps to new block. Which is a feature of that type of blocks*/
					data=board[board[i][j].jump_x][board[i][j].jump_y].data;
					for(k=0;k<10;k++){
						for(m=0;m<10;m++){
							if(board[k][m].data==data){
								temp=single_player(board,play_counter,data);
								show_board(board);
								printf("Dice:%d\n",dice_value);
								printf("Coordinates after the move:(x:%d  y=%d)",board[k][m].pos_x,board[k][m].pos_y);
								printf("\n\n\n");
								return temp;
							}
						}
					}
				}
				else if(board[i][j].type==B || board[i][j].type==T){
					if(board[i][j].type==B){
						data+=15; /* If block's type is B, it jumps 15 blocks forward */
						for(k=0;k<10;k++){
							for(m=0;m<10;m++){
								if(board[k][m].data==data){
									temp= single_player(board,play_counter,data);
									show_board(board);
									printf("Dice:%d\n",dice_value);
									printf("Coordinates after the move:(x:%d  y=%d)",board[k][m].pos_x,board[k][m].pos_y);
									printf("\n\n\n");
									return temp;
								}
							}
						}
					}
					if(board[i][j].type==T){ /* If block's type is T, it jumps 15 blocks backward */
						data-=15;
						for(k=0;k<10;k++){
							for(m=0;m<10;m++){
								if(board[k][m].data==data){
									temp=single_player(board,play_counter,data);
									show_board(board);
									printf("Dice:%d\n",dice_value);
									printf("Coordinates after the move:(x:%d  y=%d)",board[k][m].pos_x,board[k][m].pos_y);
									printf("\n\n\n");
									return temp;
								}
							}
						}
					}
				}
				else if(board[i][j].type==P){ /* If block's type is P, it comes back to it's previous block */
					data-=dice_value;
					for(k=0;k<10;k++){
						for(m=0;m<10;m++){
							if(board[k][m].data==data){
								temp=single_player(board,play_counter,data);
								show_board(board);
								printf("Dice:%d\n",dice_value);		
								printf("Coordinates after the move:(x:%d  y=%d)",board[k][m].pos_x,board[k][m].pos_y);
								printf("\n\n\n");
								return temp;
							}
						}
					}
				}
				else if(board[i][j].type==Finish){ /* If reaches the finish block, Game ends and returns play_counter */
					show_board(board);
					printf("Dice:%d\n",dice_value);
					printf("Coordinates after the move:(x:%d  y=%d)\n\n",board[i][j].pos_x,board[i][j].pos_y);
					return play_counter;
				}
				else if(board[i][j].type==N){
					temp=single_player(board,play_counter,data);
					show_board(board);
					printf("Dice:%d\n",dice_value);	
					printf("Coordinates after the move:(x:%d  y=%d)",board[i][j].pos_x,board[i][j].pos_y);	
					printf("\n\n\n");	
					return temp;
				}
			}
		}
	}
}
void compatative_game(){ /*That function make play two different player with each other and returns which player wins */
	int player1_counter,player2_counter;
	block board1[10][10],board2[10][10];
	init_board(board1);
	init_board(board2);
	player1_counter=single_player(board1,0,1);
	player2_counter=single_player(board2,0,1);
	if(player1_counter>player2_counter){
		printf("\nSecond player won the game!!! It's finish the game in %d turns",player2_counter);
	}else if(player1_counter<player2_counter){
		printf("\nFirst player won the game!!! It's finish the game in %d turns",player1_counter);
	}else{
		printf("DRAW!!!");
	}	
}



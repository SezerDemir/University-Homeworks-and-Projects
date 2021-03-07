#include <stdio.h>
typedef enum {white_man, black_man, white_king, black_king, empty} piece;
typedef enum {white = 10, black = 20} player; 
void sample_game_1(); 
void init_board(piece [][8]); 
int move(piece[][8], int, int, int, int, player);
int check_end_of_game(piece[][8]); 

int main(){
	sample_game_1();
	return 0;
}

void sample_game_1(){
	piece board[8][8];
	int i,j,s,flag=1,flag1=1,counter=0,from_x,from_y,to_x,to_y,check;
	player turn=white;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(i==1 || i==2){
				board[i][j]=black_man;
			}
			else if(i==5 || i==6){
				board[i][j]=white_man;
			}
			else{
				board[i][j]=empty;
			}
		}
	}
	{
		for(i=0;i<8;i++){ //
			for(j=0;j<8;j++){
				if(board[i][j]==white_man){
					if(j>1 && (board[i][j-1]==black_man || board[i][j-1]==black_king) && board[i][j-2]==empty){ //sol kisim kontrolu
						board[i][j-2]=white_man;
						board[i][j-1]=empty;
						board[i][j]=empty;
					if(i==0){
						board[i][j-2]=white_king;
					}
					}
					if(i>1 && (board[i-1][j]==black_man || board[i-1][j]==black_king) && board[i-2][j]==empty){ //ust kisim kontrolu
						board[i-2][j]=white_man;
						board[i-1][j]=empty;
						board[i][j]=empty;
						if((i-2)==0){
							board[i-2][j]=white_king;
						}
					}
					if(j<6 && (board[i][j+1]==black_man || board[i][j+1]==black_king) && board[i][j+2]==empty){ //sag kisim kontrolu
						board[i][j+2]=white_man;
						board[i][j+1]=empty;
						board[i][j]=empty;
						if(i==0){
							board[i][j+2]=white_king;
						}
					}
				}
			}
		}
	}
		/*for(i=0;i<8;i++){ //beyaz kiral yedirttirme
			for(j=0;j<8;i++){
				if(board[i][j]==white_king){
					for(s=i+1;s<7;s++){ //alt kisim kontrolu
						if(board[s][j]==white_man || board[s][j]==white_king){
							counter++;
						}
						if( (board[s][j]==black_man || board[s][j]==black_king) && board[s+1][j]==empty && counter==0){
							board[i][j]=empty;
							board[s][j]=empty;
							board[s+1][j]=white_king;
							s++;
						}	
					}
					counter=0;
					if(i>1){
						for(s=i-1;s>1;s--){ //ust kisim komtrol
							if(board[s][j]==white_man || board[s][j]==white_king){
								counter++;
							}	
							if( (board[s][j]==black_man || board[s][j]==black_king) && board[s-1][j]==empty && counter==0){ 
								board[i][j]=empty;
								board[s][j]=empty;
								board[s-1][j]=white_king;
								s--;
							}
						}
					}
					counter=0;
					for(s=j+1;s<7;s++){ //sag kisim komtrol
						if(board[i][s]==white_man || board[s][j]==white_king){
							counter++;
						}
						if( (board[i][s]==black_man || board[i][s]==black_king) && board[i][s+1]==empty && counter==0){ 
							board[i][j]=empty;
							board[i][s]=empty;
							board[i][s+1]=white_king;
							s++;
						}
					}
					counter=0;
					if(j>1){
						for(s=j-1;s>1;s--){ //sol kisim komtrol
							if(board[i][s]==white_man || board[i][s]==white_king){
								counter++;
							}
							if( (board[i][s]==black_man || board[i][s]==black_king) && board[i][s-1]==empty && counter==0){ 
								board[i][j]=empty;
								board[i][s]=empty;
								board[i][s-1]=white_king;
								s--;
							}
						}
					}
					counter=0;
				}
			}
		}*/
		for(i=0;i<8;i++){ 
			for(j=0;j<8;j++){
				if(board[i][j]==black_man){
					if(j>1 && (board[i][j-1]==white_man || board[i][j-1]==white_king) && board[i][j-2]==empty){ //sol kisim kontrolu
						board[i][j-2]=black_man;
						board[i][j-1]=empty;
						board[i][j]=empty;
					if(i==7){
						board[i][j-2]=black_king;
					}
					}
					if(i<6 && (board[i+1][j]==white_man || board[i+1][j]==white_king) && board[i+2][j]==empty){ //alt kontrolu
						board[i+2][j]=black_man;
						board[i+1][j]=empty;
						board[i][j]=empty;
						if((i+2)==7){
							board[i+2][j]=black_king;
						}
					}
					if(j<6 && (board[i][j+1]==white_man || board[i][j+1]==white_king) && board[i][j+2]==empty){ //sag kontrolu
						board[i][j+2]=black_man;
						board[i][j+1]=empty;
						board[i][j]=empty;
						if(i==7){
							board[i][j+2]=black_king;
						}
					}
				}
			}
		}/*
		for(i=0;i<8;i++){ //siyah kiral otomatik yedirtme
			for(j=0;j<8;i++){
				if(board[i][j]==black_king){
					for(s=i+1;s<7;s++){ //alt kisim kontrolu
						if(board[s][j]==black_man || board[s][j]==black_king){
							counter++;
						}
						if( (board[s][j]==white_man || board[s][j]==white_king) && board[s+1][j]==empty && counter==0){
							board[i][j]=empty;
							board[s][j]=empty;
							board[s+1][j]=black_king;
							s++;
						}	
					}
					counter=0;
					for(s=i-1;s>1;s--){ //ust kisim komtrol
						if(board[s][j]==black_man || board[s][j]==black_king){
							counter++;
						}	
						if( (board[s][j]==white_man || board[s][j]==white_king) && board[s-1][j]==empty && counter==0){ 
							board[i][j]=empty;
							board[s][j]=empty;
							board[s-1][j]=black_king;
							s--;
						}
					}
					for(s=j+1;s<7;s++){ //sag kisim komtrol
						if(board[i][s]==black_man || board[s][j]==black_king){
							counter++;
						}
						if( (board[i][s]==white_man || board[i][s]==white_king) && board[i][s+1]==empty && counter==0){ 
							board[i][j]=empty;
							board[i][s]=empty;
							board[i][s+1]=black_king;
							s++;
						}
					}
					counter=0;
					for(s=j-1;s>1;s--){ //sol kisim komtrol
						if(board[i][s]==black_man || board[i][s]==black_king){
							counter++;
						}
						if( (board[i][s]==white_man || board[i][s]==white_king) && board[i][s-1]==empty && counter==0){ 
							board[i][j]=empty;
							board[i][s]=empty;
							board[i][s-1]=black_king;
							s--;
						}
					}
					counter=0;
				}
			}
		}  */
		init_board(board);
		if(check_end_of_game(board)==10){
			printf("Winner is White!!!");
			flag=0;
		}else if(check_end_of_game(board)==20){
			printf("Winner is Black!!!");
			flag=0;
		}
		do{
			printf("Select the stone:");
			scanf("%d%d",&from_x,&from_y);
			printf("Enter the coordinate:");
			scanf("%d%d",&to_x,&to_y);
			check=move(board,from_x,from_y,to_x,to_y,turn);
			if(check==0){
				flag1=0;
				turn+=10;
			}
		}while(flag1);
		if(turn>20){
			turn=10;
		}
		flag1=1;
	}

void init_board(piece board[][8]){
	char board_display[8][8];
	int i,j;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(board[i][j]==white_man){
				board_display[i][j]='w';
			}else if(board[i][j]==black_man){
				board_display[i][j]='b';
			}else if(board[i][j]==empty){
				board_display[i][j]='-';
			}else if(board[i][j]==white_king){
				board_display[i][j]='W';
			}else if(board[i][j]==black_king){
				board_display[i][j]='K';
			}
		}
	}
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			printf("%c",board_display[i][j]);
		}
		printf("\n");
	}
}
int move(piece board[][8], int from_x, int from_y, int to_x, int to_y, player p){
	int flag_1=1,flag_2=1,i,j,s,counter=0,counter_x1=0,counter_x2=0,counter_y1=0,counter_y2=0;
	if(from_x<8 && from_y<8 && to_x<8 && to_y<8 && from_x>=0 && from_y>=0 && to_x>=0 && to_y>=0){
		while(flag_1){
			if(p==white)
			{	
				if(board[from_y][from_x]==white_man && ( (to_x==from_x+1 && to_y==from_y) || (to_x==from_x-1 && to_y==from_y) || (to_x==from_x && to_y==to_y+1)) && board[to_y][to_x]==empty && to_x!=from_x && to_y!=from_y){
					board[from_y][from_x]=empty;
					board[to_y][to_x]=white_man;
					if(to_y==7){
						board[to_y][to_x]=white_king;
					}
					return 0;	
				}
				else{
					printf("Enter a valid coordinate(1-9)(1-9)");
					return 1;
				}
				if(board[from_y][from_x]==white_king){ 
					for(i=from_y;i<8;i++){ //kullanicinin tasi hangi koordinatlara kadar tasiyabilecegi hesaplaniyor
						if(board[i][from_x]!=white_man || board[i][from_x]!=white_king || board[i][from_x]!=black_man || board[i][from_x]!=black_king){
							counter_y1=i;
						}
					}
					for(i=from_y-1;i>=0;i--){ //ust kontrol
						if(board[i][from_x]!=white_man || board[i][from_x]!=white_king || board[i][from_x]!=black_man || board[i][from_x]!=black_king){
							counter_y2=i;
						}
					}
					for(i=from_x;i<8;i++){ //sag kontrol
						if(board[from_y][i]!=white_man || board[from_y][i]!=white_king || board[from_y][i]!=black_man || board[from_y][i]!=black_king){
							counter_x1=i;
						}
					}
					for(i=from_x-1;i>=0;i--){ //sol kontrol
						if(board[from_y][i]!=white_man || board[from_y][i]!=white_king || board[from_y][i]!=black_man || board[from_y][i]!=black_king){
							counter_x2=i;
						}
					}
					if(to_y>=counter_y2 && to_y<=counter_y1 && to_x<=counter_x1 && to_x>=counter_x2 && to_x!=from_x && to_y!=from_y){
						board[from_y][from_x]=empty;
						board[to_y][to_x]=white_king;
						return 0;
					}
					else{
						printf("Enter a valid coordinate.That stone can be move in (%d-%d)(%d-%d)",counter_y1,counter_y2,counter_x1,counter_x2);
						return 1;
					}
				}
			}
			if(p==black){
				if(board[from_y][from_x]==black_man && ( (to_x==from_x+1 && to_y==from_y) || (to_x==from_x-1 && to_y==from_y) || (to_x==from_x && to_y==to_y-1)) && board[to_y][to_x]==empty && to_x!=from_x && to_y!=from_y){
					board[from_y][from_x]=empty;
					board[to_y][to_x]=black_man;
					if(to_y==7){
						board[to_y][to_x]=black_king;
					}
					return 0;	
				}
				if(board[from_y][from_x]==black_king){ 
					for(i=from_y;i<8;i++){ //kullanicinin tasi hangi koordinatlara kadar tasiyabilecegi hesaplaniyor
						if(board[i][from_x]!=white_man || board[i][from_x]!=white_king || board[i][from_x]!=black_man || board[i][from_x]!=black_king){
							counter_y1=i;
						}
					}
					for(i=from_y-1;i>=0;i--){ //ust kontrol
						if(board[i][from_x]!=white_man || board[i][from_x]!=white_king || board[i][from_x]!=black_man || board[i][from_x]!=black_king){
							counter_y2=i;
						}
					}
					for(i=from_x;i<8;i++){ //sag kontrol
						if(board[from_y][i]!=white_man || board[from_y][i]!=white_king || board[from_y][i]!=black_man || board[from_y][i]!=black_king){
							counter_x1=i;
						}
					}
					for(i=from_x-1;i>=0;i--){ //sol kontrol
						if(board[from_y][i]!=white_man || board[from_y][i]!=white_king || board[from_y][i]!=black_man || board[from_y][i]!=black_king){
							counter_x2=i;
						}
					}
					if(to_y>=counter_y2 && to_y<=counter_y1 && to_x<=counter_x1 && to_x>=counter_x2 && to_x!=from_x && to_y!=from_y){
						board[from_y][from_x]=empty;
						board[to_y][to_x]=black_king;
						return 0;
					}
					else{
						printf("Enter a valid coordinate.That stone can be move in (%d-%d)(%d-%d)",counter_y1,counter_y2,counter_x1,counter_x2);
						return 1;
					}
				}	
			}
			flag_1=0;
		}
	}
	else{
		printf("Enter a valid coordinate(1-9)(1-9)\n");
		return 1;
	}
}
int check_end_of_game(piece board[][8]){
	int i,j,counter_white=0,counter_black=0;
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			if(board[i][j]==white_man || board[i][j]==white_king){
				counter_white++;
			}
			else if(board[i][j]==black_man || board[i][j]==black_king){
				counter_black++;
			}
		}
	}
	if(counter_white==0){
		printf("Black wins the game!");
		return 10;
	}else if(counter_black==0){
		printf("White wins the game!");
		return 20;
	}else{
		return -1;
	}
}


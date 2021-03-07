#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int getInt(int,int);
int numberGeneratorBetweenRange(int,int);
void horseRacingGame();
void countOccurrence();
void triangleOfSequences();

void main()
{
	int selection,flag=1; //de�i�kenler tan�mlan�yor
	srand(time(NULL));
	while(flag)
	{
		printf("1.Horse Racing Game\n2.Occurrence Counter\n3.Triangle of Sequances\n0.Exit\n");
		scanf("%d",&selection); //kull�c�dan hangi i�lemi istedi�i bilgisi al�n�yor
		switch(selection)
		{
			case 0:
				flag=0;
				break;
			case 1:
				horseRacingGame();
				break;
			case 2:
				countOccurrence();
				break;
			case 3:
				triangleOfSequences();
				break;
			default:
				printf("Gecerli bir secenek seciniz!!!");
				break;
		}
	}	
}

int getInt(int mini,int maxi)  //fonksiyon 2 paremetre olarak 2 tam say� al�yor ve kullan�c� bu iki say� aras�nda bir de�er girinceye kadar de�er girmesini istiyor
{
	int flag=1,number;
	while(flag){
		scanf("%d",&number);
		if(number<=maxi && number>=mini) // e�er al�nan say� istenlen aral�kta ise d�ng�n�n bitmesi i�in flag de�i�kenine 0 atan�yor
		{
			flag=0;   
		}
	}
	return number;
}

int numberGeneratorBetweenRange(int min,int max)
{	
	int number,flag=1;
	number=(rand()%max)+1;
	while(flag)
	{
		if(number<min)
		{
		number=(rand()%max)+1;
		}
		else
		{
			flag=0;
		}	
	}
	return number;
}

void countOccurrence() //fonksiyon girilen say�n�n i�inde ka� tane yine girilen bir say�dan oldu�unu hesapl�yor
{	
	int digit,result,temp,counter=0,digit_search,counter1=1,flag=1,temp_search,times=1,m;
	int big_number,search_number;
	printf("Big number:");
	scanf("%d",&big_number);
	printf("Search number:");
	scanf("%d",&search_number);
	temp_search=search_number;
	temp=big_number;
	while(flag){ //ana say�n�n i�inde aranan say�n�n basamak say�s� hesaplan�yor
		temp_search=temp_search/10;
		if(temp_search>=1){
			counter1++; 
		}
		else{
			flag=0;
		}
	}
	for(m=0;m<counter1;m++){ //mod i�lemi i�in hangi say�n�n kullan�laca�� hesaplan�yor
		times*=10;
	}
	while(big_number>0)/*say�n�n modu hesaplanan 10 �zeri say� ile al�n�yor ve aranan say�n�n i�inde olup olmad��� denetleniyor,e�er var ise counter artt�r�larak bu de�er
						 bellekte tutuluyor
						*/
	{
		result=big_number%times; 
		if(search_number==result){
			counter++;
		}
		big_number/=10;	
	}
	printf("Occurence:%d \n",counter);
	
}

void triangleOfSequences()
{
	int number = numberGeneratorBetweenRange(2,10); // number de�i�kenine 2 ile 10 aras�nda rastgele bir say� atan�yor
	int i,y,counter=0,counter1,counter2;
	for(i=0;i<number;i++) //d�ng�de istenen ��ken olu�turuluyor
	{
		counter++;
		counter1=counter;
		for(y=1;y<=counter;y++){
			if(y==1){ //ilk sat�rda kendinden �nceki say� olmad���ndan dolay� �zel bir durum i�leniyor
				printf("%d ",counter);
			}
			else{
				counter1+=counter;
				printf("%d ",counter1);
			}
			
		}
		printf("\n");
	}
	printf("\n");
}

void horseRacingGame(){
	int n_horses,selection,h1,h2,h3,h4,h5,flag=1,i,winner;
	n_horses=numberGeneratorBetweenRange(3,5); //at say�s� 3 ile 5 aras�nda rastgele olarak atan�yor
	printf("Number of Horses:%d\n",n_horses);
	printf("Horse Number:");
	if(n_horses==3){ //e�er at say�s� 3 ise 
		selection=getInt(1,n_horses);
		printf("Racing starts..");
		h1=numberGeneratorBetweenRange(10,20); /*atlara ko�u zamanlar� rastgele olarak 10 ile 20 aras�nda atan�yot*/
		h2=numberGeneratorBetweenRange(10,20); 
		h3=numberGeneratorBetweenRange(10,20); 
		printf("\nHorse 1:");
		for(i=0;i<h1;i++){
			printf("-");
		}
		printf("\nHorse 2:");
		for(i=0;i<h2;i++){            //atlar�n zamanlar�n� simgeleyen �ekiller �iziliyor
			printf("-");
		}
		printf("\nHorse 3:");
		for(i=0;i<h3;i++){
			printf("-");
		}
		if(h1<h2 && h1<h3){
			winner=1;
		}else if(h2<h1 && h2<h3){      //kazanan at belirleniyor ve numaras� winner de�i�kenine atan�yor
			winner=2;
		}else if(h3<h2 && h3<h1){
			winner=3;
		}
		else{
			winner=0;
		}
		
		if(selection==winner){
			printf("You win!\n");
		}
		else if(winner==0){
			printf("No Winner!\n");
		}
		else{
			printf("You lose! Winner is Horse:%d\n",winner);
		}				
	}
	if(n_horses==4) //at say�s� 3 i�in olan k�sm�n 4 i�in olan versiyonu yap�l�yor
	{
		selection=getInt(1,n_horses);
		printf("Racing starts..");
		h1=numberGeneratorBetweenRange(10,20);
		h2=numberGeneratorBetweenRange(10,20);
		h3=numberGeneratorBetweenRange(10,20);
		h4=numberGeneratorBetweenRange(10,20);
		printf("\nHorse 1:");
		for(i=0;i<h1;i++){
			printf("-");
		}
		printf("\nHorse 2:");
		for(i=0;i<h2;i++){
			printf("-");
		}
		printf("\nHorse 3:");
		for(i=0;i<h3;i++){
			printf("-");
		}
		printf("\nHorse 4:");
		for(i=0;i<h4;i++){
			printf("-");
		}
		if(h1<h2 && h1<h3 && h1<h4){
			winner=1;
		}else if(h2<h1 && h2<h3 && h2<h4){
			winner=2;
		}else if(h3<h2 && h3<h1 && h3<h4){
			winner=3;
		}
		else if(h4<h2 && h4<h1 && h4<h3){
			winner=4;
		}
		else{
			winner=0;
		}
		
		if(selection==winner){
			printf("You win!\n");
		}
		else if(winner==0){
			printf("No Winner!\n");
		}
		else{
			printf("You lose! Winner is Horse:%d\n",winner);
		}
		
		
	}
	if(n_horses==5) //5 at i�in ayn� i�lem yap�l�yor
	{
		selection=getInt(1,n_horses);
		printf("Racing starts..");
		h1=numberGeneratorBetweenRange(10,20);
		h2=numberGeneratorBetweenRange(10,20);
		h3=numberGeneratorBetweenRange(10,20);
		h4=numberGeneratorBetweenRange(10,20);
		h5=numberGeneratorBetweenRange(10,20);
		printf("\nHorse 1:");
		for(i=0;i<h1;i++){
			printf("-");
		}
		printf("\nHorse 2:");
		for(i=0;i<h2;i++){
			printf("-");
		}
		printf("\nHorse 3:");
		for(i=0;i<h3;i++){
			printf("-");
		}
		printf("\nHorse 4:");
		for(i=0;i<h4;i++){
			printf("-");
		}
		printf("\nHorse 5:");
		for(i=0;i<h5;i++){
			printf("-");
		}
		if(h1<h2 && h1<h3 && h1<h4 && h1<h5){
			winner=1;
		}else if(h2<h1 && h2<h3 && h2<h4 && h2<h5){
			winner=2;
		}else if(h3<h2 && h3<h1 && h3<h4 && h3<h5){
			winner=3;
		}
		else if(h4<h2 && h4<h1 && h4<h3 && h4<h5){
			winner=4;
		}
		else if(h5<h2 && h5<h1 && h5<h3 && h5<h4){
			winner=5;
		}
		else{
			winner=0;
		}
		
		if(selection==winner){
			printf("You win!\n");
		}
		else if(winner==0){
			printf("No Winner!\n");
		}
		else{
			printf("You lose! Winner is Horse:%d\n",winner);
		}
		
		
	}
	
		
}


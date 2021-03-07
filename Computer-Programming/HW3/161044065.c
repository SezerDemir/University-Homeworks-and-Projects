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
	int selection,flag=1; //deðiþkenler tanýmlanýyor
	srand(time(NULL));
	while(flag)
	{
		printf("1.Horse Racing Game\n2.Occurrence Counter\n3.Triangle of Sequances\n0.Exit\n");
		scanf("%d",&selection); //kullýcýdan hangi iþlemi istediði bilgisi alýnýyor
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

int getInt(int mini,int maxi)  //fonksiyon 2 paremetre olarak 2 tam sayý alýyor ve kullanýcý bu iki sayý arasýnda bir deðer girinceye kadar deðer girmesini istiyor
{
	int flag=1,number;
	while(flag){
		scanf("%d",&number);
		if(number<=maxi && number>=mini) // eðer alýnan sayý istenlen aralýkta ise döngünün bitmesi için flag deðiþkenine 0 atanýyor
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

void countOccurrence() //fonksiyon girilen sayýnýn içinde kaç tane yine girilen bir sayýdan olduðunu hesaplýyor
{	
	int digit,result,temp,counter=0,digit_search,counter1=1,flag=1,temp_search,times=1,m;
	int big_number,search_number;
	printf("Big number:");
	scanf("%d",&big_number);
	printf("Search number:");
	scanf("%d",&search_number);
	temp_search=search_number;
	temp=big_number;
	while(flag){ //ana sayýnýn içinde aranan sayýnýn basamak sayýsý hesaplanýyor
		temp_search=temp_search/10;
		if(temp_search>=1){
			counter1++; 
		}
		else{
			flag=0;
		}
	}
	for(m=0;m<counter1;m++){ //mod iþlemi için hangi sayýnýn kullanýlacaðý hesaplanýyor
		times*=10;
	}
	while(big_number>0)/*sayýnýn modu hesaplanan 10 üzeri sayý ile alýnýyor ve aranan sayýnýn içinde olup olmadýðý denetleniyor,eðer var ise counter arttýrýlarak bu deðer
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
	int number = numberGeneratorBetweenRange(2,10); // number deðiþkenine 2 ile 10 arasýnda rastgele bir sayý atanýyor
	int i,y,counter=0,counter1,counter2;
	for(i=0;i<number;i++) //döngüde istenen üçken oluþturuluyor
	{
		counter++;
		counter1=counter;
		for(y=1;y<=counter;y++){
			if(y==1){ //ilk satýrda kendinden önceki sayý olmadýðýndan dolayý özel bir durum iþleniyor
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
	n_horses=numberGeneratorBetweenRange(3,5); //at sayýsý 3 ile 5 arasýnda rastgele olarak atanýyor
	printf("Number of Horses:%d\n",n_horses);
	printf("Horse Number:");
	if(n_horses==3){ //eðer at sayýsý 3 ise 
		selection=getInt(1,n_horses);
		printf("Racing starts..");
		h1=numberGeneratorBetweenRange(10,20); /*atlara koþu zamanlarý rastgele olarak 10 ile 20 arasýnda atanýyot*/
		h2=numberGeneratorBetweenRange(10,20); 
		h3=numberGeneratorBetweenRange(10,20); 
		printf("\nHorse 1:");
		for(i=0;i<h1;i++){
			printf("-");
		}
		printf("\nHorse 2:");
		for(i=0;i<h2;i++){            //atlarýn zamanlarýný simgeleyen þekiller çiziliyor
			printf("-");
		}
		printf("\nHorse 3:");
		for(i=0;i<h3;i++){
			printf("-");
		}
		if(h1<h2 && h1<h3){
			winner=1;
		}else if(h2<h1 && h2<h3){      //kazanan at belirleniyor ve numarasý winner deðiþkenine atanýyor
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
	if(n_horses==4) //at sayýsý 3 için olan kýsmýn 4 için olan versiyonu yapýlýyor
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
	if(n_horses==5) //5 at için ayný iþlem yapýlýyor
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


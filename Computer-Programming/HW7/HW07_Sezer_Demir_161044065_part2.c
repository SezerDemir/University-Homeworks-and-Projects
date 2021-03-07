#include <stdio.h>

void input_date();
void new_date();

int main(){
	int flag=1,choice;
	while(flag){ //men�n�n olu�turuldu�u yer
		printf("\nSelect the process...\n1-Input Date\n2-Convert Format\n0-Exit\nChoice:");
		scanf("%d",&choice);
		switch(choice){ // se�ime g�re gerekli fonksiyonlar �a�r�l�yor
			case 1:
				input_date();
				break;
			case 2:
				new_date();
				break;
			case 0:
				flag=0;
				break;
		}
	}	
	return 0;
}

void input_date(){
	FILE *file=fopen("input_date.txt","w"); 
	char starting_date[10],ending_date[10],date[11]; /*gerekli de�i�kenler olu�turuluyor */
	int start[8],end[8],i,j=0,day_s,month_s,year_s,day_e,month_e,year_e;
	printf("Enter the starting date:"); //ba�lang�� tarihi al�n�yor
	scanf("%s",starting_date);
	printf("Enter the starting date:"); // biti� tarihi al�n�yor
	scanf("%s",ending_date);

	day_s=(starting_date[1]-48)*1+(starting_date[0]-48)*10;  /* tarihler �zerinde i�lem yap�labilmesi i�in say�sal de�erlere d�n��t�r�l�p gerekli de�i�kenlere atan�yor */
	month_s=(starting_date[4]-48)*1+(starting_date[3]-48)*10;
	year_s=(starting_date[9]-48)*1+(starting_date[8]-48)*10+(starting_date[7]-48)*100+(starting_date[6]-48)*1000;
	day_e=(ending_date[1]-48)*1+(ending_date[0]-48)*10;
	month_e=(ending_date[4]-48)*1+(ending_date[3]-48)*10;
	year_e=(ending_date[9]-48)*1+(ending_date[8]-48)*10+(ending_date[7]-48)*100+(ending_date[6]-48)*1000;
	
	do{
		day_s++;// ba�lang�� ve biti� tarihi aras�ndaki tarihler yazd�r�laca�� i�in ba�lang�� g�n� es ge�iliyor
		
		/*aylara g�re ay�n ka� g�n i�erdi�i bilgisi kontrol edilip g�n o de�eri a�ar ise 1 e e�itlenip ay 1 artt�r�l�yor */
		if(month_s==1 || month_s==3 || month_s==5 || month_s==7 || month_s==8 || month_s==10 || month_s==12){
			if(day_s>31){
				day_s=1;
				month_s++;
			}
		}
		if(month_s==4 || month_s==6 || month_s==9 || month_s==11){
			if(day_s>30){
				day_s=1;
				month_s++;
			}
		}
		/*art�k y�l olay� hesaplan�p gerekli i�lemler yap�l�yor*/
		if(month_s==2){
			if(year_s%100==0){
				if(year_s%400!=0){ 
					if(day_s>28){
					day_s=1;
					month_s++;	
					}
				}
				else{
					if(day_s>29){
						day_s=1;
						month_s++;
					}			
				}
			}else if( year_s%4==0){
				if(day_s>29){
					day_s=1;
					month_s++;
				}
			}
			else{
				if(day_s>28){
					day_s=1;
					month_s++;
				}
			}	
		}
		if(month_s>12){ //e�er ay 12 say�s�n� ge�er ise ay 1 e e�itleniyor ve y�l 1 artt�r�l�yor
			month_s=1;
			year_s++;
		}
		date[0]=day_s/10+48; //tarih gerekli formatta yazd�r�labilmesi i�in date adl� stringe atan�yor
		date[1]=day_s%10+48;
		date[2]='/';
		date[3]=month_s/10+48;
		date[4]=month_s%10+48;
		date[5]='/';
		date[6]=year_s/1000+48;
		date[7]=(year_s%1000)/100+48;
		date[8]=(year_s%100)/10+48;
		date[9]=year_s%10+48;
		date[10]=0;
		fprintf(file,"%s\n",date); //tarih istenen formatta dosyaya yazd�r�l�yor

	}while(day_s!=day_e || month_s!=month_e || year_s!=year_e);
	fclose(file);
}
	

void new_date() //Key value method u ad� verilen y�ntemle herhangi bir tarihin g�n� bulunuyor ve gerekli formatta yaz�l�yor
{
	int day,month,year,i=0,new_year,result,key_value;
	FILE *file_read,*file_write;
	file_read=fopen("input_date.txt","r");
	file_write=fopen("new_date.txt","w");
	while(fscanf(file_read,"%d/%d/%d",&day,&month,&year)!=EOF){	//i�lem input_date dosyas�n�n sonuncu k�sm�na kadar devam ediyor
		new_year = year; 
		result=year%100;
		result/=4; //key_value de�erini bulmak i�in gerekli i�lemler yap�l�yor
		result+=day;
		if(month==1 || month==10){
			key_value = 1;
		}else if(month==5){
			key_value = 2;
		}else if(month==1 || month==7){
			key_value = 0;
		}else if(month==8){
			key_value = 3;
		}else if(month==2 || month==3 || month==11){
			key_value = 4;
		}else if(month==6){
			key_value = 5;
		}else if(month==9 || month==12){
			key_value = 6;
		}
		result+=key_value;
		if(month==1 || month==2){ // tarih art�k y�lda ise resultdan 1 ��kart�l�yor
			if(year%100==0){
				if(year%400==0){
					result-=1;
				}
			}
			else if(year%4==0){
				result-=1;
			}
		}
		while(new_year<1700 || new_year>2099){ //e�er verilen tarih 1700s ve 2000s li y�llar�n d���nda ise o araya �ekiliyor
			if(new_year<1700){
				new_year+=400;
			}else if(new_year > 2099){
				new_year-=400;
			}
		}
		if(new_year>=1700 && new_year<1800){
			result+=4;
		}else if(new_year>=1800 && new_year<1900){
			result+=2;
		}else if(new_year>=2000 && new_year<2100){
			result+=6;
		}
		result=result+(year%100);
		result=result%7;
		switch(result){ //Bu methoda g�re belirlenen de�er ile e�le�en g�n yazd�r�l�yor
			case 1:
				fprintf(file_write,"Sunday");
				break;
			case 2:
				fprintf(file_write,"Monday");
				break;
			case 3:
				fprintf(file_write,"Tuesday");;
				break;
			case 4:
				fprintf(file_write,"Wednesday");;
				break;
			case 5:
				fprintf(file_write,"Thursday");;
				break;
			case 6:
				fprintf(file_write,"Friday");;
				break;
			case 0:
				fprintf(file_write,"Saturday");
				break;
		} 
		switch(month){ // ay ad� dosyaya yazd�r�l�yor
			case 1:
				fprintf(file_write,",January ");
				break;
			case 2:
				fprintf(file_write,",February ");
				break;
			case 3:
				fprintf(file_write,",March ");
				break;
			case 4:
				fprintf(file_write,",April ");
				break;
			case 5:
				fprintf(file_write,",May ");
				break;
			case 6:
				fprintf(file_write,",June ");
				break;
			case 7:
				fprintf(file_write,",July ");
				break;
			case 8:
				fprintf(file_write,",August ");
				break;
			case 9:
				fprintf(file_write,",September ");
				break;
			case 10:
				fprintf(file_write,",October ");
				break;
			case 11:
				fprintf(file_write,",November ");
				break;
			case 12:
				fprintf(file_write,",December ");
				break;
		} 
		fprintf(file_write,"%d,%d\n",day,year); // verilen formata g�re g�n ve y�l say�sal olarak yazd�r�l�yor
	}
	fclose(file_read);
	fclose(file_write);
}

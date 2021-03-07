#include <stdio.h>
int find_size_of_line(char[]);
void reverse_line(char[], int);
void encrypt_open_msg();
void decrypt_secret_msg();

int main(){
	int flag=1,choice;
	while(flag){
		printf("1.Encrypt\n2.Decrypt\n0.Exit\nChoice:"); //men� olu�turuluyor
		scanf("%d",&choice);
		switch(choice){ //kullan�c�n�n se�imine g�re i�lemler ger�ekle�tiriliyor
			case 1:{
				encrypt_open_msg();
				break;
			}
			case 2:{
				decrypt_secret_msg();
				break;
			}
			case 0:{
				flag=0;
				break;
			}
		}
	}
	
	return 0;
}

int find_size_of_line(char line[]){
	int size=0,i=0;
	while(line[i]!=0){ //arrayin sonuna gelene kadar size arttt�r�l�yor b�ylece dizinin boyutu elde ediliyor
		size++;
		i++;
	}
	return size;
}

void reverse_line(char line[], int line_lenght){
	int i=0,k,j,temp,s_index=0,e_index=0;
	while(line[i]!=0){ //dizi sonuna gelinceye kadar i�lem yap�l�yor
		if(line[i]=='\n' || line[i+1]==0){ //e�er alt sat�ra ge�iliyor ise yada dizinin sonuna geliniyor ise 
			if(line[i+1]==0){ //e�er dizinin sonuna gelinmi� ise biti� indexi i olarak atan�yor
				e_index=i;
			}else{ //e�er alt sat�ra gelinmi� ise i-1 olarak atan�yor
				e_index=i-1;
			}
			for(j=s_index,k=e_index;j<(s_index+e_index+1)/2;j++,k--){ //bir sat�r�n ba�lang�� ve biti� indexlerine g�re sat�r ters �evriliyor
				temp=line[j];
				line[j]=line[k];
				line[k]=temp;
			}
			s_index=e_index+2; 
		}
		i++;
	}
}
void encrypt_open_msg(){
	char arr[1024];
	int i=0,size,flag=1,key,temp_value;
	while(key<1 || key>26){ //kullan�c�dan �ifreleme anahtar� al�n�yor
		printf("Key:");
		scanf("%d",&key);
	}
	FILE *file,*file_o; //dosya t�r�nde pointer olu�turuluyor
	file=fopen("open_msg.txt","r"); //bu pointer open_msg.txt dosyas�n�n reading modunda a�mak i�in ayarlan�yor
	while(fscanf(file,"%c",&arr[i])!=EOF){ //dosynan�n i�indeki b�t�n karakterler arr dizisine aktar�l�yor
		i++;
	}
	arr[i]=0;
	size=find_size_of_line(arr); //arr dizisinin uzunlu�u fonksiyon yard�m� ile bulunuyor ve atan�yor
	reverse_line(arr,size); //arr dizisi istenilen formata g�re bu fonksiyon ile ters �evriliyor
	i=0;
	while(arr[i]!=0){
		if(arr[i]!='\n'){//e�er de�er \n de�il ise 
			temp_value=arr[i];
			if(arr[i]==' '){ //b�luklar _ ile de�i�tiriliyor
				arr[i]='_';
			}else if(arr[i]=='.'){ //noktalar * ile de�i�tiriliyor
				arr[i]='*';
			}else{
				temp_value-=key; // al�nan anahtara g�re karakterler �ifreleniyor
				if(temp_value<97){
				temp_value+=26;
				}
				arr[i]=temp_value;
			}
		}
		i++;
	}
	printf("\n\n%s\n\n",arr); // metnin �ifrelenmi� hali ekranda g�steriliyor
	file_o=fopen("secret_msg.txt","w"); //file_o pointer� ile secter_msg.txt dosyas� writing modu ile a��l�yor
	fprintf(file_o,"%s",arr); // dizi aynen dosyaya yazd�r�l�yor
	fclose(file_o);
	fclose(file);
}
void decrypt_secret_msg(){ //�ifreleme fonksiyonunun yapt��� �eyi terisine yap�yor
	char arr[1024];
	int i=0,size,temp_value,key=0;
	FILE *file_r,*file_w;
	file_r=fopen("secret_msg.txt","r");
	while(key<1 || key>26){
		printf("Key:");
		scanf("%d",&key);
	}
	while(fscanf(file_r,"%c",&arr[i])!=EOF){
		i++;
	}
	arr[i]=0;
	size=find_size_of_line(arr);
	reverse_line(arr,size);
	i=0;
	while(arr[i]!=0){
			if(arr[i]!='\n'){
				temp_value=arr[i];
				if(arr[i]=='_'){
					arr[i]=' ';
				}else if(arr[i]=='*'){
					arr[i]='.';
				}else{
					temp_value+=key;
					if(temp_value>122){
					temp_value-=26;
				}
				arr[i]=temp_value;
			}
		}
		i++;
	}
	printf("\n\n%s\n\n",arr);
	file_w=fopen("open_msg.txt","w");
	fprintf(file_w,"%s",arr);
	fclose(file_w);
	fclose(file_r);
}

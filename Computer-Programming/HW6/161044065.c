#include <stdio.h>
int find_size_of_line(char[]);
void reverse_line(char[], int);
void encrypt_open_msg();
void decrypt_secret_msg();

int main(){
	int flag=1,choice;
	while(flag){
		printf("1.Encrypt\n2.Decrypt\n0.Exit\nChoice:"); //menü oluþturuluyor
		scanf("%d",&choice);
		switch(choice){ //kullanýcýnýn seçimine göre iþlemler gerçekleþtiriliyor
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
	while(line[i]!=0){ //arrayin sonuna gelene kadar size artttýrýlýyor böylece dizinin boyutu elde ediliyor
		size++;
		i++;
	}
	return size;
}

void reverse_line(char line[], int line_lenght){
	int i=0,k,j,temp,s_index=0,e_index=0;
	while(line[i]!=0){ //dizi sonuna gelinceye kadar iþlem yapýlýyor
		if(line[i]=='\n' || line[i+1]==0){ //eðer alt satýra geçiliyor ise yada dizinin sonuna geliniyor ise 
			if(line[i+1]==0){ //eðer dizinin sonuna gelinmiþ ise bitiþ indexi i olarak atanýyor
				e_index=i;
			}else{ //eðer alt satýra gelinmiþ ise i-1 olarak atanýyor
				e_index=i-1;
			}
			for(j=s_index,k=e_index;j<(s_index+e_index+1)/2;j++,k--){ //bir satýrýn baþlangýç ve bitiþ indexlerine göre satýr ters çevriliyor
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
	while(key<1 || key>26){ //kullanýcýdan þifreleme anahtarý alýnýyor
		printf("Key:");
		scanf("%d",&key);
	}
	FILE *file,*file_o; //dosya türünde pointer oluþturuluyor
	file=fopen("open_msg.txt","r"); //bu pointer open_msg.txt dosyasýnýn reading modunda açmak için ayarlanýyor
	while(fscanf(file,"%c",&arr[i])!=EOF){ //dosynanýn içindeki bütün karakterler arr dizisine aktarýlýyor
		i++;
	}
	arr[i]=0;
	size=find_size_of_line(arr); //arr dizisinin uzunluðu fonksiyon yardýmý ile bulunuyor ve atanýyor
	reverse_line(arr,size); //arr dizisi istenilen formata göre bu fonksiyon ile ters çevriliyor
	i=0;
	while(arr[i]!=0){
		if(arr[i]!='\n'){//eðer deðer \n deðil ise 
			temp_value=arr[i];
			if(arr[i]==' '){ //bþluklar _ ile deðiþtiriliyor
				arr[i]='_';
			}else if(arr[i]=='.'){ //noktalar * ile deðiþtiriliyor
				arr[i]='*';
			}else{
				temp_value-=key; // alýnan anahtara göre karakterler þifreleniyor
				if(temp_value<97){
				temp_value+=26;
				}
				arr[i]=temp_value;
			}
		}
		i++;
	}
	printf("\n\n%s\n\n",arr); // metnin þifrelenmiþ hali ekranda gösteriliyor
	file_o=fopen("secret_msg.txt","w"); //file_o pointerý ile secter_msg.txt dosyasý writing modu ile açýlýyor
	fprintf(file_o,"%s",arr); // dizi aynen dosyaya yazdýrýlýyor
	fclose(file_o);
	fclose(file);
}
void decrypt_secret_msg(){ //þifreleme fonksiyonunun yaptýðý þeyi terisine yapýyor
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

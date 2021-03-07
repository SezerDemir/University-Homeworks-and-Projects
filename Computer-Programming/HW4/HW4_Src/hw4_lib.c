/*
** hw4_lib.c:
**
** The source file implementing library functions.
**
** Author: Yakup Genc. (c) 2018
**
** Revision: 2018.03.27.08.19
** 
*/

#include "hw4_lib.h"
#include <math.h>
double integral(double f(double x), double xs, double xe, double delta){
	double total=0.0;
	double i=0.0;
	for(i=f(xs);i<f(xe);i+=delta){ //alt sýnýrdan üst sýnýra kadar olan dikdörtgenlerin alanlarý toplanýyor
		total=total+(delta*f(xs));
		xs+=delta; // her seferinde alt sýnýr delta kadar arttýrýlarak diðer dikdörtgene geçilmesi saðlanýyor
	}
	return total;
}
double integral2(double f(double x, double y), double xs, double xe, double ys, double ye, double delta){
	double total=0.0,total1=0.0;
	double i,j;
	for(i=f(xs,y);i<f(xe,y);i+=delta){ //dýþ döngüde bir dikdörtgenin alaný hesaplanýyor
		total=(delta*f(xs,y));
		for(j=f(x,ys);j<f(x,ye);j+=delta){ //iç döngüde bu alanýn diðer bir boyuta göre integrali alýnarak hacmine ulaþýlýyor ve total1 deðiþkenine ekleniyor
			total1=total1+(total*delta);
		}
		xs+=delta; // her seferinde alt sýnýr delta kadar arttýrýlarak diðer dikdörtgene geçilmesi saðlanýyor
	}
	return total1;
}
int derivatives(double f(double a), double x, double eps, double * d1, double * d2){
	if(eps==0){
		return -1; //payda 0 olduðu durum için -1 döndürülüyor çünkü matematiksel bir hata ile karþýlaþýlacak
	}
	else{
		*d1=(f(x+eps)-f(x-eps))/(2*eps); //fonksiyonun ilk türevi
		*d2=(f(x+eps)-2*f(x)+f(x-eps))/(pow(eps,2)); // fonksiyonun ikinci türevi
		return 0;
	}
}
int compare_derivatives(double f(double a), double d1(double b), double d2(double b), double x, double eps, double * e1, double *e2){
	double der1,der2;
	derivatives(f,x,eps,&der1,&der2); //der1 ve der2 deðiþkenlerine kendi yazdýðýmýz türev alma fonksiyonlarýndan elde edilen sonuçlar atlýyor
	*e1=fabs(der1-d1(x)); //gerçek türev ile elde ettiðimiz türevler arasýndaki mutlak fark hesaplanýp e1 ve e2 deðiþkenlerine aktarýlýyor
	*e2=fabs(der2-d2(x));
	return 0;
}
int find_roots(double f(double x), double * x1, double * x2, double * x3, double * x4){
	int flag=1,counter=0;
	double y=-100.0;
	while(flag){ // y -100 den baþlýyor ve fonksiyondan yerine yazýlýp 0 a eþit olmadýðý denetleniyor , deðil ise 0.001 arttýrýlýyor ve kök aranýyor.
		if(f(y)==0){ //eðer y 100 e ulaþmýþsa kök yok sayýlýyor ve 0.0 deðeri verliyor
			*x1=f(y);
			flag=0;
		}
		else{
			y+=0.001;
		}
		if(y==100.001){
			flag=0;
			*x1=0;
		}
	}
	flag=1;
	while(flag){
		if(f(y)==0){
			*x2=f(y);
			flag=0;
		}
		else{
			y+=0.001;
		}
		if(y==100){
			flag=0;
			*x1=0;
		}
	}
	flag=1;
	while(flag){
		if(f(y)==0){
			*x2=f(y);
			flag=0;
		}
		else{
			y+=0.001;
		}
		if(y==100){
			flag=0;
			*x3=0;
		}
	}
	flag=1;
	while(flag){
		if(f(y)==0){
			*x4=f(y);
			flag=0;
		}
		else{
			y+=0.001;
		}
		if(y==100){
			flag=0;
			*x4=0;
		}
	}
}


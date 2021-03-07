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
	for(i=f(xs);i<f(xe);i+=delta){ //alt s�n�rdan �st s�n�ra kadar olan dikd�rtgenlerin alanlar� toplan�yor
		total=total+(delta*f(xs));
		xs+=delta; // her seferinde alt s�n�r delta kadar artt�r�larak di�er dikd�rtgene ge�ilmesi sa�lan�yor
	}
	return total;
}
double integral2(double f(double x, double y), double xs, double xe, double ys, double ye, double delta){
	double total=0.0,total1=0.0;
	double i,j;
	for(i=f(xs,y);i<f(xe,y);i+=delta){ //d�� d�ng�de bir dikd�rtgenin alan� hesaplan�yor
		total=(delta*f(xs,y));
		for(j=f(x,ys);j<f(x,ye);j+=delta){ //i� d�ng�de bu alan�n di�er bir boyuta g�re integrali al�narak hacmine ula��l�yor ve total1 de�i�kenine ekleniyor
			total1=total1+(total*delta);
		}
		xs+=delta; // her seferinde alt s�n�r delta kadar artt�r�larak di�er dikd�rtgene ge�ilmesi sa�lan�yor
	}
	return total1;
}
int derivatives(double f(double a), double x, double eps, double * d1, double * d2){
	if(eps==0){
		return -1; //payda 0 oldu�u durum i�in -1 d�nd�r�l�yor ��nk� matematiksel bir hata ile kar��la��lacak
	}
	else{
		*d1=(f(x+eps)-f(x-eps))/(2*eps); //fonksiyonun ilk t�revi
		*d2=(f(x+eps)-2*f(x)+f(x-eps))/(pow(eps,2)); // fonksiyonun ikinci t�revi
		return 0;
	}
}
int compare_derivatives(double f(double a), double d1(double b), double d2(double b), double x, double eps, double * e1, double *e2){
	double der1,der2;
	derivatives(f,x,eps,&der1,&der2); //der1 ve der2 de�i�kenlerine kendi yazd���m�z t�rev alma fonksiyonlar�ndan elde edilen sonu�lar atl�yor
	*e1=fabs(der1-d1(x)); //ger�ek t�rev ile elde etti�imiz t�revler aras�ndaki mutlak fark hesaplan�p e1 ve e2 de�i�kenlerine aktar�l�yor
	*e2=fabs(der2-d2(x));
	return 0;
}
int find_roots(double f(double x), double * x1, double * x2, double * x3, double * x4){
	int flag=1,counter=0;
	double y=-100.0;
	while(flag){ // y -100 den ba�l�yor ve fonksiyondan yerine yaz�l�p 0 a e�it olmad��� denetleniyor , de�il ise 0.001 artt�r�l�yor ve k�k aran�yor.
		if(f(y)==0){ //e�er y 100 e ula�m��sa k�k yok say�l�yor ve 0.0 de�eri verliyor
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


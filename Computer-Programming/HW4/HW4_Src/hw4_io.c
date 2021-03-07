/*
** hw4_io.c:
**
** The source file implementing input and output functions.
**
** Author: Yakup Genc. (c) 2018
**
** Revision: 2018.03.27.08.19
** 
*/
#include <stdio.h>
#include <math.h>
#include "hw4_io.h"

int read_polynomial3(double * a0, double * a1, double * a2, double * a3){//takes coefficients of a 3 degree polynomial
	double b0,b1,b2,b3;
	printf("Enter the coefficients of 3 degree polynomial");
	scanf("%lf%lf%lf%lf",&b0,&b1,&b2,&b3);
	*a0=b0;
	*a1=b1;
	*a2=b2;
	*a3=b3;
}

int read_polynomial4(double * a0, double * a1, double * a2, double * a3, double * a4){ //takes coefficients of a 4 degree polynomial
	int b0,b1,b2,b3,b4;
	printf("Enter the coefficients of 3 degree polynomial");
	scanf("%lf%lf%lf%lf",&b0,&b1,&b2,&b3,&b4);
	*a0=b0;
	*a1=b1;
	*a2=b2;
	*a3=b3;
	*a4=b4;
}

void write_polynomial3(double a0, double a1, double a2, double a3){
	int i;
	char sign[4],x[4]={'x','x','x','x'},sign1[4]={'^','^','^','^'},power[4]={'0','1','2','3'}; 
	double coef[4]; //baþkatsayýlarýn mutlak deðerini tutan dizi
	coef[0]=fabs(a0);
	coef[1]=fabs(a1);
	coef[2]=fabs(a2);
	coef[3]=fabs(a3);
	if(a0>=0){ //sign dizisi herbir baþkatsayýnýn iþaretini tutuyor
		sign[0]='+';
	}else{
		sign[0]='-';
	}
	if(a1>=0){
		sign[1]='+';
	}else{
		sign[1]='-';
	}
	if(a2>=0){
		sign[2]='+';
	}else{
		sign[2]='-';
	}
	if(a3>=0){
		sign[3]='+';
	}else{
		sign[3]='-';
	}
	for(i=3;i>=0;i--){//i deðiþkeni elemanlarýn kuvvetini temsil ediyor
		if(coef[i]==0){
		}
		else if(coef[i]==1){//baþkatsayý 1 ise ona göre ekrana bastýrýlýyor
			if(i==1){
				printf("%c%c",sign[i],x[i]);	
			}else if(i!=0){
				printf("%c%c%c%c",sign[i],x[i],sign1[i],power[i]);
			}else if(i==0){
				printf("%c%.1lf",sign[i],coef[i]);
			}
		}
		else{
			if(i==0){
				printf("%c%.1lf",sign[i],coef[i]);
			}else if(i==1){
				printf("%c%.1lf%c",sign[i],coef[i],x[i]);
			}else{
				printf("%c%.1lf%c%c%c",sign[i],coef[i],x[i],sign1[i],power[i]);
			}
		}
	}
}
void write_polynomial4(double a0, double a1, double a2, double a3, double a4){ 
	int i;
	char sign[5],x[5]={'x','x','x','x','x'},sign1[5]={'^','^','^','^','^'},power[5]={'0','1','2','3','4'};
	double coef[5]; // baþkatsayýlarýn mutlak deðerleri tutuluyor
	coef[0]=fabs(a0);
	coef[1]=fabs(a1);
	coef[2]=fabs(a2);
	coef[3]=fabs(a3);
	coef[4]=fabs(a4);
	if(a0>=0){ //baþkatsayýlarýn iþaretleri sign dizisine atanýyor
		sign[0]='+';
	}else{
		sign[0]='-';
	}
	if(a1>=0){
		sign[1]='+';
	}else{
		sign[1]='-';
	}
	if(a2>=0){
		sign[2]='+';
	}else{
		sign[2]='-';
	}
	if(a3>=0){
		sign[3]='+';
	}else{
		sign[3]='-';
	}
	if(a4>=0){
		sign[4]='+';
	}else{
		sign[4]='-';
	}
	for(i=4;i>=0;i--){ //i deðiþkeni elemanlarýn kuvvetini temsil ediyor
		if(coef[i]==0){
		}
		else if(coef[i]==1.0){ //baþkatsayý 1 ise ona göre ekrana bastýrýlýyor
	
			if(i==1){
				printf("%c%c",sign[i],x[i]);	
			}else if(i!=0){
				printf("%c%c%c%c",sign[i],x[i],sign1[i],power[i]);
			}else if(i==0){ 
				printf("%c%.1lf",sign[i],coef[i]);
			}
		}
		else{
			if(i==0){
				printf("%c%.1lf",sign[i],coef[i]);
			}else if(i==1){
				printf("%c%.1lf%c",sign[i],coef[i],x[i]);
			}else{
				printf("%c%.1lf%c%c%c",sign[i],coef[i],x[i],sign1[i],power[i]);
			}
		}
	}
}

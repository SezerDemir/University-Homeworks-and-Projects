#include <stdio.h>
#include <math.h>
#define PI 3.14

double circle_area(double);
double calc_hypotenuse(int,int);
double calc_radius_of_smallest_circle(int,int);
double calc_radius_of_largest_circle(int,int);
double calc_area_of_smallest_circle(int,int);   /*Fonksiyon Referanslar�*/
double calc_area_of_largest_circle(int,int);
double calc_area_of_smallest_square(int,int);
double calc_area_of_largest_square(int,int);
double calc_area_of_square (int);
void display_results(double,double,double,double);

int main() {
    int side1,side2;
    double largest_circle,largest_square,smallest_circle,smallest_square;
    printf("Enter the first side of the rectangle:");
    scanf("%d",&side1);
    printf("Enter tbe second side of the rectangle:");
    scanf("%d",&side2);
    largest_circle=calc_area_of_largest_circle(side1,side2); /* b�y�k �emberin alan� hesaplan�p de�i�kene atan�r*/
    largest_square=calc_area_of_largest_square(side1,side2); /*b�y�k karenin alan� hesaplan�p de�i�kene atan�r*/
    smallest_circle=calc_area_of_smallest_circle(side1,side2); /*k���k �emberin alan� hesaplan�p de�i�kene atan�r*/
    smallest_square=calc_area_of_smallest_square(side1,side2); /*k���k karenin alan� hesaplan�p de�i�kene atan�r*/


    display_results(largest_circle,largest_square,smallest_circle,smallest_square);
    return 0;
}

double circle_area(double radius){    /*�emberin alan�n� , �emberin yar��ap�n� paremetre olarak al�p hesaplayan fonksiyon*/
    return pow(radius,2.0)*PI;
}
double calc_hypotenuse(int side1, int side2){ /*Dikd�rtgenin hipoten�s�n� hesaplayan fonksiyon */
    double square=pow(side1,2)+pow(side2,2); /*kenarlar�n kareleri al�n�p topland�ktan sonra bir de�i�kene atan�yor */
    double hypotenuse=sqrt(square); /*de�i�kenin k�k� al�n�yor */
    return hypotenuse;
}

double calc_radius_of_smallest_circle(int side1, int side2){/*Dikd�rtgeni �evreleyen �emberin yar��ap� hesaplan�yor */
    return calc_hypotenuse(side1,side2)/2.0;
}

double calc_radius_of_largest_circle(int side1, int side2){ /*Dikd�rtgenin i�ine s��an en b�y�k �emberin yar��ap� hesaplan�yor*/
    if(side1<=side2){
        return side1/2.0;
    }
    else if (side2<side1){
        return side2/2.0;
    }
}

double calc_area_of_smallest_circle(int side1, int side2){/*Dikd�rtgeni �evreleyen �emberin alan� hesaplan�yor*/
    return circle_area(calc_radius_of_smallest_circle(side1,side2));
}

double calc_area_of_largest_circle(int side1, int side2){/*Dild�rtgenin i�ine s��an en b�y�k �emberin alan� hesaplan�yor */
    return circle_area(calc_radius_of_largest_circle(side1,side2));
}

double calc_area_of_smallest_square(int side1, int side2){/*dikd�rtgeni �evreleyen en k���k karenin alan� hesaplan�yor */
    if(side1<=side2){
        return calc_area_of_square(side2);
    }
    else{
        return calc_area_of_square(side1);
    }
}

double calc_area_of_largest_square(int side1, int side2){/*dikd�rtgenin i�ine s��an en b�y�k karenin alan� hesaplan�yor */
    if(side1>=side2){
        return calc_area_of_square(side2);
    }
    else{
        return calc_area_of_square(side1);
    }
}

double calc_area_of_square (int side){/*Paremetre olarak bir kesar� verilen karenin alan� hesaplan�yor*/
    double n_side=side;
    return pow(n_side,2.0);
}

void display_results(double largest_circle,double largest_square,double smallest_circle,double smallest_square){/*Fonksiyonlar ile hesaplanan alan de�erleri ekrana bast�r�l�yor*/
    printf("The area of the largest circle that fits inside the rectangle is:%.2lf \n"
                   "The area of the largest square that fits inside the rectangle is:%.2lf\n"
                   "The area of the smallest circle that surrounds the same rectangle is:%.2lf\n"
                   "The area of the smallest square that surrounds the same rectangle is:%.2lf",largest_circle,largest_square,smallest_circle,smallest_square);
}

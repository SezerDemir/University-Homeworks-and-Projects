#include <stdio.h>
#include <math.h>
#define PI 3.14

double circle_area(double);
double calc_hypotenuse(int,int);
double calc_radius_of_smallest_circle(int,int);
double calc_radius_of_largest_circle(int,int);
double calc_area_of_smallest_circle(int,int);   /*Fonksiyon Referanslarý*/
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
    largest_circle=calc_area_of_largest_circle(side1,side2); /* büyük çemberin alaný hesaplanýp deðiþkene atanýr*/
    largest_square=calc_area_of_largest_square(side1,side2); /*büyük karenin alaný hesaplanýp deðiþkene atanýr*/
    smallest_circle=calc_area_of_smallest_circle(side1,side2); /*küçük çemberin alaný hesaplanýp deðiþkene atanýr*/
    smallest_square=calc_area_of_smallest_square(side1,side2); /*küçük karenin alaný hesaplanýp deðiþkene atanýr*/


    display_results(largest_circle,largest_square,smallest_circle,smallest_square);
    return 0;
}

double circle_area(double radius){    /*Çemberin alanýný , çemberin yarýçapýný paremetre olarak alýp hesaplayan fonksiyon*/
    return pow(radius,2.0)*PI;
}
double calc_hypotenuse(int side1, int side2){ /*Dikdörtgenin hipotenüsünü hesaplayan fonksiyon */
    double square=pow(side1,2)+pow(side2,2); /*kenarlarýn kareleri alýnýp toplandýktan sonra bir deðiþkene atanýyor */
    double hypotenuse=sqrt(square); /*deðiþkenin kökü alýnýyor */
    return hypotenuse;
}

double calc_radius_of_smallest_circle(int side1, int side2){/*Dikdörtgeni çevreleyen çemberin yarýçapý hesaplanýyor */
    return calc_hypotenuse(side1,side2)/2.0;
}

double calc_radius_of_largest_circle(int side1, int side2){ /*Dikdörtgenin içine sýðan en büyük çemberin yarýçapý hesaplanýyor*/
    if(side1<=side2){
        return side1/2.0;
    }
    else if (side2<side1){
        return side2/2.0;
    }
}

double calc_area_of_smallest_circle(int side1, int side2){/*Dikdörtgeni çevreleyen çemberin alaný hesaplanýyor*/
    return circle_area(calc_radius_of_smallest_circle(side1,side2));
}

double calc_area_of_largest_circle(int side1, int side2){/*Dildörtgenin içine sýðan en büyük çemberin alaný hesaplanýyor */
    return circle_area(calc_radius_of_largest_circle(side1,side2));
}

double calc_area_of_smallest_square(int side1, int side2){/*dikdörtgeni çevreleyen en küçük karenin alaný hesaplanýyor */
    if(side1<=side2){
        return calc_area_of_square(side2);
    }
    else{
        return calc_area_of_square(side1);
    }
}

double calc_area_of_largest_square(int side1, int side2){/*dikdörtgenin içine sýðan en büyük karenin alaný hesaplanýyor */
    if(side1>=side2){
        return calc_area_of_square(side2);
    }
    else{
        return calc_area_of_square(side1);
    }
}

double calc_area_of_square (int side){/*Paremetre olarak bir kesarý verilen karenin alaný hesaplanýyor*/
    double n_side=side;
    return pow(n_side,2.0);
}

void display_results(double largest_circle,double largest_square,double smallest_circle,double smallest_square){/*Fonksiyonlar ile hesaplanan alan deðerleri ekrana bastýrýlýyor*/
    printf("The area of the largest circle that fits inside the rectangle is:%.2lf \n"
                   "The area of the largest square that fits inside the rectangle is:%.2lf\n"
                   "The area of the smallest circle that surrounds the same rectangle is:%.2lf\n"
                   "The area of the smallest square that surrounds the same rectangle is:%.2lf",largest_circle,largest_square,smallest_circle,smallest_square);
}

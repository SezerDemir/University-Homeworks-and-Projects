#include <stdio.h>

int main() {
    int number,digit_n,flag=1,result; //declares veriables
    while(flag) {     // loop continious infinithly until user enters a number in range
        printf("Enter a number:");
        scanf("%d", &number);
        if(number>=23 && number <=98760) //if number is in range , flag is decraled as 0 which is performs to exit from loop
        {
            flag=0;
        }
        else
        {
            printf("\nEnter a number between 23 and 98760 !!!\n");
        }


    }
    result=number/10; //to find numberofplaces number divided by 10 and that value assign to result
    if (result < 10) //numberofplaces assigns to digit_n veriable
    {
        digit_n = 2;
    }
    else if(result>=10 && result<100)
    {
        digit_n=3;
    }
    else if(result>=100 && result<1000)
    {
        digit_n=4;
    }
    else if(result>=1000 && result<10000)
    {
        digit_n=5;
    }

    if(digit_n==2) // for each digit_n , it seperates digits into parts and displays them on the screne
    {
        printf("The first digit is %d\nThe second digit is %d",(number%10),number/10);

    }
    else if(digit_n==3)
    {
        printf("The first digit is %d\nThe second digit is %d\nThe third digit is %d",(number%10),(number/10)%10,(number/100));
    }
    else if(digit_n==4)
    {
        printf("The first digit is %d\nThe second digit is %d\nThe third digit is %d\nThe fourth digit is %d",(number%10),(number/10)%10,(number/100)%10,(number/1000));
    }
    else if(digit_n==5)
    {
        printf("The first digit is %d\nThe second digit is %d\nThe third digit is %d\nThe fourth digit is %d\nThe fifth digit is %d\n",(number%10),(number/10)%10,(number/100)%10,(number/1000)%10,(number/10000));
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
int main() {
    int count,score,flag1=1,flag2=1,MAX_score=-1,MIN_score=101,counter_A=0,counter_B=0,counter_C=0,counter_D=0,counter_F=0,MAX_index,MIN_index,index=0,select,i,counter=-1;
    double avarage=0;                         /* Declaring veriables */
    char MAX_grade,MIN_grade;    
    srand(40);
    while(flag1)            /* User enters student count, if it's out of the range , program gives error message */
    {
        printf("Enter student count:");
        scanf("%d",&count);
        if(count<3 || count>50)
        {
            printf("Not in Range!!\n");
        }
        else{
            flag1=0;
        }
    }

    for(i=0;i<count;i++)    
    {
        score=rand()%101;     /* Declaring grades randomly*/
        avarage+=score;         
        index++;
		counter++;      
		if(counter==10){
			printf("\n%d ",score);
			counter=0;
		}        
        else{
			printf("%d ",score); 
		}
        if(score>=0 && score<=59) /* Evulating numbers of how many people takes which grade */
        {
            counter_F++;
        }
        if(score>=60 && score<=69)
        {
            counter_D++;
        }
        if(score>=70 && score<=79)
        {
            counter_C++;
        }
        if(score>=80 && score<=89)
        {
            counter_B++;
        }
        if(score>=90 && score<=100)
        {
            counter_A++;
        }
        if(score>MAX_score)
        {
            MAX_score=score;
            MAX_index=index;
        }
        if(score<MIN_score)
        {
            MIN_score=score;
            MIN_index=index;
        }
    }
    avarage=avarage/count;       /* Calculating avarage of class */
    if(counter_A!=0)      /* Determining highest grade of class */
    {
        MAX_grade='A';
    }
    else if(counter_B!=0){
        MAX_grade='B';
    }
    else if(counter_C!=0){
        MAX_grade='C';
    }
    else if(counter_D!=0){
        MAX_grade='D';
    }
    else if(counter_F!=0){
        MAX_grade='F';
    }
    if(counter_F!=0)   /* Determining lowest grade of class */
    {
        MIN_grade='F';
    }
    else if(counter_D!=0){
        MIN_grade='D';
    }
    else if(counter_C!=0){
        MIN_grade='C';
    }
    else if(counter_B!=0){
        MIN_grade='B';
    }
    else if(counter_A!=0){
        MIN_grade='A';
    }

    while(flag2)  /* Crating menu of program */
    {
        printf("\nStudent Score Calculator Menu for %d Student\n1-)Most Successful Student\n2-)Most Unsuccessful Student\n"
                       "3-)Letter Grade Statistics\n4-)Calculate Avarage\n5-)Show All Data\n                  "
                       "        Make Selection:",count);
        scanf("%d",&select); /*User selects which process it want to pick */
        switch (select)
        {
            case 1:
                printf("Most Successfully Student:\nIndex:%d\nScore:%d\nLatter Grade:%c\n",MAX_index,MAX_score,MAX_grade);
                break;
            case 2:
                printf("Most Unsuccessfully Student:\nIndex:%d\nScore:%d\nLatter Grade:%c\n",MIN_index,MIN_score,MIN_grade);
                break;
            case 3:
                printf("%d Student got letter grade 'A'\n%d Student got letter grade 'B'\n%d Student got letter grade 'C'\n%d Student got letter grade 'D'\n"
                               "%d Student got letter grade 'F'\n ",counter_A,counter_B,counter_C,counter_D,counter_F);
                break;
            case 4:
                printf("The average Score of %d Student is %.2lf\n",count,avarage);
                break;
            case 5:
                printf("Most Successfully Student:\nIndex:%d\nScore:%d\nLatter Grade:%c\n\n",MAX_index,MAX_score,MAX_grade);
                printf("Most Unsuccessfully Student:\nIndex:%d\nScore:%d\nLatter Grade:%c\n\n",MIN_index,MIN_score,MIN_grade);
                printf("%d Student got letter grade'A'\n%d Student got letter grade'B'\n%d Student got letter grade'C'\n%d Student got letter grade'D'\n"
                               "%d Student got letter grade'F'\n\n",counter_A,counter_B,counter_C,counter_D,counter_F);
                printf("The avarege Score of %d Student is %.2lf\n",count,avarage);
                break;
            case -1:   /* If user enters value of -1 program ends*/
            	flag2=0;
            	break;
                	
            default:
                printf("False Selection!!!");
                break;
        }
        

    }

    return 0;


}

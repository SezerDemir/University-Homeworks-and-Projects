%{ 
#include<stdio.h> 
int counter = 1; 
%} 

%%
"-" {printf("OP_MINUS\n"); fprintf(yyout, "OP_MINUS\n");}
"/" {printf("OP_DIV\n"); fprintf(yyout, "OP_DIV\n");}
"+" {printf("OP_PLUS\n"); fprintf(yyout, "OP_PLUS\n");}
"*" {printf("OP_MULT\n"); fprintf(yyout, "OP_MULT\n");}
"(" {printf("OP_OP\n"); fprintf(yyout, "OP_OP\n");}
")" {printf("OP_CP\n"); fprintf(yyout, "OP_CP\n");} 
"**" {printf("OP_DBLMULT\n"); fprintf(yyout, "OP_DBLMULT\n");}
"," {printf("OP_COMMA\n"); fprintf(yyout, "OP_COMMA\n");}
"\"" {    
            if(counter == 1)
            {
                printf("OP_OC\n");
                fprintf(yyout, "OP_OC\n");
                counter = 0;
            }             
            else{
                printf("OP_CC\n");
                fprintf(yyout, "OP_CC\n");
                counter = 1;
            }  
        }
"and" {printf("KW_AND\n"); fprintf(yyout, "KW_AND\n");}
"or" {printf("KW_OR\n"); fprintf(yyout, "KW_OR\n");}
"not" {printf("KW_NOT\n"); fprintf(yyout, "KW_NOT\n");}
"equal" {printf("KW_EQUAL\n"); fprintf(yyout, "KW_EQUAL\n");} 
"less" {printf("KW_LESS\n"); fprintf(yyout, "KW_LESS\n");}
"nil" {printf("KW_NIL\n"); fprintf(yyout, "KW_NIL\n");}
"list" {printf("KW_LIST\n"); fprintf(yyout, "KW_LIST\n");}
"append" {printf("KW_APPEND\n"); fprintf(yyout, "KW_APPEND\n");}
"concat" {printf("KW_CONCAT\n"); fprintf(yyout, "KW_CONCAT\n");}
"set" {printf("KW_SET\n"); fprintf(yyout, "KW_SET\n");}
"deffun" {printf("KW_DEFFUN\n"); fprintf(yyout, "KW_DEFFUN\n");}
"for" {printf("KW_FOR\n"); fprintf(yyout, "KW_FOR\n");}
"if" {printf("KW_IF\n"); fprintf(yyout, "KW_IF\n");}
"exit" {printf("KW_EXIT\n"); fprintf(yyout, "KW_EXIT\n");}
"load" {printf("KW_LOAD\n"); fprintf(yyout, "KW_LOAD\n");}
"disp" {printf("KW_DISP\n"); fprintf(yyout, "KW_DISP\n");}
"true" {printf("KW_TRUE\n"); fprintf(yyout, "KW_TRUE\n");}
"false" {printf("KW_FALSE\n"); fprintf(yyout, "KW_FALSE\n");}
";;".* {printf ("COMMENT\n"); fprintf (yyout, "COMMENT\n");}
"\n\n" { return 0;}
"\n"|"\t"|"\r"|" " {}

"+"[^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);} //[a-zA-Z][a-zA-Z0-9]*[^a-zA-Z0-9"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext);} 
"-"[^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);}
"/"[^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);}
"*"[^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);}
","[^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);} 

[0][^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);}
[1-9]+[^0-9"" " "\n" "(" ")" ";;"][^"\t""\n" " " "(" ")" ";;"]+ {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);}
[a-zA-Z][a-zA-Z0-9]*[^a-zA-Z0-9"\t""\n" " " "(" ")" ";;"]+[^"\t""\n" " " "(" ")" ";;"]* {printf("3SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext);} 
[a-zA-Z][a-zA-Z0-9]* {printf("IDENTIFIER\n"); fprintf(yyout, "IDENTIFIER\n");}
[1-9][0-9]*|[0] {printf("VALUE\n"); fprintf(yyout, "VALUE\n");}


. {printf("SYNTAX_ERROR %s could not be tokonized\n", yytext); fprintf(yyout, "SYNTAX_ERROR %s could not be tokonized\n", yytext); }

%%

int yywrap(){}

int main(int argc, char const *argv[]){ 
    if(argc > 0)
    {
        yyout=fopen("parsed_cpp.txt","w");
        yyin = fopen(argv[1], "r");
        yylex();
    }  
    else
    {
        yyin = stdin;
        while(1){
            yyout=fopen("parsed_cpp.txt","w");
            yylex();
        }
    }

    return 0; 
} 
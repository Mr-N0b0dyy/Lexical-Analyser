#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int c1;
char id[20];
char integer[10];
char *string;
int strLen;
bool isKeyword = false;

int fpeek(FILE *f_in)
{
    int c;
    c = fgetc(f_in);
    ungetc(c, f_in);
    return c;
}

FILE *fPtr;
FILE *out;
char *operators[] = { "+", "-", "*", "/", "++", "--", ":=" }; // OPERATORLER TANIMLANIYOR

const char *keywords[18] = { // KEYWORDLER TANIMLANIYOR
        "break", "case", "char",
        "const", "continue", "do",
        "else", "enum", "float",
        "for", "goto", "if",
        "int", "long", "record",
        "return", "static", "while",
};

void empty(char *str) { // LİSTENİN HEPSİNİ NULLUYOR
    int i;
    for (i = 0; i < 10; i++) {
        str[i] = '\0';
    }
}

void isId(){

    int length = 0;

    if(isalpha(c1) != 0){ // C1 ALFEBETİK KARAKTERSE

        while (isalnum(c1) != 0 || c1 == '_'){ // HARFSE
            id[length++] = c1;
            if (length == 20){
                fprintf(out,"Identifier length cannot be larger than 20 characters.");
                exit(1);
            }
            c1 = fgetc(fPtr);
        }
        id[length] = '\0'; // en sonuna null kouyyor
        if (strlen(id)!=0){
            int i;
            for (i = 0; i < strlen(id); i++) {
                id[i] = (char)toupper(id[i]); // id upper yapıyor
            }

            for(i = 0; i<18; i++){
                if(!strcasecmp(id, keywords[i])){ // id ile keyword compare ediyor. keywordse keyword olarak aliyor degilse identifer oluyor
                    fprintf(out,"Keyword(%s)\n", id);
                    isKeyword=true;
                    break;
                }
            }
            if(!isKeyword) {
                fprintf(out,"Identifier(%s)\n", id);


            }

            empty(id);
            isKeyword = false;
        }
    }

}

void isInt(){

    int length = 0;
    while(isdigit(c1) != 0){ // integersa
        integer[length] = c1;
        length++;

        if(length > 10){
            fprintf(out,"Integer length cannot be larger then 10 characters.");
        }

        c1 = fgetc(fPtr);//bir sonraki karakteri okuyor
    }
    if(length != 0) {
        fprintf(out, "Integer(%s)\n", integer);
        empty(integer);
    }

}

bool isDoubleOperator(char firstOp, char c2){
    int i;

    char* temp = (char*) calloc(3, sizeof(char));
    temp[0] = firstOp;
    temp[1] = c2;
    temp[2] = '\0';

    for(i = 0; i < 7; i++) {
        if(strcasecmp(temp,operators[i]) == 0){
            fprintf(out, "Operator(%s)\n", operators[i]);
            fgetc(fPtr);
            free(temp);
            return 1;
        }
    }
    return 0;

}

bool isSingleOperator(char firstOp){
    int i;
    char* temp = (char*) calloc(2,sizeof(char));
    temp[0] = firstOp;
    temp[1] = '\0';

    for(i = 0; i < 7; i++) {
        if(strcasecmp(temp,operators[i]) == 0){
            fprintf(out, "Operator(%s)\n", operators[i]);
            free(temp);
            return 1;
        }
    }
    free(temp);
    return 0;
}

void isOp(){
    if(isDoubleOperator(c1, peek(fPtr)) == 0) {
        isSingleOperator(c1);
    }
}

void isComment() {
    fgetc(fPtr);
    while ((c1 = fgetc(fPtr)) != EOF) {
        if (c1 == '*' && peek(fPtr) == ')') {
            fgetc(fPtr);
            break;
        }
    }
    if(c1 == EOF){
        fprintf(out, "Comment not closed\n");
    }

}

void isBracket(){

    if(c1 == '('){
        if(peek(fPtr) == '*'){
            fgetc(fPtr);
            isComment();
        }else fprintf(out, "Left Parenthesis\n");

    }else if(c1 == ')'){
        fprintf(out, "Right Parenthesis\n");
    }else if(c1 == '{'){
        fprintf(out, "Left Curly Bracket\n");
    }else if(c1 == '}'){
        fprintf(out, "Right Curly Bracket\n");
    }else if(c1 == '['){
        fprintf(out, "Right Square Bracket\n");
    }else if(c1 == ']'){
        fprintf(out, "Right Square Bracket\n");
    }

}

void isString() {
    if (c1 == '"') {
        string = (char*) calloc(20,20);
        strLen = 0;
        while ((c1 = fgetc(fPtr)) != EOF) {
            string[strLen++] = c1;
            if (peek(fPtr) == '"') {
                fgetc(fPtr);
                string[strLen] = '\0';
                break;
            }
        }

        if (c1 == EOF) {
            fprintf(out, "End Of File before string closed\n");
        }
        fprintf(out, "String(%s)\n", string);
        free(string);
    }
}

void isEOL(){ //end of line
    if(c1 == ';'){
        fprintf(out,"End Of Line\n");
    }
}



int main() {

    if ((fPtr = fopen("code.psi", "r")) == NULL) {
        printf("Error trying to open file\n");
    }

    out = fopen("code.lex", "w");

    do{
    c1 = fgetc(fPtr);

    isId();
    isInt();
    isOp();
    isBracket(); // isComment() is inside it
    isString();
    isEOL();

    }while(c1 != EOF);

    return 0;
}

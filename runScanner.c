//Andrew Bertrand
//10/8/10
//Lexigraphical Analyzer
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "runScanner.h"


//this is an enum of all the different symbols in the grammar,
//as given to us in the assignment sheet
typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, outsym,
insym , elsesym } token_type;

//the "token" struct will be the nodes in a linked list
//of all the tokens in the program
struct token
{
    int type;
    char identifier[12];
    struct token* next;
};
int hasError;
int main(void)
{
    //this section gets the filename and opens it
    char filename[100];
    
    printf("What is the name of the file?\n");
    scanf("%s", filename);
    FILE* fin = fopen(filename, "r");
    
    //tempChar can get individual characters from the file
    //and amtChar will keep track of how many characters are in the file
    char tempChar;
    int amtChar = 0;
    //this goes through the file and counts the number of characters
    //and prints them to the screen
    while(!feof(fin))
    {
        tempChar = fgetc(fin);
        amtChar++;
        
    }
    fclose(fin);
    
    
    //program will store all the characters in the file
    char program[amtChar];
    //reopens the file to get all the characters
    fin = fopen(filename, "r");
    int i;
    for(i = 0; i < amtChar; i++)
        program[i] = fgetc(fin);
        
    fclose(fin);
    //curChar stores the character we're looking at,
    //charCtr tracks the index of the character we're looking at,
    //tokenList is the head of a linked list of tokens
    //curToken is the current token we're adding to the list
    //tempType will store what the type is of the unit we're looking at
    //tempIdentifier will store the characters of the unit we're looking at
    char curChar;
    int charCtr = 0;
    struct token* tokenList = malloc(sizeof(struct token));
    struct token* curToken = tokenList;
    int tempType = 0;
    char tempIdentifier[11];
    //********************************
    FILE* fout = fopen("scannererror.txt", "w");
    //********************************
    
    
    while(charCtr < amtChar)
    {
        curChar = program[charCtr];
        //this statement is if the current item is a letter and could
        //be a keyword or identifier
        if(isalpha(curChar))
        {
            
            tempIdentifier[0] = curChar;
            charCtr++;
            int charNum = 1;
            curChar = program[charCtr];
            //this loop will get all the letters in the string
            while(isalnum(curChar))
            {
                //informs if the identifier is too long and stops
                //adding to the string, which would be full
                if(charNum == 11)
                {
                    fprintf(fout, "Identifier is too long.\n");
                    hasError = 1;
                    tempType = 0;
                }
                else if(charNum < 12)
                {
                    tempIdentifier[charNum] = curChar;
                    
                }
                charCtr++;
                curChar = program[charCtr];
                charNum++;
            }
            //adds the terminator to the end if the identifier was too long
            if(charNum > 12)
                tempIdentifier[11] = '\0';
            //adds it where it belongs otherwise
            else
                tempIdentifier[charNum] = '\0';
            //these if statements all check to see if the string
            //is any of the keywords
            if(strcmp(tempIdentifier, "call") == 0)
                tempType = callsym;
            else if(strcmp(tempIdentifier, "begin") == 0)
                tempType = beginsym;
            else if(strcmp(tempIdentifier, "end") == 0)
                tempType = endsym;
            else if(strcmp(tempIdentifier, "if") == 0)
                tempType = ifsym;
            else if(strcmp(tempIdentifier, "then") == 0)
                tempType = thensym;
            else if(strcmp(tempIdentifier, "else") == 0)
                tempType = elsesym;
            else if(strcmp(tempIdentifier, "in") == 0)
                tempType = insym;
            else if(strcmp(tempIdentifier, "while") == 0)
                tempType = whilesym;
            else if(strcmp(tempIdentifier, "do") == 0)
                tempType = dosym;
            else if(strcmp(tempIdentifier, "out") == 0)
                tempType = outsym;
            else if(strcmp(tempIdentifier, "var") == 0)
                tempType = varsym;
            else if(strcmp(tempIdentifier, "procedure") == 0)
                tempType = procsym;
            else if(strcmp(tempIdentifier, "const") == 0)
                tempType = constsym;
            else if(strcmp(tempIdentifier, "odd") == 0)
                tempType = oddsym;
            else if(strcmp(tempIdentifier, "null") == 0)
                tempType =  nulsym;
            else
            {
                //if the string was too long then this won't be a new token
                if(charNum >= 12)
                    tempType = 0;
                //if it isn't too long and isn't a keyword then it's an identifier
                else
                    tempType = identsym;
              
            }

            charCtr--;
        }
        //this if statement is for a number
        else if(isdigit(curChar))
        {
            int charNum = 0;
            //loops through and gets all the digits
            while(isalnum(curChar))
            {
                tempIdentifier[charNum] = curChar;
                //if it finds a character then this must be an
                //identifier that starts with a number
                if(isalpha(curChar))
                {
                    fprintf(fout, "Variable does not start with a letter.\n");
                    hasError = 1;
                    break;
                }
                //this is if it's too long
                if(charNum == 5)
                {
                    fprintf(fout, "Number is too long.\n");
                    hasError = 1;
                    break;
                }
                charNum++;
                charCtr++;
                curChar = program[charCtr];
            }
            //if the curChar is alnum then either it was an identifier
            //or a number that was too long, so we need to get to the end
            //of the string
            if(isalnum(curChar))
            {
                while(isalnum(curChar))
                {
                    charCtr++;
                    curChar = program[charCtr];
                }
                charCtr--;
                
            }
            //otherwise, it was just a number
            else
            {
                charCtr--;
                tempType = numbersym;
                tempIdentifier[charNum] = '\0';
            }
        }
        //this is to go through all the space characters
        else if(isspace(curChar))
        {
            charCtr++;
            curChar = program[charCtr];
            while(isspace(curChar))
            {
                charCtr++;
                curChar = program[charCtr];
            }
            charCtr--;
        }
        //this is for all the symbols
        else if(ispunct(curChar))
        {
            //this will need to check if it's just a slash or a comment
            if(curChar == '/')
            {
                charCtr++;
                curChar = program[charCtr];
                //if the next character isn't '*' then it's a slash
                if(curChar != '*')
                {
                    
                    strcpy(tempIdentifier, "/");
                    tempType = slashsym;
                    charCtr--;
                }
                //otherwise, it's a comment
                else
                {
                    //this loop continues until it finds an asterisk, then
                    //checks to see if the next character is a slash. if so,
                    //end of comment
                    while(1)
                    {
                        charCtr++;
                        curChar = program[charCtr];
                        if(curChar == '*')
                        {
                            charCtr++;
                            curChar = program[charCtr];
                            if(curChar == '/')
                                break;
                            else
                                charCtr--;
                        }
                    }
                }
            }
            //checks to see if this is '<' or '<='
            else if(curChar == '<')
            {
                
                charCtr++;
                curChar = program[charCtr];
                
                if(curChar == '=')
                {
                    strcpy(tempIdentifier, "<=");
                    tempType = leqsym;
                }
                else
                {
                    strcpy(tempIdentifier, "<");
                    tempType = lessym;
                    charCtr--;
                }
            }
            //checks to see if the symbol is '>' or '>='
            else if(curChar == '>')
            {
                
                charCtr++;
                curChar = program[charCtr];
                
                if(curChar == '=')
                {
                    strcpy(tempIdentifier, ">=");
                    tempType = geqsym;
                }
                else
                {
                    strcpy(tempIdentifier, ">");
                    tempType = gtrsym;
                    charCtr--;
                }
            }
            //if the ':' is followed by '=' the it's the becomes symbol
            //otherwise it's invalid
            else if(curChar == ':')
            {
                
                charCtr++;
                curChar = program[charCtr];
                
                if(curChar == '=')
                {
                    strcpy(tempIdentifier, ":=");
                    tempType = becomessym;
                }
                else
                {
                    charCtr--;
                    fprintf(fout, "Invalid symbol.\n");
                    hasError = 1;
                }
            }
            //the next few just get the symbol, straightforward
            else if(curChar == '+')
            {
                
                strcpy(tempIdentifier, "+");
                tempType = plussym;
                
            }
            else if(curChar == '-')
            {
                
                strcpy(tempIdentifier, "-");
                tempType = minussym;
            }
            else if(curChar == '(')
            {
                
                strcpy(tempIdentifier, "(");
                tempType = lparentsym;
            }
            else if(curChar == ')')
            {
                
                strcpy(tempIdentifier, ")");
                tempType = rparentsym;
            }
            else if(curChar == ';')
            {
               
                strcpy(tempIdentifier, ";");
                tempType = semicolonsym;
            }
            else if(curChar == '=')
            {
                strcpy(tempIdentifier, "=");
                tempType = eqsym;
               
            }
            else if(curChar == '.')
            {
                strcpy(tempIdentifier, ".");
                tempType = periodsym;
            }
            //if the '!' is followed by an '=' then it's the not equal
            //symbol, otherwise it's invalid
            else if(curChar == '!')
            {
                charCtr++;
                curChar = program[charCtr];
                if(curChar == '=')
                {
                    strcpy(tempIdentifier, "!=");
                    tempType = neqsym;
                }
                else
                {
                    charCtr--;
                    fprintf(fout, "Invalid symbol.\n");
                    hasError = 1;
                }
            }
            else if(curChar == '*')
            {
                strcpy(tempIdentifier, "*");
                tempType = multsym;
            }
            else if(curChar == ',')
            {
                strcpy(tempIdentifier, ",");
                tempType = commasym;
            }
            //if the symbol isn't any of these, it's invalid
            else
            {
                fprintf(fout, "Invalid symbol.\n");
                hasError = 1;
            }
        }
        
        charCtr++;
        //if there is a new token, which is when the tempType isn't 0, this
        //adds it to the list and creates a new link, and prints out the
        //information properly
        if(tempType != 0)
        {
            
            curToken->type = tempType;
            strcpy(curToken->identifier, tempIdentifier);
            curToken->next = malloc(sizeof(struct token));
            curToken = curToken->next;
            curToken->next = NULL;
            tempType = 0;
        }
        
    }
    curToken = tokenList;
    fclose(fout);
    //prints out the token types in the linked list, and the identifier
    //if the token is an identifier
    fout = fopen("scanneroutput.txt", "w");
    while(curToken->next != NULL)
    {
        fprintf(fout, "%d ", curToken->type);
        if(curToken->type == identsym)
            fprintf(fout, "%s ", curToken->identifier);
        else if(curToken->type == numbersym)
            fprintf(fout, "%s ", curToken->identifier);
        curToken = curToken->next;
    }
    
    fclose(fout);
    return hasError;
    
    
        
}

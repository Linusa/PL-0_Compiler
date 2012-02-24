#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "runParser.h"


#define MAX_SYMBOL_TABLE_SIZE 5000

typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, outsym,
insym , elsesym } Symbol;

struct Types
{
    int kind;   //const = 1, var = 2, proc = 3
    char name[10];
    int val;
    int level;
    int address;
};

struct token
{
    int type;
    char identifier[12];
    int number;
    struct token* next;
};

struct code
{
    int line;
    int op;
    int m;
    int l;
    struct code* next;
};

struct Types symbollist[MAX_SYMBOL_TABLE_SIZE];
Symbol sym;
FILE* fin, *fout;

void error(int x);
void statement(void);
void getsym(void);
void createCode(int op, int l, int m);
void expression(void);
void program(void);
int lexLevel, tempSym, tempVal;
struct token *token, *tempToken, *tokenList;
int line, amtSym;
struct code *codelist, *curLine, *tempLine;
void freeTokens(struct token *x);
void freeCodes(struct code *x);
int findIdentifier(char identifier[]);


int main(void)
{
    line = 0;
    codelist = malloc(sizeof(struct code));
    curLine = codelist;
    lexLevel = 0;
    tokenList = malloc(sizeof(struct token));
    token = tokenList;
    
    fin = fopen("scanneroutput.txt", "r");
    
    while(!feof(fin))
    {
        
        fscanf(fin, "%d ", &token->type);
        
        
        if(token->type == identsym)
            fscanf(fin, "%s ", token->identifier);
        else if(token->type == numbersym)
            fscanf(fin, "%d ", &token->number);
            
        token->next = malloc(sizeof(struct token));
        token = token->next;
        token->type = 0;
        
    }
    token = tokenList;
    fclose(fin);
    fout = fopen("tokenoutput.txt", "w");
    token = tokenList;
    lexLevel = 0;
    while(token->next != NULL)
    {
        fprintf(fout, "%d ", token->type);
        if(token->type == numbersym)
            fprintf(fout, "%d ", token->number);
        else if(token->type == identsym)
        {
            int location = findIdentifier(token->identifier);
            fprintf(fout, "%d ", location+1);
        }
        token = token->next; 
    }
    fprintf(fout, "\n");
    token = tokenList;
    while(token->next != NULL)
    {
        if(token->type == nulsym)
            fprintf(fout, "nulsym");
        else if(token->type == identsym)
        {
            fprintf(fout, "identsym");
            int location = findIdentifier(token->identifier);
            fprintf(fout, ".%d", location+1);
        }
        else if(token->type == numbersym)
            fprintf(fout, "numbersym.%d", token->number);
        else if(token->type == plussym)
            fprintf(fout, "plussym");
        else if(token->type == multsym)
            fprintf(fout, "multsym");
        else if(token->type == slashsym)
            fprintf(fout, "slashsym");
        else if(token->type == oddsym)
            fprintf(fout, "oddsym");
        else if(token->type == eqsym)
            fprintf(fout, "eqlsym");
        else if(token->type == neqsym)
            fprintf(fout, "neqsym");
        else if(token->type == lessym)
            fprintf(fout, "lessym");
        else if(token->type == leqsym)
            fprintf(fout, "leqsym");
        else if(token->type == gtrsym)
            fprintf(fout, "gtrsym");
        else if(token->type == geqsym)
            fprintf(fout, "geqsym");
        else if(token->type == lparentsym)
            fprintf(fout, "lparentsym");
        else if(token->type == rparentsym)
            fprintf(fout, "rparentsym");
        else if(token->type == commasym)
            fprintf(fout, "commasym");
        else if(token->type == semicolonsym)
            fprintf(fout, "semicolonsym");
        else if(token->type == periodsym)
            fprintf(fout, "periodsym");
        else if(token->type == becomessym)
            fprintf(fout, "becomessym");
        else if(token->type == beginsym)
        {
            fprintf(fout, "beginsym");
            lexLevel++;
        }
        else if(token->type == endsym)
        {
            fprintf(fout, "endsym");
            lexLevel--;
        }
        else if(token->type == ifsym)
            fprintf(fout, "ifsym");
        else if(token->type == thensym)
            fprintf(fout, "thensym");
        else if(token->type == whilesym)
            fprintf(fout, "whilesym");
        else if(token->type == dosym)
            fprintf(fout, "dosym");
        else if(token->type == callsym)
            fprintf(fout, "callsym");
        else if(token->type == constsym)
            fprintf(fout, "constsym");
        else if(token->type == varsym)
            fprintf(fout, "varsym");
        else if(token->type == procsym)
            fprintf(fout, "procsym");
        else if(token->type == outsym)
            fprintf(fout, "outsym");
        else if(token->type == insym)
            fprintf(fout, "insym");
        else if(token->type == elsesym)
            fprintf(fout, "elsesym");
        else if(token->type == identsym)
            fprintf(fout, "identsym");
        fprintf(fout, " ");
        token = token->next;
        
    }
    fclose(fout);
    fout = fopen("parseroutput.txt", "w");
    
    program();
    curLine = codelist;
    while(curLine->next != NULL)
    {
        fprintf(fout, "%d %d %d\n", curLine->op, curLine->l, curLine->m);
        curLine = curLine->next;
    }
    fclose(fout);
    
    return 0;
}

void getsym(void)
{
    
       
    token = token->next;
        
    sym = token->type;
    
    
}
 
int accept(Symbol s) {
    if (sym == s) {
        
        getsym();
        return 1;
    }
    
    
    return 0;
}

void error(int x)
{
    fprintf(fout, "Error number %d: ", x);
    
    if(x == 1)
        fprintf(fout, "Use '=' instead of ':='");
    else if(x == 2)
        fprintf(fout, "'=' must be followed by a number.");
    else if(x == 3)
        fprintf(fout, "Indentifer must be followed by '='.");
    else if(x == 4)
        fprintf(fout, "Semicolon or comma missing.");
    else if(x == 5)
        fprintf(fout, "Semicolon between statements missing.");
    else if(x == 6)
        fprintf(fout, "'const' must be followed by an identifier.");
    else if(x == 7)
        fprintf(fout, "'var' must be followed by an identifier.");
    else if(x == 8)
        fprintf(fout, "'procedure' must be followed by an identifier.");
    else if(x == 9)
        fprintf(fout, "Period expected.");
    else if(x == 10)
        fprintf(fout, "Call must be followed by an identifier.");
    else if(x == 11)
        fprintf(fout, "Undeclared identifier.");
    else if(x == 12)
        fprintf(fout, "Assignment to constant or procedure is not allowed.");
    else if(x == 13)
        fprintf(fout, "Assignment operator expected.");
    else if(x == 14)
        fprintf(fout, "Call of a constant or variable is meaningless.");
    else if(x == 15)
        fprintf(fout, "'then' expected.");
    else if(x == 16)
        fprintf(fout, "'do' expected.");
    else if(x == 17)
        fprintf(fout, "Relational operator expected.");
    else if(x == 18)
        fprintf(fout, "Expression must not contain a procedure identifier.");
    else if(x == 19)
        fprintf(fout, "Right parenthesis missing.");
    else if(x == 20)
        fprintf(fout, "Factor cannot begin with this symbol.");
    else if(x == 21)
        fprintf(fout, "An unknown error occurred, reached end of code while parsing.");
    
    fprintf(fout, "\n");
    exit(1);
}

void createCode(int op, int l, int m)
{
    
    curLine->op = op;
    curLine->l = l;
    curLine->m = m;
    curLine->line = line;
    
    curLine->next = malloc(sizeof(struct code));
    curLine = curLine->next;
    curLine->op = -1;
    line++;
}

//finds the identifier in the symbol table and returns the location, or a default
//number if it's undeclared
int findIdentifier(char identifier[])
{
    int x;
    
    for(x = amtSym - 1; x >= 0; x--)
    {
        
        if(symbollist[x].level <= lexLevel && strcmp(symbollist[x].name, identifier) == 0)
            return x;
    }
    
    return MAX_SYMBOL_TABLE_SIZE + 1;
}
 
int expect(Symbol s) {
    
    if (accept(s))
        return 1;
    if(sym == eqsym && s == becomessym)
        error(1);
    else if(s == numbersym)
        error(2);
    else if(s == eqsym)
        error(3);
    else if(s == semicolonsym)
        error(4);
    else if(s == becomessym)
        error(13);
    else if(s == rparentsym)
        error(19);
    
    
    
    
    return 0;
}
 
void factor(void) {
    tempToken = token;
    if (accept(identsym)) 
    {
        int location = findIdentifier(tempToken->identifier);
        
        if(location == MAX_SYMBOL_TABLE_SIZE + 1)
            error(11);
        if(symbollist[location].kind == 3)
            error(18);
        if(symbollist[location].kind == 1)
            createCode(1, 0, symbollist[location].val);
        else
            createCode(3, lexLevel - symbollist[location].level, symbollist[location].address);
    } 
    else if (accept(numbersym)) 
    {
        createCode(1, 0, tempToken->number);
    } 
    else if (accept(lparentsym)) 
    {
        expression();
        expect(rparentsym);
    } 
    else 
    {
        error(20);
        getsym();
    }
}
 
void term(void) {
    factor();
    while (sym == multsym || sym == slashsym) 
    {
        int tempsym = sym;
        getsym();
        factor();
        if(tempsym == multsym)
            createCode(2, 0, 4);
        else if(tempsym == slashsym)
            createCode(2, 0, 5);
    }
}
 
void expression(void) {
    
    int negate = 0;
    if (sym == plussym || sym == minussym)
    {
        if(sym == minussym)
            negate = 1;
        getsym();
        
    }
    term();
    if(negate == 1)
        createCode(2, 0, 1);
    while (sym == plussym || sym == minussym) 
    {
        int tempsym = sym;
        getsym();
        term();
        if(tempsym == plussym)
            createCode(2, 0, 2);
        else if(tempsym == minussym)
            createCode(2, 0, 3);
    }
}
 
void condition(void) {
    if (accept(oddsym)) 
    {
        expression();
        createCode(2, 0, 6);
    } 
    else 
    {
        expression();
        if (sym == eqsym || sym == neqsym || sym == lessym || sym == leqsym || sym == gtrsym || sym == geqsym) 
        {
            int tempsym = sym;
            getsym();
            expression();
            if(tempsym == eqsym)
                createCode(2, 0, 8);
            else if(tempsym == neqsym)
                createCode(2, 0, 9);
            else if(tempsym == lessym)
                createCode(2, 0, 10);
            else if(tempsym == leqsym)
                createCode(2, 0, 11);
            else if(tempsym == gtrsym)
                createCode(2, 0, 12);
            else if(tempsym == geqsym)
                createCode(2, 0, 13);
        } 
        else 
        {
            error(17);
            getsym();
        }
    }
}
 
void statement(void) {
    tempToken = token;
    
    if (accept(identsym)) 
    {
        int location = findIdentifier(tempToken->identifier);
        
        if(location == MAX_SYMBOL_TABLE_SIZE + 1)
            error(11);
        expect(becomessym);
        if(symbollist[location].kind != 2)
            error(12);
        expression();
        createCode(4, lexLevel - symbollist[location].level, symbollist[location].address);
        
    } 
    else if (accept(callsym)) 
    {
        tempToken = token;
        if(!expect(identsym))
            error(10);
        
        int location = findIdentifier(tempToken->identifier);
        if(location == MAX_SYMBOL_TABLE_SIZE + 1)
            error(11);
        else if(symbollist[location].kind != 3)
            error(14);
        //createCode(6, 0, 3);
        createCode(5, symbollist[location].level, symbollist[location].address);
        //createCode(7, 0, symbollist[location].address);
        
    } 
    else if (accept(beginsym)) 
    {
        
        
        do 
        {
            
            statement();
            
        } while (accept(semicolonsym));
        
        if(!expect(endsym))
            error(5);
        
            
    } 
    else if (accept(ifsym)) 
    {
        
        condition();
        tempLine = curLine;
        createCode(8, 0, 0);
        
        if(!expect(thensym))
            error(15);
        statement();
        tempLine->m = line;
        
    } 
    else if (accept(whilesym)) 
    {
        int conLine = line;
        condition();
        tempLine = curLine;
        createCode(8, 0, 0);
        
        if(!expect(dosym))
            error(16);
        statement();
        
        createCode(7, 0, conLine);
        tempLine->m = line;
    }
    
}
 
void block(void) {
    int procStart = line;
    createCode(7, 0, 0);
    
    if (accept(constsym)) 
    {
        do {
            
            
            symbollist[amtSym].kind = 1;
            tempToken = token;
            if(!expect(identsym))
                error(6);
            strcpy(symbollist[amtSym].name, tempToken->identifier);
            expect(eqsym);
            tempToken = token;
            expect(numbersym);
            symbollist[amtSym].val = tempToken->number;
            amtSym++;
        } while (accept(commasym));
        expect(semicolonsym);
        
    }
    int varNum;
    if (accept(varsym)) {
        varNum = 3;
        do {
            symbollist[amtSym].kind = 2;
            tempToken = token;
            if(!expect(identsym))
                error(7);
            
            strcpy(symbollist[amtSym].name, tempToken->identifier);
            symbollist[amtSym].level = lexLevel;
            symbollist[amtSym].address = varNum;
            varNum++;
            amtSym++;
        } while (accept(commasym));
        expect(semicolonsym);
        
    }
    while (accept(procsym)) 
    {
        int procLine = line, location = amtSym;
        symbollist[location].kind = 3;
        tempToken = token;
        if(!expect(identsym))
            error(8);
        strcpy(symbollist[location].name, tempToken->identifier);
        symbollist[location].level = lexLevel;
        amtSym++;
        expect(semicolonsym);
        lexLevel++;
        
        block();
        symbollist[location].address = procLine;
        expect(semicolonsym);
        
        lexLevel--;
        createCode(2, 0, 0);
    }
    
    tempLine = codelist;
    int x;
    for(x = 0; x < procStart; x++)
        tempLine = tempLine->next;
    tempLine->m = line;
    
    createCode(6, 0, varNum);
    statement();
}
 
void program(void) {
    sym = token->type;
    //createCode(6, 0, 3);
    block();
    
    if(!expect(periodsym))
        error(9);
        createCode(2, 0, 0);
}


/*void freeTokens(struct token *x)
{
    if(x->next != NULL)
        freeTokens(x->next);
    free(x);
}
void freeCodes(struct code *x)
{
    if(x->next != NULL)
        freeTokens(x->next);
    free(x);
}*/

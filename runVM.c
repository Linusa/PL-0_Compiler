//Andrew Bertrand
//COP 3402
//Homework 1
//9/17/10

#include <stdio.h>
#include <string.h>
//#include "runVM.h"


#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

//this is a struct for the information for each line of code
struct instruction{
    int op, l, m;
};

int base(int l, int base, int* stack);
void printstack(int start, int end, int* stack);

FILE* fout;
int main()
{
    //initialize all the variables
    //"pc" is the program counter, "bp" is the base pointer,
    //and "sp" is the stack pointer
    int pc = 0, bp = 1, sp = 0;
    //the stack of the numbers
    int stack[MAX_STACK_HEIGHT];
    //this array holds all the lines of code
    struct instruction program[MAX_CODE_LENGTH];
    int i;
    //this struct is the ir
    struct instruction ir;
    //initializes the stack to zero
    for(i = 0; i < MAX_STACK_HEIGHT; i++)
        stack[i] = 0;
    i = 0;
    //gets the name of the input file
    //***************printf("What is the name of the input file?\n");
    //********************scanf("%s", filename);
    FILE* fin = fopen("parseroutput.txt", "r");
    fout = fopen("vmoutput.txt", "w");
    //scans in all the lines of code
    while(!feof(fin))
    {
        fscanf(fin, "%d %d %d ", &program[i].op, &program[i].l, &program[i].m);
        i++;
    }
  
    fclose(fin);
    //these lines and this loop prints out the code
    fprintf(fout, "Line\tOP\tL\tM\n");
    int x;
    for(x = 0; x < i; x++)
    {
        if(program[x].op == 1)
            fprintf(fout, "%d\tlit\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 2)
            fprintf(fout, "%d\topr\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 3)
            fprintf(fout, "%d\tlod\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 4)
            fprintf(fout, "%d\tsto\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 5)
            fprintf(fout, "%d\tcal\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 6)
            fprintf(fout, "%d\tinc\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 7)
            fprintf(fout, "%d\tjmp\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 8)
            fprintf(fout, "%d\tjpc\t%d\t%d\n", x, program[x].l, program[x].m);
            
        else if(program[x].op == 9)
            fprintf(fout, "%d\tsoi\t%d\t%d\n", x, program[x].l, program[x].m);
        
        else if(program[x].op == 10)
            fprintf(fout, "%d\tsio\t%d\t%d\n", x, program[x].l, program[x].m);
            
    }       
    //starts printing out the program
    fprintf(fout, "\n\t\t\t\t\tpc\tbp\tsp\tstack\nInitial Values\t\t\t\t%d\t%d\t%d\t0\n", pc, bp, sp);
    //this loops runs the program
    while(sp != -1)
    {
        //this is the fetch cycle, which puts the next line in the ir
        //and increments the program counter
        ir.op = program[pc].op;
        ir.m = program[pc].m;
        ir.l = program[pc].l;
        pc++;
        //this adds a literal to the stack
        if(ir.op == 1)
        {
            fprintf(fout, "%d\tlit\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            sp++;
            stack[sp] = ir.m;
        }    
        //this performs a variety of operations
        else if(ir.op == 2)
        {
            fprintf(fout, "%d\topr\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            //this is the return function
            if(ir.m == 0)
            {
                sp = bp - 1;
                pc = stack[sp + 3];
                bp = stack[sp + 2];
                if(sp == 0)
                    break;
            }
            //this returns the negative of the top of the stack
            else if(ir.m == 1)
            {
                stack[sp] *= -1; 
            }
            //this adds the top two elements of the stack
            else if(ir.m == 2)
            {
                sp--;
                stack[sp] += stack[sp + 1];
            }
            //this subtracts the top two numbers of the stack
            else if(ir.m == 3)
            {
                sp--;
                stack[sp] -= stack[sp + 1];
            }
            //this multiplies the top two numbers of the stack
            else if(ir.m == 4)
            {
                sp--;
                stack[sp] *= stack[sp + 1];
            }
            //this divides the top two numbers of the stack
            else if(ir.m == 5)
            {
                sp--;
                stack[sp] /= stack[sp + 1]; 
            }
            //this returns whether or not the top of the stack is even
            else if(ir.m == 6)
            {
                stack[sp] %= 2;
            }
            //this mods the top two numbers of the stack
            else if(ir.m == 7)
            {
                sp--;
                stack[sp] %= stack[sp + 1];
            }
            //this returns if the top two numbers of the stack are equal
            else if(ir.m == 8)
            {
                sp--;
                if(stack[sp] == stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0;
            }
            //this returns if the top two numbers of the stack aren't equal
            else if(ir.m == 9)
            {
                sp--;
                if(stack[sp] != stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0;
            }
            //this returns if the second is less than the first
            else if(ir.m == 10)
            {
                sp--;
                if(stack[sp] < stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0; 
            }
            //this returns if the second is less than or equal to the first
            else if(ir.m == 11)
            {
                sp--;
                if(stack[sp] <= stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0;
            }
            //this returns if the second is greater than the first
            else if(ir.m == 12)
            {
                sp--;
                if(stack[sp] > stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0;
            }
            //this returns if the second is greater than or equal to the first
            else if(ir.m == 13)
            {
                sp--;
                if(stack[sp] >= stack[sp + 1])
                    stack[sp] = 1;
                else
                    stack[sp] = 0;   
            }
               
        }
        //loads a number to the top of the stack
        else if(ir.op == 3)
        {
            fprintf(fout, "%d\tlod\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            sp++;
            stack[sp] = stack[base(ir.l, bp, stack) + ir.m];   
        }
        //stores a number somewhere in the stack
        else if(ir.op == 4)
        {
            fprintf(fout, "%d\tsto\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            stack[base(ir.l, bp, stack) + ir.m] = stack[sp];
            sp--;   
        }
        //calls a new function
        else if(ir.op == 5)
        {
            
            fprintf(fout, "%d\tcal\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            stack[sp + 1] = base(ir.l, bp, stack);
            stack[sp + 2] = bp;
            stack[sp + 3] = pc;
            bp = sp + 1;
            pc = ir.m;
        }
        //makes more room on the stack
        else if(ir.op == 6)
        {
            fprintf(fout, "%d\tinc\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            sp += ir.m;
        }
        //jumps to a line in the code
        else if(ir.op == 7)
        {
            fprintf(fout, "%d\tjmp\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            pc = ir.m;
        }
        //jumps to a line in the code if the top of the stack is zero
        else if(ir.op == 8)
        {
            fprintf(fout, "%d\tjpc\t%d\t%d\t\t", pc-1, ir.m, ir.l);
            if(stack[sp] == 0)
            {
                pc = ir.m;
                sp--;
            }
        }
        //prints the top of the stack
        else if(ir.op == 9)
        {
            sp--;
            fprintf(fout, "%d\tsoi\t%d\t%d\t\t\n", pc-1, ir.m, ir.l);
            fprintf(fout, "Output: %d", stack[sp+1]);
            
        }
        //scans in a number
        else if(ir.op == 10)
        {
            sp++;
            printf("Please input a number");
            scanf("%d", &stack[sp]);
            fprintf(fout, "%d\tsio\t%d\t%d\t\t", pc-1, ir.m, ir.l);
        }
        //prints the information
        fprintf(fout, "%d\t%d\t%d\t", pc, bp, sp);
        //just prints a zero if the stack is empty
        if(sp == 0)
            fprintf(fout, "0");
        //prints the stack
        printstack(bp, sp, stack);
        fprintf(fout, "\n");
    }
    
    fclose(fout);
    
    return 0;     
}
//finds the appropriate level "l" in the stack
int base(int l, int base, int* stack)
{
    int b1 = base;
    //loops back using the dynamic links
    while(l > 0)
    {
        b1 = stack[b1];
        l--;
    }
    return b1;
}

void printstack(int start, int end, int* stack)
{
    //goes back recursively until it is at the first AR
    if(start != 1)
    {
        printstack(stack[start + 1], start - 1, stack);
        //only prints a "|" if the current AR isn't empty
        if(start < end)
            fprintf(fout, "| ");
    }
    int i;
    //prints the stack for the current AR
    for(i = start; i <= end; i++)
        fprintf(fout, "%d ", stack[i]);
}    

            
            
            
    
    
    

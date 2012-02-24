#include <stdio.h>
#include "runParser.h"
#include "runScanner.h"
#include "runVM.h"
int main(void)
{	
    int correct = runScanner();
    FILE* fin;
    FILE* fout = fopen("programoutput.txt", "w");
    char tempChar;
    if(correct == 0) 
    {
        correct = runParser();
        
        
        
        if (correct == 0) 
        {
            fin = fopen("tokenoutput.txt", "r");
            
    		while((tempChar = fgetc(fin)) != EOF)
    		{
    			fprintf(fout, "%c", tempChar);
    		}
    		fclose(fin);
            fprintf(fout, "\n\n");
            fin = fopen("parseroutput.txt", "r");
    		while((tempChar = fgetc(fin)) != EOF)
    		{
    			fprintf(fout, "%c", tempChar);
    		}
    		fclose(fin);
    		fprintf(fout, "\n\nThe code contains no syntax errors.\n\n");
			runVM();
			
			fin = fopen("vmoutput.txt", "r");
			while((tempChar = fgetc(fin)) != EOF)
			{
				fprintf(fout, "%c", tempChar);
			}
			fclose(fin);
			fprintf(fout, "\n");
		}
		else
		{
            fin = fopen("parseroutput.txt", "r");
    		while((tempChar = fgetc(fin)) != EOF)
    		{
    			fprintf(fout, "%c", tempChar);
    		}
    		fclose(fin);
    		fprintf(fout, "\n");
        }
	}
	else
	{
        fin = fopen("scannererror.txt", "r");
		while((tempChar = fgetc(fin)) != EOF)
		{
			fprintf(fout, "%c", tempChar);
		}
		fclose(fin);
		fprintf(fout, "\n");
    }
	
    fclose(fout);
    return 0;
}

/*
Lucas Kaplan
ENEE140
0101
12/3/19

Project 4

This project will emulate the usage of python methods on a list (an array of strings) given by the user's input file. The user will choose which method to be used through a menu displayed by the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 256

int i = 0, j = 0; //counters

/*===================================================
Functions
===================================================*/

//Takes in the list and an index within the list and returns the number of times substring occurs within that line
int count(char stringList[][MAX_LINE],int index,char substring[])
{	int subLength = strlen(substring); //length of substring
	int fullLength = strlen(stringList[index]); //length of string on list at index
	int subIndex = 0; //index on substring
	int occurs = 0; //number of occurences of substring
	char newString[subLength]; //comparison string, same length as substring
	//initializing comparison string as NULL
	for(i=0;i<subLength;i++)
		newString[subLength] = '\0';
	
	//loop to run through each char of string at index
	for(i=0;i<fullLength;i++)
	{	//creates new, cut-off string that is length of substring
		for(subIndex=0;subIndex<subLength;subIndex++)
			newString[subIndex] = stringList[index][i+subIndex];
		
		//if newString is same as substring
		if(strcmp(newString, substring) == 0)
			occurs++; //number of substring occurences increases by 1
	}
	
	return occurs;
}

//Takes in the List and removes the leading and trailing whitespace from the element in the list at index
void strip(char stringList[][MAX_LINE],int index)
{	int firstChar = 0, lastChar = 0; //position of first char and last char on string	

	//finding position of first char
	for(i=0;i<MAX_LINE-1;i++)
	{	if(stringList[index][i] != ' ')
		{	firstChar = i;
			break;
		}
	}
	
	//finding position of last char
	for(i=MAX_LINE-1;i>0;i--)
	{	if(stringList[index][i] != ' ' && stringList[index][i] != '\0' && stringList[index][i] != '\n')
		{	lastChar = i;
			break;
		}
	}
	
	//removing leading spaces
	for(i=firstChar;i<MAX_LINE-1;i++)
	{	//if i is between the first and last char of the strings
		if(i>=firstChar && i<=lastChar)
		{	//move previous position to begginning of string 
			stringList[index][i-firstChar] = stringList[index][i];
		}
	}
	
	//char after new string should be new line char
	stringList[index][lastChar-firstChar+1] = '\n';
	
	//changing all trailing spaces to NULL chars, i starts as 2 greater than size of string
	for(i = lastChar - firstChar + 2;i<MAX_LINE-1;i++)
	{	stringList[index][i] = '\0';
	}
}

//Takes in the List and reverses the strings about the midpoint of the List
void reverse(char stringList[][MAX_LINE],int size)
{	char temp[MAX_LINE];
	
	for(i=0;i < (size/2);i++)
	{	strcpy(temp, stringList[i]); //copies earlier string to temp
		strcpy(stringList[i], stringList[(size-1)-i]); //copies later string to earlier string
		strcpy(stringList[(size-1)-i], temp); //copies temp to later string
	}
}

//Takes in a List and returns the index where the string s occurs.
int listIndex(char stringList[][MAX_LINE],char s[],int size)
{	int stringFound = 0; //tracking whether string is found or not

	for(i=0;i<size;i++)
	{	//if string compare returns 0, then strings are the same
		if(strcmp(stringList[i], s) == 0)
		{	stringFound = 1;
			break;
		}
	}
	
	//if the string was found, return index it was found on (equals loop counter) 
	if(stringFound)
		return i;
	
	else
		return -1;		
}

//Takes in a List sorts the List in alphabetical order or backwards based on the parameter input.
void sort(char stringList[][MAX_LINE],int reverseOrder,int size)
{	char temp[MAX_LINE]; //stores arrays when switching places

	for(i=0;i<size;i++)
	{	for(j=0;j<size;j++)
		{	//if string2 is in front string1 alphabetically
			if(strcmp(stringList[j], stringList[i]) > 0)
			{	strcpy(temp, stringList[i]); //copy string1 to temp
				strcpy(stringList[i], stringList[j]); //copy string2 to string1
				strcpy(stringList[j], temp); //copy temp to string2
			}
		}	
	}
	
	if(reverseOrder == 0)
		reverse(stringList, size);
}

//Returns the length of a string at a specific index within the List
int len(char List[][MAX_LINE],int index)
{	//returns length of string, not including '\0'
	return strlen(List[index]);
}

//Replaces String s1 with String s2 within the List. Returns 1 if replacement occurred or 0 if s1 doesn't exist.
int replace(char stringList[][MAX_LINE],char s1[],char s2[],int size)
{	int stringMatch = 0; //tracks if strings match
	
	for(i=0;i<size;i++)
	{	//if the string1 is same as string on line
		if(strcmp(s1, stringList[i]) == 0)
		{	stringMatch = 1;
			break;
		}
	}
	
	//if string1 matches string in list
	if(stringMatch)
		strcpy(stringList[i], s2); //copies string2 to line of list where string1 matches
	
	return stringMatch;
}

//Empty Contents of the List and end the program.
void delete(char stringList[][MAX_LINE],int size)
{	for(i=0;i<size;i++)
	{	for(j=0;j<MAX_LINE;j++)
			stringList[i][j] = '\0';
	}
	
	exit(0);
}

//Print out the current contents of the list with line number
void print(char stringList[][MAX_LINE],int size)
{	printf("\n");
	for(i=0;i<size;i++)
		printf("%d\t%s", i+1, stringList[i]);
}

//Writes contents of List into output filename
void writeFile(FILE* out,char filename[],char List[][MAX_LINE],int size)
{	out = fopen(filename, "wx"); //opening file
	
	//safety check
	if(out == NULL)
	{	printf("%s could not be opened properly.\n", filename); 
		exit(0);
	}
	
	//printing to file
	for(i=0;i<size;i++)
		fprintf(out, "%s", List[i]);
	
	fclose(out);
}

/*===================================================
					Main Function
===================================================*/
int main(int argc, char *argv[])
{	//incorrect command line input
	if(argc != 4)
	{	printf("Incorrect number of arguments provided.\nUsage: a.out listLength input.txt output.txt\n");
		return 0;
	}
	
	int inputLength = atoi(argv[1]); //Number of lines in input file
	//Array that will act as a python list to hold all the file information
	char stringList[inputLength][MAX_LINE];
	//initializing array as NULL
	for(i=0;i<inputLength;i++)
	{	for(j=0;j<MAX_LINE;j++)
			stringList[i][j] = '\0';
	}		
	
	FILE *inputFile = fopen(argv[2],"r"); 
	FILE *outputFile;
	
	//safety check for inputFile (outputFile checked in fxn)
	if(inputFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[2]); 
		exit(0);
	}
	
	i=0;
	//scanning in inputFile
	while(fgets(stringList[i], 256, inputFile))
	{	i++;
	}
	
	int choice = 0; //integer for user's choice of function
	int attemptNum = 0; //number of incorrect function names user has input
	int index = 0; //index for certain functions
	char inputString[MAX_LINE], replacementString[MAX_LINE]; //string user inputs for comparisons
	//intializing strings as NULL
	for(i=0;i<MAX_LINE;i++)
	{	inputString[i] = '\0';
		replacementString[i] = '\0';
	}
	
	char garbage; //garbage char for reading in new lines, etc.
	int reverseSort = 0; //for reversing sort fxn
	
	//beginning of user interface
	printf("\nWelcome to the Python Processor Program!\n"); 
	printf("Below are a bunch of different Python Functionalities to perform on the List of information you provided!"); 
	
	//user interface, prints while number of incorrect function names input is less than 3
	while(attemptNum < 3)
	{	printf("\nWhat would you like to do?\n");
		printf("1) Python count method on String in List!\n2) Python strip method on String in List!\n3) Python reverse method on List!\n4) Python index method on String in List!\n");
		printf("5) Python sort method on List!\n6) Python len method on String in List!\n7) Python replace method on String in List!\n");	
		printf("8) Print out the current contents of the List! (Includes the Line Number)\n9) Python write method to print current contents of list to the outputFile!\n10) Python delete method List and then exit the program!\n");
		printf("Choose an option (1 - 10)!\n");
			
		scanf("%d%c", &choice, &garbage);  //scans in number and return
		
		//functions based on user's choice
		switch(choice)
		{	//fxn that counts number of occurences of substring within string
			case 1:
				printf("\nEnter an index from the List (0 - %d): ", inputLength-1);
				scanf("%d%c", &index, &garbage);
				printf("Enter a subtring you want to detect: ");
				scanf("%s", inputString); //scanning in string
				
				printf("\nSubstring '%s' occurs %d times at index %d!\n", inputString, count(stringList, index, inputString), index);		
				
				break;
			
			//fxn to remove leading and trailing spaces
			case 2:	
				printf("\nEnter which index (0 - %d) you would like to strip: ", inputLength-1);
				scanf("%d", &index);
				strip(stringList, index);
				printf("\nIndex %d has been stripped of leading and trailing spaces!\n", index); 
				
				break;
			
			//fxn to reverse order of list
			case 3:	
				reverse(stringList, inputLength);
				printf("\nList reversal has occured!\n");
				
				break;
			
			//fxn to find index of input string
			case 4:	
				printf("\nEnter a string to find which index it occurs on: \n");
				fgets(inputString, MAX_LINE, stdin); //scanning in string
				
				//if string found, print the mathcing index
				if(listIndex(stringList, inputString, inputLength) != -1)
				{	printf("\nString found!\n");
					printf("Index which strings match: %d\n", listIndex(stringList, inputString, inputLength));
				}
				//otherwise print that string isn't found
				else
					printf("\nString not found!\n");
				
				break;
			
			//fxn that sorts list alphabetically and reverse-alphabetically	
			case 5:	
				printf("\nWould you like to sort the list alphabetically (1) or reverse alphabetically (0)?\n");
				scanf("%d", &reverseSort);
				
				sort(stringList, reverseSort, inputLength);
				printf("\nSorting has occured!\n");
				
				break;
			
			//fxn to find number of characters in string
			case 6:	
				printf("\nEnter an index of the list (0 - %d) to find its size: ", inputLength-1);
				scanf("%d", &index);
				printf("\nLength of Index %d is: %d\n", index, len(stringList, index));
				
				break;
			
			//fxn searches for string1, and if string1 exists, the fxn replaces it with string2
			case 7:	
				printf("\nEnter a string you would like to find:\n");
				fgets(inputString, MAX_LINE, stdin); //scanning in string
				printf("\nEnter a string you would like to replace it with: \n");
				fgets(replacementString, MAX_LINE, stdin); //scanning in replacementString
				
				//if string found, replacement occured
				if(replace(stringList, inputString, replacementString, inputLength))
					printf("\nReplacement with new string has occured!\n");
				
				else
					printf("\nFirst string not found!\n");
				
				break;
			
			//print function
			case 8:
				print(stringList, inputLength);
				break;
			
			//fxn to write to outputFile
			case 9:	
				writeFile(outputFile, argv[3], stringList, inputLength);
				printf("\nList written to output file!\n");
				
				break;
			
			//delete fxn
			case 10:	
				printf("The list has been deleted! Goodbye!\n");
				delete(stringList, inputLength);
				
				break;
			
			//if user input is invalid
			default:
				if(attemptNum < 2)
					printf("\nInvalid input! Try again.\n");
				else
					printf("\nToo many invalid inputs! Program terminating.\n\n");
				
				attemptNum++;
		}			
	}
	
	//closing inputFile
	fclose(inputFile);
	
	return 0;
}
/*
Lucas Kaplan
ENEE140 0101
10-3-19

Project 1

This project will simulate the variable declaration process in C, while requiring the user to input a valid variable type and name. The user may also input an initial value for their variable.
*/

#include <stdio.h>

int main()
{
	//declare variables below
	int welcome = 0; //condensing purposes
	
	if(welcome < 1) //welcome text and invalid variable names, condensed with if statement
	{
		printf("\n\t\tWelcome to ENEE140 Project 1!\n\n");
		printf("You cannot name your variable with the following keywords:");
		
		printf("%-12s", "\nauto");
		printf("%-12s", "break");
		printf("%-12s", "case");
		printf("%-12s", "char");
		printf("%-12s", "const");
		printf("%-12s", "continue");
		printf("%-12s", "\ndefault");
		printf("%-12s", "do");
		printf("%-12s", "double");
		printf("%-12s", "else");
		printf("%-12s", "enum");
		printf("%-12s", "extern");
		printf("%-12s", "\nfloat");
		printf("%-12s", "for");
		printf("%-12s", "goto");
		printf("%-12s", "if");
		printf("%-12s", "int");
		printf("%-12s", "long");
		printf("%-12s", "\nregister");
		printf("%-12s", "return");
		printf("%-12s", "short");
		printf("%-12s", "signed");
		printf("%-12s", "sizeof");
		printf("%-12s", "static");
		printf("%-12s", "\nstruct");
		printf("%-12s", "switch");
		printf("%-12s", "typedef");
		printf("%-12s", "union");
		printf("%-12s", "unsigned");
		printf("%-12s", "void");
		printf("%-12s", "\nvolatile");
		printf("%-12s", "while\n");
		
		welcome++;
	}
	
	int counter = 0; //counter for type of variable user has input
	char vartype[99]; //type of variable user inputs
	char name[99]; //name of variable
	int namecounter = 0; //for mutliple tries to name variable
	int valid = 0; //to test if user has gone over name attempt limit
	int selection; //for upper/lowercase conversion
	int slot = 0; //slot in name string
	char inputyes; //yes/no if user wants to input initial value
	char initial[99]; //intial value that user has input
	
	while(counter < 3)
	{
		printf("\nEnter the type of your variable (int, float, or char): ");
		scanf("%s", vartype);
		
		if(vartype[0] == 'i') //checking for int
		{
			if(vartype[1] == 'n')
			{
				if(vartype[2] == 't')
				{
					if(vartype[3] == '\0')
					{
						counter = counter + 10; //if user inputs correct value, counter will be > 3
					}
					
					else
					{
						printf("\nInvalid input! Try again.\n");
						counter++;
					}
				}
				
				else
				{
					printf("\nInvalid input! Try again.\n");
					counter++;
				}					
			}
			
			else
			{
				printf("\nInvalid input! Try again.\n");
				counter++;
			}				
			
		}	
		
		else if(vartype[0] == 'f') //checking for float
		{
			if(vartype[1] == 'l')
			{
				if(vartype[2] == 'o')
				{
					if(vartype[3] == 'a')
					{
						if(vartype[4] == 't')
						{
							if(vartype[5] == '\0')
							{
								counter = counter + 10; //if user inputs correct value, counter will be > 3
							}
							
							else
							{
								printf("\nInvalid input! Try again.\n");
								counter++;
							}
						}
						
						else
						{
							printf("\nInvalid input! Try again.\n");
							counter++;
						}
					}
					
					else
					{
						printf("\nInvalid input! Try again.\n");
						counter++;
					}
				}

				else
				{
					printf("\nInvalid input! Try again.\n");
					counter++;
				}
					
				
			}
			
			else
			{
				printf("\nInvalid input! Try again.\n");
				counter++;
			}				
			
		}
		
		else if(vartype[0] == 'c')
		{
			if(vartype[1] == 'h')
			{
				if(vartype[2] == 'a')
				{
					if(vartype[3] == 'r')
					{
						if(vartype[4] == '\0')
						{
							counter = counter + 10; //if user inputs correct value, counter will be > 3
						}
						
						else
						{
							printf("\nInvalid input! Try again.\n");
							counter++;
						}
					}
					
					else
					{
						printf("\nInvalid input! Try again.\n");
						counter++;
					}
				}
				
				else
				{
					printf("\nInvalid input! Try again.\n");
					counter++;
				}
			}
			
			else
			{
				printf("\nInvalid input! Try again.\n");
				counter++;
			}
		}
		
		
		//when first letter corresponds with no valid variable type
		else if(!(vartype[0] == 'i' || vartype[0] == 'f' || vartype[0] == 'c'))
		{
			printf("\nInvalid input! Try again.\n");
			counter++;
		}
	}
	
	if(counter == 3) //when user goes through 3 invalid attempts, counter will be 3
	{
		printf("3 invalid attempts at specifying variable type. Program terminating.\n");
		return 0; //terminates program
	}
	
	else  //user has enetered a valid variable type
	{
		printf("\nYou can now name your variable. Please note that your name must start with a letter or '_', must be no more than 31 characters, and that you can't use the intially listed keywords as your name.\n\n");
		
		while(namecounter < 3) //used to allow mutliple tries to name variable
		{	
			printf("Enter the name of your variable: ");
			scanf("%s", name);
			
			//when user inputs valid first character
			if(((name[0] >= 'A') && (name[0] <= 'Z')) || ((name[0] >= 'a') && (name[0] <= 'z')) || (name[0] == '_'))
			{
				slot = 0;
				
				while(slot <= 31) //checks through all valid length name slots of string
				{
					//checking if special characters (not allowed) are used
					if(!(((name[slot] >= 'A') && (name[slot] <= 'Z')) || ((name[slot] >= 'a') && (name[slot] <= 'z')) || (name[slot] == '_') || (name[slot] == '\0') || ((name[slot] >= '0') && (name[slot] <= '9'))))
					{
						slot = (slot - slot) + 100; //exit name checking loop
						
						//program only terminates when user has gone through 3 tries
						if(namecounter == 3)
						{
							valid = 3; 
						}
					}
					
					if((name[slot] == '\0') && (slot <= 31)) //if size of name is valid
					{
						namecounter = namecounter + 10; //original while loop exited
						slot = (slot - slot) + 100; //exit valid name loop
						valid = 1; //name is valid
 					}
					
					slot++;
				}
				
				//if size of name is invalid or invalid character used
				if(valid == 0)
				{
					namecounter++; //go back to beginning of name loop
					if(namecounter < 3)
					{	
						printf("\nInvalid name input! Try again.\n\n");
					}
				}
				
			}
			
			
			else //user inputs invalid first character 
			{
				namecounter++; //go back to intial name prompt
				if(namecounter < 3)
				{	
					printf("\nInvalid name input! Try again.\n\n");
				}
			}
		}
		
		if((namecounter == 3) || (valid == 3)) //if user takes too many tries to input name
		{
			printf("\n3 invalid attempts at specifying variable name. Program terminating.\n\n");
			return 0; //terminates program
		}
		
		else //user has input valid name in 3 or less tries
		{
			printf("\nGood job! You input a valid name for your varaible.\n");
			
			printf("\nWould you like to convert your name into lowercase or uppercase?");
			printf("\nConversion options: \n1: Use all uppercase letters \n2: Use all lowercase letters \n3: Leave my variable name as is (default) \n");
			printf("\nEnter your selection (1, 2, 3): ");
			scanf("%d", &selection);
			
			if(selection == 1) //all UPPERCASE letters
			{
				slot = 0;
				
				while(slot <= 31)
				{
					
					//if the letter is lowercase
					if((name[slot] >= 'a') && (name[slot] <= 'z'))
					{
						name[slot] = name[slot] - 'a' + 'A';
						slot++;
					}
					
					else
					{
						slot++;
					}
				}
			}
			
			if(selection == 2) //all lowercase letters
			{
				slot = 0;
				
				while(slot <= 31)
				{
					//if the letter is UPPERCASE
					if((name[slot] >= 'A') && (name[slot] <= 'Z'))
					{
						name[slot] = name[slot] - 'A' + 'a';
						slot++;
					}
					
					else
					{
						slot++;
					}
				}
			}
			
			if(((selection != 1) || (selection != 2)) || (slot == 32))
			{
				
				printf("\nDo you have an initial value for %s? (Y/N): ", name);
				scanf(" %c", &inputyes); //need space before %c so that computer skips over blank space
				
				if((inputyes == 'y') || (inputyes == 'Y'))
				{
					if(vartype[0] == 'i')
					{
						printf("\nEnter your integer value: ");
						scanf("%s", initial);
						
						printf("\nint %s = %s;\n\n", name, initial);
					}

					if(vartype[0] == 'f')
					{
						printf("\nEnter your floating-point value: ");
						scanf("%s", initial);
						
						printf("\nfloat %s = %s;\n\n", name, initial);
					}
				
					if(vartype[0] == 'c')
					{
						printf("\nEnter your character: ");
						scanf("%s", initial);
						
						printf("\nchar %s = '%s';\n\n", name, initial);
					}
				
				}
			
				else if(!((inputyes == 'y') || (inputyes == 'Y')))
				{
					if(vartype[0] == 'i')
					{
						printf("\nint %s;\n\n", name);
					}
					
					if(vartype[0] == 'c')
					{
						printf("\nchar %s;\n\n", name);
					}
					
					if(vartype[0] == 'f')
					{
						printf("\nfloat %s;\n\n", name);
					}
				}
				
			}
		}	
		
	}
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
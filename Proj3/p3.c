/*
Lucas Kaplan
ENEE140
0101
11/10/19

Project 3

This project will simulate a simplified version of 2-player Monopoly, in which the user will play against the computer. There will be a 20x20 board containing a closed path, with properties that you can purchase and upgrade (twice) afterwards. There will also be Chance cards located along the path that will have different instructions. The game ends when one player runs out of cash and goes bankrupt.  
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define startBalance 1500

int main(int argc, char *argv[]) 
{	srand(time(NULL)); //to generate new random numbers each time

	int pathLength; //number of lines in pathFile, equivalent to length of path
	int p1Balance = startBalance, p2Balance = startBalance; //starting balance for each player
	int turnNumber = 1; //turn number
	int die1 = 0, die2 = 0; //values for the two dice
	int col = 0, row = 0; //counters for columns and rows on board
	FILE *pathFile, *propertyFile, *chanceFile; //for path.txt, prop.txt (property file), chance.txt
	int i=0, j=0; //counters
	
	//incorrect command line input
	if (argc != 5) 
	{	printf("Incorrect number of arguments provided.\n");
		printf("Usage: a.out path_length path_file property_file chance_file\n");
		exit(0);	
	}

	pathLength = atoi(argv[1]); //scanning in pathLength from command line
	//arrays to store each line of pathFile
	int y[pathLength], x[pathLength], yNext[pathLength], xNext[pathLength];
	//initializing all arrays as 0
	for(i=0;i<pathLength;i++)
		y[i] = 0;
	
	for(i=0;i<pathLength;i++)
		x[i] = 0;
	
	for(i=0;i<pathLength;i++)
		yNext[i] = 0;
	
	for(i=0;i<pathLength;i++)
		xNext[i] = 0;
	
	//opening pathFile, propertyFile, chanceFile
	pathFile = fopen(argv[2], "r");
	propertyFile = fopen(argv[3], "r");
	chanceFile = fopen(argv[4], "r");
	
	//safety check for all files (path, property, and chance)
	if(pathFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[2]);
		exit(0);
	}
	
	if(propertyFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[3]);
		exit(0);
	}
	
	if(chanceFile == NULL)
	{	printf("%s could not be opened properly.\n", argv[4]);
		exit(0);
	}
	
	char pathBoard[21][21]; //21x21array for the board, path, and axis labels
	for(i=0;i<21;i++) //initializing array as all spaces
	{	for(j=0;j<21;j++)
		{	pathBoard[i][j] = ' ';
		}
	}
	
	//creating the column labels (the first row)
	for(col=1; col<21; col++)
	{	if(col<=10)
			pathBoard[0][col] = col-1 + '0';
		
		else
			pathBoard[0][col] = col-11 + '0';
	}
	//creating the row labels (the first column)
	for(row=1; row<21; row++)
	{	if(row<=10)
			pathBoard[row][0] = row-1 + '0';
		
		else
			pathBoard[row][0] = row-11 + '0';
	}
	
	//store the direction characters (< > v ^)
	i=0;
	while(fscanf(pathFile, "(%d %d) -> (%d %d) ", &y[i], &x[i], &yNext[i], &xNext[i]) != EOF)
	{	if(xNext[i] > x[i])
		{	pathBoard[y[i]+1][x[i]+1] = '>';
		}
		
		if(xNext[i] < x[i])
		{	pathBoard[y[i]+1][x[i]+1] = '<';
		}
		
		if(yNext[i] > y[i])
		{	pathBoard[y[i]+1][x[i]+1] = 'v';
		}
		
		if(yNext[i] < y[i])
		{	pathBoard[y[i]+1][x[i]+1] = '^';
		}
		
		i++;
	}
	
	//******* scanning in Property File *******
	//array to store coordinates and prices on each line of property file
	int propData[pathLength][8]; 
	//initializing array as -1, so if there's less properties than pathLength value, (0, 0) won't come up as property
	for(i=0;i<pathLength;i++)
	{	for(j=0;j<8;j++)
			propData[i][j] = -1;
	}
	i = 0; //need to set i back to 0 before while loop
	while(fscanf(propertyFile, "(%d %d) %d %d %d | %d %d %d ", &propData[i][0], &propData[i][1], &propData[i][2], &propData[i][3], &propData[i][4], &propData[i][5], &propData[i][6], &propData[i][7]) != EOF)
	{	i++;
	}
	
	//******** scanning in Chance File ***********
	int chanceData[pathLength][2]; //array to store coordinates of chance cards
	//initializing array as -1, so if there's less chance cards than pathLength value, (0, 0) won't come up as chance card
	for(i=0;i<pathLength;i++)
	{	for(j=0;j<2;j++)
			chanceData[i][j] = -1;
	}
	i = 0; //need to set i back to 0 before while loop
	while(fscanf(chanceFile, "(%d %d) ", &chanceData[i][0], &chanceData[i][1]) != EOF)
	{	i++;
	}
	int chanceLine = i-1; //stores number of lines in chanceFile
	
	//********************* Variables for Actual Gameplay ***********************
	//player 1 and player 2 coordinates
	int p1CoordinateY = 1, p1CoordinateX = 1;
	int p2CoordinateY = 1, p2CoordinateX = 1;
	int p1Position = 0; //line number of pathFile player 1 is on
	int p2Position = 0; //line number of pathFile player 2 is on
	char enter, enter2; //extra char
	int jail[2] = {0, 0}; //array to keep track of if player is in jail (uses 0 for p1 and 1 for p2)
	char modTurn[2] = {' ', ' '}; //array to keep track of if player has gained/lost turn
	int chanceMove = 0; //for moving fowards and backwards due to chance cards
	int upgradeStage[pathLength]; //upgrade stage for property
	for(i=0;i<pathLength;i++)
		upgradeStage[i] = 0;
	
	char propOwner[pathLength][5]; //property owner
	//initializing each value to be "None"
	for(i=0;i<pathLength;i++)
	{	propOwner[i][0] = 'N';
		propOwner[i][1] = 'o';	
		propOwner[i][2] = 'n';
		propOwner[i][3] = 'e';
		propOwner[i][4] = '\0';
	}
	
	//************************** GAMEPLAY *****************************
	// while game not over
	while (p1Balance > 0 && p2Balance > 0) 
	{	//roll the die twice to get die1 and die2
		die1 = rand()%6 + 1;
		die2 = rand()%6 + 1;
		
		//**************** EVEN TURN = P2'S TURN (COMPUTER) ******************************
		if(turnNumber%2 == 0)
		{	printf("\n*****************************************************\n");
			printf("\t\tTurn Number: %d\n", turnNumber);
			printf("\t\tP2's Turn\n");
			printf("\t\tPlayer 1 Balance: $%d\n", p1Balance); 
			printf("\t\tPlayer 2 Balance: $%d\n", p2Balance);
			//if player2 is in jail
			if(jail[1] == 1)
				printf("\t\tIN JAIL\n");
			printf("*****************************************************\n\n");
		
			//printing out board
			for(row=0;row<21;row++)
			{	for(col=0;col<21;col++)
				{	//if both players on same space
					if((row == p1CoordinateY && col == p1CoordinateX) && (row == p2CoordinateY && col == p2CoordinateX))
						printf("%-c%-2c", pathBoard[row][col], 'E');
					
					//printing player 1's space
					else if(row == p1CoordinateY && col == p1CoordinateX)
						printf("%-c%-2c", pathBoard[row][col], 'A');
					
					//printing player 2's space
					else if(row == p2CoordinateY && col == p2CoordinateX)
						printf("%-c%-2c", pathBoard[row][col], 'B');
					
					else
						printf("%-3c", pathBoard[row][col]);
				}
				printf("\n");
			}
			
			printf("\nP2 rolled: %d + %d", die1, die2);			
			
			//if player2 is in jail and doesn't roll doubles
			if(jail[1] == 1 && die1 != die2)
				printf("\nNot doubles! Stay in jail!\n");
			
			else //if player2 NOT in jail or rolls doubles
			{	if(die1 == die2 && jail[1] == 1) //if p2 rolls doubles AND they were in jail
				{	printf("\nDoubles! Get out of jail!\n");
					jail[1] = 0; //p2 is no longer in jail
				}
				
				p2Position += die1+die2; //adding die values to player2 position
				
				//if player passes or lands on go
				if(p2Position > pathLength - 1)
				{	p2Position -= pathLength; //p2's line on pathFile resets to 0
					p2Balance += 200;
				}
				
				//using coordinates on pathFile to move player to that set of coordinates
				p2CoordinateY = y[p2Position] + 1;
				p2CoordinateX = x[p2Position] + 1;
				
				printf("\nP2 landed on: (%d %d)\n\n", p2CoordinateY-1, p2CoordinateX-1);
				
				//printing out board again
				for(row=0;row<21;row++)
				{	for(col=0;col<21;col++)
					{	//if both players on same space
						if((row == p1CoordinateY && col == p1CoordinateX) && (row == p2CoordinateY && col == p2CoordinateX))
							printf("%-c%-2c", pathBoard[row][col], 'E');
						
						//printing player 1's space
						else if(row == p1CoordinateY && col == p1CoordinateX)
							printf("%-c%-2c", pathBoard[row][col], 'A');
						
						//printing player 2's space
						else if(row == p2CoordinateY && col == p2CoordinateX)
							printf("%-c%-2c", pathBoard[row][col], 'B');
						
						else
							printf("%-3c", pathBoard[row][col]);
					}
					printf("\n");
				}
				
				//********************* CHANCE CARDS ************************************
				for(i=0;i<chanceLine;i++)
				{	//if p2 lands on chance card
					if(p2CoordinateY-1 == chanceData[i][0] && p2CoordinateX-1 == chanceData[i][1])
					{	//chance action can be 1 of 7 options
						switch(i%7)
						{	//if player lands on first chance position
							//GO TO JAIL
							case 0: 
								printf("\nPlayer 2 is visiting JAIL!\n");
								printf("Do not pass go! Do not collect $200!\n");
								jail[1] = 1;
								break;
						
							//receiving money
							case 1: 
								printf("\nDr. Loh takes pity on your soul. Receive $500.\n");
								p2Balance += 500;
								break;
								
							//losing money
							case 2: 
								printf("\nTheif! You stole a sacred offering to Testudo! Lose $250.\n");
								p2Balance -= 250;
								break;
								
							//receiving extra turn
							case 3: 
								printf("\nYour TA extends the project deadline! Receive an extra turn.\n");
								modTurn[1] = 'g'; //keeps track of p2 gaining turn
								break;
							
							//losing a turn
							case 4: 
								printf("\nYou skip discussion and couldn't understand Dr. Q's lecture! Lose a turn.\n");
								modTurn[1] = 'l'; //keeps track of player2 losing turn
								break;
								
							//move forwards
							case 5: 
								chanceMove = rand()%3 + 1;
								printf("\nYou rent an electric scooter for the day. Move forward %d spaces.\n", chanceMove);
								p2Position += chanceMove;
								
								//if p2Position increases more than pathLength (they also pass go)
								if(p2Position > pathLength - 1)
								{	p2Position -= pathLength; //p2's value on pathFile resets to 0 through pathLength
									p2Balance += 200;
								}
								
								//using coordinates on pathFile to move player to that set of coordinates
								p2CoordinateY = y[p2Position] + 1;
								p2CoordinateX = x[p2Position] + 1;
								break;
								
							//move backwards
							case 6: 
								chanceMove = rand()%3 + 1;
								printf("\nYour crabs have escaped the crab cage! Move backwards %d spaces to catch them.\n", chanceMove);
								p2Position -= chanceMove;
								
								//if p2Position becomes negative
								if(p2Position < 0)
									p2Position += pathLength; //add pathLength to p2Position to put p2Position at near-end area of propFile
									
								//using coordinates on pathFile to move player to that set of coordinates
								p2CoordinateY = y[p2Position] + 1;
								p2CoordinateX = x[p2Position] + 1;
								break;
						}
					}			
				}
				
				//************************ PROPERTY *****************************
				for(i=0;i<pathLength;i++)
				{	//if player lands on property
					if(p2CoordinateY-1 == propData[i][0] && p2CoordinateX-1 == propData[i][1])
					{	//if property already upgraded to stage 3
						if(upgradeStage[i] == 3)
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							//coordinates of property
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); 
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("%-14s", "N/A"); //upgrade cost doesn't exist
							printf("$%-10d", propData[i][4+upgradeStage[i]]); //when upgrade stage is 3, prints propData[i][7], which is rentalCost 3
						}
						
						//if property at stage 0
						else if(upgradeStage[i] == 0)
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							//coordinates of property
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); 
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("$%-13d", propData[i][2+upgradeStage[i]]); //propData[2] is upgradeCost 1, goes up by 1 for each upgradeStage
							printf("$%-10d", 0); //rental cost is 0
						}
						
						//if property at stage 1 or 2
						else
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							//coordinates of property
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); 
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("$%-13d", propData[i][2+upgradeStage[i]]); //propData[2] is upgradeCost 1, goes up by 1 for each upgradeStage
							printf("$%-10d", propData[i][4+upgradeStage[i]]); //rentalCost starts at propData[][5] when upgradeStage >= 1
						}
						
						//if property already upgraded to stage 3
						if(upgradeStage[i] == 3)
							printf("\n\nUpgrade stage already at max value. Cannot upgrade property.\n");
						
						//if property unowned
						if(propOwner[i][0] == 'N')
						{	//if p2 has enough money to purchase property, they will
							if(p2Balance > propData[i][2])
							{	//propOwner[i] becomes "P2"
								propOwner[i][0] = 'P';
								propOwner[i][1] = '2';
								propOwner[i][2] = '\0';
								propOwner[i][3] = ' ';
								propOwner[i][4] = ' ';
								
								p2Balance -= propData[i][2]; //reduce the p2Balance by the cost of purchasing property
								upgradeStage[i]++; //upgradeStage of property increases by 1
								
								printf("\n\nP2 purchased property!\n");
								printf("New P2 Balance: $%d\n", p2Balance);
							}
							
							else
								printf("\n\nP2 has insufficient funds to purchase property.\n");
						}
							
						//if property owned by p2
						else if(propOwner[i][1] == '2')
						{	//if p2 has enough money to upgrade property, they will
							if(p2Balance > propData[i][2+upgradeStage[i]])
							{	//reduce the p2Balance by the cost of upgrading property
								p2Balance -= propData[i][2+upgradeStage[i]]; 
								upgradeStage[i]++; //upgradeStage of property increases by 1 AFTER balance decreased
								
								printf("\n\nP2 upgraded property!\n");
								printf("New P2 Balance: $%d\n", p2Balance);
							}
							
							else
								printf("\n\nP2 has insufficient funds to upgrade property.\n");
						}
							
						//if property owned by p1
						else if(propOwner[i][1] == '1')
						{	printf("\n\nYou landed on P1's property!\n");
							printf("Pay $%d in rent.\n", propData[i][4+upgradeStage[i]]);
							p2Balance -= propData[i][4+upgradeStage[i]]; //p2Balance decreases by rental cost of property, based on upgrade stage
							p1Balance += propData[i][4+upgradeStage[i]]; //p1Balance increases by rental cost of property, based on upgrade stage
							
							//if player's balance becomes negative
							if(p2Balance < 0)
								printf("\nNew P2 Balance: -$%d\n", -p2Balance);
								
							else
								printf("\nNew P2 Balance: $%d\n", p2Balance);		
						}
					}
				}
			}
			
			printf("\nEnd of P2's turn.\n");
			
			//if player1 loses turn
			if(modTurn[0] == 'l')
			{	//if player2 also lost turn
				if(modTurn[1] == 'l')
				{	turnNumber++;
					modTurn[0] = ' '; //modTurn goes back to normal for both
					modTurn[1] = ' ';
				}
				
				turnNumber += 2; //turnNumber increases by 2
				modTurn[0] = ' '; //modTurn goes back to normal
			}
			
			//if player2 gains turn
			else if(modTurn[1] == 'g')
			{	//if player1 also gained turn
				if(modTurn[0] == 'g')
				{	turnNumber++;
					modTurn[0] = ' '; //modTurn goes back to normal for both
					modTurn[1] = ' ';
				}
		
				modTurn[1] = ' '; //modTurn goes back to normal
			}
			
			else //end of regular turn
				turnNumber++; //at end of turn, turnNumber increases by 1	
		}
		
		
		//******************* ODD TURN = P1'S TURN  (USER) ************************
		else 
		{	printf("\n*****************************************************\n");
			printf("\t\tTurn Number: %d\n", turnNumber);
			printf("\t\tP1's Turn\n");
			printf("\t\tPlayer 1 Balance: $%d\n", p1Balance);
			printf("\t\tPlayer 2 Balance: $%d\n", p2Balance);
			//if player1 is in jail
			if(jail[0] == 1)
				printf("\t\tIN JAIL\n");
			printf("*****************************************************\n\n");
		
			//printing out board
			for(row=0;row<21;row++)
			{	for(col=0;col<21;col++)
				{	//if both players on same space
					if((row == p1CoordinateY && col == p1CoordinateX) && (row == p2CoordinateY && col == p2CoordinateX))
						printf("%-c%-2c", pathBoard[row][col], 'E');
					
					//printing player 1's space
					else if(row == p1CoordinateY && col == p1CoordinateX)
						printf("%-c%-2c", pathBoard[row][col], 'A');
					
					//printing player 2's space
					else if(row == p2CoordinateY && col == p2CoordinateX)
						printf("%-c%-2c", pathBoard[row][col], 'B');
					
					else
						printf("%-3c", pathBoard[row][col]);
				}
				printf("\n");
			}
			
			printf("\nPress enter to roll the dice. ");
			scanf("%c", &enter);
			printf("\nYou rolled: %d + %d", die1, die2);			
			
			//if player1 is in jail and doesn't roll doubles
			if(jail[0] == 1 && die1 != die2)
				printf("\nNot doubles! Stay in jail!\n");
			
			else //if player1 NOT in jail or rolls doubles
			{	if(die1 == die2 && jail[0] == 1) //if p1 rolls doubles AND they were in jail
				{	printf("\nDoubles! Get out of jail!\n");
					jail[0] = 0; //p1 is no longer in jail
				}
				
				p1Position += die1+die2; //adding die values to player1 position
				
				//if player passes or lands on go
				if(p1Position > pathLength - 1)
				{	p1Position -= pathLength; //p1's line on pathFile resets to 0
					p1Balance += 200;
				}
				
				//using coordinates on pathFile to move player to that set of coordinates
				p1CoordinateY = y[p1Position] + 1;
				p1CoordinateX = x[p1Position] + 1;
				
				printf("\nYou landed on: (%d %d)\n\n", p1CoordinateY-1, p1CoordinateX-1);
				
				//printing out board again
				for(row=0;row<21;row++)
				{	for(col=0;col<21;col++)
					{	//if both players on same space
						if((row == p1CoordinateY && col == p1CoordinateX) && (row == p2CoordinateY && col == p2CoordinateX))
							printf("%-c%-2c", pathBoard[row][col], 'E');
						
						//printing player 1's space
						else if(row == p1CoordinateY && col == p1CoordinateX)
							printf("%-c%-2c", pathBoard[row][col], 'A');
						
						//printing player 2's space
						else if(row == p2CoordinateY && col == p2CoordinateX)
							printf("%-c%-2c", pathBoard[row][col], 'B');
						
						else
							printf("%-3c", pathBoard[row][col]);
					}
					printf("\n");
				}
				
				//********************* CHANCE CARDS ************************************
				for(i=0;i<chanceLine;i++)
				{	//if p1 lands on chance card
					if(p1CoordinateY-1 == chanceData[i][0] && p1CoordinateX-1 == chanceData[i][1])
					{	//chance action can be 1 of 7 options
						switch(i%7)
						{	//if player lands on first chance position
							//GO TO JAIL
							case 0: 
								printf("\nPlayer 1 is visiting JAIL!\n");
								printf("Do not pass go! Do not collect $200!\n");
								jail[0] = 1;
								break;
						
							//receiving money
							case 1: 
								printf("\nDr. Loh takes pity on your soul. Receive $500.\n");
								p1Balance += 500;
								break;
								
							//losing money
							case 2: 
								printf("\nTheif! You stole a sacred offering to Testudo! Lose $250.\n");
								p1Balance -= 250;
								break;
								
							//receiving extra turn
							case 3: 
								printf("\nYour TA extends the project deadline! Receive an extra turn.\n");
								modTurn[0] = 'g'; //keeps track of p1 gaining turn
								break;
							
							//losing a turn
							case 4: 
								printf("\nYou skip discussion and couldn't understand Dr. Q's lecture! Lose a turn.\n");
								modTurn[0] = 'l'; //keeps track of player1 losing turn
								break;
								
							//move forwards
							case 5: 
								chanceMove = rand()%3 + 1;
								printf("\nYou rent an electric scooter for the day. Move forward %d spaces.\n", chanceMove);
								p1Position += chanceMove;
								
								//if p1Position increases more than pathLength (they also pass go)
								if(p1Position > pathLength - 1)
								{	p1Position -= pathLength; //p1's value on pathFile resets to 0 through pathLength
									p1Balance += 200;
								}
								
								//using coordinates on pathFile to move player to that set of coordinates
								p1CoordinateY = y[p1Position] + 1;
								p1CoordinateX = x[p1Position] + 1;
								break;
								
							//move backwards
							case 6: 
								chanceMove = rand()%3 + 1;
								printf("\nYour crabs have escaped the crab cage! Move backwards %d spaces to catch them.\n", chanceMove);
								p1Position -= chanceMove;
								
								//if p1Position becomes negative
								if(p1Position < 0)
									p1Position += pathLength; //add pathLength to p1Position to put p1Position at near-end area of propFile
									
								//using coordinates on pathFile to move player to that set of coordinates
								p1CoordinateY = y[p1Position] + 1;
								p1CoordinateX = x[p1Position] + 1;
								break;
						}
					}			
				}
				
				//************************ PROPERTY *****************************
				for(i=0;i<pathLength;i++)
				{	//if player lands on property
					if(p1CoordinateY-1 == propData[i][0] && p1CoordinateX-1 == propData[i][1])
					{	//if property already upgraded to stage 3
						if(upgradeStage[i] == 3)
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); //coordinates of property
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("%-14s", "N/A"); //upgrade cost doesn't exist
							printf("$%-10d", propData[i][4+upgradeStage[i]]); //when upgrade stage is 3, prints propData[i][7], which is rentalCost 3
						}
						
						//if property at stage 0
						else if(upgradeStage[i] == 0)
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); //coordinates of property
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("$%-13d", propData[i][2+upgradeStage[i]]); //propData[2] is upgradeCost 1, goes up by 1 for each upgradeStage
							printf("$%-10d", 0);
						}
						
						//if property at stage 1 or 2
						else
						{	printf("\n%-9s%-7s%-8s%-7s%-8s%-6s%-7s%-4s\n", "Space", "Owner", "Upgrade",  "Stage", "Upgrade",  "Cost", "Rental",  "Cost");
							printf("(%02d %02d)  ", propData[i][0], propData[i][1]); //coordinates of property
							printf("%-7s", propOwner[i]);
							printf("%-15d", upgradeStage[i]);
							printf("$%-13d", propData[i][2+upgradeStage[i]]); //propData[2] is upgradeCost 1, goes up by 1 for each upgradeStage
							printf("$%-10d", propData[i][4+upgradeStage[i]]);
						}
						
						//if property already upgraded to stage 3
						if(upgradeStage[i] == 3)
							printf("\n\nUpgrade stage already at max value. Cannot upgrade property.\n");
						
						//if property unowned
						if(propOwner[i][0] == 'N')
						{	printf("\n\nWould you like to purchase this property (Y/N)? ");
							scanf("%c%c", &enter, &enter2); //y/n and carriage return  (CR) characters
							
							//if p1 purchases property
							if(enter == 'Y' || enter == 'y')
							{	//propOwner[i] becomes "P1"
								propOwner[i][0] = 'P';
								propOwner[i][1] = '1';
								propOwner[i][2] = '\0';
								propOwner[i][3] = ' ';
								propOwner[i][4] = ' ';
								
								p1Balance -= propData[i][2]; //reduce the p1Balance by the cost of purchasing property
								upgradeStage[i]++; //upgradeStage of property increases by 1
								
								//if player's balance becomes negative
								if(p1Balance < 0)
									printf("\nNew P1 Balance: -$%d\n", -p1Balance);
								
								else
									printf("\nNew P1 Balance: $%d\n", p1Balance);
							}
						}
						
						//if property owned by p1
						else if(propOwner[i][1] == '1')
						{	printf("\n\nWould you like to upgrade this property (Y/N)? ");
							scanf("%c%c", &enter, &enter2); //y/n and CR characters
							
							//if p1 upgrades property
							if(enter == 'Y' || enter == 'y')
							{	p1Balance -= propData[i][2+upgradeStage[i]]; //reduce the p1Balance by the cost of upgrading property
								upgradeStage[i]++;
								
								//if player's balance becomes negative
								if(p1Balance < 0)
									printf("\nNew P1 Balance: -$%d\n", -p1Balance);
								
								else
									printf("\nNew P1 Balance: $%d\n", p1Balance);
							}
						}
						
						//if property owned by p2
						else if(propOwner[i][1] == '2')
						{	printf("\n\nYou landed on P2's property!\n");
							printf("Pay $%d in rent.\n", propData[i][4+upgradeStage[i]]);
							p1Balance -= propData[i][4+upgradeStage[i]]; //p1Balance decreases by rental cost of property, based on upgrade stage
							p2Balance += propData[i][4+upgradeStage[i]]; //p2Balance increases by rental cost of property, based on upgrade stage
							
							//if player's balance becomes negative
							if(p1Balance < 0)
								printf("\nNew P1 Balance: -$%d\n", -p1Balance);
								
							else
								printf("\nNew P1 Balance: $%d\n", p1Balance);
						}
					}
				}
			}
		
			printf("\nPress enter to end your turn.");
			scanf("%c", &enter);
			
			//if player2 loses turn
			if(modTurn[1] == 'l')
			{	//if player1 also lost turn
				if(modTurn[0] == 'l')
				{	turnNumber++;
					modTurn[0] = ' '; //modTurn goes back to normal for both
					modTurn[1] = ' ';
				}
				
				turnNumber += 2; //turnNumber increases by 2
				modTurn[1] = ' '; //modTurn goes back to normal
			}
			
			//if player1 gains turn
			else if(modTurn[0] == 'g')
			{	//if player2 also gained turn
				if(modTurn[1] == 'g')
				{	turnNumber++;
					modTurn[0] = ' '; //modTurn goes back to normal for both
					modTurn[1] = ' ';
				}
				
				modTurn[0] = ' '; //modTurn goes back to normal
			}	
			
			else //end of normal turn
				turnNumber++; //turnNumber increases by 1
		}
	}

	//check and print the winner
	if(p1Balance < 0)
		printf("Player 1 has gone bankrupt. Player 2 wins!\n\n");
	
	if(p2Balance < 0)
		printf("Player 2 has gone bankrupt. Player 1 wins!\n\n");
	
	//closing pathFile, propertyFile, chanceFile
	fclose(pathFile);
	fclose(propertyFile);
	fclose(chanceFile);
	
	return 0;
}
	
	
	
	
	
	
	

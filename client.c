/*
    C ECHO client example using sockets
*/

// utilize libraries
	#include<stdio.h> 							// printf
	#include<string.h>    						// strlen, fgets	
	#include<stdlib.h>							// malloc

// networking libraries
	#include<sys/socket.h>						// socket
	#include<arpa/inet.h> 						// inet_addr
	#include<unistd.h>    						// write



// DBF -- creating booleans
	#define bool int								// creating booleans in C
	#define true 1									// boolean:  TRUE state
	#define false 0								// boolean:  FALSE state




// DBF -- creating booleans
	#define bool int								// creating booleans in C
	#define true 1									// boolean:  TRUE state
	#define false 0								// boolean:  FALSE state

// string break apart code
	int 	p_last;									// count of arguments
	char	p_items[20][20];						// array of arguments
	
	
// indicate which side of exchange
	bool		IsClient = true;					// not used yet
	bool		IsServer = false;					// not used yet
	
	char		Server_Error_Message[150];		// Error message for server
	char		ProblemDefined[150];				// definition of problem found
	
	
// client stuff
	char		Next_Client_Command[100];		// next Client Command
	int		New_Port_To_Change_To;			// next port to change to
	
	
//
// talking functions
//
void p(char *something_to_say)
{
	printf("%s\n", something_to_say); 
}

void P(char *something_to_say)
{
	printf("%s\n", something_to_say); 
}

	
	
	
//
// below, will break this into multiple pieces for easy processing
//	
void Break_Into_Pieces(char *LongString )
{
	// variables needed
		int	length;
		
		
	// p("\n************* IN -- Break_Into_Pieces ************* \n");
	
	// p(LongString);
	
	// variable setup
		p_last = 0;		
	
	// see if string has any length; if not then leave
		length = strlen(LongString);
		if (length == 0)
			{
				// leave if no length
					// p(" string has NO LENGTH ...");	
					goto Leave_Function;
			}
			
	// add extra delimiter to end of line -- fix it
		LongString[length] = ' ';
		length = length + 1;
		LongString[length] = '\0';
			
	// increment for first line
		p_last = 1;
			
	// process and break apart at spaces
		int	i;
		char	ch;
		int	over;
		
		over = 0;
		for (i = 0; i < length; i++)
		{
			ch = LongString[i];
			// printf ("%c", ch);
			
			if ((ch == '\0') || (ch == ' '))
				{
				
					// put EOL at current spot
						p_items[p_last][over] = '\0';						
					
					// next item
						p_last = p_last + 1;
						
					// reset over
						over = 0;
						
					// identify it
						// if (ch == '\0') { printf(" {EOL} ");	}
						// if (ch == ' ')  { printf(" {SP} ");	}
				}
			else
				{
					// store character
						p_items[p_last][over] = ch;
						// printf("!");
						
					// move over
						over = over + 1;
				}
		}
		// printf("\n");
		
	// decrement argument count
		p_last = p_last - 1;
		
	// list items
		bool List_Items; 				// for debugging
		
		//List_Items = true;
		List_Items = false;
		
		if (List_Items == true)
			{
				for (i = 1; i <= p_last; i++)
					{
						printf(" %i -- {%s} \n", i, p_items[i]);
					}
			}
		
	// 
		
	
	
Leave_Function:
	// p("\n************* OUT -- Break_Into_Pieces ************* \n");
	return;
}



//
// Verify of incoming input
//
bool Matching_Current_Pieces(int Vcount, char *V1, char *V2)
{
	// default value
		bool Result = true;
		
	// see if pieces existing are not enough
		if (Vcount > p_last)
		{
			// not enough there
				p("[Matching_Current_Pieces] -- not enough pieces ...");
				
				Result = false;
				goto Leave_Function;
		}
		
	// compare string #1
		if (Vcount == 1)
			{
				if (strcmp(p_items[1], V1) != 0)
					{
						Result = false;
						goto Leave_Function;
					}
			}
						
	// compare string #2
		if (Vcount == 2)
			{
				if ( (strcmp(p_items[1], V1) != 0) || (strcmp(p_items[2], V2) != 0))
					{
						Result = false;
						goto Leave_Function;
					}
			}
					
Leave_Function:
	return Result;
}



//
// number in string converted to integer (safe conversion)
//
int char_to_int(char* Incoming_Chars)
{
	int temp = atoi(Incoming_Chars);
	return temp;
}


//
// number in string converted to double (safe conversion)
//
double char_to_double(char* Incoming_Chars)
{
	double temp = atof(Incoming_Chars);
	return temp;
}


//
// Verify that a string is an integer
// 
bool Is_Integer(char* NumString)
{
	// default value -- assume is not an integer
		bool Result = false;
		
	// see the length
		if (strlen(NumString) == 0)
			{
				// too short
					goto Leave_Function;
			}
			
	// walk-thru it
		int i;
		char ch;
		for (i = 0 ; i < strlen(NumString) ; i++)
		{
			ch = NumString[i];
			
			// do not want a period
				if (ch == '.')
				{
					goto Leave_Function;
				}
			
			// only desired characters -- no period in it
				if (strchr("0123456789", ch) == 0)
				{
					// something else in string
						goto Leave_Function;
				}
			
		}
	
	// assume it is a good integer
		Result = true;
		
Leave_Function:
	return Result;

}


//
// Verify that a string is a double
// 
bool Is_Double(char* NumString)
{
	// default value -- assume is not an double
		bool Result = false;
		
	// see the length
		if (strlen(NumString) == 0)
			{
				// too short
					goto Leave_Function;
			}
			
	// walk-thru it
		int 	i;
		char 	ch;
		int	Count_Of_Periods = 0;
		
		for (i = 0 ; i < strlen(NumString) ; i++)
		{
			ch = NumString[i];			
				
			// only desired characters
				if (strchr("0123456789.", ch) == 0)
				{
					// something else in string
						goto Leave_Function;
				}
			
			// see if correct number of periods
				if (ch == '.')
				{
					Count_Of_Periods ++;
				}
		}
		
		// see if too many periods
			if (Count_Of_Periods > 1)
			{
				// too many periods
					goto Leave_Function;
			}
	
	// assume it is a good double
		Result = true;
		
Leave_Function:
	return Result;

}


//
// Test Is_Integer and Is_Double
//
void Testing__IsInteger__Or__IsDouble()
{

	// convert string to decimal
		char myStr[30];
		//int value1 = atoi(myStr);
		//double value2 = atof(myStr);
		
		strcpy(myStr, "123.34");
		int value1 = char_to_int(myStr);
		double value2 = char_to_double(myStr);
		
		
		printf ("number = %d \n", value1);
		printf ("number = %f \n", value2);
	
	strcpy(myStr,"abc");		printf (" {%10s} is good integer %i \n", myStr, Is_Integer(myStr));
	strcpy(myStr,"12");		printf (" {%10s} is good integer %i \n", myStr, Is_Integer(myStr));
	strcpy(myStr,"12.");		printf (" {%10s} is good integer %i \n", myStr, Is_Integer(myStr));
	strcpy(myStr,".12");		printf (" {%10s} is good integer %i \n", myStr, Is_Integer(myStr));
	strcpy(myStr,"12x");		printf (" {%10s} is good integer %i \n", myStr, Is_Integer(myStr));
	
	printf ("\n");
	
	strcpy(myStr,"abc");		printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,"12");		printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,"12.1");	printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,"12.");		printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,".12");		printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,".12x");	printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	strcpy(myStr,"");			printf (" {%10s} is good double %d \n", myStr, Is_Double(myStr));
	
	printf ("\n");
	printf ("\n");
	
}


//
// Test Break_Into_Pieces
//
void Testing__Break_Into_Pieces()
{

	char abc[50];
	strcpy (abc, "CPN 123 1234 xyz");
	
	printf (" string = %s \n", abc);
	
	// Break_Into_Pieces(abc, true);
	Break_Into_Pieces(abc );
	
	if (Matching_Current_Pieces(2,"CPN","123") == true)
		{
			printf("equal !!!\n");
		}
	else
		{
			printf("not equal !!!\n");
		}
	
}


//
// Do_All_Tests
// 
void Tests()
{

	Testing__IsInteger__Or__IsDouble();
	Testing__Break_Into_Pieces();
	
}



// test commands
int	Check_Command_Grammar( char* CommandString )
{
	// variables
		bool	GoodCommand;
		char	arg1[30];
		char	arg2[30];
		
	// reset Server Error Message
		strcpy( Server_Error_Message , "" );
		
		
	// setup
		GoodCommand = true;
		strcpy(ProblemDefined, "");
		
	// protect the Break_Into_Pieces code
		if (strlen(CommandString) < 4)
			{
				goto Bad_Command_Found;
			}
		
	// break command apart into pieces
		Break_Into_Pieces( CommandString );
	
	//********************************************************************************************************		
	// analyze Change Port Command: CPN 123
		if (Matching_Current_Pieces(1,"CPN","") == true)
			{		
				// printf (" arguments count = %i \n", p_last);
				
				// too many arguments
					if (p_last != 2)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
		
						strcpy(ProblemDefined, "Command [CPN] -- argument count is incorrect !!!");
						goto Leave_Function;
					}
				
				// 2nd must be integer
					strcpy(arg2, p_items[2]);
				
					if (Is_Integer(arg2) != true)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [CPN] -- 2nd argument is not an integer !!!");
						goto Leave_Function;
					}
					
				// otherwise -- port to change to
					strcpy(ProblemDefined, "");
					printf("Command [CPN] -- Port to change to %s \n", arg2);
					goto Leave_Function;				
			}
			
	//********************************************************************************************************
	// analyze Accept Port Change:  CPN_ACK 1 or CPN_ACK 0
		if (Matching_Current_Pieces(1,"CPN_ACK","") == true)
			{		
				// printf (" arguments count = %i \n", p_last);
				
				// too many arguments
					if (p_last != 2)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [CPN_ACK] -- argument count is incorrect !!!");
						goto Leave_Function;
					}
				
				// 2nd argument must be 0 or 1
					if	 
						(
							(Matching_Current_Pieces(2,"CPN_ACK","1") != true)
						&&
							(Matching_Current_Pieces(2,"CPN_ACK","0") != true)						
						)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [CPN_ACK] -- 2nd argument is 0 or 1 !!!");
						goto Leave_Function;
					}
					
				// otherwise -- 2nd argument is 0 or 1
					strcpy(ProblemDefined, "");
					printf("Command [CPN_ACK] -- Good because 2nd argument is 0 or 1 ... \n");
					goto Leave_Function;				
			}
		

			
	//********************************************************************************************************
	// analyze Math Instruction Line:  AOSR +-/* numbers list
		if (Matching_Current_Pieces(1,"AOSR","") == true)
			{		
				// variables
					char	MathType;
					
				// printf (" arguments count = %i \n", p_last);
				
				// too many arguments
					if (p_last < 3)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [CPN_ACK] -- argument count is too few !!!");
						goto Leave_Function;
					}
				
				// 2nd argument must be one of these +, -, /, *
					MathType = '?';
					if (Matching_Current_Pieces(2,"AOSR","+") == true) { MathType = '+'; }
					if (Matching_Current_Pieces(2,"AOSR","-") == true) { MathType = '-'; }
					if (Matching_Current_Pieces(2,"AOSR","*") == true) { MathType = '*'; }
					if (Matching_Current_Pieces(2,"AOSR","/") == true) { MathType = '/'; }
					
					// not matched
						if (MathType == '?')						
							{
								strcpy( Server_Error_Message , "ERROR INVOPERATION" );  // give Server Error Message
						
								strcpy(ProblemDefined, "Command [AOSR] -- 2nd argument must be + or - or / or * !!!");
								goto Leave_Function;
							}
										
				// test all remaining arguments that they are decimal
					int 		i;
					char		current_argument[50];
					double	Result;
					double	Current_Decimal;
					
					// calculate result
						Result = 0;
					
					// walk-thru argument list
						for (i = 3; i <= p_last; i++)
							{	
								strcpy(current_argument, p_items[i]);
							
								// see if decimal or not
									if (Is_Double(current_argument) != true)
										{
											// not decimal so leave
												strcpy( Server_Error_Message , "ERROR INVOPERATOR" );  // give Server Error Message
												
												sprintf(ProblemDefined, "Command [AOSR] -- Failed on the %i argument of {%s} ...", i - 2, current_argument);
												goto Leave_Function;		
										}
										
								// since safe, convert to a decimal
									Current_Decimal = char_to_double(current_argument);
									
								// skip math if first argument
									if (i == 3)
										{
											Result = Current_Decimal;
											goto Skip_Math_On_First_Argument;
										}
										
								// if current one is okay, then do result
									if (MathType == '+')	{ Result = Result + Current_Decimal; }
									if (MathType == '-')	{ Result = Result - Current_Decimal; }
									if (MathType == '*')	{ Result = Result * Current_Decimal; }
									if (MathType == '/')	
										{ 
										
											// see if decimal or not
												if (Current_Decimal == 0)
													{
														// can not divide by 0
															strcpy( Server_Error_Message , "ERROR INVOPERATION" );  // give Server Error Message
															
															sprintf(ProblemDefined, "Command [AOSR] -- Failed on the %i argument of {%s} because cannot divide by zero ...", i - 2, current_argument);
															goto Leave_Function;		
													}
													
											// safe to divide
												Result = Result / Current_Decimal; 
										}
																
																				
							Skip_Math_On_First_Argument:
								Result = Result;
							} // end of for-next
						
				// passed all tests for AOSR command
					strcpy(ProblemDefined, "");
					printf("Command [AOSR] -- Good because all tests were passed and result is {%f} ... \n", Result );
					goto Leave_Function;		
					
			}
		
	//********************************************************************************************************


	// analyze ACR -- Answer To Show:  ACR 16.1
		if (Matching_Current_Pieces(1,"ACR","") == true)
			{		
							
				// too many arguments
					if (p_last != 2)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [ACR] -- argument count is incorrect !!!");
						goto Leave_Function;
					}
				
				// 2nd must be double
					strcpy(arg2, p_items[2]);
				
					if (Is_Double(arg2) != true)
					{
						strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
						
						strcpy(ProblemDefined, "Command [ACR] -- 2nd argument is not a double !!!");
						goto Leave_Function;
					}
					
				// otherwise -- ACR command looks good
					strcpy(ProblemDefined, "");
					printf("Command [ACR] -- Looks like math answer is %s \n", arg2);
					goto Leave_Function;				
			}



		
	//********************************************************************************************************

	// must be a bad command -- so indicate that
		Bad_Command_Found:
			strcpy( Server_Error_Message , "ERROR UNRECOGNMESSAGE" );  // give Server Error Message
			
			strcpy(ProblemDefined, "Command [UNDEFINED] -- bad command has come in !!!");
			goto Leave_Function;



	//********************************************************************************************************
Leave_Function:
	// see if good or bad command
		if (strlen(ProblemDefined) == 0)
			{
				GoodCommand = true;
			}
		else
			{
				GoodCommand = false;
			}
		
	// output result (only if error)
		if (GoodCommand == true)
			{
				printf("Good Command {%s}\n", CommandString);
			}
		
		if (GoodCommand == false)
			{
				printf("Command {%s}   Result_Flag {%i}   {%s} \n", CommandString, GoodCommand, ProblemDefined);
			}
		
	// return boolean result
		return GoodCommand;
}


 

int toString(char []);

	//
	// borrowed from http://www.programmingsimplified.com/c/source-code/c-program-convert-string-to-integer-without-using-atoi-function
	// 
	int toString(char a[]) 
	{
	  int c, sign, offset, n;
	 
	  if (a[0] == '-') 
		  {  
		  	// Handle negative integers
				sign = -1;
		  }
	 
	  if (sign == -1) 
		  {  // Set starting position to convert
			 offset = 1;
		  }
	  else 
		  {
			 offset = 0;
		  }
	 
	  n = 0;
	 
	  for (c = offset; a[c] != '\0'; c++) 
		  {
			 n = n * 10 + a[c] - '0';
		  }
	 
	  if (sign == -1) 
		  {
			 n = -n;
		  }
	 
	  return n;
	}








// variable setup
	int 		socket_desc ;
	struct 	sockaddr_in server;
	char 		message[1000] ;
	char 		server_reply[2000];
  
  
  	int		Current_Port_Number ;
  	char		Current_Website_Address[100] ;
  	
  	char		*user_entry;
  	int 		user_entry_length;
  	
  	const		MAX_USER_ENTRY = 50;




char ShowMenuAndGetGoodOption()
{
	// wait until good input
		char	UserInputString[10];
		char	UserInput;
		
		UserInput = '!';
		
	// wait forever
		while (1)
		{	
		
			// ask for user input
				printf("\n\n");
				printf("Client -- Main Menu: \n\n");
				printf("1 - Change Client Port \n");
				printf("2 - Add numbers together \n");
				printf("3 - Send Ad-Hoc message \n");
				printf("4 - Stop Client & Server \n");
				printf("\n");
				printf("Please, select a number ...\n");
		
			// get a number from the user
				Ask_User_For_Input:
					fgets ( UserInputString , MAX_USER_ENTRY, stdin);
					//scanf("%c", UserInputString) ; // UserInput = getchar();
					UserInput = UserInputString[0];
				
			// see if good option; if not, then continue
				if ( (UserInput == '1') || (UserInput == '2') || (UserInput == '3') || (UserInput == '4') )
					{
						break;
					}
				else if (UserInput == '\0')
					{
						goto Ask_User_For_Input;
					}
				else
					{
						printf("\n");
						goto Ask_User_For_Input;
					}
					
		}
		
	// give good option
		return UserInput;

}



void Get_Port_To_Change_To_From_User()
{

	// port to change to
		int	iNewPort;
		char	NewPort[100];		
		
	// clear command
		strcpy(Next_Client_Command, "");
		
	// wait forever 
		while (1)
		{	
		
			// ask for new port
				printf("\n\n");
				printf("What is the new port to change to?\n");
		
			// get port from user
				scanf("%s", NewPort );
				//printf(" Port entered %s \n", NewPort);
				
			// see if good port
				int iNewPort;
				
				iNewPort = char_to_int(NewPort);
				
				if ( (iNewPort > 0) && (iNewPort <= 50000) )
					{
						printf(" A good port was entered between 0 and 50000 ...\n");
						break;
					}
				else
					{
						// bad port entered
							printf("BAD PORT ENTERED:  A port must be between 0 and 50000 ...\n\n");
					}
				
		} // while forever loop ends
		
	// let globally know next port
		New_Port_To_Change_To = char_to_int(NewPort);
		
	// create good command
		sprintf(Next_Client_Command, "CPN %s", NewPort);		
		
		printf("Command created = {%s} \n", Next_Client_Command);
		
}




void Get_List_Of_Numbers_To_Add_From_User()
{
	printf("\n\n");
		
	// ask what math operation
		char	MathOpStr[20];
		char	MathOp;
		
Enter_Good_Math_Operation:
		printf("What math operation '+', '-', '*', '/' ?\n");
		scanf("%s", MathOpStr );
		
		printf("You entered {%s}\n", MathOpStr);
		
		MathOp = MathOpStr[0];
		
		if ((MathOp == '+') || (MathOp == '-') || (MathOp == '*') || (MathOp == '/') )
			{
				printf("\n Good Math operation ... \n\n");
			}
		else
			{
				printf("\n *** Bad Math Operation!!! ... re-enter it!!! \n\n");
				goto Enter_Good_Math_Operation;
			}
		
	// ask how many numbers
		char 	HowManyNumbers[100];
		int	iNumbers;
		
Re_entry__Of_Count:
		printf("How many numbers to add?\n");
		scanf("%s", HowManyNumbers );
		
		iNumbers = char_to_int( HowManyNumbers ); 
		
	// check entry	
		if ( (iNumbers > 0) && (iNumbers <= 5) )
			{
				printf(" Good.  You can only add between 1 and 5 numbers ...\n");
			}
		else
			{
				// bad number entered				
					printf(" You can only add between 1 and 5 numbers ...\n");
					goto Re_entry__Of_Count;
			}
			
	// variable setup
		char	List[10][20];
		char	Entry[20];
		
	// loop for number of times
		int i;
		for (i = 1; i <= iNumbers; i++)
		{
		
	Enter_A_Number:
			printf("Enter number %i :  ", i);
			scanf("%s", Entry );
			
			// verify entry is good
				if (Is_Double(Entry) == true)
					{
						// store it
							strcpy(List[i], Entry);
							
						// praise them
							printf(" ... [Good Entry]\n");
					}
				else
					{
						printf(" ... [Bad Entry]\n");
						goto Enter_A_Number;
					}
					
		}
		
	// build the command after all good entries
		sprintf(Next_Client_Command, "AOSR %c", MathOp);
	
		
		for (i = 1; i <= iNumbers; i++)
		{
			strcat(Next_Client_Command, " ");
			strcat(Next_Client_Command, List[i]);
		}
		
	// show final result	
		printf("Command created = {%s} \n", Next_Client_Command);
		
}




 
int main(int argc , char *argv[])
{

	// setup of variable
		user_entry = (char *) malloc( MAX_USER_ENTRY );
		
	// basic setup
		Current_Port_Number = 8888 ;
		strcpy(Current_Website_Address, "127.0.0.1" );


// ---------------------------------------------------------------------------------------------------------
	// process the arguments for Main
		//Process_Main_Arguments(argc, argv);
		
		printf("\n");
		if (argc > 1)
			{
				printf("CLIENT -- Number of Incoming Arguments (%i) ...\n", argc);
				
				int i;
				for (i = 1; i < argc; i++)
				{
					printf("Argument #%i is %s \n", i, argv[i]);
					
					// see if "-p" for PORT CHANGE
						if ((argv[i][0] == '-') && (argv[i][1] == 'p'))
						{
							printf ("CLIENT -- Port will be changed to %s \n", argv[i+1]);
							
							// change incoming Port_Number to integer
								Current_Port_Number = toString( argv[i+1] );
							
							i++;
						}
						
					// see if "-h" for HOSTNAME change
						if ((argv[i][0] == '-') && (argv[i][1] == 'h'))
						{
							printf ("CLIENT -- Hostname will be changed to %s \n", argv[i+1]);
							
							strcpy(Current_Website_Address, argv[i+1] );
							
							i++;
						}
					
				}
			}
		else
			{
				printf("SERVER -- No Incoming Arguments ...\n");
			}
		printf("\n");		
// ---------------------------------------------------------------------------------------------------------



    //Create socket
Create_Current_Socket: // Current_Port_Number
		socket_desc = socket(AF_INET , SOCK_STREAM , 0);
		if ( socket_desc == -1 )
		{
		    printf("Could not create socket");
		}
		puts("CLIENT -- Socket created");

	// setup the Socket Address 
		server.sin_family = AF_INET;    
		server.sin_addr.s_addr = inet_addr( Current_Website_Address );
		server.sin_port = htons( Current_Port_Number );
 
    //Connect to remote server
		if (connect( socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
		    perror("connect failed. Error");
		    return 1;
		}
   
	// indicate the connection was successfully made	
		printf("CLIENT -- Connection to Server on socket at Port (%d) successful \n\n" , Current_Port_Number );
		printf("CLIENT -- Connection is established ...\n\n");
     
    //keep communicating with server
		while(1)
		{
			// reset variables
				New_Port_To_Change_To = -1;
				
			// clear message
				strcpy(message, "                                                             ");

			// get User Input with a menu until good option
				char UserSelection;
				
				UserSelection = ShowMenuAndGetGoodOption();
				// printf (" good selection %c \n", UserSelection);  exit(0);
				
			// OPTION #1 - Change Client Port 
				if (UserSelection == '1')
				{					
					strcpy(Next_Client_Command, "");
					
					Get_Port_To_Change_To_From_User();
					
					strcpy(message, Next_Client_Command );
					goto Send_Data_To_Server;				
				}
			
			
			// OPTION #2 - Add numbers together 
				if (UserSelection == '2')
				{					
					strcpy(Next_Client_Command, "");
					
					Get_List_Of_Numbers_To_Add_From_User();
					
					strcpy(message, Next_Client_Command );
					goto Send_Data_To_Server;				
				}
			
			
			// OPTION #3 - Send Ad-Hoc message 
				if (UserSelection == '3')
				{					
						// test new message here
							bool TestCommand;
						
							// ask for new command	
					Re_Enter_Next_Command:							
								printf("Command to send >");	
								
							// get user entry
								strcpy(user_entry,"");
								fgets (user_entry, MAX_USER_ENTRY, stdin);
								
							// Remove trailing newline, if there. 
								user_entry_length = strlen(user_entry);
								if ((user_entry_length > 0) && ( user_entry[user_entry_length - 1] == '\n'))
								{
									printf (" *** came in here *** \n");
									user_entry[user_entry_length - 1] = '\0';
								}
							
						// test user_entry as next_command
							strcpy( Next_Client_Command , user_entry );
							
							printf (" New command {%s} \n", Next_Client_Command);
							TestCommand = Check_Command_Grammar( Next_Client_Command );
					
							if (TestCommand == true)
								{
									printf (" ... Good Command entered {%s} \n\n", Next_Client_Command );
								}
							else
								{
									printf (" ... Bad Command entered {%s}, so re-enter ... \n\n", Next_Client_Command );
									goto Re_Enter_Next_Command;
								}
					
					strcpy(message, Next_Client_Command );
					goto Send_Data_To_Server;				
				}
			
			
			
			// OPTION #4 - Stop Client & Server 
				if (UserSelection == '4')
				{
					printf("\n\n *** Stopping both programs !!! *** \n\n");
					strcpy(message,"Q");
					goto Send_Data_To_Server;				
				}
			
				



			// ask for input
				printf("\n\n");
				printf("CLIENT -- Enter message : ");
				
				// get it in, even with spaces and at a particular length
					strcpy(user_entry,"");
					fgets (user_entry, MAX_USER_ENTRY, stdin);
					
				// Remove trailing newline, if there. 
					user_entry_length = strlen(user_entry);
					if ((user_entry_length > 0) && ( user_entry[user_entry_length - 1] == '\n'))
					{
						printf (" *** came in here *** \n");
						user_entry[user_entry_length - 1] = '\0';
					}

					//printf (" 2 -- Entered {%s} \n", user_entry);
					
				// copy it into message
					strcpy(message, user_entry);
					
			// indicate message read
				printf("\nCLIENT -- This message was entered !%s! with a length of (%i) \n", message, (int) strlen(message) );
			
		   // Send some data		   
	Send_Data_To_Server:
				printf(" ");
				
				int Check_For_Error_After_Sending_To_Client;
				Check_For_Error_After_Sending_To_Client = send( socket_desc , message , strlen(message) , 0);

				// check for failure
					if( Check_For_Error_After_Sending_To_Client < 0 )
					{
						puts("CLIENT -- Send failed");
						return 1;
					}
 
			// reset server_reply message
				Get_Reply_From_Server:
					strcpy(server_reply, "                                                             ");

			// Receive a reply from the server
				int Check_For_Error_When_Receiving_From_Server;
				Check_For_Error_When_Receiving_From_Server = recv( socket_desc , server_reply , 2000 , 0) ;

				// check for failure
					if( Check_For_Error_When_Receiving_From_Server < 0 )
					{
						puts("CLIENT -- recv failed");
						break;
					}

			// determine size of the communication
				int read_size;
				read_size = Check_For_Error_When_Receiving_From_Server;

				// fix oversized read
					server_reply[read_size] = '\0';
	
			// output it on client side				
				printf("\n");
		    	printf("CLIENT ---> Server reply :   !%s! ", server_reply);
				printf("\n\n");
				
			// break server message into pieces
				bool GoodCommand;
				
				GoodCommand = Check_Command_Grammar( server_reply );

			// clear out value
				//strcpy( server_reply , "                                                   ");

			// analyze input and see what to do
				if ( message[0] == 'Q' )
					{
						printf("CLIENT -- Client wants to quit so closing socket ...\n");
						break;  // leaving loop
					}
					
			// see if server OKAY port change
				if (New_Port_To_Change_To > 0)
				{
					if (Matching_Current_Pieces(2,"CPN_ACK","1") == true)
						{
							printf ("Will change port to %i \n", New_Port_To_Change_To);
							
							// close current socket
								close( socket_desc );
								
							// set up next socket
								Current_Port_Number = New_Port_To_Change_To;
								goto Create_Current_Socket;
						}
					else if (Matching_Current_Pieces(2,"CPN_ACK","0") == true)
						{
							printf ("WILL NOT change port to %i \n", New_Port_To_Change_To);
						}
				
				}

				 
		}
     

	// close the existing socket
		close( socket_desc );

	// update user
		printf ("\n");
		printf ("CLIENT -- socket closed ... \n");
		printf ("\n\n\n");

	// Main will return to operating system     
	    return 0;
}




/*
    C socket server example
*/
 
// utilize libraries
	#include <stdio.h> 							// printf, sprintf
	#include <string.h>   				 		// strlen, strchr, strcpy
	#include <stdlib.h>							// atoi, atof, malloc

// networking libraries
	#include<sys/socket.h>						// socket
	#include<arpa/inet.h> 						// inet_addr
	#include<unistd.h>    						// write




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
	
// server variables
	int		Port_To_Change_To;
	double	Math_Result_Is;
	
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
					
					Port_To_Change_To = char_to_int(arg2);
					
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
					Math_Result_Is = Result;
					strcpy(ProblemDefined, "");
					printf("Command [AOSR] -- Good because all tests were passed and result is {%8.3f} ... \n", Result );
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
	int toString(char a[]) {
	  int c, sign, offset, n;
	 
	  if (a[0] == '-') {  // Handle negative integers
		 sign = -1;
	  }
	 
	  if (sign == -1) {  // Set starting position to convert
		 offset = 1;
	  }
	  else {
		 offset = 0;
	  }
	 
	  n = 0;
	 
	  for (c = offset; a[c] != '\0'; c++) {
		 n = n * 10 + a[c] - '0';
	  }
	 
	  if (sign == -1) {
		 n = -n;
	  }
	 
	  return n;
	}







// variable setup
	int 		socket_desc ;
	int 		client_sock ;
	int 		c ;
	int 		read_size ;

	struct 	sockaddr_in 	server ;
	struct 	sockaddr_in 	client ;

	char		client_message[2000];


  	int		Current_Port_Number ;
  	char		Current_Website_Address[100] ;




void Process_Main_Arguments()
{
	

}

 
int main(int argc , char *argv[])
{   
	// basic setup
		Current_Port_Number = 8888 ;
		strcpy(Current_Website_Address, "127.0.0.1" );



// ---------------------------------------------------------------------------------------------------------
	// process the arguments for Main
		//Process_Main_Arguments(argc, argv);
		
		printf("\n");
		if (argc > 1)
			{
				printf("SERVER -- Number of Incoming Arguments (%i) ...\n", argc);
				
				int i;
				for (i = 1; i < argc; i++)
				{
					printf("Argument #%i is %s \n", i, argv[i]);
					
					// see if "-p" for PORT CHANGE
						if ((argv[i][0] == '-') && (argv[i][1] == 'p'))
						{
							printf ("SERVER -- Port will be changed to %s \n", argv[i+1]);
							
							// change incoming Port_Number to integer
								Current_Port_Number = toString( argv[i+1] );
								
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

	// See if Port Should Change
		Changing_Port_Through_Code:
			printf(" ");			

	// Create socket
	    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	    if (socket_desc == -1)
	    {
			printf("Could not create socket");
	    }
	    puts("SERVER -- Socket created");
	     
	// Prepare the sockaddr_in structure
	   server.sin_family = AF_INET;
	   server.sin_addr.s_addr = INADDR_ANY;
	   server.sin_port = htons( Current_Port_Number );
     
	// Bind socket
		if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
		{
		    //print the error message
				perror("bind failed. Error");
				return 1;
		}
		printf("SERVER -- bind socket at Port (%d) successful \n" , Current_Port_Number );
     
	// Listen
	   listen(socket_desc , 3);
     
	// Accept and incoming connection
		puts("SERVER -- Waiting for incoming connections...");
		puts("");
		c = sizeof(struct sockaddr_in);
     
	// accept connection from an incoming client
		client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
		if (client_sock < 0)
		{
		    perror("accept failed");
		    return 1;
		}
		puts("SERVER -- Connection accepted from the client ...");
		puts("");
		
	// set port tracker
		Port_To_Change_To = -1;
     
	// Keep looping as receiving messages from client
		while ( 1 ) // loop until BREAK out with a condition
		{

		   // reset message
				strcpy(client_message , "");

			// attempt a communication with client
				read_size = recv(client_sock , client_message , 2000 , 0);
						

			// if has any size then do something
				if (read_size > 0)
					{
						// bound the read size
							client_message[read_size] = '\0';
							
						// drop a character if necessary
							if ( client_message[read_size -1] == ' ')
							{
								read_size --;
								client_message[read_size] = '\0';								
							}

						// output the code to the server window
							printf ("\n");
							printf ("SERVER -- Received !%s! with length of (%i) :  \n", client_message , read_size );
							printf ("\n");
			
						// analyze input and see what to do
							if ( client_message[0] == 'Q' )
								{
									printf("SERVER -- Client wants to quit so closing socket ...\n");
									break;  // leaving loop
								}
												
						// process new message here
							bool GoodCommand;
							
							GoodCommand = Check_Command_Grammar( client_message );
						
						// see what to do						
							if (Matching_Current_Pieces(1,"CPN","") == true)
								{
									if (GoodCommand == true) 
										{
								
											// Good port
												strcpy(client_message, "CPN_ACK 1");
												goto Send_Message_Back_To_Client;
											
										}
									else
										{
											// bad port
												strcpy(client_message, "CPN_ACK 0");
											
											// do not change
												Port_To_Change_To = -1;
												
											// jump out											
												goto Send_Message_Back_To_Client;
										}
								}											
							else if (Matching_Current_Pieces(1,"AOSR","") == true)
								{
									if (GoodCommand == true) 
										{
								
											// Good math instruction set so send result back
												sprintf(client_message, "ACR %f", Math_Result_Is);
												
											// jump out	
												goto Send_Message_Back_To_Client;
											
										}
									else
										{
											// test info											
												printf (" Server_Error_Message = {%s} \n", Server_Error_Message);									
												printf (" ProblemDefined = {%s} \n", ProblemDefined);		
									
											// send back bad instruction found
												strcpy(client_message, Server_Error_Message);
												
											// jump out	
												goto Send_Message_Back_To_Client;
											
										}
								}
							else
								{
									printf (" Server_Error_Message = {%s} \n", Server_Error_Message);									
									printf (" ProblemDefined = {%s} \n", ProblemDefined);									
								}			
							
							

						// Send the message back to client						
							Send_Message_Back_To_Client:
								write(client_sock , client_message , strlen(client_message));
								
						// see if need to change port?
							if ((Matching_Current_Pieces(1,"CPN","") == true) && (Port_To_Change_To != -1))
							{
								// show new port
									printf(" Changing from port %i to port %i ...\n", Current_Port_Number , Port_To_Change_To);
															
								// flush it										
									fflush(stdout);
										
								// close the existing socket
									close( socket_desc );
		
								// do it manually								
									Current_Port_Number = Port_To_Change_To;
									goto Changing_Port_Through_Code;
			
							}
	
					}
				else
					{

						// determine problem with read    
							if(read_size == 0)
							{
								puts("SERVER SIDE -- Client disconnected");
								fflush(stdout);
							}
							else if(read_size == -1)
							{
								perror("SERVER SIDE -- recv failed");
							}
					 
						// read was bad because read_size = 0
							break;  // leave while loop
					}

		}


	// close the existing socket
		close( socket_desc );

	// update user
		printf ("\n");
		printf ("SERVER -- socket closed ... \n");
		printf ("\n\n\n");

	// Main will return to operating system     
		return 0;

}



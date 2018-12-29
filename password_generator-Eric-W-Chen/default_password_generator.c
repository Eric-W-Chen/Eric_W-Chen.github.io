//Eric Chen
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

bool isStrongPassword(const char*, const char*);
bool isStrongDefaultPassword(const char*, const char*);
bool substring(const char*, const char*);
char* generateDefaultPassword(char*, const char*);

int main()
{

	char password[80];
	char username[80];
	char answer;


	while(1) //infinite loop
		{

		printf("Please enter the username: \n");

		scanf("%s", username);

		printf("Generating a default password...\n");

		generateDefaultPassword(password, username);

		printf("Generated default password: %s \n", password);

		printf("Would you like to change the default password to one of your own? (y/n) \n");

		while(1) //loop until the user decides which password to use
		{

			scanf("%c", &answer);

			if((answer == 'y') || (answer == 'Y')) //if user wants to use their own, break out of loop
			{
				break;//break out of infinite loop
			}

			if((answer == 'n') || (answer == 'N')) //if user likes generated password, exits program
			{
				printf("Thanks for using the generated password!\n");
				return 0;
			}

		}

		printf("Please enter the password: \n");
		scanf("%s", password);

		if(isStrongPassword(username, password) > 0) //calling procedure that validates password
			{	
			printf("Strong password! \n");
			return 0;
			}


		}

	return 0;

	
}

bool isStrongPassword(const char* username, const char* password)
{
	int pass_len;
	int upper;
	int lower;
	int numeric;
	char chr;
	int i;
	int error;
	int user_len;
	int char_len;
	int max_len;

	user_len = strlen(username);
	pass_len = strlen(password);

	if(user_len == 0)
	{
		printf("Please enter a username!\n");
		return 0;
	}

	if(pass_len < 8)
	{	
		printf("The password is too short! Please try again. \n");
		return 0; //an error
	}
	
	upper = 0;
	lower = 0;
	numeric = 0;
	error = 0;
	char_len = 0; //number of currently consecutive letters
	max_len = 0; //maximum number of consecutive letters found so far

	for(i = 0; i < pass_len; i++)
	{
	chr = password[i];

		if(((chr >= ('a')) && (chr <= ('z'))))
		{
		char_len++;//found another consecutive character	
		lower++;
		}
		else if(((chr >= ('A')) && (chr <= ('Z'))))
		{
		char_len++;//found another consecutive character
		upper++;
		}
		else if(((chr >= ('0')) && (chr <= ('9'))))
		{
		//any current consecutive character is aborted, so save it if it's the max
		if(char_len > max_len) //if current sequence is longer than any previously
		{
			max_len = char_len;//save this as the new max so far
			char_len = 0; //if we found a numeric, we have to start over
		}
		numeric++;
		}
		else
		{
		error = 1; //sets a error flag
		break; //break out of the for loop
		}
	}

	if(char_len > max_len) //if we are still in the middle of some currently consecutive characters
	{
		max_len = char_len;//save this as the new max so far
	}

	if(error > 0) 
	{
		printf("No special characters! Try again!");
		return 0;
	}
	if(upper < 1)
	{
		printf("Needs at least 1 upper case character! Try again!\n");
		return 0;
	}
	if(lower < 1)
	{
		printf("Needs at least 1 lower case character! Try again!\n");
		return 0;
	}	
	if(numeric < 1)
	{
		printf("Needs at least 1 numeric character! Try again!\n");
		return 0;
	}	
	if(max_len < 4)
	{
		printf("Needs at least 4 letters in a row! Try again!\n");
		return 0;
	}

	if((substring(username, password)) > 0)
	{
		printf("The password cannot contain the username! Try again!\n");
		return 0;
	}

	return 1; //success

}

bool isStrongDefaultPassword(const char* username, const char* password)
{
	int pass_len;
	int upper;
	int lower;
	int numeric;
	char chr;
	int i;
	int error;
	int user_len;

	user_len = strlen(username);
	pass_len = strlen(password);

	if(user_len == 0)
	{
		printf("Please enter a username!\n");
		return 0;
	}

	if(pass_len < 8)
	{	
		printf("The password is too short! Please try again. \n");
		return 0; //an error
	}
	
	upper = 0;
	lower = 0;
	numeric = 0;
	error = 0;
	

	for(i = 0; i < pass_len; i++)
	{
	chr = password[i];

		if(((chr >= ('a')) && (chr <= ('z'))))
		{
		lower++;
		}
		else if(((chr >= ('A')) && (chr <= ('Z'))))
		{
		upper++;
		}
		else if(((chr >= ('0')) && (chr <= ('9'))))
		{ 
		numeric++;
		}
		else
		{
		error = 1; //sets a error flag
		break; //break out of the for loop
		}
	}


	if(error > 0) 
	{
		printf("No special characters! Try again!\n");
		return 0;
	}
	if(upper < 1)
	{
		printf("Needs at least 1 upper case character! Try again!\n");
		return 0;
	}
	if(lower < 1)
	{
		printf("Needs at least 1 lower case character! Try again!\n");
		return 0;
	}	
	if(numeric < 1)
	{
		printf("Needs at least 1 numeric character! Try again!\n");
		return 0;
	}	

	if((substring(username, password)) > 0)
	{
		printf("The password cannot contain the username! Try again!\n");
		return 0;
	}

	return 1; //success

}




bool substring(const char* mask, const char* target)
{
	int mask_len;
	int target_len;
	int i;
	int j;
	bool found;
	char char1;
	char char2;


	mask_len = strlen(mask);
	target_len = strlen(target);

	if((mask_len == 0) || (target_len == 0) || (mask_len > target_len)) //makes sure there's strings in user and pass
	{
		return 0;
	}

	for (i = 0 ; (i < target_len) ; i++) //iterates to look for a match starting from index 0 in the target
	{
		if((i + mask_len) >= (target_len)) //if mask is too long for what's left of the target, then stop
		{
			return false;
		} 

		found = true; //assume there is going to be a match

		for (j = 0; (j < mask_len) ; j++) //looking for a match
		{
			char1 = mask[j];
			char2 = target[i+j]; //i+j defines which index to define target to

			if ((char1 >= 'a') && (char1 <= 'z')) //if it's lower case
			{
				char1 = char1 - 0x20; //make it upper case.
			}

			if ((char2 >= 'a') && (char2 <= 'z'))
			{
				char2 = char2 - 0x20;
			}


			if (char1 != char2) //makes sure username not in password. 
			{
				found = false; //there is not match
				break;
			} 

		}
		if (found == true) //there is a match
		{
			return true;
		}

	}
	return false; //didn't find anything
}

char* generateDefaultPassword(char* default_password, const char* username)
{

int i;
int len;
int isLetter;
int isUpper;
int character;

srand(time(0));

while(1) //generates passwords until we get a valid one
	{
	len = (rand());
	len = ((len % 8) + 8); //generates length of random characters between 8-15

	for(i = 0 ; i < len ; i++)
	{
		isLetter = rand();
		isLetter = (isLetter % 2); //isChar is 0 or 1. 0 for letter 1 for number
		isUpper = rand(); //whether capital letter or not
		isUpper = (isUpper % 2); //isCap 0 = no isCap 1 = yes

		if(isLetter == 1)//if this is a number
		{
			character = rand();
			character = ((character % 10) + '0'); // goes from hex 30-39. makes only 10 possibilities
		}
		else if(isUpper == 0)//if this is to be a lower case character
		{
			character = rand();
			character = ((character % 26) + 'a'); //goes from hex 61 - 7A. 26 possibilities a-z
		}
		else //character is to be upper case
		{
			character = rand();
			character = ((character % 26) + 'A');
		}

		default_password[i] = character;

	}

	default_password[len] = 0; //null terminated the string so now its an actual string

	if(isStrongDefaultPassword(username, default_password) > 0) //this is good
	{
		return default_password; //the password is good
	}

	}


}

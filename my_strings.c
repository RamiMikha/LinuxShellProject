#include "my_strings.h"

int tokenize(char *user_input, char *tokens[MAX_TOKENS+1]){
  int numTokens = 0;
  char *token_start = NULL; //pointer to the start of the token

  for (int i = 0; user_input[i] != '\0'; i++){
    if (user_input[i] != ' '){
      if (token_start == NULL){
	token_start = &user_input[i]; //if not white space and token_start is not currently poiting to a token start of a new token
      }
    }else {
      //If we find white space and token_start is pointing to a token, end the token
      if(token_start != NULL){
	int token_length = &user_input[i] - token_start;
	tokens[numTokens] = (char *)my_malloc(token_length + 1);
	if(tokens[numTokens] == NULL) {
	  return -1; //memory allocation failed
	}
	for (int j = 0; j < token_length; j++) {
	  tokens[numTokens][j] = token_start[j]; //copy the token into the allocated memory
	}
	tokens[numTokens][token_length] = '\0'; //Null terminate the token
	numTokens++;
	token_start = NULL;
      }
    }
  }

  /*Handling the case if we reach the end of input without encountering a whitespace*/
  if (token_start != NULL) {
    int token_length = &user_input[my_strlen(user_input)] - token_start;
    tokens[numTokens] = (char *)my_malloc(token_length + 1);
    if (tokens[numTokens] == NULL){
      return -1; //memory allocation failed
    }
    for (int j = 0; j < token_length; j++){
      tokens[numTokens][j] = token_start[j];
    }
    tokens[numTokens][token_length] = '\0';
    numTokens++;

  }

  tokens[numTokens] = NULL;
  return numTokens;
}


 /*
 Function Name: my_strcmp
 Purpose: compares the contents of two strings
 Details: The function returns a value of 0 if the strings are 0, and returns
          any other integer value if they aren't.


       Input: str1 - the first string that is to be compared to of type char *
              str2 - the second string that is to be compared to of type char *
       Output: outputs the result of the comparison, where 0 means the strings are the same,
               and any other value means that the strings are different
 */
 int my_strcmp(const char *str1, const char *str2){
   while (*str1 && *str2) {
     if (*str1 != *str2) {
       /*If the characters are different return the difference*/
       return *str1 - *str2;

     }

     /*Increment to the next set of characters*/
     str1++;
     str2++;
   }

   /*If we reach this point in means there is no difference in the strings or one string is longer than the other so we compare one final time*/
   return *str1 - *str2;
 }




 /*
 Function Name: my_strcpy
 Purpose: to make a copy of the original string
 Details:
        Input: dest - where the original string is to be copied over to, of type char *
               src - the original string that is to be copied, of type char *
 */
 void my_strcpy(char *dest, char *src){
   while (*src){
     *dest++ = *src++;
   }

   *dest = '\0';
 }




 /*
 Function Name: my_strlen
 Purpose: to determine the length of a given string
 Details:
        Input: str - the string that is to be measured
 */
 int my_strlen(const char *str){
   int length = 0;
   while (str[length] != '\0'){
     length++;
   }
   return length;
 }

#include<stdio.h>
#include<curses.h>
#include<string.h>
#include <stdlib.h>

int PROGADDR;

typedef struct textRecordStruct{
  char textString[100]; 
  int startingAddress;
  int modifiedStartingAddress;
} textRecordStruct;

typedef struct pass2tracker{
  int controlSectionNum;
  textRecordStruct textRecord[10]; //max 10 text records (T's) for one control section
}pass2tracker;

pass2tracker p2t[20]; //max 20 control sections



int findTextRecordIndex(int address, int count) {
   int ans;
        for (int j = 0; j < 10; j++) {
          if(p2t[count].textRecord[j].startingAddress!=0){
            if (address >= p2t[count].textRecord[j].startingAddress) {
                ans=j;
                 // Calculate the index based on controlSectionNum and textRecord index
            }
            }
        }
        
    
    return ans; // Return -1 if address not found
}




//////////////////////////////////////////////////
typedef struct external_symbol {
    char name[20]; // External symbol name
    int address;   // Address corresponding to the external symbol
} ExternalSymbol;


typedef struct estab //store name and address of each external symbol in the control section
{
  char csname[10]; //control section name
  ExternalSymbol extsym[10]; //external symbol max no.=10
  int address; //start address of that control section
  int length; //length of that control section
} estab;

estab es[20]; //20 control sections

///////////////////////////////////////////////////




int searchestab(char* str){
    for(int i=0;i<sizeof(es)/sizeof(es[0]);i++){
        if(strcmp(str,es[i].csname)==0){
            return 1;
        }
    }

    return 0;
}


int searchestabForSymbol(char* str){
  for(int i=0;i<sizeof(es)/sizeof(es[0]);i++){
    for(int j=0;j<10;j++){
      if(strcmp(str,es[i].extsym[j].name)==0){
      return 1;
      }
    }
  }
  
  return 0;
  
}

int getAddressOfSymbol(char *symbolName) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (strcmp(es[i].extsym[j].name, symbolName) == 0) {
                return es[i].extsym[j].address; // Return the address if symbolName is found
            }
        }
    }
    
    
    //it comes here means it's returning -1
   // return -1; // Return -1 if symbolName is not found
   
   for(int i=0;i<20;i++){
    if(strcmp(es[i].csname,symbolName)==0){
      return es[i].address;
    }
   }
   
}




void appendZeros(char* inputString, int targetLength) {
    int inputLength = strlen(inputString);
    if (inputLength >= targetLength) {
        return;  // If the string length is already equal or greater than the target length, return as is
    }

    int zerosToAppend = targetLength - inputLength;
    memmove(inputString + zerosToAppend, inputString, inputLength + 1); // Shift the original string to make space for zeros
    memset(inputString, '0', zerosToAppend); // Fill the beginning of the string with zeros
}



/*
void insertIntoSYMTAB(char* label,int locctr){
    strcpy(SYMTAB[SYMTABCOUNTER].label,label);
    SYMTAB[SYMTABCOUNTER].locctr=locctr;
}*/










void itoa(int n, char* buffer, int radix) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly, otherwise empty string is printed
    if (n == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (n < 0 && radix == 10) {
        isNegative = 1;
        n = -n;
    }

    // Process individual digits
    while (n != 0) {
        int rem = n % radix;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        n = n / radix;
    }

    // If number is negative, append '-'
    if (isNegative)
        buffer[i++] = '-';

    buffer[i] = '\0'; // Append string terminator

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    return;
}




// Function to count the number of words in a string
int countWords(char *str) {
    int count = 0;
    bool inWord = false;

    // Iterate through each character in the string
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if the current character is a space or a newline
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t') {
            // If we were in a word, increment the count
            if (inWord) {
                count++;
                inWord = false; // Set inWord to false until the next word is found
            }
        } else {
            // We are in a word
            inWord = true;
        }
    }

    // If the last word is not followed by a space, increment the count
    if (inWord) {
        count++;
    }

    return count;
}


void splitString(char* input, char words[][50]) {
    int i = 0, j = 0, word_count = 0;
    while (input[i] != '\0') {
        // Skip leading spaces
        while (input[i] == ' ') {
            i++;
        }
        // Copy characters until a space or end of string is encountered
        while (input[i] != ' ' && input[i] != '\0') {
            words[word_count][j++] = input[i++];
        }
        if (j > 0) {
            words[word_count++][j] = '\0'; // Null-terminate the word
            j = 0; // Reset index for the next word
        }
    }
    
}


void clearWordsArray(char words[][50], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            words[i][j] = '\0'; // Set each character to null
        }
    }
}


// Function to process the input string
int processString(char *input, char *output) {
    // Check if the first character is '+'
    if (input[0] == '+') {
        // Copy the substring starting from the second character
        strcpy(output, input + 1);
        return 1; // Return 1 if the first character is '+'
    } else if (input[0] == '-') {
        // Copy the substring starting from the second character
        strcpy(output, input + 1);
        return 0; // Return 0 if the first character is '-'
    } else {
        // Invalid input, neither '+' nor '-'
        printf("Invalid input format.\n");
        return -1; // Return -1 for invalid input
    }
}

void replaceSubstring(char *input, int low, int high, char *replace) {
    int length = strlen(input);
    int replaceLength = strlen(replace);
    int i, j;

    // Check for valid indices
    if (low < 0 || low > length || high < 0 || high > length || low > high) {
        printf("Invalid indices.\n");
        return;
    }

    // Calculate the length of the new string after replacement
    int newLength = length - (high - low + 1) + replaceLength;

    // Allocate memory for the new string
    char *newString = (char *)malloc((newLength + 1) * sizeof(char));

    // Copy characters before the low index
    for (i = 0; i < low; i++) {
        newString[i] = input[i];
    }

    // Copy the replacement string
    for (j = 0; j < replaceLength; j++) {
        newString[i++] = replace[j];
    }

    // Copy characters after the high index
    for (j = high + 1; j < length; j++) {
        newString[i++] = input[j];
    }

    // Null-terminate the new string
    newString[i] = '\0';

    // Update the input string with the new string
    strcpy(input, newString);

    // Free the dynamically allocated memory
    free(newString);
}


char* extractSubstring(char *input, int low, int high) {
    int length = strlen(input);
    
    // Check for valid indices
    if (low < 0 || low >= length || high < 0 || high > length || low > high) {
      printf("\n%s\n",input);
        printf("length:%d",length);
        printf("Invalid indices.hihihi\n");
        
        return NULL;
    }
    
    // Calculate the length of the substring
    int subLength = high - low + 1;
    
    // Allocate memory for the substring
    char *substring = (char*)malloc((subLength + 1) * sizeof(char));
    if (substring == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }
    
    // Copy the substring from the input string
    for (int i = 0; i < subLength; i++) {
        substring[i] = input[low + i];
    }
    
    // Null-terminate the substring
    substring[subLength] = '\0';
    
    return substring;
}









char* decimalToHex(char* str) {
  int decimal=atoi(str);
  char* hex=(char*)malloc(7);
  sprintf(hex, "%06X", decimal);  // Pad with leading zeros to ensure 6 characters
  return hex;
}




// Function to convert hex to decimal
int HexToDecimal(char hex[]) {
    int len = strlen(hex);
    int decimal = 0;
    int base = 1;

    for (int i = len - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            decimal += (hex[i] - '0') * base;
        } else if (hex[i] >= 'A' && hex[i] <= 'F') {
            decimal += (hex[i] - 'A' + 10) * base;
        } else if (hex[i] >= 'a' && hex[i] <= 'f') {
            decimal += (hex[i] - 'a' + 10) * base;
        }
        base *= 16;
    }

    return decimal;
}

// Function to convert decimal to hex
char* DecimalToHex(int decimal) {
    char* hex;
    int temp = decimal;
    int len = 0;

    // Count the number of digits in the decimal number
    while (temp > 0) {
        temp /= 16;
        len++;
    }

    hex = (char*)malloc((len + 1) * sizeof(char));
    hex[len] = '\0';

    // Convert decimal to hex
    for (int i = len - 1; i >= 0; i--) {
        int remainder = decimal % 16;
        if (remainder < 10) {
            hex[i] = remainder + '0';
        } else {
            hex[i] = remainder - 10 + 'A';
        }
        decimal /= 16;
    }

    return hex;
}











void pass1(FILE* inputFile, FILE* outputFile) {
    
    int count=0;
    ////
    char line[100];
    char str0[10],str1[10],str2[10],str3[10]; //for header record
    char words[50][50];
    int CSLTH; //controlSectionLength
    int CSADDR;
    //int PROGADDR; //where to load program-------Defined globally!
    ////
    
    
    
    
    

    

    printf("\n\nEnter the address where the program has to be loaded : ");
    scanf("%x", &PROGADDR);
    CSADDR = PROGADDR;

    fprintf(outputFile, "CS_NAME\tEXT_SYM_NAME\tADDRESS\tLENGTH\n");
    fprintf(outputFile, "--------------------------------------\n");

    while (fgets(line,sizeof(line),inputFile) != NULL) {
    
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
     
     printf("ReadLine:%s\n",line);
     
        
        if(line[0]=='*'){//one control section done!
          count++;
          
        }
        
       

        else if (line[0]=='H') {
          
            if(countWords(line)!=4){
              printf("\nSome Error in the Header record of Control Secton: %d\n",count);
            }
            else{
              sscanf(line,"%s %s %s %s\n",&str0,&str1,&str2,&str3);
              
              printf("%s\n",str0);
              printf("%s\n",str1);
              printf("%s\n",str2);
              printf("%s\n",str3);
              
              
              
              if(searchestab(str1)==1){
                printf("\nDUPLICATE CONTROL SECTION NAME FOUND!\n");
              }
              else{
              //main
              CSLTH=HexToDecimal(str3);
              //////
              strcpy(es[count].csname,str1); //copy control section name
              es[count].address=CSADDR; //copy control section address
              es[count].length=HexToDecimal(str3); //copy control section length
              
              
              }
              
              
            }
            
        } 
        
        else if (line[0]=='D') {
          
            int numWords=countWords(line);
            printf("\nEntered D\n");
            
            
            
                
                if(numWords%2==0){
                  
                  printf("\nSome error in Define Record of Control Section: %d\n",count);
                }
                
                
                else{
                
                
                  splitString(line,words);
                  
                  for(int i=0;i<countWords(line);i++){
                  
                    if(i%2==1){//symbols
                      if(searchestabForSymbol(words[i])==1){
                        printf("\nDuplicate Symbol");
                      }
                      else{
                        strcpy(es[count].extsym[(i-1)/2].name,words[i]);
                        es[count].extsym[(i-1)/2].address=CSADDR+HexToDecimal(words[i+1]);
                          }
                    }
                  }
                  
                  
                }
                
                
                

            }
            
       
        
        else if (line[0]== 'T') {
            //Do NOTHING
            
        }
        
        
        else if (line[0]=='E'){
            //do something and then
            printf("\n\n\nEND OF CONTROL SECTION: %d REACHED\n\n\n",count);
            CSADDR=CSADDR+CSLTH;
           
            }
        
        
         
    
    
    }
    
    for(int i=0;i<sizeof(es)/sizeof(es[0]);i++){
      if(es[i].length!=0){    
      fprintf(outputFile,"\n%s\t\t%x\t%x",es[i].csname,es[i].address,es[i].length);}
      for(int j=0;j<sizeof(es[i].extsym)/sizeof(es[i].extsym[0]);j++){
      if(es[i].extsym[j].address!=0){
        fprintf(outputFile,"\n\t%s\t%x",es[i].extsym[j].name,es[i].extsym[j].address);
        }
      }
    }
    
    

}














void pass2(FILE* inputFile, FILE* outputFile) {

  printf("\n\n\n\n--------------------BEGIN PASS 2-----------------------\n\n\n\n");
    
    int count=0;
    int obtabCounter=0;
    ////
    char line[100];
    char str0[10],str1[10],str2[10],str3[10]; //for header record
    char words[50][50];
    int CSLTH; //controlSectionLength
    int CSADDR;
    int EXECADDR; //execution address
   // int PROGADDR; //where to load program
   
   int stringIndex=0;
   
   
   int textRecordIndex;
    ////
    
    CSADDR=PROGADDR;
    EXECADDR=PROGADDR;
    
    
    

    
  
  

    while (fgets(line,sizeof(line),inputFile) != NULL) {
    
    
    
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
     
        printf("ReadLine (PASS-2):%s\n",line);
     
        
        if(line[0]=='*'){//one control section done!
          count++;
          
        }
        
       

        else if (line[0]=='H') {
            
              sscanf(line,"%s %s %s %s\n",&str0,&str1,&str2,&str3);
              
              printf("%s\n",str0);
              printf("%s\n",str1);
              printf("%s\n",str2);
              printf("%s\n",str3);

           
              //main
              CSLTH=HexToDecimal(str3);
              //////
              

            
        } 
        
        else if (line[0]=='D') {
          
           
                //DO NOTHING
                
                

            }
            
       
        
        else if (line[0]== 'T') {
            clearWordsArray(words,50,50);
            splitString(line,words);
            
            
            char concatenatedString[100]="";
            
            int startaddr=HexToDecimal(words[1]);
            
            for(int i=3;i<countWords(line);i++){ //to form concatenated string
              strcat(concatenatedString,words[i]);
            }
            
            strcpy(p2t[count].textRecord[stringIndex].textString,concatenatedString);
            p2t[count].textRecord[stringIndex].startingAddress=startaddr;
            p2t[count].controlSectionNum=count;
            
            
            p2t[count].textRecord[stringIndex].modifiedStartingAddress=startaddr+CSADDR;
            
            
            
            
            
            
            //last written index
            stringIndex++;
        }
        
        else if(line[0]=='M'){
        
        //reached M record which means all the text records are loaded into the memory for one control section
        char strM1[10];
        char strM2[10];
        char strM3[10];
        char strM4[10];
        
        char symbol[10];
        
        
        sscanf(line,"%s %s %s %s\n",&strM1,&strM2,&strM3,&strM4);
        
        int startingFrom=HexToDecimal(strM2);
        int numberOfBytes=HexToDecimal(strM3); //halfBytes
        int operation=processString(strM4,symbol); //returns 1 if addition
        
        if(searchestabForSymbol(symbol) || searchestab(symbol)){
          
          textRecordIndex=findTextRecordIndex(startingFrom,count);
          printf("textRecordIndex:%d\n",textRecordIndex);
          int low= (startingFrom- p2t[count].textRecord[textRecordIndex].startingAddress)*2+(numberOfBytes%2);
          int high= low+(numberOfBytes)-1;
          
          printf("low:%d high:%d\n",low,high);
          
          char* stringBetween=extractSubstring(p2t[count].textRecord[textRecordIndex].textString,low,high);
          
          printf("Extracted Substring:%s\n",stringBetween);
          
        
          if(operation==1){ //addition
            
            char* newString= DecimalToHex(HexToDecimal(stringBetween)+getAddressOfSymbol(symbol));
            
            appendZeros(newString,numberOfBytes);
            
            printf("Substring to replace:%s\n\n",newString);
            replaceSubstring(p2t[count].textRecord[textRecordIndex].textString,low,high,newString);
            
            
          }
          else if(operation==0){ //subtraction
              
              char* newString= DecimalToHex(HexToDecimal(stringBetween)-getAddressOfSymbol(symbol));
              
              appendZeros(newString,numberOfBytes);
              
              printf("Substring to replace:%s\n\n",newString);
              replaceSubstring(p2t[count].textRecord[textRecordIndex].textString,low,high,newString);
              
          }
        }
        else{ //if symbol not found
          printf("\nsymbol not found error (pass2)\n");
        }
        
            
            
            
            
            
            
            
            
            
            
            
            
            
            
        }
        
        
        else if (line[0]=='E'){
            //do something and then
            count++;
            stringIndex=0;
            
            char endStr1[10];
            char endAddress[10];
            sscanf(line,"%s %s\n",endStr1,endAddress);
            
            EXECADDR= CSADDR+ HexToDecimal(endAddress);
            CSADDR=CSADDR+CSLTH;
            
            }
        
        
         
    
    
    } //end while read!
    
    
    
    
    printf("\n\n---------------PASS 2 DONE!-----------\n\n");
    
    fprintf(outputFile,"------------------------------------------\n");
    fprintf(outputFile,"ADDRESS\tOBJECT-CODES\n");
    fprintf(outputFile,"------------------------------------------\n");
    for(int i=0;i<20;i++){
      for(int j=0;j<10;j++){
        if(p2t[i].textRecord[j].modifiedStartingAddress!=0){
         fprintf(outputFile,"%x\t%s\n",p2t[i].textRecord[j].modifiedStartingAddress,p2t[i].textRecord[j].textString);
         }
      }
    }
    
    
    
    
    
    
    

}








int main(){

FILE *inputFile=fopen("LINK1IN.DAT","r");
FILE *outputFile=fopen("output2.txt","w");


  pass1(inputFile,outputFile);
  
  fclose(inputFile);
  fclose(outputFile);

FILE *inputFile2=fopen("LINK1IN.DAT","r");
FILE *outputFile2=fopen("output3.txt","w");

  pass2(inputFile2,outputFile2);
  
  fclose(inputFile2);
  fclose(outputFile2);
  return 0;
}


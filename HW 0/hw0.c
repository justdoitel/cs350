/* THIS CODE WAS MY OWN WORK, IT WAS WRITTEN WITHOUT CONSULTING ANY
SOURCES OUTSIDE OF THOSE APPROVED BY THE INSTRUCTOR. EL YIRDAW */

#include <stdio.h>
#include <stdlib.h>

//returns 1 if the number is prime. else, returns 0.
int isprime(long x){
    if(x<=1) return 0; //if a number is 1 or less, it's not prime.
    if(x<=3) return 1; //2 and 3 are prime
    if(x%2==0) return 0; //even numbers above 2 aren't prime
    for(int i=3;i<=x/2;i=i+2){ //this loops through every number starting at 3 and ending at x/2 (the max number we need to check if the number is above 4)
        if(x%i==0) return 0;
    }
    return 1;
}

//this method prints out the first x numbers of prime numbers (Separated by comma)
void prime(int x){
    int i=0;
    long curr=2;//we start with 2 since we know that's the first prime number.
    while(i!=x){ //we check that we haven't reached the number of prime numbers we need
        if(isprime(curr)){ //this checks if the number is prime and prints it according to the comma rule (then increments the counter of numbers of prime)
            if(i==0) printf("%ld",curr); 
            else printf(", %ld",curr);
            i++;
        }
        curr++; //for each iteration (whether it's prime or not, we need to move on to the next number)
    }
    printf("\n");
}

//this method prints the original string and the resulting string after the vowels are stripped from it
void rem (const char *s){
    int constcounter=0; //used to count the number of consonants.
    for (int i=0; s[i]!=0; i++){
        if(!(s[i]=='a'|s[i]=='e'|s[i]=='i'|s[i]=='o'|s[i]=='u'|s[i]=='A'|s[i]=='E'|s[i]=='I'|s[i]=='O'|s[i]=='U')) constcounter++; //increments everytime a consonant is found
    }
    char newstring [constcounter+1]; //we need to create the result string (along a space for nul)
    int index=0; //this is the first index of the counter
    for (int i=0; s[i]!=0; i++){ //goes through the original array and copy the non-vowel charachters to the new string
        if(!(s[i]=='a'|s[i]=='e'|s[i]=='i'|s[i]=='o'|s[i]=='u'|s[i]=='A'|s[i]=='E'|s[i]=='I'|s[i]=='O'|s[i]=='U')) newstring[index++]=s[i];
    }
    
    newstring[constcounter]=0; //the end of the new string is marked with null
    
    //both strings are printed
    printf("%s\n",s);
    printf("%s\n",newstring);

}

//This method takes in a long number and checks and prints whether or not it's perfect
void perfect(long num){
    if(num<=0) {//0 and negative numbers aren't perfect
        printf("%ld is not perfect\n",num);
        return;
    }
    long sum=0; //used to keep the sum of factors. 
    for (long i=1;i<num;i++){ //sums every factor of num from 1 to n (exclusive) into sum
        if(num%i==0) sum=sum+i;
    }

    //this prints whether or not the number is perfect.
    if(sum==num) printf("%ld is perfect\n",num);
    else printf("%ld is not perfect\n",num);
}

//This method takes in a long number and checks and prints whether or not it's prime. If it's not prime, it returns the closest prime number to it (with lower numbers preffered)
void check(long x){
    if (isprime(x)){ 
        printf("The number %ld is prime.\n",x); 
        return;
    }
    long w=x;
    long y=x;
    while(1){ //so long as the number is not a prime, we check the numbers one far off from the original number than the ones we've already checked
        if(isprime(--w)){ //decrements it to check the number one down and we're done if the number is prime
            printf("The nearest prime number to %ld is %ld.\n",x,w);
            return;
        }
        if(isprime(++y)){ //increments it to check the number one up and we're done if the number is prime
            printf("The nearest prime number to %ld is %ld.\n",x,y);
            return;
        }
    }
}

//This method asks the user to input a positive integer (guaranteed per class instruction to be 20 digits max) and, per the rules of check(), prints if it's a prime and, if not, the closest prime to it
void nearest(void){ 
    char input [21]; //used to store the input (20 numbers + 1 for nul)
    char c; //used to hold the current charachter
    int i=0; //used to index the char array
    printf("Enter a positive integer: "); 
    while((c = getchar())!=EOF) { //this only goes on until EOF isn't detected
        if(c=='\n'){ //if a newline is detected, that means the user has fully inputted a number
            input[i]=0; //we end the string
            if(i==0) printf("Nothing was entered.\n"); //this means they pressed enter without entering anything.
            else check(atol(input)); //this sends the input to the helper method to do the actual work
            i=0; //once we're done, we can empty out the string and rewrite it
            printf("Enter a positive integer: "); //once done with computing, we ask for an input again.
        }
        else{
            input[i++]=c; //if it's not a newline, we just continue filling the char array
        }
    }
    printf("\n"); //end on a newline  
}


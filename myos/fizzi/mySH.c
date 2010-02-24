#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "myOS.h"
#include "mySH.h"
#include "myKernel.h"

char verbose;

typedef struct var list;

struct var {
    char *value;
    char *name;
    list *next;
} *head, *prompt;

void shhelp(){
    puts("Shell Parameters:");
    puts("-H: Show this information");
    puts("-V: Run shell in verbose mode");
}

void shversion(){
    puts("Shell for Mios, linux for feline companions, version 0.002");
    puts("By Jasmin \"Fizzi\" Laferriere");
}

void help(){
    puts("EXIT\t\tExit shell and shut down Mios");
    puts("VERBOSE\t\tVERBOSE ON/OFF to enable or disable verbose mode");
    puts("VER\t\tDisplay shell version");
    puts("SET\t\tSET VAR VALUE Creates a variable in shell memory with name VAR and value VALUE");
    puts("GET\t\tGET VAR Outputs the value of variable VAR");
    puts("PROMPT\t\tPROMPT VALUE will change prompt to string in VALUE");
    puts("HELP\t\tDisplays this information");
    puts("CLR\t\tClears the screen");
    puts("SCRIPT\t\tSCRIPT FILENAME Runs script with name FILENAME");
}

void clear(){
    int i;

    //1000 lines
    for(i = 0; i<10; i++){
        printf("\n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n\
                \n\n\n\n\n\n\n\n\n\n");
    }
}

void script(char *command[25], int numtoks){
	int i;
	char *temp;
	char nextline[256];
	FILE *script;

	for(i = 2; i<numtoks; i++){
		temp = malloc(strlen(command[i])+1);
		strcpy(temp, command[i]);
		strcat(command[1], " ");
		strcat(command[1], temp);
		free(temp);
	}

	if(command[1] == NULL){
		puts("Enter a file name after SCRIPT");
		return;
	}

	script = fopen(command[1], "r");
	if(script == NULL){
		puts("Failed to open file");
		if(verbose) puts("Confirm if file exists or was properly written");
		return;
	}

	fgets(nextline, 256, script);
	while(nextline != NULL){
		interpret(nextline, NULL);
		if((fgets(nextline, 256, script)) == NULL) break;
	}
	fclose(script);
}

void changeverbose(char *newverb){
	if(newverb == NULL) {
		puts("Follow the VERBOSE command with an ON or OFF");
		return;
	}

	if(strcmp(newverb, "ON") == 0) {
		verbose = 1;
		if(verbose) puts("Verbose mode is now on...");
	}
	else if(strcmp(newverb, "OFF") == 0) {		
		if(verbose) puts("Turning verbose mode off...");
		verbose = 0;
	}
	else puts("Follow the VERBOSE command with an ON or OFF");

}

// Returns pointer to variable of name NAME, prints value if print > 1
list *getvar(char *name, char print, char *memory[]){
	list *curr;
	
	if(verbose && print) puts("Getting variable...");

	if(name == NULL){
		puts("Enter a variable name after GET");
		return NULL;
	}

	curr = head;
	while(curr != NULL){
		if(strcmp(name, curr->name) == 0){
			if(print) printf("%s: %s\n", name, curr->value);
			return curr;
		}
		curr = curr->next;
	}

	if(print) printf("Variable %s does not exist in memory\n", name);
	return NULL;
}

// Set variable in shell memory if memory == NULL, or in registers is memory is not NULL
void setvar(char *command[25], int numtoks, char *memory[]){
	int i;
	char *temp;
	list *newvar;

	if(verbose) puts("Setting variable...");

	if(command[1] == NULL){
		puts("Enter variable name after var");
		return;
	}

	for(i = 3; i<numtoks; i++){
		temp = malloc(strlen(command[i])+1);
		strcpy(temp, command[i]);
		strcat(command[2], " ");
		strcat(command[2], temp);
		free(temp);
	}

	if(command[2] == NULL){
		puts("Enter value after variable name");
		return;
	}

	// Add variable to shell memory
	if(memory == NULL){
		newvar = getvar(command[1], 0, memory);
		if(newvar != NULL){
			free(newvar->value);
			newvar->value = malloc(strlen(command[2])+1);
			strcpy(newvar->value, command[2]);
			return;
		}

		if ((newvar = malloc(sizeof(list))) == NULL){
			puts("Not enough memory to create new variable");
			return;
		}

		newvar->value = malloc(strlen(command[2])+1);
		strcpy(newvar->value, command[2]);
		newvar->name = malloc(strlen(command[1])+1);
		strcpy(newvar->name, command[1]);

		//If head is null, also make newvar tail.
		if(head == NULL) prompt = newvar;
	    
		newvar->next = head;
		head = newvar;
	}
	
	// Add variable to registers
	else{
		for(i=0; i<20; i++){
			
	}
}

// Change prompt directly
void changeprompt(char *command[25], int numtoks){
	int i;
	char *temp;

	if(command[1] == NULL){
		puts("Enter string following PROMPT");
		return;
	}

	for(i = 2; i<numtoks; i++){
		temp = malloc(strlen(command[i])+1);
		strcpy(temp, command[i]);
		strcat(command[1], " ");
		strcat(command[1], temp);
		free(temp);
	}
	
	free(prompt->value);
	prompt->value = malloc(strlen(command[1])+1);
	strcpy(prompt->value, command[1]);
}

// Prints out sentence following echo command
void echo(char *input){
	// If there is no sentence to print, return
	if(strlen(input) <= 6) {
		puts("Enter SENTENCE following ECHO");
		return;
	}
	
	if(*(input+strlen(input)-1) == '\n') *(input+strlen(input)-1) = '\0'; //remove trailing new line
	printf("%s\n", input+5);
}

// Displays something of type TYPE
void display(char *type){
	list *curr;

	if(type == NULL) return;

	if(strcmp("VAR", type) == 0) {
		curr = head;
		while(curr != NULL){
			printf("%s: %s\n", curr->name, curr->value);
			curr = curr->next;
		}
	}
	else puts("Improper type following DISPLAY");
}

// Runs multiple scripts at the same time.
void run(char *command[25], int numtoks){
	FILE *script;
	int i;
	
	for(i = 1; i<numtoks; i++){
		if((script = fopen(command[i], "r")) == NULL){ 
			printf("Could not open file: %s\n", command[i]);
			return;
		}
		else loadScript(script, command[i]);
		
		fclose(script);
	}
}

// Interpret command line input
int interpret(char *input, char *memory[]){
	int i, numtoks;
	char *command[25], *temp;

	if(input == NULL) return 0;
	
	temp = malloc(strlen(input)+1);
	strcpy(temp, input);

	command[0] = strtok(temp, " \n");
	if(command[0] == NULL) return 0; //Handle user simply pressing enter
	for(i = 1; i<25; i++){
		command[i] = strtok(NULL, " \n");
		if(command[i] == NULL) break;
	}
	numtoks = i;

	//Send command to proper location
	if(strcmp(command[0], "EXIT") == 0) return -1;
	else if(strcmp(command[0], "LOGOUT") == 0) return -1;
	else if(strcmp(command[0], "VER") == 0) shversion();
	else if(strcmp(command[0], "PROMPT") == 0) changeprompt(command, numtoks);
	else if(strcmp(command[0], "HELP") == 0) help();
	else if(strcmp(command[0], "VERBOSE") == 0) changeverbose(command[1]);
	else if(strcmp(command[0], "SET") == 0) setvar(command, numtoks, memory);
	else if(strcmp(command[0], "GET") == 0) getvar(command[1], 1, memory);
	else if(strcmp(command[0], "DISPLAY") == 0) display(command[1]);
	else if(strcmp(command[0], "SCRIPT") == 0) script(command, numtoks);
	else if(strcmp(command[0], "CLR") == 0) clear();
	else if(strcmp(command[0], "ECHO") == 0) echo(input);
	else if(strcmp(command[0], "RUN") == 0) run(command, numtoks);
	else return system(input);

	free(temp);
	return 0;
}

// Prompt user for a command
int showprompt(){
	char input[256];
    
	if(verbose) puts("You are in verbose mode. Enter a command or enter HELP for a list of commands");
        printf("%s> ", prompt->value);
        fgets(input, 256, stdin);

        return interpret(input, NULL);
}

// Initialize shell
void mysh(int bose){
	//Initialize variables
	verbose = bose;
	head = NULL;

	//Set prompt variable
	char *command[3];
	command[1] = "prompt";
	command[2] = "!1one";
	setvar(command, 3, NULL);
}
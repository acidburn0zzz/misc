#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "myKernel.h"
#include "myOS.h"
#include "mySH.h"

struct RAM_STRUCTURE {
	int freeSpaceSize;
	int zeroPageSize;
} ramFormat;

struct PCB {
        char filename[50]; // stores the filename of the script loaded into memory
        int start;          // start address - where it is loaded into RAM
        int length;         // the number of instructions
        int current;        // current instruction being executed
        char *heap[20];     // implemented as a list of Shell Memory variables and values
        struct PCB *next;   // next node in the linked-list
} *readyHead, *readyTail, *running, *terminatingHead, *terminatingTail;

struct CPU {
        int quanta;
        int returnCode;
        int pc;
        char *memory[20];
} cpu;

// Searches for block of free ram of int size and returns the starting location of the block. Returns -1 if not found.
int findFreeRAM(int size){
	int i, count, totalRAM, pnt = -1;
	totalRAM = ramFormat.zeroPageSize + ramFormat.freeSpaceSize;
	
	count = 0;
	for(i = ramFormat.zeroPageSize; i<totalRAM; i++){
		if(count == 0 && RAM[i] == NULL) pnt = i;
		
		if(RAM[i] != NULL) count = 0;
		else count++;
		
		if(count >= size) return pnt;
	}
	
	return -1;
}

// Returns the amount of new line characters found in file
int countLines(FILE *script){
	int count = 0;
	char dummy[1000];
	
	// Assumes one line will be no greater than 1000 bytes
	while( fgets(dummy, 1000, script) != NULL) 
		if(strcmp(dummy, "\n") != 0) count++; //Ignore single new line characters
	rewind(script);
	
	return count;
}

// Add new PCB to ready queue
void pushReadyQueue(struct PCB *process){
	if(readyHead == NULL) {
		readyHead = process;
		readyTail = process;
		return;
	}
	
	// Add new item to tail
	readyTail->next = process;
	readyTail = process;
}

// Remove PCB from ready queue. Returns PCB previously pointing to element
struct PCB *removeReadyQueue(struct PCB *element){
	struct PCB *curr;
	
	// Remove head special case
	if(element == readyHead) {
		readyHead = element->next;
		if(readyHead == NULL) readyTail = NULL;
		return NULL;
	}
		
	// If element is not head
	curr = readyHead;
	
	// Make curr the process pointing to element
	while(curr->next != element) 
		curr = curr->next;
	
	curr->next = element->next; //remove running from queue
	
	if(element == readyTail) readyTail = curr; //preserve tail
	
	return curr;
}

// Move running pointer to next PCB
void nextReadyQueue(){
	if(running == NULL) {
		running = readyHead;
		return;
	}
	
	if(running->next == NULL) running = readyHead;
	else running = running->next;
}

// Switch process on CPU
void taskSwitchIn(){	
	if(running == NULL) return;
	
	int i;

	for(i=0;i<20;i++)
		cpu.memory[i] = running->heap[i];
	
	cpu.pc = running->current;
}

// Switch process out of CPU
void taskSwitchOut(){
	if(running == NULL) return;
	
	int i;
	
	for(i=0;i<20;i++)
		running->heap[i] = cpu.memory[i];
	
	running->current = cpu.pc;
}

// Remove running process from ready queue and place in terminating queue
void terminate(){
	struct PCB *previous;
	
	// Remove running from ready queue and make previous the element previous to running to maintain nextReadyQueue order
	previous = removeReadyQueue(running);
	
	// Push running onto terminating queue	
	if(terminatingHead == NULL) {
		terminatingHead = running;
		terminatingTail = running;
		running = previous;
		return;
	}	
	// Add new item to tail
	terminatingTail->next = running;
	terminatingTail = running;
	terminatingTail->next = NULL;
	
	running = previous;
}

// Sets CPU Quanta
void setQuanta(int num){
	cpu.quanta = num;
}

// Run rwhatever is in CPU for quanta
void runForQuanta(){
	if(running == NULL) return;
	
	int i, ramLoc;
	
	for(i = 0; i < cpu.quanta; i++){
		// Check that RAM being accessed actually belong to running. If not, call for terminationg
		if(cpu.pc >= running->length){
			cpu.returnCode = -1;
			break;
		}
		
		ramLoc = running->start + cpu.pc;
		cpu.returnCode = interpret(RAM[ramLoc], cpu.memory);
		
		// Program called to exit
		if(cpu.returnCode < 0) break;
			
		cpu.pc++;
	}
}

// Terminates all processes in terminating queue
void teminateProcesses(){
	int i;
	struct PCB *temp;
	
	while(terminatingHead != NULL){
		for(i = terminatingHead->start; i < (terminatingHead->start+terminatingHead->length); i++){
			free(RAM[i]);
			RAM[i] = NULL;
		}
		temp = terminatingHead;
		terminatingHead = terminatingHead->next;
		free(temp);
	}
	
	terminatingTail = NULL;
}		
	
// Load script into RAM and create PCB
void loadScript(FILE *script, char *name){
	struct PCB *process = malloc(sizeof *process);
	int loc, i;
	char line[1000];
		
	// Create PCB for process
	strcpy(process->filename, name);
	process->length = countLines(script);
	process->start = findFreeRAM(process->length);
	process->current = 0;
	for(i = 0; i<20; i++) process->heap[i] = NULL;
	process->next = NULL;
	
	if(process->start < 0) {
		puts("Insufficient free space in RAM to load");
		return;
	}
	
	// Copy code into RAM
	loc = process->start;
	while( fgets(line, 1000, script) != NULL){
		if(*(line+strlen(line)-1) == '\n') *(line+strlen(line)-1) = '\0'; //Remove trailing new line
		if(strlen(line) == 0) continue; //Skip addition to RAM if empty line
		RAM[loc] = malloc(strlen(line)+1);
		strcpy(RAM[loc], line);
		loc++;
	}
	pushReadyQueue(process); // Put PCB on ready queue
	
	rewind(script);
}

void printRAM(){
	int i;
	
	for(i = 0; i<RAMSIZE; i++)
		if(RAM[i] != NULL) printf("%d: %s\n", i, RAM[i]);
}

void printReady(){
	struct PCB *curr;
	
	puts("Printing ready queue");
	curr = readyHead;
	while(curr != NULL){
		printf("Start: %d, Length: %d, Name: %s\n", curr->start, curr->length, curr->filename);
		curr = curr->next;
	}
	puts("test");
}

void printTerminating(){
	struct PCB *curr;
	
	puts("Printing terminating queue");
	curr = terminatingHead;
	while(curr != NULL){
		printf("Start: %d, Length: %d, Name: %s\n", curr->start, curr->length, curr->filename);
		curr = curr->next;
	}
}

void printRunning(){
	puts("Printing running");
	if(running == NULL) return;
	
	printf("Start: %d, Length: %d, Name: %s\n", running->start, running->length, running->filename);
}

// Convert a string presumed to contain all decimal values to int. Will return -1 if string contained non-integer numbers/characters. Only cares about returning positive numbers properly.
int strToInt(char *str){
	int i, num = 0, temp;
	if(str == NULL) return -1;
	
	for(i = 0; i < strlen(str); i++){
		num = num*10;
		temp = *(str+i)-48; // Convert ascii code to number
		
		// If character was actually an integer, add it to num
		if(temp >= 0 && temp < 10) num = num + temp;
		else return -1;
	}
	
	return num;
}

//Prompt user for name and pw
int login(){
	char username[15], password[20], info[35], curr[35];
	FILE *userfile;
	
	printf("Username: ");
	if(fgets(username, 15, stdin) == NULL) return 0;
	printf("Password: ");
	if(fgets(password, 20, stdin) == NULL) return 0;
	
	strcpy(info, username);
	*(info+strlen(info)-1) = ','; //change the \n from fgets to a comma
	strcat(info, password); //put username,pass in the same format as file
	
	userfile = fopen("pass.cvs", "r");
	if(userfile == NULL){
		puts("User file is missing, fatal error");
		//normally would go into panic for this assignment i'll just say the person is allowed in
		return 1;
	}
	
	// Returns a true if the information matches one in pass.cvs
	while( fgets(curr, 35, userfile) != NULL)
		if(strcmp(info, curr) == 0) return 1;
	
	// Information did not match anything
	puts("Incorrect username/password, try again");
	return 0;
}

// Main kernel
int kernel (int argc, char *argv[]) {
	int fSwitch = 0, i;
	char fverbose = 0, fhelp = 0;

	// Step 1: check for kernel specific switches
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i], "-H") == 0 || strcmp(argv[i], "-h") == 0) fhelp = 1;
		else if(strcmp(argv[i], "-V") == 0 || strcmp(argv[i], "-v") == 0) fverbose = 1;
		else if(strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "-f") == 0) {
			fSwitch = strToInt(argv[i+1]);
			if(fSwitch != -1) i++;
		}
		else printf("%s is an invalid parameter. Use -H for help.\n", argv[i]);
	}
	
	// Step 2: format RAM (since it was already initialized by boot-strap to nulls)
	ramFormat.zeroPageSize = 10;
	if(fSwitch <= 0 || fSwitch > (RAMSIZE-ramFormat.zeroPageSize))
		fSwitch = RAMSIZE-ramFormat.zeroPageSize;
	ramFormat.freeSpaceSize = fSwitch;

	// Step 3: initialize run-time environment
	readyHead = NULL;
	readyTail = NULL;
	running = NULL;
	terminatingHead = NULL;
	terminatingTail = NULL;
	
	cpu.quanta = 0;
	cpu.pc = 0;
	cpu.returnCode = 0;
	for(i = 0; i<20; i++) cpu.memory[i] = NULL;
	
	//show help if requested and initialize shell with correct settings
	if(fhelp) shhelp();
	mysh(fverbose);
	
	// Step 4: start run-time environment with login screen and later shell
	while(!login());
	
	//loop shell prompt
	while(showprompt() >= 0){
		
		while(readyHead != NULL){
			nextReadyQueue();
			taskSwitchIn();
			setQuanta(3);
			runForQuanta();
			taskSwitchOut();
						
			if(cpu.returnCode < 0) terminate(); // If process requested termination
		}
		
		cpu.returnCode = 0; //reset cpu return code
		teminateProcesses();
	}
	
	return 0;
}

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 4
#define ADDRESS_SIZE 5
#define FRAME_SIZE 4
#define PROC_IMG_SIZE

#define MEM_SIZE 2000*FRAME_SIZE
#define SEC_MEM_SIZE 500*FRAME_SIZE


/** Memory data def **/

typedef struct memoryNode {
	int start;
	int size;
	int free;
	struct memoryNode *next;
	struct memoryNode *prev;
};

typedef struct memoryNode MemoryNode;

typedef struct  {
	MemoryNode *begin;
} Memory;

/** Process data def **/
typedef struct {
	int pos;
	int inMemory;
} TableNode;



typedef struct  {
	int n;
	TableNode *memorysTable;
} Process;


/** Command def **/
typedef struct {
	char *process;
	char command;
	int arg;
	int base;
} Command;

char *bitmap;
Memory memory;

void execute(Command);
int allocMemory(int);
void freeMemory(int, int);

void run();

void printMen() {
	MemoryNode* mn = memory.begin;
	while(mn) {
		printf("%d %d %d\n", mn->start, mn->size, mn->free);
		mn = mn->next;
	}
	printf("\n\n");

}

int main() {
	memory.begin = (MemoryNode*) malloc(sizeof(MemoryNode));
	memory.begin->start = 0;
	memory.begin->free = 1;
	memory.begin->size = MEM_SIZE;
	memory.begin->prev = memory.begin->next = NULL;
	run();
	printMen();

}


int allocMemory(int len) {
	MemoryNode *mn = memory.begin, *new;
	int start;
	while(mn) {
		if(mn->free && mn->size >= len) {
			mn->free = 0;
			if(mn->size > len) {
				start = mn->start;
				new = (MemoryNode*) malloc(sizeof(MemoryNode));
				new->free = 1;
				new->size = mn->size - len;
				new->start = mn->start + len;

				new->prev = mn;
				new->next = mn->next;
				mn->next = new;

				mn->size = len;
			}
			break;
		}
		mn = mn->next;
	}
	return start;
}
void freeMemory(int start, int size) {
	MemoryNode *mn = memory.begin, *new;
	while(mn) {
		if(!(mn->free) && mn->start == start) {
			mn->free = 1;
			if(mn->size > size) {
				new = (MemoryNode*) malloc(sizeof(MemoryNode));
				new->free = 0;
				new->size = size;
				new->start = mn->start + size;

				new->prev = mn;
				new->next = mn->next;
				mn->next = new;
				if(mn->prev->free) {
					mn->prev->size += mn->size;
					mn->prev->next = mn->next;
					if(mn->next->prev)
						mn->next->prev = mn->prev;
					free(mn);
				}
			} else {
				if((mn->prev && mn->next) && mn->prev->free && mn->next->free) {
					mn->prev->size += mn->size + mn->next->size;
					mn->prev->next = mn->next->next;
					if(mn->next->next)
						mn->next->next->prev = mn->prev;
				} else if(mn->prev && mn->prev->free) {
					mn->prev->size += mn->size;
					mn->prev->next = mn->next;
					if(mn->next->prev)
						mn->next->prev = mn->prev;
				} else if(mn->next && mn->next->free) {
					mn->next->size += mn->size;
					mn->next->prev = mn->prev;
					if(mn->prev->next)
						mn->prev->next = mn->next;
				}
			}
		}
		mn = mn->next;
	}
}


void execute(Command com) {
	switch(com.command) {
		case 'C':
			allocMemory(com.arg);
			break;
		case 'R':
			break;
		case 'W':
			break;
		case 'I':
			break;
		case 'P':
			break;

		default:
			return;

	}
}


void run() {
	FILE *arq;
	Command com;
	char aux1[30], aux2[30], c, aux3[30];
	arq = fopen("arq.txt", "r");
	if(!arq)
		return;
	printf("test\n");
	// fscanf (arq, "%[^\n]", aux1);
	while(!feof(arq)){
		fscanf (arq, "%s %c %s", aux1, &c, aux2);
		com.process = aux1;
		com.command = toupper(c);
		if(aux2[0] == '(') {
			int i = 0;
			while(i < 30 && aux2[i] != ')')
				i++;
			strncpy(aux3, aux2+1, i-1);
			aux3[i-1] = '\0';
			com.arg = atoi(aux3);
			com.base = 2;


		} else {
			com.arg = atoi(aux2);
			com.base = 10;
		}

		printf("%s %c %d %d\n", com.process, com.command, com.arg, com.base);
		execute(com);
	}


	fclose(arq);
}
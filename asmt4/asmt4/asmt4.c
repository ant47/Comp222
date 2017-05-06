/*
Comp 222
Assignment/Project #4
Virtual Memory
*/
#include <stdio.h>
#include <stdlib.h>


//Parameters to enter
int sizeOfMainMemory;
int sizeOfPage;
int replacementPolicy;//Can be either 1 or 0

int mainMemoryAddress;
int contentsOfAddress;
int numPages;
int numFrames;


int choice;//user selection for options

struct pageTable {
	//Page frames will be mapped to virtual pages 
	int pageFrame;
	int virtualPage;
} *memory; 

void enterParameters() {
	printf("Enter main memory size (words):");
	scanf("%d", &sizeOfMainMemory);

	printf("Enter page size (words/page):");
	scanf("%d", &sizeOfPage);

	printf("Enter replacement policy (0=LRU, 1=FIFO): ");
	scanf("%d", &replacementPolicy);

	numPages = sizeOfMainMemory / sizeOfPage;

	memory = (struct pageTable *)malloc(numPages * sizeof(struct pageTable));


	int i;
	for (i = 0; i < numPages; i++) {
		memory[i].pageFrame = -1;
		memory[i].virtualPage = -1;
	}


}

void mapVA() {
	int VMAddress;//From user
	int realAddress;//Generated
	int virtualPage;
	int offset;


	printf("Enter virtual memory address to access: ");
	scanf("%d", &VMAddress);

	offset = VMAddress % sizeOfPage;
	virtualPage = VMAddress / sizeOfPage;

	int i;
	for (i = 0; i < numPages; i++) {
		//if there is a hit
		if (memory[i].virtualPage == virtualPage) {
			realAddress = (memory[i].pageFrame * sizeOfPage) + offset;

			if (replacementPolicy == 0) {
				int pfHolder;//placeholder for pageframe
				int vpHolder;//placeholder for virtualpage
				int g;
				for (g = i; g < numPages - 1; g++) {
					//Shift page table 
					pfHolder = memory[g].pageFrame;
					vpHolder = memory[g].virtualPage;
					memory[g] = memory[g + 1];
					memory[g + 1].pageFrame = pfHolder;
					memory[g + 1].virtualPage = vpHolder;
				}
			}
			printf("Virtual Address: %d maps to physical address %d", VMAddress,realAddress);
			i = numPages-1;
		}
		else if (memory[i].virtualPage == -1) {
			memory[i].virtualPage = virtualPage;
			memory[i].pageFrame = i;
			printf("Page fault!\n");
			i = numPages - 1;
		}
		else if (i == numPages - 1) {
			memory[0].virtualPage = virtualPage;
			int pfHolder;//placeholder for pageframe
			int vpHolder;//placeholder for virtualpage
			int h;
			for (h = 0; h < numPages - 1; h++) {
				pfHolder = memory[h].pageFrame;
				vpHolder = memory[h].virtualPage;
				memory[h] = memory[h + 1];
				memory[h + 1].pageFrame = pfHolder;
				memory[h + 1].virtualPage = vpHolder;
			}
			printf("Page Fault! \n");
		}
	
	}



	printf("\n----------------------\n");
	printf("| VP   | PF   |\n");
	printf("----------------------\n");
	for (i = 0; i < numPages; i++) {
		if (memory[i].pageFrame != -1 && memory[i].virtualPage != -1) {//To avoid unnecessary printing 
			printf("| %d    |", memory[i].virtualPage);
			printf(" %d    |", memory[i].pageFrame);
			printf("\n----------------------\n");
		}
	}
}


int main() {
	printf("Virtual memory to Main memory mapping:\n");
	printf("------------------------------------\n");
	int boolean = 0;
	while (!boolean) {
		printf("1) Enter parameters\n");
		printf("2) Map virtual address \n");
		printf("3) Quit: \n");

		scanf("%d", &choice);

		if (choice == 1) {
			enterParameters();
		}

		else if (choice == 2) {
			mapVA();
		}

		else if (choice == 3) {
			return 0;
		}

		else//Catches anything outside of 1-3
			printf("Invalid Entry");
		printf("\n");
		printf("\n");

	}
}
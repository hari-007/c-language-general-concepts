/*
 * Author: hari-007 (Hari)
 * Description:  A Fully Associative & Set Associative Cache simmulation based on input arguments.
 * 				 Curently 4 Set 2 Way cache Simmulation available here
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Fuly associative cache line Node Structure */
typedef struct FaNode {
	unsigned int tag;
	struct FaNode *next;
} FaNode;

/* temparay Fully Associative traversal node, hit count remembering node */
typedef struct tempFaStruct {
	int hitCount;
	struct FaNode *tail;
} tempFaStruct;

/* Pointer Macro function for creating a new FaNode structure node and returning its address */
FaNode *get_new_FaNode() {
	FaNode *node = malloc(sizeof *node);

	if(node) {
		node->tag = -1;
		node->next = node;
	} else {
		printf("Unable to create Lined list. Malloc failed!! \n");
        exit(-1);
	}

	return node;
}

/* Creates Circular Linked List with insert at end of each tag node created of totally 'n' */
FaNode *create_fa_nodes_chain(int n) {
	
	FaNode *head, *tail, *temp;

	for(int i=1; i<=n; i++) {
		temp = get_new_FaNode();

		/* For First Link of 1st tag insertion */
		if(i == 1) {
			head = tail = temp;
			head->next = tail->next = head;	
		} 
		/* For Second Link of 2nd tag insertion */
		else if(i == 2) {
			temp->next = head;
			head->next = temp;
			tail = temp;
		} 
		/* For nth Link of nth tag Insertion */ 
		else {
			temp->next = head;
			tail->next = temp;
			tail = temp;
		}
	}

	return tail;
}

/* LRU cahce replacement organisation by changing nodes from tail / anywhere to head */
FaNode *update_Fa_Cache_with_LRU(FaNode *tmpNode, FaNode *tail) {
	if(tmpNode == tail->next) {
		return tail;
	}
	else {
		FaNode *current = tail->next;
		while(current->next != tail->next) {
			if(current->next == tmpNode) {
				break;
			}
			current = current->next;
		}
		if(current->next == tail) {
			tail = current;
		} else {
			// here node goes to head position to show as recently used
			current->next = tmpNode->next;
			tmpNode->next = tail->next;     //strangest line of logic
			tail->next = tmpNode;
		}
		return tail;
	}
}

/* check for hit / miss from a given input address to existed list */
tempFaStruct *check_FA_cache_Hit_and_update(FaNode *tail, unsigned int tempTag, tempFaStruct *tFaStruct, int n) {
	int count = 0, hitFlag = 0;

	/* indirectly current starting from head */
	FaNode *current = tail->next;

	/* Traversing List from Head to Tail */
	while(current->next != tail->next) {
		if(current->tag == -1) {
			break;
		} else if(current->tag == tempTag) {
			hitFlag = 1;
			tFaStruct->hitCount += 1;
			printf("Hit ");
			tFaStruct->tail = update_Fa_Cache_with_LRU(current, tail);
			break;
		} else {
			++count;
			current = current->next;
		}
	}
	if(hitFlag == 0) {
		printf("Miss ");
		current->tag = tempTag;
		tFaStruct->tail = update_Fa_Cache_with_LRU(current, tail);
	}
	return tFaStruct;
}

/* free entire list of created */
void _free_fa_node_list(FaNode *tail, int n) {
	int i =0;
	FaNode *current = tail->next;
	while(i<n) {
		FaNode *tmp = current;
		current = current->next;
		free(tmp);
		++i;
	}
}

/* simulate a full associative cache with 16 blocks and one word each*/
void simuFA(char filename[], int n)
{
	int access=0;

	unsigned int addr;
	
	tempFaStruct *tFaStruct = malloc(sizeof *tFaStruct);
	tFaStruct->hitCount = 0;
	tFaStruct->tail = create_fa_nodes_chain(n);

	FILE *fp;
	fp = fopen(filename, "r");
	
	while (fscanf(fp, "%x", &addr) > 0) {
		access += 1;

		unsigned int tmpTag = addr >> 2;

		printf("%3d: 0x%08x ", access, addr);
		
		tFaStruct = check_FA_cache_Hit_and_update(tFaStruct->tail, tmpTag, tFaStruct, n);
        printf("\n");
    }

	printf("Hits = %d, Accesses = %d, Hit ratio = %f\n", tFaStruct->hitCount, access, ((float)tFaStruct->hitCount)/access);
	_free_fa_node_list(tFaStruct->tail, n);
	free(tFaStruct);
	fclose(fp);
}



/* simulate a set associative cache: 4-set 2-way cache*/
void simuSA(char filename[], int k, int n)
{
	unsigned int* tag = (unsigned int *) malloc(8 * sizeof(int));
	
	unsigned int addr;
	int hits = 0, access = 0, saSets[4][2] = {{0,1}, {0,1}, {0,1}, {0,1}};
	FILE *fp;
	fp = fopen(filename, "r");

	while(fscanf(fp, "%x", &addr) > 0) {

		access += 1;
		printf("%3d: 0x%08x ", access, addr);
		int index =(addr & 0x0f) % 4;

		if(index == 3) {
			if(tag[6] == addr) {
				saSets[3][0] = 1; 
				saSets[3][1] = 0;
				
				++hits;
				printf("Hit");
			} else if(tag[7] == addr) {
				saSets[3][1] = 1; 
				saSets[3][0] = 0;
				
				++hits;
				printf("Hit");
			} else {
				if(saSets[3][0] == 0) {
					
					saSets[3][0] = 1;
					saSets[3][1] = 0;

					tag[6] = addr; 
				} else {
					saSets[3][1] = 1; 
					saSets[3][0] = 0;

					tag[7] = addr;
				}
				printf("Miss");
			}
		} else if(index == 2) {
			if(tag[4] == addr) {
				saSets[2][0] = 1; 
				saSets[2][1] = 0;
				
				++hits;
				printf("Hit");
			} else if(tag[5] == addr) {
				saSets[2][1] = 1; 
				saSets[2][0] = 0;
				
				++hits;
				printf("Hit");
			} else {
				if(saSets[2][0] == 0) {
					saSets[2][0] = 1;
					saSets[2][1] = 0;
					
					tag[4] = addr;
				} else {
					saSets[2][1] = 1; 
					saSets[2][0] = 0;
					
					tag[5] = addr;
				}
				printf("Miss");
			}
		} else if(index == 1) {
			if(tag[2] == addr) {
				saSets[1][0] = 1; 
				saSets[1][1] = 0;

				++hits;
				printf("Hit");
			} else if(tag[3] == addr) {
				saSets[1][1] = 1; 
				saSets[1][0] = 0;
				
				++hits;
				printf("Hit");
			} else {
				if(saSets[1][0] == 0) {
					saSets[1][0] = 1; 
					saSets[1][1] = 0;
					
					tag[2] = addr;
				} else {
					saSets[1][1] = 1; 
					saSets[1][0] = 0;
					
					tag[3] = addr;
				}
				printf("Miss");
			}
		} else {
			if(tag[0] == addr) {
				saSets[0][0] = 1; 
				saSets[0][1] = 0;

				++hits;
				printf("Hit");
			} else if(tag[1] == addr) {
				saSets[0][1] = 1; 
				saSets[0][0] = 0;

				++hits;
				printf("Hit");
			} else {
				if(saSets[0][0] == 0) {
					saSets[0][0] = 1; 
					saSets[0][1] = 0;
					
					tag[0] = addr;
				} else {
					saSets[0][1] = 1; 
					saSets[0][0] = 0;

					tag[1] = addr;
				}
				printf("Miss");
			}
		}
		printf("\n");
	}

	free(tag);

	printf("Hits = %d, Accesses = %d, Hit ratio = %f\n", hits, access, ((float)hits)/access);

	fclose(fp);
}

/* Printing error message and exits the programm peacefuly */
void printfError(int err_num) {
	if(err_num == 1) {
		printf("Invalid input file!! \nPlease try again!!\n");
	} else if(err_num == 2) {
		printf("Invalid information of cahce size!! size should be 0< size> 10000 . \nPlease try again!!\n");
	} else {
		printf("Not a valid input!! \nusuage: ./[output_code_name] -[DM | Fa | sA ] [N-M ways checks for SA | Nothing ] [test_file.txt]\n");
	}	
	exit(0);
}

int checkFileExists(char *filename) {

	if(access(filename, F_OK ) != -1 ) {
		return 1;
	} else {
		return 0;
	}
}

/* Here determines type of operation / action to perform based on input arguments */
int getAction(int argc, char **argv) {
	if(argc == 3 || argc == 4) {
		if(strcmp(argv[1], "-dm") == 0 || strcmp(argv[1], "-DM") == 0 || strcmp(argv[1], "-Dm") == 0 || strcmp(argv[1], "-dM") == 0) {
			return 1;
		} else if((strcmp(argv[1], "-fa") == 0 || strcmp(argv[1], "-FA") == 0 || strcmp(argv[1], "-Fa") == 0 || strcmp(argv[1], "-fA") == 0)) {
			return 2;
		} else if((strcmp(argv[1], "-sa") == 0 || strcmp(argv[1], "-SA") == 0 || strcmp(argv[1], "-Sa") == 0 || strcmp(argv[1], "-sA") == 0)) {
			return 3;
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

int main(int argCount, char **argVector)
{
	int fileNameIndex = 2, faCacheSize = 16;

	switch(getAction(argCount,argVector)) {
		case 1:
				if(checkFileExists(argVector[2]) == 1) {
					simuDM(argVector[2]);
				} else {
					printfError(1);
				}
			break;
		case 2:
				if(argCount == 4) {
					++fileNameIndex;
					faCacheSize = atoi(argVector[2]);
					if(faCacheSize <= 0 && faCacheSize >= 10000) {
						printfError(2);
					}
				}
				if(checkFileExists(argVector[fileNameIndex]) == 1) {
					simuFA(argVector[fileNameIndex], faCacheSize);
				} else {
					printfError(1);
				}
			break;
		case 3:
				if(argCount == 4) ++fileNameIndex;
				
				if(checkFileExists(argVector[fileNameIndex]) == 1) {
					// need to work on k,n string extraction to number
					simuSA(argVector[fileNameIndex],4,2);
				} else {
					printfError(1);
				}
			break;
		default:
			printfError(0);
			break;
	}
	return 0;
}

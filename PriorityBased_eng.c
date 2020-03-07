#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#define TIMEQ 20

//struct
typedef struct process_structure {

	int process_id; //process id
	int priority; //priority
	int computing_time; //service time
	int arrival_time; // arrival time
	int turn_around_time; //turn around time
	int time_left; //purpose: to save left computing time as time goes by  

	struct process_structure* next;//singly linked list

}Node;


//function
void begin(); // print a banner before scheduling process 
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum);//insert based priority
void nodeInitialize(Node* pNew, Node new, int time_lapse);//purpose : the part for initializing new node is repeated in func for inserting node 
void updateComputingtime(Node* phead, int* ptime_lapse, int* pSum, int *pmid_time_lapse);//update the left computing time of the process as time goes by 
void FinishingHeadProcess(Node* pHead, int* pSum); //As the head process in link is finished, print on the console and then free the node. 


//main
int main() {

	Node new; //newly inserted node 
	Node* pHead = (Node *)malloc(sizeof(Node)); pHead->next = NULL; //head pointer: pointer to point the very front node in link
	
	int time_lapse = 0; int* ptime_lapse = &time_lapse;/*variabe: to get the time lapse and arrival time, purpose: initialized by 0 because 
														at first the 3 nodes are arrived at 0s*/
	int sum = 0; int* pSum = &sum; //purpose : to acumulate the each computing time of previous process   
	
	int mid_time_lapse = 0; int* pmid_time_lapse = &mid_time_lapse; // purpose: to save the left computing time if it is less than time quantum, 20s.
																	//(time lapse is always not a multiple of time quantum) 
																					

	//(1) banner
	begin();

	//(2) scheduling process while reading file called "input.txt"
	FILE* fp = fopen("input.txt", "r"); //open file by read mode 	

	while (1) {

		int type; //input type  

		fscanf(fp, "%d %d %d %d", &type, &new.process_id, &new.priority, &new.computing_time); //input data 

		if (type == -1) {//explanation:if type is -1, waiting process(nodes) for being scheduled is finished and then reading file is terminated

			while (1) {
				if (pHead->next == NULL)
					break;
				FinishingHeadProcess(pHead, pSum); //process waiting process for being scheduled without time quantum
			}
			        
			break;
		}
		else if (type == 0) { // get 3 process(nodes) at the same time
			insertNode(pHead, new, time_lapse, pmid_time_lapse, pSum); // insertion sort : insert node in the proper location based priority
		}
		else if (type == 1) { //after 20s change left computing time 
			time_lapse += TIMEQ;  //after 20s 
			updateComputingtime(pHead, ptime_lapse, pSum, pmid_time_lapse); //purpose : update computing time of first node in link as time goes by
		}																	  

	}

	//(3) close file pointer
	fclose(fp);

	return 0;
}

void begin() {
	printf("===============================================================================================\n");
	printf("========================== Priority Based Scheduling Algorithm ================================\n");
	printf("===========================Software 2017156037 JungSooGyung====================================\n\n");

	printf("Process_id\tpriority\tcomputing_time\t\tturn_around time\n");
	printf("_______________________________________________________________________________________________\n");

}


/*
name : insertNode
function : insertion sort based priority in ascending power
-> whenever type in file is 0, insert in proper location based priority in ascending power
*/
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum) {

	Node* pNew = (Node *)malloc(sizeof(Node)); //pointer to point new node that would be inserted
	Node* pCurr = pHead; //always initialize pCurr in first location -> for searching the location to insert from the fist node in link

	if (pHead->next == NULL) { //(1) nothing in link   
		pNew->next = pHead->next;
		nodeInitialize(pNew, new, time_lapse); //insert data
		pHead->next = pNew; //link node that phead is pointing to new node using pNew pointer   

		return;
	}
	else {//(2) more than 1 node in link : insert in ascending power 

		while (pCurr != NULL) {

			//consider 3 cases inserting in ascendin power
			if (pCurr->priority <= new.priority) {//a
				if (pCurr->next == NULL) { //a-1
					pNew->next = NULL;
					nodeInitialize(pNew, new, time_lapse); //insert data
					pCurr->next = pNew;//link
					break;
				}
				else if (pCurr->next->priority > new.priority) { //a-2
					pNew->next = pCurr->next; //link
					nodeInitialize(pNew, new, time_lapse); //insert data
					pCurr->next = pNew;//link
					break;
				}
			}
			else { //b
				pNew->next = pCurr->next; //link
				nodeInitialize(pNew, new, time_lapse); //insert data
				pCurr->next = pNew; //link
				break;
			}

			pCurr = pCurr->next; //purpose :move to next node to search location for new node to be sorted, if not apply to upper all 3 cases

		}//end while

	}//end else

	//check below things after inserting new node    
	if (0 < pHead->next->time_left&&pHead->next->time_left < TIMEQ) { //case1: 0 < left computing time of first node in link< 20 
		*pmid_time_lapse = time_lapse + pHead->next->time_left; /*pmid_time_lapse = time lapse + left computing time 
																purpose of this: Because time lapse is always not a multiple of 20(time quantum), 
																we need another single variable to deal with this situation */
		FinishingHeadProcess(pHead, pSum);  //finish process 
	}
	if (0 < pHead->next->computing_time&&pHead->next->computing_time < TIMEQ) { //case2: 0 < computing time of first node in link < 20 
		*pmid_time_lapse = time_lapse + pHead->next->computing_time; //same reason to up
		FinishingHeadProcess(pHead, pSum);  //finish process
	}


}


/*
name : nodeInitialize
function : purpose : the part for initializing new node is repeated in func for inserting node 
*/
void nodeInitialize(Node * pNew, Node new, int time_lapse)
{
	pNew->process_id = new.process_id;
	pNew->priority = new.priority;
	pNew->computing_time = new.computing_time; //computing time
	pNew->time_left = new.computing_time; //explanation: at first left computing time of process is initialized by originalcomputing time
	pNew->arrival_time = time_lapse;   

	//the member of struct, tat would be calculated finishing process
}


/*
name : computingtimeCalculate
function :  calculate left computing time of the first node to reflect time lapse
(if left computing time is 0s, call FinishHeadProcess to finish the process)
*/
void updateComputingtime(Node* pHead, int* ptime_lapse, int* pSum, int *pmid_time_lapse)
{

	if (*pmid_time_lapse == 0) { //this means left computing time has not been upper to zero and below 20 yet

		pHead->next->time_left = pHead->next->computing_time - TIMEQ; //updated computing time
		int time_left = pHead->next->time_left; //make new variable -> purpose : phead->next->time_left is too long name

		if (time_left == 0)
			FinishingHeadProcess(pHead, pSum);
		else if (0 < time_left)
			return; //do nothing, terminate this function and then continue to next insertion  
	}
	else { //explanation: left computing time has been upper to zero and below 20 ever at least one time
		int x = *ptime_lapse - *pmid_time_lapse;  // 
		if (x == pHead->next->time_left || x == pHead->next->computing_time) { 
			FinishingHeadProcess(pHead, pSum);
			*pmid_time_lapse = 0; //initialize to zero back for another case for good//if left computing time is same with 
		}
	}

}


/*
name: FinishingHeadProcess
function: finish the front node
(1) calculate TAT
(2) print the information of first node(finished node)
(3) disconnect front node to linke and then free from heap memory 
*/
void FinishingHeadProcess(Node* pHead, int* pSum) {

	//1. calculate TAT
	if (*pSum == 0) //explanation: if there is no finished process, sum is initalized value, zero
		*pSum = pHead->next->computing_time; // initialize for the first time
	else
		*pSum += pHead->next->computing_time; //explanation: acumulate the each computing time of previous finished process
											
	pHead->next->turn_around_time = *pSum - pHead->next->arrival_time; /*TAT = acumulative sum of computing time - arrival time of current process
																		to be finished*/

	//2. print 
	printf("%d\t\t%d\t\t%d\t\t\t%d\n", pHead->next->process_id, pHead->next->priority, pHead->next->computing_time, pHead->next->turn_around_time);

	//3. disconnect and free
	Node* pointer = pHead->next;
	pHead->next = pointer->next; //connection: connect pHead to second node
	free(pointer); //free from heap memory 

}






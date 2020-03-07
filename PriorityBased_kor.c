#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#define TIMEQ 20

//구조체
typedef struct process_structure {

	int process_id; //프로세스 id
	int priority; //우선순위
	int computing_time; //서비스 시간
	int arrival_time; // 도착시간
	int turn_around_time; //TAT(턴어라운드 타임)
	int time_left; //목적: 시간의 경과에 따라 남은 서비스 시간을 따로 저장

	struct process_structure* next;//단일연결리스트

}Node;


//함수
void begin(); //초기 화면 출력 
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum);//삽입 정렬(우선순위 기준) 
void nodeInitialize(Node* pNew, Node new, int time_lapse);//목적 : 삽입정렬에서 노드 초기화 부분 반복되서 빼줌
void updateComputingtime(Node* phead, int* ptime_lapse, int* pSum, int *pmid_time_lapse);//시간의 경과에 따른 프로세스의 computing_time 계산 및 업데이트 
void FinishingHeadProcess(Node* pHead, int* pSum); //맨 앞 프로세스 스케출링 완료되서 출력 및 동적할당 해제 


//메인
int main() {

	Node new; //새로 입력받는 노드 
	Node* pHead = (Node *)malloc(sizeof(Node)); pHead->next = NULL; //헤드 포인터 : 맨 앞에 있는 노드를 가리킬 포인터

	int time_lapse = 0; int* ptime_lapse = &time_lapse;//기능: 시간의 경과 및 도착시간 측정, 목적: 맨 처음 type 0으로 받는 노드 3개의 도착시간을 0으로 초기화
	int sum = 0; int* pSum = &sum; //목적 : 각 프로세스의 computing_time 누적해서 더하기 위해
	int mid_time_lapse = 0; int* pmid_time_lapse = &mid_time_lapse; // 목적: 20초 단위로 시간 경과하는 중도에 스케줄링 완료될 경우 그 시간을 저장


	//(1) 시작화면
	begin(); 

	//(2) 파일 읽으면서 스케줄링
	FILE* fp = fopen("input.txt", "r"); //'r' 읽기 모드로 열고, 'fp' 파일포인터 반환(기본 경로는 현재 파일이 있는 경로임 )
													
	while (1) {

		int type; //입력 타입  

		fscanf(fp, "%d %d %d %d", &type, &new.process_id, &new.priority, &new.computing_time); //입력받기 

		if (type == -1) {//설명: -1을 만나면 남아있는 대기중인 프로세스들 완료시키고 파일 읽기 종료
			
			while (1) {
				if (pHead->next == NULL)
					break;
				FinishingHeadProcess(pHead, pSum); //-> 타임제한 없이 뒤에 남아있는 프로세스들 처리
			}

			break;
		}
		else if (type == 0) { // 프로세스 3개를 동시간에 받아들이기 
			insertNode(pHead, new, time_lapse, pmid_time_lapse, pSum); //삽입정렬 : 우선순위대로 정렬하여 3개의 프로세스 삽입
		}
		else if (type == 1) { //20초 경과, 맨 앞에 있는 프로세스 처리시간 삭감과 스케줄링 
			time_lapse += TIMEQ;  //20초 경과 
			updateComputingtime(pHead, ptime_lapse, pSum, pmid_time_lapse); //목적 : 시간의 경과를 반영하여 맨 앞에 있는 프로세스 computing_time 계산 및 업데이트
		} 

	}

	//(3) 파일 포인터 닫기
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
이름 : insertNode
기능 : 오름차순 삽입
-> type이 0일 때마다 뒤로 갈 수록 우선순위가 큰 것이 위치하도록 정렬하면서 삽입
*/
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum) {

	Node* pNew = (Node *)malloc(sizeof(Node)); //새로 들어온 노드를 가리킬 포인터
	Node* pCurr = pHead; //pCurr은 항상 맨 앞으로 초기화 -> 맨 앞에서부터 비교하면서 삽입 위치를 찾아야 하기 때문에


	if (pHead->next == NULL) { //(1) 노드를 처음 받을 때 : pHead 뒤에 뭐가 없을 경우에  
		pNew->next = pHead->next;
		nodeInitialize(pNew, new, time_lapse); //데이터 삽입
		pHead->next = pNew; //pHead 노드 뒤에 pNew 노드 연결 

		return;
	}
	else {//(2) 노드가 1개 이상 있을 경우 : 객체 오름차순 삽입

		while (pCurr != NULL) {

			//오름차순 삽입할 때 총 3가지 경우 고려
			if (pCurr->priority <= new.priority) {//a
				if (pCurr->next == NULL) { //a-1
					pNew->next = NULL;
					nodeInitialize(pNew, new, time_lapse); //데이터 삽입
					pCurr->next = pNew;//연결
					break;
				}
				else if (pCurr->next->priority > new.priority) { //a-2
					pNew->next = pCurr->next; //연결
					nodeInitialize(pNew, new, time_lapse); //데이터 삽입
					pCurr->next = pNew;//연결
					break;
				}
			}
			else { //b
				pNew->next = pCurr->next; //연결
				nodeInitialize(pNew, new, time_lapse); //데이터 삽입
				pCurr->next = pNew; //연결
				break;
			}
			
			pCurr = pCurr->next; //목적 : 위의 경우 중 어떤 것에도 해당하지 않는 경우 연결된 다음 노드로 이동
		
		}//end while

	}//end else

	//삽입 후 검사
	if (0 < pHead->next->time_left&&pHead->next->time_left < TIMEQ) { //0 < 맨 앞에 정렬된 노드의 남은 computing_time < 20 일 경우
		*pmid_time_lapse = time_lapse + pHead->next->time_left; // 중간 시간경과 = 시간경과 + 맨 앞 노드 컴퓨팅시간
		FinishingHeadProcess(pHead, pSum);  //스케줄링 완료시키기
	}
	if (0 < pHead->next->computing_time&&pHead->next->computing_time < TIMEQ) { //0 < 맨 앞에 정렬된 노드의 computing_time < 20 일 경우
		*pmid_time_lapse = time_lapse + pHead->next->computing_time; // 중간 시간경과 = 시간경과 + 맨 앞 노드 컴퓨팅시간
		FinishingHeadProcess(pHead, pSum);  //스케줄링 완료시키기
	}
	

}


/*
이름 : nodeInitialize
기능 : 삽입정렬 inserNode()함수에서 노드값 초기화하는 코드가 반복되서 가독성을 위해 nodeInitialize 함수로 빼줌
*/
void nodeInitialize(Node * pNew, Node new, int time_lapse)
{
	pNew->process_id = new.process_id;
	pNew->priority = new.priority;
	pNew->computing_time = new.computing_time; //처리시간
	pNew->time_left = new.computing_time; //설명: 남은 처리시간 맨 처음엔 처리시간과 동일하게 초기화
	pNew->arrival_time = time_lapse;
	
	//TAT 멤버는 나중에 프로세스 스케줄링 완료시킬 때 계산
}


/*
함수명 : updateComputingtime
기능 :  시간의 경과를 반영하여 맨 앞에 있는 프로세스 computing_time 계산, 업데이트, 스케줄링 완료
*/
void updateComputingtime(Node* pHead, int* ptime_lapse, int* pSum, int *pmid_time_lapse) 
{
	
	if (*pmid_time_lapse == 0) {
		
		pHead->next->time_left = pHead->next->computing_time - TIMEQ; //업데이트된 컴퓨팅 시간
		int time_left = pHead->next->time_left; //목적 : phead->next->time_left변수명 너무 길어서
		
		if (time_left == 0)
			FinishingHeadProcess(pHead, pSum);
		else if (0 < time_left)
			return; //처리x, 그 다음 삽입정렬로 넘어가기
	}
	else { //설명: 20초단위의 시간이 경과하는 도중에 스케줄링이 완료된 경우가 있다면 mid_time_lapse가 '0'에서 변동됨
		int x = *ptime_lapse - *pmid_time_lapse;

		if ( x== pHead->next->time_left || x == pHead->next->computing_time) { //설명 : 남은 처리 시간 혹은 전체 처리시간과 경과시간이 같다면
			FinishingHeadProcess(pHead, pSum);
			*pmid_time_lapse = 0; //초기화
		}
	}

}


/*
함수명: FinishingHeadProcess
기능: 맨 앞 프로세스 스케줄링 완료시키기
(1) TAT 계산
(2) 맨 앞 프로세스(노드) 정보 출력 
(3) 동적할당 해제 
*/
void FinishingHeadProcess(Node* pHead, int* pSum) {  

	//1. TAT 계산
	if (*pSum == 0) //설명: 이전에 스케줄링 완료된 프로세스가 없을 경우 : pSum이 가리키고 있는 sum은 0으로 초기화 된 상태
		*pSum = pHead->next->computing_time; // 처음으로 초기화
	else
		*pSum += pHead->next->computing_time; //설명: 이전에 완료된 프로세스의 computing_time에 현재 완료시킬 프로세스의 computing_time 누적하여 계산

	pHead->next->turn_around_time = *pSum - pHead->next->arrival_time; //TAT = 바로 앞에 완료되었던 프로세스들의 computing_time 누적 합 - 현재 스케줄링 완료시킬 프로세스의 도착시간
 											
	//2. 화면 출력
	printf("%d\t\t%d\t\t%d\t\t\t%d\n", pHead->next->process_id, pHead->next->priority, pHead->next->computing_time, pHead->next->turn_around_time);
	
	//3. 동적 할당 해제
	Node* pointer = pHead->next; 
	pHead->next = pointer->next; //연결: pHead가 이제 2번째 노드를 가리키도록 연결
	free(pointer); //메모리 반납 
	
}






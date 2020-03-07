#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#define TIMEQ 20

//����ü
typedef struct process_structure {

	int process_id; //���μ��� id
	int priority; //�켱����
	int computing_time; //���� �ð�
	int arrival_time; // �����ð�
	int turn_around_time; //TAT(�Ͼ���� Ÿ��)
	int time_left; //����: �ð��� ����� ���� ���� ���� �ð��� ���� ����

	struct process_structure* next;//���Ͽ��Ḯ��Ʈ

}Node;


//�Լ�
void begin(); //�ʱ� ȭ�� ��� 
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum);//���� ����(�켱���� ����) 
void nodeInitialize(Node* pNew, Node new, int time_lapse);//���� : �������Ŀ��� ��� �ʱ�ȭ �κ� �ݺ��Ǽ� ����
void updateComputingtime(Node* phead, int* ptime_lapse, int* pSum, int *pmid_time_lapse);//�ð��� ����� ���� ���μ����� computing_time ��� �� ������Ʈ 
void FinishingHeadProcess(Node* pHead, int* pSum); //�� �� ���μ��� �����⸵ �Ϸ�Ǽ� ��� �� �����Ҵ� ���� 


//����
int main() {

	Node new; //���� �Է¹޴� ��� 
	Node* pHead = (Node *)malloc(sizeof(Node)); pHead->next = NULL; //��� ������ : �� �տ� �ִ� ��带 ����ų ������

	int time_lapse = 0; int* ptime_lapse = &time_lapse;//���: �ð��� ��� �� �����ð� ����, ����: �� ó�� type 0���� �޴� ��� 3���� �����ð��� 0���� �ʱ�ȭ
	int sum = 0; int* pSum = &sum; //���� : �� ���μ����� computing_time �����ؼ� ���ϱ� ����
	int mid_time_lapse = 0; int* pmid_time_lapse = &mid_time_lapse; // ����: 20�� ������ �ð� ����ϴ� �ߵ��� �����ٸ� �Ϸ�� ��� �� �ð��� ����


	//(1) ����ȭ��
	begin(); 

	//(2) ���� �����鼭 �����ٸ�
	FILE* fp = fopen("input.txt", "r"); //'r' �б� ���� ����, 'fp' ���������� ��ȯ(�⺻ ��δ� ���� ������ �ִ� ����� )
													
	while (1) {

		int type; //�Է� Ÿ��  

		fscanf(fp, "%d %d %d %d", &type, &new.process_id, &new.priority, &new.computing_time); //�Է¹ޱ� 

		if (type == -1) {//����: -1�� ������ �����ִ� ������� ���μ����� �Ϸ��Ű�� ���� �б� ����
			
			while (1) {
				if (pHead->next == NULL)
					break;
				FinishingHeadProcess(pHead, pSum); //-> Ÿ������ ���� �ڿ� �����ִ� ���μ����� ó��
			}

			break;
		}
		else if (type == 0) { // ���μ��� 3���� ���ð��� �޾Ƶ��̱� 
			insertNode(pHead, new, time_lapse, pmid_time_lapse, pSum); //�������� : �켱������� �����Ͽ� 3���� ���μ��� ����
		}
		else if (type == 1) { //20�� ���, �� �տ� �ִ� ���μ��� ó���ð� �谨�� �����ٸ� 
			time_lapse += TIMEQ;  //20�� ��� 
			updateComputingtime(pHead, ptime_lapse, pSum, pmid_time_lapse); //���� : �ð��� ����� �ݿ��Ͽ� �� �տ� �ִ� ���μ��� computing_time ��� �� ������Ʈ
		} 

	}

	//(3) ���� ������ �ݱ�
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
�̸� : insertNode
��� : �������� ����
-> type�� 0�� ������ �ڷ� �� ���� �켱������ ū ���� ��ġ�ϵ��� �����ϸ鼭 ����
*/
void insertNode(Node* pHead, Node new, int time_lapse, int *pmid_time_lapse, int* pSum) {

	Node* pNew = (Node *)malloc(sizeof(Node)); //���� ���� ��带 ����ų ������
	Node* pCurr = pHead; //pCurr�� �׻� �� ������ �ʱ�ȭ -> �� �տ������� ���ϸ鼭 ���� ��ġ�� ã�ƾ� �ϱ� ������


	if (pHead->next == NULL) { //(1) ��带 ó�� ���� �� : pHead �ڿ� ���� ���� ��쿡  
		pNew->next = pHead->next;
		nodeInitialize(pNew, new, time_lapse); //������ ����
		pHead->next = pNew; //pHead ��� �ڿ� pNew ��� ���� 

		return;
	}
	else {//(2) ��尡 1�� �̻� ���� ��� : ��ü �������� ����

		while (pCurr != NULL) {

			//�������� ������ �� �� 3���� ��� ���
			if (pCurr->priority <= new.priority) {//a
				if (pCurr->next == NULL) { //a-1
					pNew->next = NULL;
					nodeInitialize(pNew, new, time_lapse); //������ ����
					pCurr->next = pNew;//����
					break;
				}
				else if (pCurr->next->priority > new.priority) { //a-2
					pNew->next = pCurr->next; //����
					nodeInitialize(pNew, new, time_lapse); //������ ����
					pCurr->next = pNew;//����
					break;
				}
			}
			else { //b
				pNew->next = pCurr->next; //����
				nodeInitialize(pNew, new, time_lapse); //������ ����
				pCurr->next = pNew; //����
				break;
			}
			
			pCurr = pCurr->next; //���� : ���� ��� �� � �Ϳ��� �ش����� �ʴ� ��� ����� ���� ���� �̵�
		
		}//end while

	}//end else

	//���� �� �˻�
	if (0 < pHead->next->time_left&&pHead->next->time_left < TIMEQ) { //0 < �� �տ� ���ĵ� ����� ���� computing_time < 20 �� ���
		*pmid_time_lapse = time_lapse + pHead->next->time_left; // �߰� �ð���� = �ð���� + �� �� ��� ��ǻ�ýð�
		FinishingHeadProcess(pHead, pSum);  //�����ٸ� �Ϸ��Ű��
	}
	if (0 < pHead->next->computing_time&&pHead->next->computing_time < TIMEQ) { //0 < �� �տ� ���ĵ� ����� computing_time < 20 �� ���
		*pmid_time_lapse = time_lapse + pHead->next->computing_time; // �߰� �ð���� = �ð���� + �� �� ��� ��ǻ�ýð�
		FinishingHeadProcess(pHead, pSum);  //�����ٸ� �Ϸ��Ű��
	}
	

}


/*
�̸� : nodeInitialize
��� : �������� inserNode()�Լ����� ��尪 �ʱ�ȭ�ϴ� �ڵ尡 �ݺ��Ǽ� �������� ���� nodeInitialize �Լ��� ����
*/
void nodeInitialize(Node * pNew, Node new, int time_lapse)
{
	pNew->process_id = new.process_id;
	pNew->priority = new.priority;
	pNew->computing_time = new.computing_time; //ó���ð�
	pNew->time_left = new.computing_time; //����: ���� ó���ð� �� ó���� ó���ð��� �����ϰ� �ʱ�ȭ
	pNew->arrival_time = time_lapse;
	
	//TAT ����� ���߿� ���μ��� �����ٸ� �Ϸ��ų �� ���
}


/*
�Լ��� : updateComputingtime
��� :  �ð��� ����� �ݿ��Ͽ� �� �տ� �ִ� ���μ��� computing_time ���, ������Ʈ, �����ٸ� �Ϸ�
*/
void updateComputingtime(Node* pHead, int* ptime_lapse, int* pSum, int *pmid_time_lapse) 
{
	
	if (*pmid_time_lapse == 0) {
		
		pHead->next->time_left = pHead->next->computing_time - TIMEQ; //������Ʈ�� ��ǻ�� �ð�
		int time_left = pHead->next->time_left; //���� : phead->next->time_left������ �ʹ� ��
		
		if (time_left == 0)
			FinishingHeadProcess(pHead, pSum);
		else if (0 < time_left)
			return; //ó��x, �� ���� �������ķ� �Ѿ��
	}
	else { //����: 20�ʴ����� �ð��� ����ϴ� ���߿� �����ٸ��� �Ϸ�� ��찡 �ִٸ� mid_time_lapse�� '0'���� ������
		int x = *ptime_lapse - *pmid_time_lapse;

		if ( x== pHead->next->time_left || x == pHead->next->computing_time) { //���� : ���� ó�� �ð� Ȥ�� ��ü ó���ð��� ����ð��� ���ٸ�
			FinishingHeadProcess(pHead, pSum);
			*pmid_time_lapse = 0; //�ʱ�ȭ
		}
	}

}


/*
�Լ���: FinishingHeadProcess
���: �� �� ���μ��� �����ٸ� �Ϸ��Ű��
(1) TAT ���
(2) �� �� ���μ���(���) ���� ��� 
(3) �����Ҵ� ���� 
*/
void FinishingHeadProcess(Node* pHead, int* pSum) {  

	//1. TAT ���
	if (*pSum == 0) //����: ������ �����ٸ� �Ϸ�� ���μ����� ���� ��� : pSum�� ����Ű�� �ִ� sum�� 0���� �ʱ�ȭ �� ����
		*pSum = pHead->next->computing_time; // ó������ �ʱ�ȭ
	else
		*pSum += pHead->next->computing_time; //����: ������ �Ϸ�� ���μ����� computing_time�� ���� �Ϸ��ų ���μ����� computing_time �����Ͽ� ���

	pHead->next->turn_around_time = *pSum - pHead->next->arrival_time; //TAT = �ٷ� �տ� �Ϸ�Ǿ��� ���μ������� computing_time ���� �� - ���� �����ٸ� �Ϸ��ų ���μ����� �����ð�
 											
	//2. ȭ�� ���
	printf("%d\t\t%d\t\t%d\t\t\t%d\n", pHead->next->process_id, pHead->next->priority, pHead->next->computing_time, pHead->next->turn_around_time);
	
	//3. ���� �Ҵ� ����
	Node* pointer = pHead->next; 
	pHead->next = pointer->next; //����: pHead�� ���� 2��° ��带 ����Ű���� ����
	free(pointer); //�޸� �ݳ� 
	
}






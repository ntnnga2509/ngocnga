//Ho va ten: Nguyen Thi Ngoc Nga
//Ma so sinh vien: B1809154
//Nhom: 03
//So thu tu: 20
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#define max_X 9
#define max_Y 4
#define goal 6
#define Maxlength 100

typedef struct{
	int x,y;
}State;

void makeNullState(State *state){
	state->x=0;
	state->y=0;
}

void print_State(State state){
	printf("( %d, %d )\n",state.x,state.y);
}
int goalCheck(State state){
	return (state.x==goal || state.y==goal);
}
int lamDayX(State cur_state,State *result){
	if(cur_state.x < max_X){
		result->x = max_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
int lamDayY(State cur_state,State *result){
	if(cur_state.y < max_Y){
		result->y = max_Y;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}
int lamRongX(State cur_state,State *result){
	if(cur_state.x>0){
		result->x = 0;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
int lamRongY(State cur_state,State *result){
	if(cur_state.y>0){
		result->y = 0;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}
int max(int x,int y){
	return	(x > y) ? x : y;
}
int min(int x,int y){
	return	(x < y) ?  x :  y;
}

int chuyenXsangY(State cur_state,State *result){
	if(cur_state.x>0 && cur_state.y<max_Y){
		result->x=max(cur_state.x + cur_state.y- max_Y , 0);
		result->y=min(cur_state.x+cur_state.y, max_Y);
	
		return 1;
	}
	return 0;
}
int chuyenYsangX(State cur_state,State *result){
	if(cur_state.y>0 && cur_state.x<max_Y){
		result->x=max(cur_state.x + cur_state.y- max_X, 0);
		result->y=min(cur_state.y+cur_state.x, max_X);
		return 1;
	}
	return 0;
}
int call_operation(State cur_state,State *result, int option){
	switch(option){
		case 1: return lamDayX(cur_state, result);
		case 2: return lamDayY(cur_state, result);
		case 3: return lamRongX(cur_state, result);
		case 4: return lamRongY(cur_state, result);
		case 5: return chuyenXsangY(cur_state, result);
		case 6: return chuyenYsangX(cur_state, result);
		default: printf("Yeu cau khong hop le.");
			return 0;
	}
}
const char* action[]={"Trang thai dau","Lam day X","Lam day Y","Lam rong X","Lam rong Y","Do tu X -> Y","Do tu Y->X"};

typedef struct Node{
	State state;
	struct Node *Parent;
	int no_function;
}Node;
//Khai bao hang doi
typedef struct{
	Node* Elements[Maxlength];
	int front,rear;
}Queue;
void makeNull_Queue(Queue *Q){
	Q->front=-1;
	Q->rear=-1;
}
int empty_Queue(Queue Q){
	return Q.front==-1;
}
int full_Queue(Queue Q){
	return (Q.rear-Q.front+1)%Maxlength==0;
}

Node* getFront(Queue Q){
	if (empty_Queue(Q))
		printf("Hang doi rong");
	else return Q.Elements[Q.front];
}
void del_Queue(Queue *Q){
	if (!empty_Queue(*Q)){
		if (Q->front==Q->rear)
			makeNull_Queue(Q);
		else Q->front=Q->front+1;		
	}
	else printf("Hang doi rong");

}
void push_Queue(Queue *Q, Node *node){
	if (!full_Queue(*Q)){
		if (empty_Queue(*Q))
			Q->front=0;
		Q->rear=Q->rear+1;
		Q->Elements[Q->rear]=node;
	}
	else printf("Hang doi day");
}

int find_State(State S, Queue Q){
	int i;
	for(i=Q.front;i<Q.rear;i++){
		if (Q.Elements[i]->state.x==S.x && Q.Elements[i]->state.y==S.y)	return 1;
	}
	return 0;
}

Node* breathFirstSearch(State S){
	Queue Open;
	Queue Close;
	makeNull_Queue(&Open);
	makeNull_Queue(&Close);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state=S;
	root->Parent=NULL;
	root->no_function=0;
	push_Queue(&Open,root);
	while (!empty_Queue(Open)){
		Node* node = getFront(Open);	//lay phan tu dau trong hang doi Open
		del_Queue(&Open);
		push_Queue(&Close,node);
		if (goalCheck(node->state))	return node;
		int i;
		for (i=1;i<=6;i++){
			State S1;
			makeNullState(&S1);
			if (call_operation(node->state,&S1,i)){
				if (!find_State(S1,Close) && !find_State(S1,Open)) {
					
					Node* newNode=(Node*)malloc(sizeof(Node));
					newNode->state=S1;
					newNode->Parent=node;
					newNode->no_function=i;
					push_Queue(&Open,newNode);
				}
			}
			
		}
	}
	
	return NULL;
}
//chuyen tu mot hang doi sang hang doi khac voi thu tu nguoc lai
void dn_Queue(Queue Q, Queue *result){
	makeNull_Queue(result);
	int i;
	for (i=Q.rear;i>=Q.front;i--){
		push_Queue(result,Q.Elements[i]);
	}
}

void print(Node* node){
	printf("Ket qua duyet:\n");
	Queue Q;
	makeNull_Queue(&Q);
	while(node->Parent != NULL){
		push_Queue(&Q,node);
		node = node->Parent;
	}
	int no_action=0;
	Queue queuePrint;
	dn_Queue(Q,&queuePrint);
	while (!empty_Queue(queuePrint)){
		printf(" Hanh dong %d: %s \t",no_action, action[getFront(queuePrint)->no_function]);
		print_State(getFront(queuePrint)->state);
		del_Queue(&queuePrint);
		no_action++;
	}
}
int main(){
	State S={0,0};
	Node* duyet=(Node*)malloc(sizeof(Node));
	duyet=breathFirstSearch(S);
	print(duyet);
	return 0;
}

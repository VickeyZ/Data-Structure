#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<time.h>
using namespace std;

#define MAXLEVEL 10 //max level of skip list
clock_t start, stop;
double duration;

//every node consists of its value and pointers to next nodes in each level
struct Node {
	int value;//value should be positive integer
	//set value to be 0 when the node is empty
	struct Node* next[1];//pointers to next nodes in each level
}; 

struct SkipList {
	int level;
	struct Node* top;//pointer to the head of skip list
};
struct SkipList* skiplist;
struct Node* route[MAXLEVEL+1];//record the search route
struct Node* tmp1 = NULL;
struct Node* tmp2 = NULL;

//create a new node
struct Node* AddNode( int value, int level ) {
	struct Node* newnode 
		= (struct Node*)malloc(sizeof(struct Node) + sizeof(struct Node*) * level);
	newnode->value = value;										
	return newnode;										
}

//initialize the empty skip list
void InitSkiplist() {
	printf("initialize the empty skip list\n");
	skiplist = (struct SkipList*)malloc(sizeof(struct SkipList));
	skiplist->level = 1;
	skiplist->top = AddNode(0, MAXLEVEL);
	int i;
	for (i = 1; i <= MAXLEVEL; i++) {
		skiplist->top->next[i] = NULL;
	}
}

//printf the whole skip list
void Print() {
	tmp1 = NULL;
	tmp2 = skiplist->top;
	int i;
	
	//print from the top level
	for (i = skiplist->level; i>=1; i--) {
		tmp2 = skiplist->top;
		while (1) {
			tmp1 = tmp2->next[i];
			if (tmp1 != NULL) {
				//printf("%d ", tmp1->value);
				tmp2 = tmp1;				
			} else {
				break;
			}						
		}
	//	printf("\n");
	}
	//printf("\n");
} 

//randomly get the level to insert 
//prelevel is the present total level of skiplist
int Random(int prelevel) {
	int level = 1;
	
	//toss a coin
	while (rand()%2 && level<prelevel+1 && level<MAXLEVEL) {
		level++;
	}
	//update the total level
	if (level > prelevel) {
		int i;
		for (i = prelevel+1; i<= level; i++) {
			route[i] = skiplist->top;
		}
		skiplist->level = level;
	}
	return level;
}

void GetRoute(int value) {
	tmp1 = NULL;
	tmp2 = skiplist->top;
	int i;
	
	//clear route[]
	for (i = skiplist->level; i>=1; i--) {
		route[i] = NULL;
	}
	//record the search route and find the place to insert
	for (i = skiplist->level; i>=1; i--) {
		while (1) {
			tmp1 = tmp2->next[i];
			if (tmp1 != NULL) {
				if (tmp1->value < value) {
					tmp2 = tmp1;
				} else {
					break;
				}
			} else {
				break;
			}			
		}
		route[i] = tmp2;
	}
}

//insert a new num into skip list
void Insert(int value){
	int i;
	struct Node* tmp = NULL;
	
	//record the search route and find the place to insert
	GetRoute(value);
	//judge whether value is repeated
	if (tmp1 != NULL) {
		if (tmp1->value == value) {
			//printf("%d is already in skip list\n", value);
			//Print();	
			return;
		}
	}
	//randomly get the level to insert 
	int insertlevel = Random(skiplist->level);
	//initial the new node
	tmp = AddNode(value, insertlevel);	
	//insert and update every level
	for (i = 1; i <= insertlevel; i++) {
		tmp->next[i] = route[i]->next[i];
		route[i]->next[i] = tmp;
	}
	//Print();	
}

//judge whether the value is in skip list or not
void Search (int value) {
	tmp1 = NULL;
	tmp2 = skiplist->top;	
	int i;
	
	//search from the top level
	for (i = skiplist->level; i>=1; i--) {
		tmp2 = skiplist->top;
		while (1) {
			tmp1 = tmp2->next[i];
			if (tmp1 != NULL) {
				if (tmp1->value <= value) {
					if (tmp1->value == value) {
						//printf("%d is in skip list\n\n",value);
						return;
					}
					tmp2 = tmp1;
				} else {
					break;
				}			
			} else {
				break;
			}						
		}
	}
	//printf("%d is not in skip list\n\n",value);
}

void Delete(int value) {
	//record the search route and find the place to insert
	GetRoute(value);
	int j;
	if (tmp1 &&tmp1->value == value) {
		//delete and update every level
		for (j = 1; j <= skiplist->level; j++) {
			if (route[j]->next[j] == tmp1) {
				route[j]->next[j] = tmp1->next[j];
			}
		}
		free(tmp1);
		//update the total level
		for (j = skiplist->level; j>=1; j--) {
			if (skiplist->top->next[j] == NULL) {
				skiplist->level--;
			}
		}
		//printf("delete %d successfully\n", value);		
		//Print();
		return;
	}
	//printf("delete failed\n%d is not in skip list\n\n", value);
	return;
}

int main()
{
	long Num;
	int* Array;
	printf("Please input the number of items inserted into the skiplist:");
	scanf("%ld", &Num);
	Array = (int*)malloc(sizeof(int) * Num);

	InitSkiplist();
	srand((unsigned)time(NULL));

    //测试插入
    start = clock();
    for(int i = 0; i < Num; ++i){
		Array[i] = rand();
		Insert(Array[i]);
    }
	stop = clock();
	duration = ((double)(stop - start))/CLK_TCK;
	printf("Time for inserting by second is %lf\n", duration);

	//测试查找
    start = clock();
    for(int i = 0; i < Num; ++i){
		Search(Array[rand() % Num]);
    }
	stop = clock();
	duration = ((double)(stop - start))/CLK_TCK;
	printf("Time for searching by second is %lf\n", duration);

	//测试删除
    start = clock();
    for(int i = 0; i < Num; ++i){
		Delete(Array[rand() % Num]);
    }
	stop = clock();
	duration = ((double)(stop - start))/CLK_TCK;
	printf("Time for deleting by second is %lf\n", duration);

    return 0;
}

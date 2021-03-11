//Shortest Path Algorithm with Heaps
//Implement Dijkstra's algorithm with Fibonacci heap and min heap

//Input file is like:
//    p sp 264346 733846
//    c graph contains 264346 nodes and 733846 arcs
//    a 1 2 803
//    a 2 1 803
//    a 3 4 158
//    a 4 3 158
//    a 5 6 774
//    a 6 5 774
//    ......
//

#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<algorithm>
#include<vector>
#include<queue>
#include<cstring>
#include<string>
#include<cctype>
#include<iostream>
#include<time.h>
#include<windows.h>
using namespace std;


#define maxnode  1000000  //the maximal number of nodes in a map
const int maxline = 100;  //the maximal character number in a line
const long long infinity = 10000000000; 

//use adjecency list to save the distance map
struct Adj_List {
    long long adjnode; 
    long long distance;
    struct Adj_List *next;
};
struct Adj_List *AdjList[maxnode+5];
long long totalnodes, totalarcs;

//Clock_t is the variable type returned by the clock () function
clock_t start,stop; 
//Record the running time of the function under test in seconds
double duration;


typedef long long Type;

typedef struct FibonacciNode *FibNode;
struct FibonacciNode{
    Type key; //key value
    Type index; // node index
    Type degree; 
    int mark; // whether marked or not
    FibNode left; //left node
    FibNode right; //right node
    FibNode child; // children node
    FibNode parent; //parent node
};

typedef struct FibonacciHeap *FibHeap;
struct FibonacciHeap{
    Type keyNum; // the number of nodes
    Type maxDegree; 
    FibNode min; //the minimum node
    FibNode *cons; //children of the previous minimum node
};

void AddRoad(long long node1, long long node2, long long distance);
void DijkByMinheap(long long startnode, long long endnode);
void DijkByFibheap(long long startnode, long long endnode);

FibHeap create_fibheap();

void insertKey(FibHeap heap, Type idx, Type ikey);
void insertNode(FibNode node, FibNode root);

FibNode getmin(FibHeap heap);//remove and return the minimum node
void deletemin(FibHeap heap);//delete the minimum node, adjust the heap, and return the root list

void fiblink(FibHeap heap, FibNode node, FibNode root); //link node to the double linked list
void fibconsolidate(FibHeap heap); //consolidate heaps with the same degree

int main() {
	char flag;
    char str[maxline];
    long long i=0,j=0;
    long long node1, node2, distance;
    long long startnode, endnode; 
	long long query_pairs;  //  total number of query pairs
	int heapflag;
    
    printf("Please input the flag of heap(Fibonacci heap is 1, min heap is 2): ");
    scanf("%ld", &heapflag);
	printf("Please input total number of query pairs : ");
    scanf("%lld", &query_pairs);
	//read from file
    //replace the filename to read different test data file     
    freopen("USA-road-d.NY.gr", "r", stdin);     
    while (1) {  //get the flag in front of each line
        scanf("%c", &flag);
        if (flag == 'c') {  
            //if 'c', skip the line
            gets(str);
        } else if (flag == 'p') {  
            //if 'p', get total number of nodes and arcs
            getchar();getchar();getchar();
            scanf("%lld%lld", &totalnodes, &totalarcs);
            for (i=1; i<=totalnodes ;i++) {
			    AdjList[i] = NULL;
			}
            gets(str);
        } else if (flag == 'a') {
            //if 'a', get 'node1 node2 distance'
            scanf("%lld%lld%lld", &node1,&node2,&distance);
        	AddRoad(node1,node2,distance);
        	j++;
            if (j == totalarcs) {  //end input
                fclose(stdin);
                break;
            }
        }
    }
    //print the test result into file "testtresult.txt"
    freopen("testresult.txt", "w", stdout);
    srand((unsigned)time(NULL));
    start = clock();
    for (i=0; i<query_pairs; i++) {
    	//get startnode and endnode randomly
        startnode = rand()%totalnodes + 1; 
        endnode = rand()%totalnodes + 1; 
        if (heapflag == 1) {
        	DijkByFibheap(startnode, endnode);
		} else {
			DijkByMinheap(startnode, endnode);
		}   	
	}
	stop = clock();
	duration = ((double)(stop - start))/CLK_TCK;
	/*Output total run time in S*/
	printf("Whole run time by second is %lf\n", duration);
	fclose(stdout);
	return 0;
}

void AddRoad(long long node1, long long node2, long long distance) {
    //add road into the adjecency list
    struct Adj_List *s1 = (struct Adj_List *)malloc(sizeof(struct Adj_List)); 
    s1->adjnode = node2;
    s1->distance = distance;
    s1->next = NULL;
    if (!AdjList[node1]) {  //the first adjecent node  	
	    AdjList[node1] = s1;
	} else {
		s1->next = AdjList[node1]->next;
		AdjList[node1]->next = s1;
	}
}


//create Fibonacci heap
FibHeap create_fibheap()
{
    FibHeap heap = (FibHeap)malloc(sizeof(struct FibonacciHeap));
    heap->keyNum = 0;
    heap->maxDegree = 0;
    heap->min = NULL;
    heap->cons = NULL;
    return heap;
}

//insert node to the front of the double linked list root
void insertNode(FibNode node, FibNode root)
{
    node->left = root->left;
    root->left->right = node;
    node->right = root;
    root->left = node;
    return;
}

//insert node with idx as index, ikey as key
void insertKey(FibHeap heap, Type idx, Type ikey)
{
    FibNode node = (FibNode)malloc(sizeof(struct FibonacciNode));
    node->key = ikey;
    node->index = idx;
    node->degree = 0;
    node->right = node->left = node;
    node->parent = node->child = NULL;
    
    if(heap->keyNum == 0)
        heap->min = node;
    else 
    {
        //insert node to the front of the double linked list
        insertNode(node, heap->min);
        //update the minimum node
        if(node->key < heap->min->key)
            heap->min = node;
    }
    heap->keyNum ++;
    return;
}

FibNode getmin(FibHeap heap)
{
    if(!heap || !heap->min)
        return NULL;
    else
        return heap->min;
}

//delete the minimum node and adjust its root nodes
void deletemin(FibHeap heap)
{
    if(!heap || !heap->min)
        return;
    
    FibNode root = NULL;
    FibNode minnode = heap->min;

    //add the children of the minimum node to the root
    while(minnode->child != NULL)
    {
        root = minnode->child;
        root->mark = 0;
        //remove root
        root->left->right = root->right;
        root->right->left = root->left;

        //no more child
        if(root == root->right)
            minnode->child = NULL;
        else minnode->child = root->right;

        //add root to the heap
        insertNode(root, heap->min);
        root->parent = NULL;
    }

    //remove minnode
    minnode = heap->min;
    heap->min->left->right = heap->min->right;
    heap->min->right->left = heap->min->left;

    if(minnode == minnode->right)
        heap->min = NULL;
    else
    {
        heap->min = minnode->right;
        //adjust the heap
        fibconsolidate(heap);
    }
    
    heap->keyNum--;
    free(minnode);    
}

//add node to be the child of root
void fiblink(FibHeap heap, FibNode node, FibNode root)
{
    //remove node
    node->left->right = node->right;
    node->right->left = node->left;

    if(!root->child)
        root->child = node;
    else
        insertNode(node, root->child);

    node->parent = root;
    root->degree++;
    node->mark = 0;
}

//consolidate all the subtrees with the same degree
void fibconsolidate(FibHeap heap)
{
    Type tmp = heap->maxDegree;
    //the maxDegree of Fibnacci heap is log2N round up to an integar
    heap->maxDegree = (Type)(heap->keyNum) / log(2.0) + 1;

    if(tmp >= heap->maxDegree)
        return;

    //realloc space for cons
    Type Degree = heap->maxDegree + 1;
    heap->cons = (FibNode *)realloc(heap->cons, 
        sizeof(FibNode) * Degree);

    Type i;
    for(i = 0; i < Degree; i++)
        heap->cons[i] = NULL;
    
    FibNode x = NULL, y = NULL;
    Type dgr;
    
    while(heap->min != NULL)
    {   
        //find the heap with the minimum nodem and delete the min node
        x = getmin(heap);
        heap->min->left->right = heap->min->right;
        heap->min->right->left = heap->min->left;
        if(x->right == x)
            heap->min = NULL;
        else
            heap->min = x->right;
        x->left = x;
        x->right = x;

        //find the trees with the same degree
        for(dgr = x->degree; heap->cons[dgr] != NULL; dgr++)
        {
            y = heap->cons[dgr];
            //link the two trees
            if(x->key > y->key)
                fiblink(heap, x, y);
            else
                fiblink(heap, y, x);
            heap->cons[dgr] = NULL;
            dgr++;
        }
        heap->cons[dgr] = x;
    }

    //add nodes in the cons space to the heap
    for(i = 0; i < Degree; i++)
    {
        if(heap->cons[i] != NULL)
        {
            if(!heap->min)
                heap->min = heap->cons[i];
            else
            {
                insertNode(heap->cons[i], heap->min);
                if(heap->cons[i]->key < heap->min->key)
                    heap->min = heap->cons[i];
            }
        }
    }   
}
/*
//find node with skey as key
FibNode fibnodeSearch(FibNode root, Type skey)
{
    if(root == NULL)
        return NULL;

    FibNode tmp = root;
    FibNode result = NULL;

	do
	{
        //equal then found
		if (tmp->key == skey)
		{
			result = tmp;
			break;
		} 
        //search in its children
		else
		{
            result = fibnodeSearch(tmp->child, skey);
			if(result != NULL) 
				break;
		}	
		tmp = tmp->right;
	}while(tmp != root); //end when double linked list matches

    return result; 
}

//update the node with oldkey as former key, idx as index, newkey as new key
void fibnodeUpdate(FibHeap heap, Type oldkey, Type idx, Type newkey)
{
    if(!heap || !heap->min)
        return;
    //find the node with oldkey
    FibNode node = fibnodeSearch(heap->min, oldkey);
    //remove node
    node->left->right = node->right;
    node->right->left = node->left;
    //insert new key
    insertKey(heap, idx, newkey);
}
*/
//Implement Dijkstra's algorithm with Fibonacci heap.
Type dist[maxnode];
Type visit[maxnode];
void DijkByFibheap(long long startnode, long long endnode) {
	
    Type i;
    FibHeap fibheap = create_fibheap();
    FibNode minnode;

    for(i = 1; i <= totalnodes; i++)
    {
        dist[i] = infinity;
        visit[i] = 0;
    }
    dist[startnode] = 0;
    insertKey(fibheap, startnode, dist[startnode]);
    struct Adj_List *p = (struct Adj_List *)malloc(sizeof(struct Adj_List));

    while(fibheap->min != NULL)
    {
        minnode = getmin(fibheap);

        if(minnode->key <= dist[minnode->index])
        {
            Type minindex = minnode->index;
            visit[minindex] = 1;
            if(visit[endnode])
            {
                printf("The shortest distance from %lld to %lld is %lld\n", startnode, endnode, dist[endnode]);
	            return ;
            }
            
            for(p = AdjList[minindex]; p != NULL; p = p->next)
            {
                //visit[] = 1 means already reach the minimum distance
                if(visit[p->adjnode]) continue;
                //visit[] = 0 means have not visited
                if(!visit[p->adjnode])
                {
                    if(dist[p->adjnode] <= dist[minindex] + p->distance)
                        continue;
                    dist[p->adjnode] = dist[minindex] + p->distance;
                    insertKey(fibheap, p->adjnode, dist[p->adjnode]);
                }              
            }            
        }
        deletemin(fibheap);
    }

    printf("There's no way from %lld to %lld\n", startnode, endnode);
    return;  

/*
    Type minindex = startnode;
    struct Adj_List *p = (struct Adj_List *)malloc(sizeof(struct Adj_List));
    while(minindex != endnode)
    {
        //update the adjnodes
        for(p = AdjList[minindex]; p != NULL; p = p->next)
        {
            //visit[] = 2 means already reach the minimum distance
            if(visit[p->adjnode] == 1) continue;
            //visit[] = 0 means have not visited
            if(!visit[p->adjnode])
            {
                if(dist[p->adjnode] <= fibheap->min->key + p->distance)
                    continue;
                dist[p->adjnode] = fibheap->min->key + p->distance;
                fibnodeUpdate(fibheap, infinity, p->adjnode, dist[p->adjnode]);
                visit[p->adjnode] = 1;
            }              
        }

        //delete the minimum node and adjust the heap
        extractmin(fibheap);
        if(fibheap->min->key == infinity)
        {
            printf("There's no way from %lld to %lld\n", startnode, endnode);
    	    return;
        }
        visit[minindex] = 1;
    }

    printf("The shortest distance from %lld to %lld is %lld\n", startnode, endnode, dist[endnode]);
	return ;
*/
}

//Implement Dijkstra's algorithm with min heap.
long long dis[maxnode];  //record the minimal distance 
long long visited[maxnode];  //record the node be visited or not
void DijkByMinheap(long long startnode, long long endnode) {
	long long i;    
    long long position;  //record the currrent position during searching
    
    //initialize all the distance to be infinite
    //initialize all the nodes to be unvisited
    for (i=1; i<=totalnodes; i++) {          
        dis[i] = infinity;
        visited[i] = 0;
    }
    //set the distance of startnode itself to be 0
    dis[startnode] = 0;
    //set up the minheap of pair<distance, node>
    priority_queue<pair<long long,long long>,vector<pair<long long,long long> >,greater<pair<long long,long long> > > minheap;
    minheap.push(make_pair(0, startnode));
    while (!minheap.empty()) {
        //get the minimal distance in the heap
        pair<long long, long long> temp = minheap.top();
        minheap.pop();
        position = temp.second;
        if (position == endnode) {
            printf("The shortest distance from %lld to %lld is %lld\n", startnode, endnode, temp.first);
            return;
        } else {
            //if not visited, visit the node
            if (visited[position] == 1) {
                continue;
            } else {
                visited[position] = 1;
                //update the distance of nodes adjecent to current position
                struct Adj_List *p = AdjList[position];
                while (p) {
                    if (!visited[p->adjnode] && dis[p->adjnode] > dis[position] + p->distance) {
                        dis[p->adjnode] = dis[position] + p->distance;
                        minheap.push(make_pair(dis[p->adjnode], p->adjnode));
                    }
                    p = p->next;
                }
            }
        }        
    }
    if (dis[endnode] == infinity) {
    	printf("There's no way from %lld to %lld\n", startnode, endnode);
    	return;
	}
    return;
}


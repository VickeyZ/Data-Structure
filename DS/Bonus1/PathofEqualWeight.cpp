#include<bits/stdc++.h>
using namespace std;

struct TreeNode
{
    long Weight;
    vector<int> Children;
};
vector<TreeNode> Tree; /* Record all the nodes in an array of structure */
vector<vector<long>> Path; /* Record all the paths in a two-dimension array */
int n,m,s;
void AddNode(int a, int WeightRecord, vector<long> Stack) /* DFS for each node a */
{
    if(WeightRecord + Tree[a].Weight == s && Tree[a].Children.empty()) /* Reach the given weight and the leaf node */
    {
        Stack.push_back(Tree[a].Weight);/* Push into the stack */
        Path.push_back(Stack); /* Store all the element in the stack as one path */
        return;
    }    
    for(auto i:Tree[a].Children) /* Go through all the children of node a */
    {
        Stack.push_back(Tree[a].Weight);
        AddNode(i, WeightRecord + Tree[a].Weight, Stack);
        Stack.pop_back(); /* Pop out the element after calculated */
    }
}

int main()
{
    cin>>n>>m>>s;
    Tree.resize(n);
    for(int i = 0; i < n; i++)
        cin >> Tree[i].Weight;
    for(int i = 0; i < m; i++)
    {
        int name, num, child;
        cin>>name>>num;
        for(int j = 0; j < num; j++)
        {
            cin>>child;
            Tree[name].Children.push_back(child);
        }
    }
    vector<long> Stack; /* Record all the weight element along one path */

    AddNode(0, 0, Stack); /* Start from the root node  */

    sort(Path.begin(), Path.end(),greater<vector<long>>()); /* Quick sort using C++ standard library function */
    for(auto pathi:Path)
    {   
        for(int i = 0; i < pathi.size(); i++)
        {
            if(i == 0) cout<<pathi[0];
            else cout<<" "<<pathi[i];
        } 
        cout<<endl;
    }
    return 0;
}
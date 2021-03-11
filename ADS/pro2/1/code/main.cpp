#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <string>

/* This is a switch for switching the input method */
//#define _TEST_

#define MAX_N 1024

using namespace std;

int N, M;                                                       // the N and M
int G[MAX_N][MAX_N];                                            // the graph
int best_price;                                                 // the least price when the count is most
int best_cnt;                                                   // the most count of vertex in maximal clique
int cur_price;                                                  // current price
vector<int> cur_ans;                                            // current answer set
vector<int> maxcli_cnt;                                         // the count of maximal clique from i to M
vector<int> answer;                                             // the answer set
vector<int> id_to_fruit;                                        // the Transformation from id to fruit
vector<int> fruit_to_id;                                        // the Transformation from fruit to id
vector<int> id_to_price;                                        // the Transformation from id to price

void Bron_kerbosch();                                           // the function of Bron Kerbosch Algorithm

void Dfs(vector<int> &adj, int cur_cnt);                        // the function of dfs which is a part of Bron Kerbosch

void Print_ans();                                               // Print the final answer

int main() {
#ifdef _TEST_                                                   // file stream input and output
    freopen("./test_case.txt", "r", stdin);                         // You can store the input data in this file
    freopen("./output.txt", "w", stdout);                       // the output data will be output into this file
#endif
    int fruit1, fruit2;                                         // temp variable
    int fruit, price;                                           // temp variable
    vector<pair<int, int> > tips;                               // temp variable used to store tips
    /* initial the global variables */
    id_to_fruit.resize(MAX_N);
    fruit_to_id.resize(MAX_N);
    id_to_price.resize(MAX_N);
    maxcli_cnt.resize(MAX_N);

    cin >> N >> M;                                              // read in the N and M
    for (int i = 0; i < 1024; ++i) {                            // initial the fruit_to_id array
        fruit_to_id[i] = -1;                                    // -1 means this index is not used
    }

    for (int i = 0; i < N; ++i) {
        cin >> fruit1 >> fruit2;                                // read the tips
        tips.emplace_back(make_pair(fruit1, fruit2));
    }

    for (int i = 0; i < M; ++i) {
        cin >> fruit >> price;                                  // read the fruit and price
        fruit_to_id[fruit] = i;                                 // store the mapping relations
        id_to_price[i] = price;                                 // store the mapping relations
        id_to_fruit[i] = fruit;                                 // store the mapping relations
    }

    for (int i = 0; i < N; ++i) {
        G[i][i] = 1;
        /* if A and B is adjacent then G[A][B] = G[B][A] = 1  */
        if (fruit_to_id[tips[i].first] != -1 && fruit_to_id[tips[i].second] != -1) {
            G[fruit_to_id[tips[i].first]][fruit_to_id[tips[i].second]] = 1;
            G[fruit_to_id[tips[i].second]][fruit_to_id[tips[i].first]] = 1;
        }
    }

    Bron_kerbosch();
    Print_ans();
}

void Bron_kerbosch() {
    /* initial the best_cnt */
    best_cnt = 0;
    /* this loop is used to call the dfs from i = M-1 to i = 0 (decrease) */
    for (int i = M - 1; i >= 0; --i) {
        vector<int> adj;                                        // the adjacent array of cur vertex
        for (int j = i + 1; j < M; ++j) {
            if (G[i][j] == 0)
                adj.emplace_back(j);                            // add the vertex which is adjacent to i into this array
        }
        cur_ans.emplace_back(i);                                // "DO"
        cur_price += id_to_price[i];                            // "DO"
        Dfs(adj, 1);                                // call the dfs
        cur_price -= id_to_price[i];                            // "UNDO"
        cur_ans.pop_back();                                     // "UNDO"
        maxcli_cnt[i] = best_cnt;                               // update the maxcli_cnt
    }
}

void Dfs(vector<int> &adj, int cur_cnt) {
    /* if adj is empty, means we get a maximal clique */
    if (adj.empty()) {
        /* if this maximal clique is bigger than answer, update the answer
         * if it is as bigger as the answer, compare the price */
        if (cur_cnt > best_cnt || (cur_cnt == best_cnt && cur_price < best_price)) {
            answer.assign(cur_ans.begin(), cur_ans.end());
            best_price = cur_price;
            best_cnt = cur_cnt;
        }
        /* other situation has been pruned */
        return;
    }

    for (int i = 0; i < adj.size(); i++) {
        int cur_vertex = adj[i];

        if (cur_cnt + M - cur_vertex < best_cnt)return;         // prune
        if (cur_cnt + maxcli_cnt[cur_vertex] < best_cnt)return; // prune

        vector<int> new_adj;                                    // create a new vector to store the adj of cur vertex
        for (int j = i + 1; j < adj.size(); ++j) {
            if (G[cur_vertex][adj[j]] == 0)                     // if adj[j] is adj to cur vertex, add it into vector
                new_adj.emplace_back(adj[j]);
        }

        cur_ans.emplace_back(cur_vertex);                       // "DO"
        cur_price += id_to_price[cur_vertex];                   // "DO"
        Dfs(new_adj, cur_cnt + 1);                              // call dfs recursive
        cur_price -= id_to_price[cur_vertex];                   // "UNDO"
        cur_ans.pop_back();                                     // "UNDO"
    }
}


void Print_ans() {
    vector<int> answer_out;                                     // create a vector to store the fruit of answer
    for (auto &iter:answer)                                     // convert id to fruit
        answer_out.emplace_back(id_to_fruit[iter]);
    sort(answer_out.begin(), answer_out.end());                 // sort the answer set
    cout << best_cnt << endl;                                   // print the number of answer set
    for (auto &iter:answer_out)                                 // print item of the answer set
        cout << setw(3) << setfill('0')
             << iter << ((iter == answer_out.back()) ? "" : " ");
    cout << endl << best_price;                                 // print the total price of answer set
}


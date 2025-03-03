#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;
struct Item {
    int profit;
    int weight;
    double ratio; 
};
struct Node {
    int level;      
    int profit;     
    int weight;     
    double bound;   
};
struct Compare {
    bool operator()(const Node &a, const Node &b) {
        return a.bound < b.bound;
    }
};
double bound(Node u, int W, Item *items, int n) {
    if (u.weight >= W) return 0; 
    double bound = u.profit;
    int totalWeight = u.weight;
    int i = u.level + 1;
    while (i < n && totalWeight + items[i].weight <= W) {
        totalWeight += items[i].weight;
        bound += items[i].profit;
        i++;
    }
    if (i < n) {
        bound += (W - totalWeight) * items[i].ratio;
    }
    return bound;
}
int branch_and_bound(int W, Item *items, int n) {
    priority_queue<Node, deque<Node>, Compare> pq;
    Node root = {-1, 0, 0, bound({-1, 0, 0}, W, items, n)};
    pq.push(root);
    int maxProfit = 0;
    while (!pq.empty()) {
        Node u = pq.top();
        pq.pop();
        if (u.bound <= maxProfit) continue; Node v;
        v.level = u.level + 1;
        v.weight = u.weight + items[v.level].weight;
        v.profit = u.profit + items[v.level].profit;
        if (v.weight <= W && v.profit > maxProfit) {
            maxProfit = v.profit;
        }
        v.bound = bound(v, W, items, n);
        if (v.bound > maxProfit) {
            pq.push(v);
        }
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound = bound(v, W, items, n);
        if (v.bound > maxProfit) {
            pq.push(v);
        }
    }
    return maxProfit;
}
int main() {
    int n = 4; // Number of items
    Item *items = new Item[n];
   items[0] = {60, 10};
    items[1] = {100, 20};
    items[2] = {120, 30};
    items[3] = {30, 5};
    int W = 50;
    for (int i = 0; i < n; i++) {
        items[i].ratio = (double)items[i].profit / items[i].weight;
    }
    sort(items, items + n, [](const Item &a, const Item &b) {
        return a.ratio > b.ratio;
    });
    int maxProfit = branch_and_bound(W, items, n);
    cout << "Maximum Profit: " << maxProfit << endl;
    delete[] items;
    return 0;
}

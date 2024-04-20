//Run code using this command - g++ -fopenmp src.cpp -o dfsbfs

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

class Graph {
   int V; // Number of vertices
   vector<int> *adj; // Adjacency list

public:

   Graph(int V);
   void addEdge(int v, int w);
   void parallelBFS(int s);
   void parallelDFS(int s);
};

Graph::Graph(int V) {
   this->V = V;
   adj = new vector<int>[V];
}

void Graph::addEdge(int v, int w) {
   adj[v].push_back(w);
   adj[w].push_back(v);
}

void Graph::parallelBFS(int s) {
   bool *visited = new bool[V];
   for (int i = 0; i < V; i++) {
       visited[i] = false;
   }

   queue<int> q;
   visited[s] = true;
   q.push(s);
  
   while (!q.empty()) {
       s = q.front();
       cout << s << " ";
       q.pop();
       #pragma omp parallel for
       for (auto i = adj[s].begin(); i != adj[s].end(); ++i) {
           if (!visited[*i]) {
               visited[*i] = true;
               q.push(*i);
           }
       }
   }
}

void Graph::parallelDFS(int s) {
   bool *visited = new bool[V];
   for (int i = 0; i < V; i++) {
       visited[i] = false;
   }
   stack<int> st;
   st.push(s);
   while (!st.empty()) {
       s = st.top();
       st.pop();
       if (!visited[s]) {
           cout << s << " ";
           visited[s] = true;
       }

       #pragma omp parallel for
       for (auto i = adj[s].begin(); i != adj[s].end(); ++i) {
           if (!visited[*i]) {
               st.push(*i);
           }
       }
   }
}

int main() {
   Graph g(4);
   g.addEdge(0, 1);
   g.addEdge(0, 2);
   g.addEdge(1, 2);
   g.addEdge(2, 0);
   g.addEdge(2, 3);
   g.addEdge(3, 3);
   cout << "Parallel Breadth First Traversal (starting from vertex 2):\n";
   g.parallelBFS(2);
   cout << "\nParallel Depth First Traversal (starting from vertex 2):\n";
   g.parallelDFS(2);
   return 0;
}

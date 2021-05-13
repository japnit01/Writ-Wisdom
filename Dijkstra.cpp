#include <bits/stdc++.h>
using namespace std;

class Graph 
{
    public:

    int V;
    vector<list<pair<int,int>>> adj;

    Graph(int V);
    void addEdge(int u,int v,int w);
    void printGraph();
    void printShortestPath(int source, int destination);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = vector<list<pair<int,int>>> (V, list<pair<int,int>>());
}

void Graph::addEdge(int u, int v,int w)
{
    adj[u].push_back(make_pair(v,w));
    adj[v].push_back(make_pair(u,w));
}

void Graph::printGraph()
{
    for(int i=0;i<V;i++)
    {
        cout<<i<<" ";

        for(auto ptr : adj[i])
        {
            cout<<ptr.first<<" ";
        }
        cout<<"\n";
    }
}

// Using Dijkstra's Algorithm
void Graph::printShortestPath(int source, int destination)
{
    vector<int> connected(V);
    stack<int> s;
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq; // min heap
    vector<int> distance(V,INT_MAX);

    pq.push(make_pair(0,source));
    distance[source] = 0;

    while(!pq.empty())
    {
        int node = pq.top().second, u, wt;
        pq.pop();

        for(auto ptr : adj[node])
        {
            u = ptr.first;
            wt = ptr.second;

            if(distance[node] + wt < distance[u])
            {
                distance[u] = distance[node] + wt;
                pq.push(make_pair(distance[u],u));
                connected[u] = node;
            }
        }
    }

    int i = destination;
    s.push(i);

    while(i != source)
    {
        i = connected[i];
        s.push(i);
    }

    cout<<"\nDelivery man will travel from "<<source<<" to "<<destination<<" via the following route :- \n\n\t";

    while(!s.empty())
    {
        cout<<s.top();
            
        if(s.size() > 1)
        {
            cout<<" -> ";
        }

        s.pop();
    }

    cout<<"\n\nTotal distance : "<<distance[destination]<<"\n\n";

}

int main()
{
    int V = 6;
    Graph g(V);
    g.addEdge(0,3,4);
    g.addEdge(1,2,2);
    g.addEdge(2,3,7);
    g.addEdge(3,4,3);
    g.addEdge(4,1,5);
    g.addEdge(1,0,1);
    g.addEdge(3,5,9);
    g.addEdge(1,5,6);
    g.addEdge(5,4,1);

    // Dijkstra's algorithm
    g.printShortestPath(0,5);

    return 0;
}

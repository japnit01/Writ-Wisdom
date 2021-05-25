#include<bits/stdc++.h>
using namespace std;
class Graph
{
    public:

    int V;
    vector<vector<float>> adj;

    Graph(int v)
    {
        this->V = v;
        this->adj.resize(v);
        
        for(int i=0;i<v;i++)
        {
            this->adj[i].resize(v);
        }
    }
};


   map<string,int> m = {
    {"murthal",0},{"dtu entrance",1},{"dtu sports ground",2},{"dtu library",3},{"dtu oat",4},
    {"samaypur badli",5},{"rithala",6},{"pitampura",7},{"netaji subhash place",8},{"shalimar bagh",9},
    {"azadpur",10},{"jharoda",11},{"keshav puram",12},{"ashok vihar",13},{"gtb nagar",14},
    {"kashmere gate",15},{"signature bridge",16},{"yamuna vihar",17},{"welcome",18},{"dilshad garden",19},
    {"inderlok",20},{"bahadurgarh",21},{"peeragarhi",22},{"punjabi bagh",23},{"ashoka park main",24},
    {"kirti nagar",25},{"rajouri garden",26},{"janakpuri",27},{"dwarka",28},{"najafgarh",29},
    {"connaught place",30},{"new delhi railway station",31},{"bangla sahib",32},{"parliament",33},{"india gate",34},
    {"supreme court",35},{"red fort",36},{"mandi house",37},{"yamuna bank",38},{"vaishali",39},
    {"mayur vihar",40},{"botanical garden",41},{"noida",42},{"lajpat nagar",43},{"nehru place",44},
    {"faridabad",45},{"greater kailash",46},{"hauz khas",47},{"malviya nagar",48},{"qutub minar",49},
    {"gurugram",50},{"iit delhi",51},{"ina delhi hatt",52},{"dhaula kuan",53},{"igia",54},{"botanical park",55}
                        };

int main()
{
    string startpt,endpt;

    Graph g(56);
    g.addEdge(0,1,43);
    g.addEdge(0,5,42);
    g.addEdge(1,5,2);
    g.addEdge(1,6,2);
    g.addEdge(6,7,3);
    g.addEdge(7,8,3);
    g.addEdge(5,11,9.5);d
    g.addEdge(5,10,11);
    g.addEdge(10,11,6);
    g.addEdge(8,9,2);
    g.addEdge(9,10,3);
    g.addEdge(11,16,4);
    g.addEdge(14,16,4);
    g.addEdge(13,14,4.5);
    g.addEdge(10,14,4);
    g.addEdge(10,13,4);
    g.addEdge(9,13,4);
    g.addEdge(12,13,5);
    g.addEdge(8,12,2);
    g.addEdge(14,15,10);
    g.addEdge(15,20,12);
    g.addEdge(12,20,5);
    g.addEdge(16,17,5);
    g.addEdge(17,19,6);
    g.addEdge(18,19,4.5);
    g.addEdge(15,18,5);
    g.addEdge(7,22,6);
    g.addEdge(22,23,4);
    g.addEdge(23,24,3);
    g.addEdge(20,24,2);
    g.addEdge(21,22,16);
    g.addEdge(24,25,3.5);
    g.addEdge(25,26,4.5);
    g.addEdge(23,26,4.5);
    g.addEdge(26,27,5.5);
    g.addEdge(27,28,7);
    g.addEdge(28,29,8);
    g.addEdge(53,54,7.5);
    g.addEdge(27,54,12);
    g.addEdge(26,53,8.5);
    g.addEdge(51,54,16);
    g.addEdge(47,51,1.5);
    g.addEdge(46,47,7);
    g.addEdge(44,45,22);
    g.addEdge(44,46,2);
    g.addEdge(47,48,2.5);
    g.addEdge(48,49,3);
    g.addEdge(48,50,22);
    g.addEdge(43,46,3);
    g.addEdge(43,44,2.5);
    g.addEdge(40,43,11);
    g.addEdge(38,40,7);
    g.addEdge(38,39,12);
    g.addEdge(37,38,6);
    g.addEdge(35,37,2);
    g.addEdge(34,35,3);
    g.addEdge(33,34,3.5);
    g.addEdge(36,37,4);
    g.addEdge(34,36,6);
    g.addEdge(15,30,6);
    g.addEdge(20,30,8.5);
    g.addEdge(25,30,8);
    g.addEdge(30,37,2.5);
    g.addEdge(30,43,12);
    g.addEdge(32,33,1.5);
    g.addEdge(30,32,1);
    g.addEdge(30,31,1.5);
    g.addEdge(15,31,4);
    g.addEdge(31,53,10);
    g.addEdge(52,53,8);
    g.addEdge(52,43,3.5);
    g.addEdge(47,52,3);
    g.addEdge(30,52,8);
    g.addEdge(43,55,12);
    g.addEdge(42,55,11);
    g.addEdge(40,55,11);
    g.addEdge(44,55,12);

}
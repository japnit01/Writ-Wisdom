#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>
using namespace std;

int E_count = 0,P_count = 0;
string user_path = "user_files/",decompress_path = "decompressed_files/",initiate_path = "files/";
vector<string> place;
class book{
    public:
    string id;
    string name;
    string author;
    string subject;
    string tag;
    int price;
    int quantity;
    string type;
    string filename;

    book(){}
    book(string n, string a, string s, string t, string tp, int p){
        if(tp == "ebook"){
            id = "EB" + to_string(E_count++);
        }
        else if( tp == "pbook"){
            id = "PB" + to_string(P_count++);
        }        
        name = n;
        author = a;
        subject = s;
        tag = t;
        type = tp;
        price = p;
        quantity = 0;
        filename = "";
    }
};

class Graph {
    public:

    // number of vertices
    int V;
    // adjacency list for graph representation
    vector<list<pair<int,int>>> adj;

    Graph(int V);
    void addEdge(int u,int v,int w);
    void printGraph();
};

// class graph constructor
Graph::Graph(int V)
{
    this->V = V;
    adj = vector<list<pair<int,int>>> (V, list<pair<int,int>>());
}

// function to add a weighted edge between two vertices
void Graph::addEdge(int u, int v,int w)
{
    adj[u].push_back(make_pair(v,w));
    adj[v].push_back(make_pair(u,w));
}

// function to print the graph
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

struct Trienode{
        Trienode *children[255];
        bool eow;
        string id;
    };

Graph g(56);
map<string,int> placesTonodes;
map<int,string> nodesToplaces;
map<string,book> library;
map<string,int> Cart;
vector<book> Ebook;
vector<book> Pbook;
vector<string> searchresults;
vector<string> disc = {"SAVE20","SAVE50","BMSBEG"};

map<char, string> codes;
string compressed = "",dectext = "";;


Trienode *newnode(){
    Trienode *root = new Trienode;
    root->eow = false;
    root->id = "PBEB";
    for(int i=0;i<255;i++)
    {
        root->children[i] = NULL;
    }
    return root;
}

void insertkey(Trienode *root,string key,string endkey){
    Trienode *temp = root;
    for(int i=0;i<key.size();i++)
    {    //Calculating the ASCII value of char
        int ind = key[i] - '\0';
        //Checking wether the index is null or not
        if(!temp->children[ind])
        {
            temp->children[ind] = newnode();
        }
        temp = temp->children[ind];
    }
    //Last node marked as eow and given the id
    temp->eow = true;
    temp->id = endkey;
}

bool check(Trienode *root)
{
    for(int i=0;i<255;i++)
    {
        if(root->children[i])
        {
            return false;
        }
    }
    return true;
}

void options(Trienode *root,string s)
{
    if(root->eow){   
        //searches  for places
        if(root->id == "place")
        {
            searchresults.push_back(s);            
        }
        else //searches for names and author
        {
            searchresults.push_back(root->id);
        }   
    }    
    //if its last node
    if(check(root))
    {
        return ;
    }
    //backtracking
    for(int i=0;i<255;i++)
    {
        if(root->children[i])
        {   
            s.push_back(0+i);
            options(root->children[i],s);
            s.pop_back();
        }
    }
}

void suggestion(Trienode *root,string s)
{
    Trienode *reroot = root;
    //Traversing over query
    for(int i=0;i<s.size();i++)
    {  
        int ind = s[i] - '\0';
        //if the ind is null there is no book
        if(!reroot->children[ind])
        {
            cout<<"No book found\n";
            return ;
        }
        reroot = reroot->children[ind];
    }
    bool checklast = check(reroot);
    // if the query has ended and eow and children array 
    // has all index null 
    if(checklast && reroot->eow){
        //serches for places
        if(root->id == "place")
        {
            searchresults.push_back(reroot->id);            
        }
        else //searches for names and author
        {   
            searchresults.push_back(reroot->id);
            cout<<"No other book found\n";
        }
        return;
    }
    else{ //auto completion of query
        options(reroot,s);
        return;
    }
}

struct minheapnode
{
    char l;
    int freq;
    minheapnode *left, *right;

    minheapnode(char l, int freq)
    {
        left = right = NULL;
        this->l = l;
        this->freq = freq;
    }
};

struct cmp
{
    bool operator()(minheapnode *a, minheapnode *b)
    {
        return a->freq > b->freq;
    }
};

void text_encode(string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        compressed += codes[text[i]];
    }
}

void codestable(minheapnode *root, string str)
{
    if (!root)
        return;

    if (root->l != '#')
    {
        //cout<<root->l<<": "<<str<<"\n";
        codes[root->l] = str;
    }

    codestable(root->left, str + "0");
    codestable(root->right, str + "1");
}

void preorder(minheapnode *root, string str, string a)
{
    if (!root)
    {
        return;
    }

    compressed += a;
    if (root->l != '#')
    {
        compressed += root->l;
    }

    preorder(root->left, str, "0");
    preorder(root->right, str, "1");
}

void huffman(map<char, int> freq)
{
    priority_queue<minheapnode *, vector<minheapnode *>, cmp> q;
    //Storing all characters in the priority queue
    for (auto it = freq.begin(); it != freq.end(); it++){
        q.push(new minheapnode(it->first, it->second));
    }
    minheapnode *top, *right, *left;
    //Keep on traversing until one node remains
    while (q.size() != 1){
        // node with least freq is queue
        left = q.top();
        q.pop();
        // node with least freq is queue
        right = q.top();
        q.pop();
        //sum of freq of left and right is assigned to top
        top = new minheapnode('#', left->freq + right->freq);

        top->left = left;
        top->right = right;
        q.push(top);
    }

    codestable(q.top(), "");
    string a;
    preorder(q.top(), "", a);
}

void calcfreq(string text)
{
    map<char, int> freq;
    for (int i = 0; i < text.size(); i++)
    {
        freq[text[i]]++;
    }
    cout<<"\n";
    huffman(freq);
}

void decodetext(string text,minheapnode *root)
{   
    minheapnode *temp = root;
    
    for(int i=0;i<text.size();i++)
    {
        //until leaf node not found
        root = temp;
        while(root->l == '#' && i<text.size())
        {   //left edge
            if(text[i] == '0')
            {   
                i++;
                root = root->left;
            }
            else if(text[i] == '1')
            {   //right edge
                i++;
                root = root->right;
            }
        }
        //adding the leaf character
        dectext+=root->l;
        i--;
    }
}


int dectreec = 0;
void decodetree(string text,int n,minheapnode* reroot)
{   
    //for leaf node
    if(reroot->l !='#'){
        return ;
    } 
    //assinging char to leaf node
    if(text[dectreec]!='1' && text[dectreec]!='0'){
        reroot->l = text[dectreec];
        dectreec++;
        return ;
    }
    //left edge
    if(text[dectreec] == '0'){
        reroot->left = new minheapnode('#',-1);
        dectreec++;
        decodetree(text,n,reroot->left);
    
    }
    //right edge
    if(text[dectreec] == '1'){
       reroot->right = new minheapnode('#',-1);
       dectreec++;
       decodetree(text,n,reroot->right); 
    }    
}

void decodehuffman(string text)
{   
    minheapnode *reroot;
    reroot = new minheapnode('#',-1);
    //Tree size
    int n = stoi(text.substr(0,32),0,2);
    //Rebuilding Huffman Tree
    decodetree(text.substr(32,n),n,reroot);
    //Print the codes from the tree
    codestable(reroot,"");
    minheapnode *temp = reroot;
    int textsize = text.size()-32-n;
    //decoding the text
    decodetext(text.substr(32+n,text.size()-32-n),reroot);
}

void download(vector<string> eb,int e)
{   
    for(int i=0;i<e;i++){   
        string inputfile,outputfile;
        
        inputfile = library[eb[i]].filename;
        ifstream inpfile;
        string s="",temp;
        //opening the compressed file
        inpfile.open("compressed_files/" + inputfile,ios::in);
    
        if(!inpfile)
        {
            cout<<"File not found\n";
        }
        else{
            while(!inpfile.eof()){
                getline(inpfile,temp);
                s+=temp;
            }
            inpfile.close();
            codes.clear();
            dectext="";
            dectreec = 0;
            //Decoding the bit string of the file
            decodehuffman(s);
            ofstream outpfile;
            
            if(inputfile.substr(inputfile.size()-15,inputfile.size()) == "_compressed.txt")
            {    outputfile = inputfile.substr(0,inputfile.size()-15) + "_decompressed.txt";}
            else
            {    outputfile = inputfile + "_decompressed.txt";}
            outpfile.open(user_path + outputfile,ios::out);
        
            if(!outpfile)
            {
                cout<<"File not created\n";
            }
            else
            {
                outpfile<<dectext<<endl;
                //writing text on output file
                cout<<inputfile<<" downloaded\n";
            }
            outpfile.close();
        }   
    }
}

// Using Dijkstra's Algorithm
void deliver(vector<list<pair<int,int>>> adj, int V, string src, string dest)
{
    cout<<src<<" "<<dest<<"\n";

    // converting source and destination places' names to nodes
    int source = placesTonodes[src];
    int destination = placesTonodes[dest];

    cout<<placesTonodes[src]<<" "<<placesTonodes[dest]<<"\n";
    
    // array storing the previous connected node on the shortest path for each node
    vector<int> connected(V);
    stack<int> s;

    // min heap storing distance-node pairs sorted according to their distances from the source
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq; 

    // array storing the distances of all the nodes from the source node
    vector<int> distance(V,INT_MAX);

    // inserting source into the priority queue with 0 distance from itself
    pq.push(make_pair(0,source));
    distance[source] = 0;

    while(!pq.empty())
    {
        // popping and storing the node with min distance from the source 
        int node = pq.top().second, u, wt;
        pq.pop();

        // traversing through all the nodes connected to the current node
        for(auto ptr : adj[node])
        {
            u = ptr.first;
            wt = ptr.second;

            // updating the distance of the node connected to current node 
            // if distance to the connected node via the current node is less than its initial distance
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

    // traversing back to the source node via the shortest path
    while(i != source)
    {
        i = connected[i];
        s.push(i);
    }

    cout<<"\nDelivery man will travel from "<<source<<" to "<<destination<<" via the following route :- \n\n\t";

    // printing the shortest path 
    while(!s.empty())
    {
        cout<<nodesToplaces[s.top()];
            
        if(s.size() > 1)
        {
            cout<<" -> ";
        }

        s.pop();
    }

    // printing the shortest distance of destination from the source
    cout<<"\n\nTotal distance : "<<distance[destination]<<"\n\n";

}

bool billing(int p,int e,float sum,vector<string> eb,vector<string> pb,Trienode *placeroot){
    codes.clear();
    cout<<"\n\nOrder Summary\n";
    cout<<"Item Ordered: "<<p+e<<"\n";
    cout<<"Ebook: "<<e<<"\n";
    cout<<"Printed Book: "<<p<<"\n";
    cout<<"Total Price: "<<sum<<"\n";
    cout<<"GST(18%): " <<sum*0.18<<"\n";
    cout<<"Total Amount: "<<sum + sum*0.18<<"\n";

    if(sum != 0)  
    {
        cout<<"Avail Discount Coupons: ";
        string coupon;
        cin>>coupon;
        if(coupon == "SAVE20")
        {
            sum*=0.8;        
        }
        else if(coupon == "SAVE50")
        {
            sum*=0.5;
        }
        else if(coupon == "BMSBEG")
        {
            sum*=0.6;
        }
        cout<<"\nTotal: "<<sum<<"\n";
        cout<<"GST(18%): " <<sum*0.18<<"\n";
        cout<<"Total Amount: "<<sum + sum*0.18<<"\n";   
    }

    string source = "Warehouse";
    string destination;
    if(p!=0)
    {   cout<<"If you want to search for location enter (s)";
        cout<<"\nEnter the place for collecting the order : ";
        getline(cin>>ws,destination);

        if(destination == "s")
        {
            string keyword;
            cout<<"\nSearch Location: ";
            getline(cin>>ws,keyword);
            transform(keyword.begin(),keyword.end(),keyword.begin(),::tolower);
            searchresults.clear();
            suggestion(placeroot,keyword);
            for(int i=0;i<searchresults.size();i++)
            {
                cout<<i+1<<" "<<searchresults[i]<<"\n";
            }

            cout<<"\nEnter the place for collecting the order : ";
            getline(cin>>ws,destination);
        }
        // cout<<destination;
        // cout<<placesTonodes[destination]<<"\n";
    }

    int op;
    cout<<"\n1. UPI(Patym/Phonepe/Gpay)\n";
    cout<<"2. Credit Card\n";
    cout<<"3. Debit Card\n";
    cout<<"Enter Choice: ";
    cin>>op;
    
    if(op == 1)
    {
        string add;
        cout<<"\nEnter UPI Address: ";
        cin>>add;
    }
    else if(op == 2 || op == 3)
    {   string cardno,cvv,cardh;
        int expm,expy;
        cout<<"\nName of Card Holder: ";
        cin>>cardh;
        cout<<"\nCredit Card No.: ";
        cin>>cardno;
        cout<<"\nExpiry Month: ";
        cin>>expm;
        cout<<"\nExpiry Year: ";
        cin>>expy;
        cout<<"\nCVV: ";
        cin>>cvv;
    }
    else{
        cout<<"Transaction Failed!!!\n";
        return false;
    }
        cout<<"Transaction Succesfull!!!\n";
    
    if(p!=0)
    {
        deliver(g.adj,g.V,source,destination);
    }
    if(e!=0)
    {
        download(eb,e);
    }
    return true;
}

bool compareByName(book a, book b)
{
    return (a.name < b.name);
}

bool compareByPricehl(book a, book b)
{
    return (a.price < b.price);
}

bool compareByPricelh(book a,book b)
{
    return (a.price>b.price);
}

void info(Trienode *root,int a,string inputfile,string name, string author, string subject, string tag,string type,int price){   
    string path;
    if(a ==1)
        path = initiate_path; 
    else if(a ==0)
        path = user_path;
    string outputfile;
    compressed = "";
    ifstream inpfile;
    string s = "", temp;
    //opening the input file
    inpfile.open(path + inputfile, ios::in);
    if (!inpfile){
        cout << "File not found\n";
    }
    else{
        while (!inpfile.eof()){
            getline(inpfile, temp);
            s += temp;
        }
        inpfile.close();
        codes.clear();
        //calculating frequrncy
        calcfreq(s);
        int n = compressed.size();
        string tree_size = "";
        // size of tree into 32 bit int
        for (int i = 31; i >= 0; i--){
            int k = n >> i;
            if (k & 1)
                tree_size += "1";
            else
                tree_size += "0";}
        //32bit int for size of tree + tree in pre order trvarsal
        compressed = tree_size + compressed;
        //encoding text with codes generted
        text_encode(s);
        ofstream outpfile;
        if (inputfile.size()>4 && inputfile.substr(inputfile.size() - 4, 4) == ".txt")
        {
            outputfile = inputfile.substr(0, inputfile.size() - 4) + "_compressed.txt";
        }
        else
        {
            outputfile = inputfile + "_compressed.txt";
        }
        outpfile.open("compressed_files/" + outputfile, ios::out);
        if (!outpfile){
            cout << "File not created\n";
        }
        else{
            //writing text in output file
            outpfile << compressed << endl;
            cout << "File Uploaded\n";
            //calling the contructor 
            book b(name, author, subject, tag, "ebook",price);
            b.filename = outputfile;
            Ebook.push_back(b);
            library[b.id] = b;
            //Adding it in trie for search
            insertkey(root,b.name,b.id);}
        outpfile.close();
        codes.clear();
        }
    }

void downloadfile()
{
    map<int,string> display;
    cout << "\nBOOK BANK\n";
    int count = 1;
    for (int i = 0; i < Ebook.size(); i++)
    {
        if (Ebook[i].price == 0)
        {
            cout << "\n";
            cout << Ebook[i].subject<<"\n";
            cout << "Name: "<<Ebook[i].name<<"\n";
            cout << Ebook[i].tag<<"\nAuthor: "<<Ebook[i].author<<"\n";
            cout << "Price: "<<Ebook[i].price<<"\n";
            cout << "\nAdd to Cart --> "<<count<<"\n";
            cout << "-----------------------------";
            display[count] = Ebook[i].id;
            //cout<<count<<" "<<Ebook[i].id<<"\n";
            count++;
        }
    }

    int c;
    cout << "\n Exit --> " << count;
    int exit = count;
    char ch = 'y';
    
    do
    {
        cout << "\nEnter Choice: ";
        cin >> c;
        if (c <= exit)
        {
            if (c == exit)            {
                return ;
            }
            else
            {  
                string id = display[c];
                //cout<<c<<" "<<id<<"\n";
                if( Cart[id] == 0)
                {
                    Cart[id]++;
                }
                else
                {
                    cout<<"\nOnly one can be added";
                }
            }
        }
        else
        {
            cout << "\nPlease input correct choice";
        }

    } while (ch == 'y');
}

void addfile(Trienode *root)
{
    cout << "\nAdd Book\n";
    string name, author, subject, tag;
    cout << "Name: ";
    getline(cin>>ws,name);
    cout << "\nAuthor: ";
    getline(cin>>ws,author);
    cout << "\nSubject: ";
    getline(cin>>ws,subject);
    cout << "\nTag (Class/Engineering/Fiction/Mystery): ";
    getline(cin>>ws,tag);

    string inputfile;
    cout << "Enter the name of file: \n";
    getline(cin>>ws,inputfile);
    info(root,0,inputfile,name, author, subject, tag,"ebook",0);
}

void cart(Trienode *placeroot)
{
    vector<string> pb;
    vector<string> eb;
    cout << "\nCart\n";
    float sum = 0;
    int p = 0,e = 0;
    for(auto it = Cart.begin();it != Cart.end();it++)
    {       string id = it->first;
            cout<<"ID: "<<id<<"\n";
            cout<< "Name: " <<library[id].name<<"\n";
            cout<< "Price: "<<library[id].price<<"\n";
            cout<< "\nItem count "<<it->second<< "\n";
            cout<< "-----------------------------\n";
            sum+=((library[id].price)*it->second);
            if(library[id].type == "ebook")
            {
                e+=it->second;
                eb.push_back(id);
            }
            else
            {
                p+=it->second;
                pb.push_back(id);

            }
    }
    cout<<"Total: "<<sum<<"\n";
    char ch = 'y';
    do
    {
        //cout<<"1. Add to Cart\n";
        cout<<"1. Proceed to buy\n";
        cout<<"2. Edit Cart\n";
        cout<<"3. Exit\n";
        int choice;
        cout<<"Enter Choice: ";
        cin>>choice;

        if(choice == 1)
        {   
            if(billing(p,e,sum,eb,pb,placeroot))
            {
                Cart.clear();
                break;
            }
        }
        else if(choice == 2)
        {
            cout<<"\n";
            cout<<"To delete a item enter it's ID\n";
            cout<<"Enter the code: ";
            string q;
            cin>>q;
            if(Cart.find(q) != Cart.end())
            {
                Cart[q]--;
                if(Cart[q] == 0)
                {
                    Cart.erase(q);
                }
            }
            else{
                cout<<"\nNo item found\n";
            }
        }
        else if(choice == 3)
        {
            return ;
        }

    }while(ch == 'y');
}

void bookbank(Trienode *root)
{
    cout << "\n\nBook Bank\n";
    cout << "1. Upload Books\n";
    cout << "2. Download Books\n";
    int choice1;
    cout << "Enter Choice: ";
    cin >> choice1;
    if (choice1 == 1)
    {
        addfile(root);
    }
    else if (choice1 == 2)
    {
        downloadfile();
    }
}

void orderbook()
{
    cout << "\nBooks\n";
    int count = 1;
    map<int,string> display;

    string books = "No";
    int filter;
    vector<book> sortBooks;

    cout<<"\nFilter by book(Ebook/Pbook/Genre/No): ";
    cin>>books;
    transform(books.begin(),books.end(),books.begin(),::tolower);

    if(books == "genre")
    {
        cout<<"Enter by Genre: ";
        cin>>books;
    }

    cout<<"\nSort by: 1.Name   2.Price low to high   3.Price high to low :";
    cin>>filter;

    
    //transform(filter.begin(),filter.end(),filter.begin(),::tolower);
    cout<<"\n";
    if(books == "pbook")
    {
        sortBooks = Pbook;
        cout<<"Filter: Printed Books\n";
    }
    else if(books == "ebook")
    {
        sortBooks = Ebook;
        cout<<"Filter: Electronic Books\n";
    }
    else if(books != "ebook" && books!= "pbook")
    {
            sortBooks.insert(sortBooks.end(), Pbook.begin(), Pbook.end());
            sortBooks.insert(sortBooks.end(), Ebook.begin(), Ebook.end()); 
    }

    if(filter == 1)
    {   
        sort(sortBooks.begin(),sortBooks.end(),compareByName);
        cout<<"Sorted by: Name\n";
    }
    else if(filter == 2)
    {
        sort(sortBooks.begin(),sortBooks.end(),compareByPricehl);
        cout<<"Sorted by: Price low to high\n";
    }
    else if(filter == 3)
    {
        sort(sortBooks.begin(),sortBooks.end(),compareByPricelh);
        cout<<"Sorted by: Price high to low\n";
    }
    
        for (int i = 0; i < sortBooks.size(); i++)
        {        if (sortBooks[i].price != 0)
            {
                cout << "\n"
                    << sortBooks[i].subject << "\n";
                cout << "Name: " << sortBooks[i].name << "\n";
                cout << sortBooks[i].tag << "\nAuthor: " << sortBooks[i].author << "\n";
                cout << "Price: " << sortBooks[i].price << "\n";
                if(sortBooks[i].type == "pbook")
                    cout<<"In Stock: "<<sortBooks[i].quantity<<"\n";
                cout << "\nAdd to Cart --> " << count << "\n";
                cout << "-----------------------------";
                display[count] = sortBooks[i].id;
                count++;
            }
        }
    
    // if( books != "pbook")
    // {    
    //     for (int i = 0; i < Ebook.size(); i++)
    //     {
    //         if (Ebook[i].price != 0)
    //         {
    //             cout << "\n"
    //                 << Ebook[i].subject << "\n";
    //             cout << "Name: " << Ebook[i].name << "\n";
    //             cout << Ebook[i].tag << "\nAuthor: " << Ebook[i].author << "\n";
    //             cout << "Price: " << Ebook[i].price << "\n";
    //             cout << "\nAdd to Cart --> " << count << "\n";
    //             cout << "-----------------------------";
    //             display[count] = Ebook[i].id;
    //             count++;
    //         }
    //     }
    // }

    int c;
    cout << "\n Exit --> " << count;
    int exit = count;
    char ch = 'y';

    do
    {
        cout << "\nEnter Choice: ";
        cin >> c;
        if (c <= exit)
        {
            if (c == exit)
            {
                return ;
            }
            else
            {   string id = display[c];

                if(id.substr(0,2) == "EB")
                {
                    if(Cart[id] == 0)
                    {
                        Cart[id]++;
                    }
                    else
                    {
                        cout<<"\nOnly one can be added";
                    }
                }
                else if(id.substr(0,2) == "PB")
                {

                    if(Cart[id] < library[id].quantity)
                    {
                        Cart[id]++;
                    }
                    else
                    {
                        cout<<"\nNo more in stock";
                    }
                }
            }
        }
        else
        {
            cout << "\nPlease input correct choice";
        }

    } while (ch == 'y');
}

void search(Trienode *root)
{
    int op;
    cout<<"\n\nSearch on the basis of: \n";
    cout<<"1.Author 2.Name\n";
    cin>>op;

    searchresults.clear();
    string search;
    cout<<"Search(enter keyword): ";
    cin>>search;
    suggestion(root,search);

    cout<<"\nSearch Results\n";
    cout<<"\n"<<searchresults.size()<<" results displayed";
    int count = 1;
    map<int,string> display;
    for (int i = 0; i < searchresults.size(); i++)
    {
        string id = searchresults[i];
            cout << "\n";
            cout << library[id].subject << "\n";
            cout << "Name: " << library[id].name << "\n";
            cout << library[id].tag << "\nAuthor: " << library[id].author << "\n";
            cout << "Price: " << library[id].price << "\n";
            if(id.substr(0,2) == "PB")
                cout<<"In Stock: "<<library[id].quantity<<"\n";
            cout << "\nAdd to Cart --> " << count << "\n";
            cout << "-----------------------------";
            display[count] = library[id].id;
            count++;
    }

    int c;
    cout << "\n Exit --> " << count;
    int exit = count;
    char ch = 'y';

    do
    {
        cout << "\nEnter Choice: ";
        cin >> c;
        if (c <= exit)
        {
            if (c == exit)
            {
                return ;
            }
            else
            {   string id = display[c];

                if(id.substr(0,2) == "EB")
                {
                    if(Cart[id] == 0)
                    {
                        Cart[id]++;
                    }
                    else
                    {
                        cout<<"\nOnly one can be added";
                    }
                }
                else if(id.substr(0,2) == "PB")
                {

                    if(Cart[id] < library[id].quantity)
                    {
                        Cart[id]++;
                    }
                    else
                    {
                        cout<<"\nNo more in stock";
                    }
                }
            }
        }
        else
        {
            cout << "\nPlease input correct choice";
        }

    } while (ch == 'y');
}

void customer(Trienode *root,Trienode *placeroot)
{
    char ch = 'y';
    do
    {
        cout << "1. Book bank\n";
        cout << "2. Order Books\n";
        cout << "3. Cart\n";
        cout << "4. Search\n";
        cout << "5. Exit\n";
        cout << "Enter Choice: ";
        int choice1;
        cin >> choice1;
        switch (choice1)
        {
            case 1:
                bookbank(root);
                break;
            case 2:
                orderbook();
                break;
            case 3:
                cart(placeroot);
                break;
            case 4:
                search(root);
                break;
            case 5:
                cout << "\nExiting....\n";
                _Exit(10);
                break;
            default:
                "\nWrong Choice\n";
            }
            cout << "\nWant to retry(Y/y): ";
            cin >> ch;
    } while (ch == 'y' || ch == 'Y');
}

void DelhiMap()
{
    g.addEdge(0,1,43);
    g.addEdge(0,5,42);
    g.addEdge(1,5,2);
    g.addEdge(1,6,2);
    g.addEdge(6,7,3);
    g.addEdge(7,8,3);
    g.addEdge(5,11,9.5);
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

void initiate_code(Trienode *root,Trienode *placeroot){
    DelhiMap();
    //mapping names of places with nodes of Map
    placesTonodes = {
    {"Warehouse",0},{"dtu entrance",1},{"dtu sports ground",2},{"dtu library",3},{"dtu oat",4},
    {"samaypur badli",5},{"rithala",6},{"pitampura",7},{"netaji subhash place",8},{"shalimar bagh",9},
    {"azadpur",10},{"jharoda",11},{"keshav puram",12},{"ashok vihar",13},{"gtb nagar",14},
    {"kashmere gate",15},{"signature bridge",16},{"yamuna vihar",17},{"welcome",18},{"dilshad garden",19},
    {"inderlok",20},{"bahadurgarh",21},{"peeragarhi",22},{"punjabi bagh",23},{"ashoka park main",24},
    {"kirti nagar",25},{"rajouri garden",26},{"janakpuri",27},{"dwarka",28},{"najafgarh",29},
    {"connaught place",30},{"new delhi railway station",31},{"bangla sahib",32},{"parliament",33},{"india gate",34},
    {"supreme court",35},{"red fort",36},{"mandi house",37},{"yamuna bank",38},{"vaishali",39},
    {"mayur vihar",40},{"botanical garden",41},{"noida",42},{"lajpat nagar",43},{"nehru place",44},
    {"faridabad",45},{"greater kailash",46},{"hauz khas",47},{"malviya nagar",48},{"qutub minar",49},
    {"gurugram",50},{"iit delhi",51},{"ina delhi hatt",52},{"dhaula kuan",53},{"igia",54},{"botanical park",55}};
    for(auto it = placesTonodes.begin();it != placesTonodes.end();++it){
        nodesToplaces[it->second] = it->first;
        place.push_back(it->first);
    }
    //Adding Electronic books to databse
    info(root,1,"Machine_Learning","Machine Learning", "Tom Mitchell", "Machine Learning", "Enigneering","ebook", 765);
    info(root,1,"Computer_Graphics","Computer Graphics C Version 2nd Edition", "Hearn,Bakers", "Computer Graphics", "Enigneering", "ebook", 555);
    info(root,1,"Theory_of_Computation","Theory of Computer Science - Automata, Languages and Computation", "K.L.P. Mishra", "Theory of Computation", "Enigneering", "ebook",455);
    info(root,1,"Discrete_Maths","Discrete Mathematics and its Applications, 7th Edition", "Kenneth H. Rossen", "Discrete Mathematics", "Engineering", "ebook",300);
    info(root,1,"EVS","Perspectives in Environmental Studies", "Anubha Kaushik - C.P. Kaushik", "Environmental Science", "Engineering","ebook", 100);
    info(root,1,"NCERT_chemistry_11","NCERT chemistry class 11","NCERT","Chemistry","11th","ebook",0);
    info(root,1,"NCERT_physics_11","NCERT physics class 11","NCERT","Physics","11th","ebook",0);
    info(root,1,"NCERT_mathematics_11","NCERT mathematics class 11","NCERT","Mathematics","11th","ebook",0);
    info(root,1,"NCERT_chemistry_12","NCERT chemistry class 12","NCERT", "Chemistry","12th", "ebook",0);
    info(root,1,"NCERT_physics_12"," NCERT physics class 12","NCERT", "Physics", "12th", "ebook",0);
    info(root,1,"NCERT_mathematics_12","NCERT mathematics class 12","NCERT", "Mathematics", "12th", "ebook",0);
    //Adding printed books to database
    Pbook = {
        book("Elementary Problems in Organic Chemistry", "M.S. Chouhan", "Chemistry", "Engineering", "pbook", 500),
        book("Understanding Physics", "D.C. Pandey", "Physics", "Engineering", "pbook", 432),
        book("New Simplified Physics 11th", "SL Arora", "Physics", "11", "pbook", 1049),
        book("New Simplified Physics 12th", "SL Arora", "Physics", "12", "pbook", 1049),
        book("Introduction to Algorithms", "T.H.Cormen, C.E.Leiserson, R.L Rivest", "Algorithm Design and Analysis", "Engineering", "pbook", 830),
        book("Methods of Real Analysis", "Richard R. Goldberg", "Real Analysis", "Engineering", "pbook", 945),        
        book ("Cengage Calculus for JEE Advanced", "G. Tewani", "mathematics", "engineering", "pbook",899),
        book ("Cengage Coordinate geometry for JEE Advanced", "G. Tewani", "mathematics", "engineering", "pbook",739),
        book ("Cengage Mechanics-1 for JEE Advanced", "B.M. Sharma", "physics", "engineering", "pbook",849),
        book ("Elementary Problems in Organic Chemistry", "MS Chouhan", "chemistry", "engineering", "pbook",499)};
    Pbook[0].quantity = 3; Pbook[1].quantity = 1; Pbook[2].quantity = 2; Pbook[3].quantity = 3;
    Pbook[4].quantity = 4; Pbook[5].quantity = 2; Pbook[6].quantity = 1; Pbook[7].quantity = 3;
    Pbook[8].quantity = 1; Pbook[9].quantity = 2; Pbook[10].quantity = 3;
    for(int i = 0;i<Pbook.size();i++){
        library[Pbook[i].id] = Pbook[i];
        insertkey(root,Pbook[i].name,Pbook[i].id);
    }
    //Adding places in Trie
    for(int i=0;i<place.size();i++){
        insertkey(placeroot,place[i],"place");
    }
}

int main()
{
    //Root of Trie with names and authors  
    Trienode *root = newnode();
    //Root of Trie with places
    Trienode *placeroot = newnode();
    initiate_code(root,placeroot);
    cout << "\nBOOK MANAGEMENT SYSTEM\n";
    
            customer(root,placeroot);
            cout << "\nExiting....\n";
            _Exit(10);
    return 0;
}

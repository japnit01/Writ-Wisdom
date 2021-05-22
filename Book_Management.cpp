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
string location;
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

    book()
    {
        cout<<"Default Constructor\n";
    }

    book(string n, string a, string s, string t, string tp, int p)
    {
        if(tp == "ebook")
        {
            id = "EB" + to_string(E_count++);
        }
        else if( tp == "pbook")
        {
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

class Graph 
{
    public:

    int V;
    vector<list<pair<int,int>>> adj;

    Graph(int V);
    void addEdge(int u,int v,int w);
    void printGraph();
    // void printShortestPath(int source, int destination);s
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

struct Trienode{
        Trienode *children[255];
        bool eow;
        string id;
    };

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
    cout<<key<<"\n";
    for(int i=0;i<key.size();i++)
    {    
        int ind = key[i] - '\0';
        if(!temp->children[ind])
        {
            temp->children[ind] = newnode();
        }
        temp = temp->children[ind];
    }
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
    if(root->eow)
    {
        cout<<s<<"\n";
    }    

    if(check(root))
    {
        return ;
    }

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
    for(int i=0;i<s.size();i++)
    {  
        int ind = s[i] - '\0';
        //cout<<ind<<"\n";
        if(!reroot->children[ind])
        {
            cout<<"No book found\n";
            return ;
        }
        reroot = reroot->children[ind];
    }
    bool checklast = check(reroot);

    if(checklast && reroot->eow)
    {
        cout<<s<<"\n";
        cout<<"No other book found\n";
        return;
    }
    else{
        options(reroot,s);
        return;
    }
}

Graph g(10);
map<string,int> placesTonodes;
map<int,string> nodesToplaces;
map<string,book> library;
map<string,int> Cart;
vector<book> Ebook;
vector<book> Pbook;

map<char, string> codes;
string compressed = "",dectext = "";;

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

    for (auto it = freq.begin(); it != freq.end(); it++)
    {
        q.push(new minheapnode(it->first, it->second));
    }

    //printq(q);
    minheapnode *top, *right, *left;

    while (q.size() != 1)
    {
        left = q.top();
        q.pop();

        right = q.top();
        q.pop();

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
    //cout<<text.size()<<"\n";
    for(int i=0;i<text.size();i++)
    {
        //cout<<i<<" ";
        root = temp;
        while(root->l == '#' && i<text.size())
        {
            if(text[i] == '0')
            {   
                i++;
                root = root->left;
            }
            else if(text[i] == '1')
            {
                i++;
                root = root->right;
            }
        }
        dectext+=root->l;
        //cout<<dectext<<"\n";
        i--;
    }
}


int dectreec = 0;
void decodetree(string text,int n,minheapnode* reroot)
{   
    if(reroot->l !='#')
    {
        return ;
    } 

    if(text[dectreec]!='1' && text[dectreec]!='0')
    {
        reroot->l = text[dectreec];
        dectreec++;
        return ;
    }

    if(text[dectreec] == '0')
    {
        reroot->left = new minheapnode('#',-1);
        dectreec++;
        decodetree(text,n,reroot->left);
    
    }

    if(text[dectreec] == '1')
    {
       reroot->right = new minheapnode('#',-1);
       dectreec++;
       decodetree(text,n,reroot->right); 
    }    
}

void decodehuffman(string text)
{   
    minheapnode *reroot;
    reroot = new minheapnode('#',-1);
    int n = stoi(text.substr(0,32),0,2);

    decodetree(text.substr(32,n),n,reroot);
    codestable(reroot,"");
    minheapnode *temp = reroot;
    int textsize = text.size()-32-n;
    decodetext(text.substr(32+n,text.size()-32-n),reroot);
}

void download(vector<string> eb,int e)
{   
    for(int i=0;i<e;i++)
    {   
        string inputfile,outputfile;
        
        inputfile = library[eb[i]].filename;
        //cout<<inputfile<<"\n";
        ifstream inpfile;
        string s="",temp;

        inpfile.open("compressed_files/" + inputfile,ios::in);
        
        if(!inpfile)
        {
            cout<<"File not found\n";
        }
        else
        {
            while(!inpfile.eof())
            {
                getline(inpfile,temp);
                //cout<<temp<<"\n";
                //cout<<temp.size()<<"\n";
                s+=temp;
            }
            inpfile.close();
            //cout<<s.size();
            codes.clear();
            dectext="";
            dectreec = 0;
            decodehuffman(s);
            //cout<<dectext<<"\n";

            ofstream outpfile;
            
            if(inputfile.substr(inputfile.size()-15,inputfile.size()) == "_compressed.txt")
                outputfile = inputfile.substr(0,inputfile.size()-15) + "_decompressed.txt";
            else
                outputfile = inputfile + "_decompressed.txt";
            outpfile.open(user_path + outputfile,ios::out);
            
            if(!outpfile)
            {
                cout<<"File not created\n";
            }
            else
            {
                //cout<<dectext.size()<<"\n";
                outpfile<<dectext<<endl;
                cout<<"File downloaded\n";
            }
            outpfile.close();
        }
        
    }
    
}

// Using Dijkstra's Algorithm
void deliver(vector<list<pair<int,int>>> adj, int V, string src, string dest)
{
    int source = placesTonodes[src];
    int destination = placesTonodes[dest];
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
        cout<<nodesToplaces[s.top()];
            
        if(s.size() > 1)
        {
            cout<<" -> ";
        }

        s.pop();
    }

    cout<<"\n\nTotal distance : "<<distance[destination]<<"\n\n";

}

bool billing(int p,int e,int sum,vector<string> eb,vector<string> pb){
    codes.clear();
    cout<<"Order Summary\n";
    cout<<"Item Ordered: "<<p+e<<"\n";
    cout<<"Ebook: "<<e<<"\n";
    cout<<"Printed Book: "<<p<<"\n";
    cout<<"Total: "<<sum<<"\n";

    cout<<"Avail Discount Coupons: ";
    string coupon;
    
    cout<<"\n";
    cout<<"\n1. UPI(Patym/Phonepe/Gpay)\n";
    cout<<"2. Credit Card\n";
    cout<<"3. Debit Card\n";
    
    int amount;
    cout<<"Enter Amount: ";
    cin>>amount;
    if(amount == sum)
    {
        cout<<"Payment Succesfull!!!\n";
    }
    else{
        return false;
    }

    if(p!=0)
    {
        string source = "Warehouse";
        string destination;
        cout<<"\nEnter the place for collecting the order : ";
        cin>>destination;
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

bool compareByPrice(book a, book b)
{
    return (a.price < b.price);
}

void info(Trienode *root,int a,string inputfile,string name, string author, string subject, string tag,string type,int price)
{   string path;
    if(a ==1)
    {
        path = initiate_path; 
    }
    else if(a ==0)
    {
        path = user_path;
    }
    string outputfile;
    compressed = "";
    ifstream inpfile;
    string s = "", temp;
    cout<<inputfile<<"\n";
    inpfile.open(path + inputfile, ios::in);
    if (!inpfile)
    {
        cout << "File not found\n";
    }
    else
    {
        //inpfile>>s;
        while (!inpfile.eof())
        {
            getline(inpfile, temp);
            //cout<<temp<<"\n";
            s += temp;
        }
        inpfile.close();
        codes.clear();
        calcfreq(s);

        int n = compressed.size();
        //cout<<n<<"\n";
        string tree_size = "";

        for (int i = 31; i >= 0; i--)
        {
            int k = n >> i;
            if (k & 1)
                tree_size += "1";
            else
                tree_size += "0";
        }
        //cout<<tree_size.size()<<"\n";
        compressed = tree_size + compressed;
        //cout<<s<<"\n";
        text_encode(s);
        //cout<<compressed.size()<<"\n";

        ofstream outpfile;

        if (inputfile.size()>4 && inputfile.substr(inputfile.size() - 4, 4) == ".txt")
            outputfile = inputfile.substr(0, inputfile.size() - 4) + "_compressed.txt";
        else
            outputfile = inputfile + "_compressed.txt";
        outpfile.open("compressed_files/" + outputfile, ios::out);

        if (!outpfile)
        {
            cout << "File not created\n";
        }
        else
        {
            //cout<<compressed.size()<<"\n";
            outpfile << compressed << endl;
            cout << "File Uploaded\n"; 
            book b(name, author, subject, tag, "ebook",price);
            b.filename = outputfile;
            Ebook.push_back(b);
            library[b.id] = b;
            insertkey(root,b.name,b.id);
        }

        outpfile.close();
        codes.clear();
    }
}

void downloadfile()
{
    map<int,string> display;
    cout << "\nEbooks\n";
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
            if (c == exit)
            {
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
    cout << "Name: \n";
    cin >> name;
    cout << "Author: \n";
    cin >> author;
    cout << "Subject: \n";
    cin >> subject;
    cout << "Tag (Class/Engineering/Medical): \n";
    cin >> tag;

    string inputfile;
    cout << "Enter the name of file: \n";
    cin >> inputfile;
    info(root,0,inputfile,name, author, subject, tag,"ebook",0);
}

void cart()
{
    vector<string> pb;
    vector<string> eb;
    cout << "\nCart\n";
    int sum = 0,p = 0,e = 0;
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
            if(billing(p,e,sum,eb,pb))
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

void orderstatus()
{
    cout << "\nOrder Status\n";
}

void orderbook()
{
    cout << "Books\n";
    int count = 1;
    map<int,string> display;

    string books, filter;
    vector<book> sortBooks;
    cout<<"Enter the type of book, 'Physical book or Book PDF'  :  ";
    cin>>books;
    cout<<"Enter the criteria for displaying the list of books, 'Name of books or Price of books' :  ";
    cin>>filter;

    transform(books.begin(),books.end(),books.begin(),::tolower);
    transform(filter.begin(),filter.end(),filter.begin(),::tolower);

    if(books == "physical book")
    {
        sortBooks = Pbook;
    }
    else
    {
        sortBooks = Ebook;
    }

    if(filter == "price")
    {
        sort(sortBooks.begin(),sortBooks.end(),compareByPrice);
    }
    else 
    {
        sort(sortBooks.begin(),sortBooks.end(),compareByName);
    }

    for (int i = 0; i < Pbook.size(); i++)
    {
        if (Pbook[i].price != 0)
        {
            cout << "\n"
                 << Pbook[i].subject << "\n";
            cout << "Name: " << Pbook[i].name << "\n";
            cout << Pbook[i].tag << "\nAuthor: " << Pbook[i].author << "\n";
            cout << "Price: " << Pbook[i].price << "\n";
            cout<<"In Stock: "<<Pbook[i].quantity<<"\n";
            cout << "\nAdd to Cart --> " << count << "\n";
            cout << "-----------------------------";
            display[count] = Pbook[i].id;
            count++;
        }
    }

    for (int i = 0; i < Ebook.size(); i++)
    {
        if (Ebook[i].price != 0)
        {
            cout << "\n"
                 << Ebook[i].subject << "\n";
            cout << "Name: " << Ebook[i].name << "\n";
            cout << Ebook[i].tag << "\nAuthor: " << Ebook[i].author << "\n";
            cout << "Price: " << Ebook[i].price << "\n";
            cout << "\nAdd to Cart --> " << count << "\n";
            cout << "-----------------------------";
            display[count] = Ebook[i].id;
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
    string search;
    cout<<"Search: ";
    cin>>search;
    suggestion(root,search);
}

void customer(Trienode *root)
{
    char ch = 'y';
    do
    {
        cout << "\n\n\nCustomer\n";
        cout << "1. Book bank\n";
        cout << "2. Order Books\n";
        cout << "3. Order Status\n";
        cout << "4. Cart\n";
        cout << "5. Search\n";
        cout << "6. Exit\n";
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
                orderstatus();
                break;
            case 4:
                cart();
                break;
            case 5:
                search(root);
                break;
            case 6:
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
    g.addEdge(0,3,4);
    g.addEdge(1,2,2);
    g.addEdge(2,3,7);
    g.addEdge(3,4,3);
    g.addEdge(4,1,5);
    g.addEdge(1,0,1);
    g.addEdge(3,5,9);
    g.addEdge(1,5,6);
    g.addEdge(6,5,3);
    g.addEdge(1,9,12);
    g.addEdge(6,9,6);
    g.addEdge(0,7,8);
    g.addEdge(7,9,20);
    g.addEdge(2,8,8);
    g.addEdge(8,7,2);
}

void initiate_code(Trienode *root)
{
    DelhiMap();

    placesTonodes = {
        {"Warehouse",0},{"a",1},{"b",2},{"c",3},{"d",4},{"e",5},{"f",6},{"g",7},{"h",8},{"i",9}
    };

    nodesToplaces = {
        {0,"Warehouse"},{1,"a"},{2,"b"},{3,"c"},{4,"d"},{5,"e"},{6,"f"},{7,"g"},{8,"h"},{9,"i"}
    };

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
        book ("Elementary Problems in Organic Chemistry", "MS Chouhan", "chemistry", "engineering", "pbook",499)
    
    };

    Pbook[0].quantity = 3;
    Pbook[1].quantity = 1;
    Pbook[2].quantity = 2;
    Pbook[3].quantity = 3;
    Pbook[4].quantity = 4;
    Pbook[5].quantity = 2;
    Pbook[6].quantity = 1;
    Pbook[7].quantity = 3;
    Pbook[8].quantity = 1;
    Pbook[9].quantity = 2;
    Pbook[10].quantity = 3;

    for(int i = 0;i<Pbook.size();i++)
    {
        library[Pbook[i].id] = Pbook[i];
        insertkey(root,Pbook[i].name,Pbook[i].id);
    }
}

int main()
{
    Trienode *root = newnode();
    initiate_code(root);
    //cout << Ebook.size();
    cout << "\nBOOK MANAGEMENT SYSTEM\n\n\n\n";
    char ch = 'y';
    do
    {
        cout << "Log in \n";
        cout << "User --> 1    Exit --> 2\n";
        cout << "Enter Choice: ";
        int choice1;
        cin >> choice1;
        switch (choice1)
        {
        case 1:
            customer(root);
            break;

        case 2:
            cout << "\nExiting....\n";
            _Exit(10);
            break;

        default:
            "\nWrong Choice\n";
        }
        cout << "\nWant to Login Again(Y/y): ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    cout << "\nExiting.....\n";
    return 0;
}

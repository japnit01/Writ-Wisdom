#include <bits/stdc++.h>
using namespace std;

class book {
    public:
    string name;
    string author;
    string subject;
    string tag;
    int price;
    int quantity;
    string type;    
    string filename;
        
    book(string n, string a, string s, string t, string tp, int p)
    {
        name = n;
        author = a;
        subject = s;
        tag = t;
        type = tp;
        price = p;
        quantity = 0;
        filename="";
    }
};

vector<book> Ebook;
vector<book> Pbook;


map<char,string> codes;
string compressed = "";

struct minheapnode{
    char l;
    int freq;
    minheapnode *left,*right;

    minheapnode(char l,int freq)
    {
        left = right = NULL;
        this->l = l;
        this->freq = freq;
    }
};

struct cmp{
    bool operator()(minheapnode* a,minheapnode* b)
    {
        return a->freq>b->freq;
    }
};

void text_encode(string text)
{
    for(int i=0;i<text.size();i++)
    {
        compressed+=codes[text[i]];
    }
}

void codestable(minheapnode* root,string str)
{
    if(!root)
        return ;

    if(root->l != '#')
    {
        //cout<<root->l<<": "<<str<<"\n";
        codes[root->l] = str;
    }

    codestable(root->left,str + "0");
    codestable(root->right,str + "1");
}

void preorder(minheapnode* root,string str,string a)
{
    if(!root)
    {
        return ;
    }

    compressed+=a;
    if(root->l != '#')
    {
        compressed+=root->l;
    }

    preorder(root->left,str,"0");
    preorder(root->right,str,"1");
}

void huffman(map<char,int> freq)
{
     
    priority_queue<minheapnode*,vector<minheapnode*>,cmp> q;

    for(auto it = freq.begin();it!=freq.end();it++)
    {
        q.push(new minheapnode(it->first,it->second));
    }

    //printq(q);
    minheapnode *top,*right,*left;

    while(q.size()!=1)
    {
        left  = q.top();
        q.pop();

        right = q.top();
        q.pop();

        top = new minheapnode('#',left->freq + right->freq);

        top->left = left;
        top->right = right;
        
        q.push(top);
    }

   codestable(q.top(),"");
   string a;
   preorder(q.top(),"",a);
}

void calcfreq(string text)
{
    map<char,int> freq;
    for(int i=0;i<text.size();i++)
    {
        freq[text[i]]++;
    }
    /*
    for(auto it = freq.begin();it!=freq.end();++it)
    {
        cout<<it->first<<" "<<it->second<<"\n";
    }
    
    cout<<"\n";*/
    huffman(freq);
}

void downloadfile(){
    cout<<"\nEbooks\n";
   
}

void addfile(){
    cout<<"\nAdd Book\n";
    string name,author,subject,tag,file;
    cout<<"Name: \n";
    cin>>name;
    cout<<"Author: \n";
    cin>>author;
    cout<<"Subject: \n";
    cin>>subject;
    cout<<"Tag (Class/Engineering/Medical): \n";
    cin>>tag;

    string inputfile,outputfile;

    cout<<"Enter the name of file: \n";
    cin>>inputfile;

    ifstream inpfile;
    string s="",temp;

    inpfile.open("files/" + inputfile,ios::in);
    if(!inpfile)
    {
        cout<<"File not found\n";
    }
    else
    {
        //inpfile>>s;
        while(!inpfile.eof())
        {
            getline(inpfile,temp);
            //cout<<temp<<"\n";
            s+=temp;
        }
        inpfile.close();
        calcfreq(s);
      
        int n = compressed.size();
        //cout<<n<<"\n";
        string tree_size="";

        for (int i = 31; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
            tree_size+="1";
            else
            tree_size+="0";
        }
        //cout<<tree_size.size()<<"\n";
        compressed = tree_size + compressed;
        //cout<<s<<"\n";
        text_encode(s);
        //cout<<compressed.size()<<"\n";
    
        ofstream outpfile;

        if(inputfile.substr(inputfile.size()-4,4) == ".txt")                                
            outputfile = inputfile.substr(0,inputfile.size()-4) + "_compressed.txt";
        else
            outputfile = inputfile + "_compressed.txt";
        outpfile.open("compressed_files/" + outputfile,ios::out);
        
        if(!outpfile)
        {
            cout<<"File not created\n";
        }
        else
        {
            //cout<<compressed.size()<<"\n";
            outpfile<<compressed<<endl;
            cout<<"File Uploaded";
            book b(name,author,subject,tag,"Ebook",0);
            b.filename = outputfile;
            Ebook.push_back(b);
        }
        
        outpfile.close();
    }
}

void cart()
{
    cout<<"\nCart\n";
}

void bookbank()
{
    cout<<"\n\nBook Bank\n";
    cout<<"1. Upload Books\n";
    cout<<"2. Download Books\n";

    int choice1;
    cout<<"Enter Choice: ";
    cin>>choice1;
    if(choice1 == 1)
    {
        addfile();
    }
    else if(choice1 == 2)
    {
        downloadfile();
    }
}

void orderstatus()
{
    cout<<"\nOrder Status\n";
}

void orderbook()
{
    cout<<"Order Book\n";
}

void customer()
{
    cout<<"\n\n\nCustomer\n";
    cout<<"1. Book bank\n";
    cout<<"2. Order Books\n";
    cout<<"3. Check Order Status\n";
    cout<<"4. Cart\n";
    cout<<"5. Exit\n";

    char ch = 'y';

    do{
        cout<<"Enter Choice: ";
        int choice1;
        cin>>choice1;
        switch(choice1)
        {
            case 1: bookbank();
                    break;
            case 2: orderbook();        
                    break;
            case 3: orderstatus();
                break;
            case 4: cart();
                    break;
            case 5: cout<<"\nExiting....\n";
                    _Exit(10);
                    break;
            default: "\nWrong Choice\n";
        }
        cout<<"\nWant to retry(Y/y): "; 
        cin>>ch;
    }while(ch == 'y' || ch =='Y');
}

void employee()
{
    cout<<"Employee\n";
}

void initiate_code()
{
    Ebook = {
        new book("Machine Learning", "Tom Mitchell", "Machine Learning", "", "Ebook", 0),
        new book("Computer Graphics C Version 2nd Edition", "Hearn,Bakers", "Computer Graphics", "", "Ebook", 0),
        new book("Theory of Computer Science - Automata, Languages and Computation", "K.L.P. Mishra", "Theory of Computation", "", "Ebook", 0),
        new book("Discrete Mathematics and its Applications, 7th Edition", "Kenneth H. Rossen", "Discrete Mathematics", "", "Ebook", 0),
        new book("Perspectives in Environmental Studies", "Anubha Kaushik - C.P. Kaushik", "Environmental Science", "", "Ebook", 0)
    };
}

int main(){
    void initiate_code();
    cout<<"\nBOOK MANAGEMENT SYSTEM\n\n\n\n";
    cout<<"Log in \n";
    cout<<"User --> 1    Employee --> 2    Exit --> 3\n";
    char ch = 'y';

    do{
        cout<<"Enter Choice: ";
        int choice1;
        cin>>choice1;
        switch(choice1)
        {
            case 1: customer();
                    break;

            case 2: employee();        
                    break;

            case 3: cout<<"\nExiting....\n";
                    _Exit(10);
                    break;

            default: "\nWrong Choice\n";
        }

        cout<<"\nWant to Login Again(Y/y): "; 
        cin>>ch;
    }while(ch == 'y' || ch =='Y');
    cout<<"\nExiting.....\n";
    return 0;
}

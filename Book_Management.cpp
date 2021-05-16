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
//#include <boost/filesystem.hpp>
//using boost::filesystem; 

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

void info(string name,string author,string subject,string tag,string path)
{
    string inputfile,outputfile;

    cout<<"Enter the name of file: \n";
    cin>>inputfile;

    ifstream inpfile;
    string s="",temp;

    inpfile.open(path + inputfile,ios::in);
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

void downloadfile(string path){
    cout<<"\nEbooks\n";
    int count = 1;
    for(int i=0;i<Ebook.size();i++)
    {   cout<<"i";
        if(Ebook[i].price == 0)
        {   
            cout<<"\n"<<Ebook[i].subject<<"\n";
            cout<<"Name: "<<Ebook[i].name<<"\n";
            cout<<Ebook[i].tag<<"\nAuthor: "<<Ebook[i].author<<"\n";
            cout<<"Price: "<<Ebook[i].price<<"\n";
            cout<<"\n Add to Cart --> "<<count<<"\n";
            cout<<"-----------------------------";
            count++;
        }
    }
    // vector<int> addtocart;
    // do{
    //     cout<<"Enter Choice"
    // }
}

void addfile(string path){
    cout<<"\nAdd Book\n";
    string name,author,subject,tag;
    cout<<"Name: \n";
    cin>>name;
    cout<<"Author: \n";
    cin>>author;
    cout<<"Subject: \n";
    cin>>subject;
    cout<<"Tag (Class/Engineering/Medical): \n";
    cin>>tag;

    info(name,author,subject,tag,path);
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
    string path = "user_files/";
    int choice1;
    cout<<"Enter Choice: ";
    cin>>choice1;
    if(choice1 == 1)
    {
        addfile(path);
    }
    else if(choice1 == 2)
    {
        downloadfile(path);
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
         book("Machine Learning", "Tom Mitchell", "Machine Learning", "Enigneering", "Ebook", 0),
         book("Computer Graphics C Version 2nd Edition", "Hearn,Bakers", "Computer Graphics", "Enigneering", "Ebook", 0),
         book("Theory of Computer Science - Automata, Languages and Computation", "K.L.P. Mishra", "Theory of Computation", "Enigneering", "Ebook", 0),
         book("Discrete Mathematics and its Applications, 7th Edition", "Kenneth H. Rossen", "Discrete Mathematics", "Engineering", "Ebook", 0),
         book("Perspectives in Environmental Studies", "Anubha Kaushik - C.P. Kaushik", "Environmental Science", "Engineering", "Ebook", 0)
    };

    // for (directory_iterator itr( "files" ); itr != end_itr; ++itr)
    // {
    //     cout<<itr->leaf()<<"\n";
    // }

        cout<<Ebook.size()<<" ";
    
    Ebook[0].filename = "Machine Learning";
    Ebook[1].filename = "Computer Graphics";
    Ebook[2].filename = "Theory of Computation";
    Ebook[3].filename = "Discrete Mathematics";
    Ebook[4].filename = "EVS";

    Pbook = {
        book("Elementary Problems in Organic Chemistry","M.S. Chouhan","Chemistry","Engineering","pbook",500),
        book("Understanding Physics","D.C. Pandey","Physics","Engineering","pbook",432),
        book("New Simplified Physics 11th","SL Arora","Physics","11","pbook",1049),
        book("New Simplified Physics 12th","SL Arora","Physics","12","pbook",1049),
        book("Introduction to Algorithms","T.H.Cormen, C.E.Leiserson, R.L Rivest","Algorithm Design and Analysis","Engineering","pbook",830),
        book("Methods of Real Analysis","Richard R. Goldberg","Real Analysis","Engineering","pbook",945)
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

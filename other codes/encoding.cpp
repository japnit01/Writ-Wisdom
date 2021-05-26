#include<bits/stdc++.h>
using namespace std;

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

void compratio(string s)
{
    int b0 = s.size()*8;
    int b1 = 32 + (codes.size()*8) + compressed.size();
    int cod = 0;
    //cout<<compressed.size()<<"\n";
    for(auto it = codes.begin();it!=codes.end();++it)
    {
        cod+=it->second.size();
    }
    b1+=cod;
    cout<<"\n";
    cout<<"Bits before compression: "<<b0<<"\n";
    cout<<"Bits after compression: "<<b1<<"\n";
    cout<<"Compression Ratio: "<<b0/b1<<"\n";
}

void text_encode(string text)
{
    for(int i=0;i<text.size();i++)
    {
        compressed+=codes[text[i]];
    }
}

void printq(priority_queue<minheapnode*,vector<minheapnode*>,cmp> q)
{
    while(!q.empty())
    {   minheapnode* m = q.top();
        //cout<<(*m).l<<" "<<(*m).freq<<"\n";
        q.pop();
    }
}

void codestable(minheapnode* root,string str)
{
    if(!root)
        return ;

    if(root->l != '#')
    {
        cout<<root->l<<": "<<str<<"\n";
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

    printq(q);
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
    
    for(auto it = freq.begin();it!=freq.end();++it)
    {
        cout<<it->first<<" "<<it->second<<"\n";
    }
    
    cout<<"\n";
    huffman(freq);
}
int main(){
    
    string inputfile,outputfile;



    ifstream inpfile;
    string s="",temp;

    inpfile.open("C:/Users/japni/OneDrive/Desktop/Book-Management-System/files/sample.txt",ios::in);
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
        //transform(s.begin(), s.end(), s.begin(), ::tolower);

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
        string tree = compressed;
        compressed = tree_size + compressed;
        text_encode(s);
        cout<<"compressed string"<<"\n";
        cout<<tree_size<<" "<<tree<<" "<<compressed.substr(32+tree.size(),compressed.size() - (32+tree.size()))<<"\n";
        compratio(s);

        ofstream outpfile;

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
        }
        outpfile.close();
    }
    //decodehuffman(compressed);

    return 0;
}
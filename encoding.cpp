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
        cout<<(*m).l<<" "<<(*m).freq<<"\n";
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

int c = 0;
void decodetree(string text,int n,minheapnode* reroot)
{
     if(reroot->l != '#' || c == n)
     {
        return;
     }
     
     if(text[c] == '0')
     {
        if(text[c+1]==0 || text[c+1] == 1)
        {
            reroot->left = new minheapnode('#',-1);
        }
        else
        {
            reroot->left = new minheapnode(text[c+1],-1);
        }
        decodetree(text,n,reroot->left);
     }

     if(text[c] == '1')
     {
        if(text[c+1]==0 || text[c+1] == 1)
        {
            reroot->right = new minheapnode('#',-1);
        }
        else
        {
            reroot->right = new minheapnode(text[c+1],-1);
        }
        decodetree(text,n,reroot->right);
    }
}

void decodehuffman(string text,int n)
{   
    minheapnode *reroot;
    reroot = new minheapnode('#',-1);
    int i = 0;
    decodetree(text,n,reroot);
    cout<<"\n";
    codestable(reroot,"");
}

int main(){
    
    string inputfile,outputfile;

    cout<<"Enter the name of file: \n";
    cin>>inputfile;

    ifstream inpfile;
    string s="",temp;

    inpfile.open(inputfile,ios::in);
    if(!inpfile)
    {
        cout<<"File not found\n";
    }
    else
    {
        while(inpfile)
        {
            getline(inpfile,temp,'\0');
            s+=temp;
        }
        inpfile.close();

        calcfreq(s);
    }  
    int n = compressed.size();
    //compressed+="0";
    text_encode(s);
    cout<<compressed;

    decodehuffman(compressed,n);

    return 0;
}
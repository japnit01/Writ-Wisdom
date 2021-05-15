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
    /*
    for(auto it = freq.begin();it!=freq.end();++it)
    {
        cout<<it->first<<" "<<it->second<<"\n";
    }
    
    cout<<"\n";*/
    huffman(freq);
}
/*
int c = 0;
void decodetree(string text,int n,minheapnode* reroot)
{   
    if(reroot->l !='#')
    {
        return ;
    } 

    if(text[c]!='1' && text[c]!='0')
    {
        reroot->l = text[c];
        c++;
        return ;
    }

    if(text[c] == '0')
    {
        reroot->left = new minheapnode('#',-1);
        c++;
        decodetree(text,n,reroot->left);
    
    }

    if(text[c] == '1')
    {
       reroot->right = new minheapnode('#',-1);
       c++;
       decodetree(text,n,reroot->right); 
    }    
    return;
}

void decodehuffman(string text)
{   
    minheapnode *reroot;
    reroot = new minheapnode('#',-1);
    
    int n = stoi(text.substr(0,32),0,2);
    //cout<<n<<""<<text.substr(0,32)<<"\n";

    text = text.substr(32,text.size()-32);
    decodetree(text,n,reroot);
    codestable(reroot,"");
}
*/
int main(){
    
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

        outputfile = inputfile.substr(0,inputfile.size()-4) + "_compressed.txt";
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
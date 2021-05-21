#include<bits/stdc++.h>
using namespace std;

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

void printq(priority_queue<minheapnode*,vector<minheapnode*>,cmp> q)
{
    while(!q.empty())
    {   minheapnode* m = q.top();
        cout<<(*m).l<<" "<<(*m).freq<<"\n";
        q.pop();
    }
}

void printtable(minheapnode* root,string str)
{
    if(!root)
        return ;

    if(root->l != '#')
        cout<<root->l<<": "<<str<<"\n";

    printtable(root->left,str + "0");
    printtable(root->right,str + "1");
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

   printtable(q.top(),"");
}

int main(){
    
    string text="zzzzzzzzyyyyyyysssswedwdcccceededddddddd";
    
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
    //char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    //int freq[] = { 5, 9, 12, 13, 16, 45 };
    //int n = sizeof(arr) / sizeof(arr[0]);

    huffman(freq);
    
    return 0;
}
#include<bits/stdc++.h>
using namespace std;

struct Trienode
    {
        Trienode *children[26];
        bool eow;
        string id;
    };

Trienode *newnode()
{
    Trienode *root = new Trienode;
    root->eow = false;

    for(int i=0;i<26;i++)
    {
        root->children[i] = NULL;
    }

    return root;
}

void insertkey(Trienode *root,string key,string id)
{
    Trienode *temp = root;
    for(int i=0;i<key.size();i++)
    {   
        int ind = key[i] - 'a';
        if(!temp->children[ind])
        {
            temp->children[ind] = newnode();
        }
        temp = temp->children[ind];
    }
    temp->eow = true;
    temp->id = id;
}

bool check(Trienode *root)
{
    for(int i=0;i<26;i++)
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

    for(int i=0;i<26;i++)
    {
        if(root->children[i])
        {
            s.push_back(0+i);
            options(root->children[i],s);
            s.pop_back();
        }
    }
}


int suggestion(Trienode *root,string s)
{
    Trienode *reroot = root;

    for(int i=0;i<s.size();i++)
    {
        int ind = s[i] - 'a';
        if(!reroot->children[ind])
        {
            return 0;
        }
        reroot = reroot->children[ind];
    }

    bool checklast = check(reroot);

    if(checklast && reroot->eow)
    {
        cout<<s<<"\n";
        return 1;
    }
    else{

        options(reroot,s);
        return 2;
    }
}

int main(){
    
    Trienode *root = newnode();
    cout<<"here\n";
    insertkey(root, "hello", "12");
    insertkey(root, "dog", "13");
    insertkey(root, "hell","14");
    insertkey(root, "cat","15");
    insertkey(root, "a","16");
    insertkey(root, "hel","17");
    insertkey(root, "help","18");
    insertkey(root, "helps","19");
    insertkey(root, "helping","20");
    
    int a = suggestion(root,"hel");
    
    return 0;   
}
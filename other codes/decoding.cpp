#include<bits/stdc++.h>
using namespace std;

map<char,string> codes;
string dectext = "";
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
        cout<<dectext<<"\n";
        i--;
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
}

void decodehuffman(string text)
{   
    minheapnode *reroot;
    reroot = new minheapnode('#',-1);
    
    int n = stoi(text.substr(0,32),0,2);
    //cout<<n<<""<<text.substr(0,32)<<"\n";
    //cout<<text.size()<<"\n";
    //text = text.substr(32,text.size()-32);
    //cout<<text.size()<<"\n";
    
    decodetree(text.substr(32,n),n,reroot);
    codestable(reroot,"");
    
    minheapnode *temp = reroot;
    int textsize = text.size()-32-n;
    decodetext(text.substr(32+n,text.size()-32-n),reroot);
}

int main()
{
    string inputfile,outputfile;

    cout<<"Enter the name of file: \n";
    cin>>inputfile;

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
        decodehuffman(s);
        //cout<<dectext<<"\n";

        ofstream outpfile;
        
        if(inputfile.substr(inputfile.size()-15,inputfile.size()) == "_compressed.txt")
            outputfile = inputfile.substr(0,inputfile.size()-15) + "_decompressed.txt";
        else
            outputfile = inputfile + "_decompressed.txt";
        outpfile.open("decompressed_files/" + outputfile,ios::out);
        
        if(!outpfile)
        {
            cout<<"File not created\n";
        }
        else
        {
            //cout<<dectext.size()<<"\n";
            outpfile<<dectext<<endl;
        }
        outpfile.close();
    }
    return 0;
}
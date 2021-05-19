#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <queue>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iomanip>
using namespace std;

int E_count = 0,P_count = 0;
string user_path = "user_files/",decompress_path = "decompressed_files/";
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

map<string,int> places ;
map<string,book> library;
map<string,int> Cart;
vector<book> Ebook;
vector<book> Pbook;

map<char, string> codes;
string compressed = "";

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

void address()
{
    cout<<"Enter Address: ";
    cin>>location;
}

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

void download()
{

}

void deliver()
{

}

bool billing(int p,int e,int sum){
    cout<<"Order Summary\n";
    cout<<"Item Ordered: "<<Cart.size()<<"\n";
    cout<<"Ebook: "<<e<<"\n";
    cout<<"Printed Book: "<<p<<"\n";
    cout<<"Total: "<<sum<<"\n";

    cout<<"Avail Discount Coupons: ";
    string coupon;
    cin>>coupon;

    cout<<"1. UPI(Patym/Phonepe/Gpay)";
    cout<<"2. Credit Card";
    cout<<"3. Debit Card";
    
    cout<<"Payment Succesfull!!!\n";
    
    deliver();
    download();

    return false;
}

void info(string name, string author, string subject, string tag)
{
    string inputfile, outputfile;

    cout << "Enter the name of file: \n";
    cin >> inputfile;

    ifstream inpfile;
    string s = "", temp;

    inpfile.open(user_path + inputfile, ios::in);
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

        if (inputfile.substr(inputfile.size() - 4, 4) == ".txt")
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
            cout << "File Uploaded"; 
            book b(name, author, subject, tag, "ebook", 0);
            b.filename = outputfile;
            Ebook.push_back(b);
            library[b.id] = b;
        }

        outpfile.close();
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
            cout << Ebook[i].subject << "\n";
            cout << "Name: " << Ebook[i].name << "\n";
            cout << Ebook[i].tag << "\nAuthor: " << Ebook[i].author << "\n";
            cout << "Price: " << Ebook[i].price << "\n";
            cout << "\nAdd to Cart --> " << count << "\n";
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

void addfile()
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

    info(name, author, subject, tag);
}

void cart()
{
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
                e++;
            }
            else
            {
                p++;
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
        {   if(p!=0)
            {
                address();
            }
            if(billing(p,e,sum))
            {
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

void bookbank()
{
    cout << "\n\nBook Bank\n";
    cout << "1. Upload Books\n";
    cout << "2. Download Books\n";
    int choice1;
    cout << "Enter Choice: ";
    cin >> choice1;
    if (choice1 == 1)
    {
        addfile();
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

void customer()
{
    char ch = 'y';

    do
    {
        cout << "\n\n\nCustomer\n";
        cout << "1. Book bank\n";
        cout << "2. Order Books\n";
        cout << "3. Order Status\n";
        cout << "4. Cart\n";
        cout << "5. Exit\n";
        cout << "Enter Choice: ";
        int choice1;
        cin >> choice1;
        switch (choice1)
        {
        case 1:
            bookbank();
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

void employee()
{
    cout << "Employee\n";
}

void initiate_code()
{

    places



    Ebook = {
        book("Machine Learning", "Tom Mitchell", "Machine Learning", "Enigneering", "ebook", 765),
        book("Computer Graphics C Version 2nd Edition", "Hearn,Bakers", "Computer Graphics", "Enigneering", "ebook", 555),
        book("Theory of Computer Science - Automata, Languages and Computation", "K.L.P. Mishra", "Theory of Computation", "Enigneering", "ebook",455),
        book("Discrete Mathematics and its Applications, 7th Edition", "Kenneth H. Rossen", "Discrete Mathematics", "Engineering", "ebook",300),
        book("Perspectives in Environmental Studies", "Anubha Kaushik - C.P. Kaushik", "Environmental Science", "Engineering", "ebook", 100),
        book("NCERT chemistry class 11","NCERT","Chemistry","11th","ebook",0),
        book ("NCERT physics class 11","NCERT","Physics","11th","ebook",0),
        book ("NCERT mathematics class 11","NCERT","Mathematics","11th","ebook",0),
        book ("NCERT chemistry class 12","NCERT", "Chemistry","12th", "ebook",0),
        book (" NCERT physics class 12","NCERT", "Physics", "12th", "ebook",0),
        book ("NCERT mathematics class 12","NCERT", "Mathematics", "12th", "ebook",0)};



    /*for (directory_iterator itr( "files" ); itr != end_itr; ++itr)
    {
        cout<<itr->leaf()<<"\n";
    }*/

    //cout << Ebook.size() << " ";

    Ebook[0].filename = "Machine Learning";
    Ebook[1].filename = "Computer Graphics";
    Ebook[2].filename = "Theory of Computation";
    Ebook[3].filename = "Discrete Mathematics";
    Ebook[4].filename = "EVS";
    Ebook[5].filename = "NCERT chemistry class 11";
    Ebook[6].filename = "NCERT physics class 11";
    Ebook[7].filename = "NCERT mathematics class 11";
    Ebook[8].filename = "NCERT chemistry class 12";
    Ebook[9].filename = "NCERT physics class 12";
    Ebook[10].filename = "NCERT mathematics class 11";

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


    for(int i = 0;i<Ebook.size();i++)
    {
        library[Ebook[i].id] = Ebook[i];
    }

    for(int i = 0;i<Pbook.size();i++)
    {
        library[Pbook[i].id] = Pbook[i];
    }
}

int main()
{
    initiate_code();
    //cout << Ebook.size();
    cout << "\nBOOK MANAGEMENT SYSTEM\n\n\n\n";
    
    char ch = 'y';

    do
    {
        cout << "Log in \n";
        cout << "User --> 1    Employee --> 2    Exit --> 3\n";
        cout << "Enter Choice: ";
        int choice1;
        cin >> choice1;
        switch (choice1)
        {
        case 1:
            customer();
            break;

        case 2:
            employee();
            break;

        case 3:
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

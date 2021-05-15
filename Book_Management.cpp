#include<bits/stdc++.h>
using namespace std;

void downloadfile(){
    cout<<"\nDownload book\n";
}

void addfile(){
    cout<<"\nAdd Book\n";
}

void orderstatus()
{
    cout<<"\nOrder Status\n";
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

int main(){
    

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

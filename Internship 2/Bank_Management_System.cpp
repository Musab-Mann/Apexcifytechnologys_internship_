#include<iostream>
#include<iomanip>
#include<string>
#include<ctime>
using namespace std;
string intToString(int num)
{
    if(num==0) return "0";
    bool neg=false;
    if(num<0){neg=true; num=-num;}
    string s="";
    while(num>0)
    {
        char c=char('0'+(num%10));
        s=c+s;
        num/=10;
    }
    if(neg) s="-"+s;
    return s;
}
string timeNow()
{
    time_t t=time(NULL);
    string s=ctime(&t);
    if(s.size()>0&&s[s.size()-1]=='\n') s.erase(s.size()-1,1);
    return s;
}
class Transaction
{
public:
    int id;
    string type;
    double amount;
    string datetime;
    string details;
    Transaction()
    {
        id=0;
        type="";
        amount=0.0;
        datetime="";
        details="";
    }
    Transaction(int i,string t,double a,string d)
    {
        id=i;
        type=t;
        amount=a;
        datetime=timeNow();
        details=d;
    }
    void display()
    {
        cout<<left<<setw(6)<<id<<setw(14)<<type<<setw(12)<<fixed<<setprecision(2)<<amount<<setw(26)<<datetime<<details<<"\n";
    }
};
class Account
{
private:
    int accNo;
    double balance;
    Transaction **history;
    int histCount;
    int histCap;
    int nextTransId;
    void ensureHist()
    {
        if(histCount<histCap)
         return;
        int newCap=histCap*2;
        Transaction **tmp=new Transaction*[newCap];
        for(int i=0;i<histCount;i++)
        {
            tmp[i]=history[i];
        }
        for(int i=histCount;i<newCap;i++)
        {
             tmp[i]=NULL;
        }
        delete[] history;
        history=tmp;
        histCap=newCap;
    }
public:
    Account(int no)
    {
        accNo=no;
        balance=0.0;
        histCap=4;
        histCount=0;
        history=new Transaction*[histCap];
        for(int i=0;i<histCap;i++)
        {
            history[i]=NULL;
        }
        nextTransId=1;
    }
    ~Account()
    {
        for(int i=0;i<histCount;i++) if(history[i]!=NULL) delete history[i];
        delete[] history;
    }
    int getAccNo()
    {
        return accNo;
    }
    double getBalance()
    {
        return balance;
    }
    void addTransaction(string t,double a,string d)
    {
        ensureHist();
        history[histCount]=new Transaction(nextTransId,t,a,d);
        histCount++;
        nextTransId++;
    }
    bool deposit(double amt)
    {
        if(amt<=0.0)
         return false;
        balance+=amt;
        addTransaction("DEPOSIT",amt,"");
        return true;
    }
    bool withdraw(double amt)
    {
        if(amt<=0.0)
        return false;
        if(amt>balance)
         return false;
        balance-=amt;
        addTransaction("WITHDRAW",amt,"");
        return true;
    }
    bool transferTo(Account *other,double amt)
    {
        if(other==NULL)
        return false;
        if(amt<=0.0)
        return false;
        if(amt>balance)
        return false;
        balance-=amt;
        other->balance+=amt;
        addTransaction("TRANSFER_OUT",amt,"To A/c "+intToString(other->getAccNo()));
        other->addTransaction("TRANSFER_IN",amt,"From A/c "+intToString(accNo));
        return true;
    }
    void showSummary()
    {
        cout<<"\n"<<string(80,'-')<<"\n";
        cout<<left<<setw(25)<<"Account Summary"<<"\n";
        cout<<string(80,'-')<<"\n";
        cout<<left<<setw(15)<<"Account No"<<setw(15)<<accNo<<"\n";
        cout<<left<<setw(15)<<"Balance"<<setw(15)<<fixed<<setprecision(2)<<balance<<"\n";
        cout<<string(80,'-')<<"\n";
    }
    void showAllTransactions()
    {
        cout<<"\n"<<string(80,'-')<<"\n";
        cout<<left<<setw(30)<<"All Transactions for Account"<<accNo<<"\n";
        cout<<string(80,'-')<<"\n";
        cout<<left<<setw(6)<<"ID"<<setw(14)<<"Type"<<setw(12)<<"Amount"<<setw(26)<<"DateTime"<<"Details\n";
        cout<<string(80,'-')<<"\n";
        for(int i=0;i<histCount;i++)
        {
            history[i]->display();
        }
        if(histCount==0)
        {
            cout<<left<<setw(20)<<"No transactions found.\n";
        }
        cout<<string(80,'-')<<"\n";
    }
    void showRecent(int k)
    {
        if(k<=0)
        {
            cout<<left<<setw(20)<<"Invalid count\n";
            return;
        }
        int start=histCount-k;
        if(start<0)
        {
            start=0;
        }
        cout<<"\n"<<string(80,'-')<<"\n";
        cout<<left<<setw(30)<<"Last"<<setw(5)<<k<<"Transactions for Account"<<accNo<<"\n";
        cout<<string(80,'-')<<"\n";
        cout<<left<<setw(6)<<"ID"<<setw(14)<<"Type"<<setw(12)<<"Amount"<<setw(26)<<"DateTime"<<"Details\n";
        cout<<string(80,'-')<<"\n";
        for(int i=start;i<histCount;i++)
        {
            history[i]->display();
        }
        if(histCount==0)
        {
            cout<<left<<setw(20)<<"No transactions found.\n";
        }
        cout<<string(80,'-')<<"\n";
    }
};
class Customer
{
private:
    int custId;
    string name;
    Account **accounts;
    int accCount;
    int accCap;
    void ensureAcc()
    {
        if(accCount<accCap) return;
        int newCap=accCap*2;
        Account **tmp=new Account*[newCap];
        for(int i=0;i<accCount;i++)
        {
            tmp[i]=accounts[i];
        }
        for(int i=accCount;i<newCap;i++)
        {
            tmp[i]=NULL;
        }
        delete[] accounts;
        accounts=tmp;
        accCap=newCap;
    }
public:
    Customer(int id,string nm)
    {
        custId=id;
        name=nm;
        accCap=2;
        accCount=0;
        accounts=new Account*[accCap];
        for(int i=0;i<accCap;i++)
        {
            accounts[i]=NULL;
        }
    }
    ~Customer()
    {
        for(int i=0;i<accCount;i++)
        if(accounts[i]!=NULL)
        {
            delete accounts[i];
        }
        delete[] accounts;
    }
    int getId()
    {
        return custId;
    }
    string getName()
    {
        return name;
    }
    Account *createAccount(int accNo)
    {
        ensureAcc();
        Account *a=new Account(accNo);
        accounts[accCount]=a;
        accCount++;
        return a;
    }
    Account *findAccount(int accNo)
    {
        for(int i=0;i<accCount;i++)
        {
            if(accounts[i]->getAccNo()==accNo)
             return accounts[i];
        }
        return NULL;
    }
    void showInfo()
    {
        cout<<"\n+=======================================+\n";
        cout<<"Customer ID : "<<custId<<"\n";
        cout<<"Name        : "<<name<<"\n";
        cout<<"Accounts    : "<<accCount<<"\n";
        for(int i=0;i<accCount;i++)
        {
            cout<<" - A/c "<<accounts[i]->getAccNo();
            cout<<"   Bal: "<<fixed<<setprecision(2)<<accounts[i]->getBalance()<<"\n";
        }
        cout<<"+=======================================+\n";
    }
};
class Bank
{
private:
    Customer **customers;
    int custCount;
    int custCap;
    int nextCustId;
    int nextAccNo;
    void ensureCust()
    {
        if(custCount<custCap)
         return;
        int newCap=custCap*2;
        Customer **tmp=new Customer*[newCap];
        for(int i=0;i<custCount;i++)
        {
            tmp[i]=customers[i];
        }
        for(int i=custCount;i<newCap;i++)
        {
            tmp[i]=NULL;
        }
        delete[] customers;
        customers=tmp;
        custCap=newCap;
    }
public:
    Bank()
    {
        custCap=4;
        custCount=0;
        customers=new Customer*[custCap];
        for(int i=0;i<custCap;i++)
        {
            customers[i]=NULL;
        }
        nextCustId=1;
        nextAccNo=1001;
    }
    ~Bank()
    {
        for(int i=0;i<custCount;i++)
        {
            if(customers[i]!=NULL)
            {
                delete customers[i];
            }
        }
        delete[] customers;
    }
    Customer *createCustomer(string name)
    {
        ensureCust();
        Customer *c=new Customer(nextCustId,name);
        customers[custCount]=c;
        custCount++;
        nextCustId++;
        return c;
    }
    Customer *findCustomerById(int id)
    {
        for(int i=0;i<custCount;i++)
        if(customers[i]->getId()==id)
        {
            return customers[i];
        }
        return NULL;
    }
    Account *createAccountForCustomer(Customer *c)
    {
        if(c==NULL)
        {
            return NULL;
        }
        return c->createAccount(nextAccNo++);
    }
    void listCustomers()
    {
        if(custCount==0)
        {
            cout<<"No customers found.\n";
            return;
        }
        cout<<"\nRegistered Customers\n";
        cout<<string(40,'-')<<"\n";
        for(int i=0;i<custCount;i++)
        {
            cout<<" ID:"<<customers[i]->getId();
            cout<<"Name:"<<customers[i]->getName()<<"\n";
        }
        cout<<string(40,'-')<<"\n";
    }
};
int main()
{
    cout<<fixed<<setprecision(2);
    Bank bank;
    cout<<"+=======================================+\n";
    cout<<"|        WELCOME TO EASY BANK SYSTEM    |\n";
    cout<<"+=======================================+\n";
    while(true)
    {
        cout<<"\nMain Menu\n";
        cout<<string(40,'=')<<"\n";
        cout<<" 1)Create customer\n";
        cout<<" 2)Create account for customer\n";
        cout<<" 3)Deposit\n";
        cout<<" 4)Withdraw\n";
        cout<<" 5)Transfer\n";
        cout<<" 6)Show account summary\n";
        cout<<" 7)Show transactions (all)\n";
        cout<<" 8)Show recent transactions\n";
        cout<<" 9)Show customer info\n";
        cout<<"10)List customers\n";
        cout<<" 0)Exit\n";
        cout<<string(40,'=')<<"\n";
        cout<<"Choose option: ";
        int opt;
        if(!(cin>>opt))
        {
            cin.clear();
            string bad;
            getline(cin,bad);
            cout<<"Invalid input. Enter a number.\n";
            continue;
        }
        if(opt==0)
        break;
        if(opt==1)
        {
            string name;
            cout<<"Enter customer name: ";
            cin.ignore();
            getline(cin,name);
            if(name.size()==0)
            {
                cout<<"Name cannot be empty\n";
                continue;
            }
            Customer *c=bank.createCustomer(name);
            cout<<"+---------------------------------------+\n";
            cout<<" Customer created successfully!\n";
            cout<<" Customer ID : "<<c->getId()<<"\n";
            cout<<" Welcome, "<<c->getName()<<"!\n";
            cout<<"+---------------------------------------+\n";
        }
        else if(opt==2)
        {
            int id;
            cout<<"Enter customer ID: ";
            cin>>id;
            Customer *c=bank.findCustomerById(id);
            if(c==NULL)
            {
                cout<<"Customer not found\n";
                continue;
            }
            Account *a=bank.createAccountForCustomer(c);
            cout<<"+---------------------------------------+\n";
            cout<<" Account created successfully!\n";
            cout<<" Account No : "<<a->getAccNo()<<"\n";
            cout<<"+---------------------------------------+\n";
        }
        else if(opt==3)
        {
            int cid,acc;
            double amt;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<" Customer not found\n";
                continue;
            }
            cout<<"Enter account no: ";
            cin>>acc;
            Account *A=c->findAccount(acc);
            if(A==NULL)
            {
                cout<<" Account not found\n";
                continue;
            }
            cout<<"Enter amount to deposit: ";
            cin>>amt;
            if(A->deposit(amt))
             cout<<"Deposit successful. New balance: "<<A->getBalance()<<"\n";
            else
            cout<<"Deposit failed. Enter a positive amount\n";
        }
        else if(opt==4)
        {
            int cid,acc;
            double amt;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<" Customer not found\n";
                continue;
            }
            cout<<"Enter account no: ";
            cin>>acc;
            Account *A=c->findAccount(acc);
            if(A==NULL)
            {
                cout<<" Account not found\n";
                continue;
            }
            cout<<"Enter amount to withdraw: ";
            cin>>amt;
            if(A->withdraw(amt))
            cout<<" Withdraw successful. New balance: "<<A->getBalance()<<"\n";
            else
            cout<<"Withdraw failed. Check amount and balance\n";
        }
        else if(opt==5)
        {
            int cid1,acc1,cid2,acc2;
            double amt;
            cout<<"Enter your customer ID: ";
            cin>>cid1;
            Customer *c1=bank.findCustomerById(cid1);
            if(c1==NULL)
            {
                cout<<" Customer not found\n";
                continue;
            }
            cout<<"Enter your account no: ";
            cin>>acc1;
            Account *A1=c1->findAccount(acc1);
            if(A1==NULL)
            {
                cout<<"Account not found\n";
                continue;
            }
            cout<<"Enter target customer ID: ";
            cin>>cid2;
            Customer *c2=bank.findCustomerById(cid2);
            if(c2==NULL)
            {
                cout<<"Target customer not found\n";
                continue;
            }
            cout<<"Enter target account no: ";
            cin>>acc2;
            Account *A2=c2->findAccount(acc2);
            if(A2==NULL)
            {
                cout<<" Target account not found\n";
                continue;
            }
            cout<<"Enter amount to transfer: ";
            cin>>amt;
            if(A1->transferTo(A2,amt))
            {
                cout<<"Transfer successful\n";
                cout<<"From A/c "<<A1->getAccNo()<<" New Bal: "<<A1->getBalance()<<"\n";
                cout<<"To A/c "<<A2->getAccNo()<<" New Bal: "<<A2->getBalance()<<"\n";
            }
            else
            {
                cout<<"Transfer failed. Check amount and balance\n";
            }
        }
        else if(opt==6)
        {
            int cid,acc;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<" Customer not found\n";
                continue;
            }
            cout<<"Enter account no: ";
            cin>>acc;
            Account *A=c->findAccount(acc);
            if(A==NULL)
            {
                cout<<"Account not found\n";
                continue;
            }
            A->showSummary();
        }
        else if(opt==7)
        {
            int cid,acc;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<"Customer not found\n";
                continue;
            }
            cout<<"Enter account no: ";
            cin>>acc;
            Account *A=c->findAccount(acc);
            if(A==NULL)
            {
                cout<<" Account not found\n";
                continue;
            }
            A->showAllTransactions();
        }
        else if(opt==8)
        {
            int cid,acc,k;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<" Customer not found\n";
                continue;
            }
            cout<<"Enter account no: ";
            cin>>acc;
            Account *A=c->findAccount(acc);
            if(A==NULL)
            {
                cout<<"Account not found\n";
                continue;
            }
            cout<<"How many recent transactions to show: ";
            cin>>k;
            A->showRecent(k);
        }
        else if(opt==9)
        {
            int cid;
            cout<<"Enter customer ID: ";
            cin>>cid;
            Customer *c=bank.findCustomerById(cid);
            if(c==NULL)
            {
                cout<<"Customer not found\n";
                continue;
            }
            c->showInfo();
        }
        else if(opt==10)
        {
            bank.listCustomers();
        }
        else
        {
            cout<<"Invalid option\n";
        }
    }
    cout<<"\nThank you for using  Bank\n";
    return 0;
}
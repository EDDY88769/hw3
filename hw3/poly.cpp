#include <iostream>
#include <cmath>
using namespace std;

struct Node{
    int coef;   //係數
    int exp;    //指數
    Node* link; //下一個節點

    Node(int c=0,int e=0):coef(c),exp(e),link(this){}
};

class Polynomial{
private:
    Node* head;   //第一個節點 

public:
    Polynomial();
    Polynomial(const Polynomial& a);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& a);        //等於 
    Polynomial operator+(const Polynomial& b) const;   //加法
    Polynomial operator-(const Polynomial& b) const;   //減法
    Polynomial operator*(const Polynomial& b) const;   //乘法
    float Evaluate(float x) const;                     //計算多項式值

    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};

// 預設建構子 
Polynomial::Polynomial(){
    head=new Node();
    head->link=head;
}

Polynomial::Polynomial(const Polynomial& a){
    head=new Node();
    head->link=head;
    Node* p=a.head->link;
    Node* r=head;
    while(p!=a.head){
        Node* newNode=new Node(p->coef,p->exp);
        r->link=newNode;
        r=newNode;
        p=p->link;
    }
    r->link=head;
}

//解構子 
Polynomial::~Polynomial(){
}

//等於a=b 
Polynomial& Polynomial::operator=(const Polynomial& a){
    if(this==&a)return *this;
    head=new Node();
    head->link=head;
    Node* p=a.head->link;
    Node* r=head;
    while(p!=a.head){
        Node* newNode=new Node(p->coef,p->exp);
        r->link=newNode;
        r=newNode;
        p=p->link;
    }
    r->link=head;
    return *this;
}

//加法a+b
Polynomial Polynomial::operator+(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link,*q=b.head->link;
    Node* r=result.head;

    while(p!=head && q!=b.head){
        if (p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        }else if(p->exp<q->exp){
            r->link=new Node(q->coef,q->exp);
            r=r->link;
            q=q->link;
        }else{
            int sumCoef=p->coef+q->coef;
            if(sumCoef!=0){
                r->link=new Node(sumCoef,p->exp);
                r=r->link;
            }
            p=p->link;
            q=q->link;
        }
    }

    while(p!=head){
        r->link=new Node(p->coef,p->exp);
        r=r->link;
        p=p->link;
    }

    while(q!=b.head){
        r->link=new Node(q->coef,q->exp);
        r=r->link;
        q=q->link;
	}
    r->link=result.head;
    return result;
}

//減法
Polynomial Polynomial::operator-(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link, *q=b.head->link;
    Node* r=result.head;

    while(p!=head && q!=b.head){
        if(p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        }else if(p->exp<q->exp){
            r->link=new Node(-q->coef,q->exp);
            r=r->link;
            q=q->link;
        }else{
            int diffCoef=p->coef-q->coef;
            if(diffCoef!=0){
                r->link=new Node(diffCoef,p->exp);
                r=r->link;
            }
            p=p->link;
            q=q->link;
        }
    }

    while(p!=head){
        r->link=new Node(p->coef,p->exp);
        r=r->link;
        p=p->link;
    }

    while(q!=b.head){
        r->link=new Node(-q->coef,q->exp);
        r=r->link;
        q=q->link;
    }

    r->link=result.head;
    return result;
}

//乘法
Polynomial Polynomial::operator*(const Polynomial& b) const{
    Polynomial result;
    Node* p=head->link;

    while(p!=head){
        Polynomial temp;
        Node* q=b.head->link;
        Node* r=temp.head;
        while(q!=b.head){
            r->link=new Node(p->coef*q->coef,p->exp+q->exp);
            r=r->link;
            q=q->link;
        }
        r->link=temp.head;
        result=result+temp;
        p=p->link;
    }

    return result;
}

// 計算帶入X後的值 
float Polynomial::Evaluate(float x) const{
    float result=0;
    Node* p=head->link;
    while(p!=head){
        result+=p->coef*pow(x,p->exp);
        p=p->link;
    }
    return result;
}

//輸入
istream& operator>>(istream& is,Polynomial& x){
    int n;
    is>>n;
    Node* r=x.head;
    for(int i=0;i<n;++i){
        int coef,exp;
        is>>coef>>exp;
        Node* newNode=new Node(coef,exp);
        r->link=newNode;
        r=newNode;
    }
    r->link=x.head;
    return is;
}

//輸出
ostream& operator<<(ostream& os,const Polynomial& x){
    Node* p=x.head->link;
    bool first=true;
    while(p!=x.head){
        if(!first && p->coef>0) os<<"+";
        if(p->exp == 0) {
            os<<p->coef;
        }else{
            os<<p->coef<<"x^"<<p->exp;
        }
        first=false;
        p=p->link;
    }
    return os;
}
int main() {
    Polynomial p1,p2;
    cout<<"請輸入多項式(格式為:n(項數) cof1(係數1) exp1(指數1) cof2(係數2) exp2(指數2).....)\np1:";
    cin>>p1;
    cout<<"p2:";
    cin>>p2;
    Polynomial sum=p1+p2;
    Polynomial diff=p1-p2;
    Polynomial prod=p1*p2;
    cout<<"p1: "<<p1<<endl;
    cout<<"p2: "<<p2<<endl;
    cout<<"p1+p2: "<<sum<<endl;
    cout<<"p1-p2: "<<diff<<endl;
    cout<<"p1*p2: "<<prod<<endl;
    float x;
    cout<<"請輸入要帶入的X值:";
    cin>>x;
    cout<<"p1("<<x<<")="<<p1.Evaluate(x)<<endl;
    cout<<"p2("<<x<<")="<<p2.Evaluate(x)<<endl;
    return 0;
}


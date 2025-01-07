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

    friend istream& operator>>(istream& is, Polynomial& x);       //輸入 
    friend ostream& operator<<(ostream& os, const Polynomial& x); //輸出 
};

// 預設建構子 
Polynomial::Polynomial(){
    head=new Node();
    head->link=head;
}

//建構子
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

//等於 
Polynomial& Polynomial::operator=(const Polynomial& a){
    head=new Node();
    head->link=head;
    Node* p=a.head->link;     //從a的頭後面一個開始 
    Node* r=head;			  //指向新串列的第一個 
    //開始複製 
    while(p!=a.head){         //把每一項都跑一遍 
        Node* newNode=new Node(p->coef,p->exp);
        r->link=newNode;      //把新串列串在一起 
        r=newNode;			  //再往新串列的下一個指 
        p=p->link;			  //原串列指向下一個 
    }
    r->link=head;             //形成環狀 
    return *this;
}

//加法a+b
Polynomial Polynomial::operator+(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link,*q=b.head->link;
    Node *r=result.head;

    while(p!=head && q!=b.head){
    	//這邊會先指數做比大小 大的就會先被放到串列裡面 
        if (p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        }else if(p->exp<q->exp){
            r->link=new Node(q->coef,q->exp);
            r=r->link;
            q=q->link;
        //如果相同的話就會把係數相加 然後加入串列    
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
	
	//如果有剩下的項目會再被加進去 
    while(p!=head){
        r->link=new Node(p->coef,p->exp);
        r=r->link;
        p=p->link;
    }
	//如果有剩下的項目會再被加進去 
    while(q!=b.head){
        r->link=new Node(q->coef,q->exp);
        r=r->link;
        q=q->link;
	}
    r->link=result.head;  //形成環狀 
    return result;
}

//減法
Polynomial Polynomial::operator-(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link, *q=b.head->link;
    Node* r=result.head;

    while(p!=head && q!=b.head){
    	//這邊會先指數做比大小 大的就會先被放到串列裡面 
        if(p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        //這裡比較特別因為是p-q所以再把數字新增進去的時候要加負號    
        }else if(p->exp<q->exp){
            r->link=new Node(-q->coef,q->exp);
            r=r->link;
            q=q->link;
        //如果相同的話就會把係數相減 然後加入串列      
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
	//把每一項都相乘 
    while(p!=head){             //把每一項都跑一遍 
        Polynomial temp;        //儲存相乘後結果 
        Node* q=b.head->link;
        Node* r=temp.head;      
        while(q!=b.head){
            r->link=new Node(p->coef*q->coef,p->exp+q->exp);
            r=r->link;
            q=q->link;
        }
        r->link=temp.head;     //形成環狀 
        result=result+temp;    //使用加法多載把同指數的部分相加 
        p=p->link;
    }
    return result;
}

// 計算帶入X後的值 
float Polynomial::Evaluate(float x) const{
    float result=0;
    Node* p=head->link;                //從a的頭後面一個開始
    while(p!=head){					   //把每一項都跑一遍
        result+=p->coef*pow(x,p->exp); //係數*指數的X次方 
        p=p->link;                     //往下一個跑 
    }
    return result;
}

//輸入
istream& operator>>(istream& is,Polynomial& x){
    int n;
    is>>n;                   //讀取有幾項 
    Node* r=x.head;
    for(int i=0;i<n;++i){
        int coef,exp;
        is>>coef>>exp;       //輸入係數、指數 
        Node* newNode=new Node(coef,exp);     //加入到新串列 
        r->link=newNode;                      //把新節點連結到串列裡面 
        r=newNode;                            //把R指向最新新的節點 
    }
    r->link=x.head;                           //形成環狀 
    return is;
}

//輸出
ostream& operator<<(ostream& os,const Polynomial& x){
    Node* p=x.head->link;
    bool first=true;        //檢查是不是第一個 
    while(p!=x.head){       //把整個串列跑過一遍            //a為係數  b為指數 
        if(!first && p->coef>0) os<<"+";      //如果不是第一個且為正數的話就會加正號 
        if(p->exp == 0) {                     //如果指數為0就直接把係數印出 
            os<<p->coef;
        }else if(p->coef!=1 && p->coef!=-1 && p->exp>1){     //係數不為正負1且指數大於1就依ax^b的格式印出 
            os<<p->coef<<"x^"<<p->exp;
        }else if(p->coef==-1 && p->exp>1){    //係數為正負1且指數大於1就依x^b或是-x^b的格式印出 
        	os<<"-x^"<<p->exp;	
		}else if(p->coef==1 && p->exp>1){
        	os<<"x^"<<p->exp;	
		}else if(p->coef==1 && p->exp==1){    //係數為正負1且指數等於1就依x或是-x的格式印出 
        	os<<"x";	
		}else if(p->coef==-1&& p->exp==1){
        	os<<"-x";	
		}else if(p->exp=1){                   //指數為1就直接依ax的格式印出 
        	os<<p->coef<<"x";
		}
        first=false;
        p=p->link;
    }
    return os;
}
int main() {
    Polynomial p1,p2;
    //輸入多項式 
    cout<<"請輸入多項式(格式為:n(項數) cof1(係數1) exp1(指數1) cof2(係數2) exp2(指數2).....)\np1:";
    cin>>p1;
    cout<<"p2:";
    cin>>p2;
    
    Polynomial sum=p1+p2;      //相加 
    Polynomial diff=p1-p2;	   //相減 
    Polynomial prod=p1*p2;     //相乘
	
	//印出結果 
    cout<<"p1: "<<p1<<endl;
    cout<<"p2: "<<p2<<endl;
    cout<<"p1+p2: "<<sum<<endl;
    cout<<"p1-p2: "<<diff<<endl;
    cout<<"p1*p2: "<<prod<<endl;
	
	//輸入X值 
    float x;
    cout<<"請輸入要帶入的X值:";
    cin>>x;
    cout<<"p1("<<x<<")="<<p1.Evaluate(x)<<endl;
    cout<<"p2("<<x<<")="<<p2.Evaluate(x)<<endl;
    return 0;
}


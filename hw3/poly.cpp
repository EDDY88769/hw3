#include <iostream>
#include <cmath>
using namespace std;

struct Node{
    int coef;   //�Y��
    int exp;    //����
    Node* link; //�U�@�Ӹ`�I

    Node(int c=0,int e=0):coef(c),exp(e),link(this){}
};

class Polynomial{
private:
    Node* head;   //�Ĥ@�Ӹ`�I 

public:
    Polynomial();
    Polynomial(const Polynomial& a);
    ~Polynomial();

    Polynomial& operator=(const Polynomial& a);        //���� 
    Polynomial operator+(const Polynomial& b) const;   //�[�k
    Polynomial operator-(const Polynomial& b) const;   //��k
    Polynomial operator*(const Polynomial& b) const;   //���k
    float Evaluate(float x) const;                     //�p��h������

    friend istream& operator>>(istream& is, Polynomial& x);       //��J 
    friend ostream& operator<<(ostream& os, const Polynomial& x); //��X 
};

// �w�]�غc�l 
Polynomial::Polynomial(){
    head=new Node();
    head->link=head;
}

//�غc�l
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

//�Ѻc�l 
Polynomial::~Polynomial(){
}

//���� 
Polynomial& Polynomial::operator=(const Polynomial& a){
    head=new Node();
    head->link=head;
    Node* p=a.head->link;     //�qa���Y�᭱�@�Ӷ}�l 
    Node* r=head;			  //���V�s��C���Ĥ@�� 
    //�}�l�ƻs 
    while(p!=a.head){         //��C�@�����]�@�M 
        Node* newNode=new Node(p->coef,p->exp);
        r->link=newNode;      //��s��C��b�@�_ 
        r=newNode;			  //�A���s��C���U�@�ӫ� 
        p=p->link;			  //���C���V�U�@�� 
    }
    r->link=head;             //�Φ����� 
    return *this;
}

//�[�ka+b
Polynomial Polynomial::operator+(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link,*q=b.head->link;
    Node *r=result.head;

    while(p!=head && q!=b.head){
    	//�o��|�����ư���j�p �j���N�|���Q����C�̭� 
        if (p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        }else if(p->exp<q->exp){
            r->link=new Node(q->coef,q->exp);
            r=r->link;
            q=q->link;
        //�p�G�ۦP���ܴN�|��Y�Ƭۥ[ �M��[�J��C    
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
	
	//�p�G���ѤU�����ط|�A�Q�[�i�h 
    while(p!=head){
        r->link=new Node(p->coef,p->exp);
        r=r->link;
        p=p->link;
    }
	//�p�G���ѤU�����ط|�A�Q�[�i�h 
    while(q!=b.head){
        r->link=new Node(q->coef,q->exp);
        r=r->link;
        q=q->link;
	}
    r->link=result.head;  //�Φ����� 
    return result;
}

//��k
Polynomial Polynomial::operator-(const Polynomial& b) const{
    Polynomial result;
    Node *p=head->link, *q=b.head->link;
    Node* r=result.head;

    while(p!=head && q!=b.head){
    	//�o��|�����ư���j�p �j���N�|���Q����C�̭� 
        if(p->exp>q->exp){
            r->link=new Node(p->coef,p->exp);
            r=r->link;
            p=p->link;
        //�o�̤���S�O�]���Op-q�ҥH�A��Ʀr�s�W�i�h���ɭԭn�[�t��    
        }else if(p->exp<q->exp){
            r->link=new Node(-q->coef,q->exp);
            r=r->link;
            q=q->link;
        //�p�G�ۦP���ܴN�|��Y�Ƭ۴� �M��[�J��C      
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

//���k
Polynomial Polynomial::operator*(const Polynomial& b) const{
    Polynomial result;
    Node* p=head->link;
	//��C�@�����ۭ� 
    while(p!=head){             //��C�@�����]�@�M 
        Polynomial temp;        //�x�s�ۭ��ᵲ�G 
        Node* q=b.head->link;
        Node* r=temp.head;      
        while(q!=b.head){
            r->link=new Node(p->coef*q->coef,p->exp+q->exp);
            r=r->link;
            q=q->link;
        }
        r->link=temp.head;     //�Φ����� 
        result=result+temp;    //�ϥΥ[�k�h����P���ƪ������ۥ[ 
        p=p->link;
    }
    return result;
}

// �p��a�JX�᪺�� 
float Polynomial::Evaluate(float x) const{
    float result=0;
    Node* p=head->link;                //�qa���Y�᭱�@�Ӷ}�l
    while(p!=head){					   //��C�@�����]�@�M
        result+=p->coef*pow(x,p->exp); //�Y��*���ƪ�X���� 
        p=p->link;                     //���U�@�Ӷ] 
    }
    return result;
}

//��J
istream& operator>>(istream& is,Polynomial& x){
    int n;
    is>>n;                   //Ū�����X�� 
    Node* r=x.head;
    for(int i=0;i<n;++i){
        int coef,exp;
        is>>coef>>exp;       //��J�Y�ơB���� 
        Node* newNode=new Node(coef,exp);     //�[�J��s��C 
        r->link=newNode;                      //��s�`�I�s�����C�̭� 
        r=newNode;                            //��R���V�̷s�s���`�I 
    }
    r->link=x.head;                           //�Φ����� 
    return is;
}

//��X
ostream& operator<<(ostream& os,const Polynomial& x){
    Node* p=x.head->link;
    bool first=true;        //�ˬd�O���O�Ĥ@�� 
    while(p!=x.head){       //���Ӧ�C�]�L�@�M            //a���Y��  b������ 
        if(!first && p->coef>0) os<<"+";      //�p�G���O�Ĥ@�ӥB�����ƪ��ܴN�|�[���� 
        if(p->exp == 0) {                     //�p�G���Ƭ�0�N������Y�ƦL�X 
            os<<p->coef;
        }else if(p->coef!=1 && p->coef!=-1 && p->exp>1){     //�Y�Ƥ������t1�B���Ƥj��1�N��ax^b���榡�L�X 
            os<<p->coef<<"x^"<<p->exp;
        }else if(p->coef==-1 && p->exp>1){    //�Y�Ƭ����t1�B���Ƥj��1�N��x^b�άO-x^b���榡�L�X 
        	os<<"-x^"<<p->exp;	
		}else if(p->coef==1 && p->exp>1){
        	os<<"x^"<<p->exp;	
		}else if(p->coef==1 && p->exp==1){    //�Y�Ƭ����t1�B���Ƶ���1�N��x�άO-x���榡�L�X 
        	os<<"x";	
		}else if(p->coef==-1&& p->exp==1){
        	os<<"-x";	
		}else if(p->exp=1){                   //���Ƭ�1�N������ax���榡�L�X 
        	os<<p->coef<<"x";
		}
        first=false;
        p=p->link;
    }
    return os;
}
int main() {
    Polynomial p1,p2;
    //��J�h���� 
    cout<<"�п�J�h����(�榡��:n(����) cof1(�Y��1) exp1(����1) cof2(�Y��2) exp2(����2).....)\np1:";
    cin>>p1;
    cout<<"p2:";
    cin>>p2;
    
    Polynomial sum=p1+p2;      //�ۥ[ 
    Polynomial diff=p1-p2;	   //�۴� 
    Polynomial prod=p1*p2;     //�ۭ�
	
	//�L�X���G 
    cout<<"p1: "<<p1<<endl;
    cout<<"p2: "<<p2<<endl;
    cout<<"p1+p2: "<<sum<<endl;
    cout<<"p1-p2: "<<diff<<endl;
    cout<<"p1*p2: "<<prod<<endl;
	
	//��JX�� 
    float x;
    cout<<"�п�J�n�a�J��X��:";
    cin>>x;
    cout<<"p1("<<x<<")="<<p1.Evaluate(x)<<endl;
    cout<<"p2("<<x<<")="<<p2.Evaluate(x)<<endl;
    return 0;
}


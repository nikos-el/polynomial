#include <cmath>

class Polynomial {
  protected:
    class Term {
      protected:
        int exponent;
        int coefficient;
        Term *next;
        Term(int exp, int coeff, Term *n): exponent(exp), coefficient(coeff), next(n) {}
        friend class Polynomial;
    };

  public:
    Polynomial(): head(new Term(0,0,nullptr)) {}
    Polynomial(const Polynomial &p): head(new Term(0,0,nullptr)) { copy(p); }
    ~Polynomial(){ purge(); }

    Polynomial & operator = (const Polynomial &p) {
      purge();
      head=new Term(0,0,nullptr);
      copy(p);
      return *this;
    }

    void addTerm(int expon, int coeff){
      if (coeff!=0){
        Term *i, *j=head;
        for (i=head; i->next!=nullptr && i->next->exponent>=expon; i=i->next) j=i;
        if (i->exponent==expon) {
          i->coefficient+=coeff;
          if (i->coefficient==0 && i->exponent!=0) { 
            if (i!=head) { j->next=i->next; delete i; }
            else { head=i->next; delete i; }
          }
        }
        else {
          Term *a=new Term(expon,coeff,nullptr);
          if (i==head && head->exponent<expon) { a->next=i; head=a; }
          else {
            a->next=i->next; i->next=a;
            if (a->coefficient==0 && a->exponent!=0) { i->next=a->next; delete a; }
          }
        }
      }
    }

    double evaluate(double x){
      double value=0;
      for (Term *i=head; i!=nullptr; i=i->next)
        value+=((double) i->coefficient)*pow(x,i->exponent);
      return value;
    }

    friend Polynomial operator + (const Polynomial &p, const Polynomial &q){
      Polynomial r(p);
      r.copy(q);
      return r;
    }

    friend Polynomial operator * (const Polynomial &p, const Polynomial &q){
      return p.mult(q);
    }

    friend ostream & operator << (ostream &out, const Polynomial &p){
      return p.print_Polynomial(out);
    }

  private:
    Term *head;

    void copy(const Polynomial &p) {
      for (Term *i=p.head; i!=nullptr; i=i->next)
        addTerm(i->exponent, i->coefficient);
    }

    void purge(){
      Term *i=head;
      while(i!=nullptr){
        Term *j=i;
        i=i->next;
        delete j;
      }
    }
    
    Polynomial mult(const Polynomial &q) const{
      Polynomial r;
      for (Term *i=head; i!=nullptr; i=i->next)
        for (Term *j=q.head; j!=nullptr; j=j->next)
          r.addTerm((i->exponent+j->exponent), (i->coefficient*j->coefficient));
      return r;
    }

    ostream & print_Polynomial(ostream &out) const{
      for (Term *i=head; i!=nullptr; i=i->next){
        if (i->coefficient<0) out<<"- ";
        else if (i->coefficient>0 && i!=head) out<<"+ ";
        else if (i->coefficient==0 && i==head) out<<0;
        if (abs(i->coefficient)!=1 && i->exponent>0) out<<abs(i->coefficient);
        if (i->exponent>0) out<<"x";
        else if (i->exponent==0 && i->coefficient!=0) out<<abs(i->coefficient);
        if (i->exponent>1) out<<"^"<<i->exponent;
        if (i->next!=nullptr && i->next->coefficient!=0) out<<" ";
      }
      return out;
    }
};

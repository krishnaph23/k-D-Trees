#include<iostream>
#include<algorithm>
#include"kd.hpp"

using namespace kdt;
using namespace std;

class emp{
  private:
    int eid_;
    int sal_;
  public:
    emp(int eid,int sal):eid_(eid),sal_(sal){}

    friend bool operator<(const emp& e1,const emp& e2){return e1.sal_<e2.sal_;}
    friend bool operator>(const emp& e1,const emp& e2){return e1.sal_>e2.sal_;}
    friend bool operator<=(const emp& e1,const emp& e2){return e1.sal_<=e2.sal_;}
    friend bool operator>=(const emp& e1,const emp& e2){return e1.sal_>=e2.sal_;}
    friend bool operator==(const emp& e1,const emp& e2){return e1.eid_==e2.eid_;}
    friend bool operator!=(const emp& e1,const emp& e2){return e1.eid_!=e2.eid_;}

    friend ostream& operator<<(ostream& o,const emp& e){
      return o<<"("<<e.eid_<<','<<e.sal_<<")";
    }

    int salary(){
      return sal_;
    }
};

int main(){
  KDTree<int,char> tree;
  tree.insert(10,'d');
  tree.insert(5,'e');
  tree.remove(5,'e');// removing node
  tree.insert(5,'e');
  tree.insert(5,'e');
  tree.insert(6,'z');
  tree.insert(15,'r');
  tree.insert(16,'s');
  cout<<*tree.search(5,'e')<<'\n'; // returns end iterator if not found
  cout<<*find(tree.begin(),tree.end(),KDTree_Node<int,char>(5,'e'))<<"\n";
  cout<<count(tree.begin(),tree.end(),KDTree_Node<int,char>(5,'e'))<<"\n";

  /*
  KDTree<int,emp> tree;
  tree.insert(10,emp(5,1000));
  tree.insert(5,emp(2,500));
  tree.insert(11,emp(4,7000));
  tree.insert(15,emp(6,5000));
  tree.insert(7,emp(3,100));

  for(auto i:tree)
    cout<<i<<" ";
  cout<<"\n";
  for(auto i=tree.begin_pre();i!=tree.end_pre();i++)
    cout<<*i<<" ";
  cout<<"\n";
  for(auto i=tree.begin_post();i!=tree.end_post();i++)
      cout<<*i<<" ";
  cout<<"\n";

  KDTree<int,emp> tree1;
  tree1.insert(10,emp(5,1000));
  tree1.insert(5,emp(2,500));
  tree1.insert(6,emp(9,7000));
  tree1.insert(15,emp(6,5000));
  tree1.insert(7,emp(3,100));*/
  //cout<<all_of(tree.begin(),tree.end(),[](auto i){return i.x()>0 && i.y().salary()>0;})<<"\n";
  //cout<<equal(tree.begin(),tree.end(),tree1.begin());
}

tree.insert(10,emp(15,1000));
tree.insert(15,emp(25,300));
tree.insert(5,emp(20,2000));
tree.insert(8,emp(30,1500));
tree.insert(9,emp(35,1800));
tree.insert(12,emp(45,350));
tree.insert(11,emp(50,400));
tree.insert(20,emp(40,250));

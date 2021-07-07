#pragma once
#include<iterator>
#include<cstddef>

#define K 2
namespace kdt{

template<typename X,typename Y>
class KDTree_Node{
  private:
  KDTree_Node* left_;
  KDTree_Node* right_;
  KDTree_Node* parent_;
  X x_;
  Y y_;
  template<typename T1,typename T2> friend class KDTree;
  public:
  KDTree_Node(X x,Y y):x_(x),y_(y),left_(nullptr),right_(nullptr),parent_(nullptr){}
  X x(){
    return x_;
  }
  Y y(){
    return y_;
  }
  friend std::ostream& operator<<(std::ostream& o,const KDTree_Node<X,Y>& node){
    return o<<"("<<node.x_<<","<<node.y_<<")";
  }
  friend bool operator==(const KDTree_Node<X,Y>& lhs,const KDTree_Node<X,Y>& rhs){
    return (lhs.x_==rhs.x_) && (lhs.y_==rhs.y_);
  }
  friend bool operator!=(const KDTree_Node<X,Y>& lhs,const KDTree_Node<X,Y>& rhs){
    return !(lhs==rhs);
  }

};


template<typename X,typename Y>
class KDTree{
  private:
  KDTree_Node<X,Y> *head_;
  bool align_;
  int len_;
  KDTree_Node<X,Y>* minNode(KDTree_Node<X,Y>* x,KDTree_Node<X,Y>* y,KDTree_Node<X,Y>* z,int axis);
  KDTree_Node<X,Y>* min(KDTree_Node<X,Y>* root,int axis,int depth);
  KDTree_Node<X,Y>* deleteNode(KDTree_Node<X,Y>* root,X x,Y y,int depth);
  KDTree_Node<X,Y>* copyTree(KDTree_Node<X,Y>* root);
  public:
  explicit KDTree(bool align=0):align_(align),head_(nullptr),len_(0){}
  ~KDTree(){deleteTree(head_);}
  KDTree(const KDTree<X,Y>& tree){
    align_=tree.align_;
    head_=copyTree(tree.head_);
  }
  class iterator_in;
  class iterator_pre;
  class iterator_post;
  KDTree<X,Y>& operator=(const KDTree&);
  int remove(X x,Y y);
  void insert(X x,Y y);
  auto search(X x,Y y);
  void deleteTree(KDTree_Node<X,Y>* node);


  iterator_post begin_post(){
    KDTree_Node<X,Y>* temp=head_;
    while(temp->left_!=nullptr)
      temp=temp->left_;
    while(temp->right_!=nullptr)
      temp=temp->right_;
    return iterator_post(temp);
  }

  iterator_post end_post(){
    return iterator_post(nullptr);
  }

  iterator_in begin(){
    KDTree_Node<X,Y>* temp=head_;
    while(temp->left_!=nullptr)
      temp=temp->left_;
    return iterator_in(temp);
  }

  iterator_in end(){
    return iterator_in(nullptr);
  }

  iterator_pre begin_pre(){
    return iterator_pre(head_);
  }

  iterator_pre end_pre(){
    return iterator_pre(nullptr);
  }

  iterator_in left(){
    this->node_ptr_=this->node_ptr_->left_;
    return this;
  }
  iterator_in right(){
    this->node_ptr_=this->node_ptr_->right_;
    return this;
  }
  int no_of_nodes(){return len_;}
};

template<typename X,typename Y>
class KDTree<X,Y>::iterator_post{
  public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef KDTree_Node<X,Y> value_type;
  typedef KDTree_Node<X,Y>& reference;
  typedef KDTree_Node<X,Y>* pointer;

  iterator_post(pointer ptr):node_ptr_(ptr){}
  iterator_post(const iterator_post& it){this->node_ptr_=it.node_ptr_;}
  iterator_post operator=(const iterator_post& it){this->node_ptr_=it.node_ptr_; return *this;}
  const reference operator*() const {return *node_ptr_;}
  const pointer operator->() {return node_ptr_;}

  iterator_post& operator++(){
    if(this->node_ptr_==nullptr || this->node_ptr_->parent_==nullptr)
      this->node_ptr_=nullptr;
    else{
      pointer parent=this->node_ptr_->parent_;
      if(parent->right_==nullptr || parent->right_==this->node_ptr_)
        this->node_ptr_=parent;
      else{
        pointer temp=parent->right_;
        while(temp->left_!=nullptr)
          temp=temp->left_;
        while(temp->right_!=nullptr)
          temp=temp->right_;
        this->node_ptr_=temp;
      }
    }
    return *this;
  }

  iterator_post operator++(int){
    iterator_post temp=*this;
    ++(*this);
    return temp;
  }

  iterator_post& operator--(){
    if(this->node_ptr_==nullptr)
      return *this;
    if(this->node_ptr_->right_!=nullptr)
      this->node_ptr_=this->node_ptr_->right_;
    else if(this->node_ptr_->left_!=nullptr)
      this->node_ptr_=this->node_ptr_->left_;
    else{
      pointer temp=this->node_ptr_;
      pointer parent=temp->parent_;
      while(parent!=nullptr && (parent->left_==temp||parent->left_==nullptr)){
        temp=temp->parent_;
        parent=parent->parent_;
      }
      if(parent==nullptr)
        this->node_ptr_=nullptr;
      else
        this->node_ptr_=parent->left_;
    }
    return *this;
  }

  iterator_post operator--(int){
    iterator_post temp=*this;
    --(*this);
    return temp;
  }

  friend bool operator==(const iterator_post& lhs,const iterator_post& rhs){return lhs.node_ptr_==rhs.node_ptr_;}
  friend bool operator!=(const iterator_post& lhs,const iterator_post& rhs){return !(lhs==rhs);}
  private:
  pointer node_ptr_;

};

template<typename X,typename Y>
class KDTree<X,Y>::iterator_pre{
  public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef KDTree_Node<X,Y> value_type;
  typedef KDTree_Node<X,Y>& reference;
  typedef KDTree_Node<X,Y>* pointer;

  iterator_pre(pointer ptr):node_ptr_(ptr){}
  iterator_pre(const iterator_pre& it){this->node_ptr_=it.node_ptr_;}
  iterator_pre operator=(const iterator_in& it){this->node_ptr_=it.node_ptr_; return *this;}
  const reference operator*() const {return *node_ptr_;}
  const pointer operator->() {return node_ptr_;}

  iterator_pre& operator++(){
    pointer temp=this->node_ptr_;
    if(temp==nullptr)
      return *this;
    if(temp->left_!=nullptr){
      this->node_ptr_=temp->left_;
    }
    else if(temp->right_!=nullptr){
      this->node_ptr_=temp->right_;
    }
    else{
      pointer parent=temp->parent_;
      while(parent!=nullptr && (parent->right_==nullptr || parent->right_==temp)){
        temp=temp->parent_;
        parent=parent->parent_;
      }
      if(parent==nullptr)
        this->node_ptr_=nullptr;
      else
        this->node_ptr_=parent->right_;
    }
    return *this;
  }

  iterator_pre operator++(int){
    iterator_pre temp=*this;
    ++(*this);
    return temp;
  }

  friend bool operator==(const iterator_pre& lhs,const iterator_pre& rhs){return lhs.node_ptr_==rhs.node_ptr_;}
  friend bool operator!=(const iterator_pre& lhs,const iterator_pre& rhs){return !(lhs==rhs);}

  iterator_pre& operator--(){
    pointer temp=this->node_ptr_;
    if(temp==nullptr)
      return *this;
    if(temp->parent_==nullptr)
      this->node_ptr_=nullptr;
    else{
      pointer parent=temp->parent_;
      if(parent->left_==nullptr || parent->left_==temp)
        this->node_ptr_=parent;
      else{
        temp=parent->left_;
        while(temp->right_!=nullptr)
          temp=temp->right_;
        while(temp->left_!=nullptr)
          temp=temp->left;
        this->node_ptr_=temp;
      }
    }
    return *this;
  }

  iterator_pre operator--(int){
    iterator_pre temp=*this;
    --(*this);
    return temp;
  }
  private:
  pointer node_ptr_;
};

template<typename X,typename Y>
class KDTree<X,Y>::iterator_in{
  public:
  typedef std::bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef KDTree_Node<X,Y> value_type;
  typedef KDTree_Node<X,Y>& reference;
  typedef KDTree_Node<X,Y>* pointer;

  iterator_in(pointer ptr):node_ptr_(ptr){}
  iterator_in(const iterator_in& it){this->node_ptr_=it.node_ptr_;}
  iterator_in operator=(const iterator_in& it){this->node_ptr_=it.node_ptr_; return *this;}
  const reference operator*() const {return *node_ptr_;}
  const pointer operator->() {return node_ptr_;}

  iterator_in& operator++(){
    pointer temp=nullptr;
    if(this->node_ptr_->right_!=nullptr){
      temp=this->node_ptr_->right_;
      while(temp->left_!=nullptr)
        temp=temp->left_;
      this->node_ptr_=temp;
    }
    else{
      pointer temp=this->node_ptr_;
      pointer p=temp->parent_;
      while(p!=nullptr && temp==p->right_){
        temp=p;
        p=p->parent_;
      }
      this->node_ptr_=p;
    }
    return *this;
  }

  iterator_in operator++(int){
    iterator_in temp=*this;
    ++(*this);
    return temp;
  }

  iterator_in& operator--(){
    pointer temp=nullptr;
    if(this->node_ptr_->left_!=nullptr){
      temp=this->node_ptr_->left_;
      while(temp->right_!=nullptr)
        temp=temp->right_;
      this->node_ptr_=temp;
    }
    else{
      pointer temp=this->node_ptr_;
      pointer p=temp->parent_;
      while(p!=nullptr && temp==p->left_){
        temp=p;
        p=p->parent_;
      }
      this->node_ptr_=p;
    }
    return *this;
  }

  iterator_in operator--(int){
    iterator_in temp=*this;
    --(*this);
    return temp;
  }

  friend bool operator==(const iterator_in& lhs,const iterator_in& rhs){return lhs.node_ptr_==rhs.node_ptr_;}
  friend bool operator!=(const iterator_in& lhs,const iterator_in& rhs){return !(lhs==rhs);}
  private:
  pointer node_ptr_;

};


template<typename X,typename Y>
KDTree<X,Y>& KDTree<X,Y>::operator=(const KDTree<X,Y>& tree){
  deleteTree(head_);
  head_=copyTree(tree.root);
  return *this;
}

template<typename X,typename Y>
KDTree_Node<X,Y>* KDTree<X,Y>::copyTree(KDTree_Node<X,Y>* root){
  if(root==nullptr)
    return nullptr;
  KDTree_Node<X,Y>* node=new KDTree_Node<X,Y>();
  node->x_=root->x_;
  node->y_=root->y_;
  node->left_=copyTree(root->left_);
  node->right_=copyTree(root->right_);
  return node;
}

template<typename X,typename Y>
int KDTree<X,Y>::remove(X x,Y y){
  int l=len_;
  head_=deleteNode(head_,x,y,0);
  if(l!=len_){
    return 1;
  }
  return 0;
}

template<typename X,typename Y>
void KDTree<X,Y>::deleteTree(KDTree_Node<X,Y>* node){
  if(node==nullptr)
    return;
  deleteTree(node->left_);
  deleteTree(node->right_);
  delete node;
}

template<typename X,typename Y>
auto KDTree<X,Y>::search(X x,Y y){
  int i=align_;
  KDTree_Node<X,Y>* temp=head_;
  while(temp!=nullptr){
    if(temp->x_==x && temp->y_==y)
      return iterator_in(temp);
    if(!i){
        if(x>=temp->x_){
          temp=temp->right_;
        }
        else{
          temp=temp->left_;
        }
      }
    else{
        if(y>=temp->y_){
          temp=temp->right_;
        }
        else{
          temp=temp->left_;
        }
      }
      i=(i+1)%K;
    }
    return iterator_in(nullptr);
  }

template<typename X,typename Y>
void KDTree<X,Y>::insert(X x,Y y){
  KDTree_Node<X,Y>* node=new KDTree_Node<X,Y>(x,y);
  if(head_==nullptr){
    head_=node;
  }
  else{
    int i=align_;
    KDTree_Node<X,Y>* temp=head_;
    KDTree_Node<X,Y>* prev=nullptr;
    while(temp!=nullptr){
      prev=temp;
      if(!i){
        if(x>=temp->x_){
          temp=temp->right_;
        }
        else{
          temp=temp->left_;
        }
      }
      else{
        if(y>=temp->y_){
          temp=temp->right_;
        }
        else{
          temp=temp->left_;
        }
      }
      i=(i+1)%K;
    }
    node->parent_=prev;
    if(!i){
      if(y>=prev->y_)
        prev->right_=node;
      else
        prev->left_=node;
    }
    else{
      if(x>=prev->x_)
        prev->right_=node;
      else
        prev->left_=node;
    }
  }
  len_+=1;
}

template<typename X,typename Y>
KDTree_Node<X,Y>* KDTree<X,Y>::minNode(KDTree_Node<X,Y>* x,KDTree_Node<X,Y>* y,KDTree_Node<X,Y>* z,int axis){
  KDTree_Node<X,Y>* res=x;
  if(!axis){
    if(y!=nullptr && y->x_<res->x_)
      res=y;
    if(z!=nullptr && z->x_<res->x_)
      res=z;
    return res;
  }
  else{
    if(y!=nullptr && y->y_<res->y_)
      res=y;
    if(z!=nullptr && z->y_<res->y_)
      res=z;
    return res;
  }
}

template<typename X,typename Y>
KDTree_Node<X,Y>* KDTree<X,Y>::deleteNode(KDTree_Node<X,Y>* root,X x,Y y,int depth){
    if(root==NULL)
      return nullptr;
    int i=depth%K;
    if(root->x_==x && root->y_==y){
      if(root->right_!=nullptr){
        KDTree_Node<X,Y>* node=min(root->right_,i,0);
        root->x_=node->x_;
        root->y_=node->y_;
        root->right_=deleteNode(root->right_,node->x_,node->y_,depth+1);
      }
      else if(root->left_!=nullptr){
        KDTree_Node<X,Y>* node=min(root->left_,i,0);
        root->x_=node->x_;
        root->y_=node->y_;
        root->right_=deleteNode(root->left_,node->x_,node->y_,depth+1);
      }
      else{
        delete root;
        len_-=1;
        return nullptr;
      }
      return root;
    }
    if(!i){
      if(x<root->x_)
        root->left_=deleteNode(root->left_,x,y,depth+1);
      else
        root->right_=deleteNode(root->right_,x,y,depth+1);
    }
    else{
      if(y<root->y_)
        root->left_=deleteNode(root->left_,x,y,depth+1);
      else
        root->right_=deleteNode(root->right_,x,y,depth+1);
    }
    return root;
 }

template<typename X,typename Y>
KDTree_Node<X,Y>* KDTree<X,Y>::min(KDTree_Node<X,Y>* root,int axis,int depth){
  if(root==nullptr)
    return nullptr;
  int i=depth%K;
  if(i==axis){
    if(root->left_==nullptr)
      return root;
    return min(root->left_,axis,depth+1);
  }
  return minNode(root,min(root->left_,axis,depth+1),min(root->right_,axis,depth+1),axis);
}
}
/*
int main(){
  KDTree<int,char> tree;
  tree.insert(10,'k');
  tree.insert(5,'z');
  tree.insert(11,'y');
  tree.insert(15,'a');
  tree.insert(7,'b');
  for(auto i:tree)
    cout<<i;
}
*/

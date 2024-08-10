template<typename T>
class segment_tree
{ 
  // 1-base4
  private:
      
  public:
    template<typename F>
    class node{
      public:
      int lb, rb;
      F num, tag;
      node<F> *left, *right;
      node(){
        tag = 0;
        right = nullptr, left = nullptr;
      }
      T rv(){
        return num + tag * (rb - lb + 1);
      }
      void pull(){
        if(left) left -> tag += tag;
        if(right) right -> tag += tag;
        num = rv();
        tag = 0;
      }
    };
    node<T> *root;
    node<T> *build(vector<T> &save, int l, int r){
      node<T> *temp = new node<T>; 
      temp -> lb = l;
      temp -> rb = r;
      if (l == r)
      {   
        temp -> num = save[l];
        return temp;
      }
      int mid = (l + r) / 2;
      temp -> left = build(save, l, mid);
      temp -> right = build(save, mid  + 1, r);
      node<T> *left_node, *right_node;
      left_node = temp -> left;
      right_node = temp -> right;
      temp -> num = left_node -> num + right_node -> num;
      return temp;
    }
    T query(int l, int r, node<T> *t){
      t -> pull();
      if(l == t -> lb and r == t -> rb)
          return t -> num;
      int mid = (t -> lb + t -> rb) / 2;
      if(r <= mid) return query(l, r, t -> left);
      else if(l > mid) return query(l, r, t -> right);
      else return query(l, mid, t -> left) + query(mid + 1, r, t -> right);
    }
    void modify_node(int index, T delta, node<T> *t){
      if(t -> lb == t -> rb){
        t -> num += delta;
        return;
      }
      int mid = (t -> lb + t -> rb) / 2;
      if(index > mid) modify_node(index, delta, t -> right);
      else modify_node(index, delta, t -> left);
      t -> num += delta;
    }
    void modify_scope(int lb, int rb, int delta, node<T> *t){
      if(t -> lb >= lb and t -> rb <= rb){
        t -> tag += delta;
        return;
      }
      int mid = (t -> lb + t -> rb) / 2;
      if(t -> left and rb <= mid) modify_scope(lb, rb, delta, t -> left);
      else if(t -> right and lb > mid) modify_scope(lb, rb, delta, t -> right);
      else{
        modify_scope(lb, mid, delta, t -> left);
        modify_scope(mid + 1, rb, delta, t -> right);
      }
      if(t -> left and t -> right) t -> num = t -> left -> rv() + t -> right -> rv();
    }
};

signed main()
{
  int n, q;
  cin >> n >> q;
  vector<int> save(n + 1, 0);
  for(int i = 1; i <= n; i++){
    cin >> save[i];
  } 
  segment_tree<int> s;
  // init [1, n]
  s.root = s.build(save, 1, n);
  // modify [a, b] add c
  s.modify_scope(a, b, c, s.root);
  // query [a, b]
  s.query(a, b, s.root)
}
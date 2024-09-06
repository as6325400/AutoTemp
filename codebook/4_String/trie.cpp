class trie{
  public:
    class node{
      public:
        int count;
        vector<trie::node*> child;
        node(){
          child.resize(26, nullptr);
          count = 0;
        }
        ~node() {
          for (auto c : child) 
            if (c) delete c; 
        }
    };
    node* root;
    trie(){
      root = new node;
    }
    ~trie() {
      delete root; 
    }
    void insert(string s){
      auto temp = root;
      for(int i = 0; i < s.size(); i++){
        if(!temp -> child[s[i] - 'a']) temp -> child[s[i] - 'a'] = new node;
        temp = temp -> child[s[i] - 'a'];
      }
      temp -> count++;
    }
    bool search(string &s){
      auto temp = root;
      for(int i = 0; i < s.size(); i++){
        temp = temp -> child[s[i] - 'a'];
        if(!temp) return false;
      }
      if(temp -> count > 0) return true;
      return false;
    }
};
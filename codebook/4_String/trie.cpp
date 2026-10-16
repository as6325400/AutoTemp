// 僅接受 a..z；擁有整棵樹，不可複製
// 解構採遞迴，極深單字需改用節點池
class trie {
 public:
  struct node {
    int count = 0;
    vector<node*> child = vector<node*>(26, nullptr);
    ~node() {
      for (auto c : child) delete c;
    }
  };
  node* root = new node;
  trie() = default;
  trie(const trie&) = delete;
  trie& operator=(const trie&) = delete;
  ~trie() { delete root; }
  void insert(const string& s) {
    auto p = root;
    for (char c : s) {
      int k = c - 'a';
      if (!p->child[k]) p->child[k] = new node;
      p = p->child[k];
    }
    ++p->count;
  }
  bool search(const string& s) const {
    auto p = root;
    for (char c : s) {
      p = p->child[c - 'a'];
      if (!p) return false;
    }
    return p->count > 0;
  }
};

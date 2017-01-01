#ifndef AHO_CORASICK_HPP
#define AHO_CORASICK_HPP

#include <utility>
#include <cstring>
#include <vector>
#include <queue>

// aho corasick using integers 0 <= k < N
template <int N> 
class aho_corasick {
private:
    struct ac_node {
        ac_node* ch[N];
        ac_node* p;
        ac_node* suff_link;
        ac_node* dict_link;
        int cnt, num;
        std::vector<int> ind;
        ac_node() :p(nullptr), suff_link(nullptr), dict_link(nullptr), cnt(0), num(0) {
            std::memset(ch, 0, sizeof(ch));
        }

        ~ac_node() {
            for (int i = 0; i < N; i++) {
                delete ch[i];
            }
        }
    };

    std::vector<std::vector<int>> st;
    std::vector<ac_node*> top;
    ac_node* root;
    std::vector<std::pair<int, int>> ind;
    void insert(const std::vector<int>& x, int in, ac_node* cur, int pos = 0) {
        if (pos == x.size())
            return;

        if (cur->ch[x[pos]] == 0) {
            cur->ch[x[pos]] = new ac_node;
            cur->ch[x[pos]]->p = cur;
        }

        if (pos + 1 == x.size()) {
            cur->ch[x[pos]]->num++;
            cur->ch[x[pos]]->ind.push_back(in);
        }

        insert(x, in, cur->ch[x[pos]], pos + 1);
    }

    void make_suff_links() {
        std::queue<ac_node*> qu;
        qu.push(root);
        while (!qu.empty()) {
            ac_node* x = qu.front();
            qu.pop();
            int ne = -1;
            if (x != root) {
                for (int i = 0; i < N; i++) {
                    if (x->p->ch[i] == x) {
                        ne = i;
                        break;
                    }
                }
            }

            if (x->p == root || x == root) {
                x->suff_link = root;
            } else {
                ac_node* fe = x->p->suff_link;
                while (fe != root && fe->ch[ne] == 0) {
                    fe = fe->suff_link;
                }

                if (fe->ch[ne]) {
                    x->suff_link = fe->ch[ne];
                } else {
                    x->suff_link = root;
                }
            }

            ac_node* t = x->suff_link;
            while (t != root && t->num == 0)
                t = t->dict_link;

            x->dict_link = t;
            x->cnt = x->num + x->suff_link->cnt;
            for (int i = 0; i < N; i++) {
                if (x->ch[i]) {
                    qu.push(x->ch[i]);
                }
            }
        }
    }

    int query1(const std::vector<int>& x, ac_node* cur, int pos = 0) {
        if (pos == x.size())
            return cur->cnt;

        int f = cur->cnt;
        while (cur != root && cur->ch[x[pos]] == 0)
            cur = cur->suff_link;

        if (cur->ch[x[pos]] == 0) {
            return f + query1(x, cur, pos + 1);
        } else {
            return f + query1(x, cur->ch[x[pos]], pos + 1);
        }
    }

    void bfs() {
        std::queue<ac_node*> x;
        x.push(root);
        while (!x.empty()) {
            ac_node* to = x.front();
            x.pop();
            top.push_back(to);
            for (int i = 0; i < N; i++) {
                if (to->ch[i] != nullptr) {
                    x.push(to->ch[i]);
                }
            }
        }
    }

public:
    aho_corasick(const std::vector<std::vector<int>>& s) {
        root = new ac_node;
        for (int i = 0; i < s.size(); i++)
            insert(s[i], i, root);

        bfs();
        make_suff_links();
    }

    int count(const std::vector<int>& x) {
        return query1(x, root);
    }

    ~aho_corasick() {
        delete root; // recursive here
    }
};

#endif
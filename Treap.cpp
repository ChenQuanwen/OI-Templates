#include <cstdio>
#include <cstdlib>

struct Node {
    Node *ch[2];
    int r, v, s, flag;
    Node(int v):v(v) { ch[0] = ch[1] = NULL; r = rand(); s = 1; flag = 1; }
    bool operator < (const Node& rhs) const {
        return r < rhs.r;
    }
    int cmp(int x) const {
        if(x == v) return -1;
        return x < v ? 0 : 1;
    }
    void maintain() {
        s=flag;
        if(ch[0] != NULL) s += ch[0]->s;
        if(ch[1] != NULL) s += ch[1]->s;
    }
};

Node* root;

void rotate(Node* &o, int d) {
    Node* k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
    o->maintain(); k->maintain(); o = k;
}

void insert(Node* &o, int x) {
    if(o == NULL) o = new Node(x);
    else {
        if(o->v == x) { o->flag++; o->maintain(); return; }
        int d = (x < o->v ? 0 : 1);
        insert(o->ch[d], x);
        if(o->ch[d]->r > o->r) rotate(o, d^1);
    }
	o->maintain();
}

void remove(Node* &o, int x) {
	if(o == NULL) return;
    int d = o->cmp(x);
	if(d == -1) {
		if(o->flag > 1) o->flag--;
		else {
			Node* u = o;
			if(o->ch[0] && o->ch[1]) {
				int d2 = (o->ch[0]->r < o->ch[1]->r) ? 0 : 1;
				rotate(o, d2); remove(o->ch[d2], x);
			}
			else {
				if(o->ch[0] == NULL) o = o->ch[1];
				else o = o->ch[0];
				delete u;
			}
		}
	}
	else remove(o->ch[d], x);
	if(o != NULL) o->maintain();
}

int kth(Node* o, int k) {
    if(o == NULL || k <= 0 || k > o->s) return 0;
    int s = (o->ch[0] != NULL ? o->ch[0]->s : 0);
    if(k >= s + 1 && k <= o->flag + s) return o->v;
    else if(k <= s) return kth(o->ch[0], k);
    else return kth(o->ch[1], k - s - o->flag);
}

int rank(Node* o, int k) {
    if(o == NULL) return 0;
    int s = (o->ch[0] != NULL ? o->ch[0]->s : 0);
    if(o->v == k) return s + 1;
    if(o->v > k) return rank(o->ch[0], k);
    else return s + o->flag + rank(o->ch[1], k);
}

void pred(Node* o, int k, int &ans) {
    if(o == NULL) return;
    if(o->v < k) {
        if(o->v > ans) ans = o->v;
        if(o->ch[1] != NULL) pred(o->ch[1], k, ans);
    } else if(o->v >= k) {
        if(o->ch[0] != NULL) pred(o->ch[0], k, ans);
    }
}

void succ(Node* o, int k, int &ans) {
    if(o == NULL) return;
    if(o->v > k){
        if(o->v < ans) ans = o->v;
        if(o->ch[0] != NULL) succ(o->ch[0], k, ans);
    } else if(o->v <= k) {
        if(o->ch[1] != NULL) succ(o->ch[1], k, ans);
    }
}

int main(){
    int T;
    scanf("%d", &T);
    while(T--){
        int a, b;
        scanf("%d%d", &a, &b);
        if(a == 1) insert(root, b);
        else if(a == 2) remove(root, b);
        else if(a == 3) { int ans3 = rank(root, b); printf("%d\n", ans3); }
        else if(a == 4) { int ans4 = kth(root, b); printf("%d\n", ans4); }
        else if(a == 5) { int ans5 = 0; pred(root, b, ans5); printf("%d\n", ans5); }
        else if(a == 6) { int ans6 = 2147483647; succ(root, b, ans6); printf("%d\n", ans6); }
    }
    return 0;
}

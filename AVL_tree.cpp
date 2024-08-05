#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef struct Node {
    int key,h;
    struct Node*lchild,*rchild;
}Node;
#define L(n) (n->lchild)
#define R(n) (n->rchild)
#define H(n) (n->h)
#define KEY(n) (n == NIF?-1: n->key)
#define NIF (&__NIF)
Node __NIF{-1,0,NIF,NIF};
Node*get_new_node(int key) {
    Node * n = (Node *)malloc(sizeof(Node));
    n->key = key;n->h = 1;
    n->lchild = n->rchild = NIF;
    return n;
}

void update_h(Node*root) {
    if(root == NIF)return ;
    H(root) = H(L(root)) > H(R(root)) ? H(L(root)) + 1 : H(R(root)) + 1;
    return ;
}

Node*right_rotate(Node*root) {
    Node*temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
    update_h(root);
    update_h(temp);
    return temp;
}

Node*left_rotate(Node*root) {
    Node*temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
    update_h(root);
    update_h(temp);
    return temp;
}

Node*maintain(Node*root) {
    if(root == NIF)return root;
    if(abs(H(R(root)) - H(L(root))) <= 1)return root;
    if(H(R(root)) > H(L(root))) {
        if(H(L(R(root))) > H(R(R(root)))) {
            R(root) = right_rotate(R(root));
        }
        root = left_rotate(root);
    }else {
        if(H(L(L(root))) < H(R(L(root)))) {
            L(root) = left_rotate(L(root));
        }
        root = right_rotate(root);
    }
    return root;
}

Node * insert_avl_tree(Node*root,int key) {
    if(root == NIF)return get_new_node(key);
    if(root->key == key)return root;
    if(root->key < key) {
        root->rchild = insert_avl_tree(root->rchild,key);
    }else {
        root->lchild = insert_avl_tree(root->lchild,key);
    }
    update_h(root);
    return maintain(root);
}

void clear(Node*root) {
    if(root == NIF)return ;
    clear(root->lchild);
    clear(root->rchild);
    free(root);
}

void output(Node*root) {
    if(root == NIF)return;
    printf("%d(%d)<---%d(%d)----->%d(%d)\n",
        KEY(root->lchild),H(root->lchild),
        KEY(root),H(root),
        KEY(root->rchild),H(root->rchild)
    );
    output(root->lchild);
    output(root->rchild);
}

void bfs(Node*root) {
    if(root == NIF)return ;
#define OP 100
    Node*p[OP];int k = 0;
    p[k++] = root;
    int head = 0,tail = 1;
    while(head < tail || abs(tail - head)) {
        int t =abs(tail - head);
        for(int i = 0;i < t;i ++){
            printf("%d(%d)---",p[head]->key,p[head]->h);
            if(p[head]->lchild != NIF){
                p[tail++] = p[head]->lchild;
                tail =tail % OP;
            }
            if(p[head]->rchild != NIF){
                p[tail++] = p[head]->rchild;
                tail =tail % OP;
            }
            head++;
            head =head % OP;
        }printf("\n");
    }printf("\n-----------------------\n");

}

Node*pre_node(Node*root) {
    Node*t = root->lchild;
    while(t->rchild !=NIF)t = t->rchild;
    return t;
}

Node*erase(Node*root,int key) {
    if(root == NIF)return root;
    if(root->key < key)root->rchild = erase(root->rchild,key);
    else if(root->key > key)root->lchild = erase(root->lchild,key);
    else {
        if(root->lchild == NIF || root->rchild == NIF) {
            Node*t = (root->lchild == NIF?root->rchild:root->lchild);
            free(root);
            return t;
        }else {
            Node*t = pre_node(root);
            root->key = t->key;root->h = t->h;
            root->lchild = erase(root->lchild,t->key);
        }
    }
    update_h(root);
    return maintain(root);
}

Node*find(Node*root,int k) {
    if(root == NIF)return root;
    if(k > root->key)return find(root->rchild,k);
    if(k < root->key)return find(root->lchild,k);
    return root;
}
int main() {
    Node*root=NIF;int x;
    printf("add  numbers:\n");
    while(scanf("%d",&x) == 1) {
        if(x == -1)break;
        root = insert_avl_tree(root,x);
    }
    output(root);
    bfs(root);
    printf("find numbers:\n");
    while(scanf("%d",&x) == 1) {
        if(x == -1)break;
        Node* t = find(root,x);
        if(t == NIF) {
            printf("no such number!\n");
        }else {
            printf("the number->%d is %d high\n",t->key,t->h);
        }
    }
    printf("delete numbers:\n");
    while(scanf("%d",&x) == 1) {
        if(x == -1)break;
        root =erase(root,x);
        if(root == NIF) {
            printf("NULL ACL tree,now\n");break;
        }
        bfs(root);
    }
    clear(root);
    return 0;
}
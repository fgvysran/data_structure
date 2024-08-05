//
// Created by longbin on 24-7-24.
//
#include<cstdio>
#include<cstdlib>
#include<time.h>
#define KEY(s) s==nullptr ? -1 :s->key
typedef struct Node {
    int key;
    struct Node *lchild,*rchild;
}Node;

Node*get_new_node(int key) {
    Node*n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->lchild = n->rchild = nullptr;
    return n;
}

Node*insert(Node*root,int key) {
    if(root == nullptr)return get_new_node(key);
    if(root->key == key)return root;
    if(root->key < key)root->rchild = insert(root->rchild,key);
    else root->lchild = insert(root->lchild,key);
    return root;
}

Node*pre_node(Node*root) {
    Node*t = root->lchild;
    while(t->rchild != nullptr)t=t->rchild;
    return t;
}

Node*erase(Node*root,int key) {
    if(root == nullptr)return root;
    if(root->key > key)root->lchild = erase(root->lchild,key);
    else if(root->key < key)root->rchild = erase(root->rchild,key);
    else {
        if(root->lchild ==nullptr && root->rchild == nullptr) {
            free(root);
            return nullptr;
        }else if(root->lchild == nullptr || root->rchild == nullptr) {
            Node*te = root->lchild ? root->lchild : root->rchild;
            free(root);
            return te;
        }else {
            Node * te = pre_node(root);
            root->key = te->key;
            root->lchild = erase(root->lchild,te->key);
            return root;
        }
    }return root;
}
void clear(Node * root) {
    if(root == nullptr)return ;
    clear(root->lchild);
    clear(root->rchild);
    free(root);
}

void output(Node*root) {
    if(root == nullptr)return ;
    printf("%d<--L--%d--R-->%d\n",
        KEY(root->lchild),KEY(root),KEY(root->rchild)
    );
    output(root->lchild);
    output(root->rchild);
}

void inorder(Node*root) {
    if(root == nullptr)return;
    inorder(root -> lchild);
    printf("%d--",root->key);
    inorder(root -> rchild);
}


int main(){
    srand(time(nullptr));
    int  MAX_N = 10;
    Node*root = nullptr;
    for(int i = 0;i < MAX_N;i ++) {
        int key = rand()%100;
        printf("insert key to %d to tree\n",key);
        root = insert(root,key);
    }int x;
    output(root);
    printf("in order");
    inorder(root);printf("\n\n");
    while(~scanf("%d",&x)) {
        root = erase(root,x);
        inorder(root);printf("|\n");
    }
    return 0;
}

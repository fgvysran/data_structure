//
// Created by longbin on 24-7-26.
//
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
typedef struct Node {
    int key,color;
    struct Node*lchild,*rchild;
}Node;
#define NIL (&__NIL)
Node __NIL = {-1,1,NIL,NIL};
#define RED 0
#define BLACK 1
#define DOUBLEBLACK 2
#define C(n) (n->color)
#define L(n) (n->lchild)
#define R(n) (n->rchild)
#define K(n) (n == NIL ? -1 :n->key)
Node*get_new_node(int key) {
    Node*n = (Node*)malloc(sizeof(Node));
    n->color = RED;n->key = key;
    n->rchild = n->lchild = NIL;
    return n;
}

int has_red_node(Node*root) {
    if(root == NIL)return 0;
    return C(L(root)) == RED || C(R(root)) == RED;
}

Node*left_rotate(Node*root) {
    Node*t = R(root);
    R(root) = L(t);
    L(t) = root;
    return t;
}

Node*right_rotate(Node*root) {
    Node*t = L(root);
    L(root) = R(t);
    R(t) = root;
    return t;
}
Node*insert_maintain(Node*root) {
    int f = 0;
    if(C(L(root)) == RED && has_red_node(L(root)))f = 1;
    if(C(R(root)) == RED && has_red_node(R(root)))f = 2;
    if(f == 0)return root;
    if(f == 1) {
        if(C(R(root)) == RED)goto red_up;
        if(C(R(L(root))) == RED) {
            L(root) = left_rotate(L(root));
        }
        root = right_rotate(root);
    }
    if(f == 2) {
        if(C(L(root)) == RED)goto red_up;
        if(C(L(R(root))) == RED) {
            R(root) = right_rotate(R(root));
        }
        root = left_rotate(root);
    }
red_up:
    C(root) = RED;
    C(L(root)) = C(R(root)) =BLACK;
    return root;
}
Node*__insert(Node*root,int key) {
    if(root == NIL)return get_new_node(key);
    if(key > root->key)R(root) = __insert(R(root),key);
    else if(key < root->key)L(root) = __insert(L(root),key);
    else return root;
    return insert_maintain(root);
}
Node*insert(Node*root,int key) {
    root = __insert(root,key);
    root->color = BLACK;
    return root;
}

Node*pre_node(Node*root) {
    if(root == NIL)return NIL;
    Node*t = L(root);
    while(R(t) != NIL)t=R(t);
    return t;
}

Node*erase_maintain(Node*root) {
    if(C(L(root)) != DOUBLEBLACK && C(R(root)) != DOUBLEBLACK)return root;
    if(has_red_node(root)) {
        C(root) = RED;
        if(C(R(root)) == RED) {
            root = left_rotate(root);
            L(root) = erase_maintain(L(root));
        }else {
            root = right_rotate(root);
            R(root) = erase_maintain(R(root));
        }
        C(root) = BLACK;
        return root;
    }
    if((C(L(root)) == DOUBLEBLACK && !has_red_node(root))
      ||  (C(R(root)) == DOUBLEBLACK && !has_red_node(root))
    ) {
        C(root)+=1;
        C(R(root)) -= 1;C(L(root)) -=1;
        return root;
    }
    if(C(L(root)) == DOUBLEBLACK) {
        C(L(root)) = BLACK;
        if(C(R(R(root))) !=RED) {
            R(root) = right_rotate(R(root));
        }
        C(R(root)) = C(root);
        root = left_rotate(root);
    }else {
        if(C(L(L(root))) != RED) {
            L(root) = left_rotate(L(root));
        }
        C(L(root)) = C(root);
        root = right_rotate(root);
    }
    C(L(root)) = C(R(root)) =BLACK;
    return root;
}

Node*__erase(Node*root,int key) {
    if(root == NIL)return  root;
    if(key > root->key)R(root) = __erase(R(root),key);
    else if(key < root->key)L(root) = __erase(L(root),key);
    else {
        if(L(root) == NIL || R(root) == NIL) {
            Node*t = L(root) == NIL? R(root) : L(root);
            C(t) += C(root);
            free(root);
            return t;
        }
        Node*t = pre_node(root);
        root->key = t->key;
        L(root) = __erase(L(root),t->key);
    }
    return erase_maintain(root);
}
Node*erase(Node*root,int key) {
    root = __erase(root,key);
    C(root) = BLACK;
    return root;
}
void output(Node*root) {
    if(root == NIL)return;
    printf("%d(%d)<---%d(%d)--->%d(%d)\n",
       K(L(root)),C(L(root)),K(root),C(root),K(R(root)),C(R(root))
    );
    output(L(root));
    output(R(root));
}
void bfs(Node*root) {
    if(root == NIL)return;
#define MAX 100
    Node*p[MAX];
    int head = 0,tail = 0;
    p[tail++] = root;
    while(abs(tail - head) !=0) {
        int s = abs(tail - head);
        for(int i = 0;i < s;i++) {
            printf("%d[%d]---",p[head]->key,p[head]->color);
            if(p[head]->lchild != NIL)p[tail++] = p[head]->lchild;
            if(p[head]->rchild != NIL)p[tail++] = p[head]->rchild;
            head++;
        }printf("\n");
    }
#undef MAX
    return;
}
int main() {
    Node*root = NIL;int x;
    while(~scanf("%d",&x)) {
        if(x == -1)break;
        root = insert(root,x);
        output(root);
        bfs(root);
    }
    while(~scanf("%d",&x)) {
        if(x == -1)break;
        root = erase(root,x);
        output(root);
        bfs(root);
    }
    return 0;
}

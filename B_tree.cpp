//
// Created by longbin on 24-7-30.
//
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#define MAX_M 5
#define swap(a,b){\
    __typeof(a) __c = a;\
    a = b;b = __c;\
}
#define LCHILD(root,pos) (root->next[pos])
#define RCHILD(root,pos) (root->next[pos + 1])

typedef struct Node{
    int key[MAX_M + 1],n;
    struct Node*next[MAX_M + 1];
}Node;

Node* get_new_node(){
    Node*p = (Node*)malloc(sizeof(Node));
    p->n = 0;
    memset(p->next,0,sizeof(Node*)*(MAX_M + 1));
    return p;
}

Node* insert_key(Node*root,int key){
    if(root == NULL){
        Node* p = get_new_node();
        p->key[0] = key;
        p->n = 1;
        return p;
    }
    int pos = 0;
    while(pos < root->n && root->key[pos] < key)pos += 1;
    if(pos < root->n && root->key[pos] == key)return root;
    for(int i = root->n ;i > pos;i --){
        root->key[i] = root->key[i - 1];
    }
    root->key[pos] = key;
    root->n += 1;
    return root;
}

Node*insert_maintain(Node*root,Node*child,int pos){
    if(child->n < MAX_M)return root;
    int spos = child->n/2;
    Node* p =get_new_node();
    Node* q =get_new_node();
    p->n = spos;
    q->n = MAX_M - 1 - p->n;
    for(int i = 0;i < p->n;i +=1){
        p->key[i] = child->key[i];
        p->next[i] = child->next[i];
    }
    p->next[p->n] = child->next[p->n];
    for(int i = 0;i < q->n;i +=1){
        q->key[i] = child->key[spos + 1 + i];
        q->next[i] = child->next[spos + 1 + i];
    }
    q->next[q->n] = child->next[child->n];
    for(int i = root->n + 1;i > pos;i --){
        root->key[i] = root->key[i - 1];
        root->next[i] = root->next[i-1];
    }
    root->key[pos] = child->key[spos];
    root->n +=1;
    root->next[pos] = p;
    root->next[pos + 1] = q;
    free(child);
    return root;
}

Node*__insert(Node*root,int key){
    if(root == NULL || root->next[0] == NULL){
        root = insert_key(root,key);
        return root;
    }
    int pos = 0;
    while(pos < root->n && root->key[pos] < key)pos += 1;
    if(pos < root->n && root->key[pos] == key)return root;
    root->next[pos] = __insert(root->next[pos],key);
    return insert_maintain(root,root->next[pos],pos);
}
Node*insert(Node*root,int key){
    root = __insert(root,key);
    if(root->n >= MAX_M){
        Node*p = get_new_node();
        root = insert_maintain(p,root,0);
    }
    return root;
}
void clear(Node*root){
    if(root == NULL)return;
    for(int i = 0;i <= root->n;i ++){
        clear(root->next[i]);
    }
    free(root);
    return ;
}

void erase_pos(Node*root,int pos){
    if(root == NULL)return ;
    for(int i = pos;i < root->n;i ++){
        root->key[i] = root->key[i + 1];
    }
    root->n -= 1;
    return;
}//终端节点删除

Node *left_rotate(Node*root,int pos){
    root->next[pos]->key[root->next[pos]->n] = root->key[pos];
    root->key[pos] = root->next[pos + 1]->key[0];
    root->next[pos]->n += 1;
    root->next[pos]->next[root->next[pos]->n] = root->next[pos + 1]->next[0];
    for(int i = 0;i < root->next[pos + 1]->n ;i ++){
        root->next[pos + 1]->key[i] = root->next[pos + 1]->key[i + 1];
        root->next[pos + 1]->next[i] = root->next[pos + 1]->next[i + 1];
    }
    root->next[pos + 1]->next[root->next[pos + 1]->n] = NULL;
    root->next[pos + 1]->n -= 1;
    return root;
}

Node*right_rotate(Node*root,int pos){
    for(int i = root->next[pos + 1]->n + 1;i > 0;i --){
        root->next[pos + 1]->key[i] = root->next[pos + 1]->key[i - 1];
        root->next[pos + 1]->next[i] =root->next[pos + 1]->next[i - 1];
    }
    root->next[pos + 1]->key[0] = root->key[pos];
    root->key[pos] = root->next[pos]->key[root->next[pos]->n - 1];
    root->next[pos + 1]->next[0] = root->next[pos]->next[root->next[pos]->n];
    root->next[pos]->next[root->next[pos]->n] = NULL;
    root->next[pos]->key[root->next[pos]->n - 1] = 0;
    root->next[pos]->n -= 1;
    root->next[pos + 1]->n += 1;
    return root;
}

Node*merge(Node*root,int pos){
    Node* n =get_new_node();
    for(int i = 0;i <= root->next[pos]->n;i ++){
        n->key[i] = root->next[pos]->key[i];
        n->next[i] = root->next[pos]->next[i];
    }
    n->n = root->next[pos]->n + 1;
    n->key[n->n - 1] = root->key[pos];
    for(int i = 0;i <= root->next[pos + 1]->n;i ++){
        n->key[i + n->n] = root->next[pos + 1]->key[i];
        n->next[i + n->n] = root->next[pos + 1]->next[i];
    }
    n->n += root->next[pos + 1]->n;
    free(root->next[pos]);free(root->next[pos + 1]);

    for(int i = pos + 1;i <=root->n;i ++){
        root->key[i - 1] =root->key[i];
        root->next[i - 1] =root->next[i];
    }
    root->next[root->n] = NULL;
    root->next[pos] = n;
    root->n -= 1;
    return root;
}
Node*erase_maintain(Node*root,int pos){
    int low = (MAX_M + 1)/2 - 1;
    if(root->next[pos]->n >= low)return root;
    if(pos > 0 && root->next[pos - 1]->n > low){
        root = right_rotate(root,pos - 1);
    }else if(pos < root->n && root->next[pos + 1]->n > low){
        root = left_rotate(root,pos);
    }else{
        if(pos > 0)root = merge(root,pos - 1);
        else root = merge(root,pos);  //+0 -> +1
    }
    return root;
}
Node*__erase(Node*root,int key){
    if(root == NULL)return root;
    int pos = 0;
    while(pos < root->n && root->key[pos] < key)pos ++ ;
    if(root->next[0] == NULL){
        if(root->key[pos] == key)erase_pos(root,pos);
        return root;
    }else{
        if(pos < root->n && root->key[pos] == key){
            Node*t = root->next[pos];
            while(t->next[t->n])t = t->next[t->n];
            swap(t->key[t->n - 1],root->key[pos]);
        }
        root->next[pos] = __erase(root->next[pos],key);
    }
    return erase_maintain(root,pos);
}
Node*erase(Node*root,int key){
    root = __erase(root,key);
    if(root->n == 0){
        Node*t = root->next[0];
        free(root);
        root = t;
    }
    return root;
}

void output(Node*root){
    if(root == NULL)return ;
    for(int i = 0;i < root->n;i ++){
        printf("%5d",root->key[i]);
    }printf("\n");
    for(int i = 0;i <= root->n;i ++){
        output(root->next[i]);
    }
    return ;
}
int main(){
    srand(time(0));
    Node*root = NULL;
    for(int i = 0;i < 15;i++){
        int x = rand()%100;
        printf("insert %d:\n",x);
        root = insert(root,x);
        output(root);
    }
    int x;
    while(printf("the number to erase :") && scanf("%d",&x)){
        if(x == -1)break;
        root = erase(root,x);
        output(root);
    }
    return 0;
}
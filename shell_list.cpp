#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
#define swap(a,b){\
  __typeof(a) __c=a;\
  a=b;\
  b=__c;\
}

typedef struct Node{
    char*s;
    struct Node*next;
}Node;

typedef struct HashTable{
    Node*data;
    int cnt,size;
}HashTable;

Node*getNewNode(const char*x){
    Node*p=(Node*)malloc(sizeof(Node));
    p->s=strdup(x);
    p->next=NULL;
    return p;
}

HashTable *getnewhash(int n){
    HashTable*h=(HashTable*)malloc(sizeof(HashTable));
    h->data=(Node*)malloc(sizeof(Node)*n);
    for(int i=0;i<n;i++){
        h->data[i].next=NULL;
    }
    h->size=n;
    h->cnt=0;
    return h;
}
void expand(HashTable*h);
void swap_hashtable(HashTable* ne,HashTable*h);
int hash_func(const char*s){
    int seed=131,h=0;
    for(int i= 0;s[i];i++){
        h=h*seed+s[i];
    }
    return  h&0x7fffffff;
}

bool insert(HashTable*h,const char*s){
    if(h->cnt >= h->size*2){
        expand(h);
    }
    int hcode=hash_func(s);int ind =hcode %h->size;
    Node*p =getNewNode(s);
    p->next=h->data[ind].next;
    h->data[ind].next=p;
    h->cnt++;
    return true;
}
void clearnode(Node*p){
    if(p==NULL)return;
    if(p->s)free(p->s);
    free(p);return ;
}

void clearHashTable(HashTable*s){
    if(s==NULL)return;
    for(int i=0;i<s->size;i++){
        Node*p=s->data[i].next,*q;
        while(p){
            q=p->next;
            clearnode(p);
            p=q;
        }
    }
}


void swap_hashtable(HashTable* ne,HashTable*h){
    swap(ne->data,h->data);
    swap(ne->cnt,h->cnt);
    swap(ne->size,h->size);
    return;
}
 
void expand(HashTable*h){
    printf("\nenpand to %d\n",h->size*2);
    HashTable*ne=getnewhash(h->size*2);
    for(int i=0;i<h->size;i++){
        Node*p=h->data[i].next;
        while(p){
            insert(ne,p->s);
            p=p->next;
        }
    }
    swap_hashtable(ne,h);
    clearHashTable(ne);
    return ;
}




bool find(HashTable*h,const char*s){
    int hcode=hash_func(s);int ind =hcode %h->size;
    Node*p=h->data[ind].next;
    while(p){
        if(strcmp(p->s,s)==0)return true;
        p=p->next;
    }
    return false;
}
void   output(HashTable*h){
    printf("\n\nhash table :%d/%d\n",h->cnt,h->size);
    for(int i=0;i<h->size;i++){
        printf("%d :",i);
        Node*p=h->data[i].next;
        while(p){
            printf("%s ->",p->s);
            p=p->next;
        }
        printf("\n");
    }
}

int main(){
    srand(time(0));
    char s[100];
    #define MAX_N 2
    HashTable*h=getnewhash(MAX_N);
    while(~scanf("%s",s)){
        if(strcmp(s,"end")==0)break;
        insert(h,s);
    }
    output(h);
    while(~scanf("%s",s)){
        printf("search :\n");
        printf("find(%s) = %d\n",s,find(h,s));
    }
    #undef MAX_N
    return 0;   
}
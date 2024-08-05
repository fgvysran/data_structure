//
// Created by longbin on 24-7-24.
//
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define MIN -0x7fffffff
#define MAX  0x7fffffff

typedef struct Node{
    int val;
    struct Node*next,*up,*down;
}Node;

typedef struct Skip_list{
    Node*head;
    int max_leval;
}Skip_list;

Node*getnewnode(int val){
    Node*n=(Node*)malloc(sizeof(Node));
        n->next=n->down=n->up=NULL;
        n->val=val;
    return n;
}

Skip_list*getnewskiplist(int n){
    Skip_list*s=(Skip_list*)malloc(sizeof(Skip_list));
    s->head=getnewnode(MIN);
    s->head->next=getnewnode(MAX);
    s->max_leval=n;
    Node*p=s->head;
    for(int i=1;i < n;i++){
        Node*q=p;
        p->up=getnewnode(MIN);
        p->up->next=getnewnode(MAX);
        p=p->up;
        if(p!=s->head)p->down=q;
    }
    return s;
}

void insert(Skip_list*s,int val){
    int len=rand()%(s->max_leval-1)+1;
    Node*p=s->head;Node*up,*down;
    for(int i=1;i<len;i++)p=p->up;
    for(int i=0;i<len;i++){
        Node*n=getnewnode(val);
        if(i)n->up=up;
        if( i!=0 && p->down)up->down=n;
        Node*q=p->down;
        
        while(p->next->val <= val)p=p->next;

        n->next=p->next;
        p->next=n;
        up=n;  
        p=q;
    }
}

void output(Skip_list*s,Node*x=NULL){
    Node*p=s->head,*q;
    for(int i=0;i<s->max_leval;i++){
        q=p; p=p->next;
        printf("%d :",i);
        while(p->val!=MAX){
            if(p==x)printf("\033[31m-->%d\033[0m",p->val);
            else printf("-->%d",p->val);
            p=p->next;
        }   printf("\n");     
        p=q->up;
    }
}

Node*find(Skip_list*s,int val){
    Node*p=s->head;
    while(p->up!=NULL)p=p->up;
    while(p && p->val!=val){
        if(p->next->val <= val)p=p->next;
        else p=p->down;
    }return p;
}

int main(){
    srand(time(0));
    Skip_list* s=getnewskiplist(12);
    for(int i=0;i<12;i++){
        insert(s,rand()%100);
    }
    output(s);
    int d=1;
    while(d!=0){
        printf("please enter the number you want to search:\n");
        scanf("%d",&d);
        Node*x=find(s,d);
        if(x){
           printf("search suceess!\n");
            output(s,x);
        }else{
            printf("failed !\n");
        }
    }
    return 0;
}
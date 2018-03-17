//to generate binary tree with recursion
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<stdlib.h>
int shmid;
struct msg
{
int pid1;
int pid2;
int pid3;
char sent;
int receive;
};
struct msg* m;
struct node
{
char val;
struct node* left;
struct node* right;
};

struct node* root;
void create_tree(struct node* r)
{
char ch;
printf("enter the left child of %d\n",r->val);
scanf("%c",&ch);
if(ch>=65 && ch<=91)
{
struct node* m=(struct node*)malloc(sizeof(struct node));
m->val=ch;
m->left=NULL;
m->right=NULL;
r->left=m;
create_tree(m);
}
printf("enter the right child of %d\n",r->val);
scanf("%c",&ch);
if(ch>=65 && ch<=91)
{
struct node* m=(struct node*)malloc(sizeof(struct node));
m->val=ch;
m->left=NULL;
m->right=NULL;
r->right=m;
create_tree(m);
}
}
int flag;
void search_tree(struct node* t,char ch)
{
if(t!=NULL)
{
search_tree(t->left,ch);
if(t->val==ch)
flag=1;
search_tree(t->right,ch);
}
}


int search(char ch)
{
flag=0;
struct node* t=root;
search_tree(t,ch);
return flag;
}


void sig_handle()
{
if(search(m->sent)==1)
m->receive=1;
else
m->receive=0;
m->sent='0';
signal(SIGUSR1,sig_handle);
kill(m->pid1,SIGUSR1);
}


int main()
{
root=(struct node*)malloc(sizeof(struct node));
printf("enter the root value\n");
scanf("%c",&root->val);
create_tree(root);
shmid=shmget(7654,sizeof(struct msg),IPC_CREAT|666);
m=(struct msg*)shmat(shmid,NULL,0);
m->pid2=getpid();
signal(SIGUSR1,sig_handle);
printf("ready for question\n");
while(1);
return 0;
}




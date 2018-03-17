//to generate binary tree without recursion
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

struct stack
{
struct node* size[500];
int top;
};

struct node* root;
struct stack st;

void push(struct node* r)
{
st.top++;
st.size[st.top]=r;
}

void pop()
{
st.top--;
}

struct node* top()
{
return st.size[st.top];
}

void create_tree(struct node* t)
{
char ch;
while(st.top!=-1)
{
printf("enter the left child of %d\n",t->val);
scanf("%c",&ch);
if(ch>=65 && ch<=91)
{
struct node* r=(struct node*)malloc(sizeof(struct node));
r->val=ch;
r->left=NULL;
r->right=NULL;
t->left=r;
push(r);
t=r;
}
else
{
beg:
pop();
printf("enter the right child of %d\n",t->val);
scanf("%c",&ch);
if(ch>=65 && ch<=91)
{
struct node* r=(struct node*)malloc(sizeof(struct node));
r->val=ch;
r->left=NULL;
r->right=NULL;
t->right=r;
push(r);
t=r;
}
else
{
if(st.top==-1)
break;
t=top();
goto beg;
}
}
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
st.top=-1;
root=(struct node*)malloc(sizeof(struct node));
printf("enter the root value\n");
scanf("%c",&root->val);
push(root);
create_tree(root);
shmid=shmget(7654,sizeof(struct msg),IPC_CREAT|666);
m=(struct msg*)shmat(shmid,NULL,0);
m->pid3=getpid();
signal(SIGUSR1,sig_handle);
printf("ready for question\n");
while(1);
return 0;
}

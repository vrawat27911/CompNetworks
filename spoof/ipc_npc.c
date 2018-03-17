
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#define LENGTH 1096
 int main()
{
struct sockaddr_in s;
char buff[LENGTH];
memset(buff,0,LENGTH);
int sfd;
sfd=socket(AF_INET,SOCK_RAW,52);
struct iphdr *iph=(struct iphdr *)buff;

s.sin_port=0;
s.sin_family=AF_INET;
inet_pton(AF_INET,"192.20.0.5",(struct sockaddr *)&s.sin_addr);
iph->ihl=5;
iph->version=4;
iph->tos=6;
iph->tot_len=htons(40);
iph->frag_off=0;
iph->ttl=64;
iph->protocol=52;
iph->check=12;
iph->saddr=s.sin_addr.s_addr;
iph->daddr=s.sin_addr.s_addr;

char msg[100];
while(1)
{
memset(msg,0,1024);
printf("enter message\n");
scanf("%s",msg);
int one =1;
int *val=&one;
setsockopt(sfd,IPPROTO_IP,IP_HDRINCL,val,sizeof(one));
int total=sizeof(struct iphdr);
int i=0;
while(i<sizeof(msg))
{
buff[total++]=msg[i++];
}
buff[total]='\0';

if((sendto(sfd,(char *)buff,sizeof(buff),0,(struct sockaddr *)&s,sizeof(s)))<0)
perror("send");
}
return 0;
}


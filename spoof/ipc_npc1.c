
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

int main()
{
struct sockaddr_in s;
char pack[1000];
memset(pack,sizeof(pack),0);
int sfd;
sfd=socket(AF_INET,SOCK_RAW,52);
struct iphdr *iph=(struct iphdr *)pack;
s.sin_port=0;
s.sin_family=AF_INET;
inet_pton(AF_INET,"192.20.0.5",(struct sockaddr*)&s.sin_addr);

while(1)
{
memset(pack,0,1000);
if(recvfrom(sfd,(char *)&pack,sizeof(pack),0,(struct sockaddr*)&s,sizeof(s))<0)
perror("recv");

struct iphdr *ip = (struct iphdr *)pack;
printf("ip version =%d \n",ip->version);
printf("ip header length=%d ",ip->ihl);
printf("ip type of service=%d ",ip->tos);
printf("ip packet total length=%d",ip->tot_len);
printf("ip fragment offset= %d",ip->frag_off);
printf("ip time to leave= %d ",ip->ttl);
printf("ip protocol = %d",ip->protocol);
printf("ip check sum= %d",ip->check);
printf("ip source address= %d",ip->saddr);
printf("ip destination address=%d",ip->daddr);
int total;
total=sizeof(struct iphdr);
while(total<sizeof(pack))
{
printf("%c",pack[total]);
total++;
}

}
return 0;
}

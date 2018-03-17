#include "libsock"
#include<netinet/ip.h>
#include<netinet/udp.h>

int main()
{
int sfd=socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
if(sfd<0)
{
printf("socket error\n");
exit(0);
}
char buf[4096]={0};

while(1)
{
struct ip* ipdr=buf;
struct sockaddr_in cliaddr;
socklen_t len;
if(recvfrom(sfd,buf,4096,0,SA &cliaddr,&len)<0)
{
printf("recv error\n");
exit(0);
}
printf("IP header len:%d\n",ipdr->ip_hl*4);
printf("IP version is:%d\n",ipdr->ip_v);
printf("IP service:%d\n",ipdr->ip_tos);
printf("IP len:%d\n",ipdr->ip_len);
printf("IP sequence id:%d\n",ipdr->ip_id);
printf("IP frag offset:%d\n",ipdr->ip_off);
printf("IP TTL:%d\n",ipdr->ip_ttl);
printf("Higher level protocol:%u\n",ipdr->ip_p);
printf("CHeckSum:%x\n",ipdr->ip_sum);
printf("SRC addr:%s\n",inet_ntoa(ipdr->ip_src));
printf("DST addr:%s\n",inet_ntoa(ipdr->ip_dst));
printf("\n");
/*
struct udphdr* ud=buf+20;
printf("udp sport:%d\n",ud->source);
printf("udp dport:%d\n",ud->dest);
printf("udp len:%d\n",ud->len);
printf("udp check:%x\n",ud->check);
*/
}
return 0;
}

//#include "libsock"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#include<pthread.h>
int sfd;

unsigned short csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

void* sender()
{
char buf[4096];
struct ip* ip_hdr=(struct ip*)buf;
char st[]="hi egegsrgh";
char buff[512];
ip_hdr->ip_hl=5;
ip_hdr->ip_v=4;
ip_hdr->ip_tos=0;
ip_hdr->ip_len=20+strlen(st)+1;
ip_hdr->ip_id=0;
ip_hdr->ip_off=0;
ip_hdr->ip_ttl=64;
ip_hdr->ip_p=199;
inet_pton(AF_INET,"172.30.105.191",&(ip_hdr->ip_src));
inet_pton(AF_INET,"172.30.105.191",&(ip_hdr->ip_dst));
ip_hdr->ip_sum=csum((unsigned short*)buf,9);
    int one = 1;
    const int *val = &one;
    if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
      printf ("Cannot set HDRINCL!\n");
struct sockaddr_in addr;
addr.sin_family=AF_INET;
addr.sin_port=htons(3500);
inet_pton(AF_INET,"172.30.105.191",&addr.sin_addr);
//while(1)
//{
//fgets(buff,512,stdin);
//memcpy(buf+5*4,buff,512);
int i=0;
for(i=0;st[i]!='\0';i++)
buff[20+i]=st[i];
buf[20+i]='\0';
sendto(sfd,buf,ip_hdr->ip_len,0,(struct sockaddr*) &addr,sizeof(addr));
//}

}

void* receiver()
{
char buf[4096];
//while(1)
//{
recvfrom(sfd,buf,4096,0,NULL,NULL);
printf("%s",buf+sizeof(struct ip));
//}
}

int main()
{
sfd=socket(AF_INET,SOCK_RAW,200);

pthread_t r,s;
pthread_create(&s,NULL,sender,NULL);
pthread_create(&r,NULL,receiver,NULL);
pthread_join(r,NULL);
return 0;
}


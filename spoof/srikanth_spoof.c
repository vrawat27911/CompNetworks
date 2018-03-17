//implementation of IP spoofing
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<pthread.h>
#include<string.h>
#include<fcntl.h>
#include<sys/un.h>
#include<netinet/ip.h>
#include<netinet/udp.h>
#define size 1024


unsigned short csum(unsigned short *buf, int nwords)

    {       //

            unsigned long sum;

            for(sum=0; nwords>0; nwords--)

                    sum += *buf++;

            sum = (sum >> 16) + (sum &0xffff);

            sum += (sum >> 16);

            return (unsigned short)(~sum);

    }


struct ip *iph;
struct udphdr *udph;
int main(int argc,char* argv[])
{
int fd=socket(AF_INET,SOCK_RAW,17);

char buf[size];
iph=(struct ip*)buf;
iph->ip_hl=5;
iph->ip_v=4;
iph->ip_off=0;
iph->ip_tos=0;

iph->ip_id=0;
iph->ip_ttl=255;
iph->ip_p=17;

inet_pton(AF_INET,argv[1],(void*)&iph->ip_src);
inet_pton(AF_INET,argv[3],(void*)&iph->ip_dst);
int a=1;
int *x=&a;
if(setsockopt(fd, IPPROTO_IP, IP_HDRINCL, x, sizeof(a)) < 0)
    {
    perror("setsockopt error");
    exit(-1);
    }

//sendto(fd,buf,iph->ip_len,0,(struct sockaddr*)&s,sizeof(s));

udph=(struct udphdr*)(buf+sizeof(struct ip)); 
udph->source=htons(atoi(argv[2]));
udph->dest=htons(atoi(argv[4]));

udph->check=0;
//csum((unsigned short*)(buf+sizeof(struct ip)),(iph->ip_len-sizeof(struct ip))/2);
while(1)
{
char m[200];
scanf("%s",m);
udph->len=htons(sizeof(struct udphdr)+strlen(m)+1);
iph->ip_len=sizeof(struct ip)+sizeof(struct udphdr)+strlen(m)+1;
iph->ip_sum=csum((unsigned short*)buf,10);
int i;
for(i=0;m[i]!='\0';i++)
buf[sizeof(struct ip)+sizeof(struct udphdr)+i]=m[i];
buf[sizeof(struct ip)+sizeof(struct udphdr)+i]='\0';
struct sockaddr_in s;
s.sin_family=AF_INET;
s.sin_port=htons(atoi(argv[4]));
inet_pton(AF_INET,argv[3],(void*)&s.sin_addr);
//packet ready
printf("options modified\n");
printf("sending\n");
if(sendto(fd,buf,iph->ip_len,0,(struct sockaddr*)&s,sizeof(s))==-1)
perror("sending error\n");
printf("sent\n");
}
return 0;
}




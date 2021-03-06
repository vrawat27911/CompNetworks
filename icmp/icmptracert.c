#include "libsock"
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<sys/time.h>

int recieved=0;
unsigned short
csum (unsigned short *buf, int nwords)
{
  unsigned long sum;
  for (sum = 0; nwords > 0; nwords--)
    sum += *buf++;
  sum = (sum >> 16) + (sum & 0xffff);
  sum += (sum >> 16);
  return ~sum;
}

void handler()
{
if(recieved==0)
{
printf("Destination host unreachable\n");
exit(0);
}
}

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      printf ("need destination for tracert\n");
      exit (0);
    }
  int sfd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP);
  char buf[4096] = { 0 };
  struct ip *ip_hdr = (struct ip *) buf;
  int hop = 0;

  int one = 1;
  const int *val = &one;
  if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    printf ("Cannot set HDRINCL!\n");

  struct sockaddr_in addr;
  addr.sin_port = htons (7);
  addr.sin_family = AF_INET;
  inet_pton (AF_INET, argv[1], &(addr.sin_addr));
struct timeval tv,tv3;
gettimeofday(&tv,NULL);
tv3=tv;

  while (1)
    {
    
      ip_hdr->ip_hl = 5;
      ip_hdr->ip_v = 4;
      ip_hdr->ip_tos = 0;
      ip_hdr->ip_len = 20 + 8 + 64;
      ip_hdr->ip_id = 0;
      ip_hdr->ip_off = 64;
      ip_hdr->ip_ttl = hop;
      ip_hdr->ip_p = IPPROTO_ICMP;
      inet_pton (AF_INET, "172.30.104.59", &(ip_hdr->ip_src));
      inet_pton (AF_INET, argv[1], &(ip_hdr->ip_dst));
      ip_hdr->ip_sum = csum ((unsigned short *) buf, 4);
      

      struct icmphdr *icmphd = (struct icmphdr *) (buf+20);
      icmphd->type =ICMP_ECHO;
      icmphd->code = 0;
      icmphd->checksum = 0;
      icmphd->un.echo.id = 0;
      icmphd->un.echo.sequence = 1;
      
      memset(buf+28,'a',64);
      
      icmphd->checksum = csum ((unsigned short *) (buf+20), 4+32);
      sendto (sfd, buf, 20+8+64, 0, SA & addr, sizeof addr);
      char buff[4096] = { 0 };
      struct sockaddr_in addr2;
       recieved=0;
      signal(SIGALRM,handler);
      alarm(1);
      socklen_t len = sizeof (struct sockaddr_in);
      gettimeofday(&tv3,NULL);
      recvfrom (sfd, buff, 20+8+64, 0, SA & addr2, &len);
      struct icmphdr *icmphd2 = (struct icmphdr *) (buff+20);
      struct timeval tv2;
        recieved=1;
      gettimeofday(&tv2,NULL);
      double d=(double)(tv2.tv_sec-tv.tv_sec)*1000+((double)(tv2.tv_usec-tv.tv_usec))/1000;
      double dd=(double)(tv2.tv_sec-tv3.tv_sec)*1000+((double)(tv2.tv_usec-tv3.tv_usec))/1000;
      tv3=tv2;
      if (icmphd2->type ==11)
	{printf ("hop limit:%d Address:%s Round trip delay:%f Total Time elapsed:%f ms\n\n", hop, inet_ntoa (addr2.sin_addr),dd,d);}
      else if(icmphd2->type==0)
	{
	  printf ("Destination reached! hop limit:%d Address:%s Round trip delay:%fTotal Time elapsed:%f ms \n\n",hop,
		  inet_ntoa (addr2.sin_addr),dd,d);
	  exit (0);
	}
	sleep(1);

      hop++;
    }

  return 0;
}

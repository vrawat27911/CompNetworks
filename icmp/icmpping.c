#include "libsock"
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>

double total=0,max=0,min=10000000;
int totalpackets=0,packetslost=0;
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
printf("\n");
printf("--------------PINGING STATISTICS----------------\n");
printf("AVG:%f MAX:%f MIN:%f TOTAL PACKETS:%d PACKETS LOST:%d SUCCESS PERCENTAGE:%f\n\n",total/(totalpackets-packetslost),max,min,totalpackets,packetslost,((double)(totalpackets-packetslost)/(double)totalpackets)*100);
exit(0);
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

  int one = 1;
  const int *val = &one;
  if (setsockopt (sfd, IPPROTO_IP, IP_HDRINCL, val, sizeof (one)) < 0)
    printf ("Cannot set HDRINCL!\n");
    
  struct sockaddr_in addr;
  struct ip* ip_hdr=(struct ip*)buf;
  addr.sin_port = htons (7);
  addr.sin_family = AF_INET;
  inet_pton (AF_INET, argv[1], &(addr.sin_addr));
 
      ip_hdr->ip_hl = 5;
      ip_hdr->ip_v = 4;
      ip_hdr->ip_tos = 0;
      ip_hdr->ip_len = 20 + 8 + 64;
      ip_hdr->ip_id =0;
      ip_hdr->ip_off = 64;
      ip_hdr->ip_ttl = 64;
      ip_hdr->ip_p = IPPROTO_ICMP;
      inet_pton (AF_INET, "172.30.104.59", &(ip_hdr->ip_src));
      inet_pton (AF_INET, argv[1], &(ip_hdr->ip_dst));
      ip_hdr->ip_sum = csum ((unsigned short *) buf, 4);


      struct icmphdr *icmphd = (struct icmphdr *) (buf+20);
      icmphd->type = ICMP_ECHO;
      icmphd->code = 0;
      icmphd->checksum = 0;
      icmphd->un.echo.id = 0;
      icmphd->un.echo.sequence =1;

      memset(buf+28,'a',64);
      icmphd->checksum = csum ((unsigned short *) (buf+20), 36);
      signal(SIGINT,handler);
 struct timeval tv1,tv2;  
 
      printf("Pinging %s with 64 bytes of data.\n\n",argv[1]);
                struct timeval timeout;      
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (setsockopt (sfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        error("setsockopt failed\n");
 
   
      while(1)
      {
      recieved=0;     
      totalpackets++;
      packetslost++;
      sendto (sfd, buf,20+ 8+64, 0, SA & addr, sizeof addr);
      char buff[4096] = { 0 };
      struct sockaddr_in addr2;
      socklen_t len = sizeof (struct sockaddr_in);
      gettimeofday(&tv1,NULL);
      
      int x=recvfrom (sfd, buff, 20+8+64, 0, SA & addr2, &len);
      gettimeofday(&tv2,NULL);
        double   d=(double)(tv2.tv_sec-tv1.tv_sec)*1000+((double)(tv2.tv_usec-tv1.tv_usec))/1000;
      
      if(x>0){
      struct icmphdr *icmphd2 = (struct icmphdr *) (buff + 20);
	  printf ("Reached destination:%s\tTime elapsed:%f ms\n\n",
      	  inet_ntoa (addr2.sin_addr),d);
      	  total+=d;
      	  if(d>max)
      	  max=d;
      	  if(d<min)
      	  min=d;
      	  packetslost--;
      	  }
      	  else
      	  {
          printf("Packet lost\n\n");   
      	  }
      	  }
  return 0;
}



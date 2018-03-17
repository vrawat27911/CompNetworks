#include "libsock"

int
main ()
{
  int sfd = socket (AF_INET, SOCK_DGRAM, 0);
  if (sfd == -1)
    printf ("sock error\n");
  struct sockaddr_in *myaddr, clientaddr;
  int length = sizeof (struct sockaddr_in);
  char x[512];
  myaddr = malloc (sizeof (struct sockaddr_in));
  myaddr->sin_family = AF_INET;
  myaddr->sin_port = htons (3500);
  myaddr->sin_addr.s_addr = INADDR_ANY;
  int b = bind (sfd, (struct sockaddr *) myaddr, sizeof (struct sockaddr_in));
  if (b < 0)
    exit (0);

  while (1)
    {
      memset (x, 0, 512);
      recvfrom (sfd, x, 512, 0, SA & clientaddr, &length);
      printf ("%s", x);
      printf ("%d\n", htons (clientaddr.sin_port));
      sendto (sfd, x, 512, 0, SA & clientaddr, length);
    }
  return 0;
}

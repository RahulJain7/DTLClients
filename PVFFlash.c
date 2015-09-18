#ifdef WIN32
   #include <winsock.h>
   #include <winsock2.h>
#else
   #include <sys/socket.h>
   #include <netinet/in.h>
#endif
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>

#define BUFFERSIZE 4096

void error(const char*);

void PVFFlash(char *Prop,double P,double VF, int Nc,const char *Comp[],const double x[],double Resultss[])


{ 
 char *ResultStr[Nc];
// double Resultss[3];
  char *token;
  int i = 0;
  int j = 0;
  int k = 0;
  WSADATA wsa;
     
  //  printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
       // printf("Failed. Error Code : %d",WSAGetLastError());
       // return 1;
    }
     
   // printf("Initialised.");

  struct sockaddr_in sock_addr;
  int sock_d = 0;
  char recvbuffer[BUFFERSIZE], sendbuffer[BUFFERSIZE], recvbuffer1[BUFFERSIZE];
  strcpy(sendbuffer,Prop);
  strcat(sendbuffer,",");
  sprintf(sendbuffer+ strlen(sendbuffer),"%lf",P);
  strcat(sendbuffer,",");
  sprintf(sendbuffer+ strlen(sendbuffer),"%lf",VF);
  strcat(sendbuffer,",");
  sprintf(sendbuffer+ strlen(sendbuffer),"%d",Nc);
  strcat(sendbuffer,",");
  for (i=0;i<Nc;i++)
  {
     strcat(sendbuffer,Comp[i]);
     strcat(sendbuffer,",");
  }
   for (j=0;j<Nc;j++)
  {
     sprintf(sendbuffer+ strlen(sendbuffer),"%lf",x[j]);
     if(j<(Nc-1))
    { strcat(sendbuffer,",");}
  }
 
  if ((sock_d = socket(AF_INET,SOCK_STREAM,0)) < 0)
     printf("Problem in creating socket");

  memset(&sock_addr, 0, sizeof(sock_addr));
  sock_addr.sin_family = AF_INET;
  sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  sock_addr.sin_port = htons(7000);

  if(connect(sock_d, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
      perror("Problem in connecting to server");

 
       sendto(sock_d, sendbuffer, strlen(sendbuffer), 0, (struct sockaddr*) &sock_addr, sizeof(sock_addr));
       memset(recvbuffer, 0, BUFFERSIZE);
       int size = sizeof(sock_addr);
       if (recvfrom(sock_d, recvbuffer, BUFFERSIZE, 0, (struct sockaddr*) &sock_addr, &size) == 0)
       perror("The server therminated prematurely");
       
    char *result1 = recvbuffer;
       token = strtok(result1, " ");
       while (token != NULL)
        {
 	  ResultStr[k] = token;
          token = strtok(NULL, " ");
          Resultss[k] = round(atof(ResultStr[k])*100000)/100000;
          k++;
          
        }
 close(sock_d);
}

#include	"unp.h"
#define MAXMEM 5  
#define NAMELEN 20  
 int  target=-1,sender=-1,you_can_send=0;
 char	recvline[MAXLINE],sendername[MAXLINE],filename[NAMELEN];
 char	sendline[MAXLINE];

void rcv(int *sockfd){
int r;
FILE *fp;
while(1){

memset(recvline,0,MAXLINE);
	if(read(*sockfd, recvline, MAXLINE)!=0)
	{
		
		if(recvline[0]=='@'||recvline[0]=='#'||recvline[0]=='!')fputs(recvline, stdout);
		if(recvline[0]=='!')//file			
		sscanf(recvline+1,"%d%s%s",&sender,sendername,filename);
		
		if(recvline[0]==':')
		{
	if(recvline[1]=='y'&& target!=-1){you_can_send=1;printf("press enter to send\n");}
	else printf("file refused\n");
		}
		if(sender!=-1 && strcmp(recvline,"ready to catch\n")==0)
		{
			printf("catching file\n");
			fp=fopen(filename,"wb");
			if(fp==NULL){
				perror("not open\n");continue;
			}
				memset(recvline,0,MAXLINE);
			while(read(*sockfd, recvline, MAXLINE)>0){
				
			if(strcmp(recvline,"send over")==0)break;
			fwrite(recvline,sizeof(char),MAXLINE,fp); 
			
			memset(recvline,0,MAXLINE);
			}
			fclose(fp);	
			printf("catch over\n");sender=-1;
		}
if(strcmp(recvline,"out")==0)return;
	}	
					
		
		
		

	}
}
void snd(int *sockfd){
int r;
FILE *fp;
while (1) {		
	fgets(sendline, MAXLINE, stdin);
	
	if(strcmp(sendline,"\n")==0);
	else write(*sockfd, sendline, strlen(sendline));
	
	if(sendline[0]==':')//file
	{
		if(sendline[1]=='f')//:f ID filename
		sscanf(sendline+2,"%d%s",&target,filename);
		
		//target :y ID or :n ID
	 	if(sendline[1]=='n'){printf("you don't want the file\n");sender=-1;}
		if(sendline[1]=='y' && sender==-1)//I want the file
			{printf("no file to you\n");continue;}	
		
	}
	if(sendline[0]==':' && sendline[1]=='q')return;
	if(you_can_send && target!=-1){
		memset(sendline,0,MAXLINE);
		sprintf(sendline,"send start");
		write(*sockfd, sendline, MAXLINE);
		fp=fopen(filename,"rb");
			if(fp==NULL){perror("not found\n");continue;}
			
			while(!feof(fp))
			{
				memset(sendline,0,MAXLINE);
				r=fread(sendline,sizeof(char),MAXLINE,fp);
				write(*sockfd,sendline,MAXLINE); 
			}
		fclose(fp);
		sprintf(sendline,"send over");
		write(*sockfd,sendline,MAXLINE); 
		printf("send over\n");
		you_can_send=0;		target=-1;
		}
	
	}
}

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	pthread_t tid1,tid2;
	char	sendline[MAXLINE];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

printf("君の名は：");
fgets(sendline, MAXLINE, stdin);
write(sockfd, sendline, strlen(sendline));
	 printf(":l =list\n:s ID =secret talk\n:f ID filename =send file\n");
	pthread_create(&tid1,NULL,(void*)&rcv,(void*)&sockfd);
	pthread_create(&tid2,NULL,(void*)&snd,(void*)&sockfd);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	close(sockfd);
}

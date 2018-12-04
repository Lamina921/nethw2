#include	"unp.h"


#define MAXMEM 5  
#define NAMELEN 20  
char namelist[MAXMEM][NAMELEN];
int listenfd,connfd[MAXMEM];
void quit();
void rcv_snd(int n)  
{  
	int i=0,r;
  
    char buf[MAXLINE]="";  
    char buf1[MAXLINE];  
    char buf2[MAXLINE];  
    char name[NAMELEN];  
	char filename[NAMELEN];
    	printf("n=%d\n",n);
    int retval;  
  int target;
    int len;  
    len=read(connfd[n],name,NAMELEN); 
    name[strlen(name)-1]='\0';   
    strcpy(buf,name);
    strcat(buf,"__join in\n\0"); 
     printf("%s\n",buf);
	sprintf(namelist[n],"%s",name);
	
    for(i=0;i<MAXMEM;i++)  
        if(connfd[i]!=-1)  
            write(connfd[i],buf,strlen(buf));  
           
    while(1)  
    {  
	memset(buf,0,MAXLINE);
	memset(buf1,0,MAXLINE);



        if((len=read(connfd[n],buf1,MAXLINE))>0)  
        {                

             /*if(strcmp("log out\n",buf1)==0)  
             {  
				sprintf(buf,"out");
				write(connfd[n],buf,strlen(buf));  
                close(connfd[n]);  
                connfd[n]=-1;  
                pthread_exit(&retval);  
             }  */
			if(buf1[0]==':')
				switch(buf1[1])
				{
				case 'q':
				sprintf(buf,"out");
				write(connfd[n],buf,strlen(buf));  
                close(connfd[n]);  
                connfd[n]=-1;  
                pthread_exit(&retval);  break;
				case 'l'://查看有哪些使用者正在server線上
				sprintf(buf,"# list:\n");
				for(i=0;i<MAXMEM;i++)
					if(connfd[i]!=-1){
					sprintf(buf1,"id:%d name:%s\n",i,namelist[i]);
					strcat(buf,buf1);
			}
				write(connfd[n],buf,strlen(buf));
				break;
				case 's'://指定傳送訊息給正在線上的某個使用者 :s ID
				
				sscanf(buf1+2,"%d",&target);
				
				if(connfd[target]==-1){sprintf(buf,"not found\n");write(connfd[n],buf,strlen(buf));break;}
				
				memset(buf1,0,MAXLINE);
				while(read(connfd[n],buf1,MAXLINE)&&strcmp(buf1,"<<\n")!=0)
				{

				sprintf(buf,"@  %s",name);  
				strcat(buf,"_want to say to you: ");  
				strcat(buf,buf1);
				write(connfd[target],buf,strlen(buf)); 
				memset(buf,0,MAXLINE);
				memset(buf1,0,MAXLINE);
					}
				break;
				case 'f'://也可以傳送檔案給另一個使用者，對方可決定是否接收
				sscanf(buf1+2,"%d%s",&target,filename);
			
				sprintf(buf,"!%d %s %s want to send to you(:y ID/:n ID)\n",n,name,filename);				
				write(connfd[target],buf,strlen(buf));
				memset(buf,0,MAXLINE);			
				
				break;
				case 'y':
				case 'n':
				sscanf(buf1+2,"%d",&target);
				if(connfd[target]!=-1){
					write(connfd[target],buf1,strlen(buf1));
					if(buf1[1]=='y')
				{
					sprintf(buf,"ready to catch\n");
					write(connfd[n],buf,strlen(buf));
				}
			}
				break;
				default:
				;
	}
	else if(strcmp(buf1,"send start")==0)
	{//file
		printf("s start\n");
		memset(buf,0,MAXLINE);
		
	while(read(connfd[n], buf,MAXLINE)>0 )
	{
		
			write(connfd[target],buf,MAXLINE);
			if(strcmp(buf,"send over")==0)break; 
			memset(buf,0,MAXLINE);
	}
			printf("over\n");
		}
	
else{
         sprintf(buf,"@ %s",name);  
         strcat(buf,": ");  
         strcat(buf,buf1);
	   
		printf("%s",buf);
        for(i=0;i<MAXMEM;i++)          
             if(connfd[i]!=-1)               
                  write(connfd[i],buf,strlen(buf));    
    }   
}
}  
}
  
int main()  
{  
    pthread_t thread,tid;  
    struct sockaddr_in servaddr,cliaddr;  
    socklen_t len;   
    char buf[MAXLINE];  
    int i=3;

    listenfd=socket(AF_INET,SOCK_STREAM,0);  
    if(listenfd<0)  
    {  
        perror("Socket created failed.\n");  
        return -1;  
    }  
 
    servaddr.sin_family=AF_INET;  
    servaddr.sin_port=htons(SERV_PORT);  
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);  
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)  
    {  
        perror("Bind failed.\n");  
        return -1;  
    } 
    listen(listenfd,LISTENQ); 
    
  
    for(i=0;i<MAXMEM;i++)  
    {  
        connfd[i]=-1;  
    }  
  
    while(1)  
    {  
        len=sizeof(cliaddr);  
        for(i=0;i<MAXMEM;i++)  
        {  
            if(connfd[i]==-1)  
            {  
                break;  
            }  
        }  
        connfd[i]=accept(listenfd,(struct sockaddr*)&cliaddr,&len);  
printf("accept\n");
        pthread_create(&tid,NULL,(void*)(&rcv_snd),(void*)i);  
        pthread_detach(tid);
          
    }  
    return 0;  
}  
  
void quit()  
{  
    char msg[10];  
    while(1)  
    {  
        scanf("%s",msg);  
        if(strcmp("quit",msg)==0)  
        {  
            printf("Byebye...\n");  
            close(listenfd);  
            exit(0);  
        }  
    }  
}  

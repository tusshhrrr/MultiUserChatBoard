#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>

int ifint(char* str)
{
  int c=0;
  for (int i=0;i<strlen(str);i++)
   {if (!isdigit(str[i]))
   return 0;}
   return 1;

}

char** tokenize(char str1[])
{

    char** newString=(char**)malloc(sizeof(char*)*100);
    int i,j,ctr;
    j=0; ctr=0;
		*(newString+ctr)=(char*)malloc(sizeof(char)*100);
    for(i=0;i<=(strlen(str1));i++)
    {

        if(str1[i]==' '||str1[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;
						*(newString+ctr)=(char*)malloc(sizeof(char)*100);
            j=0;
        }
        else
        {
            newString[ctr][j]=str1[i];
            j++;
        }
    }
		newString[ctr]=(char*)malloc(sizeof(char)*3);
		newString[ctr][0]='e';
		newString[ctr][1]='~';
		newString[ctr][2]=0;
    return newString;
}
struct memory {
	char buff[100];
	int pid[100];
};

struct memory* shmptr;
void handler(int signum)
{


	if (signum == SIGUSR1) {
		char* n;
		char** buffer=tokenize(strdup(shmptr->buff));
		int fin;
		for (int i=0;i<100;i++)
		{
			if (strcmp(buffer[i],"e~")==0)
			{
				fin=i;
				break;
			}

		}

		if (strlen(shmptr->buff)!=1)
		{
			if (strcmp(buffer[0],"0")==0)
			printf("Received in group chat from user %s:",buffer[fin-1]);
			else
			printf("Recieved from user %s:",buffer[fin-1]);

			for(int i=1;i<fin-1;i++)
				printf("%s ", buffer[i]);
    }
		printf("\n" );

		}
		}

int main()
{
	int pid = getpid();
	int shmid;
	int key = 123456789;
	shmid = shmget(key, sizeof(struct memory), IPC_CREAT | 0666);
	char x[10];
	printf("Enter user no.");
	fgets(x, 10, stdin);
  x[strlen(x)-1]='\0';
  // printf("%d",strlen(x));

  if (ifint(x)==0)
  {
    printf("Invalid input\n");
    exit(-1);
  }
	shmptr = (struct memory*)shmat(shmid, NULL, 0);
	char* n;
	int i=strtol(x,&n,10);
  if (i<0||i>99)
  {
    printf("Invalid user no.\n");
    exit(-1);
  }
	if (shmptr->pid[i]==0)
		{shmptr->pid[i] = pid;}
	else
	{
		printf("User already exists\n" );
		exit(-1);
  }
	signal(SIGUSR1, handler);
	fflush(stdin);
	while (1) {
		fgets(shmptr->buff, 100, stdin);

		if (strcmp(shmptr->buff,">exit\n")==0)
		{
			shmptr->pid[i]=0;
			exit(-1);
		}

		char* n;
		int x=strtol(shmptr->buff,&n,10);
    char** tok=tokenize(strdup(shmptr->buff));
    if (ifint(tok[0])==0)
    {
      printf("Invalid input\n" );
      continue;
    }

		char temp[100];

		shmptr->buff[strlen(shmptr->buff)-1]=0;
		strcat(shmptr->buff," ");
		char temp2[100];
		sprintf(temp2,"%d",i);
		strcat(shmptr->buff,temp2);
    if ((x<0||x>99) ||((x>0 && x<100) && shmptr->pid[x]==0))
    {
      printf("No such user exists\n");
      continue;
    }
		if (x==0)
		{
			x+=1;
		while (x<100)
		{


			if (x!=i && shmptr->pid[x]!=0)
				kill(shmptr->pid[x], SIGUSR1);
		x+=1;
	}}
		else
			kill(shmptr->pid[x], SIGUSR1);
	}

	return 0;
}

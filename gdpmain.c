#include "gendp.h"
#include "gdpparser.h"
#include "converter.h"
#include "normaldp.h"
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int readline(int fd, char *line)
{
  int n=0;
  char temp;
  int temp2;
  while(1)
    {
      temp2=read(fd, &temp, 1);
      if(temp2==0)
        {
          line[n]=0;
          return n;}
      else{
      line[n]=temp;
      n++;
      if(temp=='\n')
        {line[n]=0;
        return n;}
      }
    }
  return n;
}


int main(int argc, char **argv)
{
  int fd;
  char tmp;
  int *length;
  int statements=0;
  int i=0;
  int j;
  int count;
  int oldclock, newclock;
  char **strings;
  struct gbranch *rootbranch;
  struct branch *rootnbranch;

  if(argc!=3)
    {
      printf("Usage: ./%s <input file> <# of iterations>\n", argv[0]);
      exit(1);
    }

  sscanf(argv[2], "%d", &count);

  fd=open(argv[1], O_RDONLY, 0);
  while(read(fd, &tmp, 1) )
    {
      if(tmp=='\n')
	statements++;
    }
  
  fd=open(argv[1], O_RDONLY, 0);
  length=(int *)malloc(sizeof(int)*statements);
  length[0]=0;
  while(read(fd, &tmp, 1) )
    {
      length[i]++;
      if(tmp=='\n')
	{
	  length[i]--;
	  i++;
	  length[i]=0;
	}
      
    }
  fd=open(argv[1], O_RDONLY, 0);
  strings=(char **)malloc(sizeof(char *) * statements );
  strings[0]=(char *)malloc(sizeof(char)*length[0]);
  j=0;
  i=0;
  while(read(fd, &tmp, 1) )
    {
      if(tmp=='\n')
	{
	  j=0;
	  i++;
	  if(i!=statements)
	    strings[i]=(char *)malloc(sizeof(char)*length[i]);
	}
      else
	{
	  strings[i][j]=tmp;
	  j++;
	}
    }

  oldclock=clock();
  
  for(j=0;j<count;j++)
    {
      rootbranch=gdpparse(strings, length, statements);
    }
  newclock=clock();
  printf("Parser time is %d\n",(newclock-oldclock));
  

  oldclock=clock();
  
  for(j=0;j<count;j++)
    {
      rootbranch=gdpparse(strings, length, statements);
      i=resolve(rootbranch);
      
    }
  newclock=clock();
  printf("GDP Answer %d in %d\n", i, (newclock-oldclock));
  
  oldclock=clock();
  for(j=0;j<count;j++)
    {
      rootbranch=gdpparse(strings, length, statements);
      rootnbranch=gtonconvert(rootbranch);
      
      i=nresolve(rootnbranch);
    }
  newclock=clock();
  printf("NDP Answer %d in %d\n", i, (newclock-oldclock));
}

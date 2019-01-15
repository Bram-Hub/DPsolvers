#include <stdio.h>
#include "normaldp.h"


int main()
{
  struct branch *startbranch;
  struct clause *tempclause;
  int val;
  startbranch=(struct branch *)malloc(sizeof(struct branch) );
  startbranch->nextbranch=NULL;
  startbranch->numclauses=4;
  startbranch->firstclause=(struct clause *)malloc(sizeof(struct clause) );
  startbranch->firstclause->variables=(int *)calloc(2, sizeof(int) );
  startbranch->firstclause->variables[0]=2;
  startbranch->firstclause->variables[1]=3;
  tempclause=startbranch->firstclause;
  tempclause->numvariables=tempclause->varlength=2;
  tempclause->nextclause=(struct clause *)malloc(sizeof(struct clause) );
  tempclause=tempclause->nextclause;
  tempclause->numvariables=tempclause->varlength=2;
  tempclause->variables=(int *)calloc(2, sizeof(int) );
  tempclause->variables[0]=2;
  tempclause->variables[1]=-3;
  tempclause->nextclause=(struct clause *)malloc(sizeof(struct clause) );
  tempclause=tempclause->nextclause;
  tempclause->numvariables=tempclause->varlength=2;
  tempclause->variables=(int *)calloc(2, sizeof(int) );
  tempclause->variables[0]=-2;
  tempclause->variables[1]=-3;/*
  tempclause->nextclause=(struct clause *)malloc(sizeof(struct clause) );
  tempclause=tempclause->nextclause;
  tempclause->numvariables=tempclause->varlength=2;
  tempclause->variables=(int *)calloc(2, sizeof(int) );
  tempclause->variables[0]=-2;
  tempclause->variables[1]=3;*/
  tempclause->nextclause=NULL;
  printf("Starting\n");
  val=resolve(startbranch);
  printf("%d\n", val);
}


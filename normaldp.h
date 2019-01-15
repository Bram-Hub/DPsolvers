#ifndef SADVAK_NORMALDP
#define SADVAK_NORMALDP


#include <stdlib.h>
#include <stdio.h>


/* represents a clause for DP.  *variables will point to an array of literals,
   with negative values meaning negations of literals */
struct clause
{
  int *variables;
  int varlength; /*size of array*/
  int numvariables; /*number of valid variables*/
  struct clause *nextclause;/*built-in linked list*/
};

struct branch
{
  /*int arrlength;*//*size of array*/
  int numclauses;/*number left*/
  struct clause *firstclause;/*first clause in list*/

  /*built-in linked list, for when we need to branch*/
  struct branch *nextbranch;
};
/* return 0 if clause is satisfied, 1 if clause is returned, and 2 if clause
   is empty (branch closes) */
/* I decided to simply leave placeholders because it seemed like it should
   involve less overhead than rewriting the variable list at each branch.
   If time allows, I will test this later */
int handleclause(int literal, struct clause *tohandle)
{
  int i;
  for(i=0;i<tohandle->varlength;i++)
    {
      if(tohandle->variables[i]==literal)
	{
	  return 0;
	}
      if(tohandle->variables[i]==-1*literal)
	{
	  if(tohandle->numvariables==1)
	    return 2;
	  tohandle->numvariables--;
	  tohandle->variables[i]=0;
	  return 1;
	}
      
    }
  return 1;
}

/* loops until problem is solved, one way or another.  returns 1 if sat,
   0 if contra.
*/
int nresolve(struct branch *startbranch)
{
  int i,j,k,l;
  int branchvar;
  int handleval;
  struct branch *firstbranch;
  struct branch *lastbranch=startbranch;
  struct branch *tempbranch;
  struct clause *tempclause;
  struct clause *tempclause2;
  struct clause **ptrtochange;

  firstbranch=startbranch;
  /*trust that program will not let an empty branch 
    or clause continue to exist*/
  while(1)
    {/*find something to branch on*/
      for(j=0;firstbranch->firstclause->variables[j]==0;j++)
	{
	}
      branchvar=firstbranch->firstclause->variables[j];
#ifdef SADVAK_VERBOSE
      printf("Branching on %d\n", branchvar);
      printf("Root branch:\n");
      {
	int m;
	struct clause *tc;
	for(tc=firstbranch->firstclause;tc!=NULL;tc=tc->nextclause)
	  {
	    printf("Clause: ");
	    for(m=0;m<tc->varlength;m++)
	      {
		if(tc->variables[m])
		  printf("%d ", tc->variables[m]);
	      }
	    printf("\n");
	  }
      }
#endif
      /*branch on this variable, then break*/
      tempbranch=(struct branch *)malloc(sizeof(struct branch) );
      /* lastbranch->nextbranch=tempbranch;
	 lastbranch=tempbranch;*/
      tempbranch->firstclause=(struct clause *)malloc(sizeof(struct clause) );
      tempclause2=firstbranch->firstclause;
      tempclause=tempbranch->firstclause;
      /*copy the clauses to the new branch*/
      while(tempclause2!=NULL)
	{
	  tempclause->numvariables=tempclause2->numvariables;
	  tempclause->varlength=tempclause2->varlength;
	  tempclause->variables=calloc(tempclause->varlength, sizeof(int) );
	  memcpy(tempclause->variables, tempclause2->variables, 
		 tempclause->varlength*sizeof(int) );
	  tempclause2=tempclause2->nextclause;
	  if(tempclause2!=NULL)
	    {
	      tempclause->nextclause=(struct clause *)malloc(sizeof(struct clause) );
	      tempclause=tempclause->nextclause;
	    }
	}
      tempclause->nextclause=NULL;
      /*handle each branch, in opposite signs*/
      tempclause=tempbranch->firstclause;
      ptrtochange=&(tempbranch->firstclause);
      while(tempclause!=NULL)
	{
	  handleval=handleclause(-1*branchvar, tempclause);
	  if(handleval==2)
	    {
	      /*close this branch*/
	      tempclause=tempbranch->firstclause;
	      while(tempclause!=NULL)
		{
		  tempclause2=tempclause->nextclause;
		  free(tempclause->variables);
		  free(tempclause);
		  tempclause=tempclause2;
		}
	      /*free branch*/
	      free(tempbranch);
	      tempbranch=NULL;
	      break;
	    }
	  if(handleval==0)
	    {
	      /*clear this clause, and continue*/
	      tempclause2=tempclause;
	      tempclause=tempclause->nextclause;
	      *ptrtochange=tempclause;
	      free(tempclause2->variables);
	      free(tempclause2);
	      if(tempbranch->firstclause==NULL)
		{
		  /*problem is satisfied, return as appropriate*/
		  /*maybe should clean up here*/
		  return 1;
		}
	    }
	  else
	    {
	      ptrtochange=&(tempclause->nextclause);
	      tempclause=tempclause->nextclause;
	    }
	}/*end while loop to handle new branch*/
      if(tempbranch!=NULL)
	{
	  lastbranch->nextbranch=tempbranch;
	  lastbranch=tempbranch;
	}
      

      tempclause=firstbranch->firstclause;
      ptrtochange=&(firstbranch->firstclause);
      while(tempclause!=NULL)
	{
	  handleval=handleclause(branchvar, tempclause);
	  if(handleval==2)
	    {
	      /*close this branch*/
	      tempclause=firstbranch->firstclause;
	      while(tempclause!=NULL)
		{
		  tempclause2=tempclause->nextclause;
		  free(tempclause->variables);
		  free(tempclause);
		  tempclause=tempclause2;
		}
	      /*remove branch from list*/
	      tempbranch=firstbranch;
	      firstbranch=firstbranch->nextbranch;
	      free(tempbranch);
	      if(firstbranch==NULL)
		return 0;
	      break;
	    }
	  if(handleval==0)
	    {
	      /*clear this clause, and continue*/
	      tempclause2=tempclause;
	      tempclause=tempclause->nextclause;
	      *ptrtochange=tempclause;
	      free(tempclause2->variables);
	      free(tempclause2);
	      if(firstbranch->firstclause==NULL)
		{
		  /*problem is satisfied, return as appropriate*/
		  /*maybe should clean up here*/
		  return 1;
		}
	    }
	  else
	    {
	      ptrtochange=&(tempclause->nextclause);
	      tempclause=tempclause->nextclause;
	    }
	}/*end while loop to handle first branch*/


    }/*end while loop to resolve*/

}/*end function*/



#endif





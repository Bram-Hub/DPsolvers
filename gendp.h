#ifndef SADVAK_GENDP
#define SADVAK_GENDP

#include <stdlib.h>
#include <stdio.h>


/* represents a statement */
struct statement
{

  /* binary tree represented as an array
     values are:
     -5    left
     -4    right
     -3    &
     -2    |
     -1    F
     0     null
     1     T
     2     ->
     3     !left
     4     !right
     >=6   literal
     <=-6  !literal
     
  */
  int *statementarray;/*operators and literals*/
  int size;/*how large statementarray is*/
  struct statement *next; /*next statement*/
};

struct gbranch
{
  struct statement *firststatement;
  struct gbranch *nextbranch;
  
};


/* return 1 if sat, 0 if unsat */
int resolve(struct gbranch *rootbranch)
{
  struct gbranch *lastbranch=rootbranch;
  struct statement *tempstatement;
  struct statement *laststatement;
  struct gbranch *tempbranch;
  struct gbranch *holdingbranch;
  struct statement *sttocopy;
  struct statement *backstatement;
  struct statement **ptrtostatement;
  struct statement *tmst;
  int littobranch;
  int sign; /* 1==true, 0==false, -1==variable */
  int i,j,k;
  int killbranch=0;
  while(1)
    {
      /* find a variable */
      tempstatement=rootbranch->firststatement;
      for(i=0;tempstatement->statementarray[i]<6 && tempstatement->statementarray[i]>-6;i++)
	{}
      littobranch=tempstatement->statementarray[i];
#ifdef SADVAK_VERBOSE
      {
	int counter;
	struct statement *printst;
	printf("Current state of root branch:\n");
	for(printst=rootbranch->firststatement;printst!=NULL;printst=printst->next)
	  {
	    for(counter=0;counter<printst->size;counter++)
	      {
		printf("%d at %d\n", printst->statementarray[counter], counter);
	      }
	  }
	printf("Branching on %d\n", littobranch);
      }
#endif
      /* branch */
      tempbranch=(struct gbranch *)malloc(sizeof(struct gbranch) );
      tempbranch->nextbranch=NULL;
      
      tempbranch->firststatement=(struct statement *)malloc(sizeof(struct statement) );
      tempstatement=tempbranch->firststatement;
      sttocopy=rootbranch->firststatement;

      /*copy all statements to new branch*/
      while(1)
	{
	  tempstatement->size=sttocopy->size;
	  tempstatement->statementarray=(int *)malloc(sizeof(int)*sttocopy->size);
	  memcpy(tempstatement->statementarray, sttocopy->statementarray, 
		 sizeof(int)*sttocopy->size);
	  if(sttocopy->next==NULL)
	    {
	      tempstatement->next=NULL;
	      break;
	    }
	  tempstatement->next=(struct statement *)malloc(sizeof(struct statement ) );
	  tempstatement=tempstatement->next;
	  sttocopy=sttocopy->next;
	}

      holdingbranch=tempbranch;
      tempbranch=rootbranch;
      tempstatement=tempbranch->firststatement;
      ptrtostatement=&(tempbranch->firststatement);
      killbranch=0;
      /* resolve first branch - littobranch */
      while(tempstatement!=NULL)
	{
	  
	  for(i=0;i<tempstatement->size;i++)
	    {
	      int working=1;
	      int truth=0;
	      if(tempstatement->statementarray[i]==littobranch)
		{/*statementarray[i] is true*/
		  truth=1;
		}
	      else if(tempstatement->statementarray[i]==-1*littobranch)
		{/*statementarray[i] is false*/
		  truth=-1;
		}
	      while(truth && working)
		{
		  working=0;
		  tempstatement->statementarray[i]=0;
		  if(i==0)
		    {/* this is root node */
		      if(truth==1)
			{/* remove this statement and keep going */
			  tmst=tempstatement;
			  tempstatement=tempstatement->next;
			  free(tmst->statementarray);
			  free(tmst);
			  i=-1;
			  *ptrtostatement=tempstatement;
			  break;
			}
		      else
			{/* kill this branch and move on */
			  killbranch=1;
			  i=tempstatement->size;
			  break;
			}
		    }
		  /*switch of operator of parent*/
		  switch(tempstatement->statementarray[(i-1)/2])
		    {
		    case -5:/* left */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  /*ignore*/
			  break;
			case 0:/* i is left, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 1:/* i is right, true */
			  /*ignore*/
			  break;
			case 2:/* i is left, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -4:/* right */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 0:/* i is left, false */
			  /*ignore*/
			  break;
			case 1:/* i is right, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 2:/* i is left, true */
			  /*ignore*/
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -3:/* and */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 0:/* i is left, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 1:/* i is right, true */
			  tempstatement->statementarray[(i-1)/2]=-5;
			  break;
			case 2:/* i is left, true */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -2:/* or */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  tempstatement->statementarray[(i-1)/2]=-5;
			  break;
			case 0:/* i is left, false */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			case 1:/* i is right, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 2:/* i is left, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case 2:/* implication */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  tempstatement->statementarray[(i-1)/2]=3;
			  break;
			case 0:/* i is left, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 1:/* i is right, true */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 2:/* i is left, true */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			default:/* should never, ever happen */

			  
			  break;
			}
		      break;
		    case 3:/* not left */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  /*ignore*/
			  break;
			case 0:/* i is left, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 1:/* i is right, true */
			  /*ignore*/
			  break;
			case 2:/* i is left, true */
			  truth=-1;
			  working=1;
			  i=(i-1)/2;
			  break;
			}
		      break;
		    case 4:/* not right */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 0:/* i is left, false */
			  /*ignore*/
			  break;
			case 1:/* i is right, true */
			  truth=-1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 2:/* i is left, true */
			  /*ignore*/
			  break;
			}
		      break;
		      
		    default:/*means statementarray[i] is garbage, ignore*/
		      
		      break;
		    }/*end switch*/
		  
		}/*end while truth & working */	 
	      if(tempstatement==NULL)
		break;
	    }/* end for loop to search variables */
	  if(killbranch)
	    {
#ifdef SADVAK_VERBOSE
	      printf("Killing branch\n");
#endif
	      rootbranch=tempbranch->nextbranch;
	      for(tempstatement=tempbranch->firststatement;tempstatement!=NULL;)
		{
		  struct statement *holder=tempstatement->next;
		  free(tempstatement->statementarray);
		  free(tempstatement);
		  tempstatement=holder;
		}
	      free(tempbranch);
	      break;
	    }
	  if(tempstatement!=NULL)
	    {
	      ptrtostatement=&(tempstatement->next);
	      tempstatement=tempstatement->next;
	    }
	  
	}/*end while loop to resolve this branch*/
      if(rootbranch!=NULL && rootbranch->firststatement==NULL)
	{/*this branch is satisfied*/
	  return 1;
	}
      /* resolve new branch - !littobranch */
      killbranch=0;
      littobranch*=-1;
      tempbranch=holdingbranch;
      tempstatement=tempbranch->firststatement;
      ptrtostatement=&(tempbranch->firststatement);
      while(tempstatement!=NULL)
	{
	  
	  for(i=0;i<tempstatement->size;i++)
	    {
	      int working=1;
	      int truth=0;
	      if(tempstatement->statementarray[i]==littobranch)
		{/*statementarray[i] is true*/
		  truth=1;
		}
	      else if(tempstatement->statementarray[i]==-1*littobranch)
		{/*statementarray[i] is false*/
		  truth=-1;
		}
	      while(truth && working)
		{
		  working=0;
		  tempstatement->statementarray[i]=0;
		  if(i==0)
		    {/* this is root node */
		      if(truth==1)
			{/* remove this statement and keep going */
			  tmst=tempstatement;
			  tempstatement=tempstatement->next;
			  free(tmst->statementarray);
			  free(tmst);
			  i=-1;
			  *ptrtostatement=tempstatement;
			  break;
			}
		      else
			{/* kill this branch and move on */
			  killbranch=1;
			  i=tempstatement->size;
			  break;
			}
		    }
		  /*switch of operator of parent*/
		  switch(tempstatement->statementarray[(i-1)/2])
		    {
		    case -5:/* left */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  /*ignore*/
			  break;
			case 0:/* i is left, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 1:/* i is right, true */
			  /*ignore*/
			  break;
			case 2:/* i is left, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -4:/* right */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 0:/* i is left, false */
			  /*ignore*/
			  break;
			case 1:/* i is right, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 2:/* i is left, true */
			  /*ignore*/
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -3:/* and */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 0:/* i is left, false */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 1:/* i is right, true */
			  tempstatement->statementarray[(i-1)/2]=-5;
			  break;
			case 2:/* i is left, true */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case -2:/* or */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  tempstatement->statementarray[(i-1)/2]=-5;
			  break;
			case 0:/* i is left, false */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			case 1:/* i is right, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			case 2:/* i is left, true */
			  i=(i-1)/2;
			  working=1;
			  break;
			default:/* should never, ever happen */
			  
			  break;
			}
		      break;
		    case 2:/* implication */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  tempstatement->statementarray[(i-1)/2]=3;
			  break;
			case 0:/* i is left, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 1:/* i is right, true */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 2:/* i is left, true */
			  tempstatement->statementarray[(i-1)/2]=-4;
			  break;
			default:/* should never, ever happen */

			  
			  break;
			}
		      break;
		    case 3:/* not left */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  /*ignore*/
			  break;
			case 0:/* i is left, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 1:/* i is right, true */
			  /*ignore*/
			  break;
			case 2:/* i is left, true */
			  truth=-1;
			  working=1;
			  i=(i-1)/2;
			  break;
			}
		      break;
		    case 4:/* not right */
		      switch(truth + i%2)
			{
			case -1:/* i is right, false */
			  truth=1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 0:/* i is left, false */
			  /*ignore*/
			  break;
			case 1:/* i is right, true */
			  truth=-1;
			  working=1;
			  i=(i-1)/2;
			  break;
			case 2:/* i is left, true */
			  /*ignore*/
			  break;
			}
		      break;
		      
		    default:/*means statementarray[i] is garbage, ignore*/
		      
		      break;
		    }/*end switch*/
		  
		}/*end while truth & working */	 
	      if(tempstatement==NULL)
		break;
	    }/* end for loop to search variables */
	  if(killbranch)
	    {
#ifdef SADVAK_VERBOSE
	      printf("Killing branch\n");
#endif
	      for(tempstatement=tempbranch->firststatement;tempstatement!=NULL;)
		{
		  struct statement *holder=tempstatement->next;
		  free(tempstatement->statementarray);
		  free(tempstatement);
		  tempstatement=holder;
		}
	      free(tempbranch);
	      if(rootbranch==NULL)
		return 0;
	      break;
	    }
	  if(tempstatement!=NULL)
	    {
	      ptrtostatement=&(tempstatement->next);
	      tempstatement=tempstatement->next;
	    }

	}/*end loop to resolve new branch*/
      if(!killbranch)
	{
	  tempbranch->nextbranch=rootbranch;
	  rootbranch=tempbranch;
	  if(rootbranch->firststatement==NULL)
	    {/*this branch is satisfied*/
	      return 1;
	    }
	}
    }/*end main while loop*/
}/*end resolve*/



#endif




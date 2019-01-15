#include "gendp.h"
#include "normaldp.h"
#include <stdio.h>
#include <stdlib.h>


struct tree
{
  int *where;
  int howmany;
  struct tree *nexttree;
};

struct branch *gtonconvert(struct gbranch *groot)
{
  struct statement *tempstatement;
  int i,j,k;

  struct branch *retbranch;
  struct clause *tempclause;
  
  struct tree *firsttree;
  struct tree *temptree;
  struct tree *temptree2;
  struct tree *lasttree;
  retbranch=(struct branch *)malloc(sizeof(struct branch) );
  retbranch->nextbranch=NULL;
  retbranch->firstclause=NULL;
  
  
  tempstatement=groot->firststatement;
  while(tempstatement!=NULL)
    {
      /*do this for each statement*/
      for(i=0;i<tempstatement->size;i++)
	{
	  switch(tempstatement->statementarray[i])
	    {
	    case 2:/* -> */
	      tempstatement->statementarray[i]=-2;
	      switch(tempstatement->statementarray[2*i+1])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+1]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+1]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+1]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+1]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+1]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+1]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+1]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+1]*=-1;
		  break;
		}
	      
	      break;
	    case 3:/* !left (!right shouldn't come up) */
	      tempstatement->statementarray[i]=-5;
	       switch(tempstatement->statementarray[2*i+1])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+1]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+1]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+1]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+1]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+1]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+1]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+1]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+1]*=-1;
		  break;
		}
	      
	      break;
	    case -1:/* placeholder for !(&) */
	      tempstatement->statementarray[i]=-2;
	      switch(tempstatement->statementarray[2*i+1])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+1]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+1]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+1]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+1]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+1]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+1]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+1]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+1]*=-1;
		  break;
		}
	      
	      switch(tempstatement->statementarray[2*i+2])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+2]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+2]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+2]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+2]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+2]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+2]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+2]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+2]*=-1;
		  break;
		}
	      
	      break;
	    case 1:/* placeholder for !(|) */
	      
	      tempstatement->statementarray[i]=-3;
	      switch(tempstatement->statementarray[2*i+1])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+1]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+1]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+1]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+1]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+1]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+1]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+1]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+1]*=-1;
		  break;
		}
	      
	      switch(tempstatement->statementarray[2*i+2])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+2]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+2]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+2]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+2]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+2]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+2]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+2]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+2]*=-1;
		  break;
		}
	      
	      break;
	    case 5:/* placeholder for !(->) */
	      tempstatement->statementarray[i]=-3;
	      switch(tempstatement->statementarray[2*i+2])
		{/* not */
		case -1:
		  tempstatement->statementarray[2*i+2]=-3;
		  break;
		case 1:
		  tempstatement->statementarray[2*i+2]=-2;
		  break;
		case -3:
		  tempstatement->statementarray[2*i+2]=-1;
		  break;
		case -2:
		  tempstatement->statementarray[2*i+2]=1;
		  break;
		case 2:
		  tempstatement->statementarray[2*i+2]=5;
		  break;
		case 3:
		  tempstatement->statementarray[2*i+2]=-5;
		  break;
		case 5:
		  tempstatement->statementarray[2*i+2]=2;
		  break;
		default:
		  tempstatement->statementarray[2*i+2]*=-1;
		  break;
		}
	      
	      break;
	    default:/* don't need to change in this pass */
	      
	      break;
	    }
	  

	  
	}/* end for loop to convert -> and ! */
      
      temptree=(struct tree *)malloc(sizeof(struct tree) );
      
      /*each clause will be at most as large as the num of disjunctions*/
      for(i=j=0;i<tempstatement->size;i++)
	if(tempstatement->statementarray[i]==-2)
	  j++;
      
      firsttree=temptree;
      lasttree=temptree;
      temptree->nexttree=NULL;
      temptree->where=(int *)malloc(sizeof(int)*(j+1));
      temptree->where[0]=0;
      temptree->howmany=1;
      while(temptree!=NULL)
	{
	  while(1)
	    {
	      int done=1;
	      for(i=0;i<temptree->howmany;i++)
		{
		  switch(tempstatement->statementarray[temptree->where[i] ])
		    {
		    case -5:
		      temptree->where[i]=temptree->where[i]*2 + 1;
		      done=0;
		      break;
		    case -3:
		      temptree2=(struct tree *)malloc(sizeof(struct tree) );
		      temptree2->where=(int *)malloc(sizeof(int)*(j+1));
		      memcpy(temptree2->where, temptree->where, 
			     sizeof(int)*temptree->howmany);
		      temptree2->howmany=temptree->howmany;
		      temptree2->where[i]=temptree->where[i]*2 + 1;
		      temptree->where[i]=temptree->where[i]*2 + 2;
		      temptree2->nexttree=NULL;
		      lasttree->nexttree=temptree2;
		      lasttree=temptree2;
		      done=0;
		      break;
		    case -2:
		      temptree->where[temptree->howmany]=temptree->where[i]*2 + 2;
		      temptree->where[i]=temptree->where[i]*2 + 1;
		      temptree->howmany++;
		      done=0;
		      break;
		    }
		}
	      if(done)
		break;
	    }/*done that tree, move on to next */
	  temptree=temptree->nexttree;
	}
      temptree=firsttree;
      while(temptree!=NULL)
	{
	  tempclause=(struct clause *)malloc(sizeof(struct clause) );
	  tempclause->variables=malloc(sizeof(int) * temptree->howmany);
	  tempclause->varlength=tempclause->numvariables=temptree->howmany;
	  for(i=0;i<temptree->howmany;i++)
	    {
	      tempclause->variables[i]=
		tempstatement->statementarray[temptree->where[i]];
	    }
	  
	  tempclause->nextclause=retbranch->firstclause;
	  retbranch->firstclause=tempclause;
	  firsttree=temptree;
	  temptree=temptree->nexttree;
	  free(firsttree->where);
	  free(firsttree);
	}
      
      tempstatement=tempstatement->next;
    }/* end while st!NULL */

  return retbranch;
}


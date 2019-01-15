#ifndef SADVAK_GDPPARSER
#define SADVAK_GDPPARSER

#include "gendp.h"
#include <stdlib.h>
#include <stdio.h>



struct gbranch *gdpparse(char **string, int *length, int statements)
{
  int i,j,k;
  int counter;
  int deepest;
  int depth[statements];
  struct gbranch *rootbranch=(struct gbranch *)malloc(sizeof(struct gbranch));
  struct statement *tempstatement=
    (struct statement *)malloc(sizeof(struct statement) );
  rootbranch->firststatement=tempstatement;
  rootbranch->nextbranch=NULL;
  
  for(j=0;j<statements;j++)
    {
      counter=0;
      deepest=0;
      for(i=0;i<length[j];i++)
	{
	  if(string[j][i]=='(')
	    {
	      counter++;
	    }
	  else if(string[j][i]==')')
	    {
	      if(counter>deepest)
		deepest=counter;
	      counter--;
	    }
	}
      if(counter>deepest)
	deepest=counter;
      k=1;
      for(i=0;i<=deepest;i++)
	{
	  k*=2;
	}
      k--;
      tempstatement->size=k;
      tempstatement->statementarray=(int *)malloc(sizeof(int)*k);
      depth[j]=deepest;
      
      if(j!=statements-1)
	{
	  tempstatement->next= 
	    (struct statement *)malloc(sizeof(struct statement) );
	  
	  tempstatement=tempstatement->next;
	}
    }/*end for loop on all statement to get size */
  tempstatement->next=NULL;
  
  tempstatement=rootbranch->firststatement;
  for(j=0;j<statements;j++)
    {
      /* location of operator, comma, right side ')' */
      int places[tempstatement->size][3];
      int deep=0;
      for(i=0;i<tempstatement->size;i++)
	{/* locate pieces of nodes */
	  if(i==0)
	    k=0;
	  else 
	    {
	      k=places[(i-1)/2][1 - (i%2)] +2;
	    }
	  if(i!=0 && (string[j][places[(i-1)/2][0]]==0 || 
		      (string[j][places[(i-1)/2][0]]>='A' && 
		       string[j][places[(i-1)/2][0]]<='Z' ) || 
		      (string[j][places[(i-1)/2][0]]=='!' && i%2==0)))
	    {/* based on parent, this node can't be reached */
	      places[i][0]=0;
	    }
	  else
	    {/*reachable node*/
	      places[i][0]=k;
	      if(string[j][k]=='!')
		{/* the only unary operator -- could be a negated literal */
		  if(string[j][k+1]!='(')
		    {/* negated literal */
		      places[i][0]=k;
		    }
		  else
		    {
		      places[i][0]=k;
		      for(k+=2;deep || string[j][k]!=')';k++)
			{
			  if(string[j][k]=='(')
			    deep++;
			  else if(string[j][k]==')')
			    deep--;
			}
		      places[i][2]=k;
		    }
		}
	      else if(string[j][k]>='A' && string[j][k]<='Z' )
		{/* a literal */
		  places[i][0]=k;
		}
	      else
		{/* any binary operator */
		  for(;deep || string[j][k]!=',';k++)
		    {
		      if(string[j][k]=='(')
			deep++;
		      else if(string[j][k]==')')
			deep--;
		    }
		  places[i][1]=k;
		  for(k+-2;deep || string[j][k]==')';k++)
		    {
		      if(string[j][k]=='(')
			deep++;
		      else if(string[j][k]==')')
			deep--;
		    }
		}
	    }/* end else (this node is a reachable part of the tree) */
	}/* end loop to locate nodes */

      for(i=0;i<tempstatement->size;i++)
	{/* build the tree array */
	  int tmp=tempstatement->statementarray[(i-1)/2];
	  if(i!=0 && (tmp==0 || (tmp==3 && i%2==0) || tmp>=6 || tmp<=-6 ) )
	    {
	      tempstatement->statementarray[i]=0;
	    }
	  else
	    {
	      switch(string[j][places[i][0]])
		{
		case 0:/*null*/
		  tempstatement->statementarray[i]=0;
		  break;
		case '!':
		  if(string[j][places[i][0]+1]=='(')
		    tempstatement->statementarray[i]=3;
		  else
		    tempstatement->statementarray[i]=(string[j][places[i][0]+1]-'A' + 6)*-1;
		  break;
		case '&':
		  tempstatement->statementarray[i]=-3;
		  break;
		case '|':
		  tempstatement->statementarray[i]=-2;
		  break;
		case '>':
		  tempstatement->statementarray[i]=2;
		  break;
		default: /* should be a literal */
		  tempstatement->statementarray[i]=string[j][places[i][0]]-'A' + 6;
		  break;
		  
		}
	    }
	}/*end loop to build tree */

      
      tempstatement=tempstatement->next;
    }/*end for loop to parse*/
  
  
  return rootbranch;
}/*end parser function*/

#endif

#include <iostream>
#include <string>
#include <cstdio>
#include "stdarg.h"
#include <math.h>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include "matrix.h"
#include <cmath>
#include <cfloat>

using namespace std;
          // Default Constructor
CMatrix :: CMatrix ()
{
	rowsNumbers = 0 ;
	coloumnsNumbers=0;
	matrix=NULL;
}
// (zero,unit and any initial value) matrix constructor
CMatrix :: CMatrix (const char* name,int rn , int cn , int initial , double initialvalue ) 
{
	strcpy(matrixName,name);
	this->rowsNumbers = rn ;
	this->coloumnsNumbers = cn ;
	if (rn*cn == 0){ matrix = NULL ;return;}
	matrix = new double* [rn];
	for(int i=0;i<rn;i++)
	{
		matrix[i]=new double[cn];
		for(int j=0;j<cn;j++)
		{
			switch (initial)
			{
				case zeros : 
				{
					matrix[i][j]=0;
					break;
				}
				case ones :
				{
					matrix[i][j]=1;
					break;
				}
				case value :
				{
					matrix[i][j] = initialvalue ; 
					break;
				}
			}
		}
	}
}

            ////string constructor  ----------------->
CMatrix :: CMatrix (string inputLine)
{

	string ssss = inputLine.substr(inputLine.find("[") , inputLine.find("]")-inputLine.find("[")+1);
	//cout << ssss << endl;
	int rows = 1, cols = 0;
	for(int i = 0,l =ssss.length(); i < l ; i++)
	{
	   if (inputLine[i] == ';')
	       rows++;
	}
	rowsNumbers = rows;
	matrix = new double* [rowsNumbers];
	string x = inputLine.substr(inputLine.find("[")+1,inputLine.find(";")-inputLine.find("[")-1);
	char* seps_c = " ,;=[]";
	char* context_c = NULL;
    char* token_c = NULL;
	char* rowString = new char [x.length()+1];;
	strcpy(rowString,x.c_str());
	token_c = strtok(rowString , seps_c);
	while(token_c)
	{
		 cols++;
		token_c = strtok(NULL , seps_c);
	}
	coloumnsNumbers = cols;
    char* stringToBeSplitted = new char [inputLine.length()+1];
	strcpy(stringToBeSplitted,inputLine.c_str());
	char* seps = " ,;=[]";
	char* context = NULL;
    char* token = NULL;
	token = strtok(stringToBeSplitted , seps);
	strcpy(matrixName,token);
	token = strtok(NULL,seps);
	for (int i =0;i< rowsNumbers;i++)
	{
		matrix[i] = new double [coloumnsNumbers];
		for (int j=0;j<coloumnsNumbers;j++)
		{
			double item = atof(token);
			matrix[i][j]=item;
			token = strtok(NULL,seps);
		}
	}
}
       // copy constructor 
CMatrix :: CMatrix (CMatrix& m)
{
	//strcpy(matrixName,m.matrixName);
	
	this->coloumnsNumbers = m.coloumnsNumbers;
	this->rowsNumbers = m.rowsNumbers ;
	matrix = new double*[rowsNumbers] ;
	for (int i = 0 ; i < rowsNumbers ; i++)
	{    
		matrix[i] = new double[coloumnsNumbers];
		for(int j=0;j < coloumnsNumbers;j++)
		{
			this->matrix[i][j]=m.matrix[i][j] ;
		}
	}
}
       // Destructor
CMatrix :: ~CMatrix (){

	if (this->matrix != NULL ) 
	{
		for(int i = 0 ; i < rowsNumbers ; i++)
			delete[] matrix[i];
		delete[] matrix ;
	}
}
         // Retun the Matrix in a string form
string CMatrix :: getstring (){
	string s = this->getName();
	s+=" = [";
	for (int i = 0; i < rowsNumbers ; i++)
	{
		for(int j = 0; j < coloumnsNumbers ; j++)
		{
			char buffer [1000] ;
			sprintf(buffer,"%g",matrix[i][j]);
			s+=buffer;
			if (j != coloumnsNumbers-1) 
				s+=" ";
		}
		if (i!=rowsNumbers-1)
			s+=" ; ";
	}
	s+="]";

return s ;
} 
      //Multiplacation
CMatrix& CMatrix :: operator* (CMatrix &x)
	{
		if(this->coloumnsNumbers==x.rowsNumbers )
			{
			CMatrix *rere = new CMatrix (this->matrixName,rowsNumbers,x.coloumnsNumbers,0,0);

				for (int rows=0;rows<rowsNumbers;rows++)
					{
						for (int cols=0;cols<x.coloumnsNumbers;cols++)
							{
								double sum=0;
								for (int i=0;i<coloumnsNumbers;i++)
									{
										sum+= this->matrix[rows][i]*x.matrix[i][cols];
									}
								rere->matrix[rows][cols]=sum;
							}
					}
				return *rere;
			}
		else
			{
				throw("Invalid matrix dimension please fix it");

			}
	}
          // Minus operator
CMatrix& CMatrix::operator - (CMatrix& m)
	{	
      if(rowsNumbers!=m.rowsNumbers||coloumnsNumbers!=m.coloumnsNumbers)
	  {
			throw("Invalid matrix dimension please fix it");

	  }
	else { CMatrix *r = new CMatrix (this->matrixName,rowsNumbers,coloumnsNumbers,0,0);
        for(int iR=0;iR<rowsNumbers;iR++)
            for(int iC=0;iC<coloumnsNumbers;iC++)
              {
                r->matrix[iR][iC] = this->matrix[iR][iC]- m.matrix[iR][iC];
              }
	return *r; 
	}
	//r=*this;
	}
        // plus operator
CMatrix& CMatrix::operator +(CMatrix& m)
	{	
      if(rowsNumbers!=m.rowsNumbers||coloumnsNumbers!=m.coloumnsNumbers)
	  {
			throw("Invalid matrix dimension please fix it");
	  }
         else {CMatrix *r = new CMatrix(this->matrixName,this->rowsNumbers,this->coloumnsNumbers,0,0);
        for(int iR=0;iR<rowsNumbers;iR++)
		{
			for(int iC=0;iC<coloumnsNumbers;iC++)
              {
                r->matrix[iR][iC] = this->matrix[iR][iC]+ m.matrix[iR][iC];
              }
		}
	return *r;}
	//r=*this;
	}
        //Transpose function
CMatrix& CMatrix::getTranspose()
	{
		//CMatrix r = *this;
		CMatrix *r = new CMatrix("",coloumnsNumbers,rowsNumbers,0,0);
		for(int iR=0;iR<rowsNumbers;iR++)
		{
			for(int iC=0;iC<coloumnsNumbers;iC++)
			{
				r->matrix[iC][iR]=matrix[iR][iC];
			}
		}
		return *r;

	}
ostream& operator << (ostream& out , CMatrix& m){
 
	 out << m.getstring();
 return out ;
 
 }

double CMatrix:: Determinant()
{
 	int counter = 1;
   	CMatrix r = *this ;
	for (int i=0;i<r.rowsNumbers;i++)                    //Pivotisation
		for (int k=i+1;k<r.coloumnsNumbers;k++)
            if (abs(r.matrix[i][i])<abs(r.matrix[k][i]))
			{   counter *= -1 ;
				for (int j=0;j<=r.rowsNumbers;j++)
                {
                    double temp=r.matrix[i][j];
                    r.matrix[i][j]=r.matrix[k][j];
                    r.matrix[k][j]=temp;
				}
			}
	
				for (int i=0 ; i<(r.coloumnsNumbers)-1 ; i++) 
				{       
						// checking for zero determinant
					 int zero_counter = 0 ;
					for (int l = 0 ; l < r.rowsNumbers ; l++)
					   if (r.matrix[l][l] == 0) zero_counter++ ;
						if (zero_counter > 1) return 0 ;
					for (int s = 0 ; s < r.rowsNumbers ; s++)
						{int counter = 0 ;
						for(int v = 0 ; v < r.coloumnsNumbers ; v++)
							if (r.matrix[s][v] == 0)
								counter++;
					if (counter == r.coloumnsNumbers) return 0 ;
						}
					
						//loop to perform the gauss elimination
					for (int k=i+1 ; k < r.coloumnsNumbers ; k++)
               				 {
               					 double t = r.matrix[k][i] / r.matrix[i][i];
						for (int j=0 ; j < r.coloumnsNumbers  ; j++)
							r.matrix[k][j] = r.matrix[k][j]- t*r.matrix[i][j];    
//make the elements below the pivot elements equal to zero or elimnate the variables
					}
				}
	double deter = 1 ;
	for (int i = 0 ; i < rowsNumbers ; i++)
		deter *= r.matrix[i][i];
	//cout << deter*counter << endl;
	return deter*counter ;	
}
CMatrix& CMatrix:: GetCofactor()  
{
	CMatrix *cofactor = new CMatrix("COF", rowsNumbers, coloumnsNumbers,1,1);
	if (rowsNumbers != coloumnsNumbers) 
		return *cofactor; 
	if (rowsNumbers < 2)   
		return *cofactor;   
	else if (rowsNumbers == 2)    
	{        
		cofactor->matrix[0][0] = matrix[1][1];   
		cofactor->matrix[0][1] = -matrix[1][0];       
		cofactor->matrix[1][0] = -matrix[0][1];    
		cofactor->matrix[1][1] = matrix[0][0];    
		return *cofactor; 
	}  
	else if (rowsNumbers >= 3)      
	{       
		int DIM = rowsNumbers ;     
		CMatrix ***temp = new CMatrix**[DIM];    
		for (int i = 0; i < DIM; i++)   
			temp[i] = new CMatrix*[DIM];   
		for (int i = 0; i < DIM; i++)     
			for (int j = 0; j < DIM; j++)   
				temp[i][j] = new CMatrix("", DIM - 1, DIM - 1,1,1); 
     
		for (int k1 = 0; k1 < DIM; k1++)     
		{              
			for (int k2 = 0; k2 < DIM; k2++)        
			{                        

				int i1 = 0;                       
				for (int i = 0; i < DIM; i++)                        
				{     
					int j1 = 0;      
					for (int j = 0; j < DIM; j++)    
					{      
						if (k1 == i || k2 == j)   
							continue; 
						temp[k1][k2]->matrix[i1][j1++]
						= this->matrix[i][j];    
					}         
					if (k1 != i)    
						i1++;       
				}       
			}    
		}  
////***********************************************************************////////////////
		bool flagPositive = true;    
		for (int k1 = 0; k1 < DIM; k1++)      
		{       
			cout << "end of function " << endl;   
			flagPositive = ((k1 % 2) == 0);    
			for (int k2 = 0; k2 < DIM; k2++)      
			{   

				if (flagPositive == true)      
				{  
					cout << "k1 = " << k1 << "  &&  k2 = " << k2 << endl;                            
					cofactor->matrix[k1][k2] = temp[k1][k2]->Determinant();    
					flagPositive = false;    
				}          
				else     
				{     
					cout << "k1 = " << k1 << "  &&  k2 = " << k2 << endl;                            
					cofactor->matrix[k1][k2] = -temp[k1][k2]->Determinant();
					flagPositive = true; 
				}     
			}     
		}  
////***********************************************************************////////////////
		cout << "here done" << endl;
		for (int i = 0; i < DIM; i++)           
			for (int j = 0; j < DIM; j++)   
				delete temp[i][j];     
		for (int i = 0; i < DIM; i++)  
			delete[] temp[i];   
		delete[] temp;    
	}     
	cout << *cofactor << endl;
	return *cofactor;
}
 CMatrix& CMatrix:: Inverse()  
 {
 
	int i,j,k,n;
   double a[100][10000],t;
		n = this->rowsNumbers;
	   for(i=0;i<n;i++)
	      for(j=0;j<n;j++)
		 a[i][j] = this->matrix[i][j];

	   for(i=0;i<n;i++)
	   {
	      for(j=n;j<2*n;j++)
	      {
		  if(i==j-n)
		     a[i][j]=1;
		 else
		     a[i][j]=0;
	       }
	   }
	   for(i=0;i<n;i++)
	   {
	      t=a[i][i];
	      for(j=i;j<2*n;j++)
		  a[i][j]=a[i][j]/t;
	      for(j=0;j<n;j++)
	      {
		 if(i!=j)
		 {
		    t=a[j][i];
		    for(k=0;k<2*n;k++)
		        a[j][k]=a[j][k]-t*a[i][k];
		  }
	      }
	   }

	
	CMatrix *temp = new CMatrix ("temp", rowsNumbers,coloumnsNumbers,1,1);
	 for(i=0;i<n;i++)
	 {
		int nonusedindex = 0;
		 for(j=n;j<2*n;j++)
		{
		
		 temp->matrix[i][nonusedindex]=a[i][j];
		nonusedindex++;
		}
	 }
	//cout << *temp ;
	return *temp;
 }    
 
 void CMatrix:: setName(string name)
 {
	 strcpy(matrixName,name.c_str());
 }
 string CMatrix:: getName()
 {
	string s = "";
	for (int i = 0; i <128;i++)
	{
			if(matrixName[i] == NULL) break;
			else s += matrixName[i];
	}
	return s;
 }
 
CMatrix& CMatrix:: operator = (const CMatrix &m)
 {
	//strcpy(matrixName,m.matrixName);
	
	this->coloumnsNumbers = m.coloumnsNumbers;
	this->rowsNumbers = m.rowsNumbers ;
	matrix = new double*[rowsNumbers] ;
	for (int i = 0 ; i < this->rowsNumbers ; i++)
	{    
		matrix[i] = new double[this->coloumnsNumbers];
		for(int j=0;j < this->coloumnsNumbers;j++)
		{
			this->matrix[i][j]=m.matrix[i][j] ;
		}
	}
	return *this;
}
CMatrix& CMatrix:: operator / ( CMatrix& m)
{
	CMatrix* r = new CMatrix("",this->rowsNumbers,m.coloumnsNumbers,0,0);
	if ((m.rowsNumbers == m.coloumnsNumbers) )
	{	
		double det = m.Determinant(); 
                   if(det == 0/*!isnormal(det)*/)
		 {    
			 throw("Determinant is equal zero division is invalid");
		 }
		 else
		{	
			*r = *this * m.Inverse();
			return *r;
		}
	}
	else throw("Invalid matrix dimension please fix it");
}
 CMatrix& elementDivision(CMatrix &m,CMatrix &c)
{
	
	if(m.rowsNumbers!=c.rowsNumbers||m.coloumnsNumbers!=c.coloumnsNumbers)
	{
		
		throw("Invalid matrix dimension please fix it");
		
	}
	else
	{
		CMatrix *r=new CMatrix("",m.rowsNumbers,m.coloumnsNumbers,0,0);
		
		for(int ir=0;ir<m.rowsNumbers;ir++)
		{
			for(int ic=0;ic<m.coloumnsNumbers;ic++)
			{
			r->matrix[ir][ic]=m.matrix[ir][ic]/c.matrix[ir][ic];

			}
		}
	return *r;
	}
	
}
CMatrix& elementDivision(float m,CMatrix &c)
{
	
	
		CMatrix *r=new CMatrix("",c.rowsNumbers,c.coloumnsNumbers,0,0);
		
		for(int ir=0;ir<c.rowsNumbers;ir++)
		{
			for(int ic=0;ic<c.coloumnsNumbers;ic++)
			{
			r->matrix[ir][ic]=m/c.matrix[ir][ic];

			}
		}
	return *r;
	
	
}
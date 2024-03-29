#define _CRT_SECURE_NO_WARNINGS
#include "matlab.h"
#include "matrix.h"
#include <math.h>

Matlab::Matlab()
{
	matricesNumber = 0;
	
}

void Matlab::shift_array(double a[], int s, int shift_index) // s ==> size of array
{
	for (int i = shift_index; i<s; i++)
		a[i] = a[i + 1];
}

void Matlab::shift_array(string a[], int s, int shift_index) // s ==> size of array
{
	for (int i = shift_index;i<s;i++)
		a[i] = a[i + 1];
}
void Matlab::shift_array(char a[], int s, int shift_index) // s ==> size of array
{
	for (int i = shift_index;i<s;i++)
		a[i] = a[i + 1];
}
void Matlab::shift_array(CMatrix a[], int s, int shift_index) // s ==> size of array
{
	for (int i = shift_index; i<s; i++)
		a[i] = a[i + 1];
}
bool Matlab:: isNumber(string s)
{
	bool isNum = 0;
	int c = 0;
	bool num ;
	for (int i = 0;i <s.length();i++)
	{ 
	     num = isdigit(s[i])  ;
		if ( num == 1 || s[i] == '-' || s[i] == '.' )
			c++;
	}
	
	if (c == s.length())
		isNum = 1;
	return isNum;
}

int Matlab::needconcat(string s)
{
	int n=0;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='[')
		n++;
	}
	return n;
}


int Matlab::needExtract(string s)
{
	int n=0;
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='(')
		n++;
	}
	return n;
}

//========================================================= calculate matrix expression =================================================
#define Shift shift_array(operands_m, n, i + 1);n--;shift_array(operators_e, r, i);r--;
CMatrix Matlab:: calculate_MExpression(string operands[], int n, string operators_e[], int r)
{
	
	CMatrix operands_m[100];
	int matrix_1x1[50];
	int count = 0; 
	for (int i = 0;i < n;i++)
	{
		if (isNumber(operands[i]))
		{ 
			operands_m[i] = CMatrix(stod(operands[i]));
			matrix_1x1[count] = i;
			count++;
		}

		else
		{
		
			for (int j = 0;j <matricesNumber;j++)
			{
				if (matrices[j]->getName() == operands[i])
				{
					operands_m[i] = *matrices[j];
				}
			}
		
		}
	}

	int end = 0;
	while (end == 0)
	{
		end = 1;
		for (int i = 0;i < r;i++)
		{
			if (operators_e[i] == "^")
			{ operands_m[i] = operands_m[i] ^ operands_m[i + 1];Shift end = 0; }

			else if (operators_e[i] == ",^")
			{
				operands_m[i]= elementoperations(operands_m[i], operands_m[i + 1], '^', 0);
				Shift end = 0;
			}
		}
	}
	end = 0;
	while (end == 0)
	{
		end = 1;
		for (int i = 0;i < r;i++)
		{
			// Division
			if (operators_e[i] == "/")
			{ operands_m[i] = operands_m[i] / operands_m[i + 1]; Shift end = 0; }

			else if (operators_e[i] == ",/")
			{
				if(count==0)
				{
						operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '/',2); 
				}

				for (int j = 0;j < count;j++)
				{
					
					if (matrix_1x1[j] == i && matrix_1x1[j] != i + 1)
					{
						operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '/',1); //const/matrix
						Shift end = 0;
					}

					 if (matrix_1x1[j] == i + 1 && matrix_1x1[j] != i )
					{
						operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '/', 0); // matrix/const
						Shift end = 0;
					}
								
					if((matrix_1x1[j]) !=(i) && (matrix_1x1[j]) != (i+1))
				
					{
						operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '/',2); 
						Shift end = 0;
					}
					 
				
				}
				
			}


			// Multiplication
			else if (operators_e[i] == "*")
			{ operands_m[i] = operands_m[i] * operands_m[i + 1]; Shift end = 0; }

			else if (operators_e[i] == ",*")
			{
				operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1],'*',0); 
				Shift end = 0;
			}

		}
	}
	end = 0;
	while (end == 0)
	{
		end = 1;
		for (int i = 0;i < r;i++)
		{
			//Addition
			if (operators_e[i] == "+")
			{
				operands_m[i] = operands_m[i] + operands_m[i + 1]; Shift end = 0;
			}
			 
			else if (operators_e[i] == ",+")
			{
				operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '+', 0); 
				Shift end = 0;
			}

			//Subtraction
			else if (operators_e[i] == "-")
			{
				operands_m[i] = operands_m[i] - operands_m[i + 1]; Shift end = 0;
			}

			else if (operators_e[i] == ",-")
			{
				operands_m[i] = elementoperations(operands_m[i], operands_m[i + 1], '-', 0);
				Shift end = 0;
			}
		}
	}
	
	return operands_m[0];
	
}


//======================================================================================================
static int x = 0;
 void Matlab:: Extract (string exp, int pos, string& result, int& new_pos,int& flaag)
{
	string s="res" ; // Matrix Name
	string s2 ;
	flaag=0;
	new_pos = exp.find(")", pos) + 1;
	while (pos != string::npos)
	{
		
		for (int i = exp.find("(", pos) + 1; i < exp.find(")", pos); i++)
			s2 += exp[i];//s2 what inside the ()
		string operators = "*-+/^%";  int flagay=0; 
		for (int i=0;i<s2.length();i++)
		{
			for (int j=0;j<operators.length();j++)
			{
		      if(s2[i]==operators[j])
		       {
				   flagay=1;
		       }
			}
		}	
		if(flagay==0)
		{
			result=s2;
			return;
		}
		int new_pos2 = exp.find(")", pos);
		string operands[100], operators_e[50];
		int n, r;string name;
	    bool  matrices_expression=0;
		Split_Expression(s2, operands, n, operators_e, r, matrices_expression);

		if (matrices_expression)
		{
			matrices[matricesNumber] = new CMatrix();
			*matrices[matricesNumber] = calculate_MExpression(operands, n, operators_e, r);
			string matrix_Name =s+ to_string(x);  x++;
			matrices[matricesNumber]->setName(matrix_Name);  matricesNumber++;
			result = matrix_Name;
		}
		else
		{
		result = to_string(calculate_rExpression(operands, n, operators_e, r,flaag));
		s2.replace(0, new_pos2 - pos, result);
		}
		pos = s2.rfind("(");
				
	}
	
}


 void Matlab::special_Extract(string s1,string& result,int& flag)
{
	string operators = "*-+/^%";  
	string ss;
    ss='('+s1+')';

	int new_pos;
	result=ss;
	int pos = ss.rfind("(");
	
	while(!isNumber(ss))
	{
		 int flagay=0;
		 
	 if (pos != string::npos)
	    {
		Extract(ss, pos, result, new_pos,flag);
		ss.replace(pos, new_pos - pos, result);
		pos = ss.rfind("(")	;

		}	
	  for (int i=0;i<ss.length();i++)
		{
		    for (int j=0;j<operators.length();j++)
			{
		      if(ss[i]==operators[j])
				   flagay=1;
			}
		}
		if(flagay==0)
		{break;}

	}
 }
	
 
 //======================================================================================================
void Matlab::Split_Expression(string expression, string operands[], int& n, string operators_e[],int& r, bool& matrices_expression)
{
	string operators = "*-/^%+";
	char* sep = " =+-*/^%;,";
	char* token = NULL;
	n = 0; //operands Index
    r = 0; //operators Index

	if (expression[0] == '-')
		expression[0] = '$';

	for (int i = 1;i < expression.length();i++)
	{
		if (expression[i] == '-')
		{
			for (int j = 0;j < operators.length();j++)
				if (expression[i - 1] == operators[j])
					expression[i] = '$';
		}
	}

	for (int i = 0;i < expression.length();i++)
	{
		if (expression[i] == '.')
		{
			for (int j = 0;j < operators.length();j++)
			{
				if (expression[i + 1] == operators[j])
				{
					expression[i] = ',';
					operators_e[r] = expression[i];
					operators_e[r] += expression[i + 1];
					r++;
					i++;
					break;
				}
			}
		}

		else
		{
			for (int j = 0;j < operators.length();j++)
			{
				if (expression[i] == operators[j])
				{
					operators_e[r] = expression[i];
					r++;
					break;
				}
			}
		}
	}
	// filling the array of operands
	char* LineToBeSplitted = new char[expression.length() + 1];
	strcpy(LineToBeSplitted, expression.c_str());
	token = strtok(LineToBeSplitted, sep);

	 matrices_expression = 0;
	while (token != NULL)
	{
		if (token[0] == '$')
			token[0] = '-';

		if (isNumber(token) == 0) //Operations on matrices
			matrices_expression = 1;

		operands[n] = token;
		n++;
		token = strtok(NULL, sep);
	}
}		



//=================================================== Retun the Matrix in a string form ===================================================
string Matlab:: getstring_arr(string** arr,int row,int col) 
{
	double** arr1=new double*[row];
	for (int i = 0; i < row; i++)
	{
		arr1[i]=new double [col];
		for (int j = 0; j < col; j++)
		{
			arr1[i][j]=stod(arr[i][j]);	
		}
	}
	string s ;
	s='[';
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			char buffer[100];
			sprintf(buffer, "%g", arr1[i][j]);
			s += buffer;
			if (j != col - 1)
				s += " ";
		}
		if (i != row - 1)
			s += " ; ";
	}
	s+=']';
	return s;
}


//====================================================== set a string in form of a 2D array ====================================================================/
string**Matlab::  SetArray( string inputLine,int& rows,int& cols,int& flaag)
{
	int colls=0;
	if (inputLine[0]!='[')
	{
		 inputLine='['+inputLine+']';
	}
	
	string ss = inputLine.substr(inputLine.find("[")+1, inputLine.find("]") - inputLine.find("[")-1 );
	
	 rows = 1; cols = 0;
	for(int i = 0;i<ss.length();i++)
	{
		if (inputLine[i] == '$')
			rows++;
	}
	string**array= new string*[rows];
	string x = inputLine.substr(inputLine.find("[") + 1, inputLine.find("$") - inputLine.find("[") - 1);
	
	char * sep= " ";
	char* token= NULL;
	char* rowString = new char[x.length() + 1];;
	strcpy(rowString, x.c_str());
	token= strtok(rowString,sep);
	
	while (token!=NULL)
	{
		cols++;
		token = strtok(NULL,sep);
	}

	 flaag=0;
	string y = inputLine.substr(inputLine.find("$") + 1, inputLine.find("$",inputLine.find("$") + 1) - inputLine.find("$") - 1);
	
	char * sepp= " ";
	char* tokennn= NULL;
	char* rowStringg = new char[y.length() + 1];
	strcpy(rowStringg, y.c_str());
	tokennn= strtok(rowStringg,sepp);
	
	while (tokennn!=NULL)
	{
		colls++;
		tokennn = strtok(NULL,sep);
	}
	if(cols!=colls)
	{
		flaag=1;
	}

	if(!flaag)
	{
	char* seps = " $";
	char* tokenn = NULL;
	char* T = new char[ss.length()+1]; 
	strcpy(T,ss.c_str());
	tokenn = strtok(T, seps);
	
	for(int i=0;i< rows;i++)
	{
		array[i] = new string[cols];
	}
	for(int i=0;i< rows;i++)
	{
		for (int j=0;j<cols;j++)
		{
			array[i][j] = tokenn;
			tokenn = strtok(NULL, seps);
		}
		
	}
	}
	
	return array;
}
//==================================================== concatenation col==========================================================
string**  Matlab::CatCol( string** arr1,int arr1R,int arr1C,string**arr2,int arr2R,int arr2C ,int &rows,int& cols)
{ 
	
	 cols=arr1C+arr2C;rows=arr1R;
	if(arr1R==arr2R)
	{
		rows=arr1R;
		string** CatArr=new string*[arr1R];
	    for (int i=0;i<arr1R;i++)
	   {
		CatArr[i]=new string [cols];
	   }
	   for (int i=0;i<arr1R;i++)//filling the big array with first array
	     {
		for(int j=0;j<arr1C;j++)
			CatArr[i][j]=arr1[i][j];

	     }
	for (int i=0;i<arr1R;i++)//filling the big array with second array
	{
		for(int j=arr1C;j<cols;j++)
			CatArr[i][j]=arr2[i][j-arr1C];
	}
	return CatArr;
	}
	
}
//==========================================  concatenation row ================================================================
string** Matlab:: CatRow( string** arr1,int arr1R,int arr1C,string** arr2,int arr2R,int arr2C ,int& rows,int& cols)
{
	 rows=arr1R+arr2R;
	if(arr1C==arr2C)
	{
		cols=arr1C;
		string** CatArr=new string*[rows];
	    for (int i=0;i<rows;i++)
	   {
		CatArr[i]=new string [arr1C];
	   }
	   for (int i=0;i<arr1R;i++)//filling the big array with first array
	     {
		for(int j=0;j<arr1C;j++)
			CatArr[i][j]=arr1[i][j];

	     }
	for (int i=arr1R;i<rows;i++)//filling the big array with second array
	{
		for(int j=0;j<arr1C;j++)
			CatArr[i][j]=arr2[i-arr1R][j];

	}
	return CatArr;
	}
}

//============================================================ to delete spaces ============================================
string Matlab::delete_spaces (string s,int& new_size)
{
	  new_size=s.length();
	 for(int i=0;i<new_size;i++)
	 {
		 char* e=NULL;
		 int in=0;
	 
		 if((s.find(' ',i)==i+1&&( s.find(',',i-1)==i||s.find(';',i-1)==i)))
			 {
				 for (int j = i+1; j<new_size; j++)
		          s[j] = s[j + 1];
	              i--; new_size--;
		     }
		 if(s.rfind(' ',i+1)==i&& (s.find(',',i)==i+1||s.find(';',i)==i+1))
		 {
			 for (int j = i; j<new_size; j++)
		          s[j] = s[j + 1];
	              i-=2; new_size--;
		 }
	 }
	 for(int i=0;i<new_size;i++)
	 {
		  if(s.find(' ',i)==i+1 && s.find(']',i)==i && s.find('[',i)==i+2)
			 {
		          s[i+1] = ',';
		     }

	 }
	 return s;
}


//=========================================================== calculate regular Expression ===============================================

#define Shift shift_array(operands_d, n, i + 1);n--;shift_array(operators_e, r, i);r--;
 double Matlab:: calculate_rExpression(string operands[],int n,string operators_e[],int r,int& flag) //regular Expression
{
	double operands_d[30];
	flag=0;
	for (int i = 0;i < n;i++)
		operands_d[i] = stod(operands[i].c_str());
	
	int end = 0;
	while (end == 0)
	{
		end = 1;
		for (int i = 0;i < r;i++)
		{
			if (operators_e[i] == "^")
			{operands_d[i] = pow(operands_d[i], operands_d[i + 1]);Shift end = 0;}
		}
	}
	end = 0; 
	while (end == 0)
	{
		end = 1;
		for (int i = 0;i < r;i++)
		{
			if (operators_e[i] == "/")
			{ 

					if(operands_d[i + 1]!=0)
					{
				    operands_d[i] = operands_d[i] / operands_d[i + 1];		
					Shift end = 0; 
					}
					else{
						
						flag=1;
					}
				}			
			
			
			else if (operators_e[i] == "*")
			{ operands_d[i] = operands_d[i] * operands_d[i + 1]; Shift end = 0; }

			else if (operators_e[i] == "%")
			{ operands_d[i] = int(operands_d[i]) % int(operands_d[i + 1]); Shift end = 0; }
		}
	}	
	end = 0;
	while (end == 0)
	{
		end = 1 ;
		for (int i = 0;i < r;i++)
		{
			if (operators_e[i] == "+")
			{ operands_d[i] = operands_d[i] + operands_d[i + 1]; Shift end = 0; }
			
			else if (operators_e[i] == "-")
			{ operands_d[i] = operands_d[i] - operands_d[i + 1]; Shift end = 0; }
		}
	}
	return operands_d[0];
}




 //check for expression and solve it 
void Matlab:: Cal_Array_Expression(string **arr,int row,int col)
 {
	 int fll=0;
	 int fl=0;
    for(int j =0;j<row;j++)
	  for(int k =0;k<col;k++)
   {
	 int flag=0;
	 flag=isNumber(arr[j][k]);
	 if(!flag)
	{
	//Trigonometric functions
			for (int i = 0;i < arr[j][k].length();i++)
			{
				
				 int pos = 0;
			     string arg;
			     string result;

		      if (arr[j][k][i] == 's' && arr[j][k][i+1] == 'i' && arr[j][k][i + 2] == 'n') //sin
				{
					pos = i;
					Extract(arr[j][k], i + 3, arg, i,fll);
					arr[j][k].replace(pos, i - pos, to_string((sin(atof(arg.c_str())))));
				}

				else if (arr[j][k][i] == 'c' && arr[j][k][i + 1] == 'o' && arr[j][k][i + 2] == 'n') //cos
				{
					pos = i;
					Extract(arr[j][k], i + 3, arg, i,fll);
					arr[j][k].replace(pos, i - pos, to_string((cos(atof(arg.c_str())))));
				}

				else if (arr[j][k][i] == 't' && arr[j][k][i + 1] == 'a' && arr[j][k][i + 2] == 'n') //tan
				{
					pos = i;
					Extract(arr[j][k], i + 3, arg, i,fll);
					arr[j][k].replace(pos, i - pos, to_string((tan(atof(arg.c_str())))));
				}

				else if (arr[j][k][i] == 's' && arr[j][k][i + 1] == 'q' && arr[j][k][i + 2] == 'r' && arr[j][k][i + 3] == 'r') //sqrt
				{
					pos = i;
					Extract(arr[j][k], i + 4, arg, i,fll);
					arr[j][k].replace(pos, i - pos, to_string((sqrt(atof(arg.c_str())))));
				}

			}

					
					if(!needExtract(arr[j][k]))
					{
						string operands[100],operators_e[100];
					    int n=0,r=0;bool matrices_expression=0;
					    Split_Expression( arr[j][k], operands, n, operators_e, r, matrices_expression);
					    arr[j][k]=to_string(calculate_rExpression(operands, n, operators_e,r,fl)) ;
					}
					else
						 special_Extract(arr[j][k], arr[j][k],fll) ;
				}
			}

 }


//========================= concatenation func ========================================================
string Matlab::concatenation(string extract)
 {

	 string output,input;
	 int size;
	 string p="\r\n";
	 extract= extract.substr(extract.find("[") + 1, extract.rfind("]") - extract.find("[") - 1);
	 input=delete_spaces (extract, size);
	 for(int i=0;i<size;i++)
	 {
		 string s ;
		 char* e=NULL;
	 if(input[i]=='[')
	 {
		 e=&input[i];
		 s=e;
		 for(int j=0 ;j<s.find(']');j++)
		 {

			 if(s[j]==';')
				 input[i+j]='$';
		 }
	 }
	 }
	 string compare[20];
	 int compcounter=0;

	  for(int i=0;i<size;i++)
	  {
		  string a,d;
		 char* comp=NULL;
		 char* ss=NULL;
	     string f;
	 if((input[i]==','&&input[i+1]=='[') || (input[i]==' '&&input[i+1]=='['))
	   {
		   comp=&input[i];
		   d=comp;
		   f=input.substr(0,i);
		   compare[compcounter]=f.substr(f.rfind('[')+1,i-f.rfind('[')-2);
		   compare[compcounter+1]=d.substr(2,d.find(']')-2);
		   compcounter+=2;
		   
	   }
	  }
	 //================== to delete repeated =========================
	 for(int y=0;y<compcounter;y+=2)
	 {
		 for(int i=y;i<compcounter-2;i+=2)
		 {
		    if(!compare[y].compare(compare[i+2])&&!compare[y+1].compare(compare[i+3]))
			{
			 compare[i+2]=compare[i+3]=" ";
			}
		 }
	 }
	 
	  char* c= new char[size+1];
	  strcpy(c,input.c_str());
	  string arr[20];
	  string** rturn[20];
	  int co =0;// local rturn size 
	  int row[20]={0} , col[20]={0};
	  
	  string ** catcolout[20];
      char*seps="\r\n;,[]";
	  char*token = NULL;
	  token=strtok(c,seps);
	  int flaag=0;

	while (token)
	{
		arr[co]=token;
		co++;
		token=strtok(NULL,seps);
	}

	for(int i =0;i<co;i++)
	{
		rturn[i]=SetArray(arr[i],row[i],col[i],flaag);
		
	}
	for(int i =0;i<co;i++)
	{
		Cal_Array_Expression(rturn[i], row[i], col[i]);
		
	}

	
	//==================== concatenation ccoloum =================================
	int outcount=0,outcatrows[20]={0},outcatcols[20]={0};
	for (int i=0;i<compcounter;i+=2)
	{
		for (int j=0;j<co;j++)
		{
			if (!compare[i].compare(arr[j]) && !compare[i+1].compare(arr[j+1]))
			{
				catcolout[outcount] = CatCol(rturn[j],row[j],col[j],rturn[j+1],row[j+1],col[j+1],outcatrows[outcount],outcatcols[outcount]);
				outcount++;
			}
		}
	}


	
	
	  //=============== concatenation row =============================
	int index=0,conditionIndex1=0,conditionIndex2=0;
	string **catrow[20];
	int catrows[20]={0},catcols[20]={0};
	
	for (int i=0;i<size;i++)
	{
		if ( (input[i]=='['&& i==0)|| (input[i]==';' && input[i+1]=='['))
		{
		    catrow[index]=catcolout[conditionIndex1];
			catrows[index]=outcatrows[conditionIndex1];
			catcols[index]=outcatcols[conditionIndex1];
			conditionIndex1++;
		    conditionIndex2 += 2;
			index++;
		}
	     if ((input[i]!='['&&i==0 )|| (input[i]==';'&& input[i+1]!='[')|| (input[input.find('\r\n')+2]!='['&& input.find('\r\n',i-1)==i)) 
		{		
	        catrow[index]=rturn[conditionIndex2];
		    catrows[index]=row[conditionIndex2];
		    catcols[index]=col[conditionIndex2];
		    conditionIndex2++;
		    index++;		   
		}
	}

	if(index>1)
	{
	for (int j=0;j<index;j++)
	{
		catrow[j+1]= CatRow(catrow[j],catrows[j],catcols[j],catrow[j+1],catrows[j+1],catcols[j+1],catrows[j+1],catcols[j+1]);
	}
	}
	output=getstring_arr(catrow[index-1],catrows[index-1],catcols[index-1]);
	return output;
	
}


//==================================================================================================================================================
//get string 
string Matlab::GetStringByName(string name)
{
	string s;
	for (int j = 0;j < matricesNumber;j++)
	{ 
				if (matrices[j]->getName() == name)
				{
					s=matrices[j]->getstring();
			
				}
			}
	return s;

}

//=================================================================================================================================================


void  Matlab::CheckAndGet_matrix_instring(string &l)
{
	string s,k;
	string g;
	k=l;
	int h=0 ,j=0;
	string matrix_in_string;
	for(int i=l.find('[');i<l.length();i++)
	{
		
		//check if there is matrix name //  '['=91 ']'=93
		if (l[i]>64 && l[i]!=91 && l[i]!=93 && l[i]!=94 && l[i]!=92)
		{
			j++;
			if(l[i+1]>64 &&l[i+2]>64)
			{
				i+=3;
				continue;
			}
			h=1;
			g=l[i];
		   if(l.rfind(';',i)==i-1 ||l.rfind(';',i)==i-2|| ((l.rfind(' ',i)==i-1 ||l.rfind(',',i)==i-2)&&l.find(';',i)==i+1)||((l.rfind(' ',i)==i-1 ||l.rfind(',',i)==i-2)&&l.find(';',i)==i+2))
		   {
			  
			  s = GetStringByName(g);
			  matrix_in_string=s.substr(s.find('[')+1,s.rfind(']')-s.find('[')-1);
			  l.replace(i,1,matrix_in_string);
			
			   
		   }
		
		   else if(l.rfind(';',i)!=i-1 ||l.rfind(';',i)!=i-2|| ((l.rfind(' ',i)==i-1 ||l.rfind(',',i)==i-2)&&l.find(';',i)!=i+1)||((l.rfind(' ',i)==i-1 ||l.rfind(',',i)==i-2)&&l.find(';',i)!=i+2)) 
		   {
			 s = GetStringByName(g);
			 matrix_in_string=s.substr(s.find('['),s.rfind(']')-s.find('[')+1);
			 l.replace(i,1,matrix_in_string);	  
		   }   
		}       
	}
	      if(h==0)
		  {  l=k;  }
		  
}

static int y=0;
// Load File
void Matlab::loadfile(const char* File)

{
	ifstream file(File);
	while (!file.eof())
	{
		string inputLine;
		getline(file, inputLine);
		if(inputLine.length()<1)
			break;
		//cout<< inputLine<<endl;
		int length = inputLine.length();
		int counter=0;
		for(int i=0;i<length;i++)
		{
			
			if(inputLine[i]=='[')
				counter++;
			if(inputLine[i]==']')
				counter--;
		}
		string in =inputLine;
		while(counter)
		{
			
			getline(file, inputLine);
			for(int i=0;i<inputLine.length();i++)
			{
			if(inputLine[i]=='[')
				counter++;
			if(inputLine[i]==']')
				counter--;
			}
			
			inputLine=in+';'+inputLine;
		}
		
		    int equalCounter=0;
			for(int i=0;i<inputLine.length();i++)
				if(inputLine[i]=='=')
				{	
				   equalCounter++;
				   break;
				}

				char* seps = " =";
				char* token = NULL;
				char* LineToBeExecuted = new char [inputLine.length()+1];
				strcpy(LineToBeExecuted,inputLine.c_str());
				token = strtok(LineToBeExecuted , seps);
				string name=token;
				if(equalCounter==0)
				{
					for (int i=0;i<matricesNumber;i++)
					{
						if(matrices[i]->getName()==name)
						{
				           	cout<<matrices[i]->getstring_();
						}
					}
				}
		// A = [];
	 if(equalCounter==1)
	 {

		if (inputLine.find("[")>0 && inputLine.find("[")<length)
			{
			  CheckAndGet_matrix_instring(inputLine);
				char* sep = " =;	";
				char* tokenn = NULL;
				char* Line = new char [inputLine.length()+1];
				strcpy(Line,inputLine.c_str());
				tokenn = strtok(Line , sep);
				
				for (int i=0;i<matricesNumber;i++)
				{
					if (tokenn == matrices[i]->getName())   
					{
						matrices[i] = new CMatrix (inputLine);
						break;
					}
				}
				
					int ifcat=needconcat(inputLine);
					if(ifcat==1)
					{
						for(int y=0;y<inputLine.length()-1;y++)
						{
							if(inputLine[y]==';')
								inputLine[y]='$';
						}
						
						int flaag=0;
						string name_=inputLine.substr(0 , inputLine.find("[")-1);
                        string input_= inputLine.substr(inputLine.find("[")+1 , inputLine.rfind("]") - inputLine.find("[")-1);
						int row=0,col=0;
						//check for expression and solve it 
						string ** Array=SetArray(input_,row,col,flaag);
						try
						{
							if(flaag==1)
							{
								throw(input_);
							}
						}
						catch(string dd)
						{
							cout<<"vertical dimensions mismatch"<<endl;
							continue;
						}
						Cal_Array_Expression(Array,row, col); //if there is an exp inside the line,calc it
						inputLine=name_+getstring_arr(Array,row, col);

						matrices[matricesNumber] = new CMatrix(inputLine);
			             
			            if(inputLine[inputLine.length()-1] != ';')
				        cout << matrices[matricesNumber]->getstring_() << endl;
						matricesNumber++;
					}
					if(ifcat>1)
					{
					    
						string name_=inputLine.substr(0 , inputLine.find("[")-1);
						string input_= inputLine.substr(inputLine.find("[") , inputLine.rfind("]") - inputLine.find("[")+1);
						string newinput = concatenation(input_);
						matrices[matricesNumber] = new CMatrix(name_+newinput);			           
						if(inputLine[inputLine.length()-1] != ';')
				            cout << matrices[matricesNumber]->getstring_() << endl;
						 matricesNumber++;
					}
				}
	 
		
		
		//////////////////////////////////////////////////////   operations  //////////////////////////////////////////////////////////
		else if (inputLine.find("[") == string::npos )
		{ 
			int counter=0;
			for(int i=0;i<inputLine.length();i++)
		{		
			if(inputLine[i]=='(')
				counter++;
			if(inputLine[i]==')')
				counter--;
		}
			try
			{
				if(counter!=0)
				{
					throw(inputLine);
				}
			}
                catch(string ff){
				cout<<"parse error while reading file"<<endl;
				continue;

			}
			
				if (inputLine.find("rand") != string::npos)
			   {  
				string s = inputLine.substr(inputLine.find("(")+1 , inputLine.rfind(")") - inputLine.find("("));
				char* sepp = " ,";
				char* tokenns = NULL;
				char* Line_ = new char [s.length()+1];
				strcpy(Line_,s.c_str());
				tokenns = strtok(Line_ , sepp);
				char * IRandIC[2];
				for(int i=0;i<2;i++)
				 {
				   IRandIC[i]=tokenns;
				   tokenns=strtok(NULL,sepp);
				 }
				int rows=atoi(IRandIC[0]);
				int cols=atoi(IRandIC[1]);
				matrices[matricesNumber]=new CMatrix(name.c_str(),rows,cols,3,0);
				if(inputLine[inputLine.length()-1] != ';')
				    cout << matrices[matricesNumber]->getstring_() << endl;
				matricesNumber++;
				continue;
			}
			else if (inputLine.find("eye") != string::npos)
			{  
				string s = inputLine.substr(inputLine.find("(")+1 , inputLine.rfind(")") - inputLine.find("("));
				char* sepp = " ,";
				char* tokenns = NULL;
				char* Line_ = new char [s.length()+1];
				strcpy(Line_,s.c_str());
				tokenns = strtok(Line_ , sepp);
				char * IRandIC[2];
				for(int i=0;i<2;i++)
				{
				   IRandIC[i]=tokenns;
				   tokenns=strtok(NULL,sepp);
				}
				int rows=atoi(IRandIC[0]);
				int cols=atoi(IRandIC[1]);
				matrices[matricesNumber]=new CMatrix(name.c_str(),rows,cols,2,0);
				if(inputLine[inputLine.length()-1] != ';')
				    cout << matrices[matricesNumber]->getstring_() << endl;
				matricesNumber++;
				continue;
			}
			else if (inputLine.find("zeros") != string::npos)
			{  //F = zeros(2, 3)
				string s = inputLine.substr(inputLine.find("(")+1 , inputLine.rfind(")") - inputLine.find("("));
				char* sepp = " ,";
				char* tokenns = NULL;
				char* Line_ = new char [s.length()+1];
				strcpy(Line_,s.c_str());
				tokenns = strtok(Line_ , sepp);
				char * IRandIC[2];
				for(int i=0;i<2;i++)
				{
				   IRandIC[i]=tokenns;
				   tokenns=strtok(NULL,sepp);
				}
				int rows=atoi(IRandIC[0]);
				int cols=atoi(IRandIC[1]);
				matrices[matricesNumber]=new CMatrix(name.c_str(),rows,cols,0,0);
				if(inputLine[inputLine.length()-1] != ';')
				    cout << matrices[matricesNumber]->getstring_() << endl;
				matricesNumber++;
				continue;
			}
			else if (inputLine.find("ones") != string::npos)
			{   
				string s = inputLine.substr(inputLine.find("(")+1 , inputLine.rfind(")") - inputLine.find("("));
				char* sepp = " ,";
				char* tokenns = NULL;
				char* Line_ = new char [s.length()+1];
				strcpy(Line_,s.c_str());
				tokenns = strtok(Line_ , sepp);
				char * IRandIC[2];
				for(int i=0;i<2;i++)
				{
				   IRandIC[i]=tokenns;
				   tokenns=strtok(NULL,sepp);
				}
				int rows=atoi(IRandIC[0]);
				int cols=atoi(IRandIC[1]);
				matrices[matricesNumber]=new CMatrix(name.c_str(),rows,cols,1,1);
				if(inputLine[inputLine.length()-1] != ';')
				       cout << matrices[matricesNumber]->getstring_() << endl;
				matricesNumber++;
				continue;
			}
		
			for (int i = 0;i < inputLine.length();i++)
			{
				int fla;
				int pos = 0;
		     	string arg;
		/*///////////////////////////////////Trigonometric functions\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
			if (inputLine[i] == 's' && inputLine[i + 1] == 'i' && inputLine[i + 2] == 'n') //sin
				{
					pos = i;
					Extract(inputLine, i + 3, arg, i,fla);
					if (isNumber(arg))
					{
					  inputLine.replace(pos, i - pos, to_string((sin(stod(arg.c_str())))));
				    }
					else
					{
						string s= "s";
						 for(int j=0;j<matricesNumber;j++)
				        {
					        if(matrices[j]->getName()==arg)
							{
								matrices[matricesNumber]=new CMatrix(sin(*matrices[j]));
								string matrix_Name = s + to_string(y);  y++;
			                    matrices[matricesNumber]->setName(matrix_Name);
								inputLine.replace(pos, i - pos, (matrices[matricesNumber]->getName()));
								matricesNumber++;
							
							}
						       
						}
					}
			     }
				else if (inputLine[i] == 'c' && inputLine[i + 1] == 'o' && inputLine[i + 2] == 's') //cos
				{
					pos = i;
					Extract(inputLine, i + 3, arg, i,fla);
					if (isNumber(arg))
					{
					  inputLine.replace(pos, i - pos, to_string((cos(stod(arg.c_str())))));
				    }
					else
					{
						string s= "s";
						 for(int j=0;j<matricesNumber;j++)
				        {
					        if(matrices[j]->getName()==arg)
							{
								matrices[matricesNumber]=new CMatrix(sin(*matrices[j]));
								string matrix_Name = s + to_string(y);  y++;
			                    matrices[matricesNumber]->setName(matrix_Name);
								inputLine.replace(pos, i - pos, (matrices[matricesNumber]->getName()));
								matricesNumber++;
							
							}
						       
						}
					}
			     }
				else if (inputLine[i] == 't' && inputLine[i + 1] == 'a' && inputLine[i + 2] == 'n') //tan
				{
					pos = i;
					Extract(inputLine, i + 3, arg, i,fla);
					if (isNumber(arg))
					{
					  inputLine.replace(pos, i - pos, to_string((sin(stod(arg.c_str())))));
				    }
					else
					{
						string s= "s";
						 for(int j=0;j<matricesNumber;j++)
				        {
					        if(matrices[j]->getName()==arg)
							{
								matrices[matricesNumber]=new CMatrix(tan(*matrices[j]));
								string matrix_Name = s + to_string(y);  y++;
			                    matrices[matricesNumber]->setName(matrix_Name);
								inputLine.replace(pos, i - pos, (matrices[matricesNumber]->getName()));
								matricesNumber++;
							
							}
						       
						}
					}
			     }
				else if (inputLine[i] == 's' && inputLine[i + 1] == 'q' && inputLine[i + 2] == 'r' && inputLine[i + 3] == 't') //sqrt
				
				{
					pos = i;
					Extract(inputLine, i + 4, arg, i,fla);
					if (isNumber(arg))
					{
					  inputLine.replace(pos, i - pos, to_string((sqrt(stod(arg.c_str())))));
				    }
					
					else
					{
						string s= "s";
						 for(int j=0;j<matricesNumber;j++)
				        {
					        if(matrices[j]->getName()==arg)
							{
							
								CMatrix x(matrices[j]);
								CMatrix z(sqrt(x));
								matrices[matricesNumber]=new CMatrix (z);
								string matrix_Name = s + to_string(y);  y++;
			                    matrices[matricesNumber]->setName(matrix_Name);
								inputLine.replace(pos, i - pos, matrix_Name);		
							
							}
						       
						}
						 matricesNumber++;
					}
			     }

			}

			   
       //============================================================ calculate expression  =========================================================
			    string resultt;
			    string inputt=inputLine.substr(inputLine.find('=')+1,inputLine.length()-inputLine.find('='));
			
				char* lline=new char [inputt.length()+1];
				strcpy(lline,inputt.c_str());

				for(int i=0;i<inputt.length();i++)
				{		
				if(inputt[i]==' ')
				{
					shift_array( lline, inputt.length() , 0) ;
					if(inputt[i+1]!=32)
						break;
				}
				}
				int fll;


				  special_Extract(lline,resultt,fll);
				  try
				  {
					  if(fll)
						  throw(resultt);
				  }
				  catch(string ff)
				  {
					  cout<<"warning: division by zero"<<endl;
					  cout<<name<<" = INF"<<endl;
					  continue;
				  }

			if(isNumber(resultt))
			{ 
				
				matrices[matricesNumber]=new CMatrix(stod(resultt));	
				matrices[matricesNumber]->setName(name);
				if(inputLine[inputLine.length()-1] != ';')
				       cout << matrices[matricesNumber]->getstring_() << endl;
				       matricesNumber++;
			}
			else
			{
				int flagaa=0;
			   for (int i=0;i<matricesNumber;i++)
			   {
				if(matrices[i]->getName()==resultt)   
				{
				  for(int j=0;j<matricesNumber;j++)
				  {
					  if(matrices[j]->getName()==name)
					  {
                       matrices[j]=new CMatrix(*matrices[i]);
					   flagaa=1;
					  }
				  }
					 if (flagaa==0)
					  {
                       matrices[matricesNumber]=new CMatrix(*matrices[i]);
					   matrices[matricesNumber]->setName(name);
					   matricesNumber++;
					  }

				  
				}
			   }
			
				if(inputLine[inputLine.length()-1] != ';')
				              cout << matrices[matricesNumber-1]->getstring_() << endl;
			}
			}
		}
	}
}
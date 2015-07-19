#include"2-stack.h"
#include<iostream>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

///����ջ��������׺���ʽ��ֵ
///������򾭳��õ�������������

//��׺���ʽ��ֵ
double  Compute(char* str)
{
	Stack<double> s(5);
	double x,y;
	int i=0;
	while(str[i])
	{
		if(str[i]==' ')
		{
			i++;
			continue;
		}
		switch(str[i])
		{
		case'+':
			x=s.Pop()+s.Pop();
			i++;
			break;
		case'-':
			x=s.Pop();
			x=s.Pop()-x;
			i++;
			break;
		case'*':
			x=s.Pop()*s.Pop();
			i++;
			break;
		case'/':
			x=s.Pop();
			if(x!=0.0)
				x=s.Pop()/x;
			else
				cerr<<"Error!"<<endl;
			i++;
			break;
		default:
			x=0;
			while(str[i]>=48 && str[i]<=57)
			{
				x=x*10+str[i]-48;
				i++;
			}
			if(str[i]=='.')
			{
				i++;
				y=0;
				double j=10.0;
				while(str[i]>=48 && str[i]<=57)
				{
					y+=(str[i]-48)/j;
					i++;
					j*=10;
				}
				x+=y;
			}
		}
		s.Push(x);
	}
	if(s.IsEmpty())
		cerr<<"Stack is empty!"<<endl;
	x=s.Pop();
	if(s.IsEmpty())
		return x;
	cerr<<"expression is wrong !"<<endl;
	return 0;
}

int Precede(char op)
{
	switch(op)
	{
		case'+':
		case'-':
			return 1;
		case'*':
		case'/':
			return 2;
		case'(':
		case'@':
		default:
			return 0;
	}
}

//��׺ת��׺
void Change(char* s1,char* s2)
{
	Stack<char> R(5);
	R.Push('@');
	int i=0,j=0;
	char ch=s1[i];
	while(ch!='\0')
	{
		if(ch==' ')
		ch=s1[++i];
		else if(ch=='(')
		{
			R.Push(ch);
			ch=s1[++i];
		}
		else if(ch==')')
		{
			while(R.Peek()!='(')
				s2[j++]=R.Pop();
			R.Pop();
			ch=s1[++i];
		}
		else if(ch=='+'||ch=='-'||ch=='*'||ch=='/')
		{
			char w=R.Peek();
			while(Precede(w)>=Precede(ch))
			{
				s2[j++]=w;
				R.Pop();
				w=R.Peek();
			}
			R.Push(ch);
			ch=s1[++i];
		}
		else
		{
			if((ch<'0'||ch>'9')&&ch!='.')
				cout<<"Error!"<<endl;
			while((ch>='0'&&ch<='9')||ch=='.')
			{
				s2[j++]=ch;
				ch=s1[++i];
			}
			s2[j++]=' ';
		}
	}
	ch=R.Pop();
	while(ch!='@')
	{
		if(ch=='(')
			cerr<<"expression is wrong !"<<endl;
		else
		{
			s2[j++]=ch;
			ch=R.Pop();
		}
	}
	s2[j++]='\0';
}

void ComputeExpression()
{
	char a[50],b[50];
	cout<<"������׺���ʽ"<<endl;
	cin.getline(a,50);
	Change(a,b);
	cout<<"��׺:"<<a<<endl;
	cout<<"��׺:"<<b<<endl;
	cout<<"��ֵ:"<<Compute(b)<<endl;
}

//����ת��,��n��ʮ����ת��Ϊr����
void Conversion(int n,int r)
{	
	if(n<0)
	{
		return;
	}
	cout<<n<<"=";
	int e;
	Stack<int> s(10);
	while(n)
	{
		s.Push(n%r);
		n=n/r;
	}
	while(!s.IsEmpty())
	{
		e = s.Pop();
		cout<<e;
	}
	cout<<endl;
}

//����ƥ��
int Match()
{
	
	char b[80] = "{[]}#";
	int i=0;
	int state=1;
	char ch=(*b)++;
	Stack<int> s(100);

	while(ch!='#' && state)
	{
		switch(ch)
		{
		case '{':
		case '[':
		case '(':
			{
				s.Push(ch);
			}
			break;
		case '}':
			{
				if(!s.IsEmpty()&&s.Peek()=='{')
					s.Pop();
				else
					state=0;
			}
			break;
		case ']':
			{
				if(!s.IsEmpty()&&s.Peek()=='[')
					s.Pop();
				else
					state=0;
			}
			break;
		case ')':
			{
				if(!s.IsEmpty()&&s.Peek()=='(')
					s.Pop();
				else
					state=0;
			}
			break;
		}
		//ch=(*b)++;
		ch=b[++i];
	}
	if(state && s.IsEmpty())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

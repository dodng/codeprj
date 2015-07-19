////////////////////////////////////////////////////////////////////////////////////
// 
// @file: 2-stack.h
// @author: by Mr.Chen
// @date: 2014/9/15	14:10
// @brief: ջ
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef __STACK_H_
#define  __STACK_H_

#include <iostream>
using std::cerr;
using std::endl;

template<class ElemType>
class Stack
{
public:
	enum 
	{
		INIT_STACK_SIZE = 100,
	};

public:
	Stack(int ms)
	{
		if(ms<=0 || ms>INIT_STACK_SIZE)
		{
			cerr<<"Stack InitSize Error!"<<endl;
			ms = INIT_STACK_SIZE;
		}
		base=new ElemType[ms];
		if(!base)
		{
			cerr<<"Memory allocation failure!"<<endl;
		}
		top=-1;
	}

	void Push(ElemType item)
	{
		if(top==size-1)
		{
			AllocStack();
		}
		top++;
		base[top]=item;
	}

	ElemType Pop()
	{
		if(top==-1)
		{
			cerr<<"Stack is empty!"<<endl;
		}
		top--;
		return base[top+1];
	}

	ElemType Peek()
	{
		if(top==-1)
		{
			cerr<<"Stack is empty!"<<endl;
		}
		return base[top];
	}

	bool IsEmpty() {return top==-1;}

	~Stack()
	{
		delete []base;
		base=0;
		top=-1;
		size = 0;
	}

protected:
	void AllocStack()
	{
		size*=2;
		ElemType *p=new ElemType[size];
		for(int i=0;i<=top;i++)
		{
			p[i]=base[i];
		}
		delete []base;
		base=p;
	}

private:
	ElemType *base; //ջ��ָ��
	int top; //ջ��
	int size; //ջ�ռ�size
};

//��׺ת��׺
void Change(char* s1,char* s2);
//���ݺ�׺���ʽ��ֵ
double Compute(char* str);
//��׺���ʽ��ֵ
void ComputeExpression();
//����ת��
void Conversion(int n,int r);
//����ƥ��
int Match();

#endif

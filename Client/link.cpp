#include "StdAfx.h"

#include "link.h"

/************************************************************************/
/*   CPlayListNode * l_createLink()创建一个新链表 返回他的头            */
/************************************************************************/
CPlayListNode * l_createLink()
{
	CPlayListNode * p_Head = new CPlayListNode() ;
	p_Head->name = "_HEAD_";
	p_Head->path = "\\" ;
	p_Head->p_Next = NULL ;
	p_Head->p_Prev = p_Head;
	return p_Head ;
}

/************************************************************************/
/*   CPlayListNode * l_createNode(CString path,CString name)            */
/*   创建一个节点 返回节点指针                                          */
/************************************************************************/
CPlayListNode * l_createNode(CString path="",CString name="")
{
	CPlayListNode * p_Node = new CPlayListNode() ;
	p_Node->name = name ;
	p_Node->path = path ;
	p_Node->p_Next = NULL ;
	p_Node->p_Prev = p_Node;
	return p_Node ;
}

/************************************************************************/
/* l_getNodesCount(CPlayListNode * p_Head)获取节点数目(不包括头)        */
/************************************************************************/
int l_getNodesCount(CPlayListNode * p_Head)
{
	int i = 0;
	if(p_Head == NULL)
		return 0;
	while(p_Head->p_Next !=NULL)
	{
		i++;
		p_Head = p_Head->p_Next ;
	}
	return i;
}
/************************************************************************/
/* CPlayListNode * l_getNode(CPlayListNode * p_Head,int id)             */
/* 获取指定节点，参数为头指针、节点id ，返回节点指针，若找不到则返回NULL*/
/************************************************************************/
CPlayListNode * l_getNode(CPlayListNode * p_Head,CString name)
{
	if(p_Head == NULL || name.IsEmpty())
		return NULL;
	while(p_Head->p_Next !=NULL)
	{
		p_Head = p_Head->p_Next ;
		if(p_Head->name == name)
			return p_Head ;
	}
	
	return NULL ;
}

/************************************************************************/
/* CPlayListNode * l_getLastNode(CPlayListNode *p_Head)                 */
/*  获取最后位置上的节点                                                */
/************************************************************************/
CPlayListNode * l_getLastNode(CPlayListNode *p_Head)
{
	if(p_Head == NULL)
		return NULL;
	while(p_Head->p_Next !=NULL)
		p_Head = p_Head->p_Next;
	return p_Head;
}

/************************************************************************/
/*  CPlayListNode * l_appendNode(CPlayListNode *p_Node,CPlayListNode * p_Node_new)                 */
/*  在指定节点后添加节点到链表，返回新添加的节点指针                    */
/************************************************************************/
CPlayListNode * l_appendNode(CPlayListNode *p_Node,CPlayListNode * p_Node_new)
{
	if(p_Node == NULL || p_Node_new == NULL)
		return NULL;
	p_Node_new->p_Prev = p_Node ;
	p_Node_new->p_Next = p_Node->p_Next;
	p_Node->p_Next = p_Node_new;
	return p_Node_new;
}

/************************************************************************/
/* CPlayListNode * l_delNode(CPlayListNode * p_Head,CString name)       */
/* 删除指定节点 返回被删除节点之前一个节点                              */
/************************************************************************/
CPlayListNode * l_delNode(CPlayListNode * p_Head,CString name)
{
	if(p_Head == NULL)
		return NULL;
	CPlayListNode * p_Node = l_getNode(p_Head,name);
	if(p_Node == NULL)
		return NULL;
	CPlayListNode * p_Node2 = p_Node->p_Prev ;
	if(p_Node2 == NULL)
		return NULL;
	p_Node2->p_Next = p_Node->p_Next ;
	if(p_Node->p_Next != NULL)
		p_Node->p_Next->p_Prev = p_Node2 ;
	delete p_Node ;
	//如果删除的是第一个有效节点，则返回它后面的一个节点指针，仍处在第一的位置
	if(p_Node2->name == "_HEAD_")
		return p_Node2->p_Next;
	return p_Node2 ;
}

/************************************************************************/
/* void l_destroyLink(CPlayListNode * p_Head)销毁链表    */
/************************************************************************/
void l_destroyLink(CPlayListNode * p_Head)
{
	CPlayListNode * p_Node = NULL;
	while(p_Head)
	{
		p_Node = p_Head->p_Next;
		delete p_Head;
		p_Head = p_Node; 
	}
}

/************************************************************************/
/*  CPlayListNode * l_getPrevNode(CPlayListNode *p_Node)返回上一个节点  */
/************************************************************************/
CPlayListNode * l_getPrevNode(CPlayListNode *p_Node)
{
	if(p_Node == NULL)
		return NULL;
	if(p_Node->p_Prev && p_Node->p_Prev->name != "_HEAD_")
		return p_Node->p_Prev ;
	else
		return p_Node;
}

/************************************************************************/
/* CPlayListNode * l_getNextNode(CPlayListNode *p_Node)返回下一个节点   */
/************************************************************************/
CPlayListNode * l_getNextNode(CPlayListNode *p_Node)
{
	if(p_Node== NULL)
		return NULL;
	if(p_Node->p_Next)
		return p_Node->p_Next ;
	else
		return p_Node;
}

/************************************************************************/
/*CPlayListNode * l_moveNodePrev(CPlayListNode *p_Node)节点往前移       */
/*                         返回移动完成后的节点指针，					*/
/************************************************************************/
CPlayListNode * l_moveNodePrev(CPlayListNode *p_Node)
{
	//头节点或 第一个有效点 或 无效节点 直接返回 确保操作安全性
	if(p_Node == NULL || p_Node->name == "_HEAD_" || p_Node->p_Prev->name == "_HEAD_")
		return p_Node;
	CPlayListNode * a, * b,* c, * d;
	a = p_Node;
	b = a->p_Prev;
	c = a->p_Next;
	d = b->p_Prev;
	a->p_Next = b;
	a->p_Prev = d;
	b->p_Prev = a;
	b->p_Next = c;
	if(c)
		c->p_Prev = b;
	d->p_Next = a;
	return a;
}

/************************************************************************/
/*CPlayListNode * l_moveNodeNext(CPlayListNode *p_Node)节点往后移       */
/*                         返回移动完成后的节点指针						*/
/************************************************************************/
CPlayListNode * l_moveNodeNext(CPlayListNode *p_Node)
{
	//如果是无效节点或者是末尾节点 则直接返回
	if(p_Node == NULL || p_Node->p_Next == NULL)
		return p_Node;
	CPlayListNode * a, * b,* c, * d;
	a = p_Node ;
	d = a->p_Prev;
	b = a->p_Next;
	c = b->p_Next;
	a->p_Next = c;
	a->p_Prev = b;
	b->p_Next = a;
	//If a is the head node then b becomes the head node 
	if(d == a)
		b->p_Prev = b;
	else//else the previous point of b instead it of a
		b->p_Prev = d;

	if(d != a)
		d->p_Next = b;
	if(c != NULL)
		c->p_Prev = a;

	return a;
}

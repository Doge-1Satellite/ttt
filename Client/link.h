#ifndef _LINK_H_
#define _LINK_H_ 
struct CPlayListNode
{
	int length;
	CString path;
	CString name;
	CPlayListNode * p_Prev;
	CPlayListNode * p_Next;
};


CPlayListNode * l_getLastNode(CPlayListNode *p_Head);
CPlayListNode * l_getNode(CPlayListNode * p_Head,CString name);
CPlayListNode * l_getPrevNode(CPlayListNode *p_Node);
CPlayListNode * l_getNextNode(CPlayListNode *p_Node);
CPlayListNode * l_moveNodePrev(CPlayListNode *p_Node);
CPlayListNode * l_moveNodeNext(CPlayListNode *p_Node);

CPlayListNode * l_createLink();
CPlayListNode * l_createNode(CString path,CString name);
CPlayListNode * l_appendNode(CPlayListNode *p_Node,CPlayListNode * p_Node_new);
CPlayListNode * l_delNode(CPlayListNode * p_Head,CString name);

void l_destroyLink(CPlayListNode * p_Head);
void l_printList(CPlayListNode * p_Head);

int l_getNodesCount(CPlayListNode * p_Head);

#endif

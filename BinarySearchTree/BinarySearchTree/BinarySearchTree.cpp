#include "stdafx.h"
#include "CBSearchTree.h"

int main()
{
	int retval;

	CBSearchTree searchTree;	

	searchTree.InsertNode(8);
	
	searchTree.InsertNode(6);
	
	searchTree.InsertNode(4);

	searchTree.InsertNode(5);

	searchTree.InsertNode(7);

	searchTree.InsertNode(2);

	searchTree.InsertNode(11);

	searchTree.InsertNode(12);

	searchTree.InsertNode(9);

	searchTree.InsertNode(10);

	/*retval = searchTree.DeleteNode(5);
	if (!retval)
	{
		printf_s("delete error\n");
	}*/


	searchTree.InOrder();

	
}




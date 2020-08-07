#pragma once
class CBSearchTree
{
public:
	struct StructNode
	{
		StructNode* mParentsNode;
		StructNode* mLeft;
		StructNode* mRight;	
		int mData;
	};

public:

	CBSearchTree()
	{

		this->mDepth = -1;
		this->mRoot = nullptr;
	}

	~CBSearchTree()
	{	
		ReleaseTree();
	}

	//------------------------------------------------------
	// ���ο� �����͸� �߰��Ѵ�.
	//------------------------------------------------------
	bool InsertNode(int data)
	{
		StructNode* newNode = (StructNode*)malloc(sizeof(StructNode));
		newNode->mParentsNode = nullptr;
		newNode->mLeft = nullptr;
		newNode->mRight = nullptr;
		newNode->mData = data;		

		if (this->mRoot == nullptr)
		{
			this->mRoot = newNode;
			this->mNodeCount += 1;			
			return true;
		}
		else
		{
			if (linkNode(mRoot, newNode))
			{
				return true;
			}
		}

		return false;
	}

	//------------------------------------------------------
	// Ư�� �����͸� �����Ѵ�.
	//------------------------------------------------------
	bool DeleteNode(int data)
	{				
		StructNode* pNodeBuffer;
		pNodeBuffer = findNode(this->mRoot, data);
		if (pNodeBuffer == nullptr)
		{
			return false;
		}
	
		deleteNode(pNodeBuffer);
		
		return true;
	}

	//------------------------------------------------------
	// Ư�� �����͸� �˻��Ͽ� ��� ����
	//------------------------------------------------------
	StructNode* SearchNode(int data)
	{
		return findNode(this->mRoot, data);
	}

	//------------------------------------------------------
	// Ʈ�� ��ü ����
	//------------------------------------------------------
	bool ReleaseTree(void)
	{
		if (this->mRoot == nullptr)
		{
			return false;
		}

		postorderReleaseTreeRecursive(&this->mRoot);
		return true;
	}



	//------------------------------------------------------
	// ���
	//------------------------------------------------------
	void Print(StructNode* pNode = NULL, int Depth = 0, int LR = 0)
	{
		return;
	}


	// ���� �����Ͽ� ����ϱ�
	void InOrder()
	{
		if (this->mRoot == nullptr)
		{
			return;
		}
		else
		{
			inoderPrintRecursive(this->mRoot);
		}
	}


private:

	//------------------------------------------------------
	// InsertNode ���ο��� ȣ��Ǵ� ��ġ ã�� �߰� 
	//------------------------------------------------------
	bool linkNode(StructNode* pParent, StructNode* pChild)
	{
		
		while (1)
		{
			if (pParent->mData > pChild->mData)
			{
				if (pParent->mLeft == nullptr)
				{
					// �θ��� ���� ��� ����
					pParent->mLeft = pChild;

					// �θ� ��� ����
					pChild->mParentsNode = pParent;
					
					

					break;
				}
				else
				{
					
					pParent = pParent->mLeft;
					continue;
				}
			}
			else if (pParent->mData < pChild->mData)
			{
				
				if (pParent->mRight == nullptr)
				{
					// ������ ��� ����
					pParent->mRight = pChild;
		
					// �θ� ��� ����
					pChild->mParentsNode = pParent;
				

					break;
				}
				else
				{

					pParent = pParent->mRight;
					continue;
				}
			}
			else
			{
				free(pChild);
				return false;
			}
		}
		this->mNodeCount += 1;
		return true;
	}



	//------------------------------------------------------
	// SearchNode ���ο��� ȣ��Ǵ� ���ã��
	//------------------------------------------------------
	StructNode* findNode(StructNode* pNode, int data)
	{
		if (pNode == nullptr)
		{
			return nullptr;
		}

		while (1)
		{
			if (pNode->mData > data)
			{
				if (pNode->mLeft == nullptr)
				{
					return nullptr;
				}

				pNode = pNode->mLeft;
				continue;
			}
			else if(pNode->mData < data)
			{
				if (pNode->mRight == nullptr)
				{
					return nullptr;
				}

				pNode = pNode->mRight;
				continue;
			}
			else if (pNode->mData == data)
			{
				return pNode;
			}
		}
	}

	//------------------------------------------------------
	// DeleteNode ���ο��� ȣ��Ǵ� ���ã�� & ���� & ��ó�� ����Լ�
	//------------------------------------------------------
	void deleteNode(StructNode* pNode)
	{
		if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
		{		
			// ��� �� 1�� ����
			this->mNodeCount -= 1;			

			// �θ��� ���� ������� ������ ������� Ȯ�� �� ��� �ڸ���
			if (pNode->mParentsNode->mLeft == pNode)
			{
				pNode->mParentsNode->mLeft = nullptr;
			}
			else
			{
				pNode->mParentsNode->mRight = nullptr;
			}

			// �ش� ��带 free ��Ų��. 
			free(pNode);
			
			return;
		}
		else if (pNode->mLeft == nullptr && pNode->mRight != nullptr)
		{
	
			// ������ ���� �θ� ����� �������� ����������
			if (pNode->mParentsNode->mLeft == pNode)
			{
				// �θ��� ���ʰ� ������ ����� ������ ����
				pNode->mParentsNode->mLeft = pNode->mRight;

				// ������ ����� �θ� ������ ��� �θ��� ����
				pNode->mRight->mParentsNode = pNode->mParentsNode;
			
				this->mNodeCount -= 1;
			
				free(pNode);
			}
			else
			{

				pNode->mParentsNode->mRight = pNode->mRight;

				pNode->mRight->mParentsNode = pNode->mParentsNode;
	
				this->mNodeCount -= 1;
			
				free(pNode);
			}
		}
		else
		{
			StructNode* deleteNodeData = pNode;
			pNode = pNode->mLeft;

			while (1)
			{
				if (pNode->mRight == nullptr)
				{
					if (pNode->mParentsNode->mLeft == pNode)
					{
						// �θ� ��� �ڽ� ��� ����
						pNode->mParentsNode->mLeft = nullptr;

						// ������ ������ �ڸ��� ���ʿ��� ���� ������ ��� �߰�
						pNode->mParentsNode->mData = pNode->mData;
					}
					else
					{
						pNode->mParentsNode->mRight = nullptr;

						deleteNodeData->mData = pNode->mData;
					}


					// ��� �� 1�� ����
					this->mNodeCount -= 1;

					// �ش� ��带 free ��Ų��. 
					free(pNode);

					return;
				}

				pNode = pNode->mRight;
			}

		}

	}


	//------------------------------------------------------
	// ������ȸ ���� ����Լ�
	//------------------------------------------------------
	void postorderReleaseTreeRecursive(StructNode** pNode)
	{
		if (*pNode == nullptr)
		{
			return;
		}
		
		postorderReleaseTreeRecursive(&(*pNode)->mLeft);
		postorderReleaseTreeRecursive(&(*pNode)->mRight);
		free(*pNode);	
		*pNode = nullptr;
		this->mNodeCount -= 1;
		return;
	}


	// ���� ��ȸ �߷�
	void inoderPrintRecursive(StructNode* pNode)
	{
		static int rootX = 800;
		static int moveX = 800;


		if (pNode == nullptr)
		{
			return;
		}

		int parentsX;

		this->mDepth += 1;

		moveX /= 2;
		rootX = rootX - moveX;
		inoderPrintRecursive(pNode->mLeft);		
		rootX = rootX + moveX;
		moveX *= 2;

		printf_s("Depth : %d, Data : %d, X��ǥ : %d\n" ,this->mDepth,pNode->mData,rootX);

		moveX /= 2;
		rootX = rootX + moveX;
		inoderPrintRecursive(pNode->mRight);
		rootX = rootX - moveX;
		moveX *= 2;

		this->mDepth -= 1;

	}


	
private:
	int mDepth;

	StructNode* mRoot;

	int mNodeCount;

};


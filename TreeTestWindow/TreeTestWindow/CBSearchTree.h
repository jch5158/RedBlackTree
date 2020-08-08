#pragma once

#define X_POSITION 740
#define Y_DUMMY 30

class CBSearchTree
{
private:

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
		this->mNodeCount = 0;
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
		newNode->mRight = nullptr;
		newNode->mLeft = nullptr;
		newNode->mParentsNode = nullptr;
		newNode->mData = data;

		if (this->mRoot == nullptr)
		{
			this->mRoot = newNode;
			this->mNodeCount += 1;
			return true;
		}
		else
		{
			if (linkNode(this->mRoot, newNode))
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

		this->mNodeCount -= 1;

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

		// ���ǹ��� ���ؼ� return �Ǳ� ���ؼ��� ���������ͷ� �����ؾ� �Ѵ�.
		postorderReleaseTreeRecursive(&this->mRoot);
		return true;
	}



	//------------------------------------------------------
	// ���
	//------------------------------------------------------
	void Print(HDC hdc)
	{
		if (this->mRoot == nullptr)
		{
			return;
		}
		else
		{
			postorderPrintRecursive(this->mRoot, hdc);
		}
	}


	// ���� �����Ͽ� ����ϱ�
	void Inorder()
	{
		if (this->mRoot == nullptr)
		{
			return;
		}
		else
		{
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
				pParent = pParent->mLeft;
				continue;
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
				pParent = pParent->mRight;				
				continue;
			}
			
			free(pChild);
			return false;		
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
			else if (pNode->mData < data)
			{
				if (pNode->mRight == nullptr)
				{
					return nullptr;
				}

				pNode = pNode->mRight;
				continue;
			}
			
			// ã�� ��带 return �Ѵ�.
			return pNode;	
		}
	}

	//------------------------------------------------------
	// DeleteNode ���ο��� ȣ��Ǵ� ���ã�� & ���� & ��ó��
	//------------------------------------------------------
	void deleteNode(StructNode* pNode)
	{

		// ������ ����� ����, ������ �ڽ� ��尡 ���� ��� 
		if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
		{
			// �θ��� ���� ������� ������ ������� Ȯ�� �� ��� �ڸ��� 
			// �Ǵ� �θ� ���ٸ� ��Ʈ ����̴�.
			if (pNode->mParentsNode != nullptr)
			{
				if (pNode->mParentsNode->mLeft == pNode)
				{
					pNode->mParentsNode->mLeft = nullptr;
				}
				else
				{
					pNode->mParentsNode->mRight = nullptr;
				}
			}
			else
			{
				this->mRoot = nullptr;
			}	

			// �ش� ��带 free ��Ų��. 
			free(pNode);
	
			return;
		}
		// ������ ����� ������ �ڽ� ��尡 ���� ���
		else if (pNode->mLeft == nullptr && pNode->mRight != nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				// ������ ���� �θ� ����� �������� ����������
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// �θ��� ���ʰ� ������ ����� ������ ����
					pNode->mParentsNode->mLeft = pNode->mRight;
				}
				else
				{
					pNode->mParentsNode->mRight = pNode->mRight;
				}

				// ������ ����� �������� �θ� ������ ����� �θ�� �������ش�.
				pNode->mRight->mParentsNode = pNode->mParentsNode;	
			}
			else
			{
				// �ڽĳ�尡 �θ� ��带 �����ش�.
				pNode->mRight->mParentsNode = nullptr;
				this->mRoot = pNode->mRight;
			}
			
			free(pNode);

			return;
		}
		// ������ ����� ���ʿ� �ڽ��� ���� ���
		else if (pNode->mLeft != nullptr && pNode->mRight == nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// �θ��� ���ʰ� ������ ����� ������ ����
					pNode->mParentsNode->mLeft = pNode->mLeft;
				}
				else
				{
					// �θ��� ���ʰ� ������ ����� ������ ����
					pNode->mParentsNode->mRight = pNode->mLeft;
				}

				// ������ ����� �θ� ������ ��� �θ��� ����
				pNode->mLeft->mParentsNode = pNode->mParentsNode;
			}
			else
			{
				pNode->mLeft->mParentsNode = nullptr;
				this->mRoot = pNode->mLeft;
			}
			
			free(pNode);

			return;
		}
		else
		{
			// ������ ����� �����͸� �����صд�.
			StructNode* deleteNodeData = pNode;
			
			// ������ ����� ���� ��带 ��´�.
			pNode = pNode->mLeft;

			// ���� �ڽ� ����� ������ �ڽ� ��尡 ���� ���
			if (pNode->mRight == nullptr)
			{
				// ������ ���� �ٽ� �����͸� �ű��.
				pNode = pNode->mParentsNode;

				// �θ� ��尡 ���� ���
				if (pNode->mParentsNode != nullptr) 
				{
					// �θ� ����� ���� �ڽ����� ������ �ڽ����� Ȯ���Ѵ�.
					if (pNode->mParentsNode->mLeft == pNode)
					{
						pNode->mParentsNode->mLeft = pNode->mLeft;						
					}
					else
					{
						pNode->mParentsNode->mRight = pNode->mLeft;						
					}					
					
					pNode->mLeft->mParentsNode = pNode->mParentsNode;
				}
				else
				{
					this->mRoot = pNode->mLeft;
					pNode->mLeft->mParentsNode = nullptr;
				}

				pNode->mLeft->mRight = pNode->mRight;
				pNode->mRight->mParentsNode = pNode->mLeft;

				// �ش� ��带 free ��Ų��. 
				free(pNode);

				return;
			}
			else
			{
				while (1)
				{
					if (pNode->mRight == nullptr)
					{
						if (pNode->mLeft != nullptr)
						{
							pNode->mParentsNode->mRight = pNode->mLeft;
							pNode->mLeft->mParentsNode = pNode->mParentsNode;
						}
						else
						{
							pNode->mParentsNode->mRight = nullptr;
						}
						
						deleteNodeData->mData = pNode->mData;

						// �ش� ��带 free ��Ų��. 
						free(pNode);

						return;
					}

					pNode = pNode->mRight;
				}
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
	void postorderPrintRecursive(StructNode* pNode, HDC hdc)
	{
		static int rootX = X_POSITION;
		static int moveX = X_POSITION;

		if (pNode == nullptr)
		{
			return;
		}

		this->mDepth += 1;

		moveX /= 2;
		rootX = rootX - moveX;
		postorderPrintRecursive(pNode->mLeft,hdc);
		rootX = rootX + moveX;
		moveX *= 2;


		moveX /= 2;
		rootX = rootX + moveX;
		postorderPrintRecursive(pNode->mRight, hdc);
		rootX = rootX - moveX;
		moveX *= 2;

		if (pNode->mParentsNode != nullptr)
		{
			if (pNode->mParentsNode->mLeft == pNode)
			{
				MoveToEx(hdc, rootX, Y_DUMMY + ((this->mDepth + 1) * 80), nullptr);

				rootX = rootX + moveX;

				LineTo(hdc, rootX, Y_DUMMY + ((this->mDepth) * 80));

				rootX = rootX - moveX;
			}
			else if (pNode->mParentsNode->mRight == pNode)
			{
				
				MoveToEx(hdc, rootX, Y_DUMMY + ((this->mDepth + 1) * 80), nullptr);
				
				rootX = rootX - moveX;

				LineTo(hdc, rootX, Y_DUMMY + (this->mDepth * 80));

				rootX = rootX + moveX;
			}
		}

		Ellipse(hdc, rootX - 20, Y_DUMMY + ((this->mDepth + 1)*80) - 20, rootX + 20, Y_DUMMY + ((this->mDepth + 1) * 80) + 20);

		char str[20];

		sprintf_s(str, "%d", pNode->mData);

		TextOutA(hdc, rootX - 10, Y_DUMMY + ((this->mDepth + 1)*80) - 10,str,strlen(str));

		
		sprintf_s(str, "%d", this->mNodeCount);
		TextOutA(hdc, 50, 750, str,strlen(str));

		this->mDepth -= 1;
	}



private:
	int mDepth;

	StructNode* mRoot;

	int mNodeCount;

};


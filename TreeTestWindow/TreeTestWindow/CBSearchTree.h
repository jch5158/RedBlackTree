#pragma once

#define X_POSITION 740
#define Y_DUMMY 30

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
		newNode->mLeft = nullptr;
		newNode->mParentsNode = nullptr;
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
	void Inorder(HDC hdc)
	{
		if (this->mRoot == nullptr)
		{
			return;
		}
		else
		{
			inoderPrintRecursive(this->mRoot,hdc);
		}
	}

	//void Postoder(HDC hdc)
	//{
	//	if (this->mRoot == nullptr)
	//	{
	//		return;
	//	}
	//	else
	//	{
	//		postPrintRecursive(this->mRoot, hdc);
	//	}
	//}

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
			else if (pNode->mData < data)
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
	// DeleteNode ���ο��� ȣ��Ǵ� ���ã�� & ���� & ��ó��
	//------------------------------------------------------
	void deleteNode(StructNode* pNode)
	{
		if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
		{
			// ��� �� 1�� ����
			this->mNodeCount -= 1;

			// �θ��� ���� ������� ������ ������� Ȯ�� �� ��� �ڸ��� /
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
		else if (pNode->mLeft == nullptr && pNode->mRight != nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				// ������ ���� �θ� ����� �������� ����������
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// �θ��� ���ʰ� ������ ����� ������ ����
					pNode->mParentsNode->mLeft = pNode->mRight;

					// ������ ����� �θ� ������ ��� �θ��� ����wwww
					pNode->mRight->mParentsNode = pNode->mParentsNode;

				}
				else if (pNode->mParentsNode->mRight == pNode)
				{
					pNode->mParentsNode->mRight = pNode->mRight;

					pNode->mRight->mParentsNode = pNode->mParentsNode;
				}
			}
			else
			{
				pNode->mRight->mParentsNode = nullptr;
				this->mRoot = pNode->mRight;
			}


			this->mNodeCount -= 1;

			free(pNode);
		}
		else if (pNode->mLeft != nullptr && pNode->mRight == nullptr)
		{
			if (pNode->mParentsNode->mLeft == pNode)
			{
				// �θ��� ���ʰ� ������ ����� ������ ����
				pNode->mParentsNode->mLeft = pNode->mLeft;

				// ������ ����� �θ� ������ ��� �θ��� ����
				pNode->mLeft->mParentsNode = pNode->mParentsNode;

			}
			else
			{
				// �θ��� ���ʰ� ������ ����� ������ ����
				pNode->mParentsNode->mRight = pNode->mLeft;

				// ������ ����� �θ� ������ ��� �θ��� ����
				pNode->mLeft->mParentsNode = pNode->mParentsNode;

			}

			this->mNodeCount -= 1;

			free(pNode);
		}
		else
		{
			StructNode* deleteNodeData = pNode;
			pNode = pNode->mLeft;


			if (pNode->mRight == nullptr)
			{
				pNode = pNode->mParentsNode;

				if (pNode->mParentsNode != nullptr) 
				{
					if (pNode->mParentsNode->mLeft == pNode)
					{
						pNode->mParentsNode->mLeft = pNode->mLeft;

						pNode->mLeft->mRight = pNode->mRight;

						pNode->mRight->mParentsNode = pNode->mLeft;

						pNode->mLeft->mParentsNode = pNode->mParentsNode;

					}
					else
					{
						pNode->mParentsNode->mRight = pNode->mLeft;

						pNode->mLeft->mRight = pNode->mRight;

						pNode->mRight->mParentsNode = pNode->mLeft;

						pNode->mLeft->mParentsNode = pNode->mParentsNode;

					}
				}
				else
				{
					pNode->mRight->mParentsNode = pNode->mLeft;
					pNode->mLeft->mRight = pNode->mRight;
					pNode->mLeft->mParentsNode = nullptr;
					this->mRoot = pNode->mLeft;
				}

				this->mNodeCount -= 1;

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
	void inoderPrintRecursive(StructNode* pNode, HDC hdc)
	{
		static int rootX = 740;
		static int moveX = 740;

		if (pNode == nullptr)
		{
			return;
		}

		this->mDepth += 1;

		moveX /= 2;
		rootX = rootX - moveX;
		inoderPrintRecursive(pNode->mLeft,hdc);
		rootX = rootX + moveX;
		moveX *= 2;


		moveX /= 2;
		rootX = rootX + moveX;
		inoderPrintRecursive(pNode->mRight, hdc);
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



		//printf_s("xPosition : %d, Depth : %d, Data : %d, �θ� ��ǥ : %d\n", pNode->mX, this->mDepth, pNode->mData, parentsX);	

		Ellipse(hdc, rootX - 20, Y_DUMMY + ((this->mDepth + 1)*80) - 20, rootX + 20, Y_DUMMY + ((this->mDepth + 1) * 80) + 20);

		char str[20];

		sprintf_s(str, "%d", pNode->mData);

		TextOutA(hdc, rootX - 10, Y_DUMMY + ((this->mDepth + 1)*80) - 10,str,strlen(str));

		this->mDepth -= 1;

	
		
	}

	//void postPrintRecursive(StructNode* pNode, HDC hdc)
	//{
	//	if (pNode == nullptr)
	//	{
	//		return;
	//	}

	//	int parentsX;

	//	this->mDepth += 1;
	//	postPrintRecursive(pNode->mLeft, hdc);

	//	postPrintRecursive(pNode->mRight, hdc);

	//	if (pNode->mParentsNode == nullptr)
	//	{
	//		parentsX = 0;
	//	}
	//	else
	//	{
	//		parentsX = pNode->mParentsNode->mX;
	//	}

	//	//printf_s("xPosition : %d, Depth : %d, Data : %d, �θ� ��ǥ : %d\n", pNode->mX, this->mDepth, pNode->mData, parentsX);

	//	if (parentsX != 0)
	//	{
	//		MoveToEx(hdc, parentsX, Y_DUMMY + (this->mDepth * 80), nullptr);

	//		LineTo(hdc, pNode->mX, Y_DUMMY + ((this->mDepth + 1) * 80));
	//	}

	//	Ellipse(hdc, pNode->mX - 20, Y_DUMMY + ((this->mDepth + 1) * 80) - 20, pNode->mX + 20, Y_DUMMY + ((this->mDepth + 1) * 80) + 20);

	//	char str[20];

	//	sprintf_s(str, "%d", pNode->mData);

	//	TextOutA(hdc, pNode->mX - 10, Y_DUMMY + ((this->mDepth + 1) * 80) - 10, str, strlen(str));

	//	this->mDepth -= 1;
	//}
	


private:
	int mDepth;

	StructNode* mRoot;

	int mNodeCount;

};


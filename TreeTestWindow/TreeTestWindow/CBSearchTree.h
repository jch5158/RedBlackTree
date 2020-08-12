#pragma once

#define X_POSITION 740
#define Y_DUMMY 30

class CBSearchTree
{
private:

	enum class NODE_COLOR
	{
		BLACK = 0,
		RED
	};

	struct StructNode
	{
		StructNode* mParentsNode;
		StructNode* mLeft;
		StructNode* mRight;
		NODE_COLOR mColor;
		int mData;
	};

public:

	CBSearchTree()
	{
		this->mDepth = -1;
		this->mRoot = nullptr;
		this->mNodeCount = 0;
		this->mNil = (StructNode*)malloc(sizeof(StructNode));
		this->mNil->mColor = NODE_COLOR::BLACK;
		this->mNil->mLeft = nullptr;
		this->mNil->mRight = nullptr;
		this->mNil->mParentsNode = nullptr;
	}

	~CBSearchTree()
	{
		ReleaseTree();
		free(this->mNil);
		this->mNil = nullptr;	
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
		newNode->mColor = NODE_COLOR::RED;
		newNode->mData = data;
		newNode->mLeft = this->mNil;
		newNode->mRight = this->mNil;

		if (this->mRoot == nullptr)
		{
			this->mRoot = newNode;
			insertCase1Recursive(this->mRoot);
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
	void Print(HDC hdc,HBRUSH *redBrush,HBRUSH *blackBrush)
	{
		if (this->mRoot == nullptr)
		{
			return;
		}
		else
		{
			postorderPrintRecursive(this->mRoot, hdc, redBrush, blackBrush);
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
				if (pParent->mLeft == this->mNil)
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
				if (pParent->mRight == this->mNil)
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

		insertCase1Recursive(pChild);
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
		// ������ ����� �����͸� �����صд�.
		StructNode* deleteNodeData = pNode;

		// ������ ����� ���� ��带 ��´�.
		if (pNode->mLeft != this->mNil)
		{
			pNode = pNode->mLeft;
		}

		while (1)
		{
			if (pNode->mRight == this->mNil)
			{
				deleteNodeData->mData = pNode->mData;

				deleteOneChild(pNode);

				return;
			}

			pNode = pNode->mRight;
		}
	}

	//===============================================================
	// ���� ��带 return �ϴ� �Լ��̴�.
	//===============================================================
	StructNode* getSibling(StructNode* node)
	{
		// ���� �θ� ��带 ����Ű�� �θ� ����� ����� �ڽ����� Ȯ�� ��
		// ���� �θ��尡 �Ҿƹ��� ����� ���� �ڽ��� ��� �ݴ��� �ڽ���
		// ���� ����̴�.
		if (node == node->mParentsNode->mLeft)
		{
			return node->mParentsNode->mRight;
		}
		else
		{
			return node->mParentsNode->mLeft;
		}
	}

	//===============================================================
	// �ش� ��尡 nil ���� Ȯ�� �� true�� return �մϴ�.
	//===============================================================
	bool isNil(StructNode* node)
	{
		return (this->mNil == node);
	}


	//===============================================================
	// ������ ����� �Ҿƹ��� ���� ������ ����� �ڽ� ��带 �̾��ش�.
	//===============================================================
	void replaceNode(StructNode* node, StructNode* child)
	{
		child->mParentsNode = node->mParentsNode;

		if (node->mParentsNode->mLeft == node)
		{
			node->mParentsNode->mLeft = child;
		}
		else if (node->mParentsNode->mRight == node)
		{
			node->mParentsNode->mRight = child;
		}
	}


	void deleteOneChild(StructNode* node)
	{
		StructNode* child;


		// ���� ����� �ݴ��� ��带 child ���� �����Ѵ�.
		if (isNil(node->mRight))
		{
			child = node->mLeft;
		}
		else
		{
			child = node->mRight;
		}

		// �̹��� ������ ��尡 ��Ʈ�̸鼭, �ڽ��� Nil�� ������ ���� ���
		// this->mRoot�� nullptr�� �����ϰ� deleteCase�� �����Ѵ�.
		if (this->mRoot == node && child == this->mNil)
		{
			this->mRoot = nullptr;
		}
		else
		{
			replaceNode(node, child);

			// ������ ��尡 �������� �߰� �۾��� �ʿ����.
			if (node->mColor == NODE_COLOR::BLACK)
			{
				// ���� �ڽ� ��尡 ���忴�ٸ� �߰� �۾��� �ʿ����
				// �ٷ� return
				if (child->mColor == NODE_COLOR::RED)
				{
					child->mColor = NODE_COLOR::BLACK;
				}
				else
				{
					deleteCase1(child);
				}
			}
		}
		free(node);
	}

	void deleteCase1(StructNode* node)
	{
		// �θ� ��尡 ���� ���� ��Ʈ ��带 �����ϰ� �ڽ� ��带 �̹� �Լ� ���ڷ� ���� �޾��� ����̴�.
		if (node->mParentsNode != nullptr)
		{
			deleteCase2(node);
		}
	}

	void deleteCase2(StructNode* node)
	{
		// ���� ��带 ���Ѵ�.
		StructNode* siblingNode = getSibling(node);

		// ���� ���� ��尡 ������ ��� ���� �θ� ���� ������ ���̴�.
		// ���� ��带 "��"���� ����� �θ� ��带 "����"�� ���� ������
		// node �������� ȸ���Ͽ� ���� ������ ���߱� ���� ���� ������ �Ѵ�.
		// �� �� deleteCase3 ���� ����.
		if (siblingNode->mColor == NODE_COLOR::RED)
		{
			node->mParentsNode->mColor = NODE_COLOR::RED;
			siblingNode->mColor = NODE_COLOR::BLACK;
			
			if (node == node->mParentsNode->mLeft)
			{
				rotateLeft(node->mParentsNode);
			}
			else
			{
				rotateRight(node->mParentsNode);
			}
		}
		deleteCase3(node);
	}


	void deleteCase3(StructNode* node)
	{
		StructNode* siblingNode = getSibling(node);

		// ȸ���ϰų� ������ ���� ��尡 ���̿��ٸ�, ������ �ڽ� ��尡 nill �̰ų� �� �ڽĳ���̴�.
		// �� ��쿡�� ���� ��带 "����"�� �ٲ����μ� �ش� ���� ���� ����Ʈ�������� ������ ��������.
		// ������, �θ� ��带 Ÿ�� ������ ��ο� �θ��带 Ÿ����� �ʴ� ����� �� ������ ���̰� 
		// ���� �� �ֱ� ������ �θ� ��� ���� �ٽ� deleteCase1�� Ȯ�� �� ���� �Ѵ�.
		if (node->mParentsNode->mColor == NODE_COLOR::BLACK &&
			siblingNode->mColor == NODE_COLOR::BLACK &&
			siblingNode->mLeft->mColor == NODE_COLOR::BLACK &&
			siblingNode->mRight->mColor == NODE_COLOR::BLACK)
		{
			siblingNode->mColor = NODE_COLOR::RED;
			deleteCase1(node->mParentsNode);
		}	
		else
		{
			deleteCase4(node);
		}
	}

	void deleteCase4(StructNode* node)
	{
		StructNode* siblingNode = getSibling(node);

		// deleteCase2 ���� ȸ���Ͽ� �θ� ��尡 �����̰� ���� ���� ��尡 ���̸�
		// ������带 "����"�� �ٲپ� �ְ� �θ� ��带 "��"���� ������ �ش�.
		if (node->mParentsNode->mColor == NODE_COLOR::RED &&
			siblingNode->mColor == NODE_COLOR::BLACK &&
			siblingNode->mLeft->mColor == NODE_COLOR::BLACK &&
			siblingNode->mRight->mColor == NODE_COLOR::BLACK)
		{
			siblingNode->mColor = NODE_COLOR::RED;
			node->mParentsNode->mColor = NODE_COLOR::BLACK;
		}	
		else
		{
			deleteCase5(node);
		}		
	}

	void deleteCase5(StructNode* node)
	{
		StructNode* siblingNode = getSibling(node);
		
		// ���� �θ� ����� �����̸鼭 ���� ���� ��尡 ���̰�,  
		// ���� ����� ���� ��尡 ������ ��� ���尡 �ִ� ����� �ݴ� �������� ȸ���� �Ͽ� ���������� �����.
		// ������ �������� ȸ���� �Ͽ� ���������� ������ش�. 
		// else�� ���� �ݴ� �������� ���ش�. 
		if (
			node == node->mParentsNode->mLeft &&
			siblingNode->mRight->mColor == NODE_COLOR::BLACK &&
			siblingNode->mLeft->mColor == NODE_COLOR::RED
			)
		{
			siblingNode->mColor = NODE_COLOR::RED;
			siblingNode->mLeft->mColor = NODE_COLOR::BLACK;
			rotateRight(siblingNode);
		}
		else if 
			(
			node == node->mParentsNode->mRight &&
			siblingNode->mLeft->mColor == NODE_COLOR::BLACK &&
			siblingNode->mRight->mColor == NODE_COLOR::RED 
			)
		{
			siblingNode->mColor = NODE_COLOR::RED;
			siblingNode->mRight->mColor = NODE_COLOR::BLACK;
			rotateLeft(siblingNode);
		}

		deleteCase6(node);
	}


	void deleteCase6(StructNode* node)
	{
		StructNode* siblingNode = getSibling(node);
		
		siblingNode->mColor = node->mParentsNode->mColor;
		node->mParentsNode->mColor = NODE_COLOR::BLACK;

		// ���������� �� ���� ���� �ڽĵ��� ���� ��尡 �ִ� �������� ȸ���Ѵ�.
		// ����� ����� deleteCase5�� ���� ��带 ������ �ٲپ� ���� ������ ���߾� �ش�. 
		if (node == node->mParentsNode->mLeft)
		{
			siblingNode->mRight->mColor = NODE_COLOR::BLACK;
			rotateLeft(node->mParentsNode);
		}
		else 
		{
			siblingNode->mLeft->mColor = NODE_COLOR::BLACK;
			rotateRight(node->mParentsNode);
		}
	}


	//------------------------------------------------------
	// ������ȸ Ʈ�� ���� ����Լ�
	//------------------------------------------------------
	void postorderReleaseTreeRecursive(StructNode** pNode)
	{
		if (*pNode == this->mNil)
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


	//=======================================================
	//���ڷ� ���޵� ��带 �������� �Ҿƹ��� ��带 return �Ѵ�.
	//=======================================================
	StructNode* getGrandparent(StructNode* node)
	{
		if (node != nullptr && node->mParentsNode != nullptr)
		{
			return node->mParentsNode->mParentsNode;
		}
		else
		{
			return nullptr;
		}
	}


	//==================================================================
	//���ڷ� ���޵� ����� �θ� �ݴ��� ��� ��,���� ��带 ���ϴ� �Լ��̴ϴ�.
	//==================================================================
	StructNode* getUncle(StructNode* node)
	{
		StructNode* grandparentNode = getGrandparent(node);

		if (grandparentNode == nullptr)
		{
			return nullptr;
		}

		if (grandparentNode->mLeft == node->mParentsNode)
		{
			return grandparentNode->mRight;
		}
		else 
		{
			return grandparentNode->mLeft;
		}
	}

	// ==========================================================================
	// ��Ʈ ��尡 ���� ��ϵǴ� case�� ��� ���� ���� ��� case1���� ������ �����Ѵ�.
	//===========================================================================
	void insertCase1Recursive(StructNode *node)
	{
		// ù ��°, (��Ʈ ���) ���� ������ ���̴�. ��, �θ� ��尡 ����.
		if (node->mParentsNode == nullptr)
		{
			node->mColor = NODE_COLOR::BLACK;
		}
		else
		{
			insertCase2Recursive(node);
		}
	}

	//========================================================================
	//�θ� ��尡 ���� ��� ���ο� ���� ���ؼ� ����� ������ Ʋ������ �ʴ´�.
	//���ο� ����� �θ� ������ ��� case3���� ������.
	//========================================================================
	void insertCase2Recursive(StructNode* node)
	{
		// �θ� ��尡 ���̸��� �ٷ� return �Ѵ�.
		if (node->mParentsNode->mColor == NODE_COLOR::BLACK)
		{
			return;
		}
		else
		{
			insertCase3Recursive(node);
		}
	}

	//====================================================================================
	// ���� ��尡 ������ ��� �Ҿƹ��� ��带 ����� �ٲٰ� ���̰� �θ� ��带 ������ �ٲ۴�.
	// �̷��� �ؼ� �ش� Ʈ�������� ���� �� Ʈ���� ��Ģ�� ��������, �Ҿƹ��� ��带 �������� 
	// �Ҿƹ��� ����� �θ� ��尡 ������ ��� Ʋ���� �� �ֱ� ������ �Ҿƹ��� ��带 �ٽ�
	// case1 �� ��������� ����.
	// ���� ���� ��尡 ���̸� case4�� ���ڸ� �����Ѵ�.
	//=============================================================================
	void insertCase3Recursive(StructNode* node)
	{
		StructNode* uncleNode = getUncle(node);
		
		StructNode* grandparent;

		// =================================================================
		// ���� ��尡 �����̸��� �� �θ� ��嵵 �����̱� ������ �Ҿƹ��� ��带 ����� ���� �� �θ�� ���� ��带 ������ �����.
		// �Ҿƹ��� ��尡 ��Ʈ�̸��� ������ ���д�.
		if (uncleNode != nullptr && uncleNode->mColor == NODE_COLOR::RED)
		{
			uncleNode->mColor = NODE_COLOR::BLACK;
			node->mParentsNode->mColor = NODE_COLOR::BLACK;
			grandparent = getGrandparent(node);
			
			if (grandparent != this->mRoot)
			{
				grandparent->mColor = NODE_COLOR::RED;
			}

			// �Ҿƹ��� ��尡 ���������� �����Ͽ���, �Ҿƹ��� ����� �θ� ��尡 �������� �� �ֱ� ������
			// insertCase1 �Լ��� ���ڷ� �����Ͽ� ��������� ó���Ѵ�.
			insertCase1Recursive(grandparent);
		}
		else
		{
			// ���� ��尡 ���� ��� ���� Case4�� ���ڸ� �ѱ��.
			insertCase4Recursive(node);
		}

	}


	//================================================================
	// ���� �Ҿƹ��� ��带 �������� �������� �ƴ� ��� ȸ���� �Ͽ��� 
	// ���� �� Ʈ���� Ʋ���� ������ �������� �ʱ� ������ �Ҿƹ��� ��带
	// �������� ���������� ����� ���� ���� ����� �θ� ��带 �������� 
	// ȸ���Ѵ�.
	//=================================================================
	void insertCase4Recursive(StructNode* node)
	{		
		StructNode* grandparent = getGrandparent(node);

		// �Ҿƹ��� ������ �߰��� ��� ���� �������� �ƴ� ��� �θ� ��带 �������� 
		// �������� ȸ���� �ϰ� case5 �Լ��� ���ڸ� ������.
		// �Ǵ� �̹� �������� ��� �ٷ� case5�� �Լ��� ������.
 	    if (node->mParentsNode->mRight == node && grandparent->mLeft == node->mParentsNode)
		{
			rotateLeft(node->mParentsNode);
			node = node->mLeft;
		}
		else if(node->mParentsNode->mLeft == node && grandparent->mRight == node->mParentsNode)
		{
			rotateRight(node->mParentsNode);
			node = node->mRight;
		}
		insertCase5Recursive(node);
	}

	//==================================================================
	// �Ҿƹ��� ��带 �������� ������ �������� ��� ���� �������� ȸ����Ų��. 
	// �׸��� �Ҿƹ��� ���, ���� ��带 ����� �θ� ��带 ������ �����Ѵ�.
	//==================================================================
	void insertCase5Recursive(StructNode* node)
	{
		StructNode* grandparent = getGrandparent(node);

		// �θ� ����� �÷��� ������ �ٲ۴�.
		node->mParentsNode->mColor = NODE_COLOR::BLACK;
		
		// �Ҿƹ��� ��带 ����� �ٲ۴�.
		grandparent->mColor = NODE_COLOR::RED;

		// �θ� ���� �������� �Ǵ� ������ �ݴ� �������� ȸ���Ͽ� ������ �����.
		if (node == node->mParentsNode->mLeft)
		{
			rotateRight(grandparent);
		}
		else
		{	
			rotateLeft(grandparent);
		}
	}


	void rotateRight(StructNode* pivotNode)
	{
		if (pivotNode->mLeft == nullptr)
		{
			return;
		}

		// ���� ����� ���� ���
		StructNode* leftNode = pivotNode->mLeft;

		// ���� ����� �θ� ���
		StructNode* parentNode = pivotNode->mParentsNode;


		// ���� ����� ���� ����� �����ʿ� �ڽ� ��尡 �������� ���� ����� �ڽ� ���� �����Ѵ�.
		// ���������� �Ѱܾ� �� ��尡 Nil �� ��� �θ� ������ �����Ѵ�.	
		if (leftNode->mRight != this->mNil && leftNode->mRight != nullptr )
		{
			leftNode->mRight->mParentsNode = pivotNode;
		}

		pivotNode->mLeft = leftNode->mRight;

		pivotNode->mParentsNode = leftNode;

		leftNode->mRight = pivotNode;

		leftNode->mParentsNode = parentNode;

		// �Ҿƹ��� ��尡 nullptr�� �ƴ� ��� ȸ���Ͽ� ������ �� ��带 
		// ���� �Ǵ� ���������� �����Ѵ�.
		if (parentNode != nullptr)
		{
			if (parentNode->mLeft == pivotNode)
			{
				parentNode->mLeft = leftNode;
			}
			else
			{
				parentNode->mRight = leftNode;
			}
		}
		else
		{
			this->mRoot = leftNode;
		}
	}


	void rotateLeft(StructNode* pivotNode)
	{
		if (pivotNode->mRight == nullptr)
		{
			return;
		}

		StructNode* rightNode = pivotNode->mRight;

		StructNode* parentNode = pivotNode->mParentsNode;

		// �Ѱܾߵ� ��尡 Nil�� ��� �θ� ������ �����Ѵ�.
		if (rightNode->mLeft != this->mNil && rightNode->mLeft != nullptr)
		{
			rightNode->mLeft->mParentsNode = pivotNode;

		}

		pivotNode->mRight = rightNode->mLeft;

		pivotNode->mParentsNode = rightNode;

		rightNode->mLeft = pivotNode;

		rightNode->mParentsNode = parentNode;

		if (parentNode != nullptr) {
			if (parentNode->mLeft == pivotNode)
			{
				parentNode->mLeft = rightNode;
			}
			else
			{
				parentNode->mRight = rightNode;
			}
		}
		else
		{
			this->mRoot = rightNode;
		}

	}

	// ���� ��ȸ �߷�
	void postorderPrintRecursive(StructNode* pNode, HDC hdc, HBRUSH* redBrush, HBRUSH* blackBrush)
	{


		static int rootX = X_POSITION;
		static int moveX = X_POSITION;

		if (pNode == this->mNil)
		{
			return;
		}

		this->mDepth += 1;

		moveX /= 2;
		rootX = rootX - moveX;
		postorderPrintRecursive(pNode->mLeft,hdc,redBrush, blackBrush);
		rootX = rootX + moveX;
		moveX *= 2;


		moveX /= 2;
		rootX = rootX + moveX;
		postorderPrintRecursive(pNode->mRight, hdc, redBrush, blackBrush);
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

		if (pNode->mColor == NODE_COLOR::BLACK && pNode != this->mNil)
		{
			SelectObject(hdc, *blackBrush);
			Ellipse(hdc, rootX - 20, Y_DUMMY + ((this->mDepth + 1) * 80) - 20, rootX + 20, Y_DUMMY + ((this->mDepth + 1) * 80) + 20);
		}
		else if(pNode->mColor == NODE_COLOR::RED && pNode != this->mNil)
		{
			SelectObject(hdc, *redBrush);
			Ellipse(hdc, rootX - 20, Y_DUMMY + ((this->mDepth + 1) * 80) - 20, rootX + 20, Y_DUMMY + ((this->mDepth + 1) * 80) + 20);
		}
	
		char str[20];

		if (this->mNil != pNode) 
		{
			sprintf_s(str, "%d", pNode->mData);
			TextOutA(hdc, rootX - 10, Y_DUMMY + ((this->mDepth + 1) * 80) - 10, str, strlen(str));
		}

		sprintf_s(str, "%d", this->mNodeCount);
		TextOutA(hdc, 50, 750, str,strlen(str));

		this->mDepth -= 1;
	}



public:
	int mDepth;

	StructNode* mRoot;

	StructNode* mNil;

	int mNodeCount;

};


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
	// 새로운 데이터를 추가한다.
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
	// 특정 데이터를 삭제한다.
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
	// 특정 데이터를 검색하여 노드 리턴
	//------------------------------------------------------
	StructNode* SearchNode(int data)
	{
		return findNode(this->mRoot, data);
	}

	//------------------------------------------------------
	// 트리 전체 삭제
	//------------------------------------------------------
	bool ReleaseTree(void)
	{
		if (this->mRoot == nullptr)
		{
			return false;
		}

		// 조건문을 통해서 return 되기 위해서는 이중포인터로 전달해야 한다.
		postorderReleaseTreeRecursive(&this->mRoot);
		return true;
	}



	//------------------------------------------------------
	// 출력
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


	// 전위 순휘하여 출력하기
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
	// InsertNode 내부에서 호출되는 위치 찾아 추가 
	//------------------------------------------------------
	bool linkNode(StructNode* pParent, StructNode* pChild)
	{	
		while (1)
		{
			if (pParent->mData > pChild->mData)
			{
				if (pParent->mLeft == this->mNil)
				{
					// 부모의 왼쪽 노드 연결
					pParent->mLeft = pChild;

					// 부모 노드 연결
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
					// 오른쪽 노드 연결
					pParent->mRight = pChild;

					// 부모 노드 연결
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
	// SearchNode 내부에서 호출되는 노드찾기
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
			
			// 찾을 노드를 return 한다.
			return pNode;	
		}
	}



	//------------------------------------------------------
	// DeleteNode 내부에서 호출되는 노드찾기 & 삭제 & 후처리
	//------------------------------------------------------
	void deleteNode(StructNode* pNode)
	{
		// 삭제할 노드의 포인터를 저장해둔다.
		StructNode* deleteNodeData = pNode;

		// 삭제할 노드의 왼쪽 노드를 얻는다.
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
	// 삼촌 노드를 return 하는 함수이다.
	//===============================================================
	StructNode* getSibling(StructNode* node)
	{
		// 나의 부모 노드를 가리키는 부모 노드의 어느쪽 자식인지 확인 후
		// 나의 부모노드가 할아버지 노드의 왼쪽 자식일 경우 반대편 자식이
		// 삼촌 노드이다.
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
	// 해당 노드가 nil 인지 확인 후 true를 return 합니다.
	//===============================================================
	bool isNil(StructNode* node)
	{
		return (this->mNil == node);
	}


	//===============================================================
	// 삭제할 노드의 할아버지 노드와 삭제할 노드의 자식 노드를 이어준다.
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


		// 리프 노드의 반대편 노드를 child 노드로 지정한다.
		if (isNil(node->mRight))
		{
			child = node->mLeft;
		}
		else
		{
			child = node->mRight;
		}

		// 이번에 삭제할 노드가 루트이면서, 자식은 Nil만 가지고 있을 경우
		// this->mRoot를 nullptr로 변경하고 deleteCase는 생략한다.
		if (this->mRoot == node && child == this->mNil)
		{
			this->mRoot = nullptr;
		}
		else
		{
			replaceNode(node, child);

			// 삭제할 노드가 레드라면은 추가 작업이 필요없다.
			if (node->mColor == NODE_COLOR::BLACK)
			{
				// 만약 자식 노드가 레드였다며 추가 작업이 필요없음
				// 바로 return
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
		// 부모 노드가 없을 경우는 루트 노드를 삭제하고 자식 노드를 이번 함수 인자로 전달 받았을 경우이다.
		if (node->mParentsNode != nullptr)
		{
			deleteCase2(node);
		}
	}

	void deleteCase2(StructNode* node)
	{
		// 형제 노드를 구한다.
		StructNode* siblingNode = getSibling(node);

		// 만약 형제 노드가 레드일 경우 나의 부모 노드는 무조건 블랙이다.
		// 형제 노드를 "블랙"으로 만들고 부모 노드를 "레드"로 만든 다음에
		// node 방향으로 회전하여 블랙의 개수를 맞추기 위한 예비 동작을 한다.
		// 그 후 deleteCase3 으로 간다.
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

		// 회전하거나 기존의 형제 노드가 블랙이였다면, 형제의 자식 노드가 nill 이거나 블랙 자식노드이다.
		// 이 경우에는 형제 노드를 "레드"로 바꿈으로서 해당 내가 속한 서브트리에서의 균형은 맞춰졌다.
		// 하지만, 부모 노드를 타고 들어오는 경로와 부모노드를 타고오지 않는 경로의 블랙 개수의 차이가 
		// 있을 수 있기 때문에 부모 노드 부터 다시 deleteCase1을 확인 해 봐야 한다.
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

		// deleteCase2 에서 회전하여 부모 노드가 레드이고 나의 형제 노드가 블랙이면
		// 형제노드를 "레드"로 바꾸어 주고 부모 노드를 "블랙"으로 변경해 준다.
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
		
		// 내가 부모 노드의 왼쪽이면서 나의 형제 노드가 블랙이고,  
		// 형제 노드의 왼쪽 노드가 레드일 경우 레드가 있는 노드의 반대 방향으로 회전을 하여 일직선으로 만든다.
		// 오른쪽 방향으로 회전을 하여 일직선으로 만들어준다. 
		// else는 이의 반대 방향으로 해준다. 
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

		// 일직선으로 된 형제 노드와 자식들을 지운 노드가 있던 방향으로 회전한다.
		// 레드로 변경된 deleteCase5의 형제 노드를 블랙으로 바꾸어 블랙의 균형을 맞추어 준다. 
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
	// 후위순회 트리 삭제 재귀함수
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
	//인자로 전달된 노드를 기준으로 할아버지 노드를 return 한다.
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
	//인자로 전달된 노드의 부모 반대편 노드 즉,삼촌 노드를 구하는 함수이니다.
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
	// 루트 노드가 이제 등록되는 case일 경우 아직 없을 경우 case1에서 색상을 지정한다.
	//===========================================================================
	void insertCase1Recursive(StructNode *node)
	{
		// 첫 번째, (루트 노드) 노드는 무조건 블랙이다. 즉, 부모 노드가 없다.
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
	//부모 노드가 블랙일 경우 새로운 노드로 인해서 레드블랙 조건이 틀어지지 않는다.
	//새로운 노드의 부모가 레드일 경우 case3으로 던진다.
	//========================================================================
	void insertCase2Recursive(StructNode* node)
	{
		// 부모 노드가 블랙이면은 바로 return 한다.
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
	// 삼촌 노드가 레드일 경우 할아버지 노드를 레드로 바꾸고 삼촌과 부모 노드를 블랙으로 바꾼다.
	// 이렇게 해서 해당 트리에서의 레드 블랙 트리의 규칙은 지켰지만, 할아버지 노드를 기준으로 
	// 할아버지 노드의 부모 노드가 레드일 경우 틀어질 수 있기 때문에 할아버지 노드를 다시
	// case1 로 재귀적으로 들어간다.
	// 만약 삼촌 노드가 블랙이면 case4로 인자를 전달한다.
	//=============================================================================
	void insertCase3Recursive(StructNode* node)
	{
		StructNode* uncleNode = getUncle(node);
		
		StructNode* grandparent;

		// =================================================================
		// 삼촌 노드가 레드이면은 내 부모 노드도 레드이기 때문에 할아버지 노드를 레드로 만든 후 부모와 삼촌 노드를 블랙으로 만든다.
		// 할아버지 노드가 루트이면은 블랙으로 냅둔다.
		if (uncleNode != nullptr && uncleNode->mColor == NODE_COLOR::RED)
		{
			uncleNode->mColor = NODE_COLOR::BLACK;
			node->mParentsNode->mColor = NODE_COLOR::BLACK;
			grandparent = getGrandparent(node);
			
			if (grandparent != this->mRoot)
			{
				grandparent->mColor = NODE_COLOR::RED;
			}

			// 할아버지 노드가 빨간색으로 변경하였고, 할아버지 노드의 부모 노드가 빨간색일 수 있기 때문에
			// insertCase1 함수로 인자로 전달하여 재귀적으로 처리한다.
			insertCase1Recursive(grandparent);
		}
		else
		{
			// 삼촌 노드가 블랙일 경우 다음 Case4로 인자를 넘긴다.
			insertCase4Recursive(node);
		}

	}


	//================================================================
	// 만약 할아버지 노드를 기준으로 일직선이 아닐 경우 회전을 하여도 
	// 레드 블랙 트리의 틀어진 조건이 교정되지 않기 때문에 할아버지 노드를
	// 기준으로 일직선으로 만들기 위해 기준 노드의 부모 노드를 기준으로 
	// 회전한다.
	//=================================================================
	void insertCase4Recursive(StructNode* node)
	{		
		StructNode* grandparent = getGrandparent(node);

		// 할아버지 노드부터 추가할 노드 까지 일직선이 아닐 경우 부모 노드를 기준으로 
		// 왼쪽으로 회전을 하고 case5 함수로 인자를 던진다.
		// 또는 이미 일직선일 경우 바로 case5로 함수를 던진다.
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
	// 할아버지 노드를 기준으로 오른쪽 일직선일 경우 왼쪽 방향으로 회전시킨다. 
	// 그리고 할아버지 노드, 삼촌 노드를 레드로 부모 노드를 블랙으로 변경한다.
	//==================================================================
	void insertCase5Recursive(StructNode* node)
	{
		StructNode* grandparent = getGrandparent(node);

		// 부모 노드의 컬러를 블랙으로 바꾼다.
		node->mParentsNode->mColor = NODE_COLOR::BLACK;
		
		// 할아버지 노드를 레드로 바꾼다.
		grandparent->mColor = NODE_COLOR::RED;

		// 부모 노드와 일직선이 되는 방향의 반대 방향으로 회전하여 균형을 맞춘다.
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

		// 기준 노드의 왼쪽 노드
		StructNode* leftNode = pivotNode->mLeft;

		// 기준 노드의 부모 노드
		StructNode* parentNode = pivotNode->mParentsNode;


		// 기준 노드의 왼쪽 노드의 오른쪽에 자식 노드가 있으면은 기준 노드의 자식 노드로 전달한다.
		// 오른쪽으로 넘겨야 될 노드가 Nil 일 경우 부모 연결을 생략한다.	
		if (leftNode->mRight != this->mNil && leftNode->mRight != nullptr )
		{
			leftNode->mRight->mParentsNode = pivotNode;
		}

		pivotNode->mLeft = leftNode->mRight;

		pivotNode->mParentsNode = leftNode;

		leftNode->mRight = pivotNode;

		leftNode->mParentsNode = parentNode;

		// 할아버지 노드가 nullptr이 아닐 경우 회전하여 기준이 될 노드를 
		// 왼쪽 또는 오른쪽으로 연결한다.
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

		// 넘겨야될 노드가 Nil일 경우 부모 연결은 생략한다.
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

	// 후위 순회 추력
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


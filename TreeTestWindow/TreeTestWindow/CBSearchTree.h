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
	// 새로운 데이터를 추가한다.
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
				if (pParent->mLeft == nullptr)
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
				
				if (pParent->mRight == nullptr)
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

		// 삭제할 노드의 왼쪽, 오른쪽 자식 노드가 없을 경우 
		if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
		{
			// 부모의 왼쪽 노드인지 오른쪽 노드인지 확인 후 노드 자르기 
			// 또는 부모가 없다면 루트 노드이다.
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

			// 해당 노드를 free 시킨다. 
			free(pNode);
	
			return;
		}
		// 삭제할 노드의 오른쪽 자식 노드가 있을 경우
		else if (pNode->mLeft == nullptr && pNode->mRight != nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				// 삭제할 노드는 부모 노드의 왼쪽인지 오른쪽인지
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
					pNode->mParentsNode->mLeft = pNode->mRight;
				}
				else
				{
					pNode->mParentsNode->mRight = pNode->mRight;
				}

				// 삭제할 노드의 오른쪽의 부모를 삭제할 노드의 부모로 연결해준다.
				pNode->mRight->mParentsNode = pNode->mParentsNode;	
			}
			else
			{
				// 자식노드가 부모 노드를 끊어준다.
				pNode->mRight->mParentsNode = nullptr;
				this->mRoot = pNode->mRight;
			}
			
			free(pNode);

			return;
		}
		// 삭제할 노드의 왼쪽에 자식이 있을 경우
		else if (pNode->mLeft != nullptr && pNode->mRight == nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
					pNode->mParentsNode->mLeft = pNode->mLeft;
				}
				else
				{
					// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
					pNode->mParentsNode->mRight = pNode->mLeft;
				}

				// 연결할 노드의 부모를 삭제할 노드 부모의 연결
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
			// 삭제할 노드의 포인터를 저장해둔다.
			StructNode* deleteNodeData = pNode;
			
			// 삭제할 노드의 왼쪽 노드를 얻는다.
			pNode = pNode->mLeft;

			// 왼쪽 자식 노드의 오른쪽 자식 노드가 없을 경우
			if (pNode->mRight == nullptr)
			{
				// 삭제할 노드로 다시 포인터를 옮긴다.
				pNode = pNode->mParentsNode;

				// 부모 노드가 있을 경우
				if (pNode->mParentsNode != nullptr) 
				{
					// 부모 노드의 왼쪽 자식인지 오른쪽 자식인지 확인한다.
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

				// 해당 노드를 free 시킨다. 
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

						// 해당 노드를 free 시킨다. 
						free(pNode);

						return;
					}

					pNode = pNode->mRight;
				}
			}
		}


	}


	//------------------------------------------------------
	// 후위순회 삭제 재귀함수
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


	// 후위 순회 추력
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


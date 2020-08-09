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
	// 새로운 데이터를 추가한다.
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

		postorderReleaseTreeRecursive(&this->mRoot);
		return true;
	}



	//------------------------------------------------------
	// 출력
	//------------------------------------------------------
	void Print(StructNode* pNode = NULL, int Depth = 0, int LR = 0)
	{
		return;
	}


	// 전위 순휘하여 출력하기
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
	// DeleteNode 내부에서 호출되는 노드찾기 & 삭제 & 후처리 재귀함수
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


	// 중위 순회 추력
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

		printf_s("Depth : %d, Data : %d, X좌표 : %d\n" ,this->mDepth,pNode->mData,rootX);

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


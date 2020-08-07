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
	// 새로운 데이터를 추가한다.
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
					// 오른쪽 노드 연결
					pParent->mRight = pChild;

					// 부모 노드 연결
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
			else if (pNode->mData == data)
			{
				return pNode;
			}
		}
	}

	//------------------------------------------------------
	// DeleteNode 내부에서 호출되는 노드찾기 & 삭제 & 후처리
	//------------------------------------------------------
	void deleteNode(StructNode* pNode)
	{
		if (pNode->mLeft == nullptr && pNode->mRight == nullptr)
		{
			// 노드 수 1개 감소
			this->mNodeCount -= 1;

			// 부모의 왼쪽 노드인지 오른쪽 노드인지 확인 후 노드 자르기 /
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
		else if (pNode->mLeft == nullptr && pNode->mRight != nullptr)
		{
			if (pNode->mParentsNode != nullptr)
			{
				// 삭제할 노드는 부모 노드의 왼쪽인지 오른쪽인지
				if (pNode->mParentsNode->mLeft == pNode)
				{
					// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
					pNode->mParentsNode->mLeft = pNode->mRight;

					// 연결할 노드의 부모를 삭제할 노드 부모의 연결wwww
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
				// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
				pNode->mParentsNode->mLeft = pNode->mLeft;

				// 연결할 노드의 부모를 삭제할 노드 부모의 연결
				pNode->mLeft->mParentsNode = pNode->mParentsNode;

			}
			else
			{
				// 부모의 왼쪽과 삭제할 노드의 오른쪽 연결
				pNode->mParentsNode->mRight = pNode->mLeft;

				// 연결할 노드의 부모를 삭제할 노드 부모의 연결
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
						if (pNode->mParentsNode->mLeft == pNode)
						{
							// 부모 노드 자식 노드 끊기
							pNode->mParentsNode->mLeft = nullptr;

							// 삭제된 데이터 자리에 왼쪽에서 가장 오른쪽 노드 추가
							pNode->mParentsNode->mData = pNode->mData;
						}
						else
						{
							pNode->mParentsNode->mRight = nullptr;

							deleteNodeData->mData = pNode->mData;
						}


						// 노드 수 1개 감소
						this->mNodeCount -= 1;

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



		//printf_s("xPosition : %d, Depth : %d, Data : %d, 부모 좌표 : %d\n", pNode->mX, this->mDepth, pNode->mData, parentsX);	

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

	//	//printf_s("xPosition : %d, Depth : %d, Data : %d, 부모 좌표 : %d\n", pNode->mX, this->mDepth, pNode->mData, parentsX);

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


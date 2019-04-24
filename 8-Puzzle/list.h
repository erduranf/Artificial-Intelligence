#pragma once
typedef struct Node Node;

typedef struct ListNode { //this is the node structure in a linkedlist
	Node *cur;
	ListNode *prev;
	ListNode *next;
} ListNode;

typedef struct NodeList { //this is the linkedlist we will use for the fringe
	unsigned int nodeCount;
	ListNode *head; //first node in list
	ListNode *tail; //last node in list
} NodeList;

typedef struct sol_Path { //holds the solution path
	Move action;
	sol_Path *next;
} sol_Path;

void destroySolution(sol_Path **list) { //deallocates solution path
	sol_Path *next;
	while (*list) {
		next = (*list)->next;
		delete *list;
		*list = next;
	}
	*list = nullptr;
}

//pushes a node to the list, returns true on success, false on failure
bool push(Node *node, NodeList** const list) { 
	if (!node)
		return false;

	ListNode *doublyNode = new ListNode;
	if (!doublyNode)
		return false;
	doublyNode->cur = node;

	if (*list && !(*list)->nodeCount) { //if the list is allocated but it has no nodes
		(*list)->head = doublyNode;
		(*list)->tail = doublyNode;
		doublyNode->next = nullptr;
		doublyNode->prev = nullptr;
		(*list)->nodeCount++;
		return true;
	}

	if (*list == nullptr) { //if the list has no memory allocated
		*list = new NodeList;
		if (*list == nullptr)
			return false;
		(*list)->nodeCount = 0;
		(*list)->head = nullptr;
		(*list)->tail = doublyNode;
	}
	else {
		(*list)->head->prev = doublyNode;
	}

	doublyNode->next = (*list)->head;
	doublyNode->prev = nullptr;
	(*list)->head = doublyNode;
	(*list)->nodeCount++;
	return true;
}

//removes a node from the tail; used in BFS; FIFO structure
Node* BFS_pop(NodeList** const list) { 
	if (!list || !(*list)->nodeCount)
		return nullptr;

	Node *popped = (*list)->tail->cur;
	ListNode *prevNode = (*list)->tail->prev;
	delete (*list)->tail;

	if (!*list || !(*list)->nodeCount)
		return nullptr;

	if ((*list)->nodeCount == 1)
		(*list)->head = nullptr;
	else prevNode->next = nullptr;

	(*list)->tail = prevNode;
	(*list)->nodeCount--;
	return popped;
}

//removes a node from the head; used in DFS and its variants; LIFO structure
Node* DFS_pop(NodeList** const list) {
	if (!list || !(*list)->nodeCount)
		return nullptr;

	Node *popped = (*list)->head->cur;
	ListNode *nextNode = (*list)->head->next;
	delete (*list)->head;

	if (!*list || !(*list)->nodeCount)
		return nullptr;

	if ((*list)->nodeCount == 1)
		(*list)->tail = nullptr;
	else nextNode->prev = nullptr;

	(*list)->head = nextNode;
	(*list)->nodeCount--;
	return popped;
}

//appends the source to the head of destination; source is deallocated
void pushList(NodeList **source, NodeList *dest) {
	//if either list is NULL; or source's head is empty; or the two are the same
	if (!*source || !dest || !(*source)->head || (*source)->head == dest->head) return;

	if (!dest->nodeCount) { //is source has no elements
		dest->head = (*source)->head;
		dest->tail = (*source)->tail;
	}
	else {  //connect the lists
		(*source)->tail->next = dest->head;
		dest->head->prev = (*source)->tail;
		dest->head = (*source)->head;
	}
	dest->nodeCount += (*source)->nodeCount; //update node count
	delete *source;
	*source = nullptr;
}

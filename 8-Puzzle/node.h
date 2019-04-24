#pragma once
extern unsigned int maxFringeSize; //declared in main.cpp

typedef struct Node { //representation of a state
	unsigned int depth; //distance from the root
	State *state;
	Node *parent;
	NodeList *children; 
} Node;

//creates a node; sets its children to NULL; returns a pointer to the node on success, NULL on failure
Node* createNode(unsigned int d, State *s, Node *parent) {
	Node *newNode = new Node;
	if (newNode) {
		newNode->depth = d;
		newNode->state = s;
		newNode->parent = parent;
		newNode->children = nullptr;
		maxFringeSize++;
	}
	return newNode;
}

//deallocates all nodes starting from the root
void destroyTree(Node *node) {
	if (node->children == nullptr) {
		delete node->state;
		delete node;
		return;
	}

	ListNode *listNode = node->children->head;
	ListNode *nextNode;
	while (listNode) {
		nextNode = listNode->next;
		destroyTree(listNode->cur);
		listNode = nextNode;
	}
	delete node->children;
	delete node;
}

//expands the node and links it to its children; returns a NodeList pointer on success, NULL on failure
NodeList* expandNode(Node *parent) {
	NodeList  *childrenList = nullptr;
	Node *child = nullptr;
	State *test = nullptr;

	//tries to create children for all 4 of the possible moves
	if (parent->state->action != RIGHT && (test = createState(parent->state, LEFT))) {
		child = createNode(parent->depth + 1, test, parent);
		push(child, &parent->children);
		push(child, &childrenList);
	}
	if (parent->state->action != LEFT && (test = createState(parent->state, RIGHT))) {
		child = createNode(parent->depth + 1, test, parent);
		push(child, &parent->children);
		push(child, &childrenList);
	}
	if (parent->state->action != DOWN && (test = createState(parent->state, UP))) {
		child = createNode(parent->depth + 1, test, parent);
		push(child, &parent->children);
		push(child, &childrenList);
	}
	if (parent->state->action != UP && (test = createState(parent->state, DOWN))) {
		child = createNode(parent->depth + 1, test, parent);
		push(child, &parent->children);
		push(child, &childrenList);
	}
	return childrenList;
}

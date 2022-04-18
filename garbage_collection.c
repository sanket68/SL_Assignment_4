#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node_tag
{
	int value;
	bool mark;	   // for marking the nodes to use mark and sweep mechanism
	int ref_count; // reference count mechanism
	struct node_tag *link1;
	struct node_tag *link2;
	struct node_tag *link3;
	// Maximum three links are shown in the diagram
} Node;
// array of Node pointer
Node *array_of_pointers[8];

// function to display the node
void display_node(int i);

// function to set edges between nodes
void set_links(int so, int destination1, int destination2, int destination3);

// function to display the nodes of root
void display_all_nodes(Node *root);

// function to print adjacency_list
void adjacency_list();

// function to check weather the root is present or not
int check_root_present(Node *root1, Node *temp);

// function to referal counting mechanism
void garbage_collector(Node *root);

// function toprint adjacency matrix
void adjacency_Matrix();

// function to mark the nodes
void marking_mechanism(Node *root);
// function sweep mechanism

void sweep_mechanism();
// function to initialize the array of pointers
void initialize(int arr[]);

int main()
{
	// Node number  		   	   0,1,2,3,4,5,6,7
	// value
	int arr[] = {1, 2, 3, 5, 7, 8, 9, 10};

	// initialize the array of pointers
	initialize(arr);

	Node *root1 = array_of_pointers[3];
	array_of_pointers[3]->ref_count += 1;

	Node *root2 = array_of_pointers[0];
	array_of_pointers[0]->ref_count += 1;

	// calling function to set edges with links
	set_links(0, 1, 6, 7);
	set_links(2, 5, 7, -1);
	set_links(3, 0, -1, -1);
	set_links(4, 0, 5, -1);
	set_links(5, 6, -1, -1);

	int flag = 1;
	printf("call function like entering 1 -> 2 -> 3 -> 4 ->2 ->3 \n");
	while (flag = 1)
	{	printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("enter 1 to display all the nodes \nenter 2 to display all adajacency list \nenter 3 to display adjacency matrix \nenter 4 to display mark and sweep \nenter 0 to exit\nenter your choice: ");
		int choice;
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("\nAll nodes through Root-1:\n");
			display_all_nodes(root1);

			printf("\nAll nodes through Root-2:\n");
			display_all_nodes(root2);
			break;
		case 2:
			printf("\n\nAdjacency list :\n"); // Displaying Adjacency list of the nodes with corresponding value or vertex
			adjacency_list();
			break;
		case 3:
			printf("\n\nAdjacency matrix:\n"); // Displaying Adjacency Matrix of the nodes rather than root 1 and root 2
			adjacency_Matrix();
			break;
		case 4:
			printf("\nCalling the marking  and sweep garbage collector\n");
			marking_mechanism(root1);
			sweep_mechanism();
			break;
		case 0:
			return 0;
			break;
		default:
			printf("invalid choice");
			break;
		}
	}

	return 0;
}

void initialize(int arr[])
{
	int i;
	for (i = 0; i < 8; i++)
	{
		Node *newNode = (Node *)malloc(sizeof(Node));
		newNode->value = arr[i];
		newNode->link1 = NULL;
		newNode->link2 = NULL;
		newNode->link3 = NULL;
		newNode->ref_count = 0;
		newNode->mark = false;
		array_of_pointers[i] = newNode;
	}
}

// function to display reference count and freed size
void display_node(int i)
{
	printf("value=%d\t reference_count=%d \t freed_size=%d\n", array_of_pointers[i]->value, array_of_pointers[i]->ref_count, sizeof(Node));
}

void set_links(int i, int destination1, int destination2, int destination3)
{
	if (destination1 != -1)
	{
		array_of_pointers[i]->link1 = array_of_pointers[destination1];
		array_of_pointers[destination1]->ref_count += 1;
	}
	if (destination2 != -1)
	{
		array_of_pointers[i]->link2 = array_of_pointers[destination2];
		array_of_pointers[destination2]->ref_count += 1;
	}
	if (destination3 != -1)
	{
		array_of_pointers[i]->link3 = array_of_pointers[destination3];
		array_of_pointers[destination3]->ref_count += 1;
	}
}

void display_all_nodes(Node *root)
{
	if (root != NULL)
	{
		printf("value=%d \t ref_count=%d\n", root->value, root->ref_count);
	}
	if (root == NULL)
	{
		return;
	}
	display_all_nodes(root->link1);
	display_all_nodes(root->link2);
	display_all_nodes(root->link3);
}

void adjacency_list()
{
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		if (array_of_pointers[i] != NULL)
		{
			printf("Value=%d: ", array_of_pointers[i]->value);
			if (array_of_pointers[i]->link1 != NULL)
			{
				printf("%d ->", array_of_pointers[i]->link1->value);
			}
			if (array_of_pointers[i]->link2 != NULL)
			{
				printf("%d ->", array_of_pointers[i]->link2->value);
			}
			if (array_of_pointers[i]->link3 != NULL)
			{
				printf("%d ->", array_of_pointers[i]->link3->value);
			}
			printf("NULL\n");
		}
	}
}

int check_root_present(Node *root1, Node *temp)
{
	if (root1 == NULL)
	{
		return 0;
	}
	if (root1->value == temp->value)
	{
		return 1;
	}

	if (check_root_present(root1->link1, temp))
	{
		return 1;
	}

	if (check_root_present(root1->link2, temp))
	{
		return 1;
	}
	if (check_root_present(root1->link3, temp))
	{
		return 1;
	}
	return 0;
}

void garbage_collector(Node *root)
{
	int i = 0;
	while (i < 8)
	{
		if (check_root_present(root, array_of_pointers[i]) == 0)
		{
			if (array_of_pointers[i]->link1 != NULL)
			{
				array_of_pointers[i]->link1->ref_count -= 1;
			}
			if (array_of_pointers[i]->link2 != NULL)
			{
				array_of_pointers[i]->link2->ref_count -= 1;
			}
			if (array_of_pointers[i]->link3 != NULL)
			{
				array_of_pointers[i]->link3->ref_count -= 1;
			}
			printf("Garbage:");
			display_node(i);
			free(array_of_pointers[i]);
			array_of_pointers[i] = NULL;
		}
		i++;
	}
}

void adjacency_Matrix()
{
	int adm[8][8];
	int i, j, k;

	for (i = 0; i < 8; i++) // initialising all indices values to 0
	{
		for (j = 0; j < 8; j++)
		{
			adm[i][j] = 0;
		}
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{

			if (array_of_pointers[j] != NULL && array_of_pointers[i] != NULL)
			{

				if (array_of_pointers[i]->link1 != NULL)
				{
					if (array_of_pointers[i]->link1->value == array_of_pointers[j]->value && i != j)
					{
						adm[i][j] = 1;
					}
				}
				if (array_of_pointers[i]->link2 != NULL)
				{
					if (array_of_pointers[i]->link2->value == array_of_pointers[j]->value && i != j)
					{
						adm[i][j] = 1;
					}
				}
				if (array_of_pointers[i]->link3 != NULL)
				{
					if (array_of_pointers[i]->link3->value == array_of_pointers[j]->value && i != j)
					{
						adm[i][j] = 1;
					}
				}
			}
		}
	}

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 8; j++)
		{
			printf("%d ", adm[i][j]); // printing the adjacency matrix
		}
		printf("\n");
	}
}

void marking_mechanism(Node *root)
{

	if (root != NULL)
	{
		root->mark = true;
	}
	if (root == NULL)
	{
		return;
	}
	marking_mechanism(root->link1);
	marking_mechanism(root->link2);
	marking_mechanism(root->link3);
}

void sweep_mechanism()
{
	int i;
	for (i = 0; i < 8; i++)
	{
		if (array_of_pointers[i]->mark == false)
		{
			if (array_of_pointers[i]->link1 != NULL)
			{
				array_of_pointers[i]->link1->ref_count -= 1;
			}
			if (array_of_pointers[i]->link2 != NULL)
			{
				array_of_pointers[i]->link2->ref_count -= 1;
			}
			if (array_of_pointers[i]->link3 != NULL)
			{
				array_of_pointers[i]->link3->ref_count -= 1;
			}
			printf("Garbage:");
			display_node(i);
			free(array_of_pointers[i]);
			array_of_pointers[i] = NULL;
		}
	}
}

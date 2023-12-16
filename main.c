#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TreeNode {
  int key;
  struct TreeNode* left;
  struct TreeNode* right;
  int height;
}TreeNode;




int max(int a, int b)
{
  return a>b?a:b;
}
int getHeight(TreeNode* node)
{
  if(node == NULL)
    return 0;
  return node->height;
}

TreeNode* createNode(int key) {
  TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
  if (node == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  node->key = key;
  node->left = node->right = NULL;
  node->height = 1;  // Set height to 1 for a new node
  return node;
}

int getBalanceFactor(TreeNode* n)
{
  if(n == NULL)
    return 0;
  return getHeight(n->left) - getHeight(n->right);

}

struct TreeNode *leftRotate(struct TreeNode *root) {
  if (root == NULL || root->right == NULL)
    return root;

  struct TreeNode *rootRightChild = root->right;
  struct TreeNode *childLeft = rootRightChild->left;

  rootRightChild->left = root;
  root->right = childLeft;

  rootRightChild->height = max(getHeight(rootRightChild->right), getHeight(rootRightChild->left)) + 1;
  root->height = max(getHeight(root->right), getHeight(root->left)) + 1;

  return rootRightChild;
}

struct TreeNode *rightRotate(struct TreeNode *root) {
  if (root == NULL || root->left == NULL)
    return root;

  struct TreeNode *rootLeftChild = root->left;
  struct TreeNode *childRight = rootLeftChild->right;

  rootLeftChild->right = root;
  root->left = childRight;

  root->height = max(getHeight(root->right), getHeight(root->left)) + 1;
  rootLeftChild->height = max(getHeight(rootLeftChild->right), getHeight(rootLeftChild->left)) + 1;

  return rootLeftChild;
}

TreeNode* balancedInsert(TreeNode* node, int key) {
  if (node == NULL)
    return createNode(key);

  if (key < node->key)
    node->left = balancedInsert(node->left, key);
  else if (key > node->key)
    node->right = balancedInsert(node->right, key);

  node->height = 1 + max(getHeight(node->left), getHeight(node->right));
  int bf = getBalanceFactor(node);

  // LL
  if (bf > 1 && key < node->left->key)
    return rightRotate(node);
  // RR
  if (bf < -1 && key > node->right->key)
    return leftRotate(node);
  // LR
  if (bf > 1 && key > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }
  // RL
  if (bf < -1 && key < node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

TreeNode *unbalancedInsert(TreeNode *root, int key) {
  if (root == NULL)
    return createNode(key);

  if (key > root->key) {
    root->right = unbalancedInsert(root->right, key);
  } else if (key < root->key) {
    root->left = unbalancedInsert(root->left, key);
  }


  return root;
}

TreeNode* balancedDeleteNode(TreeNode* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = balancedDeleteNode(root->left, key);
    else if (key > root->key)
        root->right = balancedDeleteNode(root->right, key);
    else {
        // Node with only one child or no child
        if (root->left == NULL || root->right == NULL) {
            TreeNode* temp = root->left ? root->left : root->right;

            // No child case
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else // One child case
                *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        } else {
            // Node with two children, get the inorder successor
            TreeNode* temp = root->right;
            while (temp->left != NULL)
                temp = temp->left;

            // Copy the inorder successor's data to this node
            root->key = temp->key;

            // Delete the inorder successor
            root->right = balancedDeleteNode(root->right, temp->key);
        }
    }

    // If the tree had only one node, then return
    if (root == NULL)
        return root;

    // Update height of the current node
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    // Get the balance factor
    int balance = getBalanceFactor(root);

    // Perform rotations if needed to balance the tree
    // LL
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // LR
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

TreeNode* unbalancedDeleteMin(TreeNode* root) {
  if(!root)
    return NULL;
  if (root->left == NULL) {
    TreeNode* temp = root->right;
    free(root);
    return temp;
  }

  root->left = unbalancedDeleteMin(root->left);
  return root;
}
TreeNode* balancedDeleteMin(TreeNode* root) {
    if (root == NULL)
        return NULL;

    if (root->left == NULL) {
        TreeNode* temp = root->right;
        free(root);
        return temp;
    }

    root->left = balancedDeleteMin(root->left);

    // Update height and balance factor
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int bf = getBalanceFactor(root);

    // Perform balancing rotations
    // RR
    if (bf < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // RL
    if (bf < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
void deleteTree(TreeNode* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}

int main(void) {

  int size = 10;
  int arr10[size];

  for(int i = 0; i < 10; i++)
    {
      int val10 = rand() % 1000;
      arr10[i] = val10;
    }

  size = 100;
  int arr100[size];

  for(int i = 0; i < 100; i++)
    {
      int val100 = rand() % 1000;
      arr100[i] = val100;
    }

  size = 1000;
  int arr1000[size];

  for(int i = 0; i < 1000; i++)
    {
      int val1000 = rand() % 1000;
      arr1000[i] = val1000;

    } 

  size = 100000;
  int arr100000[size];

  for(int i = 0; i < 100000; i++)
    {
      int val100000 = rand() % 1000;
      arr100000[i] = val100000;

    } 

  size = 1000000;
  int arr1000000[size];

  for(int i = 0; i < 1000000; i++)
    {
      int val1000000 = rand() % 1000;
      arr1000000[i] = val1000000;
    }

  size = 10;
  int worstarr10[size];
  for(int i = 0; i < 100; i++)
  {
    worstarr10[i] = i;
  }

  size = 100;
  int worstarr100[size];
  for(int i = 0; i < 1000; i++)
  {
    worstarr100[i] = i;
  }

  size = 1000;
  int worstarr1000[size];
  for(int i = 0; i < 10000; i++)
  {
    worstarr1000[i] = i;
  }



  printf("CPU Time for Average Case Balanced Tree Insertions: \n");
  TreeNode* root = NULL;


  clock_t begin = clock();
  for(int i = 0; i < 10; i++)
    {
      root = balancedInsert(root, arr10[i]);

    }
  clock_t end = clock();

 


  double time_spent10 = (double)(end - begin) / CLOCKS_PER_SEC; // clock end for 10 inserts
  printf("%lf \n", time_spent10);

  begin = clock();
    int delVal10 = rand() % 10;
    root = balancedDeleteNode(root, delVal10);
    end = clock();
    time_spent10 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for delete operation: %.6lf seconds\n", time_spent10);

    // Print time for delete_min operation
    begin = clock();
    root = balancedDeleteMin(root);
    end = clock();
    time_spent10 = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for delete_min operation: %.6lf seconds\n", time_spent10);


  TreeNode* root2 = NULL;


  clock_t begin2 = clock();
  for(int i = 0; i < 100; i++)
    {

      root2 = balancedInsert(root2, arr100[i]);

    }
  clock_t end2 = clock();
  double time_spent100 = (double)(end2 - begin2) / CLOCKS_PER_SEC; // clock end for 100 inserts
  printf("%lf \n", time_spent100);




  TreeNode* root3 = NULL;


  clock_t begin3 = clock();
  for(int i = 0; i < 1000; i++)
    {

      root3 = balancedInsert(root3, arr1000[i]);

    }
  clock_t end3 = clock();
  double time_spent1000 = (double)(end3 - begin3) / CLOCKS_PER_SEC; // clock end for 1000 inserts
  printf("%lf \n", time_spent1000);


  TreeNode* root4 = NULL;


  clock_t begin4 = clock();
  for(int i = 0; i < 100000; i++)
    {

      root4 = balancedInsert(root4, arr100000[i]);

    }
  clock_t end4 = clock();
  double time_spent100000 = (double)(end4 - begin4) / CLOCKS_PER_SEC; // clock end for 100000 inserts
  printf("%lf \n", time_spent100000);


  TreeNode* root5 = NULL;


  clock_t begin5 = clock();
  for(int i = 0; i < 1000000; i++)
    {
      root5 = balancedInsert(root5, arr1000000[i]);

    }
  clock_t end5 = clock();
  double time_spent1000000 = (double)(end5 - begin5) / CLOCKS_PER_SEC; // clock end for 1000000 inserts
  printf("%lf \n", time_spent1000000);


printf("\nCPU Time for Average Case Unbalanced Tree Insertions: \n");
  TreeNode *uroot = NULL;
  clock_t ubegin10 = clock();
  for (int i = 0; i < 10; i++) // unbalanced 10 insertions
  {
   
    uroot = unbalancedInsert(uroot, arr10[i]);
  }
  clock_t uend10 = clock();
  double utime_spent10 = (double)(uend10 - ubegin10) / CLOCKS_PER_SEC;
  printf("\n");
  printf("%f ", utime_spent10);

  TreeNode *uroot2 = NULL;
  clock_t ubegin100 = clock();
  for (int i = 0; i < 100; i++) // unbalanced 100 insertions
  {
  
    uroot2 = unbalancedInsert(uroot2, arr100[i]);
  }
  clock_t uend100 = clock();
  double utime_spent100 = (double)(uend100 - ubegin100) / CLOCKS_PER_SEC;
  printf("\n");
  printf("%f ", utime_spent100);

  TreeNode *uroot3 = NULL;
  clock_t ubegin1000 = clock();
  for (int i = 0; i < 1000; i++) // unbalanced 1,000 insertions
  {
    uroot3 = unbalancedInsert(uroot3, arr1000[i]);
  }
  clock_t uend1000 = clock();
  double utime_spent1000 = (double)(uend1000 - ubegin1000) / CLOCKS_PER_SEC;
  printf("\n");
  printf("%f ", utime_spent1000);

  TreeNode *uroot4 = NULL;
  clock_t ubegin100000 = clock();
  for (int i = 0; i < 100000; i++) // unbalanced 10,00000 insertions
  {
    uroot4 = unbalancedInsert(uroot4, arr100000[i]);
  }
  clock_t uend100000 = clock();
  double utime_spent100000 = (double)(uend100000 - ubegin100000) / CLOCKS_PER_SEC;
  printf("\n");
  printf("%f ", utime_spent100000);

  TreeNode *uroot5 = NULL;
  clock_t ubegin1000000 = clock();
  for (int i = 0; i < 1000000; i++) // unbalanced 10000000 insertions
  {
    uroot5 = unbalancedInsert(uroot5,arr1000000[i]);
  }
  clock_t uend1000000 = clock();
  double utime_spent1000000 = (double)(uend1000000 - ubegin1000000) / CLOCKS_PER_SEC;
  printf("\n");
  printf("%f ", utime_spent1000000);

  return 0;
}
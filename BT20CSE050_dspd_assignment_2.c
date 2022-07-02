/* Assumptions:-
             1.Dates entered here are of same month
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// An AVL tree node
typedef struct book_DB
{

    struct book_DB *left;
    struct book_DB *right;
    char Titel[40];
    char subject[20];
    char author[30];
    int number_of_copies_issued;
    int number_of_copies_available;
    int id;
    int height;
} book_DB;

typedef struct request_queue
{
    char Name_of_the_student[20];
    char title_of_the_book[20];
    struct request_queue *next;
} request_queue;

typedef struct borrower_list
{
    char Name_of_the_student[20];
    char title_of_the_book[20];
    int date_of_issue;
    int date_of_return;
    struct borrower_list *next;
} borrower_list;

typedef struct array
{
    char title[30];
    char author[30];
    int no_of_copies;
} array;

struct stack
{
    int size;
    int top;
    book_DB **s;
};

void stackcreate(struct stack *st, int size)
{
    st->size = size;
    st->top = -1;
    st->s = (book_DB **)malloc(st->size * sizeof(book_DB *));
}

void push(struct stack *st, book_DB *x)
{
    if (st->top == st->size - 1)
    {
        printf("stack overflow\n");
    }
    else
    {
        st->top++;
        st->s[st->top] = x;
    }
}

book_DB *pop(struct stack *st)
{
    book_DB *x = NULL;
    if (st->top == -1)
    {
        printf("stack underflow\n");
    }

    else
    {
        x = st->s[st->top--];
    }
    return x;
}

int isEmpty(struct stack st)
{
    if (st.top == -1)
    {
        return 1;
    }
    return 0;
}

int isFull(struct stack st)
{
    return st.top == st.size - 1;
}

int co = 0;
array *a = NULL;
borrower_list *first2 = NULL, *last2 = NULL;
request_queue *first3 = NULL, *last3 = NULL, *tfirst3 = NULL, *tlast3 = NULL, *Dfirst = NULL, *Dlast = NULL;

// A utility function to get maximum of two integers
int max(int a, int b);

// A utility function to get the height of the tree
int height(struct book_DB *N)
{
    if (N == NULL)
        return 0;
    return 1 + max(height(N->left), height(N->right));
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b) ? a : b;
}

/* Helper function that allocates a new node with the given key and
    NULL left and right pointers. */
struct book_DB *newNode(int key, int no_available, int no_issued, char *title, char *sub, char *author)
{
    struct book_DB *node = (struct book_DB *)
        malloc(sizeof(struct book_DB));
    node->id = key;
    node->number_of_copies_available = no_available;
    node->number_of_copies_issued = no_issued;
    strcpy(node->Titel, title);
    strcpy(node->subject, sub);
    strcpy(node->author, author);
    node->left = NULL;
    node->right = NULL;
    node->height = 0; // new node is initially added at leaf
    return (node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above.
struct book_DB *rightRotate(struct book_DB *y)
{
    struct book_DB *x = y->left;
    struct book_DB *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = height(y);
    x->height = height(x);

    // Return new root
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct book_DB *leftRotate(struct book_DB *x)
{
    struct book_DB *y = x->right;
    struct book_DB *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = height(x);
    y->height = height(y);

    // Return new root
    return y;
}

// Get Balance factor of node N

int getBalance(struct book_DB *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Recursive function to insert a key in the subtree rooted
// with node and returns the new root of the subtree.
struct book_DB *insert(struct book_DB *node, int key, int no_available, int no_issued, char *title, char *sub, char *author)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
    {
        return (newNode(key, no_available, no_issued, title, sub, author));
    }

    if (key < node->id)
    {
        node->left = insert(node->left, key, no_available, no_issued, title, sub, author);
    }
    else if (key > node->id)
    {
        node->right = insert(node->right, key, no_available, no_issued, title, sub, author);
    }
    else
    { // Equal keys are not allowed in BST
        return node;
    }

    /* 2. Update height of this ancestor node */
    node->height = height(node);

    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->id)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->id)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->id)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->id)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}

// A utility function to print preorder traversal
// of the tree.
// The function also prints height of every node
void preOrder(struct book_DB *root)
{
    if (root != NULL)
    {

        printf("%d\n ", root->number_of_copies_available);
        puts(root->author);
        preOrder(root->left);

        preOrder(root->right);
    }
}

// function to create borrower list
void create_borrower_list(char name[], char title[], int issue, int re)
{

    if (first2 == NULL)
    {
        first2 = (borrower_list *)malloc(sizeof(borrower_list));
        strcpy(first2->Name_of_the_student, name);
        strcpy(first2->title_of_the_book, title);
        first2->date_of_issue = issue;
        first2->date_of_return = re;
        first2->next = NULL;
        last2 = first2;
    }

    else
    {
        last2->next = (borrower_list *)malloc(sizeof(borrower_list));
        last2 = last2->next;
        strcpy(last2->Name_of_the_student, name);
        strcpy(last2->title_of_the_book, title);
        last2->date_of_issue = issue;
        last2->date_of_return = re;
        last2->next = NULL;
    }
}

// function to create request queue
void create_request_queue(char name[], char title[])
{
    borrower_list *temp = first2;
    int flag = 0;
    while (temp)
    {
        if (strcmp(temp->Name_of_the_student, name) == 0)
        {
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if (flag) // students who already borrowed books
    {
        if (tfirst3 == NULL)
        {
            tfirst3 = (request_queue *)malloc(sizeof(request_queue));
            strcpy(tfirst3->Name_of_the_student, name);
            strcpy(tfirst3->title_of_the_book, title);
            tfirst3->next = NULL;
            last3 = tfirst3;
        }
        else
        {
            last3->next = (request_queue *)malloc(sizeof(request_queue));
            last3 = last3->next;
            strcpy(last3->Name_of_the_student, name);
            strcpy(last3->title_of_the_book, title);
            last3->next = NULL;
        }
    }
    else // students who requested books first time
    {
        if (first3 == NULL)
        {
            first3 = (request_queue *)malloc(sizeof(request_queue));
            strcpy(first3->Name_of_the_student, name);
            strcpy(first3->title_of_the_book, title);
            first3->next = NULL;
            tlast3 = first3;
        }
        else
        {
            tlast3->next = (request_queue *)malloc(sizeof(request_queue));
            tlast3 = tlast3->next;
            strcpy(tlast3->Name_of_the_student, name);
            strcpy(tlast3->title_of_the_book, title);
            tlast3->next = NULL;
        }
    }
}

// function to merge above two linked lists
void merge()
{
    if (last3 == NULL)
    {
        last3 = tlast3;
    }
    else if (first3 == NULL)
    {
        first3 = tfirst3;
    }
    else
    {
        tlast3->next = tfirst3;
    }
}

// checks books more than 3 borrowed of not
// return 1 if borrowed more than 3 time and 0 if less than 3 time
int is_borrow(char *s)
{
    borrower_list *temp = first2;
    int i = 0;
    while (temp)
    {
        if (strcmp(s, temp->Name_of_the_student) == 0)
        {
            i++;
        }
        temp = temp->next;
    }
    if (i >= 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// checks book returned on time or not
// return 0 if it returned in time 1 if it is not returned in time
int is_return(char *s)
{
    borrower_list *temp = first2;
    while (temp)
    {
        if (strcmp(s, temp->Name_of_the_student) == 0)
        {
            if ((temp->date_of_return - temp->date_of_issue) > 15)
            {
                return 1;
            }
        }
        temp = temp->next;
    }
    return 0;
}

// return 1 if book is available and 0 if it is not available
int is_available(book_DB *p, char *book)
{
    struct stack stk;
    stackcreate(&stk, 100);

    while (p || !isEmpty(stk))
    {
        if (p)
        {
            if (strcmp(book, p->Titel) == 0 && p->number_of_copies_available > 0)
            {
                return 1;
            }
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
    return 0;
}

// 1. Display the name of the students who will get books.
// conditions 1 are student not borrowed more than 3 books
// 2 student must returned book within 15 days
// 3 book must be available in book list
void display_stu_who_get_books(book_DB *root)
{
    request_queue *temp = first3;
    printf("\nstudents who will get the books are\n");
    while (temp)
    {
        int t1 = is_borrow(temp->Name_of_the_student);
        int t2 = is_return(temp->Name_of_the_student);
        int t3 = is_available(root, temp->title_of_the_book);
        if (!(t1) && !(t2) && t3)
        {
            puts(temp->Name_of_the_student);
        }
        temp = temp->next;
    }
}

// counts the appearance of book in request list
int book_count(char *s)
{
    int i = 0;
    request_queue *temp = first3;
    while (temp)
    {
        if (strcmp(s, temp->title_of_the_book) == 0)
        {
            i++;
        }
        temp = temp->next;
    }
    return i;
}

// 2. function to find book which in most demand
void display_book_most_demand(book_DB *p)
{
    struct stack stk;
    stackcreate(&stk, 100);
    int i = 0;

    int max = -1;
    char m_book[40];
    while (p || !isEmpty(stk))
    {
        if (p)
        {
            i = book_count(p->Titel) + p->number_of_copies_issued;
            if (i > max)
            {
                max = i;
                strcpy(m_book, p->Titel);
            }
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
    printf("Book which is in most demand is\n");
    puts(m_book);
}

// 3. Books given in first 3 days
void books_given_in_3_days(book_DB *p)
{
    borrower_list *temp = first2;
    struct stack stk;
    stackcreate(&stk, 100);
    int i = 0;
    while (temp)
    {
        if (temp->date_of_return <= 4 && temp->date_of_return >= 2)
        {
            i++;
        }
        temp = temp->next;
    }

    while (p || !isEmpty(stk))
    {
        if (p)
        {
            i = i + p->number_of_copies_available;
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
    printf("\nWithin first three days maximum books can be given to the students are: %d\n", i);
}

// 4. sort and veiw borrower list in desending order
void borrow_List_Descending()
{
    borrower_list *p = first2;
    borrower_list *q;

    int count;
    char arr[37], arr1[37];

    printf("\nThe borrower_list according to the number of books issued in descending order");
    while (p != 0)
    {
        q = p->next;
        count = 0;
        while (q != 0)
        {
            if (strcmp(q->Name_of_the_student, p->Name_of_the_student) == 0)
            {
                count++;
            }
            q = q->next;
        }
        if (count == 2)
        {
            strcpy(arr, p->Name_of_the_student);
            printf("\n%s", p->Name_of_the_student);
        }
        p = p->next;
    }

    p = first2;
    while (p != 0)
    {
        q = p->next;
        count = 0;
        while (q != 0)
        {
            if (strcmp(q->Name_of_the_student, p->Name_of_the_student) == 0)
            {
                count++;
            }
            q = q->next;
        }
        if (count == 1 && strcmp(arr, p->Name_of_the_student) != 0)
        {
            strcpy(arr1, p->Name_of_the_student);
            printf("\n%s", p->Name_of_the_student);
        }
        p = p->next;
    }

    p = first2;
    while (p != 0)
    {
        q = p->next;
        count = 0;
        while (q != 0)
        {
            if (strcmp(q->Name_of_the_student, p->Name_of_the_student) == 0)
            {
                count++;
            }
            q = q->next;
        }
        if (count == 0 && strcmp(arr, p->Name_of_the_student) != 0 && strcmp(arr1, p->Name_of_the_student) != 0)
        {
            printf("\n%s", p->Name_of_the_student);
        }
        p = p->next;
    }
    printf("\n");
}

// 6. Display the names of the requested books whose copies are available.
void display_re_copies_available(book_DB *root)
{
    request_queue *temp1 = first3;

    struct stack stk;
    stackcreate(&stk, 100);
    printf("names of the requested books whose copies are available:\n");
    while (temp1)
    {
        book_DB *p = root;
        while (p || !isEmpty(stk))
        {
            if (p)
            {
                if (strcmp(p->Titel, temp1->title_of_the_book) == 0 && p->number_of_copies_available > 0)
                {
                    puts(p->Titel);
                    break;
                }
                push(&stk, p);
                p = p->left;
            }
            else
            {
                p = pop(&stk);
                p = p->right;
            }
        }
        temp1 = temp1->next;
    }
}

// 7. checks book is there in borrower list or not
int check_in_borrower(char *s)
{
    borrower_list *temp2 = first2;
    while (temp2)
    {
        if (strcmp(s, temp2->title_of_the_book) == 0)
        {
            break;
        }
        temp2 = temp2->next;
    }
    if (temp2 == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// 7. function to display not issued books
void display_not_issued(book_DB *p)
{
    struct stack stk;
    stackcreate(&stk, 100);
    int check = 0;
    printf("Number of books that not issued by anyone are: \n");
    while (p || !isEmpty(stk))
    {
        if (p)
        {
            check = check_in_borrower(p->Titel);
            if (check)
            {
                puts(p->Titel);
            }
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
}

// 8. Given From-Book-ID and To-Book-ID, display all books with their IDs in the range defined by From-Book-ID and To-Book-ID
void display_fromId_toId(book_DB *p, int from, int to)
{
    struct stack stk;
    stackcreate(&stk, 100);
    printf("\nNumber of books from bookId %d to %d are: \n", from, to);
    while (p || !isEmpty(stk))
    {
        if (p)
        {
            if (p->id >= from && p->id <= to)
            {
                puts(p->Titel);
            }
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
}

// intialize size
void intialize()
{
    a = (array *)malloc(100 * sizeof(array));
}

// creating an array of size 100
void create_array(char *title, char *author, int no_copies)
{

    strcpy(a[co].title, title);
    strcpy(a[co].author, author);
    a[co].no_of_copies = no_copies;
    co++;
}

// supports to create array
void support_create_array(book_DB *p)
{
    struct stack stk;
    stackcreate(&stk, 100);

    while (p || !isEmpty(stk))
    {
        if (p)
        {
            create_array(p->Titel, p->author, p->number_of_copies_available);
            push(&stk, p);
            p = p->left;
        }
        else
        {
            p = pop(&stk);
            p = p->right;
        }
    }
}

// A function to implement bubble sort
void bubbleSort()
{
    int i, j;
    for (i = 0; i < co - 1; i++)
    {

        // Last i elements are already in place
        for (j = 0; j < co - i - 1; j++)
        {
            if (a[j].no_of_copies > a[j + 1].no_of_copies)
            {
                array temp;
                temp.no_of_copies = a[j].no_of_copies;
                a[j].no_of_copies = a[j + 1].no_of_copies;
                a[j + 1].no_of_copies = temp.no_of_copies;

                strcpy(temp.title, a[j].title);
                strcpy(a[j].title, a[j + 1].title);
                strcpy(a[j + 1].title, temp.title);

                strcpy(temp.author, a[j].author);
                strcpy(a[j].author, a[j + 1].author);
                strcpy(a[j + 1].author, temp.author);
            }
        }
    }
}

// 5. displays sorted book list
void printArray()
{
    int i;
    printf("\nBook list in sorted order is as follows\n");
    for (i = 1; i < co; i++)
    {
        printf("Title of book is: ");
        puts(a[i].title);
        printf("Author of book is: ");
        puts(a[i].author);
        printf("Number of copies available are: %d\n\n", a[i].no_of_copies);
    }
}

// Main function
int main()
{
    book_DB *root = NULL;
    int n;
    char titel[40], subject[20], author[30];
    int copies_issued, copies_available, id;
    printf("\n\nAssumption of a code dates that we entered here are of same month\n\n");
    printf("Enter number of books that you want to add\n");
    scanf("%d", &n);
    fflush(stdin);
    for (int i = 0; i < n; i++)
    {
        printf("Enter book id\n");
        scanf("%d", &id);
        fflush(stdin);
        printf("Enter title of book\n");
        gets(titel);
        printf("Enter subject of book\n");
        gets(subject);
        printf("Enter author of book\n");
        gets(author);
        printf("Enter number of copies that issued\n");
        scanf("%d", &copies_issued);
        printf("Enter number of copies that available\n");
        scanf("%d", &copies_available);
        fflush(stdin);
        root = insert(root, id, copies_available, copies_issued, titel, subject, author);
    }

    printf("\n");

    int nb;
    char stu_name[20], bo_title[20];
    int date_i, date_re;

    printf("Enter no. of students who borrowed the book\n");
    scanf("%d", &nb);
    fflush(stdin);
    for (int i = 0; i < nb; i++)
    {
        printf("Enter name of student\n");
        gets(stu_name);
        printf("Enter name of book\n");
        gets(bo_title);
        printf("Enter date of issue\n");
        scanf("%d", &date_i);
        printf("Enter date of return\n");
        scanf("%d", &date_re);
        fflush(stdin);
        create_borrower_list(stu_name, bo_title, date_i, date_re);
    }
    printf("\n");

    int rb;
    char r_name[20], r_title[20];
    printf("Enter number of students who request the book\n");
    scanf("%d", &rb);
    fflush(stdin);
    for (int i = 0; i < rb; i++)
    {
        printf("Enter the name of student who request book\n");
        gets(r_name);
        printf("Enter the title of the requested book\n");
        gets(r_title);
        create_request_queue(r_name, r_title);
    }
    merge();

    int choice = 1;

    while (choice > 0 && choice < 9)
    {
        printf("\n__________________________CHOICES________________________\n");
        printf("1. Display the name of the students who will get books.\n");
        printf("2. Display book in most demand\n");
        printf("3. No. of books can be given in three days\n");
        printf("4. Sort and display borrower list in decending order\n");
        printf("5. Sort and display the title and author’s name for all books of a particular subject on the basis of number_of_copies_available.\n");
        printf("6. Display the names of the requested books whose copies are available.\n");
        printf("7. Display the title of all the books which have not been issued by anyone.\n");
        printf("8. Given From-Book-ID and To-Book-ID, display all books with their IDs in the range defined by From-Book-ID and To-Book-ID.\n");
        printf("\n");

        printf("Enter the valid choice to continue\n");
        scanf("%d", &choice);

        if (choice == 1)
        {
            display_stu_who_get_books(root);
        }
        else if (choice == 2)
        {
            display_book_most_demand(root);
        }
        else if (choice == 3)
        {
            books_given_in_3_days(root);
        }
        else if (choice == 4)
        {
            borrow_List_Descending();
        }
        else if (choice == 5)
        {
            co++;
            intialize();
            support_create_array(root);
            bubbleSort();
            printArray();
        }
        else if (choice == 6)
        {
            display_re_copies_available(root);
        }
        else if (choice == 7)
        {
            display_not_issued(root);
        }
        else if (choice == 8)
        {
            int from, to;
            printf("Enter starting id\n");
            scanf("%d", &from);
            printf("Enter ending id\n");
            scanf("%d", &to);
            display_fromId_toId(root, from, to);
        }
        else
        {
            printf("Invalid choice\n");
        }
    }

    printf("\n\nEND OF PROGRAM");
    printf("\n\n-----------------------------------------------------------------------------\n");

    return 0;
}
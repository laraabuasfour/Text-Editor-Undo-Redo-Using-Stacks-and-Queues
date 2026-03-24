// Name : Lara Abu Asfour

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 800

//define structures for linked list node and stacks and queue.

struct Node {
    char data[MAX_LENGTH]; //the data which we store it in node
    char operation[7]; // operation type remove or insert
    int index; // the operation index
    struct Node* next;
};

struct Stack {
    struct Node* top;
};

struct Queue {
    struct Node* front;
    struct Node* rear;
};
// functions prototypes
void initializeStack(struct Stack* stack); // to initialize an empty stack
void initializeQueue(struct Queue* queue); // to initialize an empty queue
void push(struct Stack* stack, char data[], int index, char operation[]);// to push a node onto the stack
struct Node* pop(struct Stack* stack); //to pop the node from the top of the stack
void enqueue(struct Queue* queue, char data[], int index, char operation[]); //to enqueue the node into queue
struct Node* dequeue(struct Queue* queue);
void freeStack(struct Stack* stack); //to free memory from all nodes in a stack
void freeQueue(struct Queue* queue);//to free memory from all nodes in a queue
void loadedTextFromFile(char text[]);//to load the input file which contains the initial text
void printLoadedText(char text[]);//to print loaded text
void insertStringsToText(char text[], char string1[], int index);// to inseart a string to the text
void removeWord(char text[], char word[]);// to remove a word from the text
void performUndoOperation(struct Stack* undoStack, struct Stack* redoStack, char text[]);//to perform undo operation
void performRedoOperation(struct Stack* undoStack, struct Stack* redoStack, char text[]);//to perform redo operation
void printStacks(struct Stack* stack, char* operation);// to print undo stack and redo stack
void saveTextToFile(char text[]);// to save the updated text to the output file
void displayMainMenu();

int main()
{
    char text[MAX_LENGTH];//to store the text
    struct Stack undoStack,redoStack;
    struct Queue addQueue;// to store the added strings
    char string1[MAX_LENGTH];// to store the entered string
    char word[MAX_LENGTH];// to store the entered word
    int index;
    int operation = 0;

    initializeStack(&undoStack);
    initializeStack(&redoStack);
    initializeQueue(&addQueue);

    while(operation != 9){
        displayMainMenu();
        scanf("%d", &operation);

        switch(operation){
        case 1:
            loadedTextFromFile(text);
            printf("The load opearation is successful.\n");
            break;
        case 2:
            printLoadedText(text);
            break;
        case 3:
            getchar();
            printf("Please enter the string you want to insert: ");
            fgets(string1, MAX_LENGTH, stdin);//read string to insert
            printf("Please enter the index to insert the string in it, if you choose -1 the insertion will be at the end: ");
            scanf("%d", &index);
            insertStringsToText(text, string1, index);
            if (index == -1){
                push(&undoStack, string1, index, "Insert");// to push operation onto undo stack
            }
            else {
                push(&undoStack, string1, index, "Insert");
            }
            break;
        case 4:
            printf("Please enter the word you want to remove it: ");
            scanf("%s", word);
            char removed[MAX_LENGTH];
            strcpy(removed, word);// to save removed word for undo operation
            removeWord(text, word);
            push(&undoStack, removed, index, "Remove");
            break;
        case 5:
            performUndoOperation(&undoStack, &redoStack, text);
            break;
        case 6:
            performRedoOperation(&undoStack, &redoStack, text);
            break;
        case 7:
            printStacks(&undoStack, "Undo");
            printStacks(&redoStack, "Redo");
            break;
        case 8:
            saveTextToFile(text);
            printf("the updated text is saved successful in output file.\n");
            break;
        case 9:
            printf("Exit.\n");
            break;
        default:
            printf("Please choose a valid operation from 1 to 9.\n");
        }
    }
    // to free memory before exiting
    freeStack(&undoStack);
    freeStack(&redoStack);
    freeQueue(&addQueue);

    return 0;
}

void displayMainMenu(){
    printf("\nPlease choose an operation from 1 to 9.\n\n");
    printf("1. Loaded the input file which contains the initial text.\n");
    printf("2. Print the loaded text.\n");
    printf("3. Insert strings to the text.\n");
    printf("4. Remove strings from the text.\n");
    printf("5. Perform Undo operation.\n");
    printf("6. Perform Redo operation.\n");
    printf("7. Print the Undo Stack and the Redo stack.\n");
    printf("8. Save the updated text to the output file.\n");
    printf("9. Exit.\n");
}

void initializeStack(struct Stack* stack){
    stack->top = NULL;// set top of stack to null
}

void initializeQueue(struct Queue* queue){
    queue->front = NULL;
    queue->rear = NULL;
}

void push(struct Stack* stack, char data[], int index, char operation[]){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL){
        printf("The memory allocation is failed.\n");
        return;
    }
    strcpy(newNode->data, data);
    newNode->index = index;
    strcpy(newNode->operation, operation);
    newNode->next = stack->top;
    stack->top = newNode;
}

struct Node* pop(struct Stack* stack){
    if (stack->top == NULL)
        return NULL;
    struct Node* poppedNode = stack->top;
    stack->top = stack->top->next;
    return poppedNode;
}
void enqueue(struct Queue* queue, char data[], int index, char operation[]){
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL){
        printf("The memory allocation is failed.\n");
        return;
    }
    strcpy(newNode->data, data);
    newNode->index = index;
    strcpy(newNode->operation, operation);
    newNode->next = NULL;

    if (queue->rear == NULL){
        queue->front = newNode;
    }
    else{
        queue->rear->next = newNode;
    }
    queue->rear = newNode;
}
struct Node* dequeue(struct Queue* queue){
    if (queue->front == NULL){
        return NULL;
    }
    struct Node* dequeuedNode = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL){
        queue->rear = NULL;
    }
    return dequeuedNode;
}
void freeStack(struct Stack* stack){
    struct Node* current = stack->top;
    while(current != NULL){
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}
void freeQueue(struct Queue* queue){
    struct Node* current = queue->front;
    while(current != NULL){
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
}
void loadedTextFromFile(char text[]){
    FILE* input = fopen("input.txt", "r");
    if(input == NULL){
        printf("The file does not open.\n");
        return;
    }
    fgets(text, MAX_LENGTH, input);
    fclose(input);
}

void printLoadedText(char text[]){
    FILE* input = fopen("input.txt", "r");
    if(input == NULL){
        printf("The file does not open.\n");
        return;
    }
    fgets(text, MAX_LENGTH, input);
    fclose(input);
    printf("The Initial Text is : %s\n", text);
}

void insertStringsToText(char text[], char string1[], int index){
    int textLength = strlen(text) ;
    int string1Length = strlen(string1);

    //if index is -1 find the last character and insert the string there
    if (index == -1){
        // to find the index of the last character in the text
        int lastIndex = textLength - 1;
        while (lastIndex >= 0 && (text[lastIndex] == '\n' || text[lastIndex] == '\0')){
               lastIndex--;
               }
        //if no character found start from the beginning
        if (lastIndex < 0){
            index = 0 ;
        }
        else {
            index = lastIndex + 1; // to insert after the last character
        }
    }
    else{
        if (index < textLength && text[index] == '\n'){
            index++;
        }
    }

    // to shift the text to the right to make a space to new string and the space
    for (int i = textLength + string1Length; i>= index + string1Length; i--){
        text[i]= text[i - string1Length];
    }
    // to insert a space between the initial text and new string
    text[index + string1Length] = ' ';

    // to copy new string to the text
    for(int i=0 ; i<string1Length; i++){
        if (string1[i] != '\n'){
            text[index + i] = string1[i];
        }
        else{
            text[index + i] = ' ';
            text[index + i + 1] = '\n';
            index++;
        }
    }
}


void removeWord(char text [], char word[]){
    char* first = strstr(text, word);
    if (first != NULL){
        // to know the index of first occurrence in the text
        int index = first - text;
        int length = strlen(word);
        int textLength = strlen(text);

        for (int i = index ; i<= textLength - length;i++){
            text[i]= text[i + length];
        }
        text[textLength - length] = '\0';
    }
}

void performUndoOperation(struct Stack* undoStack, struct Stack* redoStack, char text[]){
    if (undoStack->top != NULL){
        //pop the top operation from the undo
        struct Node* node = undoStack->top;
        undoStack->top = node->next;
        //perform the inverse operation and push it onto redo
        if (strcmp(node->operation, "Insert")== 0){
            removeWord(text, node->data);
        }
        else if (strcmp(node->operation, "Remove") == 0){
            insertStringsToText(text, node->data, node->index);
        }
        //to push the operation onto redo
        node->next = redoStack->top;
        redoStack->top = node;
    }
}
void performRedoOperation(struct Stack* undoStack, struct Stack* redoStack, char text[]){
    if (redoStack->top != NULL){
        //pop the top operation from the redo
        struct Node* node = redoStack->top;
        redoStack->top = node->next;
        //perform the inverse operation and push it onto undo
        if (strcmp(node->operation, "Insert")== 0){
            insertStringsToText(text, node->data, node->index);
        }
        else if (strcmp(node->operation, "Remove") == 0){
            removeWord(text, node->data);
        }
        //to push the operation onto undo
        node->next = undoStack->top;
        undoStack->top = node;
    }
}

void printStacks(struct Stack* stack, char* operation) {
    printf(" \tThe %s Stack \n", operation);
    struct Node* current = stack->top;
    int count = 0;
    while (current != NULL) {
        printf("\t%s\t%s\t%d\n", current->data, current->operation, current->index);
        current = current->next;
        count++;
    }
    if (count == 0) {
        printf("\t(Empty)\n");
    }
    printf("\n");
}

void saveTextToFile(char text[]){
    FILE* output = fopen("output.txt", "w");
    if (output == NULL){
        printf("The file does not open.\n");
        return;
    }
    fprintf(output, "%s\n", text);
    fclose(output);
}

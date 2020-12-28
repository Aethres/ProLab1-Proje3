#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Node
{
    char word[100];
    int count;
    struct Node *next;
} Node;

void add(Node *list, Node *addNode, int index);
void addToStart(Node *list, Node *addNode);
void addToEnd(Node *list, Node *addNode);
void printList(Node *list);
int listSize(Node *list);
Node *newNode(char *word, int count);
Node *wordList(char *string);
bool isExist(Node *list, char *string);
int countWords(char *string, char *word);
int findIndex(Node *list, int count);
char *readFile();

int main()
{
    Node *list = wordList(readFile());

    printList(list);
}

void add(Node *list, Node *addNode, int index)
{
    if (index == 0)
        addToStart(list, addNode);
    else
    {
        int i = 0;
        while (list->next != NULL && i < index - 1)
        {
            list = list->next;
            i++;
        }

        addNode->next = list->next;
        list->next = addNode;
    }
}

void addToStart(Node *list, Node *addNode)
{
    char tempChar[100];
    strcpy(tempChar, list->word);
    int tempInt = list->count;

    strcpy(list->word, addNode->word);
    list->count = addNode->count;

    strcpy(addNode->word, tempChar);
    addNode->count = tempInt;

    addNode->next = list->next;

    list->next = addNode;
}

void addToEnd(Node *list, Node *addNode)
{
    while (list->next != NULL)
    {
        list = list->next;
    }

    list->next = addNode;
}

void printList(Node *list)
{
    int count = 1;

    while (list != NULL)
    {
        printf("%d. %s: %d\n", count, list->word, list->count);
        list = list->next;
        count++;
    }
}

int listSize(Node *list)
{
    int count = 0;
    while (list != NULL)
    {
        list = list->next;
        count++;
    }

    return count;
}

Node *newNode(char *word, int count)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->count = count;
    strcpy(newNode->word, word);
    newNode->next = NULL;

    return newNode;
}

Node *wordList(char *string)
{
    char *token;
    char delim[] = " \n";
    int count = 0;
    char *tempString = strdup(string);

    token = strtok(tempString, delim);

    Node *list = newNode(token, countWords(string, token));

    while (token != NULL)
    {
        if (!isExist(list, token))
        {
            count = countWords(string, token);
            int index = findIndex(list, count);
            Node *temp = newNode(token, count);

            if (index == 0)
            {
                addToStart(list, temp);
            }
            else if (index >= listSize(list))
            {
                addToEnd(list, temp);
            }
            else
            {
                add(list, temp, index);
            }
        }

        token = strtok(NULL, delim);
    }

    return list;
}

bool isExist(Node *list, char *string)
{
    while (list != NULL)
    {
        if (strcmp(list->word, string) == 0)
        {
            return true;
        }
        list = list->next;
    }

    return false;
}

int countWords(char *string, char *word)
{
    char *token;
    char delim[] = " \n";
    int count = 0;
    char *tempString = strdup(string);

    while (token = strtok_r(tempString, delim, &tempString))
    {
        if (strcmp(token, word) == 0)
        {
            count++;
        }
    }

    return count;
}

int findIndex(Node *list, int count)
{
    int index = 0;

    while (list != NULL)
    {
        if (count >= list->count)
        {
            return index;
        }

        index++;
        list = list->next;
    }

    return index;
}

char *readFile()
{
    char *token;
    char delim[] = " \n";

    FILE *file;
    char *string = NULL;
    int length = 0;
    int count = 0;

    if ((file = fopen("input.txt", "r")) == NULL)
    {
        printf("\nCant open file\n");
        return "\nCant open file\n";
    }

    fseek(file, 0, SEEK_END);   //https://stackoverflow.com/questions/3463426/in-c-how-should-i-read-a-text-file-and-print-all-strings
    length = ftell(file);
    rewind(file);
    string = malloc(sizeof(char) * (length + 1));

    fread(string, sizeof(char), length, file);

    for (int i = 0; i < length; i++)
    {
        if (string[i] == '\n')
        {
            count++;
        }
    }

    string[length - count] = '\0';

    fclose(file);

    return string;
}
#include <stdio.h>

typedef struct hashData * HashDataPtr;
typedef struct listnode * ListNodePtr;
typedef struct listhead * ListHeadPtr;

typedef struct listhead
{
   ListNodePtr head;
   unsigned size;
}ListHead;

typedef struct hashData
{
	char *key;
	ListHead list;
	HashDataPtr next;
}HashData;

typedef struct hashList
{
	int size;
	struct hashData **table;
}HashList;

typedef struct listnode
{
  long int docno;
  long int freq;
  ListNodePtr next;
}ListNode;

/* functions prototype */
HashList *createHash();
int setData ( HashList *, char *, long int);
int hashing( HashList *, char *);
HashData *hashDataBaru( char *);
int insertList(ListHead *, long int );
void hashTraversal(HashList *, int, FILE *, FILE *, FILE *);
int compare_node(const void *, const void *);
void cetakFile(HashData *, FILE *, FILE *);
int readList(ListHead *, FILE *);
void freeHash(HashList *);
void freeList(ListHead *);

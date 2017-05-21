#include <stdio.h>

/**
 * struct for loading data from file vocabulary (fvoc)
 **/

typedef struct hashData * HashDataPtr;

typedef struct hashData
{
	char *key;
	int len;
	long int offset;
  HashDataPtr next;
}HashData;

typedef struct hashList
{
	int size;
	struct hashData **table;
}HashList;


/* struct of heap */
typedef struct heap
{
  long int docno;
  double ranked;
}Heap;


/* struct for accumulator */
typedef struct accumulator
{
  unsigned long docno;
  float ranked;
}Accu;


/* struct for holding docno, doclen and offset. The offset
   is required to retrieve file name in file data.nme */
typedef struct fileinfo
{
  long int docno;
  long int doclen;
  long int offset;
}FileInfo;


/* functions implemented in query-tools.c */
HashList *createHash();
void setData ( HashList *, char *, int, long int );
int hashing( HashList *, char * );
HashData *hashDataBaru( char *);
HashData *getData( HashList *, char *);
int buildHeap(Heap *, int, unsigned long, double);
int adjustHeap(Heap *, int, int);
void freeHash(HashList *);

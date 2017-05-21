#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "query-tools-Hash.h"
#include "define.h"


/* fungsi membuat tabel hash */
HashList *createHash()
{
	HashList *hashTable = NULL;
	int i;

	if( HASHSIZE < 1 ) return NULL;

	/* alokasi memori untuk tabel hash */
	if( ( hashTable = malloc( sizeof( HashList ) ) ) == NULL )
	{
		printf("Memory allocation for Hash List fails...\n");
		return NULL;
	}

	/* alokasi memori untuk setiap tabel */
	if( ( hashTable->table = malloc( sizeof( HashData * ) * HASHSIZE ) ) == NULL )
	{
		printf("Memory allocation for Hash Data fails...\n");
		return NULL;
	}

	/* inisialisasi tabel hash */
	for( i = 0; i < HASHSIZE; i++ )
	{
		hashTable->table[i] = NULL;
	}
	hashTable->size = HASHSIZE;

	return hashTable;
}


/* fungsi memasukkan data ke dalam HashList */
void setData ( HashList *hashTable, char *key, int len, long int offset )
{
	int nilaiHash = 0;
	HashData *dataBaru = NULL;
	HashData *next = NULL;

	/* mendapatkan nilai hash */
	nilaiHash = hashing( hashTable, key );

	/*
	 * mencegah collision maka dibentuk linked list
	 * untuk sesama data tabel yang memiliki nilai hash yang sama
	 */
	next = hashTable->table[ nilaiHash ];
	while( next != NULL && next->key != NULL && strcmp( key, next->key ) != 0 )
	{
		next = next->next;
	}

	/* belum ada dataHash yang sesuai, maka akan dibuat baru */
	if( next == NULL )
	{
		dataBaru = hashDataBaru( key );
    dataBaru->next = hashTable->table[ nilaiHash ];
    hashTable->table[ nilaiHash ] = dataBaru;
    dataBaru->len = len;
    dataBaru->offset = offset;
	}
}


/* fungsi mendapatkan nilai hash */
int hashing( HashList *hashTable, char *key )
{
	unsigned long int hashval = 5381;
	int c;

	while( (c = *key++) )
	{
		hashval = ((hashval << 5) + hashval) + c;
	}

	return hashval % hashTable->size;
}


/* fungsi membuat key-value dari hash */
HashData *hashDataBaru( char *key)
{
	HashData *dataBaru;
	char *term;

	/* alokasi memori dataHash baru */
	if( ( dataBaru = malloc( sizeof( HashData ) ) ) == NULL )
	{
		printf("Memory allocation for Hash Data fails...\n");
		return NULL;
	}

	term = (char *) malloc(strlen(key)+1);
  strcpy(term,key);

	/* inisialisasi */
	if( ( dataBaru->key = term ) == NULL )
	{
		return NULL;
	}
	dataBaru->len=0;
  dataBaru->offset=0;
	dataBaru->next = NULL;

	return dataBaru;
}


/* fungsi mengambil data dari hash */
HashData *getData( HashList *hashTable, char *key)
{
  int nilaiHash = 0;
  HashData *hasil;

  nilaiHash = hashing( hashTable, key );
  hasil = hashTable->table[ nilaiHash ];
	while( hasil != NULL && hasil->key != NULL && strcmp( key, hasil->key ) > 0 )
	{
		hasil = hasil->next;
	}
  if( hasil == NULL || hasil->key == NULL || strcmp( key, hasil->key ) != 0 )
	{
		return NULL;
	}
	else
	{
		return hasil;
	}
}


/* fungsi membersihkan memori HashList */
void freeHash(HashList *hashTable)
{
  int i;
  for(i = 0; i < HASHSIZE; i++)
	{
    if(hashTable->table[i] != NULL)
		{
			HashDataPtr current = hashTable->table[i];
	    HashDataPtr delete = current;

	    while(delete != NULL)
			{
	      current = delete->next;
				free(delete->key);
	      free(delete);
	      delete = current;
	    }
    }
  }
	free(hashTable->table);
	free(hashTable);
}


/* build min heap */
int buildHeap(Heap *heap, int n, unsigned long newdocno, double newranked)
{
  int i=n;

  heap[i].ranked=newranked;
  heap[i].docno=i;
  while((i>0) && (heap[(i-1)/2].ranked > newranked))
  {
    heap[i].ranked=heap[(i-1)/2].ranked;
    heap[i].docno=heap[(i-1)/2].docno;
    i=(i-1)/2;
  }
  heap[i].ranked=newranked;
  heap[i].docno=newdocno;
  return 1;
}


/* adjust heap */
int adjustHeap(Heap *heap, int pos, int HEAPSIZE)
{
  double num = heap[pos].ranked;
  unsigned long docno = heap[pos].docno;
  int i = 2*(pos+1)-1;

  while(i<=HEAPSIZE-1)
  {
	if((i<HEAPSIZE-1) && (heap[i].ranked > heap[i+1].ranked))
	{
	  i++;
    }
    if(num<=heap[i].ranked)
    {
	  break;
    }
    heap[(i-1)/2].ranked = heap[i].ranked;
    heap[(i-1)/2].docno = heap[i].docno;
    i= 2*(i+1)-1;
  }
  heap[(i-1)/2].ranked=num;
  heap[(i-1)/2].docno=docno;
  return 1;
}

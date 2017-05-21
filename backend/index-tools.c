/*
 * Index-tools.c
 *
 * Author: MRA
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "index-tools.h"
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
int setData ( HashList *hashTable, char *key, long int docno )
{
	int nilaiHash = 0;
	HashData *dataBaru = NULL;
	HashData *next = NULL;
	HashData *last = NULL;

	/* mendapatkan nilai hash */
	nilaiHash = hashing( hashTable, key );

	/*
	 * mencegah collision maka dibentuk linked list
	 * untuk sesama data tabel yang memiliki nilai hash yang sama
	 */

	next = hashTable->table[ nilaiHash ];

	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 )
	{
		last = next;
		next = next->next;
	}

	/* dataHash telah ada, tinggal menambahkan informasi data */
	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 )
	{
		insertList(&(next->list),docno);
		return 0; /* tidak ada penambambah kata unik yang baru */
	}
	/* belum ada dataHash yang sesuai, maka akan dibuat baru */
	else
	{
		dataBaru = hashDataBaru( key );

		/* next terletak di awal linkedlist hashTable */
		if( next == hashTable->table[ nilaiHash ] )
		{
			dataBaru->next = next;
			hashTable->table[ nilaiHash ] = dataBaru;
		}
		/* next terletak di akhir linkedlist hashTable */
		else if ( next == NULL )
		{
			last->next = dataBaru;
		}
		/* next terletak di pertengahan linkedlist hashTable */
		else
		{
			dataBaru->next = next;
			last->next = dataBaru;
		}
		insertList(&(dataBaru->list),docno);
		return 1; /* menambah kata unik yang baru */
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
	dataBaru->list.head=NULL;
  dataBaru->list.size=0;
	dataBaru->next = NULL;

	return dataBaru;
}

/* fungsi memasukkan data ke dalam linkedlist */
int insertList(ListHead *ls, long int docno)
{
  ListNodePtr new;
	ListNodePtr next = ls->head;

  /* if it is the same document, then just update the frequency, without having to
     create new linked list node */

	while((next!=NULL) && ((next->docno) != docno))
	{
		next = next->next;
	}

  if((next!=NULL) && ((next->docno) == docno))
  {
    (next->freq)++;
  }
  else
  {
    new = (ListNodePtr) malloc(sizeof(ListNode));
    if(new==NULL)
    {
      printf("Memory allocation for linked list node fails...\n");
      return 0;
    }
    new->docno=docno;
    new->freq=1;
    new->next=ls->head;
    ls->head=new;
    ls->size++;
  }
  return 1;
}

static int totalTerm=0;
/* fungsi membaca seluruh isi hash dan membuat file inverted index */
void hashTraversal(HashList *hashTable, int jmlKataUnik, FILE *finv, FILE *fvoc, FILE *fpar)
{
	/* menalokasikan memori penampung isi hash */
	HashData **sortedHash = malloc(jmlKataUnik * sizeof(HashData*));

	int i,idx=0;
	/* memasukkan semua data hash ke dalam array sortedHash (penampung) */
	for(i=0;i<HASHSIZE;i++)
	{
		HashDataPtr p = hashTable->table[i];
		while (p)
		{
			sortedHash[idx++] = p;
			p = p->next;
		}
	}

	/* melakukan pengurutan data secara ascending */
	qsort(sortedHash, jmlKataUnik, sizeof(HashData*), compare_node);

	/* membuat file inverted index untuk setiap kata yang unik */
	for(i=0; i<jmlKataUnik; i++)
	{
		cetakFile(sortedHash[i], finv, fvoc);
	}

	/* membuat file parameter */
	fwrite(&totalTerm, sizeof(long int),1,fpar);
	free(sortedHash);
	printf("Total Term : %d\n",totalTerm);
}

/* fungsi perbadingan kata pada qsort */
int compare_node(const void *left, const void *right)
{
	const HashDataPtr kiri = *(HashData **)left;
	const HashDataPtr kanan = *(HashData **)right;
	return strcmp(kiri->key,kanan->key);
}

/* fungsi mencetak file inverted index dan file vocabulary */
void cetakFile(HashData *pDataHash, FILE *finv, FILE *fvoc)
{
  if(pDataHash!=NULL)
  {
    long int offset;
    int totalList;

    offset=ftell(finv);                /* get the offset of finv pointer */
    totalList = readList(&(pDataHash->list), finv);
    fprintf(fvoc,"%s %d %ld\n",pDataHash->key, totalList, offset);
    totalTerm++;
  }
}

/* fungsi membaca linked list */
int readList(ListHead *ls, FILE *finv)
{
  int totalvalue=0;
  ListNodePtr current=ls->head;

  while(current!=NULL)
  {
    fwrite(&(current->docno),sizeof(long int),1,finv);
    fwrite(&(current->freq),sizeof(long int),1,finv);
    current=current->next;
    totalvalue+=2;
  }
  return totalvalue;
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
				freeList(&(delete->list));
				free(delete->key);
	      free(delete);
	      delete = current;
	    }
    }
  }
	free(hashTable->table);
	free(hashTable);
}

/* fungsi membersihkan memori LinkedList */
void freeList(ListHead *pListHead)
{
  if(pListHead != NULL)
	{
    ListNodePtr current = pListHead->head;
    ListNodePtr delete = current;

    while(delete != NULL)
		{
      current = delete->next;
      free(delete);
      delete = current;
    }
  }
}

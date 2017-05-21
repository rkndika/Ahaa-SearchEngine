#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <mcheck.h>
#include "query-tools-Hash.h"
#include "util.h"
#include "define.h"
#include "time.h"

/* prototype functions */
void query(FILE *, FILE *, HashList *, char *, int, int);
int getQueryTerm(char *, char [MAXQUERY][WORDLEN]);
int sort(HashList *, int , char [MAXQUERY][WORDLEN]);

/* global variable yang dibutuhkan */
long int totalTerm,totalDocs;
StopList stoplist[TOTLIST];
FileInfo *fileinfo;


/* fungsi utama program query */
int main(int argc, char *argv[])
{
  FILE *fvoc,*finv,*fpar,*finf,*fnme;
  char buffer[BUFLEN],qry[BUFLEN];
  int i,len,HEAPSIZE;
  long int docno,doclen,offset;
  /* variable tabel hash */
  HashList *hashTable;
  clock_t start, end;
  double cpu_time_used;

  /* cek memory leak */
  mtrace();

  /* program berhenti jika tidak ada query */
  if (argc <=1)
  {
    printf("input a query or more!!!\n");
    exit(1);
  }

  /* mengambil query yang diberikan */
  strcpy(qry,argv[1]);


  /**
   * membuka beberapa file yang dibutuhkan
   **/

  if((finv=fopen("index-db/data.inv","rb"))==NULL)
  {
    printf("Cannot open inverted file [data.inv]...\n");
    return 0;
  }

  if((fpar=fopen("index-db/data.par","rb"))==NULL)
  {
    printf("Cannot open parameter file [data.par]...\n");
    return 0;
  }

  if((fvoc=fopen("index-db/data.voc","r"))==NULL)
  {
    printf("Cannot open vocabulary file [data.voc]...\n");
    return 0;
  }
  if((finf=fopen("index-db/data.wdl","r"))==NULL)
  {
    printf("Cannot open information file [data.wdl]...\n");
    return 0;
  }

  if((fnme=fopen("index-db/data.nme","r"))==NULL)
  {
    printf("Cannot open file [data.nme]...\n");
    return 0;
  }

  /* load data stoplist ke memori */
  loadStopList(stoplist);

  /* load parameter file */
  fread(&totalDocs,sizeof(long int),1,fpar);
  fread(&totalTerm,sizeof(long int),1,fpar);
  fclose(fpar);

  /* inisialisasi heapsize, default = 15 */
  HEAPSIZE = argc > 2 ? atoi(argv[2]) : 15;

  /* printf("#Found %ld distict terms in %ld documents\n\n",totalTerm,totalDocs); */
  /* allocate array files of type struct FileInfo and load file information
     in data.inf in the format of [docno] [offset to data.nme] into an array,
     so that binary search can be applied. The data in the array are sorted
     in ascending order */
  fileinfo=(FileInfo *) malloc(sizeof(FileInfo) * (totalDocs + 1));
  for(i=0; fscanf(finf,"%ld %ld %ld",&docno,&doclen,&offset)==3;i++)
  {
    fileinfo[i].docno=docno;
    fileinfo[i].doclen=doclen;
    fileinfo[i].offset=offset;
  }
  fclose(finf);

  /* membuat hash table */
  hashTable = createHash();

  /* baca file vocabulary dan input data ke dalam hash */
  while(fscanf(fvoc,"%s %d %ld",buffer,&len,&offset) != EOF) {
     setData( hashTable,buffer,len,offset);
  }

  fclose(fvoc);

  /*startTiming();*/
  start = clock();
  /* query data pada hash */
  query(finv,fnme,hashTable,qry,totalDocs,HEAPSIZE);
  /*stopTiming();*/
  end = clock();

  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  /* printf("\n#Time required: %f mseconds\n",timingDuration()); */
  printf("informasi\n");
  /*printf("%s\t%d\t%ld\t%ld\t%d\t%f\n","Hash",TOTLIST,totalDocs,totalTerm,HEAPSIZE,timingDuration());*/
  printf("%s\t%d\t%ld\t%ld\t%d\t%f\n","Hash",TOTLIST,totalDocs,totalTerm,HEAPSIZE,cpu_time_used);

  freeHash(hashTable);
  freeStopList(stoplist);
  free(fileinfo);
  fclose(finv);
  fclose(fnme);
  return 0;
}


/* fungsi query data yang diinput dengan melihat informasi didalam hash */
void query(FILE *finv, FILE *fnme, HashList *hashTable, char *query, int N, int HEAPSIZE)
{
  long int  *ilbuf;
  long int  freqwd,docno,offset,tempd;
  int       i,t,len,totalqterm;
  float     fw;
  double    *accumulator;
  double    s_q_d,tempr;
  Heap      *heap;
  char      buffer[BUFLEN];
  char      queryterm[MAXQUERY][WORDLEN];
  HashData  *hashData;
  /**
   * ketersediaan
   * 1 jika kata tersedia
   * 0 jika kata tidak tersedia salah satu atau seluruhnya
   * default : 1
   **/
  int       ketersediaan = 1;


  if(*query!='\0')
  {
    /* *
     * memisahkan query kedalam beberapa kata
     * hanya mengambil term yang bukan stopword
     **/
    totalqterm = getQueryTerm(query,queryterm);

    if(totalqterm==0) /* jika seluruh query adalah stopword */
    {
	     /* printf("All query terms are stopword\n"); */
       printf("stopword\n");
    }
    else
    {
      /* alokasi memori untuk heap, berukuran sesuai user atau default : 15 */
      heap  =  (Heap *) malloc(sizeof(Heap)*HEAPSIZE);

      /* sort is a function to sort query terms by their frequency value in ascending
         order, rare term should examine first in order to heuristically limited the
         accumulator */
      sort(hashTable,totalqterm,queryterm);

      /* allocate array of accumulator and set all to zero */
      accumulator = (double *) malloc(sizeof(double)*N);
      for(i=0;i<N;i++){
        accumulator[i]=0;
      }

      printf("query-start\n");
      printf("%d\n",totalqterm);
      /* for each query terms, calculate the similarity */
      for(i=0;i<totalqterm;i++)
      {
        hashData = getData(hashTable,queryterm[i]);
        if(hashData == NULL)
        {
          /* printf("#Word ['%s'] is not indexed\n", queryterm[i]); */
          printf("%s\t%d\n", queryterm[i],0);
          ketersediaan = 0;
        }
        else
        {
          len = hashData->len;
          offset = hashData->offset;

          /* read inverted list for the term */
          fseek(finv,offset,0);
          ilbuf=(long int *) malloc(sizeof(long int)*(len));
          fread(ilbuf,sizeof(int),len,finv);
          fw = len/2;
          /* printf("#Word ['%s'], fw (num of doc containing the word) = %g\n",queryterm[i],fw); */
          printf("%s\t%d\n", queryterm[i],(int)fw);

          /* for each pair, show the result */
          for(t=0;t<fw;t++)
          {
            docno  = ilbuf[t*2];
            freqwd = ilbuf[t*2+1];
            s_q_d  = log(N/fw+1)*log(freqwd+1);
            accumulator[docno]+=s_q_d;
	        }
          free(ilbuf);
        }
      }

      printf("query-stop\n");

      /* tampilkan jika seluruh query tersedia dalam file data */
      if(ketersediaan != 0)
      {
        /* Normalize accumulator by document length */
        for(i=0;i<N;i++)
        {
          accumulator[i]+=accumulator[i]/fileinfo[docno].doclen;  /*using L(D)*/
        }

        /* build heap of size HEAPSIZE */
        for(i=0;i<N;i++)
        {
          if(i<HEAPSIZE)
          {
            buildHeap(heap,i,i,accumulator[i]);
          }
          else
          {
            /* compare new value with the root of the heap, if the new value is
  	           larger then the root, insert the new value into the heap */
  	        if(accumulator[i]>heap[0].ranked)
            {
              heap[0].ranked=heap[HEAPSIZE-1].ranked;
              heap[0].docno=heap[HEAPSIZE-1].docno;
        	    heap[HEAPSIZE-1].ranked=accumulator[i];
        	    heap[HEAPSIZE-1].docno=i;
            }

            /* adjust heap */
            for(t=(HEAPSIZE/2)-1;t>=0;t--)
  	        {
              adjustHeap(heap,t,HEAPSIZE);
  	        }
          }
        }

        /* sort heap: bubble sort */
        for(i=HEAPSIZE-1;i>0;i--)
        {
          for(t=0;t<i;t++)
          {
    	      if(heap[t].ranked<heap[t+1].ranked)
            {
        	    tempr=heap[t].ranked;
        	    tempd=heap[t].docno;
        	    heap[t].ranked=heap[t+1].ranked;
        	    heap[t].docno=heap[t+1].docno;
        	    heap[t+1].ranked=tempr;
        	    heap[t+1].docno=tempd;
            }
          }
        }

        /* printf("\n#Top %dth documents are:\n",HEAPSIZE); */

        /* print heap */
        for(i=0;i<HEAPSIZE;i++)
        {
          char * filename;
          offset=fileinfo[heap[i].docno].offset;
          fseek(fnme,offset,0);
          fgets(buffer,STRPATH,fnme);
          buffer[strlen(buffer)-1]='\0';  /*eliminate character '\n' sucked by fgets */
          /* mengambil string kedua sebagai nama file */
          strtok(buffer, "\t");
          filename = strtok(NULL, "\t");
          /*printf("Document [%s]\t or docno %ld ranked = %f\n",filename,heap[i].docno,heap[i].ranked);*/
          printf("%s\t %f\n", filename, heap[i].ranked);
        }
      }
      free(accumulator);
      free(heap);
    }
  }
}


/* getQueryTerm is the function to parse the query into
   several terms */
int getQueryTerm(char *query, char queryterm[MAXQUERY][WORDLEN])
{
  char word[WORDLEN];
  char *pw=word;
  char *pq=query;
  int  totalqueryterm=0;

  query[strlen(query)]='\0';     /* replace character '\n' with '\0'I*/
  while(*pq!='\0')
  {
    while(isspace(*pq))
    {
      pq++;
    }

    while(*pq!='\0')
    {
      if(!isalnum(*pq))
      {
      	pq++;
      	break;
      }
      *pw=*pq;
      pq++;
      pw++;
    }
    *pw='\0';
    if(strlen(word)!=0)
    {
      /* check whether the word is a stopword, if
	       it is not, then add to queryterm array */
      if(!isstopword(word,TOTLIST,stoplist))
      {
        strcpy(queryterm[totalqueryterm++],word);
      }
    }
    word[0]='\0';
    pw=word;
  }
  return totalqueryterm;
}


/* fungsi mengurutkan queryterm secara ascending menurut frekuensi */
int sort(HashList *hashTable, int totalqterm, char queryterm[MAXQUERY][WORDLEN])
{
  int i,t;
  char temp[WORDLEN];
  float fw1,fw2;

  for(i=totalqterm-1;i>0;i--)
  {
    for(t=0;t<i;t++)
    {
      HashData *tempData1 = getData(hashTable,queryterm[t]);
      HashData *tempData2 = getData(hashTable,queryterm[t+1]);
      if(tempData1 != NULL){
          fw1=tempData1->len/2;
      }
      if(tempData2 != NULL){
          fw2=tempData2->len/2;
      }

      if(tempData2==NULL || fw1>fw2)              /*sort in ascending order*/
      {
        strcpy(temp,queryterm[t]);
      	strcpy(queryterm[t],queryterm[t+1]);
      	strcpy(queryterm[t+1],temp);
      }
    }
  }
  return 1;
}

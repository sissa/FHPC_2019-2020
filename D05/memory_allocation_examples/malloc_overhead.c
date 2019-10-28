
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */



#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>



#define BASE_CHUNK_SIZE 45        // the number of bytes in my basic brick
#define TOT_BASE_CHUNKS 10000000  // the number of basic bricks I need in memory

void allocate_chunks(int n, int size)
{
  int          i;
  char       **ptr;
  char         buffer[2000];
  const char   key1[] = {"system bytes"};
  const char   key2[] = {"mmap bytes"};
  const char   key3 = '=';
  unsigned long long int ptrs_size, cum_user_size, cum_alloc_size, tot_size;

  int          fd;
  FILE        *file;

  file = fopen("temporary_garbage", "w");
  
  cum_user_size  = 0;
  cum_alloc_size = 0;
  tot_size       = 0;

  ptr = (char**)malloc(n * sizeof(void*));
  
  //  ptrs_size = n * sizeof(void*);
  ptrs_size  = malloc_usable_size(ptr) + sizeof(void**);
  
  
  for(i = 0; i < n; i++)
    {
      ptr[i] = (char*)malloc(size);
      cum_user_size += size;
      cum_alloc_size  += malloc_usable_size(ptr[i]);
      if (i > 0)
	tot_size += abs((char*)(ptr[i]+size) - (char*)(ptr[i-1]+size));
      else
	tot_size = malloc_usable_size(ptr[i]);
    }

  printf("basic allocated block is of %zd bytes (excess of %g %%)\n", malloc_usable_size(ptr[i-1]),
	 ((double)malloc_usable_size(ptr[i-1])/size -1)*100);
  //printf("--> %u\n", abs((char*)ptr[n-1]+size - (char*)ptr[0]));
  
  printf("\tStat for %d chunks of %d bytes::\n"
	 "\t\t     user size: %llu\n"
	 "\t\t pointers size: %llu\n"
	 "\t\tallocated size: %llu (%g)\n"
	 "\t\t      tot_size: %llu (%g)\n",
	 n, size, cum_user_size, ptrs_size, cum_alloc_size, ((double)cum_alloc_size/cum_user_size), tot_size, (double)tot_size/cum_user_size);

  // redirect stderr to a buffer so that to intercept the output of malloc_stats()
  fd = dup(fileno(stderr));
  setbuf(stderr, buffer);
  malloc_stats();
  // redirect stderr to a file, so that not to be bored
  dup2(fileno(file), fileno(stderr));

  unsigned long long sys_bytes = 0;
  unsigned long long mmap_bytes = 0;
  
  char *sptr, *sptr2;
  if ( (sptr = strstr(buffer, key1)) != NULL )    
    {
      // we have found the substring key1
      
      char token[200];

      // this is the end of that line
      sptr2 = strchr(sptr, '\n');
      // copy the line into token
      // note: sadly, we are not checking whether token is large enough..
      strncpy(token, sptr, sptr2-sptr);
      // impose token is null-terminated
      token[sptr2-sptr] = '\0';
      // having now the '"' bytes as a pivot, find the token on its left      
      sptr2 = strtok(token, &key3);
      // .. and the token on its right
      sptr2 = strtok(NULL, &key3);

      // if we succeeded, it should be the wanted number of bytes
      if(sptr2 != NULL)
	sys_bytes = atoll(sptr2);
      else
	printf("warning: I did not succeed in capturing string %s in malloc_stats() output\n",
	       key1);
    }
    
  if ( (sptr = strstr(buffer, key2)) != NULL )
    {
      char token[100];
      
      sptr2 = strchr(sptr, '\n');
      strncpy(token, sptr, sptr2-sptr);
      token[sptr2-sptr] = '\0';
      sptr2 = strtok(token, &key3);
      sptr2 = strtok(NULL, &key3);
      if(sptr2 != NULL)
	mmap_bytes = atoll(sptr2);
      else
	printf("warning: I did not succeed in capturing string %s in malloc_stats() output\n",
	       key2);      
    }


  if(sys_bytes > 0)
    printf("\t\t sys rep. size: %llu (%g)\n", sys_bytes, (double)sys_bytes/cum_user_size);

  if(mmap_bytes > 0)
    printf("\t\tmmap rep. size: %llu (%g)\n", mmap_bytes, (double)mmap_bytes/cum_user_size);

  printf("hit EOF to continue..\n");
  while(fgetc(stdin) != EOF)
    ;
  printf("\n");

  for(i = n-1; i--;)
    free(ptr[i]);

  free(ptr);

  // now restore stderr
  fclose(file);
  clearerr(stderr);

  dup2(fileno(stderr), fd);
  system("rm -f ./temporary_garbage");

  return;
}


int main(int argc, char **argv)
{
  int chunks_num;
  unsigned int n;
  unsigned int tot_allocation_size;
  
  if(argc == 2)
    chunks_num = atoi( *(argv + 1) );
  else
    chunks_num = 0;

  tot_allocation_size = TOT_BASE_CHUNKS * BASE_CHUNK_SIZE;
  

  if(chunks_num == 0)
    for(chunks_num = 1; chunks_num < TOT_BASE_CHUNKS; chunks_num *= 10)
      {      
	printf("allocating %d chunks of %d bytes..\n\t", chunks_num, tot_allocation_size / chunks_num); fflush(stdout);
	allocate_chunks(chunks_num, tot_allocation_size / chunks_num); 
      }
  else
    {
      printf("allocating %d chunks of %d bytes..\n\t", chunks_num, tot_allocation_size / chunks_num); fflush(stdout);
      allocate_chunks(chunks_num, tot_allocation_size / chunks_num);
    }



  return 0;
}

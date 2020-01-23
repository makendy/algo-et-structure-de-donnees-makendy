/*** submitted by Makendy Piton (makendy.piton) on 2018-10-26 16:26:48.022842+00:00 from 82.241.21.236 ***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#define MAX(A,B) ((A > B) ? A : B)

int increasing(int a, int b)
{
   if (a < b)
     return -1;
   return a > b;
}

int decreasing(int a, int b)
{
   if (a > b)
     return -1;
   return a < b;
}

unsigned int_width(int i)
{
  if(i >= 0 && i < 10)
    return 1;
  if(i >= -9 && i < 0)
    return 2;

  int r;
  long int a = i;
  long int q = 10;
  unsigned cpt = 0;

  if(a < 0)
    {
      cpt++;
      a = -1 * a;
    }

  while(a != 0)
    {
      r = a % q;
      if(a > 0)
	a -= r;
      else if(i < 0)
	a += r;
      cpt++;
      q *= 10;
    }
  return cpt;
}

static int widest_number(int a, int b)
{
  if(a >= 0 && b >= 0)
    {
      if(a > b)
	return a;
      return b;
    }
  else if(a < 0 && b < 0)
    {
      if(a < b)
	return a;
      return b;
    }
  if(int_width(a) > int_width(b))
    return a;
  return b;
}

unsigned ints_width(const int* tab, unsigned count)
{
  int num = tab[0];

  for(unsigned i = 1; i < count; i++)
    {
      num = widest_number(num, tab[i]);
    }
  return int_width(num);
}

void print_int_array(FILE* out, const int* tab, unsigned count)
{
  unsigned size_max = ints_width(tab, count);
  unsigned n = int_width(count - 1) + 2;
  unsigned index = n;
  unsigned width;

  for(unsigned i = 0; i < count; i++)
    {
      width = int_width(tab[i]);
      if((index + size_max + 1) > 80)
	{
	  index = n;
	  fprintf(out, "\n");
	}
      if(index == n)
	{
	  int k = int_width(i) + 2;
	  for (unsigned j = k; j < n; j++)
	    {
	      fprintf(out, " ");
	    }
	  fprintf(out, "[%d]", i);
	}
      for (unsigned j = width; j < (size_max + 1); j++)
	{
	  fprintf(out, " ");
	}
      fprintf(out, "%d", tab[i]);
      index += size_max + 1;
    }
  fprintf(out, "\n");
}


void heapify(int *tab, unsigned pos, unsigned n)
{
  unsigned l = 2 * pos + 1;
  unsigned r = 2 * pos + 2;
  unsigned largest;
  if(pos >= n)
  	 return;
  if(l < n && tab[l] > tab[pos])
    largest = l;
  else
    largest = pos;

  if(r < n && tab[r] > tab[largest])
    largest = r;

  if(largest != pos)
  {
    int nb = tab[largest];
    tab[largest] = tab[pos];
    tab[pos] = nb;
    heapify(tab, largest, n);
  }
}

void make_heap(int *tab, unsigned n)
{
  for(unsigned i = n / 2 - 1; i >= 1; i--){
  	 long int r = i;
  	 if(r < 0 || r > n)
  	 	break;
     heapify(tab, i, n);
  }
  heapify(tab, 0, n);
}

bool check_heap(int *heap, unsigned n)
{
    unsigned l;
    unsigned r;
    for(unsigned i = 0; i < n; i++)
    {
	   	l = 2 * i + 1;
	   	r = 2 * i + 2;
	   	if(r < n && heap[i] < heap[r])
	   		return false;
	   	if(l < n && heap[i] < heap[l])
	   		return false;
    }
    return true;
}

static unsigned depth_heap(const int *heap, unsigned node, unsigned n)
{
    if(node >= n)
    	return 0;
    return 1 +  MAX(depth_heap(heap, 2 * node + 1, n), depth_heap(heap, 2 * node + 2, n));
}

void pretty_print_heap(FILE* out, const int* v, unsigned n)
{
	unsigned w = ints_width(v, n);
	unsigned depth = depth_heap(v, 0, n);
	unsigned t = (w + 1) * pow(2, depth - 1);
	unsigned c;
	unsigned index = 0;
	unsigned s;
	unsigned s1;
	unsigned tmp;
	for(unsigned i = 0; i < depth; i++)
	{
	  tmp = 0;
	  c = pow(2, i);
	  s = t / c - w;
	  s1 = s / 2;
	  for(unsigned j = 0; j < c; j++)
	  {
	    if(index < n)
	    {
	    	unsigned w1 = int_width(v[index]);
			if(j != 0){
				for (unsigned k = 0; k < (w1 < w ? (s + w - w1) : s); k++){
					fprintf(out, " ");
					tmp++;
				}
			  	fprintf(out, "%d", v[index]);
			  	tmp += w1;
			}
			else
			{

				for (unsigned k = 0; k < (w1 < w ? (s1 + w - w1) : s1); k++){
					fprintf(out, " ");
					tmp++;
				}
				fprintf(out, "%d", v[index]);
				tmp = tmp + w1;
			}  
		    index++;
	    }
	  }
	  fprintf(out, "\n");
	}
}

int pop_heap(int *heap, unsigned *n)
{
	unsigned m = *n;
    if(m <= 0)
    	return -1;
    
    m -= 1;
    int root = heap[0];
    heap[0] = heap[m];
    heap[m] = root;
    *n = m;
    make_heap(heap, *n);
    return root;
}

void heap_sort(int *tab, unsigned n)
{
	make_heap(tab, n);
    for(unsigned i = n - 1; i >= 1; i--){
	  	long int r = i;
	  	if(r < 0 || r > n)
	  	 	break;
	  	int move = tab[0];
	  	tab[0] = tab[i];
	  	tab[i] = move;
	    heapify(tab, 0, i);
  }
}


void heapify_cmp(int* v, unsigned i, unsigned n, int (*cmp)(int a, int b))
{
	unsigned l = 2 * i + 1;
	unsigned r = 2 * i + 2;
	unsigned largest;
	if(i >= n)
		 return;
	if(l < n && cmp(v[l],v[i]) > 0)
		largest = l;
	else
		largest = i;

	if(r < n && cmp(v[r],v[largest]) > 0)
		largest = r;

	if(largest != i)
	{
		int nb = v[largest];
		v[largest] = v[i];
		v[i] = nb;
		heapify_cmp(v, largest, n, cmp);
	}
}

void make_heap_cmp(int* v, unsigned n, int (*cmp)(int a, int b))
{
	for(unsigned i = n / 2 - 1; i >= 1; i--){
  	 long int r = i;
  	 if(r < 0 || r > n)
  	 	break;
     heapify_cmp(v, i, n, cmp);
  }
  heapify_cmp(v, 0, n, cmp);
}

bool check_heap_cmp(const int* v, unsigned n, int (*cmp)(int a, int b))
{
	unsigned l;
    unsigned r;
    for(unsigned i = 0; i < n; i++)
    {
	   	l = 2 * i + 1;
	   	r = 2 * i + 2;
	   	if(cmp(r, n) < 0 && cmp(v[i],v[r]) < 0)
	   		return false;
	   	if(cmp(l,n) < 0 && cmp(v[i], v[l]) < 0)
	   		return false;
    }
    return true;
}

int pop_heap_cmp(int* heap, unsigned* n, int (*cmp)(int a, int b))
{
	unsigned m = *n;
    if(m <= 0)
    	return -1;
    
    m -= 1;
    int root = heap[0];
    heap[0] = heap[m];
    heap[m] = root;
    *n = m;
    make_heap_cmp(heap, *n, cmp);
    return root;
}

void heap_sort_cmp(int* tab, unsigned n, int (*cmp)(int a, int b))
{
	make_heap_cmp(tab, n, cmp);
    for(unsigned i = n - 1; i >= 1; i--){
	  	long int r = i;
	  	if(r < 0 || r > n)
	  	 	break;
	  	int move = tab[0];
	  	tab[0] = tab[i];
	  	tab[i] = move;
	    heapify_cmp(tab, 0, i, cmp);
  }
}


int main()
{
	
  int a[] = {
    1, 2, 3, 4, 5, 6, 13, -35, 129, -4, 123, -4555, 1341, 2432, 111, 0, 1230
  };

  unsigned asize = sizeof(a)/sizeof(*a);
  puts("original");
  print_int_array(stdout, a, asize);
  puts("decreasing");
  heap_sort_cmp(a, asize, decreasing);
  print_int_array(stdout, a, asize);
  puts("increasing");
  heap_sort_cmp(a, asize, increasing);
  print_int_array(stdout, a, asize);
  return 0;
}
/*** submitted by Makendy Piton (makendy.piton) on 2018-10-26 14:10:18.154113+00:00 from 185.125.226.4 ***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#define MAX(A,B) ((A > B) ? A : B)

uintmax_t cmp_count;
int my_increasing(int a, int b)
{
   ++cmp_count;
   if (a < b)
     return -1;
   return a > b;
}


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

unsigned pivot_first(const int* tab, unsigned l, unsigned r,
                     int (*cmp)(int a, int b))
{
  (void) tab;
  (void) r;
  (void) cmp;
  return l;
}

unsigned pivot_rand(const int* tab, unsigned l, unsigned r,
                    int (*cmp)(int a, int b))
{
  (void) tab;
  return l + (rand() % (r - l));
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

unsigned partition_cmp(int* tab, unsigned b, unsigned e,
                       unsigned p, int (*cmp)(int a, int b))
{
	int i = b - 1;
	int j = e;
	int pivot = tab[p];

	while(1)
	{
		for (i = i + 1; i < e; i++)
		{
			if(cmp(tab[i], pivot) >= 0)
				break;
		}

		for (j = j - 1; j >= 0; j--)
		{
			if(cmp(tab[j], pivot) <= 0)
				break;
		}

	    if(i < j)
	    {
	    	int n = tab[i];
	    	tab[i] = tab[j];
	    	tab[j] = n;
	    }
	    else
	    	return i + (b == i);
	}

}

unsigned pivot_median3(const int* tab, unsigned l, unsigned r,
                       int (*cmp)(int a, int b))
{
	int min = l;
	int max = r - 1;
	unsigned m = (l + r) / 2;

	if(cmp(tab[min],tab[max]) >= 0)
	{
		int n = min;
		min = max;
		max = n;
	}
    
	if(cmp(tab[m],tab[min]) <= 0)
		return min;
	if(cmp(tab[m], tab[max]) <= 0)
		return m;
	return max;	
}


void quick_sort(int *tab, unsigned b, unsigned e,  unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                      int (*cmp)(int a, int b)),
                    int (*cmp)(int a, int b))
{
	if(e - b > 1)
	{
		unsigned m = partition_cmp(tab, b, e, pivot(tab, b, e, cmp), cmp);
		quick_sort(tab, b, m, pivot, cmp);
		quick_sort(tab, m, e, pivot, cmp);
	}
}
void quick_sort_cmp(int* tab, unsigned count,
                    unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                      int (*cmp)(int a, int b)),
                    int (*cmp)(int a, int b))
{
	quick_sort(tab, 0, count, pivot, cmp);
}

void quick_sort_loop(int *tab, unsigned b, unsigned e,  unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                      int (*cmp)(int a, int b)),
                    int (*cmp)(int a, int b))
{
  while(e - b > 1)
  {
    unsigned p = partition_cmp(tab, b, e, pivot(tab, b, e, cmp),cmp);
    quick_sort_loop(tab, b, p, pivot, cmp);
    b = p;
  }
}

void quick_sort_cmp_loop(int* tab, unsigned count,
                         unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                         int (*cmp)(int a, int b)),
                         int (*cmp)(int a, int b))
{
  quick_sort_loop(tab, 0, count, pivot, cmp);
}

unsigned small_threshold = 10;
void quick_sort_cmp_adapt_loop(int *tab, unsigned l, unsigned r,  unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                      int (*cmp)(int a, int b)),
                    int (*cmp)(int a, int b))
{
  while(r - l > 1)
  {
    unsigned p = partition_cmp(tab, l, r, pivot(tab, l, r, cmp),cmp);

    if(p - l <= small_threshold)
    {
        quick_sort_cmp_adapt_loop(tab, l, p, pivot, cmp);
        l = p;
    }
    else
    {
        quick_sort_cmp_adapt_loop(tab, p, r, pivot, cmp);
        r = p;
    }
  }
}

void quick_sort_cmp_adapt(int* tab, unsigned count,
                          unsigned (*pivot)(const int* tab, unsigned l, unsigned r,
                                            int (*cmp)(int a, int b)),
                          int (*cmp)(int a, int b))
{
      quick_sort_cmp_adapt_loop(tab, 0, count, pivot, cmp);
}

void intro_sort_cmp_rec(int *tab, unsigned b, unsigned e, double max, 
                        int (*cmp)(int a, int b))
{
    if(e - b > 1){

      if(max == 0)
        heap_sort_cmp(tab, e, cmp);
      else
      {
          unsigned m = partition_cmp(tab, b, e, pivot_median3(tab, b, e, cmp),cmp);
          intro_sort_cmp_rec(tab, b, m, max, cmp);
          intro_sort_cmp_rec(tab, m, e, max - 1, cmp);
      }
    }
}

void intro_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
    intro_sort_cmp_rec(tab, 0, count, 2 * log2(count), cmp);
}

unsigned int sum(int a, int b)
{
  return a + b;
}

unsigned int coin_combo(const unsigned int* values, unsigned count, unsigned int amount)
{
  if(amount == 0)
    return 1;
  if(count <= 0)
    return 0;
  if(amount < values[count - 1])
    return coin_combo(values, count - 1, amount);
  return coin_combo(values, count - 1, amount) + coin_combo(values, count, amount - values[count - 1]);
}

int main()
{
  unsigned coins[] = { 50, 1, 5, 10, 20, 2 };
printf("coin_combo(..., 10) = %u\n", coin_combo(coins, 6, 10));
  printf("coin_combo(..., 100) = %u\n", coin_combo(coins, 6, 100));
  return 0;
}
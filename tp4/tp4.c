/*** submitted by Makendy Piton (makendy.piton) on 2018-10-25 15:24:15.283191+00:00 from 185.125.224.4 ***/
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

int main()
{/*
	for (unsigned p = 0; p <= 10; p += 5)
	{
	  int tab[11] = { 2, 8, 42, 9, 13, 5, 3, 23, 40, -3, 55 };
	  printf("p = %u  =>  m = %u\n", p, partition_cmp(tab, 0, 11, p, increasing));
	  print_int_array(stdout, tab, 11);
	}*//*
	int tab[6] = {21, 18,  0, 37,  8, 30 };
	printf("p = %u\n", pivot_median3(tab, 0, 6, xx_cmp));*/
	/*int tab[10] = { 30, 8, 42, 9, 13, -3, 3, 23, 40, 5 };
printf("p = %u\n", pivot_median3(tab, 0, 10, increasing));*/
int tab[10] = { 30, 8, 42, 9, 13, 5, 3, 23, 40, -3 };
puts("increasing");
quick_sort_cmp(tab, 10, pivot_rand, increasing);
print_int_array(stdout, tab, 10);
puts("decreasing");
quick_sort_cmp(tab, 10, pivot_median3, decreasing);
print_int_array(stdout, tab, 10);
	return 0;
}
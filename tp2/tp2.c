/*** submitted by Makendy Piton (makendy.piton) on 2018-10-23 21:51:34.168831+00:00 from 82.241.21.236 ***/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

unsigned cmp_count;

int increasing(int a, int b)
{
    ++cmp_count;
    if (a < b)
        return -1;
    return a > b;
}

#define my_test(val) \
    cmp_count = 0; \
    printf("binary_search(a, %u, %d, increasing) = %u\n", \
           asize, (val), binary_search(a, asize, (val), increasing)); \
    printf("\twith %u comparisons\n", cmp_count);

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


void insert_sort(int* tab, unsigned count)
{
	int key;
	int j;
	for (unsigned i = 1; i < count; i++)
	{
		key = tab[i];
		j = i - 1;

		while(j >= 0 && tab[j] > key)
	    {
	    	tab[j + 1] = tab[j];
	    	j--;
	    }
	    tab[j + 1] = key;
	}
}

int decreasing(int a, int b)
{
   if (a > b)
     return -1;
   return a < b;
}


void insert_sort_cmp(int* tab, unsigned count, int (*cmp)(int a, int b))
{
    int key;
	int j;
	for (unsigned i = 1; i < count; i++)
	{
		key = tab[i];
		j = i - 1;

		while(j >= 0 && cmp(tab[j], key) == 1)
	    {
	    	tab[j + 1] = tab[j];
	    	j--;
	    }
	    tab[j + 1] = key;
	}
}

unsigned linear_search(const int* tab, unsigned count,
                       int val, int (*cmp)(int a, int b))
{
    for(unsigned i = 0; i < count; i++)
    {
        if(cmp(tab[i],val) >= 0)
            return i;
    }
    return count;
}

unsigned binary_search(int* tab, unsigned count,
                       int val, int (*cmp)(int a, int b))
{
    unsigned m;
    unsigned b = 0;

    while(b < count)
    {
        m = (b + count) / 2;
        int c = cmp(tab[m],val);
        if(c == 0)
            return m;
        if(c > 0)
            count = m;
        else
            b = m + 1;
    }
    return count;
}

void bs_insert_sort_cmp(int *tab, unsigned count, int (*cmp)(int a, int b))
{
	int key;
	unsigned j;
	for (unsigned i = 1; i < count; i++)
	{
		key = tab[i];
		j = binary_search(tab, i, key, cmp);
		for(unsigned k = i; k > j; k--)
			tab[k] = tab[k - 1];
		tab[j] = key;
	}
}

unsigned interpolate(int *tab, unsigned b, unsigned e, int val,
                     int (*cmp)(int, int))
{
	unsigned beg = b;
	unsigned end = e;
    int tmp;
    double slope;
    double tmpb = b;
    double tmpe = e;
    long int y_intercept;
    long int i;

    do{
    	if(e > end || e < beg || b > end || b < beg)
    		return b;
    	tmp = cmp(tab[e],tab[b]);
    	slope = (tmpe - tmpb) / (tab[e] - tab[b]);
    	
    	y_intercept = tmpe - slope * tab[e];
    	i = slope * val + y_intercept;//linear function : ax + b = y
    	if(i < beg)
			i = b;
	   
	    else if(i > end)
			i = e;

		if(tab[i] > val)
			tmpe = i - 1;
		if(tab[i] < val)
			tmpb = i + 1;
		if(tab[i] == val)
			return i;

		b = tmpb;
	    e = tmpe;
    }while(tmp > 0);

    if(tmp < 0)
    	return b;
	
    int t = cmp(tab[e],val);
	if(t >= 0)
		return b;
   /* 
	long int 
	//printf("slope : %d -- e = %d\n",slope, e);
	long int 
	printf("y_intercept : %ld\n",y_intercept);
	int 
	
	
	*/
	return e;
}

int main()
{
  int a[] = {1, 2, 4, 7, 10, 10, 10, 14, 17, 18, 20 };
  unsigned asize = sizeof(a) / sizeof(*a);
  puts("-- full range [0..10] --");
  for (unsigned val = 0; val <= 21; ++val)
    printf("interpolate(..., 0, %d, %d, ...) = %u\n", asize - 1, val,
           interpolate(a, 0, asize - 1, val, increasing));
  puts("-- shorter range [6..10] --");
  for (unsigned val = 0; val <= 21; ++val)
    printf("interpolate(..., 6, %d, %d, ...) = %u\n", asize - 1, val,
           interpolate(a, 6, asize - 1, val, increasing));
  return 0;
}
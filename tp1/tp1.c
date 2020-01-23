/*** submitted by Makendy Piton (makendy.piton) on 2018-10-22 20:20:06.757872+00:00 from 82.241.21.236 ***/
#include <stdio.h>
#include <stdlib.h>

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

int main()
{
/*
//----------------Exo 1---------------------------------------------------
  int a[] = { -42, -1, 0, 13, 9999, 10000, 10001 };
  for (unsigned i = 0; i < sizeof(a) / sizeof(*a); ++i)
  printf("int_width(%d) == %d\n", a[i], int_width(a[i]));
*/
//---------------------Exo 2---------------------------------------------
	/*
int a[] = {
  1, 2, 3, 4, 5, 6, 13, -35, 129, -4, 123, -4555, 1341, 2432, 111, 0, 1230
};
printf("%d\n", ints_width(a, sizeof(a)/sizeof(*a)));
a[3] = 99999999;
printf("%d\n", ints_width(a, sizeof(a)/sizeof(*a)));
*/

//-------------------------Exo 3-----------------------------------
	/*
	int a[] = {
  1, 2, 3, 4, 5, 6, 13, -35, 129, -4, 123, -4555, 1341, 2432, 111, 0, 1230
};
int b[] = {
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6,
  1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6
};
puts("-- a[]");
print_int_array(stdout, a, sizeof(a)/sizeof(*a));
puts("\n-- b[]");
print_int_array(stdout, b, sizeof(b)/sizeof(*b));
puts("\n-- b[] after b[8] = 123456");
b[8] = 123456;
print_int_array(stdout, b, sizeof(b)/sizeof(*b));
puts("\n-- a[0..9]");
print_int_array(stdout, a, 10);
puts("\n-- a[0..10]");
print_int_array(stdout, a, 11);
*/
//----------------------------Exo 4-------------------------------
	/*
	int a[] = {
  1, 2, 3, 4, 5, 6, 13, -35, 129, -4, 123, -4555, 1341, 2432, 111, 0, 1230
};

unsigned asize = sizeof(a)/sizeof(*a);
puts("before");
print_int_array(stdout, a, asize);
insert_sort(a, asize);
puts("after");
print_int_array(stdout, a, asize);
*/
//---------------------------------Exo 5-----------------------------------
int a[] = {
    1, 2, 3, 4, 5, 6, 13, -35, 129, -4, 123, -4555, 1341, 2432, 111, 0, 1230
};

  unsigned asize = sizeof(a)/sizeof(*a);
  puts("original");
  print_int_array(stdout, a, asize);
  puts("increasing");
  insert_sort_cmp(a, asize, increasing);
  print_int_array(stdout, a, asize);
  puts("decreasing");
  insert_sort_cmp(a, asize, decreasing);
  print_int_array(stdout, a, asize);
  return 0;
}
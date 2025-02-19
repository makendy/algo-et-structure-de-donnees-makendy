////////////////////////////////////////////////////////////////
//                                                            //
//  DO NOT EDIT THIS FILE -- WRITE YOUR FUNCTIONS IN  tp6.c   //
//                                                            //
////////////////////////////////////////////////////////////////


// Reads the contents of "tp6.c", ignoring any main() function.
#define main no__main
#include "tp6.c"
#undef main

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

struct xx_ranctx { uint32_t a; uint32_t b; uint32_t c; uint32_t d; } xx_ranc;

uint32_t xx_ranval()
{
#define rot(x, k) (((x) << (k)) | ((x) >> (32 - (k))))
  uint32_t e = xx_ranc.a - rot(xx_ranc.b, 27);
  xx_ranc.a = xx_ranc.b ^ rot(xx_ranc.c, 17);
  xx_ranc.b = xx_ranc.c + xx_ranc.d;
  xx_ranc.c = xx_ranc.d + e;
  xx_ranc.d = e + xx_ranc.a;
  return xx_ranc.d;
}

void xx_raninit(uint32_t seed)
{
  xx_ranc.a = 0xf1ea5eed, xx_ranc.b = xx_ranc.c = xx_ranc.d = seed;
  for (uint32_t i = 0; i < 20; ++i)
    (void) xx_ranval();
}

char* xx_ranstring(unsigned min_size, unsigned max_size)
{
  unsigned size = (unsigned) xx_ranval();
  size = size % max_size;
  if (size < min_size)
    size = min_size;
  char* r = (char*) malloc((size+1)*sizeof(char));
  r[size] = '\0';
  for (unsigned i = 0; i < size; ++i)
    {
      uint32_t rnd = xx_ranval();
      r[i] = 'a' + (rnd % 26);
    }
  return r;
}

char** xx_ranstringtab(unsigned size, unsigned str_min, unsigned str_max)
{
  char** r = (char**) malloc((size+1)*sizeof(char*));
  for (unsigned i = 0; i < size; ++i)
    r[i] = xx_ranstring(str_min, str_max);

  return r;
}

FILE* xx_trace;
const char* xx_trace_name;

void xx_trace_init(const char* name)
{
  xx_trace_name = getenv("OUTPUT");
  if (!xx_trace_name)
    {
      static char tmp[30];
      strncpy(tmp, name, 30);
      strncat(tmp, ".trace", 30);
      xx_trace_name = tmp;
    }
  xx_trace = fopen(xx_trace_name, "w+");
  if (!xx_trace)
    {
      fprintf(stderr, "cannot open '%s' for writing: %s\n",
	      xx_trace_name, strerror(errno));
      exit(2);
    }

  time_t rawtime = time(NULL);
  struct tm *timeinfo = gmtime(&rawtime);
  char buffer[20];
  strftime(buffer, 80, "%F %T%n", timeinfo);

  int u = getuid();
  int r = rawtime & 0xffffff;
  fprintf(xx_trace, "%s\n%d\n%d\n", name, u, r);
  fputs(buffer, xx_trace);
  u ^= r;
  const char* s = getenv("SEED");
  if (s)
    u = atoi(s);
  xx_raninit(u);
}

void xx_trace_done()
{
  fclose(xx_trace);
  printf("please submit '%s'\n", xx_trace_name);
}


unsigned int xx_cmp_count = 0;

int xx_cmp(int a, int b)
{
  ++xx_cmp_count;
  if (((a&1) == 0) && ((b&1) == 1))
    return -1;
  if (((a&1) == 1) && ((b&1) == 0))
    return 1;
  if (a < b)
    return -1;
  return a > b;
}

int main()
{
#if Q == 1
  xx_trace_init("tp6q1");
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  display_words(xx_trace, r_size, r);
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 2
  xx_trace_init("tp6q2");
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  for (int i = 0; i < r_size; ++i)
    fprintf(xx_trace, "%s %u\n", r[i],bad_hash(r[i]));
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 3
  xx_trace_init("tp6q3");
  int ht_size = 1 + (xx_ranval() % 100);
  struct ht* mht = ht_malloc(ht_size);
  fprintf(xx_trace, "%d\n", mht->size);
  for (int i = 0; i < ht_size; ++i)
    fprintf(xx_trace, "[%d] %d\n", i, mht->table[i] == NULL);
  xx_trace_done();
  ht_free(mht);
#elif Q == 4
  xx_trace_init("tp6q4");
  int ht_size = 1 + (xx_ranval() % 100);
  struct ht* mht = ht_malloc(ht_size);
  fprintf(xx_trace, "%d\n", mht->size);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  for (int i = 0; i < r_size; ++i)
    ht_find_or_put(r[i], 0, mht, bad_hash);
  for (int i = 0; i < ht_size; ++i)
    fprintf(xx_trace, "[%d] %d\n", i, mht->table[i] != NULL);
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
  ht_free(mht);
#elif Q == 5
  xx_trace_init("tp6q5");
  int ht_size = 1 + (xx_ranval() % 100);
  struct ht* mht = ht_malloc(ht_size);
  fprintf(xx_trace, "%d\n", mht->size);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  for (int i = 0; i < r_size; ++i)
    ht_find_or_put(r[i], 0, mht, bad_hash);
  ht_print(xx_trace, mht);
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
  ht_free(mht);
#elif Q == 6
  xx_trace_init("tp6q6");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, bad_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  ht_free(mht);
  for (int i = 1; i < r_size; ++i)
    {
      free(r[i]);
      r[i] = (char*) malloc(sizeof(char)*(strlen(r[0])+1));
      strcpy(r[i], r[0]);
    }
  mht = compute_histogram(r, r_size, ht_size, bad_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
  ht_free(mht);
#elif Q == 7
  xx_trace_init("tp6q7");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, xor_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  xx_trace_done();
  ht_free(mht);
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 8
  xx_trace_init("tp6q8");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, xor_add_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  xx_trace_done();
  ht_free(mht);
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 9
  xx_trace_init("tp6q9");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, djb_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  xx_trace_done();
  ht_free(mht);
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 10
  xx_trace_init("tp6q10");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, fvn_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  xx_trace_done();
  ht_free(mht);
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
#elif Q == 11
  xx_trace_init("tp6q11");
  int ht_size = 1 + (xx_ranval() % 100);
  int r_size = 1 + (xx_ranval() % 200);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  struct ht* mht = compute_histogram(r, r_size, ht_size, bad_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  fprintf(xx_trace, "nbcol : %d\n", nb_collisions(mht));
  ht_free(mht);
  for (int i = 1; i < r_size; ++i)
    {
      free(r[i]);
      r[i] = (char*) malloc(sizeof(char)*(strlen(r[0])+1));
      strcpy(r[i], r[0]);
    }
  mht = compute_histogram(r, r_size, ht_size, fvn_hash);
  fprintf(xx_trace, "%d\n", mht->size);
  ht_print(xx_trace, mht);
  fprintf(xx_trace, "nbcol : %d\n", nb_collisions(mht));
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
  ht_free(mht);
#elif Q == 12
  xx_trace_init("tp6q12");
  {
    int ht_size = 1;
    int r_size = 1 + (xx_ranval() % 200);
    int str_size = 5 + (xx_ranval() %20);
    char** r = xx_ranstringtab(r_size, 5, str_size);
    struct ht* mht = compute_histogram(r, r_size, ht_size, bad_hash);
    fprintf(xx_trace, "%d\n", mht->size);
    ht_print(xx_trace, mht);
    fprintf(xx_trace, "free : %d\n", find_empty_bucket(mht));
    ht_free(mht);
    for (int i = 0; i < r_size; ++i)
      free(r[i]);
    free(r);
  }
  for (int i = 0; i < 10; ++i)
  {
    int ht_size = 1 + (xx_ranval() % 100);
    int r_size = 1 + (xx_ranval() % 20);
    int str_size = 5 + (xx_ranval() %20);
    char** r = xx_ranstringtab(r_size, 5, str_size);
    struct ht* mht = compute_histogram(r, r_size, ht_size, bad_hash);
    fprintf(xx_trace, "%d\n", mht->size);
    ht_print(xx_trace, mht);
    fprintf(xx_trace, "free : %d\n", find_empty_bucket(mht));
    ht_free(mht);
    for (int i = 0; i < r_size; ++i)
      free(r[i]);
    free(r);
  }
  xx_trace_done();
#elif Q == 13
  xx_trace_init("tp6q13");
  int ht_size = 1;
  struct ht* mht = ht_malloc(ht_size);
  int r_size = 1 + (xx_ranval() % 10);
  int str_size = 5 + (xx_ranval() %20);
  char** r = xx_ranstringtab(r_size, 5, str_size);
  for (int i = 0; i < r_size; ++i)
    {
      fprintf(xx_trace, "%d\n", mht->size);
      ht_find_or_put2(r[i], 1, mht, bad_hash);
      ht_print(xx_trace, mht);
    }
  xx_trace_done();
  for (int i = 0; i < r_size; ++i)
    free(r[i]);
  free(r);
  ht_free2(mht);
#else
  puts("Compile with -DQ=n, where n is the question number.");
#endif
  return 0;
}

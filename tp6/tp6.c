#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct keyval {
  char* key;
  unsigned val;
  struct keyval* next;
};

struct ht {
  struct keyval** table;
  unsigned size;
};

int find_empty_bucket(struct ht *hashtable)
{
    for (size_t i = 0; i < hashtable->size; i++)
        if (!hashtable->table[i])
            return i;
    
    return -1;
}

unsigned nb_collisions(struct ht* hashtable)
{
    unsigned count = 0;
    for (size_t i = 0; i < hashtable->size; i++)
    {
        unsigned tmpc = 0;
        if (hashtable->table[i])
        {
            struct keyval *tmp = hashtable->table[i];
            while(tmp)
            {
                tmpc++;
                tmp = tmp->next;
            }
        }
        if (tmpc > count)
            count = tmpc;
    }
    return count;
}

unsigned fvn_hash(char* word)
{
    size_t i = 0;
    unsigned hashVal = 2166136261;
    while (word[i] != '\0')
    {
        hashVal *= 16777619;
        hashVal = hashVal ^ word[i];
        i++;
    }
    return hashVal;
}

unsigned djb_hash(char* word)
{
    size_t i = 0;
    unsigned hashVal = 0;
    while (word[i] != '\0')
    {
        hashVal *= 33;
        hashVal = hashVal + word[i];
        i++;
    }
    return hashVal;
}

unsigned xor_add_hash(char* word)
{
    size_t i = 0;
    unsigned hashVal = 0;
    while (word[i] != '\0')
    {
        hashVal = hashVal ^ word[i];
        hashVal = hashVal + word[i];
        i++;
    }
    return hashVal;
}

unsigned xor_hash(char* word)
{
    size_t i = 0;
    unsigned sum = 0;
    while (word[i] != '\0')
    {
        sum = sum ^ word[i];
        i++;
    }
    return sum;
}

static unsigned occurency_words(char *word, char **tab_words, unsigned begin,
                                unsigned end)
{
    unsigned count = 1;
    for (size_t i = begin; i < end; i++)
    {
        if(strcmp(word, tab_words[i]) == 0)
            count++;
    }
    return count;
}

void ht_print(FILE* out, struct ht* hashtable)
{
    for (size_t i = 0; i < hashtable->size; i++)
    {
        if (hashtable->table[i])
        {
            fprintf(out, "Bucket[%lu]:\t", i);
            struct keyval *tmp = hashtable->table[i];
            while (tmp != NULL)
            {
                fprintf(out, "(%s,%u)", tmp->key, tmp->val);
                tmp = tmp->next;
                if (tmp != NULL)
                    fprintf(out, " -> ");                
            }
            fprintf(out, "\n");
        }
    }
}

struct keyval *ht_find_or_put(char* word, unsigned value,
                              struct ht* hashtable,
                              unsigned (*hash)(char*))
{
    unsigned i = hash(word) % hashtable->size;
    if (hashtable->table[i] == NULL)
    {
        hashtable->table[i] = malloc(sizeof(struct keyval));
        hashtable->table[i]->val = value;
        hashtable->table[i]->key = malloc(sizeof(char) * (strlen(word) + 1));
        hashtable->table[i]->key = strcpy(hashtable->table[i]->key, word);
        hashtable->table[i]->next = NULL;
        return hashtable->table[i];
    }
    
    struct keyval *tmp = hashtable->table[i];
    struct keyval *sauv;
    while(tmp != NULL)
    {
        if(strcmp(tmp->key, word) == 0)
        {
            return tmp;
        }
        sauv = tmp;
        tmp = tmp->next;
    }

    sauv->next = malloc(sizeof(struct keyval));
    sauv->next->val = value;
    sauv->next->key = malloc(sizeof(char) * (strlen(word) + 1));
    sauv->next->key = strcpy(sauv->next->key, word);
    sauv->next->next = NULL;
    return sauv->next;
}

void ht_free(struct ht* table)
{
    if (!table)
        return;

    for (size_t i = 0; i < table->size; i++)
    {
        while (table->table[i] != NULL)
        {
            struct keyval *tmp = table->table[i];
            table->table[i] = table->table[i]->next;
            if(tmp->key)
                free(tmp->key);
            free(tmp);

        }
    }   
    free(table->table);
    free(table);
}

struct ht* ht_malloc(unsigned size)
{
    struct ht *hash = malloc(sizeof(struct ht));
    hash->size = size;
    hash->table = malloc(sizeof(struct keyval *) * size);

    for (unsigned i = 0; i < size; i++)
        hash->table[i] = NULL;
    
    return hash;
}

struct ht* ht_realloc(struct ht* hash)
{
    unsigned tmp = hash->size;
    hash->size *= 2;
    hash->table = realloc(hash->table, sizeof(struct keyval *) * hash->size);

    for (unsigned i = tmp; i < hash->size; i++)
        hash->table[i] = NULL;
    
    return hash;
}

unsigned bad_hash(char* word)
{
    size_t i = 0;
    unsigned sum = 0;
    while (word[i] != '\0')
    {
        sum += word[i];
        i++;
    }
    return sum;
}

struct ht* compute_histogram(char** words, unsigned words_size,
                             unsigned ht_size, unsigned (*hash)(char*))
{
    struct ht* hashtable = ht_malloc(ht_size);
    for (size_t i = 0; i < words_size; i++)
        ht_find_or_put(words[i], occurency_words(words[i], words, i + 1, words_size), 
                        hashtable, hash);
    
    return hashtable;
}

struct keyval* ht_find_or_put2(char* word, unsigned value,
                               struct ht* hashtable,
                               unsigned (*hash)(char*))
{
    unsigned i = hash(word) % hashtable->size;

    struct keyval *tmp = malloc(sizeof(struct keyval));
    tmp->val = value;
    tmp->key = malloc(sizeof(char) * (strlen(word) + 1));
    tmp->key = strcpy(tmp->key, word);
    tmp->next = NULL;
    
    if (hashtable->table[i] == NULL)
    {
        hashtable->table[i] = tmp;
        return hashtable->table[i];
    }

    if(strcmp(hashtable->table[i]->key, word) == 0)
    {
        free(tmp);
        return hashtable->table[i];
    }

    int pos = find_empty_bucket(hashtable);
    if (pos >= 0)
    {
        hashtable->table[pos] = tmp;
        hashtable->table[i]->next = hashtable->table[pos];
        return hashtable->table[pos];
    }
    free(tmp);
    unsigned ex_size = hashtable->size;
    hashtable = ht_realloc(hashtable);
    
    for (unsigned j = 0; j < ex_size; j++)
        ht_find_or_put2(hashtable->table[j]->key, hashtable->table[j]->val, 
                        hashtable, hash);

    ht_find_or_put2(word, value, hashtable, hash);
    return hashtable->table[pos];
}

void ht_free2(struct ht* table)
{
    if (!table)
        return;

    for (size_t i = 0; i < table->size; i++)
    {
        if (table->table[i])
            free(table->table[i]);
    }   
    free(table->table);
    free(table);
}

void display_words(FILE* out, unsigned size, char** words)
{
    for (size_t i = 0; i < size; i++)
        fprintf(out, "%s\n", words[i]);
}

int main(int argc, char** argv)
{
//-------------------Q1----------------------------------
//  display_words(stdout, argc-1, argv+1);
//----------------Q2-------------------------------
/*
  printf("%u\n", bad_hash("foo"));
  printf("%u\n", bad_hash("bar"));
  printf("%u\n", bad_hash("foobar"));*/
///------------------Q3-------------------------
/*
  struct ht *hash = ht_malloc(12);
  ht_free(hash);
  */
  //______________Q4 et 5__________________________

  struct ht* table = ht_malloc(10);
  ht_find_or_put2("foo", 1,  table, bad_hash);
  ht_find_or_put2("bar", 42, table, bad_hash);
  printf("------------ Dump table ------------\n");
  ht_print(stdout, table);

  struct keyval* ptr = ht_find_or_put2("foo", 12, table, bad_hash);
  printf("------------ Dump table ------------\n");
  ht_print(stdout, table);

  //ptr->val = 21;
  //printf("------------ Dump table ------------\n");
  //ht_print(stdout, table);

  ht_find_or_put2("oof", 1,  table, bad_hash);
  printf("------------ Dump table ------------\n");
  ht_print(stdout, table);


  ht_find_or_put2("ofo", 41,  table, bad_hash);
  printf("------------ Dump table ------------\n");
  ht_print(stdout, table);
  ht_free2(table);
  /*
    char* words[] = {"foo", "oof", "bar", "rab", "foorab", "barfoo",
                    "oorabf", "foo"};
    struct ht* table = compute_histogram(words, 8, 100, bad_hash);
    ht_print(stdout, table);

  ht_free(table);*//*
    printf("%s : %u\n", "foo", xor_add_hash("foo"));
    printf("%s : %u\n", "ofo", xor_add_hash("ofo"));
    printf("%s : %u\n", "bar", xor_add_hash("bar"));
    printf("%s : %u\n", "foobar", xor_add_hash("foobar"));
    puts("-------------------------");
    printf("%s : %u\n", "foo", djb_hash("foo"));
    printf("%s : %u\n", "ofo", djb_hash("ofo"));
    printf("%s : %u\n", "bar", djb_hash("bar"));
    printf("%s : %u\n", "foobar", djb_hash("foobar"));
    puts("-------------------------");
    printf("%s : %u\n", "foo", fvn_hash("foo"));
    printf("%s : %u\n", "ofo", fvn_hash("ofo"));
    printf("%s : %u\n", "bar", fvn_hash("bar"));
    printf("%s : %u\n", "foobar", fvn_hash("foobar"));*/
   /* char* words[] = {"foo", "oof", "bar", "rab", "foorab", "barfoo",
                    "oorabf", "foo"};
    struct ht* table;
    table = compute_histogram(words, 8, 100, bad_hash);
    ht_print(stdout, table);
    printf("nbcol : %d\n", nb_collisions(table));
    ht_free(table);
    table = compute_histogram(words, 8, 100, fvn_hash);
    ht_print(stdout, table);
    printf("nbcol : %d\n", nb_collisions(table));
    ht_free(table);*/
    /*struct ht* mht = ht_malloc(1);
    printf("%d\n", find_empty_bucket(mht));
    ht_find_or_put("hello", 0, mht, bad_hash);
    printf("%d\n", find_empty_bucket(mht));*/
    return 0;
}
#include "proxy_parse.h"

#define DEFAULT_NHDRS 8
#define MAX_REQ_LEN 65535
#define MIN_REQ_LEN 4

static const char *root_abs_path = "/";

/* private function declartions */
int ParsedRequest_printRequestLine(struct ParsedRequest *pr, 
				   char * buf, size_t buflen,
				   size_t *tmp);
size_t ParsedRequest_requestLineLen(struct ParsedRequest *pr);
/*
 * debug() prints out debugging info if DEBUG is set to 1
 *
 * parameter format: same as printf 
 *
 */
void debug(const char * format, ...) {
     va_list args;
     if (DEBUG) {
	  va_start(args, format);
	  vfprintf(stderr, format, args);
	  va_end(args);
     }
}



/*
 *  ParsedHeader Public Methods
 */

/* Set a header with key and value */
int ParsedHeader_set(struct ParsedRequest *pr, 
		     const char * key, const char * value)
{
     struct ParsedHeader *ph;
     ParsedHeader_remove (pr, key);

     if (pr->headerslen <= pr->headersused+1) {
	  pr->headerslen = pr->headerslen * 2;
	  pr->headers = 
	       (struct ParsedHeader *)realloc(pr->headers, 
		pr->headerslen * sizeof(struct ParsedHeader));
	  if (!pr->headers)
	       return -1;
     }

     ph = pr->headers + pr->headersused;
     pr->headersused += 1;
     
     ph->key = (char *)malloc(strlen(key)+1);
     memcpy(ph->key, key, strlen(key));
     ph->key[strlen(key)] = '\0';

     ph->value = (char *)malloc(strlen(value)+1);
     memcpy(ph->value, value, strlen(value));
     ph->value[strlen(value)] = '\0';

     ph->keylen = strlen(key)+1;
     ph->valuelen = strlen(value)+1;
     return 0;
}


/* get the parsedHeader with the specified key or NULL */
struct ParsedHeader* ParsedHeader_get(struct ParsedRequest *pr, 
				      const char * key)
{
     size_t i = 0;
     struct ParsedHeader * tmp;
     while(pr->headersused > i)
     {
	  tmp = pr->headers + i;
	  if(tmp->key && key && strcmp(tmp->key, key) == 0)
	  {
	       return tmp;
	  }
	  i++;
     }
     return NULL;
}

/* remove the specified key from parsedHeader */
int ParsedHeader_remove(struct ParsedRequest *pr, const char *key)
{
     struct ParsedHeader *tmp;
     tmp = ParsedHeader_get(pr, key);
     if(tmp == NULL)
	  return -1;

     free(tmp->key);
     free(tmp->value);
     tmp->key = NULL;
     return 0;
}

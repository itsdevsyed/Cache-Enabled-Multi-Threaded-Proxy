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

/* modify the header with given key, giving it a new value
 * return 1 on success and 0 if no such header found
 * 
int ParsedHeader_modify(struct ParsedRequest *pr, const char * key, 
			const char *newValue)
{
     struct ParsedHeader *tmp;
     tmp = ParsedHeader_get(pr, key);
     if(tmp != NULL)
     {
	  if(tmp->valuelen < strlen(newValue)+1)
	  {
	       tmp->valuelen = strlen(newValue)+1;
	       tmp->value = (char *) realloc(tmp->value, 
					     tmp->valuelen * sizeof(char));
	  } 
	  strcpy(tmp->value, newValue);
	  return 1;
     }
     return 0;
}
*/

/*
  ParsedHeader Private Methods
*/

void ParsedHeader_create(struct ParsedRequest *pr)
{
     pr->headers = 
     (struct ParsedHeader *)malloc(sizeof(struct ParsedHeader)*DEFAULT_NHDRS);
     pr->headerslen = DEFAULT_NHDRS;
     pr->headersused = 0;
} 


size_t ParsedHeader_lineLen(struct ParsedHeader * ph)
{
     if(ph->key != NULL)
     {
	  return strlen(ph->key)+strlen(ph->value)+4;
     }
     return 0; 
}

size_t ParsedHeader_headersLen(struct ParsedRequest *pr) 
{
     if (!pr || !pr->buf)
	  return 0;

     size_t i = 0;
     int len = 0;
     while(pr->headersused > i)
     {
	  len += ParsedHeader_lineLen(pr->headers + i);
	  i++;
     }
     len += 2;
     return len;
}

int ParsedHeader_printHeaders(struct ParsedRequest * pr, char * buf, 
			      size_t len)
{
     char * current = buf;
     struct ParsedHeader * ph;
     size_t i = 0;

     if(len < ParsedHeader_headersLen(pr))
     {
	  debug("buffer for printing headers too small\n");
	  return -1;
     }
  
     while(pr->headersused > i)
     {
	  ph = pr->headers+i;
	  if (ph->key) {
	       memcpy(current, ph->key, strlen(ph->key));
	       memcpy(current+strlen(ph->key), ": ", 2);
	       memcpy(current+strlen(ph->key) +2 , ph->value, 
		      strlen(ph->value));
	       memcpy(current+strlen(ph->key) +2+strlen(ph->value) , 
		      "\r\n", 2);
	       current += strlen(ph->key)+strlen(ph->value)+4;
	  }
	  i++;
     }
     memcpy(current, "\r\n",2);
     return 0;
}

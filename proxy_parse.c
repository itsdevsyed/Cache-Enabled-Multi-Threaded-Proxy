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
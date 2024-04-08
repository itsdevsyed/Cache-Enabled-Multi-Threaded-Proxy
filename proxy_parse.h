/*
 * proxy_parse.h -- A HTTP Request Parsing Library.
 *
 * This library provides functions for parsing HTTP requests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <ctype.h>

#ifndef PROXY_PARSE
#define PROXY_PARSE

#define DEBUG 1

/* ParsedRequest represents a parsed HTTP request. */
struct ParsedRequest {
     char *method;        // The HTTP method (e.g., GET, POST)
     char *protocol;      // The protocol (e.g., HTTP)
     char *host;          // The host (e.g., www.example.com)
     char *port;          // The port (e.g., 80)
     char *path;          // The path (e.g., /index.html)
     char *version;       // The HTTP version (e.g., 1.1)
     char *buf;           // Buffer to store the request
     size_t buflen;       // Length of the buffer
     struct ParsedHeader *headers; // Linked list of headers
     size_t headersused;  // Number of headers used
     size_t headerslen;   // Total length of headers
};

/* ParsedHeader represents a single HTTP header. */
struct ParsedHeader {
     char * key;    // The header key
     size_t keylen; // Length of the key
     char * value;  // The header value
     size_t valuelen; // Length of the value
};

/* Create an empty ParsedRequest object. */
struct ParsedRequest* ParsedRequest_create();

/* Parse an HTTP request in the buffer 'buf' of length 'buflen'. */
int ParsedRequest_parse(struct ParsedRequest * parse, const char *buf, int buflen);

/* Destroy a ParsedRequest object and free its memory. */
void ParsedRequest_destroy(struct ParsedRequest *pr);

/* Retrieve the entire buffer from a parsed request object. */
int ParsedRequest_unparse(struct ParsedRequest *pr, char *buf, size_t buflen);

/* Retrieve the headers from a parsed request object. */
int ParsedRequest_unparse_headers(struct ParsedRequest *pr, char *buf, size_t buflen);

/* Get the total length of a parsed request. */
size_t ParsedRequest_totalLen(struct ParsedRequest *pr);

/* Get the length of the headers of a parsed request. */
size_t ParsedHeader_headersLen(struct ParsedRequest *pr);

/* Set a header key-value pair in a parsed request. */
int ParsedHeader_set(struct ParsedRequest *pr, const char * key, const char * value);

/* Get a header value from a parsed request. */
struct ParsedHeader* ParsedHeader_get(struct ParsedRequest *pr, const char * key);

/* Remove a header from a parsed request. */
int ParsedHeader_remove (struct ParsedRequest *pr, const char * key);

/* Print debug information if DEBUG is set to 1. */
void debug(const char * format, ...);

/* Example usage:
   
   Create a ParsedRequest object
   struct ParsedRequest *req = ParsedRequest_create();

   Parse the request
   if (ParsedRequest_parse(req, request, strlen(request)) < 0) {
       printf("Failed to parse request\n");
       return -1;
   }

     Access the parsed fields
   printf("Method: %s\n", req->method);
   printf("Host: %s\n", req->host);
   printf("Path: %s\n", req->path);

   Clean up
   ParsedRequest_destroy(req);
*/

#endif
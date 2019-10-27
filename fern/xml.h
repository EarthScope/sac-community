
#ifndef _XML_H_
#define _XML_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "request.h"

typedef struct xml xml;

size_t xpath_len(xmlXPathObject* res);
int    xml_find_string(xml *x, xmlNode *from, const char *path, const char *key, char **s);
int    xml_find_string_copy(xml *x, xmlNode *from, const char *path, const char *key, char *s, size_t n);
int    xml_find_string_dup(xml *x, xmlNode *from, const char *path, const char *key, char **v);
int    xml_find_double(xml *x, xmlNode *from, const char *path, const char *key, double *value);
int    xml_find_attr_string(xml *x, xmlNode *from, const char *path, const char *name, char **s);
void   xml_free(xml *x);
xml  * xml_new(char *data, size_t data_len);
int    xml_merge(xml *x1, xml *x2, char *path);
xml *  xml_merge_results(result *r1, result *r2, char *path);

xmlNode * xpath_index(xmlXPathObject* v, size_t i);
xmlNode * xml_find(xml *x, xmlNode *from, const xmlChar *path);
xmlNode * xml_get_text_node(xmlNode * parent);

xmlDoc          * xml_init_doc(char *data, size_t ndata);
xmlXPathContext * create_new_context(xmlDoc *doc);
xmlXPathObject  * xml_find_all(xml *x, xmlNode *from, const xmlChar* path);

#define XPATH_FREE(x) do { if(x) { xmlXPathFreeObject(x);  x = NULL;  } } while (0)
#define XCTX_FREE(x)  do { if(x) { xmlXPathFreeContext(x);  x = NULL; } } while (0)
#define XDOC_FREE(x)  do { if(x) { xmlFreeDoc(x);  x = NULL; } } while (0)


int is_xml(char *data);
int is_xml_file(char *file);

#endif /* _XML_H_ */

/**
 * @file
 * @brief xml parsing functions
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "xml.h"
#include "defs.h"
#include "strip.h"

/**
 * @defgroup xml xml
 *
 * @brief XML parsing
 *
 */

/**
 * @brief XML Document for search
 * @ingroup xml
 */
struct xml {
    xmlDoc *doc;    /**< @private Internal xml doc value */
    xmlXPathContext *ctx; /**< @private Internal xpath context value */
};

/**
 * @brief Create a new xml doc and create search context
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param data      input xml data
 * @param data_len  length of data
 *
 * @return xml doc, NULL on error
 *
 * @note The namespaces are defined as follows
 *    - s = http://www.fdsn.org/xml/station/1
 *    - q - http://quakeml.org/xmlns/bed/1.2
 *          or whatever was found as the first defined namespace value
 *
 * @warning User owns the xml and is responsible for freeing the data with
 *    \ref xml_free
 *
 */
xml *
xml_new(char *data, size_t data_len) {
    xml *x = calloc(1, sizeof(xml));
    if(!(x->doc = xml_init_doc(data, data_len))) {
        printf("Error initializing xml parser\n");
        //*nerr = 3264;
        return NULL;
    }
    if(!(x->ctx = create_new_context(x->doc))) {
        printf("Error initializing new context\n");
        XDOC_FREE(x->doc);
        return NULL;
    }
    return x;
}

/**
 * @brief Free and xml document
 *
 * @memberof xml
 * @ingroup xml
 *
 */
void
xml_free(xml *x) {
    if(x) {
        XDOC_FREE(x->doc);
        XCTX_FREE(x->ctx);
        FREE(x);
    }
}


/**
 * @brief Find a string in xml
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the document root
 * @param path  xpath search
 * @param key   xml node attribute
 * @param s     output character string value
 *
 * @return 1 on success, 0 on failure
 *
 * @warning User owns the output value only if a key was supplied, otherwise the
 *   output is owned by the xml document
 */
int
xml_find_string(xml *x, xmlNode *from, const char *path, const char *key, char **s) {
    xmlNode *v = NULL, *txt = NULL;
    *s = NULL;
    if(key) {
        if(xml_find_attr_string(x, from, path, key, s)) {
            return 1;
        }
    } else {
        if((v = xml_find(x, from, (xmlChar *)path)) &&
           (txt = xml_get_text_node(v)) &&
           (txt->content)) {
            *s = (char *) txt->content;
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Find a string in xml and clone it
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the document root
 * @param path  xpath search
 * @param key   xml node attribute
 * @param v     output character string value
 *
 * @return 1 on success, 0 on failure
 *
 * @note If the output string is defined with a length, the function
 *    returns immediately
 *
 * @warning User owns the output value and is responsible for freeing
 *    the underlying memory
 */
int
xml_find_string_dup(xml *x, xmlNode *from, const char *path, const char *key, char **v) {
    char *s = NULL;
    if(*v && strlen(*v) > 0) {
        return 1;
    }
    if(!(xml_find_string(x, from, path, key, &s))) {
        if(!*v) {
            *v = strdup("-");
        }
        return 0;
    }
    *v = strdup(s);
    fern_rstrip(*v);
    if(key) {
        FREE(s);
        s = NULL;
    }
    return 1;
}

/**
 * @brief Find a string in xml and copy it
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the document root
 * @param path  xpath search
 * @param key   xml node attribute
 * @param s     output character string value
 * @param n     length of s
 *
 * @return 1 on success, 0 on failure
 *
 */
int
xml_find_string_copy(xml *x, xmlNode *from, const char *path, const char *key, char *s, size_t n) {
    char *p = NULL;
    if(!(xml_find_string(x, from, path, key, &p))) {
        return 0;
    }
    fern_strlcpy(s, p, n);
    fern_rstrip(s);
    if(key) {
        // If key is defined,
        //   then a parameter is desired
        //   and a new memory is returned.
        FREE(p);
    }
    return 1;
}

/**
 * @brief Find a double in xml
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the document root
 * @param path  xpath search
 * @param key   xml node attribute
 * @param value output double floating point value
 *
 * @return 1 on success, 0 on failure
 *
 */
int
xml_find_double(xml *x, xmlNode *from, const char *path, const char *key, double *value) {
    char *end = NULL, *s = NULL;
    if(xml_find_string(x, from, path, key, &s)) {
        *value = strtod(s, &end);
        if(*end == 0 && errno != ERANGE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Find a single xml object in a document
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the documentroot
 * @param path  xpath search 
 *
 * @return xmlNode if only a single value was found, NULL otherwise
 *
 */
xmlNode *
xml_find(xml *x, xmlNode *from, const xmlChar *path) {
    xmlNode * e = NULL;
    xmlXPathObject * result = xml_find_all(x, from, path);
    size_t n = xpath_len(result);
    if(n == 0) {
        goto error;
    }
    if(n > 1) {
        printf("Multiple [%zu] nodes found\n", n);
        printf("   path: %s\n", path);
        goto error;
    }
    e = result->nodesetval->nodeTab[0];
 error:
    if(result) {
        xmlXPathFreeObject(result);
        result = NULL;
    }
    return e;
}

/**
 * @brief Find all occurances of a path in a xml document
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x    xml document
 * @param from initial search position, NULL for document root
 * @param path xpath for search
 *
 * @return result of xpath search or NULL on error
 *
 * @warning user is responsible for freeing the output xmlXPathObject
 *    with http://xmlsoft.org/html/libxml-xpath.html#xmlXPathFreeObject
 */
xmlXPathObject *
xml_find_all(xml *x, xmlNode *from, const xmlChar* path) {

    if(from == NULL) {
        from = x->doc->children;
    }
    xmlXPathObject *result = xmlXPathNodeEval(from, path, x->ctx);
    //xmlXPathObjectPtr result = xmlXPathEval(path, context);
    if (result == NULL) {
        printf("Error in xmlXPathNodeEval\n");
        printf("%s\n", path);
        return NULL;
    }
    if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
        xmlXPathFreeObject(result);
        result = NULL;
        goto error;
    }
 error:
    return result;
}


/**
 * @brief Merge two documents at a path
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param x1    xml document to merge together
 * @param x2    xml document to merge together
 * @param path  xml element path to merge together
 *
 * @return 0 on failure, 1 on success
 *
 */
int
xml_merge(xml *x1, xml *x2, char *path) {
    size_t i = 0;
    size_t n2 = 0;
    xmlNode *node = NULL;
    xmlXPathObject *node1 = NULL, *node2 = NULL;
    node1 = xml_find_all(x1, NULL, (xmlChar *) path);
    node2 = xml_find_all(x2, NULL, (xmlChar *) path);

    if(xpath_len(node1) == 0 || xpath_len(node2) == 0) {
        return 0;
    }
    n2 = xpath_len(node2);
    for(i = 0; i < n2; i++) {
        node = xmlCopyNode(xpath_index(node2, i), 1);
        xmlAddSibling(xpath_index(node1, 0), node);
    }
    XPATH_FREE(node1);
    XPATH_FREE(node2);
    return 1;
}

/**
 * @brief Length of objects from a xml search
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param res  xpath search result
 *
 * @return length of res
 *
 */
size_t
xpath_len(xmlXPathObject* res) {
    if(!res || !res->nodesetval) {
        return 0;
    }
    return (size_t) res->nodesetval->nodeNr;
}

/**
 * @brief Get a value from a xml search
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param v  xpath search result
 * @param i  index of value to return
 *
 * @return requested xml object or NULL if v does not exist,
 *    or the index is out of bounds
 */
xmlNode *
xpath_index(xmlXPathObject* v, size_t i) {
    if(!v || !v->nodesetval) {
        return NULL;
    }
    if(i >= (size_t) v->nodesetval->nodeNr) {
        return NULL;
    }
    return v->nodesetval->nodeTab[i];
}



/**
 * @brief Find a string in an attribute
 *
 * @memberof xml
 * @ingroup xml
 * @private
 *
 * @param x     xml document
 * @param from  starting object to begin search from, NULL indicates the document root
 * @param path  xpath search
 * @param name  xml node attribute
 * @param s     output character string
 *
 * @return 1 on success, 0 on failure
 *
 * @warning User owns the output character string and is responsible for freeing the
 *    underlying memory
 *
 */
int
xml_find_attr_string(xml *x, xmlNode *from, const char *path, const char *name, char **s) {
    xmlNode *v = NULL;
    if((v = xml_find(x, from, (xmlChar *) path)) &&
       (*s = (char *) xmlGetProp(v, (xmlChar *) name))) {
        return 1;
    }
    return 0;
}

/**
 * @brief Create a contenxt for searching an xml document
 *
 * @memberof xml
 * @ingroup xml
 * @private
 *
 * @param doc     xmldoc
 *
 * @return xml xpath context
 *
 * @warning User owns the context and is responsible for freeing the
 *    underlying memory
 *
 * @note The namespaces are defined as follows
 *    - s = http://www.fdsn.org/xml/station/1
 *    - q - http://quakeml.org/xmlns/bed/1.2
 *          or whatever was found as the first defined namespace value
 *
 */
xmlXPathContext *
create_new_context(xmlDoc *doc) {
    xmlXPathContext *context = xmlXPathNewContext(doc);
    if (context == NULL) {
        printf("Error in xmlXPathNewContext\n");
        return NULL;
    }
    // Find the NameSpace of the Elements actually used
    const xmlChar *xmlns = NULL;
    xmlXPathObject *p = xmlXPathNodeEval(doc->children, (xmlChar *)"//*[1]", context);
    if(p) {
        int n = p->nodesetval->nodeNr;
        for(int i = n-1; i < n; i++) {
            if(p->nodesetval->nodeTab[i]->type == XML_ELEMENT_NODE) {
                if(p->nodesetval->nodeTab[i]->ns) {
                    xmlns = p->nodesetval->nodeTab[i]->ns->href;
                }
            }
        }
        xmlXPathFreeObject(p);
    }
    xmlXPathRegisterNs(context,
                       BAD_CAST "s",
                       BAD_CAST "http://www.fdsn.org/xml/station/1");
    /*    xmlXPathRegisterNs(context,
                       BAD_CAST "q",
                       BAD_CAST "http://quakeml.org/xmlns/bed/1.2");*/
    xmlXPathRegisterNs(context,
                       BAD_CAST "q",
                       BAD_CAST xmlns);
    return context;
}



/**
 * @brief Initialize an xml doc from data
 *
 * @memberof xml
 * @ingroup xml
 * @private
 *
 * @param data   input xml data
 * @param n      length of data
 *
 * @return xml doc or NULL on error
 */
xmlDoc*
xml_init_doc(char *data, size_t ndata) {
    xmlDoc *doc = xmlReadMemory(data, (int) ndata, "noname.xml", NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Failed to parse document\n");
        return NULL;
    }
    return doc;
}



/**
 * @brief Return the text node of an xml node
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param parent     xml node to get text of
 *
 * @return xml text Node
 *
 */
xmlNode *
xml_get_text_node(xmlNode * parent) {
    if(parent->type == XML_TEXT_NODE) {
        return parent;
    }
    xmlNode * child = parent->children;
    while(child && child->type != XML_TEXT_NODE) {
        child = child->next;
    }
    return child;
}

/**
 * @brief Check if data represets xml data
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param data   data to check
 *
 * @return 1 if data starts with "<?xml", 0 otherwise
 *
 */
int
is_xml(char *data) {
    return (strncmp(data, "<?xml", 5) == 0);
}
/**
 * @brief Check if a file represets xml file
 *
 * @memberof xml
 * @ingroup xml
 *
 * @param file filename of possible xml file
 *
 * @return 1 if file starts with "<?xml", 0 otherwise
 *
 */
int
is_xml_file(char *file) {
    char c[10];
    FILE *fp = fopen(file, "r");
    if(!fp) {
        return 0;
    }
    memset(c, 0, sizeof(c));
    if(fread(&c, sizeof(char), 5, fp) != 5) {
        return 0;
    }
    fclose(fp);
    return is_xml(c);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void
rstrip(char *s) {
    char *end = NULL;
    if(!s) {
        return;
    }
    end = s + strlen(s) - 1;
    while(end > s && isspace(*end)) {
        *end = 0;
        end--;
    }
    return;
}
void
lstrip(char *s, char c) {
    char *p = NULL;
    if(!s) {
        return;
    }
    p = s;
    while(p && *p && *p == c) {
        p++;
    }
    memmove(s, p, strlen(p) + 1);
    return;
}

int
main(int argc, char *argv[]) {
    char *infile = NULL;
    char *outfile = NULL;
    char buf[1024] = {0};
    char *progname = "extract";
    int code = 0;
    int n = 0;
    int i = 0;
    int k = 0;
    FILE *in = NULL;
    FILE *out = NULL;

    if(argc < 3) {
        printf("Usage: %s outfile.c infiles.c\n", progname);
        exit(1);
    }
    outfile = argv[1];
    infile  = argv[2];

    if(strcmp(outfile, "-") == 0) {
        out = stdout;
    } else if((out = fopen(outfile, "w")) == NULL) {
        printf("%s: error opening output file: %s\n", progname, outfile);
        exit(1);
    }
    fprintf(out, "\n\
#include <stdio.h>\n\
#include <stdlib.h>\n\
#include <string.h>\n\
#include <math.h>\n\
\n\
#include <assert.h>\n\
#define assert_eq(a,b) assert((a) == (b))\n\
#define assert_ne(a,b) assert((a) != (b))\n\
\n\
#include <sacio.h>\n\
\n\
");
    n = 0;
    for(k = 2; k < argc; k++) {
        infile = argv[k];
        if((in = fopen(infile, "r")) == NULL) {
            printf("%s: error opening input file: %s\n", progname, infile);
            continue;
        }
        code = 0;
        while(fgets(buf, sizeof buf, in) != NULL) {
            if(strstr(buf, "@code")) {
                code = 1;
                fprintf(out, "void test_%05d() {\n", n);
            } else if(strstr(buf, "@endcode")) {
                fprintf(out, "}\n");
                code = 0;
                n += 1;
            } else if(code) {
                rstrip(buf);
                lstrip(buf,' ');
                lstrip(buf,'*');
                lstrip(buf,' ');
                fprintf(out, "     %s\n", buf);
            } else {
                continue;
            }
        }
        fclose(in);
    }

    fprintf(out, "\nint main() {\n");
    for(i = 0; i < n; i++) {
        fprintf(out, "     test_%05d();\n", i);
    }
    fprintf(out, "}\n");

    fclose(out);
    
    return 0;
}

#!/bin/sh

# Usage: lib2bin.sh libname.a > name_all.c

LIB=$1
#SRC=$2

FUNCS=$(nm $LIB  | grep ' T ' | awk '{print $3}' |  sed 's/^_//' )

for f in $FUNCS; do
    echo "void $f();"
done

cat << EOF
void not_used(int v);
int main() {
   not_used(0);
   return 0;
}
void
not_used(int v) {
   if (v == 0) {
       return;
   }
EOF
for f in $FUNCS; do
    echo "     $f();"
done

#    awk '{printf("void %s(); %s();\n", $1, $1)}' 

cat << EOF
}
EOF

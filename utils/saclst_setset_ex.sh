#!/bin/sh
set -x
# Create basic sac file
sac --copyright-off <<EOF
echo on
fg seismo
lh kzdate kztime iztype o b a
write file.sac
quit
EOF

saclst Full f file.sac | grep iztype
saclst kzdate kztime o b a f file.sac
# Absolute reference time can be set by setting "O"
# this also sets iztype = IO
# and  shifts all other time values relative to it
#   ch allt (-1.0 * &1,o)
sacset -v -O=1981/03/29T10:37:32.570 file.sac
saclst Full f file.sac | grep iztype
saclst kzdate kztime o b a f file.sac

sac --copyright-off <<EOF
echo on
read file.sac
lh kzdate kztime iztype o b a
quit
EOF

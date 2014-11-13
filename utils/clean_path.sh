#!/bin/sh


# function pathrm
#
# Category:	Functions
#
# Author:	William Seppeler
# Email:	seppeler@yahoo.com
#
# This ksh function removed a specified directory from a path variable.  If
# no path variable is specified, the environment PATH variable is used by
# default.
#
#	example: pathrm /usr/local/lib LD_LIBRARY_PATH
#
function pathrm {
    #
    # This function makes explicit checks for command line
    # syntax.  Any deviations from that syntax will print
    # the usage info.
    #
    function usage {
	echo "usage: pathrm [-?] directory [path_variable]"
    }
    typeset opt
    if getopts ":" opt; then
	usage
	return 1
    fi
    if (($#<1)) || ((2<$#)) ; then
	usage
	return 1
    fi

    #
    # Keep all vars local to this script.  Also, IFS must
    # be clean first before doing any path var assignments.
    #
    typeset remove_dir=$1
    typeset path_var=${2:-PATH}
    #typeset IFS=
    typeset old_path="$(eval echo \$${path_var})"
    typeset new_path=
    typeset dir
    #typeset IFS=":"

    #
    # Build a new path from current path by using each directory
    # that doesn't match the directory to be removed.  Note: this
    # will remove multiple occurances of the sepecified directory.
    # (a desired result)
    #
    for dir in $(echo $old_path | sed 's/:/ /g'); do
	if [[ $dir != $remove_dir ]] ; then
	    new_path="${new_path:+"$new_path:"}$dir"
        fi
    done

    #
    # Because this is a function, this eval statement will update
    # the path variable in the current working shell environment.
    # That's why this is a function and not a script.
    #
    eval $path_var="$new_path"
}

function pathrmre {
    function usage {
	echo "usage: pathrm [-?] directory [path_variable]"
    }
    typeset opt
    if getopts ":" opt; then
	usage
	return 1
    fi
    if (($#<1)) || ((2<$#)) ; then
	usage
	return 1
    fi
    typeset pattern=$1
    typeset path_var=${2:-PATH}
    typeset old_path="$(eval echo \$${path_var})"
    for part in $( echo $old_path | sed 's/:/ /g' ) ; do
        case $part in
            *$pattern* )
                echo "${path_var}: Remove $part"
                pathrm $part $path_var
                ;;
        esac
    done
}

MACPORTS_PREFIX=/opt/local
for var in LD_LIBRARY_PATH DYLD_LIBRARY_PATH PATH; do
    echo $var
    pathrmre $MACPORTS_PREFIX $var
done


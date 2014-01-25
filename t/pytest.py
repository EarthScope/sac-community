#!/usr/bin/env python

import os
import re
import sys
import math
import glob
import string
import subprocess
import optparse

import difflib

progname = sys.argv[0]

def main() :
    usage="usage %prog [-rgtvcpsoe] file1 file2 ..."
    parser = optparse.OptionParser(usage = usage)
    parser.add_option("-r", "--run", 
                      action  = "store_true", 
                      default = False,
                      help    = "run commands and show output [ False ]")
    parser.add_option("-g", "--generate", 
                      action  = "store_true", 
                      default = False,
                      help    = "generate command output and save [ False ]")
    parser.add_option("-t", "--test", 
                      action  = "store_true", 
                      default = False,
                      help    = "test command output [ False ]")
    parser.add_option("-c", "--copyright", 
                      action  = "store_true", 
                      default = False,
                      help    = "show copyright notice [ False ]")
    parser.add_option("-p", "--prompt", 
                      action  = "store_true", 
                      default = False,
                      help    = "show command prompt [ False ]")
    parser.add_option("-s", "--sac", 
                      action  = "store", 
                      default = "sac",
                      help    = "sac program location [ sac ]")
    parser.add_option("-o", "--output", 
                      action  = "store", 
                      default = "output",
                      help    = "program output directory [ output ]")
    parser.add_option("-v", "--verbose", 
                      action  = "store_true", 
                      default = False,
                      help    = "turn verbose reporting on [ False ]")
    parser.add_option('-V', "--valgrind",
                      action = "store_true",
                      default = False,
                      help   = "turn valgrind memory checker on [ False ]")
    parser.add_option('-e', "--die-on-error",
                      action = "store_true",
                      default = False,
                      help   = "die on an error [ False ]")
    (opts, args) = parser.parse_args()

    if len(args) < 1 :
        parser.print_help()
        sys.exit(2)

    error = 0
    for file in args :
        if file.endswith(('.m', 'CVS', '.am', '.SAC', '.sac', '.GSE')):
            continue
        if os.path.basename(file).startswith('200'):
            continue
        if os.path.basename(file).startswith('RESP') :
            continue
        xfile = os.path.basename(file)
        print >>sys.stderr, 'Test:', xfile
        [Out, Err] = execute( file, opts )

        if opts.run :
            print_output(file, Out, Err, opts)
        elif opts.generate :
            save_output(file, Out, Err, opts)
        elif opts.test :
            error = error + test_output(file, Out, Err, opts)
        if opts.die_on_error and error > 0 :
            break
    print >>sys.stderr, ""
    print >>sys.stderr, "Errors: ",error
    sys.exit(error)

def read_lines(file) :
    lines = list()
    try:
        f = open(file, 'r')
    except IOError:
        sys.exit('Could not open file: ' + file)
    lines.extend( f.readlines() )
    f.close()
    return lines

def save_lines(file, lines) :
    f = open(file, 'w')
    for l in lines :
        f.write(l)
    f.close()

def commands_read(file) :
    commands = [ 'inicm', 'echo on', 'wait text off' ]
    commands.extend( read_lines(file) )
    commands.append( 'quit' )
    L = list()
    for c in commands :
        L.append( c.rstrip() )
    return L

def commands_execute( commands , opts, test_path) :
    args = [ opts.sac, '--stdout' ]

    env = os.environ.copy()

    if opts.valgrind:
        args.insert(0,'valgrind')
        args.insert(1,'--quiet')
        args.insert(1,'--suppressions='+ os.path.dirname(os.path.dirname(test_path)) + '/valgrind.x11.suppress')
        if sys.platform == 'darwin' :
            args.insert(1, '--dsymutil=yes')

    if opts.copyright :
        args.append('--copyright-on')
    else :
        args.append('--copyright-off')
    # if opts.prompt :
    #     args.append('--prompt-on')
    # else :
    #     args.append('--prompt-off')

    args.append('--history-off')
    try :
        if opts.verbose :
            print >>sys.stderr, "\tOpening process: ", opts.sac
        p = subprocess.Popen( args, 
                              bufsize = 0, 
                              shell = False, 
                              env = env,
                              stdin  = subprocess.PIPE, 
                              stdout = subprocess.PIPE, 
                              stderr = subprocess.PIPE)
        if opts.verbose :
            print '\tPID: ',p.pid
    except (OSError, ValueError), (errno, strerror) : 
        print >>sys.stderr, progname, "error({0}): {1} :".format(errno, strerror),opts.sac
        sys.exit(errno)

    for com in commands :
        print >>p.stdin, com
    out, err = p.communicate()
    if out == None: out = ''
    if err == None: err = ''
    out = [ o + '\n' for o in out.rstrip().split('\n') ]
    err = [ o + '\n' for o in err.rstrip().split('\n') ]
    p.wait()

    for d in glob.glob("test.*.dir") :
        for f in os.listdir(d) :
            os.remove(os.path.join(d,f))
        os.rmdir(d)
    for f in glob.glob("test.*") :
        if opts.verbose:
            print >>sys.stderr, "\tRemoving test file: ",f
        os.remove(f)
    for f in glob.glob("2002.054.*") :
        if opts.verbose:
            print >>sys.stderr, "\tRemoving test file: ",f
        if f.endswith('.lhor2.e') or f.endswith('.lhor2.n') or f.endswith('.lhor2.z') :
            continue
        os.remove(f)

    return [ out, err ]
    
def execute(file, opts) :
    commands = commands_read(file)
    return commands_execute(commands, opts, os.path.dirname(file))

def print_output(file, Out, Err, opts) :
    for line in Out:
        print line,
    for line in Err:
        print line,

def output_path(file, opts) :
    return os.path.join(opts.output, 
                        os.path.basename(file))
def out_file(file, opts) :
    return output_path(file,opts) + '.out'

def err_file(file, opts) :
    return output_path(file, opts) + '.err'

def save_output(file, Out, Err, opts) :
    save_lines(out_file(file, opts), Out)
    save_lines(err_file(file, opts), Err)

def test_output(file, Out, Err, opts) :
    num = re.compile('([-+]?[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?)')
    eps = 1e-5
    error = 0
    passed = '\033[1;32mPass\033[1;m'
    failed = '\033[1;31mFail\033[1;m'
    if opts.valgrind:
        for i in Err: print i,
        return 0
    for t in ['out', 'err']  :

        if t == 'out' :
            out = Out
            exp_out = read_lines(out_file(file, opts))
        if t == 'err' :
            out = Err
            exp_out = read_lines(err_file(file, opts))

        if len(out) != len(exp_out) :
            print >> sys.stderr, "Number of lines do not match"
            print >> sys.stderr, "Output:      ", len(out)
            print >> sys.stderr, "Output(exp): ", len(exp_out)
            print >> sys.stderr

        for line in difflib.unified_diff(exp_out, out, 
                                             fromfile = 'expected', 
                                             tofile   = 'current') :
            print >>sys.stderr, line,
            error = error + 1
    return error


if __name__ == '__main__':
    main()



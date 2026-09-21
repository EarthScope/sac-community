#!/usr/bin/env python3

from __future__ import print_function

import os
import io
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
        print('Test: %s' % xfile, file=sys.stderr)
        [Out, Err] = execute( file, opts )

        if opts.run :
            print_output(file, Out, Err, opts)
        elif opts.generate :
            save_output(file, Out, Err, opts)
        elif opts.test :
            error = error + test_output(file, Out, Err, opts)
        if opts.die_on_error and error > 0 :
            break
    if error > 0 :
        print("", file=sys.stderr)
        print("Errors: %s" % error, file=sys.stderr)
    if error > 0:
        sys.exit(1)
    sys.exit(0)

def read_lines(file) :
    lines = list()
    try:
        f = io.open(file, mode='r', encoding='utf-8', errors='replace')
    except IOError:
        sys.exit('Could not open file: ' + file)
    lines.extend( f.readlines() )
    f.close()
    return lines

def save_lines(file, lines) :
    f = io.open(file, 'w')
    for l in lines :
        f.write(l)
    f.close()

def commands_read(file) :
    commands = [ 'inicm', 'echo on', 'wait text off' ]
    if not os.path.exists(file) :
        os.system('pwd')
        sys.exit('File does not exist: ' + file)
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
            print("\tOpening process: %s" % opts.sac, file=sys.stderr)
        p = subprocess.Popen( args, 
                              bufsize = 0, 
                              shell = False, 
                              env = env,
                              stdin  = subprocess.PIPE, 
                              stdout = subprocess.PIPE, 
                              stderr = subprocess.PIPE)
        if opts.verbose :
            print('\tPID: %d' % p.pid)
    except (OSError, ValueError) as err :
        errno, stderror = err
        print("{0} error({1}): {2} {3}".format(progname, errno, strerror, opts.sac), file=sys.stderr)
        sys.exit(errno)

    #for com in commands :
    #    print(com, file=p.stdin)
    cmds = '\n'.join(commands) + '\n'
    out, err = p.communicate(input=cmds.encode('utf-8'))
    if out == None: out = ''
    if err == None: err = ''
    if sys.version_info >= (2,7):
        out = out.decode(encoding='utf-8', errors='replace')
        err = err.decode(encoding='utf-8', errors='replace')
    else :
        out = out.decode("utf-8", "replace")
        err = err.decode("utf-8", "replace")
    out = [ o + '\n' for o in out.rstrip().split('\n') ]
    err = [ o + '\n' for o in err.rstrip().split('\n') ]
    ecode = p.wait()
    if ecode != 0:
        print("", file=sys.stderr)
        print(''.join(out), file=sys.stderr)
        print("", file=sys.stderr)
        print(''.join(err), file=sys.stderr)
        print("", file=sys.stderr)
        print('***** Program exiting badly, return value: %d *****' % ecode, file=sys.stderr)
        print("", file=sys.stderr)
        sys.exit(99)
    for d in glob.glob("test.*.dir") :
        for f in os.listdir(d) :
            os.remove(os.path.join(d,f))
        os.rmdir(d)
    for f in glob.glob("test.*") :
        if opts.verbose:
            print("\tRemoving test file: %s" % f, file=sys.stderr)
        os.remove(f)
    for f in glob.glob("2002.054.*") :
        if opts.verbose:
            print("\tRemoving test file: %s" % f, file=sys.stderr)
        if f.endswith('.lhor2.e') or f.endswith('.lhor2.n') or f.endswith('.lhor2.z') :
            continue
        os.remove(f)

    return [ out, err ]
    
def execute(file, opts) :
    commands = commands_read(file)
    return commands_execute(commands, opts, os.path.dirname(file))

def print_output(file, Out, Err, opts) :
    for line in Out:
        print(line, end='')
    for line in Err:
        print(line, end='')

def output_path(file, opts) :
    f = os.path.splitext(os.path.basename(file))[0]
    return os.path.join(opts.output, f)

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
        for i in Err: print(i, end='')
        return 0
    for t in ['out', 'err']  :

        if t == 'out' :
            out = Out
            exp_out = read_lines(out_file(file, opts))
        if t == 'err' :
            out = Err
            exp_out = read_lines(err_file(file, opts))

        if len(out) != len(exp_out) :
            print("Number of lines do not match", file=sys.stderr)
            print("Output:      %d" % len(out), file=sys.stderr)
            print("Output(exp): %d" % len(exp_out), file=sys.stderr)
            print('', file=sys.stderr)
        out = [ o.replace('\r','') for o in out]
        for line in difflib.unified_diff(exp_out, out, 
                                         fromfile = 'expected', 
                                         tofile   = 'current') :
            print(line, file=sys.stderr, end='')
            error = error + 1
    return error


if __name__ == '__main__':
    main()



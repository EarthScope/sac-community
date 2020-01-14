#!/usr/bin/env python

import re
import sys
import os
import docutils.core
from docutils import nodes
from docutils.writers import latex2e
from docutils.writers.latex2e import LaTeXTranslator
from docutils.core import publish_string


def msg(*x):
    for xi in x:
        sys.stderr.write(str(xi) + ' ')
        sys.stderr.write('\n')

class MySacTranslator(latex2e.LaTeXTranslator):
    def __init__(self, document):
        latex2e.LaTeXTranslator.__init__(self, document)

    def visit_Text(self, node):
        if self.section_level <= 1:
            return
        self.out.append(self.encode(node.astext()))

    def visit_reference(self, node):
        # We need to escape #, \, and % if we use the URL in a command.
        special_chars = {ord('#'): u'\#',
                         ord('%'): u'%',
                         ord('\\'): u'\\',
                        }
        # external reference (URL)
        if 'refuri' in node:
            href = node['refuri'].translate(special_chars)
            # problematic chars double caret and unbalanced braces:
            if href.find('^^') != -1 or self.has_unbalanced_braces(href):
                self.error(
                    'External link "%s" not supported by LaTeX.\n'
                    ' (Must not contain "^^" or unbalanced braces.)' % href)
            if node['refuri'] == node.astext():
                self.out.append(r'\url{%s}' % href)
                raise nodes.SkipNode
            self.out.append(r'\href{%s}{' % href)
            return
            # internal reference
        if 'refid' in node:
            href = node['refid']
        elif 'refname' in node:
            href = self.document.nameids[node['refname']]
        else:
            raise AssertionError('Unknown reference.')

        if href[:2] == 'id':
            #print "Forcing lookup of indirect internal refernce: " + node['name'] + ' ' + href
            p = re.compile('^\.\. _' + node['name'] + ':\s+(\S+)_\s*\n', re.IGNORECASE)
            t = [ p.search(link) for link in links if p.search(link) ]
            if not t:
                print("Indirect lookup: Search failed for internal target link " + node['name'])
                sys.exit(-1)
            if len(t) != 1 :
                print("Indirect lookup: Multiple internal links found for " + node['name'])
                sys.exit(-1)
            t = t[0]
            if len(t.groups()) != 1:
                print("Indirect lookup: Search failed for internal target: " + t.group())
                sys.exit(-1)
            href = t.groups()[0]
        if not self.is_inline(node):
            self.out.append('\n')
        self.out.append('\\hyperref[%s]{' % href)
        if self._reference_label:
            self.out.append('\\%s{%s}}' %
                            (self._reference_label, href.replace('#', '')))
            raise nodes.SkipNode


        
    def visit_target(self, node):
        # This should avoid targets at the end
        return
        # Skip indirect targets:
        if ('refuri' in node       # external hyperlink
            or 'refid' in node     # resolved internal link
            or 'refname' in node): # unresolved internal link
            ## self.out.append('%% %s\n' % node)   # for debugging
            return
        self.out.append('%\n')
        self.out.append('%%  TARGET %s\n' % node)
        # do we need an anchor (\phantomsection)?
        set_anchor = not(isinstance(node.parent, nodes.caption) or
                         isinstance(node.parent, nodes.title))
        # TODO: where else can/must we omit the \phantomsection?
        self.out += self.ids_to_labels(node, set_anchor)


    def visit_title(self,node):
        # Document title
        if node.parent.tagname == 'document':
            self.push_output_collector(self.title)
            self.context.append('')
            self.pdfinfo.append('  pdftitle={%s},' %
                                self.encode(node.astext()))
        # Topic titles (topic, admonition, sidebar)
        elif (isinstance(node.parent, nodes.topic) or
              isinstance(node.parent, nodes.admonition) or
              isinstance(node.parent, nodes.sidebar)):
            self.fallbacks['title'] = latex2e.PreambleCmds.title
            classes = ','.join(node.parent['classes'])
            if not classes:
                classes = node.tagname
            self.out.append('\\DUtitle[%s]{' % classes)
            self.context.append('}\n')
        # Table caption
        elif isinstance(node.parent, nodes.table):
            self.push_output_collector(self.active_table.caption)
            self.context.append('')
        # Section title
        else:
            if self.section_level <= 1:
                self.context.append('')
                return
            if hasattr(latex2e.PreambleCmds, 'secnumdepth'):
                self.requirements['secnumdepth'] = latex2e.PreambleCmds.secnumdepth
            section_name = self.d_class.section(self.section_level)
            self.out.append('\n\n')
            # System messages heading in red:
            if ('system-messages' in node.parent['classes']):
                self.requirements['color'] = latex2e.PreambleCmds.color
                section_title = self.encode(node.astext())
                self.out.append(r'\%s[%s]{\color{red}' % (
                                section_name,section_title))
            else:
                if not section_name[-1] == '*':
                    self.out.append(r'\%s*{' % section_name)
                else:
                    self.out.append(r'\%s{' % section_name)
            if self.section_level > len(self.d_class.sections):
                # section level not supported by LaTeX
                self.fallbacks['title'] = latex2e.PreambleCmds.title
                # self.out.append('\\phantomsection%\n  ')
            # label and ToC entry:
            bookmark = []
            # add sections with unsupported level to toc and pdfbookmarks?
            if self.section_level <= __section_level__:
                section_title = self.encode(node.astext())
                bookmark.append('')
                bookmark.append('\\addcontentsline{toc}{%s}{%s}' % (section_name, section_title))
                bookmark.extend( self.ids_to_labels(node.parent, set_anchor=False))

            #print 'bookmark ' + section_name + ' ' + node.astext() + ' ' + str(self.section_level)
            if len(bookmark) > 0:
                self.context.append('%\n  '.join(bookmark) + '%\n}\n')
            else :
                self.context.append('}%\n')
            # MAYBE postfix paragraph and subparagraph with \leavemode to
            # ensure floats stay in the section and text starts on a new line.

w = latex2e.Writer()
w.translator_class = MySacTranslator

args = {
    'template': 'html/tmpl/body.tex',
    'report_level': 2,
    'halt_level': 4,
}

header =  '''
========
SACTITLE
========
SUBTITLE
========
SECTION
.......
'''

links = open('html/links/manual_links_pdf.txt').readlines()

cmd_re = re.compile('\n\w+\n\+\+') 

def footer( cmd, links ):
    # Remove cmd (and syntax) from links
    if cmd:
        p = re.compile('\.\. +_(syntax|' + cmd + '):.*') # .. cmd: 
        return ''.join( [ link for link in links if not p.findall(link) ] )
    return '\n'.join( [ link for link in links ] )

def read_rst( f, links ):
    skips = ['_commands.txt', '/manual/','/tmpl/', '/links/',
             'external_howto.txt', 'external_interface.txt', 'crr.txt',
             'hlpintro.txt',
             'spe.txt', 'sss.txt', 'transfertable.txt',
             'manual.txt', 'index.txt', 'error_messages.txt']
    rst = header
    rst += open(f, 'r').read()
    rst += '\n\n'
    cmd = None
    if not any( fragment in f for fragment in skips ):
        m = cmd_re.findall(rst)
        if not m:
            print("Error finding command name")
            sys.exit(-1)
        cmd = m[0].lower()[1:-3] # \nCOMMAND\n++
    if f == 'syntax.txt': # Syntax is defined twice, "fix" the second
        rst = rst.replace('SYNTAX\n---','SINTAX\n---')
    rst += footer( cmd, links )
    return rst

listcnt = 0

def find_txt_files():
    out = []
    skipdir = ['./html/links', './html/tmpl','./html/css','./examples']
    for root, subdirs, files in os.walk("."):
        if any([root.startswith(f) for f in skipdir]):
            continue
        for f in files:
            if f.endswith('.txt') and not f in ['manual.txt','index.txt']:
                out.append( os.path.join(root, f) )
    return out

files = sys.argv[1:]
if len(files) == 0:
    files = find_txt_files()

global __section_level__
__section_level__ = 2

requires_section_3 = [
    'saclib.txt',
    'data_access.txt',
    'transfer.txt',
    'tutorial.txt'
]

for arg in files:
    __section_level__ = 2
    if arg in requires_section_3:
        __section_level__ = 3
    if 'contents.txt' in arg:
        continue
    out = arg.replace('.txt', '.tex')
    print(arg)
    rst = read_rst( arg, links )
    # open('tmp.txt','w').write(rst)
    tex = publish_string(rst, writer=w, source_path=arg,
                         settings=None, settings_overrides=args)

    tex = tex.decode().replace('% \n\n','')
    tex = tex.replace('{longtable*}','{longtable}')
    if '{listcnt0}' in tex:
        tex  = tex.replace('{listcnt0}', '{listcnt' + str(listcnt) + '}')
        listcnt += 1
    open(out, 'w').write(tex)



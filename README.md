## SAC - Seismic Analysis Code

**Version 103.0**

SAC allows reading and writing of binary seismic data files containing time
series data, typically ground motion recorded by seismometers. This program is
used primarily in seismological research.  SAC includes a number of filtering
and deconvolution functions to process seismic data. SAC runs on Linux, macOS
and Windows under Cygwin.

### History
SAC is hosted for the seismological research community by the
[EarthScope Consortium](www.earthscope.edu) and was derived from Lawrence
Livermore National Laboratory's (LLNL) SAC2000. From the early 1980's to the
mid 1990's, SAC, as it was originally named, was distributed to the
seismological community as a Fortran program by [LLNL](www.llnl.gov).  In the
mid 1990's, the source was converted to C, renamed SAC2000 and generally
difficult to obtain. In 2004, IRIS started distributing derivative versions of SAC2000
under a new License negotiated between LLNL and IRIS. Now, following the termination of the
license agreement between LLNL and IRIS, EarthScope releases this derivative work
under an open source license identified in this repository (see License below).

Other versions of SAC include:
* [SAC/BRIS](https://members.elsi.jp/~george/sac-bugs.html) based on the original
Fortran version that runs on the Mac.
* [SAC2000](https://github.com/LLNL/SAC2000) based on the original SAC2000 code base,
that the EarthScope version branched from, now provided by LLNL.

### Contents

* Overview
* Package contents
* Environment Setup
* Initialization macro
* Matlab Interface
* PNG Support
* If you need help ...
* Cutting a release
* License
* References

### Overview

This `README` file is in the top directory (named sac) of a directory tree that
has either been installed from a SAC source distribution or produced by the
expansion of a SAC binary-distribution compressed tar file.  The default installation 
directory is `/usr/local/sac/`, which is then set in the environmental variable `SACHOME`.  (See Environment Setup below.)

In the past, SAC binary-distributions for the following operating systems have been built.  This repository is not intended
for distribution of such binary builds, so is left as an exercise to the user.

1. Mac OSX (64 bit) build: built on 10.13 will run on versions starting with
   10.8.  XQuartz/X11 is required to display graphics and can be downloaded
   from https://www.xquartz.org/
2. Linux 64-bit: built on CentOS release 7.0-64 (Final).
               Running on ...

    |Build \Runs | CentOS 7 | CentOS 8 | Ubuntu 14 | Ubuntu 16 | Ubuntu 18 |
    |------------|----------|----------|-----------|-----------|-----------|
    |CentOS 7    |    X     |    X     |    X      |    X      |    X      |
    |CentOS 8    |    X     |    X     |           |           |           |
    |Ubuntu 14   |    X*    |    X*    |    X      |    X      |           |
    |Ubuntu 16   |    X*    |    X*    |    X*     |    X      |           |
    |Ubuntu 18   |          |    X*    |           |           |    X      |
   - *Runs but mismatch in library is detected

   - CentOS - (7,8) libcurl, libxml2 already installed
   - Ubuntu - (14,16,18) libcurl and libxml2 need to be installed
            Version should be the openssl variant of libcurl (default)

Other platforms or operating systems did not see regular binary distributions.
Successful builds have been made and tested on Solaris 11, Linux 32-bit; and Mac
OSX 10.6 and above.  Because of licensing issues, a Windows Cygwin binary
distribution cannot be provided.

Optimization was not generally used in any of the binary builds. For more information on
supported operating systems, see HISTORY and CHANGES in this directory.  It is
assumed you have X windows on your system.

On Mac OSX, if the system refuses to open SAC due to it being from "an
unidentified developer" or the "developer cannot be verified", try
doing the following: In the Finder, control-click the app icon, then
choose Open from the shortcut menu.  If that does not work, see the
following:

https://support.apple.com/guide/mac-help/open-a-mac-app-from-an-unidentified-developer-mh40616/10.15/mac/10.15

### Package contents

The contents of the package are relative to `SACHOME` (default `/usr/local/sac`)
|File| Contents |
|-|-|
|sac/README           | this file
|sac/HISTORY          | update history
|sac/CHANGES          | Details about changes made for each update
|sac/bin/             | contains executable programs and scripts
| | |
| sac/bin/sac          | Seismic Analysis Code binary
| sac/bin/sac-config   | Helper script for compilation with SAC libraries
| sac/bin/sacinit.sh   | sh and bash environmental setup for SAC
| sac/bin/sacinit.csh  | csh and tcsh environmental setup for SAC
| | |
| sac/bin/bbfswap      | See sac/utils/README_utils
| sac/bin/sacswap      | See sac/utils/README_utils
| sac/bin/saclst       | See sac/utils/README_utils
| sac/bin/sgfswap      | See sac/utils/README_utils
| sac/bin/sgftops      | See sac/utils/README_utils
| sac/bin/sgftoeps.csh | See sac/utils/README_utils
| sac/bin/sgftox.csh   | See sac/utils/README_utils
| sac/bin/unvis        | See sac/utils/README_utils
| | |
| sac/lib/             | SAC libraries for user-generated sac programs
| sac/lib/README_lib   | Overview of sac-config and IO libraries
| sac/lib/libsacio.a   | Library for reading and writing SAC files
| sac/lib/sacio.a      | Same as sac/lib/libsacio.a
| sac/lib/libsac.a     | See sac/lib/README_lib
| libmxmlev.a          | Evalresp library
| libspline.a          | Evalresp library
| libevalresp.a        | Evalresp library
| libevalresp_log.a    | Evalresp library
| | |
|sac/doc              | sac/doc/README and sac/doc/examples/README give an overview of examples for using the libsacio.a and libsac.a libraries
|sac/aux/             |  contains files that are used by SAC during execution.  On some Windows Cygwin installations, aux is renamed winaux. (This depends on the version of Cygwin and how SAC was built on those systems.)
| sac/macros/         | contains example macros and data
| sac/utils           | makefile, sources for handling Sac files outside of SAC.  See sac/utils/README_utils for details
| sac/License          | the Apache 2 License
| sac/Editline.license | License for Editline for Command line edits and history

### Environment Setup

`SACHOME`: This must be set so that SAC can be found on the computer.
The default when SAC is installed from sources is `/usr/local/sac/`.

SAC needs to be able to find auxillary (`${SACHOME}/aux/`) data that are
installed along side the executable / binary program.  SAC will
attempt to find these data using
  1) the `SACAUX` environmental variable
  2) the default install location
  3) a set of typical install locations.
If all these fail, a message is displayed saying one must set the
`SACAUX` environment variable and SAC will exit.

There are options within SAC that can be set through environmental
variables. In `SACHOME/bin` the scripts sacinit.csh (csh and tcsh) and
sacinit.sh (bash) contain lines that set up your environment
(including `SACAUX`).  See the script for your shell for descriptions of
the options and the syntax for the commands.  If your distribution was
built from sources, these sacinit scripts were created during the
installation of the SAC package and already have the correct path for
the computer on which it was built.  If your distribution is binary,
you may need to edit the directory given for SACHOME.

#### Environment Example

If `SACHOME is `/usr/local/sac` and the sacinit file is modified accordingly,
to set `PATH`, `SACAUX`, and other options for SAC, do the following:

##### csh/tsch
  For csh and tcsh, edit ~/.cshrc adding the lines

    setenv SACHOME /usr/local/sac
    source ${SACHOME}/bin/sacinit.csh

  After saving and exiting the file, enter

    source ~/.cshrc

##### bash / sh
  For bash, edit ~/.bashrc adding the lines

    export SACHOME=/usr/local/sac
    . ${SACHOME}/bin/sacinit.sh

  After saving and exiting the file, enter

    . ~/.bashrc

  To verify that things are working, from a command line, enter

    echo $PATH ; echo $SACHOME ; echo $SACAUX

If one wants to change any of the options or if more than one person is using
the same SAC distribution, it is best to copy the path and environment
lines directly into their own shell initialization file.

### Initialization macro


It is recommended that each user create an initialization macro that runs
whenever SAC is started.  To do this, one creates a startup macro file (e.g.,
/usr/local/macros/init.m) and aliases SAC as follows:

    # For sh/bash
    alias sac='"${SACHOME}/bin/sac" "${SACHOME}/macros/init.m"'

    # For csh/tcsh
    alias sac '"${SACHOME}/bin/sac" "${SACHOME}/macros/init.m"'

Here is a sample initialization macro:

    lh columns 2 files none; qdp 10000 ; xdiv power off ; xlabel 'Time (sec)'
    setmacro /usr/local/macros ; transcript history file ./.sachist

For the meaning of these entries (lh, qdp, xdiv, xlabel, setmacro,
transcript), use the help feature in SAC.  For further discussion of the
transcript command and command-line editing, see SACHOME/utils/README_utils.

### Matlab Interface

##### Solaris
Set `LD_LIBRARY_PATH` to include matlab shared object libraries

    setenv LD_LIBRARY_PATH `${LD_LIBRARY_PATH}:/usr/local/matlab/extern/lib/sol2`

##### Mac OS X
The `DYLD_LIBRARY_PATH` needs to be set

    setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:/Applications/MATLAB74/bin/maci

  These path may differ on your machine. The path needs to include
   - The Matlab Engine Library (libeng)
   - The Matlab Executable Library (libmex)

File `utils/README_utils` describes some Matlab macros provided by a user.

### PNG Support

Saving figures as PNG files is possible, but starting with version v101.6 the
default is not to include PNG support because the default PNG libraries are
dynamic libraries so that a binary SAC distribution built on one computer may
not be portable.  PNG support can be included if one builds SAC from sources
with the configure option --enable-png.

### If you need help ...

1.  If you have SAC working, use the help command from within SAC.  You can
    print hard-copies of the help files using printhelp. HTML versions of the
    help files can be found at URL <http://ds.iris.edu/files/sac-manual/>.
    Also, a link from that site takes you to a PDF-format single-file version of
    the manual in which the search command and links from the side-bar table of
    contents allow one to navigate the manual efficiently.
2.  Some features of SAC are described in the `CHANGES`, and changes in new
    versions are summarized in `HISTORY`.  Both files are in this directory.
    For a more readable discussion of new features in an updated distribution,
    see the INTRO help file.
3.  A place to share problems and find solutions is the SAC Community mailing
    list: [SAC Community](https://groups.google.com/a/earthscope.org/g/sac-community)

### Cutting a release

The version is authored in one place: the `AC_INIT` line in `configure.ac`.
Everything built through `configure` (the version banner, `sac-config`,
distribution tarball names, the macOS `.dmg`) is derived from it
automatically. A few files that `configure` does not reach are kept in sync
by `config/set-version`.

1.  Edit the version in `configure.ac` (`AC_INIT`, second argument).
2.  Run `./bootstrap` and commit the regenerated `configure` along with
    `configure.ac`. CI builds from the committed `configure` rather than
    running `bootstrap`, so an uncommitted regeneration ships the previous
    version.
3.  Run `config/set-version` and commit the files it updates (the Windows
    build config, the Inno Setup installers, the macOS app bundle, the
    manual title page, and this README).
4.  Add a `YYYY/MM Version X.Y released` heading to `CHANGES`.
5.  Confirm everything agrees with `config/set-version --check`.
6.  Tag `vX.Y`, matching the two-component `configure.ac` version, and push
    the tag. The release workflow publishes a draft, prerelease GitHub
    release, which still needs manual promotion.

### License

   Copyright 2025 - EarthScope Consortium

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

### References

- Savage, B. (2021), sacio: A library for Seismic Analysis Code data files,
Journal of Open Source Software, 6(64), 3619, doi:10.21105/joss.03619.

- Goldstein, P., A. Snoke, (2005), "Sac Availability for the IRIS Community",
Incorporated Institutions For Seismology Data Management Center Electronic
Newsletter,
<http://ds.iris.edu/ds/newsletter/vol7/no1/sac-availability-for-the-iris-community/>

- Goldstein, P., D. Dodge, M. Firpo, Lee Minner (2003), "SAC2000: Signal
processing and analysis tools for seismologists and engineers", Invited
contribution to "The IASPEI International Handbook of Earthquake and
Engineering Seismology", Edited by WHK Lee, H. Kanamori, P.C. Jennings, and
C. Kisslinger, Academic Press, London.

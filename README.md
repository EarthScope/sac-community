# SAC (Seismic Analysis Code) Community Edition

This repository contains the source code for SAC Community Edition, a community-maintained continuation of SAC (Seismic Analysis Code). SAC allows reading and writing of binary seismic data files containing time series data, typically ground motion recorded by seismometers. This program is used primarily in seismological research.  SAC includes a number of filtering and deconvolution functions to process seismic data.

## Contributing

This software is not under active development by EarthScope, but contributions are welcome.  The process is as follows:

1. Fork the repository and make your changes against your fork.
2. Open a PR against the `main` branch of this repository, ensuring
that the checks pass.

If you would like to become a maintainer of this software please contact [help@earthscope.org](mailto:help@earthscope.org) to express your willingness.

## Download releases

Releases are available from:
[https://github.com/EarthScope/sac-community/releases](https://github.com/EarthScope/sac-community/releases)

This includes the source code, and and pre-built binaries for Linux and macOS.

If there are no binaries for your platform, or the pre-built binaries do not work, your best option is to compile from source code.  See [README.buildsac](README.buildsac) for instructions.

The `sac` program requires X11, libxml2, and libcurl.  On Linux, the shared libraries for all three must be installed.  On macOS, libxml2 and libcurl are system libraries; install X11 from [XQuartz](https://www.xquartz.org/).

For pre-built binaries on macOS, the system may refuse to run the `sac` binary due to security restrictions.  Use the following command to allow sac (or other binaries in the distribution) to run:

```
xattr -dr com.apple.quarantine /path/to/sac
```

## Environment Setup

`SACHOME`: This must be set so that SAC can be found on the computer. The default when SAC is installed from sources is `/usr/local/sac/`.

SAC needs to be able to find auxillary (`${SACHOME}/aux/`) data that are installed along side the executable / binary program.  SAC will attempt to find these data using

1. the `SACAUX` environmental variable
2. the default install location
3. a set of typical install locations.
If all these fail, a message is displayed saying one must set the `SACAUX` environment variable and SAC will exit.

There are options within SAC that can be set through environmental variables. In `SACHOME/bin` the scripts sacinit.csh (csh and tcsh) and sacinit.sh (bash) contain lines that set up your environment (including `SACAUX`).  See the script for your shell for descriptions of the options and the syntax for the commands.  If your distribution was built from sources, these sacinit scripts were created during the installation of the SAC package and already have the correct path for the computer on which it was built.  If your distribution is binary, you may need to edit the directory given for SACHOME.

### Environment Example

If `SACHOME` is `/usr/local/sac` and the sacinit file is modified accordingly, to set `PATH`, `SACAUX`, and other options for SAC, do the following:

#### bash / sh

  For bash, edit ~/.bashrc adding the lines

```
export SACHOME=/usr/local/sac
. ${SACHOME}/bin/sacinit.sh
```

  After saving and exiting the file, enter

```
. ~/.bashrc
```

  To verify that things are working, from a command line, enter

```
echo $PATH ; echo $SACHOME ; echo $SACAUX
```

#### csh/tsch

  For csh and tcsh, edit ~/.cshrc adding the lines

```
setenv SACHOME /usr/local/sac
source ${SACHOME}/bin/sacinit.csh
```

  After saving and exiting the file, enter

```
source ~/.cshrc
```

If one wants to change any of the options or if more than one person is using the same SAC distribution, it is best to copy the path and environment lines directly into their own shell initialization file.

## Initialization macro

It is recommended that each user create an initialization macro that runs whenever SAC is started.  To do this, one creates a startup macro file (e.g., /usr/local/macros/init.m) and aliases SAC as follows:

```
# For sh/bash
alias sac='"${SACHOME}/bin/sac" "${SACHOME}/macros/init.m"'

# For csh/tcsh
alias sac '"${SACHOME}/bin/sac" "${SACHOME}/macros/init.m"'
```

Here is a sample initialization macro:

```
lh columns 2 files none; qdp 10000 ; xdiv power off ; xlabel 'Time (sec)'
setmacro /usr/local/macros ; transcript history file ./.sachist
```

For the meaning of these entries (lh, qdp, xdiv, xlabel, setmacro, transcript), use the help feature in SAC.  For further discussion of the transcript command and command-line editing, see SACHOME/utils/README_utils.

## PNG Support

Saving figures as PNG files is possible, but starting with version v101.6 the default is not to include PNG support because the default PNG libraries are dynamic libraries so that a binary SAC distribution built on one computer may not be portable.  PNG support can be included if one builds SAC from sources with the configure option --enable-png.

## If you need help ...

1. If you have SAC working, use the help command from within SAC.  You can print hard-copies of the help files using printhelp. HTML versions of the help files can be found in the [SAC Manual](https://ds.iris.edu/files/sac-manual/index.html). Also, a link from that site takes you to a PDF-format single-file version of the manual in which the search command and links from the side-bar table of contents allow one to navigate the manual efficiently.
2. Some features of SAC are described in the `CHANGES`. For a more readable discussion of new features in an updated distribution, see the INTRO help file.d
3. A place to share problems and find solutions is the SAC Community mailing list: [SAC Community](https://groups.google.com/a/earthscope.org/g/sac-community)

## History

SAC is hosted for the seismological research community by the [EarthScope Consortium](https://www.earthscope.edu) and was derived from Lawrence Livermore National Laboratory's (LLNL) SAC2000. From the early 1980's to the mid 1990's, SAC, as it was originally named, was distributed to the seismological community as a Fortran program by [LLNL](https://www.llnl.gov).  In the mid 1990's, the source was converted to C, renamed SAC2000 and generally difficult to obtain. In 2004, IRIS started distributing derivative versions of SAC2000 under a new License negotiated between LLNL and IRIS. Now, following the termination of the license agreement between LLNL and IRIS, EarthScope releases this derivative work under an open source license identified in this repository (see License below).

Other versions of SAC include:

- [SAC/BRIS](https://members.elsi.jp/~george/sac-bugs.html) based on the original
Fortran version that runs on the Mac.
- [SAC2000](https://github.com/LLNL/SAC2000) based on the original SAC2000 code base, that the EarthScope version branched from, now provided by LLNL.

## License

   Copyright 2026 - EarthScope Consortium

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   [http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

## References

- Savage, B. (2021), sacio: A library for Seismic Analysis Code data files,
Journal of Open Source Software, 6(64), 3619, doi:10.21105/joss.03619.

- Goldstein, P., A. Snoke, (2005), "Sac Availability for the IRIS Community",
Incorporated Institutions For Seismology Data Management Center Electronic
Newsletter, [http://ds.iris.edu/ds/newsletter/vol7/no1/sac-availability-for-the-iris-community/](http://ds.iris.edu/ds/newsletter/vol7/no1/sac-availability-for-the-iris-community/)

- Goldstein, P., D. Dodge, M. Firpo, Lee Minner (2003), "SAC2000: Signal
processing and analysis tools for seismologists and engineers", Invited
contribution to "The IASPEI International Handbook of Earthquake and
Engineering Seismology", Edited by WHK Lee, H. Kanamori, P.C. Jennings, and
C. Kisslinger, Academic Press, London.

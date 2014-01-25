function varargout = readsac_gui
% READSAC_GUI is a GUI version of READSAC.

% *************************************************************************
% This program was developed originally from the program readsac.m of the 
% package MatSeis 1.6 by Sandia National Laboratories.
%
% Mark Harris, mharris@sandia.gov
% Copyright (c) 1996-2001 Sandia National Laboratories. All rights 
% reserved.
% *************************************************************************
%
% Copyright, 19, The Board of Governors of the Los Alamos National 
% Security, LLC. This software was produced under a U. S. Government 
% contract (DE-AC52-06NA25396) by Los Alamos National Laboratory, which is
% operated by the Los Alamos National Security, LLC for the U. S. 
% Department of Energy. The U. S. Government is licensed to use, reproduce,
% and distribute this software. Permission is granted to the public to copy
% and use this software without charge, provided that this Notice and any
% statement of authorship are reproduced on all copies. Neither the 
% Government nor the LANS makes any warranty, express or implied, or 
% assumes any liability or responsibility for the use of this software.
%
%        Xiaoning Yang, xyang@lanl.gov   2002, 2008, 2011
%

% check input
if nargin > 0
    error(' readsac_gui does not take input arguments !!!')
end

% current directory
cdr = pwd;

% get file names and go to directory containing files
[filenames, filepath] = uigetfile('*', 'Select files to read', ...
    'multiselect', 'on');
if ~iscell(filenames)
    filenames = {filenames};
end
cd(filepath)

switch nargout
    case 1
        varargout{1} = readsac(filenames);
    case 2
        [varargout{1}, varargout{2}] = readsac(filenames);
    otherwise
        [varargout{1}, varargout{2}, varargout{3}] = readsac(filenames);
end

cd(cdr)

function flag = issac(filenames)
% FLAG = ISSAC(FILENAMES); determines whether input files are SAC files.
% FILENAMES contains the names of input files to be checked. It may consist
% of a string for a single file or a cell array of strings for multiple 
% file names. Wildcard '*' is accepted. If FILENAMES is omitted, all files
% in the current directory are checked. FLAG is a logical array indicating
% whether input file(s) are SAC files.

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

% process input argument
if nargin < 1
    filenames = dir;
    filenames = filenames(~[filenames.isdir]);
    filenames = {filenames.name};
else
    % check input argument
    if nargin ~= 1
        error(' Wrong number of input arguments !!!')
    end
    if ~iscell(filenames) && (~ischar(filenames) || size(filenames, 1) ...
        ~= 1)
        error('Input must be a string or a cell array of strings !!!')
    elseif ischar(filenames)
        filenames = cellstr(filenames);
    end

    % treat wildcard
    nf = length(filenames);
    filename_temp = cell(nf, 1);
    for i = 1:nf
        if ismember('*', filenames{i})
            [pathstri, ~] = fileparts(filenames{i}); % separate path/name
            filenamei = dir(deblank(filenames{i}));
            if isempty(pathstri)
                filename_temp(i) = {{filenamei.name}};
            else  % add path to filenames
                filenamei_temp = char({filenamei.name});
                pathstr = [pathstri, filesep];
                pathstr = repmat(pathstr, size(filenamei_temp, 1), 1);
                filenamei = [pathstr filenamei_temp];
                filename_temp(i) = {cellstr(filenamei)};
            end
        else
            filename_temp(i) = {filenames(i)};
        end
    end
    filenames = [filename_temp{:}];
end
nfiles = length(filenames);

% initialize output flag
flag = false(nfiles, 1);

% check files
for f = 1:nfiles

    % open file
    fid = fopen(filenames{f}, 'r');

    if fid ~= -1
        % read SAC file header
        [~, count1] = fread(fid, [70 1], 'float32');
        if count1 ~= 70, continue; end
        [B, count2] = fread(fid, [40 1], 'int32');
        if count2 ~= 40, continue; end
        if ~ismember(B(7), 1:6) % non-native byte order
            fclose(fid);
            [~, ~, endian] = computer;
            if strcmp(endian, 'B')
                fid = fopen(filenames{f}, 'r', 'l');
            else
                fid = fopen(filenames{f}, 'r', 'b');
            end
            [~] = fread(fid, [70 1], 'float32');
            B = fread(fid, [40 1], 'int32');
        end
        [~, count3] = fread(fid, [1 192], 'char');
        if count3 ~= 192, continue; end

        % valid SAC data file
        if B(10) >= 1 && (B(16) == 1 || B(16) == 2 || B(16) == 3 || ...
            B(16) == 4 || B(16) == 51) && (B(36) == 0 || B(36) == 1)
            flag(f) = true;
        end

        % close file.
        fclose(fid);
    end
end      


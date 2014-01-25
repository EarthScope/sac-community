function status = writesac_hdr(s)
% STATUS = WRITESAC_HDR(S); writes SAC header variables in structure S to 
% existing SAC data files. STATUS has the same length as S with logical 
% values indicating headers of which files are written successfully.
%
% Structure S needs to have following fields that correspond to SAC header
% variables except the first one. The data type and length (for a 
% character variable) of these variables must be consistent with SAC 
% definitions. Values of I-type header variables must be their alphanumeric
% names (all upper case), not corresponding integers. Additional fields of
% S, if they exist, are ignored.
%
%    FILENAME   - File name of SAC file
%    DELTA      - Data sampling interval
%    DEPMIN     - Minimum value of dependent variable
%    DEPMAX     - Maximum value of dependent variable
%    SCALE      - Multiplying factor for dependent variable
%    ODELTA     - Observed increment if different from nominal
%    B          - Beginning value of independent variable     
%    E          - Ending value of independent variable     
%    O          - Event origin time                        
%    A          - First arrival time                       
%    INTERNAL1  - First internal variable                  
%    T0         - First user-defined time picks or markers 
%    T1         - Second user-defined time picks or markers 
%    T2         - Third user-defined time picks or markers 
%    T3         - Fourth user-defined time picks or markers 
%    T4         - Fifth user-defined time picks or markers 
%    T5         - Sixth user-defined time picks or markers 
%    T6         - Seventh user-defined time picks or markers 
%    T7         - Eighth user-defined time picks or markers 
%    T8         - Ninth user-defined time picks or markers 
%    T9         - Tenth user-defined time picks or markers 
%    F          - Final or end of event time               
%    RESP0      - First instrument response parameter      
%    RESP1      - Second instrument response parameter      
%    RESP2      - Third instrument response parameter      
%    RESP3      - Fourth instrument response parameter      
%    RESP4      - Fifth instrument response parameter      
%    RESP5      - Sixth instrument response parameter      
%    RESP6      - Seventh instrument response parameter      
%    RESP7      - Eighth instrument response parameter      
%    RESP8      - Ninth instrument response parameter      
%    RESP9      - Tenth instrument response parameter      
%    STLA       - Station latitude                         
%    STLO       - Station longitude                        
%    STEL       - Station elevation                        
%    STDP       - Station depth                            
%    EVLA       - Event latitude                           
%    EVLO       - Event longitude                          
%    EVEL       - Event elevation                          
%    EVDP       - Event depth                              
%    MAG        - Event magnitude                          
%    USER0      - First user-defined variable                   
%    USER1      - Second user-defined variable                   
%    USER2      - Third user-defined variable                   
%    USER3      - Fourth user-defined variable                   
%    USER4      - Fifth user-defined variable               
%    USER5      - Sixth user-defined variable                   
%    USER6      - Seventh user-defined variable                   
%    USER7      - Eighth user-defined variable                   
%    USER8      - Ninth user-defined variable                   
%    USER9      - Tenth user-defined variable                   
%    DIST       - Station-to-event distance (km)
%    AZ         - Event-to-station azimuth (degree)
%    BAZ        - Station-to-event azimuth (degree)
%    GCARC      - Station-to-event great-circle arc length (degree)
%    INTERNAL2  - Second internal variable                              
%    INTERNAL3  - Third internal variable                              
%    DEPMEN     - Mean value of dependent variable                     
%    CMPAZ      - Component azimuth                                    
%    CMPINC     - Component incident angle                             
%    XMINIMUM   - Minimum value of X (spectral file only)              
%    XMAXIMUM   - Maximum value of X (spectral file only)              
%    YMINIMUM   - Minimum value of Y (spectral file only)              
%    YMAXIMUM   - Maximum value of Y (spectral file only)              
%    UNUSED1    - First unused variable                                
%    UNUSED2    - Second unused variable                                
%    UNUSED3    - Third unused variable                                
%    UNUSED4    - Fourth unused variable                                
%    UNUSED5    - Fifth unused variable                                
%    UNUSED6    - Sixth unused variable                                
%    UNUSED7    - Seventh unused variable                                
%    NZYEAR     - GMT year corresponding to reference time               
%    NZJDAY     - GMT julian day corresponding to reference time               
%    NZHOUR     - GMT hour corresponding to reference time               
%    NZMIN      - GMT minute corresponding to reference time               
%    NZSEC      - GMT second corresponding to reference time               
%    NZMSEC     - GMT milisecond corresponding to reference time               
%    NVHDR      - Header version
%    NORID      - Origin ID     
%    NEVID      - Event ID      
%    NPTS       - Number of data points
%    INTERNAL4  - Fourth internal variable
%    NWFID      - Waveform ID
%    NXSIZE     - Spectral length (spectral file only)
%    NYSIZE     - Spectral width (spectral file only)
%    UNUSED8    - Eighth unused variable             
%    IFTYPE     - Type of file                       
%    IDEP       - Type of dependent variable            
%    IZTYPE     - Reference-time equivalence         
%    UNUSED9    - Ninth unused variable              
%    IINST      - Type of recording instrument       
%    ISTREG     - Station geographic region          
%    IEVREG     - Event geographic region          
%    IEVTYP     - Type of event                    
%    IQUAL      - Quality of data                  
%    ISYNTH     - Synthetic data flag              
%    IMAGTYP    - Magnitude type                   
%    IMAGSRC    - Magnitude source                 
%    UNUSED10   - Tenth unused variable              
%    UNUSED11   - Eleventh unused variable              
%    UNUSED12   - Twelveth unused variable              
%    UNUSED13   - Thirteenth unused variable              
%    UNUSED14   - Fourteenth unused variable              
%    UNUSED15   - Fifteenth unused variable              
%    UNUSED16   - Sixteenth unused variable              
%    UNUSED17   - Seventeenth unused variable              
%    LEVEN      - True if data is evenly spaced    
%    LPSPOL     - True if station polarity follows left-hand rule
%    LOVROK     - True if it is ok to overwrite this file on disk
%    LCALDA     - True if DIST, AZ, BAZ and GCARC are to be calculated from 
%                 station and event coordinates
%    UNUSED18   - Eighteenth unused variable              
%    KSTNM      - Station name
%    KEVNM      - Event name
%    KHOLE      - Hole ID for nuclear test
%    KO         - Event origin time ID    
%    KA         - First arrival time ID    
%    KT0        - First user-defined time pick ID    
%    KT1        - Second user-defined time pick ID    
%    KT2        - Third user-defined time pick ID    
%    KT3        - Fourth user-defined time pick ID    
%    KT4        - Fifth user-defined time pick ID    
%    KT5        - Sixth user-defined time pick ID    
%    KT6        - Seventh user-defined time pick ID    
%    KT7        - Eighth user-defined time pick ID    
%    KT8        - Ninth user-defined time pick ID    
%    KT9        - Tenth user-defined time pick ID    
%    KF         - Final or end event time ID    
%    KUSER0     - First user-defined text string
%    KUSER1     - Second user-defined text string
%    KUSER2     - Third user-defined text string
%    KCMPNM     - Component name
%    KNETWK     - Network name
%    KDATRD     - Date data were read onto computer
%    KINST      - Generic name of recording instrument

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
%           Xiaoning Yang	2002, 2008, 2011
%

% check input
if nargin < 1
    help writesac_hdr
    return
elseif nargin ~= 1
    error(' Wrong number of input arguments !!!')
end

nfiles = length(s);
status = false(nfiles,1);

% write headers of all files
for f = 1:nfiles
    
    % check if file exists
    if ~exist(s(f).FILENAME, 'file')
        error([' File ', s(f).FILENAME, ' does not exist !!!'])
    end
    
    % open file
    fid = fopen(s(f).FILENAME, 'r');
    fseek(fid, 280, -1);
    B = fread(fid, [40 1], 'int32');    
    fclose(fid);
    if ~ismember(B(7), 1:6) % non-native byte order
        [~, ~, endian] = computer;
        if strcmp(endian, 'B')
            fid = fopen(s(f).FILENAME, 'r+', 'l');
        else
            fid = fopen(s(f).FILENAME, 'r+', 'b');
        end
    else
        fid = fopen(s(f).FILENAME, 'r+');
    end
    fseek(fid, 280, -1);
    B = fread(fid, [40 1], 'int32');    
    
    % write to valid SAC data file
    if fid ~= -1  && B(10) >= 1 && (B(16) == 1 || B(16) == 2 || ...
            B(16) == 3 || B(16) == 4 || B(16) == 51) && (B(36) == 0 ...
            || B(36) == 1)

        A = zeros(70,1);
        B = zeros(40,1);
        C = cell(24,1);
        A(1) = s(f).DELTA;
        A(2) = s(f).DEPMIN;
        A(3) = s(f).DEPMAX;
        A(4) = s(f).SCALE;
        A(5) = s(f).ODELTA;
        A(6) = s(f).B;
        A(7) = s(f).E;
        A(8) = s(f).O;
        A(9) = s(f).A;
        A(10) = s(f).INTERNAL1;
        A(11) = s(f).T0;
        A(12) = s(f).T1;
        A(13) = s(f).T2;
        A(14) = s(f).T3;
        A(15) = s(f).T4;
        A(16) = s(f).T5;
        A(17) = s(f).T6;
        A(18) = s(f).T7;
        A(19) = s(f).T8;
        A(20) = s(f).T9;
        A(21) = s(f).F;
        A(22) = s(f).RESP0;
        A(23) = s(f).RESP1;
        A(24) = s(f).RESP2;
        A(25) = s(f).RESP3;
        A(26) = s(f).RESP4;
        A(27) = s(f).RESP5;
        A(28) = s(f).RESP6;
        A(29) = s(f).RESP7;
        A(30) = s(f).RESP8;
        A(31) = s(f).RESP9;
        A(32) = s(f).STLA;
        A(33) = s(f).STLO;
        A(34) = s(f).STEL;
        A(35) = s(f).STDP;
        A(36) = s(f).EVLA;
        A(37) = s(f).EVLO;
        A(38) = s(f).EVEL;
        A(39) = s(f).EVDP;
        A(40) = s(f).MAG;
        A(41) = s(f).USER0;
        A(42) = s(f).USER1;
        A(43) = s(f).USER2;
        A(44) = s(f).USER3;
        A(45) = s(f).USER4;
        A(46) = s(f).USER5;
        A(47) = s(f).USER6;
        A(48) = s(f).USER7;
        A(49) = s(f).USER8;
        A(50) = s(f).USER9;
        A(51) = s(f).DIST;
        A(52) = s(f).AZ;
        A(53) = s(f).BAZ;
        A(54) = s(f).GCARC;
        A(55) = s(f).INTERNAL2;
        A(56) = s(f).INTERNAL3;
        A(57) = s(f).DEPMEN;
        A(58) = s(f).CMPAZ;
        A(59) = s(f).CMPINC;
        A(60) = s(f).XMINIMUM;
        A(61) = s(f).XMAXIMUM;
        A(62) = s(f).YMINIMUM;
        A(63) = s(f).YMAXIMUM;
        A(64) = s(f).UNUSED1;
        A(65) = s(f).UNUSED2;
        A(66) = s(f).UNUSED3;
        A(67) = s(f).UNUSED4;
        A(68) = s(f).UNUSED5;
        A(69) = s(f).UNUSED6;
        A(70) = s(f).UNUSED7;
        B(1) = s(f).NZYEAR;
        B(2) = s(f).NZJDAY;
        B(3) = s(f).NZHOUR;
        B(4) = s(f).NZMIN;
        B(5) = s(f).NZSEC;
        B(6) = s(f).NZMSEC;
        B(7) = 6;   % enforce a version number for later byte order check
        B(8) = s(f).NORID;
        B(9) = s(f).NEVID;
        B(10) = s(f).NPTS;
        B(11) = s(f).INTERNAL4;
        B(12) = s(f).NWFID;
        B(13) = s(f).NXSIZE;
        B(14) = s(f).NYSIZE;
        B(15) = s(f).UNUSED8;
        switch s(f).IFTYPE
            case 'ITIME', B(16) = 1;
            case 'IRLIM', B(16) = 2;
            case 'IAMPH', B(16) = 3;
            case 'IXY', B(16) = 4;
            case 'IXYZ', B(16) = 51;
            otherwise, B(16) = NaN;
        end
        switch s(f).IDEP
            case 'IUNKN', B(17) = 5;
            case 'IDISP', B(17) = 6;
            case 'IVEL', B(17) = 7;
            case 'IACC', B(17) = 8;
            case 'IVOLTS', B(17) = 50;
            otherwise, B(17) = NaN;
        end
        switch s(f).IZTYPE
            case 'IUNKN', B(18) = 5;
            case 'IB', B(18) = 9;
            case 'IDAY', B(18) = 10;
            case 'IO', B(18) = 11;
            case 'IA', B(18) = 12;
            case 'IT0', B(18) = 13;
            case 'IT1', B(18) = 14;
            case 'IT2', B(18) = 15;
            case 'IT3', B(18) = 16;
            case 'IT4', B(18) = 17;
            case 'IT5', B(18) = 18;
            case 'IT6', B(18) = 19;
            case 'IT7', B(18) = 20;
            case 'IT8', B(18) = 21;
            case 'IT9', B(18) = 22;
            otherwise, B(18) = NaN;
        end
        B(19) = s(f).UNUSED9;
        B(20) = s(f).IINST;
        B(21) = s(f).ISTREG;
        B(22) = s(f).IEVREG;
        switch s(f).IEVTYP
            case 'IUNKN', B(23) = 5;
            case 'INUCL', B(23) = 37;
            case 'IPREN', B(23) = 38;
            case 'IPOSTN', B(23) = 39;
            case 'IQUAKE', B(23) = 40;
            case 'IPREQ', B(23) = 41;
            case 'IPOSTQ', B(23) = 42;
            case 'ICHEM', B(23) = 43;
            case 'IOTHER', B(23) = 44;
            case 'IQB', B(23) = 70;
            case 'IQB1', B(23) = 71;
            case 'IQB2', B(23) = 72;
            case 'IQBX', B(23) = 73;
            case 'IQMT', B(23) = 74;
            case 'IEQ', B(23) = 75;
            case 'IEQ1', B(23) = 76;
            case 'IEQ2', B(23) = 77;
            case 'IME', B(23) = 78;
            case 'IEX', B(23) = 79;
            case 'INU', B(23) = 80;
            case 'INC', B(23) = 81;
            case 'IO_', B(23) = 82;
            case 'IL', B(23) = 83;
            case 'IR', B(23) = 84;
            case 'IT', B(23) = 85;
            case 'IU', B(23) = 86;
            otherwise, B(23) = NaN;
        end
        switch s(f).IQUAL
            case 'IOTHER', B(24) = 44;
            case 'IGOOD', B(24) = 45;
            case 'IGLCH', B(24) = 46;
            case 'IDROP', B(24) = 47;
            case 'ILOWSN', B(24) = 48;
            otherwise, B(24) = NaN;
        end
        switch s(f).ISYNTH
            case 'IRLDTA', B(25) = 49;
            otherwise, B(25) = NaN;
        end
        switch s(f).IMAGTYP
            case 'IMB', B(26) = 52;
            case 'IMS', B(26) = 53;
            case 'IML', B(26) = 54;
            case 'IMW', B(26) = 55;
            case 'IMD', B(26) = 56;
            case 'IMX', B(26) = 57;
            otherwise, B(26) = NaN;
        end
        switch s(f).IMAGSRC
            case 'INEIC', B(27) = 58;
            case 'IPDE', B(27) = 59;
            case 'IISC', B(27) = 60;
            case 'IREB', B(27) = 61;
            case 'IUSGS', B(27) = 62;
            case 'IBRK', B(27) = 63;
            case 'ICALTECH', B(27) = 64;
            case 'ILLNL', B(27) = 65;
            case 'IEVLOC', B(27) = 66;
            case 'IJSOP', B(27) = 67;
            case 'IUSER', B(27) = 68;
            case 'IUNKNOWN', B(27) = 69;
            otherwise, B(27) = NaN;
        end
        B(28) = s(f).UNUSED10;
        B(29) = s(f).UNUSED11;
        B(30) = s(f).UNUSED12;
        B(31) = s(f).UNUSED13;
        B(32) = s(f).UNUSED14;
        B(33) = s(f).UNUSED15;
        B(34) = s(f).UNUSED16;
        B(35) = s(f).UNUSED17;
        B(36) = double(s(f).LEVEN);
        B(37) = double(s(f).LPSPOL);
        B(38) = double(s(f).LOVROK);
        B(39) = double(s(f).LCALDA);
        B(40) = double(s(f).UNUSED18);
        C(1) = {s(f).KSTNM};
        if length(s(f).KEVNM) > 8
            C(2) = {s(f).KEVNM(1:8)};
            C(3) = {s(f).KEVNM(9:end)};
        else
            C(2) = {s(f).KEVNM};
            C(3) = {''};
        end
        C(4) = {s(f).KHOLE};
        C(5) = {s(f).KO};
        C(6) = {s(f).KA};
        C(7) = {s(f).KT0};
        C(8) = {s(f).KT1};
        C(9) = {s(f).KT2};
        C(10) = {s(f).KT3};
        C(11) = {s(f).KT4};
        C(12) = {s(f).KT5};
        C(13) = {s(f).KT6};
        C(14) = {s(f).KT7};
        C(15) = {s(f).KT8};
        C(16) = {s(f).KT9};
        C(17) = {s(f).KF};
        C(18) = {s(f).KUSER0};
        C(19) = {s(f).KUSER1};
        C(20) = {s(f).KUSER2};
        C(21) = {s(f).KCMPNM};
        C(22) = {s(f).KNETWK};
        C(23) = {s(f).KDATRD};
        C(24) = {s(f).KINST};
        A(isnan(A)) = -12345.0;
        B(isnan(B)) = -12345;
        C(strcmp('', C)) = {'-12345'};
        C = char([{'        '}; C]);
        C = C(2:end,:)';
        if size(C,1) ~= 8
            error([' Number of characters in some of K-header-variabl', ...
                'es in file ', s(f).FILENAME,' are larger than 8 !!!']);
        end
        frewind(fid)
        count1 = fwrite(fid, A, 'float32');
        count2 = fwrite(fid, B, 'int32');
        count3 = fwrite(fid, C, 'char');
        fclose(fid);
        if count1 == 70 && count2 == 40 && count3 == 192
            status(f) = true;
        end
    elseif fid ~= -1
        fclose(fid);
    end
end

if nargout < 1
    clear status
end

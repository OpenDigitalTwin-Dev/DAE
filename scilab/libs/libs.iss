;
; Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) 2008 - DIGITEO - Allan CORNET
;
; Copyright (C) 2012 - 2016 - Scilab Enterprises
;
; This file is hereby licensed under the terms of the GNU GPL v2.0,
; pursuant to article 5.3.4 of the CeCILL v.2.1.
; This file was originally licensed under the terms of the CeCILL v2.1,
; and continues to be available under such terms.
; For more information, see the COPYING file which you should have received
; along with this program.
;
;
; f2c directory
Source: libs\f2c\f2c.h; DestDir: {app}\libs\f2c; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; intl used by localization
Source: libs\intl\*.h; DestDir: {app}\libs\intl; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; nlohmann json
Source: libs\nlohmann\include\nlohmann\*.hpp; DestDir: {app}\libs\nlohmann\include\nlohmann; Components: {#COMPN_SCILAB}

; libxml2
Source: libs\libxml2\*.h; DestDir: {app}\libs\libxml2; Flags: recursesubdirs; Components: {#COMPN_SCILAB}

; pcre
Source: libs\pcre\*.*; DestDir: {app}\libs\pcre; Flags: recursesubdirs; Components: {#COMPN_SCILAB}

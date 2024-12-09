function FComplier = getIntelInformation()
    if getenv("IFORT_COMPILER23", "") <> "" then
        FComplier = fullpath(getenv("IFORT_COMPILER23", "") + "../../..");
    elseif getenv("IFORT_COMPILER22", "") <> "" then
        FComplier = fullpath(getenv("IFORT_COMPILER22", "") + "../../..");
    end
endfunction
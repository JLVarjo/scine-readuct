#ifndef READUCT_MSVCCOMPATIBILITY_H
#define READUCT_MSVCCOMPATIBILITY_H


#if defined(_MSC_VER)
    #define SCINE_DLLEXPORT __declspec(dllexport)
#else   
    #define SCINE_DLLEXPORT
#endif


#endif // READUCT_MSVCCOMPATIBILITY_H
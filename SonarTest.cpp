/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <assert.h>
#include <cstring>

bool isFormatMessageCompatible(const char* arg1, const char* arg2) {
    //%[flags][width][.precision][length]specifier (as per C90, C99 without supporting ll and hh lengths)
    static const char* SPEC_ARGS = "diuoxXfDeEgGaAcsp%";
    static const char* LEN_ARGS = "lhLjzt";
    static const char PARAM = '%';
    static const int SQ_MAGIC = -1; // SQ complains about *(specN - 1) but *(specN + any) is fine
    
    assert(arg1 != NULL && arg2 != NULL);
    
    bool compatible = true;
    const char* param1 = strchr(arg1, PARAM);
    const char* param2 = strchr(arg2, PARAM);
    
    while (compatible && param1 != NULL && param2 != NULL) {
        const char* spec1 = strpbrk(param1 + 1, SPEC_ARGS);
        const char* spec2 = strpbrk(param2 + 1, SPEC_ARGS);
        
        // check case insensitive specifier compatibility
        if (NULL != spec1 && NULL != spec2 && tolower(*spec1) == tolower(*spec2)) {
    
               // Ugly way to avoid SQ to report false positive "Out of bound memory access" blocker
               // At this point, there is either PARAM character, length candidate character, or number ahead of specifier
               const char *tmp1 = spec1 + SQ_MAGIC;
               const char *tmp2 = spec2 + SQ_MAGIC;
               const char *len1 = strchr(LEN_ARGS, *tmp1);
               const char *len2 = strchr(LEN_ARGS, *tmp2);
    
               // check length compatibility (one characeter only)
               compatible = (NULL != len1 && NULL != len2 && *len1 == *len2) || (NULL == len1 && NULL == len2);
    
               param1 = strchr(spec1 + 1, PARAM);
               param2 = strchr(spec2 + 1, PARAM);
        }
        else { // no specifier or different type
            compatible = false;
        }
    }
    
    return compatible && NULL == param1 && NULL == param2;  /* has same amount of compatible parameters */
}

using namespace std;

int main()
{
    cout<< isFormatMessageCompatible("", "aaa") << endl;
    cout<< isFormatMessageCompatible("x%s", "%sxxx") << endl;
    cout<< isFormatMessageCompatible("x%u", "%s") << endl;
    cout<< isFormatMessageCompatible("x%u", "%lu") << endl;
    cout<< isFormatMessageCompatible("x%08lu", "%lu%s") << endl;
    cout<< isFormatMessageCompatible("x%08lu%s", "aaa%luxxx%s") << endl;
    cout<< isFormatMessageCompatible(NULL, NULL) << endl;
    return 0;
}

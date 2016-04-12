#include "sqratimport.h"
#include "sqmodule.h"
#include <string.h>
#include <stdio.h>
#include <sqstdio.h>

SQRESULT sqrat_importscript(HSQUIRRELVM v, const SQChar* moduleName) 
{
    SQChar* filename = new SQChar[scstrlen(moduleName) + 5];
	scsprintf(filename, scstrlen(moduleName) + 5, _SC("%s.nut"), moduleName);

    if(SQ_FAILED(sqstd_loadfile(v, moduleName, true))) {
        if(SQ_FAILED(sqstd_loadfile(v, filename, true))) {
            return SQ_ERROR;
        }
    }
    sq_push(v, -2);
    sq_call(v, 1, false, true);
    return SQ_OK;
}

SQRESULT sqrat_import(HSQUIRRELVM v) 
{
    const SQChar* moduleName;
    HSQOBJECT table;
    SQRESULT res = SQ_OK;

    SQInteger top = sq_gettop(v);
    sq_getstring(v, -2, &moduleName);
    sq_getstackobj(v, -1, &table);
    sq_addref(v, &table);

    sq_settop(v, 0); // Clear Stack
    sq_pushobject(v, table); // Push the target table onto the stack

    if(SQ_FAILED(sqrat_importscript(v, moduleName))) 
	{
        return SQ_ERROR;
    }

    sq_settop(v, 0); // Clean up the stack (just in case the module load leaves it messy)
    sq_pushobject(v, table); // return the target table

    return res;
}

SQInteger sqratbase_import(HSQUIRRELVM v) {
    SQInteger args = sq_gettop(v);
    switch(args) {
    case 2:
        sq_pushroottable(v);
        break;
    case 3:
        // should already have the desired table pushed onto the stack
        break;
    default:
        // Error, unexpected number of arguments
        break;
    }

    sqrat_import(v);

    return 1;
}

SQRESULT sqrat_register_importlib(HSQUIRRELVM v) {
    sq_pushroottable(v);

    sq_pushstring(v, _SC("import"), -1);
    sq_newclosure(v, &sqratbase_import, 0);
    sq_newslot(v, -3, 0);

    sq_pop(v, 1); // pop sqrat table

    return SQ_OK;
}
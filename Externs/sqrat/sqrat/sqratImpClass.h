
//
// SqratArray: Constructor Binding
//
//   usage:
//   
//   class C
//   {
//       C(char *arg) { ... }
//
//       int f() ...
//   }
//
//   To bind:
//
//   ImprovedClass<C> c_class(vm);
//   c_class.Ctor<char *>();
//   c_class.Func("f", &C::f);
//
// Copyright 2011 Alston Chen
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//  distribution.
//


#if !defined(_SCRAT_IMPLCLASS_H_)
#define _SCRAT_IMPLCLASS_H_

#include "sqratObject.h"
#include "sqratClassType.h"
#include "sqratAllocator.h"

namespace Sqrat
{
    template<class C>
    class ImprovedAllocator :public DefaultAllocator<C> 
    {
        static SQInteger setInstance(HSQUIRRELVM vm,C* instance)
        {
            sq_setinstanceup(vm, 1, instance);
            sq_setreleasehook(vm, 1, &DefaultAllocator<C>::Delete);
            return 0;
        }
    public:
        template <typename A1>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value
                ));
        }
        template <typename A1,typename A2>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value
                ));
        }
        template <typename A1,typename A2,typename A3>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4,typename A5>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value,
                Var<A5>(vm, 6).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4,typename A5,typename A6>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value,
                Var<A5>(vm, 6).value,
                Var<A6>(vm, 7).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value,
                Var<A5>(vm, 6).value,
                Var<A6>(vm, 7).value,
                Var<A7>(vm, 8).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value,
                Var<A5>(vm, 6).value,
                Var<A6>(vm, 7).value,
                Var<A7>(vm, 8).value,
                Var<A8>(vm, 9).value
                ));
        }
        template <typename A1,typename A2,typename A3,typename A4,typename A5,typename A6,typename A7,typename A8,typename A9>
        static SQInteger iNew(HSQUIRRELVM vm) {
            return setInstance(vm, new C(
                Var<A1>(vm, 2).value,
                Var<A2>(vm, 3).value,
                Var<A3>(vm, 4).value,
                Var<A4>(vm, 5).value,
                Var<A5>(vm, 6).value,
                Var<A6>(vm, 7).value,
                Var<A7>(vm, 8).value,
                Var<A8>(vm, 9).value,
                Var<A9>(vm, 10).value
                ));
        }
    };


    template<class C,  class A = ImprovedAllocator<C> >
    class ImprovedClass: public Class<C,A>
    {
    public:
        // Create a new table
        ImprovedClass(HSQUIRRELVM v = DefaultVM::Get(), bool createClass = true) : Class<C,A>(v,createClass) {}
        template<class A1>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1>,1);
        }
        template<class A1,class A2>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2>,2);
        }
        template<class A1,class A2,class A3>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3>,3);
        }
        template<class A1,class A2,class A3,class A4>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4>,4);
        }
        template<class A1,class A2,class A3,class A4,class A5>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5>,5);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6>,6);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7>,7);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7,A8>,8);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>
        ImprovedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7,A8,A9>,9);
        }
        
    private:
        ImprovedClass& BindConstructor(SQFUNCTION method,SQInteger nParams)
        {
            HSQUIRRELVM vm = Class<C,A>::vm;
            HSQOBJECT& classObj = ClassType<C>::ClassObject(vm);
            sq_pushobject(vm, classObj);
            sq_pushstring(vm,_SC("constructor"), -1);
            sq_newclosure(vm, method, 0);
            sq_setparamscheck(vm,nParams+1,NULL);
            sq_newslot(vm, -3, false);
            sq_pop(vm, 1);
            return *this;
        }
    };

	/*template<class C,  class B, class A = ImprovedAllocator<C> >
    class ImprovedDerivedClass: public DerivedClass<C,B,A>
    {
    public:
        // Create a new table
        ImprovedDerivedClass(HSQUIRRELVM v = DefaultVM::Get()) : DerivedClass<C,B,A>(v) {}

        template<class A1>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1>,1);
        }
        template<class A1,class A2>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2>,2);
        }
        template<class A1,class A2,class A3>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3>,3);
        }
        template<class A1,class A2,class A3,class A4>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4>,4);
        }
        template<class A1,class A2,class A3,class A4,class A5>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5>,5);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6>,6);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7>,7);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7,A8>,8);
        }
        template<class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9>
        ImprovedDerivedClass& Ctor() {
            return BindConstructor(A::template iNew<A1,A2,A3,A4,A5,A6,A7,A8,A9>,9);
        }
        
    private:
        ImprovedDerivedClass& BindConstructor(SQFUNCTION method,SQInteger nParams)
        {
            HSQOBJECT& classObj = ClassType<C>::ClassObject(vm);
            sq_pushobject(vm, classObj);
            sq_pushstring(vm,_SC("constructor"), -1);
            sq_newclosure(vm, method, 0);
            sq_setparamscheck(vm,nParams+1,NULL);
            sq_newslot(vm, -3, false);
            sq_pop(vm, 1);
            return *this;
        }
    };*/
}

#endif

/*
    This file is part of Corrade.

    Copyright © 2007, 2008, 2009, 2010, 2011, 2012, 2013, 2014, 2015, 2016,
                2017, 2018, 2019, 2020, 2021, 2022
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <set>
#include <sstream>
#include <vector>

#include "Corrade/Containers/GrowableArray.h"
#include "Corrade/Containers/StringStl.h"
#include "Corrade/TestSuite/Tester.h"
#include "Corrade/TestSuite/Compare/Container.h"
#include "Corrade/TestSuite/Compare/Numeric.h"
#include "Corrade/Utility/DebugStl.h"

/* No __has_feature on GCC: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60512
   Using a dedicated macro instead: https://stackoverflow.com/a/34814667 */
#ifdef __has_feature
#if __has_feature(address_sanitizer)
#define _CORRADE_CONTAINERS_SANITIZER_ENABLED
#endif
#endif
#ifdef __SANITIZE_ADDRESS__
#define _CORRADE_CONTAINERS_SANITIZER_ENABLED
#endif

#ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
/* https://github.com/llvm-mirror/compiler-rt/blob/master/include/sanitizer/common_interface_defs.h */
extern "C" int __sanitizer_verify_contiguous_container(const void *beg,
    const void *mid, const void *end);
extern "C" const void *__sanitizer_contiguous_container_find_bad_address(
    const void *beg, const void *mid, const void *end);
#define VERIFY_SANITIZED_PROPERLY(array, Allocator) \
    do {                                                                    \
        bool sanitized = __sanitizer_verify_contiguous_container(Allocator::base(array.begin()), array.end(), array.begin() + Allocator::capacity(array)); \
        if(!sanitized) {                                                    \
            Debug{} << "Sanitization annotation for array of capacity" << Allocator::capacity(array) << "and size" << array.size() << "failed at offset" << reinterpret_cast<const typename Allocator::Type*>(__sanitizer_contiguous_container_find_bad_address(Allocator::base(array.begin()), array.end(), array.begin() + Allocator::capacity(array))) - array.begin(); \
        }                                                                   \
        CORRADE_VERIFY(sanitized);                                          \
    } while(false)
#else
#define VERIFY_SANITIZED_PROPERLY(array, Allocator) do {} while(false)
#endif

namespace Corrade { namespace Containers { namespace Test { namespace {

struct GrowableArrayTest: TestSuite::Tester {
    explicit GrowableArrayTest();

    void resetCounters();

    template<class T> void reserveFromEmpty();
    template<class T> void reserveFromNonGrowable();
    template<class T> void reserveFromNonGrowableNoOp();
    template<class T> void reserveFromGrowable();
    template<class T> void reserveFromGrowableNoOp();

    template<class T> void resizeFromEmpty();
    template<class T> void resizeFromNonGrowable();
    template<class T> void resizeFromNonGrowableNoOp();
    template<class T> void resizeFromGrowable();
    template<class T> void resizeFromGrowableNoOp();
    template<class T> void resizeFromGrowableNoRealloc();

    template<class T> void resizeNoInit();
    template<class T> void resizeDefaultInit();
    template<class T> void resizeValueInit();
    void resizeDirectInit();
    void resizeCopy();

    template<class T, class Init> void resizeFromNonGrowableToLess();
    template<class T, class Init> void resizeFromGrowableToLess();

    template<class T> void appendFromEmpty();
    template<class T> void insertFromEmpty();
    template<class T> void appendFromNonGrowable();
    template<class T> void insertFromNonGrowable();
    template<class T> void appendFromGrowable();
    template<class T> void insertFromGrowable();
    template<class T> void appendFromGrowableNoRealloc();
    template<class T> void insertFromGrowableNoRealloc();
    template<class T> void insertFromGrowableNoReallocNoInit();

    /* InPlace tested in appendFrom*() already */
    void appendCopy();
    void insertCopy();
    void appendMove();
    void insertMove();
    void appendList();
    void insertList();
    void appendListEmpty();
    void insertListEmpty();
    void appendCountNoInit();
    void insertCountNoInit();
    void appendCountNoInitEmpty();
    void insertCountNoInitEmpty();

    void insertShiftOperationOrder();
    void insertShiftOperationOrderNoOp();
    void insertShiftOperationOrderNoOverlap();
    void insertInvalid();

    void appendGrowRatio();
    /* Insert uses the same growth ratio underneath, no need to test again */

    template<class T> void removeZero();
    template<class T> void removeUnorderedZero();
    template<class T> void removeSuffixZero();
    template<class T> void removeNonGrowable();
    template<class T> void removeUnorderedNonGrowable();
    template<class T> void removeSuffixNonGrowable();
    template<class T> void removeAllNonGrowable();
    template<class T> void removeUnorderedAllNonGrowable();
    template<class T> void removeSuffixAllNonGrowable();
    template<class T> void removeGrowable();
    template<class T> void removeUnorderedGrowable();
    template<class T> void removeSuffixGrowable();
    template<class T> void removeAllGrowable();
    template<class T> void removeUnorderedAllGrowable();
    template<class T> void removeSuffixAllGrowable();

    void removeShiftOperationOrder();
    void removeShiftOperationOrderNoOp();
    void removeShiftOperationOrderNoOverlap();
    void removeUnorderedShiftOperationOrder();
    void removeUnorderedShiftOperationOrderNoOp();
    void removeUnorderedShiftOperationOrderNoOverlap();
    void removeInvalid();

    template<class T> void shrinkNonGrowableEmptyNoInit();
    template<class T> void shrinkNonGrowableEmptyDefaultInit();
    template<class T> void shrinkNonGrowableNoInit();
    template<class T> void shrinkNonGrowableDefaultInit();
    template<class T> void shrinkGrowableEmptyNoInit();
    template<class T> void shrinkGrowableEmptyDefaultInit();
    template<class T> void shrinkGrowableNoInit();
    template<class T> void shrinkGrowableDefaultInit();

    template<class T> void move();

    void cast();
    void castEmpty();
    void castNonTrivial();
    void castNonGrowable();
    void castInvalid();

    void explicitAllocatorParameter();

    /* Here go the "weird unexpected corner cases" tests */

    void emplaceConstructorExplicitInCopyInitialization();
    void copyConstructPlainStruct();
    void moveConstructPlainStruct();

    template<template<class> class Allocator, std::size_t alignment> void allocationAlignment();

    void appendInsertConflictingType();

    /* Here go the benchmarks */

    void benchmarkAppendVector();
    void benchmarkAppendArray();
    void benchmarkAppendReservedVector();
    void benchmarkAppendReservedArray();

    void benchmarkAppendTrivialVector();
    template<template<class> class Allocator> void benchmarkAppendTrivialArray();
    void benchmarkAppendTrivialReservedVector();
    void benchmarkAppendTrivialReservedArray();

    void benchmarkAppendBatchTrivialVector();
    template<template<class> class Allocator> void benchmarkAppendBatchTrivialArray();
    void benchmarkAppendBatchTrivialReservedVector();
    void benchmarkAppendBatchTrivialReservedArray();

    void benchmarkAllocationsBegin();
    std::uint64_t benchmarkAllocationsEnd();

    void benchmarkAllocationsVector();
    template<template<class> class Allocator> void benchmarkAllocationsArray();
};

struct Movable {
    static int constructed;
    static int destructed;
    static int moved;
    static int assigned;

    /*implicit*/ Movable(int a = 0) noexcept: a{short(a)} { ++constructed; }
    Movable(const Movable&) = delete;
    Movable(Movable&& other) noexcept: a(other.a) {
        ++constructed;
        ++moved;
    }
    ~Movable() {
        /* Catch double frees */
        CORRADE_INTERNAL_ASSERT(!thisDestructed);
        ++destructed;
        thisDestructed = true;
    }
    Movable& operator=(const Movable&) = delete;
    Movable& operator=(Movable&& other) noexcept {
        a = other.a;
        ++assigned;
        ++moved;
        return *this;
    }

    /* "compatibility" with ints */
    Movable& operator=(int value) {
        a = value;
        return *this;
    }
    explicit operator int() const { return a; }

    short a;
    bool thisDestructed = false;
};

static_assert(sizeof(Movable) == 4, "tests require Movable to be four bytes");

int Movable::constructed = 0;
int Movable::destructed = 0;
int Movable::moved = 0;
int Movable::assigned = 0;

static_assert(!
    #ifdef CORRADE_STD_IS_TRIVIALLY_TRAITS_SUPPORTED
    std::is_trivially_constructible<Movable>::value
    #else
    Implementation::IsTriviallyConstructibleOnOldGcc<Movable>::value
    #endif
    , "Movable should be testing the non-trivial code path");
static_assert(!
    #ifdef CORRADE_STD_IS_TRIVIALLY_TRAITS_SUPPORTED
    std::is_trivially_copyable<Movable>::value
    #else
    Implementation::IsTriviallyCopyableOnOldGcc<Movable>::value
    #endif
    , "Movable should be testing the non-trivial code path");

GrowableArrayTest::GrowableArrayTest() {
    addTests({&GrowableArrayTest::reserveFromEmpty<int>,
              &GrowableArrayTest::reserveFromEmpty<Movable>,
              &GrowableArrayTest::reserveFromNonGrowable<int>,
              &GrowableArrayTest::reserveFromNonGrowable<Movable>,
              &GrowableArrayTest::reserveFromNonGrowableNoOp<int>,
              &GrowableArrayTest::reserveFromNonGrowableNoOp<Movable>,
              &GrowableArrayTest::reserveFromGrowable<int>,
              &GrowableArrayTest::reserveFromGrowable<Movable>,
              &GrowableArrayTest::reserveFromGrowableNoOp<int>,
              &GrowableArrayTest::reserveFromGrowableNoOp<Movable>,

              &GrowableArrayTest::resizeFromEmpty<int>,
              &GrowableArrayTest::resizeFromEmpty<Movable>,
              &GrowableArrayTest::resizeFromNonGrowable<int>,
              &GrowableArrayTest::resizeFromNonGrowable<Movable>,
              &GrowableArrayTest::resizeFromNonGrowableNoOp<int>,
              &GrowableArrayTest::resizeFromNonGrowableNoOp<Movable>,
              &GrowableArrayTest::resizeFromGrowable<int>,
              &GrowableArrayTest::resizeFromGrowable<Movable>,
              &GrowableArrayTest::resizeFromGrowableNoOp<int>,
              &GrowableArrayTest::resizeFromGrowableNoOp<Movable>,
              &GrowableArrayTest::resizeFromGrowableNoRealloc<int>,
              &GrowableArrayTest::resizeFromGrowableNoRealloc<Movable>,

              &GrowableArrayTest::resizeNoInit<int>,
              &GrowableArrayTest::resizeNoInit<Movable>,
              &GrowableArrayTest::resizeDefaultInit<int>,
              &GrowableArrayTest::resizeDefaultInit<Movable>,
              &GrowableArrayTest::resizeValueInit<int>,
              &GrowableArrayTest::resizeValueInit<Movable>,
              &GrowableArrayTest::resizeDirectInit,
              &GrowableArrayTest::resizeCopy,

              &GrowableArrayTest::resizeFromNonGrowableToLess<int, Corrade::NoInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<Movable, Corrade::NoInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<int, Corrade::DefaultInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<Movable, Corrade::DefaultInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<int, Corrade::ValueInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<Movable, Corrade::ValueInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<int, Corrade::DirectInitT>,
              &GrowableArrayTest::resizeFromNonGrowableToLess<Movable, Corrade::DirectInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<int, Corrade::NoInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<Movable, Corrade::NoInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<int, Corrade::DefaultInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<Movable, Corrade::DefaultInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<int, Corrade::ValueInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<Movable, Corrade::ValueInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<int, Corrade::NoInitT>,
              &GrowableArrayTest::resizeFromGrowableToLess<Movable, Corrade::NoInitT>,

              &GrowableArrayTest::appendFromEmpty<int>,
              &GrowableArrayTest::appendFromEmpty<Movable>,
              &GrowableArrayTest::insertFromEmpty<int>,
              &GrowableArrayTest::insertFromEmpty<Movable>,
              &GrowableArrayTest::appendFromNonGrowable<int>,
              &GrowableArrayTest::appendFromNonGrowable<Movable>,
              &GrowableArrayTest::insertFromNonGrowable<int>,
              &GrowableArrayTest::insertFromNonGrowable<Movable>,
              &GrowableArrayTest::appendFromGrowable<int>,
              &GrowableArrayTest::appendFromGrowable<Movable>,
              &GrowableArrayTest::insertFromGrowable<int>,
              &GrowableArrayTest::insertFromGrowable<Movable>,
              &GrowableArrayTest::appendFromGrowableNoRealloc<int>,
              &GrowableArrayTest::appendFromGrowableNoRealloc<Movable>,
              &GrowableArrayTest::insertFromGrowableNoRealloc<int>,
              &GrowableArrayTest::insertFromGrowableNoRealloc<Movable>,
              &GrowableArrayTest::insertFromGrowableNoReallocNoInit<int>,
              &GrowableArrayTest::insertFromGrowableNoReallocNoInit<Movable>,

              &GrowableArrayTest::appendCopy,
              &GrowableArrayTest::insertCopy,
              &GrowableArrayTest::appendMove,
              &GrowableArrayTest::insertMove,
              &GrowableArrayTest::appendList,
              &GrowableArrayTest::insertList,
              &GrowableArrayTest::appendListEmpty,
              &GrowableArrayTest::insertListEmpty,
              &GrowableArrayTest::appendCountNoInit,
              &GrowableArrayTest::insertCountNoInit,
              &GrowableArrayTest::appendCountNoInitEmpty,
              &GrowableArrayTest::insertCountNoInitEmpty,

              &GrowableArrayTest::insertShiftOperationOrder,
              &GrowableArrayTest::insertShiftOperationOrderNoOp,
              &GrowableArrayTest::insertShiftOperationOrderNoOverlap,
              &GrowableArrayTest::insertInvalid,

              &GrowableArrayTest::appendGrowRatio,

              &GrowableArrayTest::removeZero<int>,
              &GrowableArrayTest::removeZero<Movable>,
              &GrowableArrayTest::removeUnorderedZero<int>,
              &GrowableArrayTest::removeUnorderedZero<Movable>,
              &GrowableArrayTest::removeSuffixZero<int>,
              &GrowableArrayTest::removeSuffixZero<Movable>,
              &GrowableArrayTest::removeNonGrowable<int>,
              &GrowableArrayTest::removeNonGrowable<Movable>,
              &GrowableArrayTest::removeUnorderedNonGrowable<int>,
              &GrowableArrayTest::removeUnorderedNonGrowable<Movable>,
              &GrowableArrayTest::removeSuffixNonGrowable<int>,
              &GrowableArrayTest::removeSuffixNonGrowable<Movable>,
              &GrowableArrayTest::removeAllNonGrowable<int>,
              &GrowableArrayTest::removeAllNonGrowable<Movable>,
              &GrowableArrayTest::removeUnorderedAllNonGrowable<int>,
              &GrowableArrayTest::removeUnorderedAllNonGrowable<Movable>,
              &GrowableArrayTest::removeSuffixAllNonGrowable<int>,
              &GrowableArrayTest::removeSuffixAllNonGrowable<Movable>,
              &GrowableArrayTest::removeGrowable<int>,
              &GrowableArrayTest::removeGrowable<Movable>,
              &GrowableArrayTest::removeUnorderedGrowable<int>,
              &GrowableArrayTest::removeUnorderedGrowable<Movable>,
              &GrowableArrayTest::removeSuffixGrowable<int>,
              &GrowableArrayTest::removeSuffixGrowable<Movable>,
              &GrowableArrayTest::removeAllGrowable<int>,
              &GrowableArrayTest::removeAllGrowable<Movable>,
              &GrowableArrayTest::removeUnorderedAllGrowable<int>,
              &GrowableArrayTest::removeUnorderedAllGrowable<Movable>,
              &GrowableArrayTest::removeSuffixAllGrowable<int>,
              &GrowableArrayTest::removeSuffixAllGrowable<Movable>,

              &GrowableArrayTest::removeShiftOperationOrder,
              &GrowableArrayTest::removeShiftOperationOrderNoOp,
              &GrowableArrayTest::removeShiftOperationOrderNoOverlap,
              &GrowableArrayTest::removeUnorderedShiftOperationOrder,
              &GrowableArrayTest::removeUnorderedShiftOperationOrderNoOp,
              &GrowableArrayTest::removeUnorderedShiftOperationOrderNoOverlap,
              &GrowableArrayTest::removeInvalid,

              &GrowableArrayTest::shrinkNonGrowableEmptyNoInit<int>,
              &GrowableArrayTest::shrinkNonGrowableEmptyNoInit<Movable>,
              &GrowableArrayTest::shrinkNonGrowableEmptyDefaultInit<int>,
              &GrowableArrayTest::shrinkNonGrowableEmptyDefaultInit<Movable>,
              &GrowableArrayTest::shrinkNonGrowableNoInit<int>,
              &GrowableArrayTest::shrinkNonGrowableNoInit<Movable>,
              &GrowableArrayTest::shrinkNonGrowableDefaultInit<int>,
              &GrowableArrayTest::shrinkNonGrowableDefaultInit<Movable>,
              &GrowableArrayTest::shrinkGrowableEmptyNoInit<int>,
              &GrowableArrayTest::shrinkGrowableEmptyNoInit<Movable>,
              &GrowableArrayTest::shrinkGrowableEmptyDefaultInit<int>,
              &GrowableArrayTest::shrinkGrowableEmptyDefaultInit<Movable>,
              &GrowableArrayTest::shrinkGrowableNoInit<int>,
              &GrowableArrayTest::shrinkGrowableNoInit<Movable>,
              &GrowableArrayTest::shrinkGrowableDefaultInit<int>,
              &GrowableArrayTest::shrinkGrowableDefaultInit<Movable>,

              &GrowableArrayTest::move<int>,
              &GrowableArrayTest::move<Movable>},
        &GrowableArrayTest::resetCounters, &GrowableArrayTest::resetCounters);

    addTests({&GrowableArrayTest::cast,
              &GrowableArrayTest::castEmpty,
              &GrowableArrayTest::castNonTrivial,
              &GrowableArrayTest::castNonGrowable,
              &GrowableArrayTest::castInvalid,

              &GrowableArrayTest::explicitAllocatorParameter,

              &GrowableArrayTest::emplaceConstructorExplicitInCopyInitialization,
              &GrowableArrayTest::copyConstructPlainStruct,
              &GrowableArrayTest::moveConstructPlainStruct});

    addRepeatedTests<GrowableArrayTest>({
        &GrowableArrayTest::allocationAlignment<ArrayNewAllocator, 1>,
        &GrowableArrayTest::allocationAlignment<ArrayNewAllocator, 2>,
        &GrowableArrayTest::allocationAlignment<ArrayNewAllocator, 4>,
        &GrowableArrayTest::allocationAlignment<ArrayNewAllocator, 8>,
        &GrowableArrayTest::allocationAlignment<ArrayNewAllocator, 16>,
        &GrowableArrayTest::allocationAlignment<ArrayMallocAllocator, 1>,
        &GrowableArrayTest::allocationAlignment<ArrayMallocAllocator, 2>,
        &GrowableArrayTest::allocationAlignment<ArrayMallocAllocator, 4>,
        &GrowableArrayTest::allocationAlignment<ArrayMallocAllocator, 8>,
        &GrowableArrayTest::allocationAlignment<ArrayMallocAllocator, 16>}, 100);

    addTests({&GrowableArrayTest::appendInsertConflictingType});

    addBenchmarks({
        &GrowableArrayTest::benchmarkAppendVector,
        &GrowableArrayTest::benchmarkAppendArray,
        &GrowableArrayTest::benchmarkAppendReservedVector,
        &GrowableArrayTest::benchmarkAppendReservedArray,
        &GrowableArrayTest::benchmarkAppendTrivialVector,
        &GrowableArrayTest::benchmarkAppendTrivialArray<ArrayNewAllocator>,
        &GrowableArrayTest::benchmarkAppendTrivialArray<ArrayMallocAllocator>,
        &GrowableArrayTest::benchmarkAppendTrivialReservedVector,
        &GrowableArrayTest::benchmarkAppendTrivialReservedArray,
        &GrowableArrayTest::benchmarkAppendBatchTrivialVector,
        &GrowableArrayTest::benchmarkAppendBatchTrivialArray<ArrayNewAllocator>,
        &GrowableArrayTest::benchmarkAppendBatchTrivialArray<ArrayMallocAllocator>,
        &GrowableArrayTest::benchmarkAppendBatchTrivialReservedVector,
        &GrowableArrayTest::benchmarkAppendBatchTrivialReservedArray}, 10);

    addCustomInstancedBenchmarks({
        &GrowableArrayTest::benchmarkAllocationsVector,
        &GrowableArrayTest::benchmarkAllocationsArray<ArrayNewAllocator>,
        &GrowableArrayTest::benchmarkAllocationsArray<ArrayMallocAllocator>
    }, 1, 3,
        &GrowableArrayTest::benchmarkAllocationsBegin,
        &GrowableArrayTest::benchmarkAllocationsEnd, BenchmarkUnits::Count);

    #ifdef _CORRADE_CONTAINERS_SANITIZER_ENABLED
    Debug{} << "Address Sanitizer detected, checking container annotations";
    #endif
}

void GrowableArrayTest::resetCounters() {
    Movable::constructed = Movable::destructed = Movable::moved = Movable::assigned = 0;
}

template<class> struct TypeName;
template<> struct TypeName<int> {
    static const char* name() { return "ArrayMallocAllocator"; }
};
template<> struct TypeName<Movable> {
    static const char* name() { return "ArrayNewAllocator"; }
};

template<class T> void GrowableArrayTest::reserveFromEmpty() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        CORRADE_VERIFY(!a); /* pointer is null */
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 0);
        /* Not growable, no ASan annotation check */

        CORRADE_COMPARE(arrayReserve(a, 100), 100);
        CORRADE_VERIFY(a); /* size is 0, but pointer is non-null */
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 100);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* No construction / destruction done */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 0);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }
}

template<class T> void GrowableArrayTest::reserveFromNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        CORRADE_VERIFY(!arrayIsGrowable(a));
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        CORRADE_COMPARE(arrayReserve(a, 100), 100);
        CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 100);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* 3 times constructed initially, then 3 times moved, then all destroyed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3 + 3);
        CORRADE_COMPARE(Movable::moved, 3);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3 + 3);
    }
}

template<class T> void GrowableArrayTest::reserveFromNonGrowableNoOp() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        CORRADE_COMPARE(arrayReserve(a, 3), 3);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        /* Not growable, no ASan annotation check */
    }

    /* The reserve was a no-op, so no change */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::reserveFromGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        CORRADE_COMPARE(arrayReserve(a, 50), 50);
        T* prev = a;
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 50);
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3 + 3);
            CORRADE_COMPARE(Movable::moved, 3);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 3);
        }
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        CORRADE_COMPARE(arrayReserve(a, 100), 100);
        CORRADE_VERIFY(arrayIsGrowable(a));
        /* std::realloc() for ints might extend it in-place */
        if(std::is_same<T, Movable>::value)
            CORRADE_VERIFY(a != prev);
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 100);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* Growing an existing array twice, so 3x construction & destruction */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3 + 3 + 3);
        CORRADE_COMPARE(Movable::moved, 3 + 3);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3 + 3 + 3);
    }
}

template<class T> void GrowableArrayTest::reserveFromGrowableNoOp() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        CORRADE_COMPARE(arrayReserve(a, 100), 100);
        T* prev = a;
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 100);
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3 + 3);
            CORRADE_COMPARE(Movable::moved, 3);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 3);
        }
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        CORRADE_COMPARE(arrayReserve(a, 99), 100);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 100);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The second reserve should do nothing */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3 + 3);
        CORRADE_COMPARE(Movable::moved, 3);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3 + 3);
    }
}

template<class T> void GrowableArrayTest::resizeFromEmpty() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayResize(a, 3);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_COMPARE(int(a[0]), 0);
        CORRADE_COMPARE(int(a[1]), 0);
        CORRADE_COMPARE(int(a[2]), 0);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* Only construction (and destruction) should take place, no moves */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::resizeFromNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{2};
        a[0] = 1;
        a[1] = 2;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        arrayResize(a, 4);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 0);
        CORRADE_COMPARE(int(a[3]), 0);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* Old items move-constructed and the new ones constructed in-place */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2 + 4);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2 + 4);
    }
}

template<class T> void GrowableArrayTest::resizeFromNonGrowableNoOp() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        arrayResize(a, 3);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        /* Not growable, no ASan annotation check */
    }

    /* No change was done to the array */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::resizeFromGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    /* Should behave the same as resizeFromNonGrowable() */

    {
        Array<T> a;
        arrayResize(a, 2);
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        CORRADE_VERIFY(arrayIsGrowable(a));
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayResize(a, 4);
        /* std::realloc() for ints might extend it in-place */
        if(std::is_same<T, Movable>::value)
            CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 0);
        CORRADE_COMPARE(int(a[3]), 0);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* Old items move-constructed and the new one constructed in place */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2 + 4);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2 + 4);
    }
}

template<class T> void GrowableArrayTest::resizeFromGrowableNoOp() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayResize(a, 3);
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        CORRADE_VERIFY(arrayIsGrowable(a));
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayResize(a, 3);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* No change was done to the array */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::resizeFromGrowableNoRealloc() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 4);
        CORRADE_VERIFY(arrayIsGrowable(a));
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        T* prev = a;
        arrayResize(a, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        a[0] = 1;
        a[1] = 2;
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayResize(a, 4);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        CORRADE_COMPARE(int(a[2]), 0);
        CORRADE_COMPARE(int(a[3]), 0);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The second resize should do nothing except changing size */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::resizeNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    Array<int> a;
    arrayResize(a, Corrade::NoInit, 3);
    CORRADE_COMPARE(a.size(), 3);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    /* Welp. The contents can be kinda anything, so */
}

template<class T> void GrowableArrayTest::resizeDefaultInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    Array<T> a;
    arrayResize(a, Corrade::DefaultInit, 3);
    CORRADE_COMPARE(a.size(), 3);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

    /* Welp. The contents can be kinda anything for ints, so test just Movable */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(int(a[0]), 0);
        CORRADE_COMPARE(int(a[1]), 0);
        CORRADE_COMPARE(int(a[2]), 0);
    }
}

template<class T> void GrowableArrayTest::resizeValueInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    Array<T> a;
    arrayResize(a, Corrade::ValueInit, 3);
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(int(a[0]), 0);
    CORRADE_COMPARE(int(a[1]), 0);
    CORRADE_COMPARE(int(a[2]), 0);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
}

void GrowableArrayTest::resizeDirectInit() {
    /* This doesn't have any special handling for trivial/non-trivial types, no
       need to test twice */

    Array<int> a;
    arrayResize(a, Corrade::DirectInit, 3, 754831);
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a[0], 754831);
    CORRADE_COMPARE(a[1], 754831);
    CORRADE_COMPARE(a[2], 754831);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::resizeCopy() {
    /* This doesn't have any special handling for trivial/non-trivial types, no
       need to test twice */

    Array<int> a;
    arrayResize(a, 3, 754831);
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a[0], 754831);
    CORRADE_COMPARE(a[1], 754831);
    CORRADE_COMPARE(a[2], 754831);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

template<class> struct InitTagName;
template<> struct InitTagName<Corrade::NoInitT> {
    static const char* name() { return "NoInit"; }
};
template<> struct InitTagName<Corrade::DefaultInitT> {
    static const char* name() { return "DefaultInitT"; }
};
template<> struct InitTagName<Corrade::ValueInitT> {
    static const char* name() { return "ValueInitT"; }
};
template<> struct InitTagName<Corrade::DirectInitT> {
    static const char* name() { return "DirectInitT"; }
};

template<class T, class Init> void GrowableArrayTest::resizeFromNonGrowableToLess() {
    setTestCaseTemplateName({TypeName<T>::name(), InitTagName<Init>::name()});

    {
        Array<T> a{4};
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 4);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }
        /* Not growable, no ASan annotation check */

        arrayResize(a, Init{typename Init::Init{}}, 2);
        CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 2);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The resize move-constructed just the remaining elements */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4 + 2);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4 + 2);
    }
}

template<class T, class Init> void GrowableArrayTest::resizeFromGrowableToLess() {
    setTestCaseTemplateName({TypeName<T>::name(), InitTagName<Init>::name()});

    {
        Array<T> a;
        arrayResize(a, 4);
        T* prev = a;
        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        CORRADE_VERIFY(arrayIsGrowable(a));
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 4);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }

        arrayResize(a, Init{typename Init::Init{}}, 2);
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 1);
        CORRADE_COMPARE(int(a[1]), 2);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The resize only called half of the destructors early */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 4);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::appendFromEmpty() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        T& appended = arrayAppend(a, T{37});
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 1);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
        /** @todo expose Implementation::DefaultAllocationAlignment instead */
        #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
        CORRADE_COMPARE(arrayCapacity(a), 1);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(int(a[0]), 37);
        CORRADE_COMPARE(&appended, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The 37 is constructed as a temporary and then move-constructed into the
       new place */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 1);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::insertFromEmpty() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        T& inserted = arrayInsert(a, 0, T{37});
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 1);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
        /** @todo expose Implementation::DefaultAllocationAlignment instead */
        #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
        CORRADE_COMPARE(arrayCapacity(a), 1);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(int(a[0]), 37);
        CORRADE_COMPARE(&inserted, &a[0]);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The 37 is constructed as a temporary and then move-constructed into the
       new place */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 1);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::appendFromNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{1};
        T* prev = a;
        a[0] = 28;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 1);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }

        T& appended = arrayAppend(a, T{37});
        CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
        /** @todo expose Implementation::DefaultAllocationAlignment instead */
        #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
        CORRADE_COMPARE(arrayCapacity(a), 2);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(&appended, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first item is default-constructed in-place. Then, 37 is constructed
       as a temporary. Then, as append reallocates, 28 is move-constructed
       (third construction, first move) to new memory. Then 37 is
       move-constructed (fourth construction, second move) into the new
       place. */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::insertFromNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a;
        a[0] = 28;
        a[1] = 42;
        a[2] = 56;
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 3);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 0);
        }

        T& inserted = arrayInsert(a, 1, T{37});
        CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(int(a[2]), 42);
        CORRADE_COMPARE(int(a[3]), 56);
        CORRADE_COMPARE(&inserted, &a[1]);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first three items are default-constructed in-place. Then, 37 is
       constructed as a temporary. Then as insert reallocates, 28, 42 and 56 is
       move-constructed into new memory (fifth to seventh construction, first
       to third move), leaving a gap in between. Then 37 is move-constructed
       (eighth construction, fourth move) into the new place. */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 8);
        CORRADE_COMPARE(Movable::moved, 4);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 8);
    }
}

template<class T> void GrowableArrayTest::appendFromGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayResize(a, 1);
        T* prev = a;
        CORRADE_VERIFY(arrayIsGrowable(a));
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        a[0] = 28;
        T& appended = arrayAppend(a, T{37});
        /* std::realloc() for ints might extend it in-place */
        if(std::is_same<T, Movable>::value)
            CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(&appended, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first item is default-constructed in-place. Then, 37 is constructed
       as a temporary. Then as append reallocates, 28 is move-constructed into
       new memory (third construction, first move). Then 37 is move-constructed
       into the new place (fourth construction, second move). */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::insertFromGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayResize(a, 3);
        T* prev = a;
        CORRADE_VERIFY(arrayIsGrowable(a));
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        a[0] = 28;
        a[1] = 42;
        a[2] = 56;
        T& inserted = arrayInsert(a, 1, T{37});
        /* std::realloc() for ints might extend it in-place */
        if(std::is_same<T, Movable>::value)
            CORRADE_VERIFY(a != prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 8);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 7);
        #endif
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(int(a[2]), 42);
        CORRADE_COMPARE(int(a[3]), 56);
        CORRADE_COMPARE(&inserted, &a[1]);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first three items are default-constructed in-place. Then, 37 is
       constructed as a temporary. Then, as insert reallocates, 28, 42 and 56
       is move-constructed into new memory (fifth to seventh construction,
       first to third move), 56 move-constructed (eighth construction, fourth
       move) to a new place, 42 move-assigned (fifth move, first assignment) to
       where 56 was before, and the moved-out gap after 42 is destructed. Then,
       37 is move-constructed into the new place (9th construction, 6th move). */
    /** @todo fix to do the "widening" move directly during reallocation and
        not in a subsequent step; change destruction + move-construction to
        a move-assignment */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 9);
        CORRADE_COMPARE(Movable::moved, 6);
        CORRADE_COMPARE(Movable::assigned, 1);
        CORRADE_COMPARE(Movable::destructed, 9);
    }
}

template<class T> void GrowableArrayTest::appendFromGrowableNoRealloc() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 2);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        T* prev = a;
        arrayResize(a, 1);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        a[0] = 28;
        T& appended = arrayAppend(a, T{37});
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 2);
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(&appended, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first item is constructed in-place. Then, 37 is constructed as a
       temporary and then move-constructed into the new place. */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 1);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::insertFromGrowableNoRealloc() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 4);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        T* prev = a;
        arrayResize(a, 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        a[0] = 28;
        a[1] = 42;
        a[2] = 56;
        T& inserted = arrayInsert(a, 1, T{37});
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(int(a[2]), 42);
        CORRADE_COMPARE(int(a[3]), 56);
        CORRADE_COMPARE(&inserted, &a[1]);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first three items are constructed in-place. Then, 37 is constructed
       as a temporary. Then, 56 is move-constructed (fifth construction, first
       move) to a new place, 42 is move-assigned (second move, first
       assignment) to where 56 was before, and the moved-out gap after 42 is
       destructed. Then, 37 is move-constructed to where 42 was before (6th construction, third move). */
    /** @todo fix destruction + move-construction to a move-assignment */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 3);
        CORRADE_COMPARE(Movable::assigned, 1);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::insertFromGrowableNoReallocNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 4);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        T* prev = a;
        arrayResize(a, 3);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
        a[0] = 28;
        a[1] = 42;
        a[2] = 56;
        T& inserted = arrayInsert(a, 1, Corrade::NoInit, 1).front();
        new(&inserted) T{37};
        CORRADE_VERIFY(a == prev);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 4);
        CORRADE_COMPARE(int(a[0]), 28);
        CORRADE_COMPARE(int(a[1]), 37);
        CORRADE_COMPARE(int(a[2]), 42);
        CORRADE_COMPARE(int(a[3]), 56);
        CORRADE_COMPARE(&inserted, &a[1]);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);
    }

    /* The first three items are constructed in-place. Then, 56 is
       move-constructed (fourth construction, first move) to a new place, 42 is
       move-assigned (second move, first assignment) to where 56 was before,
       and the moved-out gap after 42 is destructed. Then, 37 is constructed
       in-place (fifth construction).

       Compared to insertFromGrowableNoRealloc(), the destruction of the gap
       after 42 has to happen so users can always perform a placement-new
       without having to worry about what was there before. */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 5);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 1);
        CORRADE_COMPARE(Movable::destructed, 5);
    }
}

void GrowableArrayTest::appendCopy() {
    Array<int> a;
    int& appended = arrayAppend(a, 2786541);
    CORRADE_COMPARE(a.size(), 1);
    #ifndef CORRADE_TARGET_32BIT
    CORRADE_COMPARE(arrayCapacity(a), 2);
    /** @todo expose Implementation::DefaultAllocationAlignment instead */
    #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
    CORRADE_COMPARE(arrayCapacity(a), 1);
    #else
    CORRADE_COMPARE(arrayCapacity(a), 3);
    #endif
    CORRADE_COMPARE(a[0], 2786541);
    CORRADE_COMPARE(&appended, &a.back());
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::insertCopy() {
    Array<int> a;
    int& inserted = arrayInsert(a, 0, 2786541);
    CORRADE_COMPARE(a.size(), 1);
    #ifndef CORRADE_TARGET_32BIT
    CORRADE_COMPARE(arrayCapacity(a), 2);
    /** @todo expose Implementation::DefaultAllocationAlignment instead */
    #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
    CORRADE_COMPARE(arrayCapacity(a), 1);
    #else
    CORRADE_COMPARE(arrayCapacity(a), 3);
    #endif
    CORRADE_COMPARE(a[0], 2786541);
    CORRADE_COMPARE(&inserted, &a.back());
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::appendMove() {
    {
        Array<Movable> a;
        Movable& appended = arrayAppend(a, Movable{25141});
        CORRADE_COMPARE(a.size(), 1);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
            /** @todo expose Implementation::DefaultAllocationAlignment instead */
        #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
        CORRADE_COMPARE(arrayCapacity(a), 1);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(a[0].a, 25141);
        CORRADE_COMPARE(&appended, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<Movable>);
    }

    CORRADE_COMPARE(Movable::constructed, 2);
    CORRADE_COMPARE(Movable::moved, 1);
    CORRADE_COMPARE(Movable::assigned, 0);
    CORRADE_COMPARE(Movable::destructed, 2);
}

void GrowableArrayTest::insertMove() {
    {
        Array<Movable> a;
        Movable& inserted = arrayInsert(a, 0, Movable{25141});
        CORRADE_COMPARE(a.size(), 1);
        #ifndef CORRADE_TARGET_32BIT
        CORRADE_COMPARE(arrayCapacity(a), 2);
        /** @todo expose Implementation::DefaultAllocationAlignment instead */
        #elif !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
        CORRADE_COMPARE(arrayCapacity(a), 1);
        #else
        CORRADE_COMPARE(arrayCapacity(a), 3);
        #endif
        CORRADE_COMPARE(a[0].a, 25141);
        CORRADE_COMPARE(&inserted, &a.back());
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<Movable>);
    }

    CORRADE_COMPARE(Movable::constructed, 2);
    CORRADE_COMPARE(Movable::moved, 1);
    CORRADE_COMPARE(Movable::assigned, 0);
    CORRADE_COMPARE(Movable::destructed, 2);
}

void GrowableArrayTest::appendList() {
    Array<int> a;
    Containers::ArrayView<int> appended = arrayAppend(a, {17, -22, 65, 2786541});
    CORRADE_COMPARE(a.size(), 4);
    CORRADE_COMPARE(arrayCapacity(a), 4); /** @todo use growing here too */
    CORRADE_COMPARE(a[0], 17);
    CORRADE_COMPARE(a[1], -22);
    CORRADE_COMPARE(a[2], 65);
    CORRADE_COMPARE(a[3], 2786541);
    CORRADE_COMPARE(appended.data(), a.data());
    CORRADE_COMPARE(appended.size(), 4);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::insertList() {
    Array<int> a;
    Containers::ArrayView<int> inserted = arrayInsert(a, 0, {17, -22, 65, 2786541});
    CORRADE_COMPARE(a.size(), 4);
    CORRADE_COMPARE(arrayCapacity(a), 4); /** @todo use growing here too */
    CORRADE_COMPARE(a[0], 17);
    CORRADE_COMPARE(a[1], -22);
    CORRADE_COMPARE(a[2], 65);
    CORRADE_COMPARE(a[3], 2786541);
    CORRADE_COMPARE(inserted.data(), a.data());
    CORRADE_COMPARE(inserted.size(), 4);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::appendListEmpty() {
    Array<int> a{3};
    int* prev = a.data();
    Containers::ArrayView<int> appended = arrayAppend(a, {});

    /* Should be a no-op, not reallocating the (non-growable) array */
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a.data(), prev);
    CORRADE_COMPARE(appended.data(), a.end());
    CORRADE_COMPARE(appended.size(), 0);
}

void GrowableArrayTest::insertListEmpty() {
    Array<int> a{3};
    int* prev = a.data();
    Containers::ArrayView<int> inserted = arrayInsert(a, 1, {});

    /* Should be a no-op, not reallocating the (non-growable) array */
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a.data(), prev);
    CORRADE_COMPARE(inserted.data(), &a[1]);
    CORRADE_COMPARE(inserted.size(), 0);
}

void GrowableArrayTest::appendCountNoInit() {
    Array<int> a;
    Containers::ArrayView<int> appended = arrayAppend(a, Corrade::NoInit, 4);
    CORRADE_COMPARE(a.size(), 4);
    CORRADE_COMPARE(arrayCapacity(a), 4); /** @todo use growing here too */
    CORRADE_COMPARE(appended.data(), a.data());
    CORRADE_COMPARE(appended.size(), 4);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::insertCountNoInit() {
    Array<int> a;
    Containers::ArrayView<int> inserted = arrayInsert(a, 0, Corrade::NoInit, 4);
    CORRADE_COMPARE(a.size(), 4);
    CORRADE_COMPARE(arrayCapacity(a), 4); /** @todo use growing here too */
    CORRADE_COMPARE(inserted.data(), a.data());
    CORRADE_COMPARE(inserted.size(), 4);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
}

void GrowableArrayTest::appendCountNoInitEmpty() {
    Array<int> a{3};
    int* prev = a.data();
    Containers::ArrayView<int> appended = arrayAppend(a, Corrade::NoInit, 0);

    /* Should be a no-op, not reallocating the (non-growable) array */
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a.data(), prev);
    CORRADE_COMPARE(appended.data(), a.end());
    CORRADE_COMPARE(appended.size(), 0);
}

void GrowableArrayTest::insertCountNoInitEmpty() {
    Array<int> a{3};
    int* prev = a.data();
    Containers::ArrayView<int> inserted = arrayInsert(a, 1, Corrade::NoInit, 0);

    /* Should be a no-op, not reallocating the (non-growable) array */
    CORRADE_COMPARE(a.size(), 3);
    CORRADE_COMPARE(a.data(), prev);
    CORRADE_COMPARE(inserted.data(), &a[1]);
    CORRADE_COMPARE(inserted.size(), 0);
}

struct VerboseMovable {
    /*implicit*/ VerboseMovable(int a = 0) noexcept: a{short(a)} {
        Utility::Debug{} << "Constructing" << a;
    }
    VerboseMovable(const VerboseMovable&) = delete;
    VerboseMovable(VerboseMovable&& other) noexcept: a(other.a) {
        Utility::Debug{} << (other.movedOut ? "Move-constructing (moved-out)" : "Move-constructing") << a << "with a" << (this - &other) << Utility::Debug::nospace << "-element offset";
        other.movedOut = true;
    }
    ~VerboseMovable() {
        Utility::Debug{} << (movedOut ? "Destructing (moved-out)" : "Destructing") << a;
    }
    VerboseMovable& operator=(const VerboseMovable&) = delete;
    VerboseMovable& operator=(VerboseMovable&& other) noexcept {
        a = other.a;
        Utility::Debug{} << (other.movedOut ? "Move-assigning (moved-out)" : "Move-assigning") << a << "with a" << (this - &other) << Utility::Debug::nospace << "-element offset";
        movedOut = other.movedOut;
        other.movedOut = true;
        return *this;
    }

    short a;
    bool movedOut = false;
};

void GrowableArrayTest::insertShiftOperationOrder() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayReserve(a, 7);
        arrayResize(a, Corrade::NoInit, 5);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};
        new(&a[3]) VerboseMovable{4};
        new(&a[4]) VerboseMovable{5};

        arrayInsert(a, 1, Corrade::NoInit, 2);
        new(&a[1]) VerboseMovable{6};
        new(&a[2]) VerboseMovable{7};
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Move-constructing 5 with a 2-element offset\n"
        "Move-constructing 4 with a 2-element offset\n"
        "Move-assigning 3 with a 2-element offset\n"
        "Move-assigning 2 with a 2-element offset\n"
        "Destructing (moved-out) 3\n"
        "Destructing (moved-out) 2\n"
        "Constructing 6\n"
        "Constructing 7\n"
        "Destructing 1\n"
        "Destructing 6\n"
        "Destructing 7\n"
        "Destructing 2\n"
        "Destructing 3\n"
        "Destructing 4\n"
        "Destructing 5\n");
}

void GrowableArrayTest::insertShiftOperationOrderNoOp() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 3);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};

        /* This does nothing, so no loop should get entered */
        arrayInsert(a, 1, Corrade::NoInit, 0);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Destructing 1\n"
        "Destructing 2\n"
        "Destructing 3\n");
}

void GrowableArrayTest::insertShiftOperationOrderNoOverlap() {
    /* Compared to insertShiftOperationOrder(), this isn't doing any move
       assignments */

    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayReserve(a, 8);
        arrayResize(a, Corrade::NoInit, 3);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};

        arrayInsert(a, 1, Corrade::NoInit, 5);
        new(&a[1]) VerboseMovable{4};
        new(&a[2]) VerboseMovable{5};
        new(&a[3]) VerboseMovable{6};
        new(&a[4]) VerboseMovable{7};
        new(&a[5]) VerboseMovable{8};
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Move-constructing 3 with a 5-element offset\n"
        "Move-constructing 2 with a 5-element offset\n"
        "Destructing (moved-out) 3\n"
        "Destructing (moved-out) 2\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Constructing 6\n"
        "Constructing 7\n"
        "Constructing 8\n"
        "Destructing 1\n"
        "Destructing 4\n"
        "Destructing 5\n"
        "Destructing 6\n"
        "Destructing 7\n"
        "Destructing 8\n"
        "Destructing 2\n"
        "Destructing 3\n");
}

void GrowableArrayTest::insertInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Array<int> a{5};

    std::ostringstream out;
    Error redirectError{&out};
    arrayInsert(a, 6, 7);
    CORRADE_COMPARE(out.str(), "Containers::arrayInsert(): can't insert at index 6 into an array of size 5\n");
}

void GrowableArrayTest::appendGrowRatio() {
    Array<int> a;

    /* On 32-bit, the growing is a bit different due to a different size of
       std::size_t */
    #ifndef CORRADE_TARGET_32BIT
    /* Double the size (minus sizeof(T)) until 64 bytes */
    arrayAppend(a, 1);
    CORRADE_COMPARE(arrayCapacity(a), 2);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, 2);
    CORRADE_COMPARE(arrayCapacity(a), 2);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 3);
    CORRADE_COMPARE(arrayCapacity(a), 6);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {4, 5, 6});
    CORRADE_COMPARE(arrayCapacity(a), 6);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 7);
    CORRADE_COMPARE(arrayCapacity(a), 14);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {8, 9, 10, 11, 12, 13, 14});
    CORRADE_COMPARE(arrayCapacity(a), 14); /* 14*4 + 8 == 64 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    /* Add 50% minus sizeof(T) after */
    arrayAppend(a, 15);
    CORRADE_COMPARE(arrayCapacity(a), 22); /* 64*1.5 = 96 = 22*4 + 8 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {16, 17, 18, 19, 20, 21, 22});
    CORRADE_COMPARE(arrayCapacity(a), 22);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 23);
    CORRADE_COMPARE(arrayCapacity(a), 34); /* 96*1.5 = 144 = 34*4 + 8 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    #else
    /* Double the size (minus sizeof(T)) until 64 bytes */
    arrayAppend(a, 1);
    /** @todo expose Implementation::DefaultAllocationAlignment instead */
    #if !defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__) || __STDCPP_DEFAULT_NEW_ALIGNMENT__ == 8 || defined(CORRADE_TARGET_EMSCRIPTEN)
    CORRADE_COMPARE(arrayCapacity(a), 1);
    #else
    CORRADE_COMPARE(arrayCapacity(a), 3);
    #endif
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {2, 3});
    CORRADE_COMPARE(arrayCapacity(a), 3);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 4);
    CORRADE_COMPARE(arrayCapacity(a), 7);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {5, 6, 7});
    CORRADE_COMPARE(arrayCapacity(a), 7);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 8);
    CORRADE_COMPARE(arrayCapacity(a), 15);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {9, 10, 11, 12, 13, 14, 15});
    CORRADE_COMPARE(arrayCapacity(a), 15); /* 15*4 + 4 == 64 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    /* Add 50% minus sizeof(T) after */
    arrayAppend(a, 16);
    CORRADE_COMPARE(arrayCapacity(a), 23); /* 64*1.5 = 96 = 23*4 + 4 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    arrayAppend(a, {17, 18, 19, 20, 21, 22, 23});
    CORRADE_COMPARE(arrayCapacity(a), 23);
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);

    arrayAppend(a, 24);
    CORRADE_COMPARE(arrayCapacity(a), 35); /* 96*1.5 = 144 = 35*4 + 4 */
    VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<int>);
    #endif
}

template<class T> void GrowableArrayTest::removeZero() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;

        /* Should do no nuthin' */
        arrayRemove(a, 3, 0);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* The three items are constructed in-place, nothing else hould be done by
       the removal */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::removeUnorderedZero() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;

        /* Should do no nuthin' */
        arrayRemoveUnordered(a, 3, 0);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* The three items are constructed in-place, nothing else hould be done by
       the removal */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::removeSuffixZero() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;

        /* Should do no nuthin' */
        arrayRemoveSuffix(a, 0);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* The three items are constructed in-place, nothing else should be done by
       the removal */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::removeNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{4};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;
        a[3] = 5786;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements */
        arrayRemove(a, 1, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 2);
        CORRADE_VERIFY(a.data() != prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The four items are constructed in-place, then the remaining two of
           them move-constructed to a new array, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 6);
            CORRADE_COMPARE(Movable::moved, 2);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 4);
        }
    }

    /* And finally also the two remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::removeUnorderedNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{4};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;
        a[3] = 5786;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements. The observable
           behavior is the same as with arrayRemove(), as the copy has to be
           done anyway and so there's no reason to differ. */
        arrayRemoveUnordered(a, 1, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 2);
        CORRADE_VERIFY(a.data() != prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The four items are constructed in-place, then the remaining two of
           them move-constructed to a new array, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 6);
            CORRADE_COMPARE(Movable::moved, 2);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 4);
        }
    }

    /* And finally also the two remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::removeSuffixNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{4};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;
        a[3] = 5786;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements */
        arrayRemoveSuffix(a, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 2);
        CORRADE_VERIFY(a.data() != prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The four items are constructed in-place, then the remaining two of
           them move-constructed to a new array, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 6);
            CORRADE_COMPARE(Movable::moved, 2);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 4);
        }
    }

    /* And finally also the two remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::removeAllNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{2};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements */
        arrayRemove(a, 0, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 0);
        CORRADE_VERIFY(a.data() != prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place, then a new empty growable
           array is constructed, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::removeUnorderedAllNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{2};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements. The observable
           behavior is again the same as with arrayRemove(), as the copy has to
           be done anyway and so there's no reason to differ. */
        arrayRemoveUnordered(a, 0, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 0);
        CORRADE_VERIFY(a.data() != prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place, then a new empty growable
           array is constructed, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::removeSuffixAllNonGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{2};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;

        /* Gets converted to growable as otherwise we can't ensure the
           destructors won't be called on removed elements */
        arrayRemoveSuffix(a, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 0);
        CORRADE_VERIFY(a.data() != prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place, then a new empty growable
           array is constructed, originals destructed */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::removeGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        arrayAppend(a, Corrade::InPlaceInit, -1);
        arrayAppend(a, Corrade::InPlaceInit, 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemove(a, 1, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The four items are constructed in-place. Then, 5786 is moved over 7
           (first assignment, first move) and -1 and the place after 5786 is
           destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 4);
            CORRADE_COMPARE(Movable::moved, 1);
            CORRADE_COMPARE(Movable::assigned, 1);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* And finally also the two remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 1);
        CORRADE_COMPARE(Movable::assigned, 1);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::removeUnorderedGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        arrayAppend(a, Corrade::InPlaceInit, -1);
        arrayAppend(a, Corrade::InPlaceInit, 15);
        arrayAppend(a, Corrade::InPlaceInit, 4);
        arrayAppend(a, Corrade::InPlaceInit, 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemoveUnordered(a, 1, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 4);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 4);
        CORRADE_COMPARE(int(a[2]), 5786);
        CORRADE_COMPARE(int(a[3]), 15);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The six items are constructed in-place. Then, 4 is moved over 7
           and 5786 over -1 (two assignments, two moves) and the places left
           after them get destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 6);
            CORRADE_COMPARE(Movable::moved, 2);
            CORRADE_COMPARE(Movable::assigned, 2);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* And finally also the four remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 2);
        CORRADE_COMPARE(Movable::assigned, 2);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::removeSuffixGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        arrayAppend(a, Corrade::InPlaceInit, -1);
        arrayAppend(a, Corrade::InPlaceInit, 5786);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemoveSuffix(a, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 2);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The four items are constructed in-place. Then, just two elements of
           those are destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 4);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* And finally also the two remaining destructed */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 4);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 4);
    }
}

template<class T> void GrowableArrayTest::removeAllGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemove(a, 0, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place. Then, all are destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::removeUnorderedAllGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemoveUnordered(a, 0, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place. Then, all are destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

template<class T> void GrowableArrayTest::removeSuffixAllGrowable() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        arrayRemoveSuffix(a, 2);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_COMPARE(arrayCapacity(a), 10);
        CORRADE_VERIFY(a.data() == prev);
        VERIFY_SANITIZED_PROPERLY(a, ArrayAllocator<T>);

        /* The two items are constructed in-place. Then, all are destructed. */
        if(std::is_same<T, Movable>::value) {
            CORRADE_COMPARE(Movable::constructed, 2);
            CORRADE_COMPARE(Movable::moved, 0);
            CORRADE_COMPARE(Movable::assigned, 0);
            CORRADE_COMPARE(Movable::destructed, 2);
        }
    }

    /* No change after the array goes out of scope */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 2);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 2);
    }
}

void GrowableArrayTest::removeShiftOperationOrder() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 6);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};
        new(&a[3]) VerboseMovable{4};
        new(&a[4]) VerboseMovable{5};
        new(&a[5]) VerboseMovable{6};

        arrayRemove(a, 1, 2);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Constructing 6\n"
        "Move-assigning 4 with a -2-element offset\n"
        "Move-assigning 5 with a -2-element offset\n"
        "Move-assigning 6 with a -2-element offset\n"
        "Destructing (moved-out) 5\n"
        "Destructing (moved-out) 6\n"
        "Destructing 1\n"
        "Destructing 4\n"
        "Destructing 5\n"
        "Destructing 6\n");
}

void GrowableArrayTest::removeShiftOperationOrderNoOp() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 3);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};

        /* This does nothing, so no loop should get entered */
        arrayRemove(a, 1, 0);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Destructing 1\n"
        "Destructing 2\n"
        "Destructing 3\n");
}

void GrowableArrayTest::removeShiftOperationOrderNoOverlap() {
    /* Compared to removeShiftOperationOrder(), this has one item that's
       directly destructed without being moved anywhere */

    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 6);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};
        new(&a[3]) VerboseMovable{4};
        new(&a[4]) VerboseMovable{5};
        new(&a[5]) VerboseMovable{6};

        arrayRemove(a, 1, 3);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Constructing 6\n"
        "Move-assigning 5 with a -3-element offset\n"
        "Move-assigning 6 with a -3-element offset\n"
        "Destructing 4\n"
        "Destructing (moved-out) 5\n"
        "Destructing (moved-out) 6\n"
        "Destructing 1\n"
        "Destructing 5\n"
        "Destructing 6\n");
}

void GrowableArrayTest::removeUnorderedShiftOperationOrder() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 6);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};
        new(&a[3]) VerboseMovable{4};
        new(&a[4]) VerboseMovable{5};
        new(&a[5]) VerboseMovable{6};

        arrayRemoveUnordered(a, 1, 2);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Constructing 6\n"
        "Move-assigning 5 with a -3-element offset\n"
        "Move-assigning 6 with a -3-element offset\n"
        "Destructing (moved-out) 5\n"
        "Destructing (moved-out) 6\n"
        "Destructing 1\n"
        "Destructing 5\n"
        "Destructing 6\n"
        "Destructing 4\n");
}

void GrowableArrayTest::removeUnorderedShiftOperationOrderNoOp() {
    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 3);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};

        /* This does nothing, so no loop should get entered */
        arrayRemoveUnordered(a, 1, 0);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Destructing 1\n"
        "Destructing 2\n"
        "Destructing 3\n");
}

void GrowableArrayTest::removeUnorderedShiftOperationOrderNoOverlap() {
    /* Compared to removeUnorderedShiftOperationOrder(), this doesn't have
       `count` items after the removed ones, so one is destructed without being
       moved anywhere. The observed behavior is the same as with
       removeShiftOperationOrderNoOverlap(). */

    std::ostringstream out;
    Debug redirectOutput{&out};
    {
        Array<VerboseMovable> a;
        arrayResize(a, Corrade::NoInit, 6);
        new(&a[0]) VerboseMovable{1};
        new(&a[1]) VerboseMovable{2};
        new(&a[2]) VerboseMovable{3};
        new(&a[3]) VerboseMovable{4};
        new(&a[4]) VerboseMovable{5};
        new(&a[5]) VerboseMovable{6};

        arrayRemoveUnordered(a, 1, 3);
    }
    CORRADE_COMPARE(out.str(),
        "Constructing 1\n"
        "Constructing 2\n"
        "Constructing 3\n"
        "Constructing 4\n"
        "Constructing 5\n"
        "Constructing 6\n"
        "Move-assigning 5 with a -3-element offset\n"
        "Move-assigning 6 with a -3-element offset\n"
        "Destructing 4\n"
        "Destructing (moved-out) 5\n"
        "Destructing (moved-out) 6\n"
        "Destructing 1\n"
        "Destructing 5\n"
        "Destructing 6\n");
}

void GrowableArrayTest::removeInvalid() {
    CORRADE_SKIP_IF_NO_DEBUG_ASSERT();

    Array<int> a{4};

    std::ostringstream out;
    Error redirectOutput{&out};
    arrayRemove(a, 4, 1);
    arrayRemove(a, 2, 3);
    arrayRemoveUnordered(a, 4, 1);
    arrayRemoveUnordered(a, 2, 3);
    arrayRemoveSuffix(a, 5);
    CORRADE_COMPARE(out.str(),
        "Containers::arrayRemove(): can't remove 1 elements at index 4 from an array of size 4\n"
        "Containers::arrayRemove(): can't remove 3 elements at index 2 from an array of size 4\n"
        "Containers::arrayRemoveUnordered(): can't remove 1 elements at index 4 from an array of size 4\n"
        "Containers::arrayRemoveUnordered(): can't remove 3 elements at index 2 from an array of size 4\n"
        "Containers::arrayRemoveSuffix(): can't remove 5 elements from an array of size 4\n");
}

template<class T> void GrowableArrayTest::shrinkNonGrowableEmptyNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;

        /* Should do no nuthin' */
        arrayShrink(a);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_VERIFY(!a.data());
    }

    /* Nothing should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 0);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }
}

template<class T> void GrowableArrayTest::shrinkNonGrowableEmptyDefaultInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;

        /* Should do no nuthin' */
        arrayShrink(a, Corrade::DefaultInit);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 0);
        CORRADE_VERIFY(!a.data());
    }

    /* Nothing should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 0);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }
}

template<class T> void GrowableArrayTest::shrinkNonGrowableNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;

        /* Should do no nuthin' */
        arrayShrink(a);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* Nothing should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::shrinkNonGrowableDefaultInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a{3};
        T* prev = a.data();
        a[0] = 2;
        a[1] = 7;
        a[2] = -1;

        /* Should do no nuthin' */
        arrayShrink(a, Corrade::DefaultInit);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_VERIFY(a.data() == prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* Nothing should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 3);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 3);
    }
}

template<class T> void GrowableArrayTest::shrinkGrowableEmptyNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayRemoveSuffix(a, 1);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_VERIFY(a.isEmpty());
        CORRADE_VERIFY(arrayCapacity(a));

        arrayShrink(a);
    }

    /* Nothing extra should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 1);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 1);
    }
}

template<class T> void GrowableArrayTest::shrinkGrowableEmptyDefaultInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayRemoveSuffix(a, 1);
        CORRADE_VERIFY(arrayIsGrowable(a));
        CORRADE_VERIFY(a.isEmpty());
        CORRADE_VERIFY(arrayCapacity(a));

        arrayShrink(a, Corrade::DefaultInit);
    }

    /* Nothing extra should be done by the shrink */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 1);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 1);
    }
}

template<class T> void GrowableArrayTest::shrinkGrowableNoInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        arrayAppend(a, Corrade::InPlaceInit, -1);

        /* Should convert to non-growable */
        arrayShrink(a);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_VERIFY(a.data() != prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* Shrink moves everything to a new array */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 3);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::shrinkGrowableDefaultInit() {
    setTestCaseTemplateName(TypeName<T>::name());

    {
        Array<T> a;
        arrayReserve(a, 10);
        T* prev = a.data();
        arrayAppend(a, Corrade::InPlaceInit, 2);
        arrayAppend(a, Corrade::InPlaceInit, 7);
        arrayAppend(a, Corrade::InPlaceInit, -1);

        /* Should convert to non-growable */
        arrayShrink(a, Corrade::DefaultInit);
        CORRADE_VERIFY(!arrayIsGrowable(a));
        CORRADE_COMPARE(a.size(), 3);
        CORRADE_COMPARE(arrayCapacity(a), 3);
        CORRADE_VERIFY(a.data() != prev);
        CORRADE_COMPARE(int(a[0]), 2);
        CORRADE_COMPARE(int(a[1]), 7);
        CORRADE_COMPARE(int(a[2]), -1);
        /* Not growable, no ASan annotation check */
    }

    /* Compared to shrinkGrowableDefaultInit(), instead of constructing
       in-place we default-construct and then assign, so three more assignments
       in addition */
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 6);
        CORRADE_COMPARE(Movable::moved, 3);
        CORRADE_COMPARE(Movable::assigned, 3);
        CORRADE_COMPARE(Movable::destructed, 6);
    }
}

template<class T> void GrowableArrayTest::move() {
    setTestCaseTemplateName(TypeName<T>::name());

    Array<T> a;
    arrayResize(a, 10);
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 10);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }

    Array<T> b = Utility::move(a);
    CORRADE_VERIFY(arrayIsGrowable(b));
    CORRADE_VERIFY(!arrayIsGrowable(a));
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 10);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }

    Array<T> c{10};
    c = Utility::move(b);
    CORRADE_VERIFY(arrayIsGrowable(c));
    CORRADE_VERIFY(!arrayIsGrowable(b));
    if(std::is_same<T, Movable>::value) {
        CORRADE_COMPARE(Movable::constructed, 20);
        CORRADE_COMPARE(Movable::moved, 0);
        CORRADE_COMPARE(Movable::assigned, 0);
        CORRADE_COMPARE(Movable::destructed, 0);
    }
}

void GrowableArrayTest::cast() {
    Array<char> a;
    arrayResize(a, 10);

    auto b = arrayAllocatorCast<std::uint16_t>(Utility::move(a));
    CORRADE_COMPARE(b.size(), 5);
    CORRADE_COMPARE(a.data(), nullptr);
}

void GrowableArrayTest::castEmpty() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Array<char> a;

    std::ostringstream out;
    Error redirectError{&out};
    arrayAllocatorCast<std::uint16_t>(Utility::move(a));
    CORRADE_COMPARE(out.str(),
        "Containers::arrayAllocatorCast(): the array has to use the ArrayMallocAllocator or a derivative\n");
}

void GrowableArrayTest::castNonTrivial() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Array<char> a;
    arrayResize<char, ArrayNewAllocator<char>>(a, 10);

    std::ostringstream out;
    Error redirectError{&out};
    arrayAllocatorCast<std::uint16_t>(Utility::move(a));
    CORRADE_COMPARE(out.str(),
        "Containers::arrayAllocatorCast(): the array has to use the ArrayMallocAllocator or a derivative\n");
}

void GrowableArrayTest::castNonGrowable() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Array<char> a{10};

    std::ostringstream out;
    Error redirectError{&out};
    arrayAllocatorCast<std::uint16_t>(Utility::move(a));
    CORRADE_COMPARE(out.str(),
        "Containers::arrayAllocatorCast(): the array has to use the ArrayMallocAllocator or a derivative\n");
}

void GrowableArrayTest::castInvalid() {
    CORRADE_SKIP_IF_NO_ASSERT();

    Array<char> a;
    arrayResize(a, 10);

    std::ostringstream out;
    Error redirectError{&out};
    arrayAllocatorCast<std::uint32_t>(Utility::move(a));
    CORRADE_COMPARE(out.str(),
        "Containers::arrayAllocatorCast(): can't reinterpret 10 1-byte items into a 4-byte type\n");
}

void GrowableArrayTest::explicitAllocatorParameter() {
    Array<int> a;
    arrayReserve<ArrayNewAllocator>(a, 10);
    /* Calling it again should be a no-op if it correctly checks for capacity */
    arrayReserve<ArrayNewAllocator>(a, 8);
    CORRADE_VERIFY(!arrayIsGrowable(a));
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
    CORRADE_COMPARE(arrayCapacity<ArrayNewAllocator>(a), 10);

    arrayResize<ArrayNewAllocator>(a, Corrade::DefaultInit, 1);
    arrayResize<ArrayNewAllocator>(a, Corrade::ValueInit, 2);
    arrayResize<ArrayNewAllocator>(a, 3);
    arrayResize<ArrayNewAllocator>(a, Corrade::NoInit, 4);
    arrayResize<ArrayNewAllocator>(a, Corrade::DirectInit, 5, 6);
    arrayResize<ArrayNewAllocator>(a, 5, 6);
    CORRADE_VERIFY(!arrayIsGrowable(a));
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
    CORRADE_COMPARE(a.size(), 5);

    const int six = 6;
    {
        int& value = arrayAppend<ArrayNewAllocator>(a, six);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(value, 6);
    } {
        int& value = arrayInsert<ArrayNewAllocator>(a, 0, six);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(value, 6);
    } {
        int& value = arrayAppend<ArrayNewAllocator>(a, Corrade::InPlaceInit, 7);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(value, 7);
    } {
        int& value = arrayInsert<ArrayNewAllocator>(a, 0, Corrade::InPlaceInit, 7);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(value, 7);
    } {
        Containers::ArrayView<int> view = arrayAppend<ArrayNewAllocator>(a, {8, 9, 10});
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 3);
        CORRADE_COMPARE(view[2], 10);
    } {
        Containers::ArrayView<int> view = arrayInsert<ArrayNewAllocator>(a, 0, {8, 9, 10});
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 3);
        CORRADE_COMPARE(view[2], 10);
    } {
        const int values[]{11, 12, 13};
        Containers::ArrayView<int> view = arrayAppend<ArrayNewAllocator>(a, arrayView(values));
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 3);
        CORRADE_COMPARE(view[1], 12);
    } {
        const int values[]{11, 12, 13};
        Containers::ArrayView<int> view = arrayInsert<ArrayNewAllocator>(a, 0, arrayView(values));
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 3);
        CORRADE_COMPARE(view[1], 12);
    } {
        Containers::ArrayView<int> view = arrayAppend<ArrayNewAllocator>(a, Corrade::NoInit, 2);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 2);
        view[0] = 14;
        view[1] = 15;
        CORRADE_COMPARE(a[21], 14);
    } {
        Containers::ArrayView<int> view = arrayInsert<ArrayNewAllocator>(a, 0, Corrade::NoInit, 2);
        CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
        CORRADE_COMPARE(view.size(), 2);
        view[0] = 14;
        view[1] = 15;
        CORRADE_COMPARE(a[23], 14);
    }
    CORRADE_COMPARE(a.size(), 25);

    arrayRemove<ArrayNewAllocator>(a, 15);
    arrayRemoveUnordered<ArrayNewAllocator>(a, 15);
    arrayRemoveSuffix<ArrayNewAllocator>(a);
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(a));
    /* After this it will finally lose the growable status */
    arrayShrink<ArrayNewAllocator>(a);
    CORRADE_VERIFY(!arrayIsGrowable<ArrayNewAllocator>(a));
    CORRADE_VERIFY(!a.deleter());
    CORRADE_COMPARE(a.size(), 22);

    /** @todo use a different allocator here once it exists -- this one would
        be picked up implicitly as well so it doesn't really test anything */
    Array<Movable> b;
    arrayResize<ArrayNewAllocator>(b, Corrade::DirectInit, 5, Movable{6});
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(b));

    arrayAppend<ArrayNewAllocator>(b, Movable{1});
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(b));

    arrayInsert<ArrayNewAllocator>(b, 0, Movable{1});
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(b));

    arrayAppend<ArrayNewAllocator>(b, Corrade::InPlaceInit, 2);
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(b));

    arrayInsert<ArrayNewAllocator>(b, 0, Corrade::InPlaceInit, 2);
    CORRADE_VERIFY(arrayIsGrowable<ArrayNewAllocator>(b));
    CORRADE_COMPARE(b.size(), 9);
}

void GrowableArrayTest::emplaceConstructorExplicitInCopyInitialization() {
    /* See constructHelpers.h for details about this compiler-specific issue */
    struct ExplicitDefault {
        explicit ExplicitDefault() = default;
    };

    struct ContainingExplicitDefaultWithImplicitConstructor {
        ExplicitDefault a;
    };

    /* This alone works */
    ContainingExplicitDefaultWithImplicitConstructor a;
    static_cast<void>(a);

    /* So this should too */
    Containers::Array<ContainingExplicitDefaultWithImplicitConstructor> b;
    arrayResize(b, Corrade::DirectInit, 1);
    arrayAppend(b, Corrade::InPlaceInit);
    arrayInsert(b, 0, Corrade::InPlaceInit);
    CORRADE_COMPARE(b.size(), 3);
}

void GrowableArrayTest::copyConstructPlainStruct() {
    struct ExtremelyTrivial {
        int a;
        char b;
    };

    Array<ExtremelyTrivial> a;

    /* This needs special handling on GCC 4.8, where T{b} (copy-construction)
       attempts to convert ExtremelyTrivial to int to initialize the first
       argument and fails miserably. */
    ExtremelyTrivial value;
    arrayAppend(a, value);
    arrayInsert(a, 0, value);

    /* This copy-constructs the new values */
    arrayResize(a, Corrade::DirectInit, 10, ExtremelyTrivial{4, 'b'});

    /* And this also */
    const ExtremelyTrivial data[2]{
        {5, 'c'},
        {6, 'd'}
    };
    arrayAppend(a, arrayView(data));
    arrayInsert(a, 0, arrayView(data));

    CORRADE_COMPARE(a.size(), 14);
}

void GrowableArrayTest::moveConstructPlainStruct() {
    struct MoveOnlyStruct {
        int a;
        char c;
        Array<int> b;
    };

    Array<MoveOnlyStruct> a;

    /* This needs special handling on GCC 4.8, where T{std::move(b)} attempts
       to convert MoveOnlyStruct to int to initialize the first argument and
       fails miserably. */
    arrayAppend(a, Corrade::InPlaceInit, 3, 'a', nullptr);
    arrayAppend(a, Corrade::InPlaceInit, 4, 'b', nullptr);
    arrayAppend(a, Corrade::InPlaceInit, 5, 'c', nullptr);

    /* This is another case where move constructors get called */
    arrayResize(a, 15);

    /* Here a move constructor gets called indirectly as the args are forwarded
       to the InPlaceInit version. In this case there's a workaround for
       the emplaceConstructorExplicitInCopyInitialization() case from above so
       we're just reusing that to mix in the 4.8-specific variant also */
    arrayAppend(a, MoveOnlyStruct{5, 'c', nullptr});

    /* Here a move constructor gets called on the last element to shift it
       forward and then a move constructor gets called to place the new
       element */
    arrayInsert(a, 16, MoveOnlyStruct{6, 'd', nullptr});

    CORRADE_COMPARE(a.size(), 17);
}

template<template<class> class> struct AllocatorName;
template<> struct AllocatorName<ArrayNewAllocator> {
    static const char* name() { return "ArrayNewAllocator"; }
};
template<> struct AllocatorName<ArrayMallocAllocator> {
    static const char* name() { return "ArrayMallocAllocator"; }
};

template<std::size_t alignment> struct alignas(alignment) Aligned {
    char foo;
};

template<template<class> class Allocator, std::size_t alignment> void GrowableArrayTest::allocationAlignment() {
    setTestCaseTemplateName({AllocatorName<Allocator>::name(), std::to_string(alignment)});

    if(alignment > Implementation::DefaultAllocationAlignment)
        CORRADE_SKIP(alignment << Debug::nospace << "-byte alignment is larger than platform default allocation alignment, skipping");

    /* Should always fit std::size_t, at least */
    CORRADE_COMPARE_AS(Allocator<Aligned<alignment>>::AllocationOffset, sizeof(std::size_t),
        TestSuite::Compare::GreaterOrEqual);

    /* Offset should be aligned for the type */
    CORRADE_COMPARE_AS(Allocator<Aligned<alignment>>::AllocationOffset, alignment,
        TestSuite::Compare::Divisible);

    /* We're not stupid with the assumptions, hopefully */
    CORRADE_COMPARE(sizeof(Aligned<alignment>), alignof(Aligned<alignment>));

    /* All (re)allocations should be aligned */
    Array<Aligned<alignment>> a;
    for(std::size_t i = 0; i != 100; ++i) {
        CORRADE_ITERATION(i);
        arrayAppend<Allocator>(a, Corrade::InPlaceInit, 'a');
        CORRADE_COMPARE_AS(reinterpret_cast<std::uintptr_t>(a.data()), alignment,
            TestSuite::Compare::Divisible);
    }
}

void GrowableArrayTest::appendInsertConflictingType() {
    Array<unsigned> a;
    Array<unsigned> b{Corrade::InPlaceInit, {3}};

    /* If the second argument is just const T& or T&&, these would fail to
       compile due to a conflict in template type resolution (int vs unsigned).
       Has to be std::common_type<T>::type, which avoids the conflict and
       instead accepts anything implicitly convertible to T. */
    const int value = 5;
    arrayAppend(a, value);
    arrayAppend(a, 5);
    arrayAppend(a, b);
    arrayAppend<ArrayAllocator>(a, value);
    arrayAppend<ArrayAllocator>(a, 5);
    arrayAppend<ArrayAllocator>(a, b);
    arrayInsert(a, 0, value);
    arrayInsert(a, 0, 5);
    arrayInsert(a, 0, b);
    arrayInsert<ArrayAllocator>(a, 0, value);
    arrayInsert<ArrayAllocator>(a, 0, 5);
    arrayInsert<ArrayAllocator>(a, 0, b);
    CORRADE_COMPARE_AS(a,
        Containers::arrayView<unsigned>({3, 5, 5, 3, 5, 5, 5, 5, 3, 5, 5, 3}),
        TestSuite::Compare::Container);
}

void GrowableArrayTest::benchmarkAppendVector() {
    std::vector<Movable> vector;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            vector.emplace_back(i);
    }

    CORRADE_COMPARE(vector.size(), 1000000);
}

void GrowableArrayTest::benchmarkAppendArray() {
    Array<Movable> array;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            arrayAppend(array, Corrade::InPlaceInit, int(i));
    }

    CORRADE_COMPARE(array.size(), 1000000);
}

void GrowableArrayTest::benchmarkAppendReservedVector() {
    std::vector<Movable> vector;
    vector.reserve(1000000);
    Movable* data = vector.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            vector.emplace_back(i);
    }

    CORRADE_COMPARE(vector.size(), 1000000);
    CORRADE_COMPARE(vector.data(), data);
}

void GrowableArrayTest::benchmarkAppendReservedArray() {
    Array<Movable> array;
    arrayReserve(array, 1000000);
    Movable* data = array.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            arrayAppend(array, Corrade::InPlaceInit, int(i));
    }

    CORRADE_COMPARE(array.size(), 1000000);
    CORRADE_COMPARE(array.data(), data);
}

void GrowableArrayTest::benchmarkAppendTrivialVector() {
    std::vector<int> vector;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            vector.push_back(i);
    }

    CORRADE_COMPARE(vector.size(), 1000000);
}

template<template<class> class Allocator> void GrowableArrayTest::benchmarkAppendTrivialArray() {
    setTestCaseTemplateName(AllocatorName<Allocator>::name());

    Array<int> array;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            arrayAppend<int, Allocator<int>>(array, int(i));
    }

    CORRADE_COMPARE(array.size(), 1000000);
}

void GrowableArrayTest::benchmarkAppendTrivialReservedVector() {
    std::vector<int> vector;
    vector.reserve(1000000);
    int* data = vector.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            vector.push_back(i);
    }

    CORRADE_COMPARE(vector.size(), 1000000);
    CORRADE_COMPARE(vector.data(), data);
}

void GrowableArrayTest::benchmarkAppendTrivialReservedArray() {
    Array<int> array;
    arrayReserve(array, 1000000);
    int* data = array.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i)
            arrayAppend(array, int(i));
    }

    CORRADE_COMPARE(array.size(), 1000000);
    CORRADE_COMPARE(array.data(), data);
}

void GrowableArrayTest::benchmarkAppendBatchTrivialVector() {
    std::vector<int> vector;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; i += 10)
            vector.insert(vector.end(), {
                int(i),
                int(i) + 1,
                int(i) + 2,
                int(i) + 3,
                int(i) + 4,
                int(i) + 5,
                int(i) + 6,
                int(i) + 7,
                int(i) + 8,
                int(i) + 9});
    }

    CORRADE_COMPARE(vector.size(), 1000000);
}

template<template<class> class Allocator> void GrowableArrayTest::benchmarkAppendBatchTrivialArray() {
    setTestCaseTemplateName(AllocatorName<Allocator>::name());

    Array<int> array;
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; i += 10)
            arrayAppend<int, Allocator<int>>(array, {
                int(i),
                int(i) + 1,
                int(i) + 2,
                int(i) + 3,
                int(i) + 4,
                int(i) + 5,
                int(i) + 6,
                int(i) + 7,
                int(i) + 8,
                int(i) + 9});
    }

    CORRADE_COMPARE(array.size(), 1000000);
}

void GrowableArrayTest::benchmarkAppendBatchTrivialReservedVector() {
    std::vector<int> vector;
    vector.reserve(1000000);
    int* data = vector.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; i += 10)
            vector.insert(vector.end(), {
                int(i),
                int(i) + 1,
                int(i) + 2,
                int(i) + 3,
                int(i) + 4,
                int(i) + 5,
                int(i) + 6,
                int(i) + 7,
                int(i) + 8,
                int(i) + 9});
    }

    CORRADE_COMPARE(vector.size(), 1000000);
    CORRADE_COMPARE(vector.data(), data);
}

void GrowableArrayTest::benchmarkAppendBatchTrivialReservedArray() {
    Array<int> array;
    arrayReserve(array, 1000000);
    int* data = array.data();
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; i += 10)
            arrayAppend(array, {
                int(i),
                int(i) + 1,
                int(i) + 2,
                int(i) + 3,
                int(i) + 4,
                int(i) + 5,
                int(i) + 6,
                int(i) + 7,
                int(i) + 8,
                int(i) + 9});
    }

    CORRADE_COMPARE(array.size(), 1000000);
    CORRADE_COMPARE(array.data(), data);
}

const char* AllocationBenchmarkName[] {
    "allocations",
    "allocation reuse",
    "reallocations"
};

std::size_t allocationCount, allocationReuseCount, reallocationCount;

void GrowableArrayTest::benchmarkAllocationsBegin() {
    allocationCount = 0;
    allocationReuseCount = 0;
    reallocationCount = 0;
}

std::uint64_t GrowableArrayTest::benchmarkAllocationsEnd() {
    if(testCaseInstanceId() == 0)
        return allocationCount;
    else if(testCaseInstanceId() == 1)
        return allocationReuseCount;
    else if(testCaseInstanceId() == 2)
        return reallocationCount;
    else CORRADE_INTERNAL_ASSERT_UNREACHABLE();
}

void GrowableArrayTest::benchmarkAllocationsVector() {
    setTestCaseDescription(AllocationBenchmarkName[testCaseInstanceId()]);

    std::vector<int> vector;
    int *prevData = nullptr;
    std::size_t prevCapacity = 0;
    std::set<int*> used;
    Debug capacities{testCaseInstanceId() ? nullptr : Debug::output()};
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i) {
            vector.push_back(i);
            if(vector.data() != prevData) {
                ++allocationCount;
                capacities << vector.capacity();
                if(used.count(vector.data())) {
                    ++allocationReuseCount;
                    capacities << Debug::nospace << "@";
                }
            } else if(vector.capacity() != prevCapacity) {
                ++reallocationCount;
                capacities << vector.capacity() << Debug::nospace << "!";
            }
            prevData = vector.data();
            prevCapacity = vector.capacity();
            used.insert(vector.data());
        }
    }

    CORRADE_COMPARE(vector.size(), 1000000);
}

template<template<class> class Allocator> void GrowableArrayTest::benchmarkAllocationsArray() {
    setTestCaseTemplateName(AllocatorName<Allocator>::name());
    setTestCaseDescription(AllocationBenchmarkName[testCaseInstanceId()]);

    Array<int> array;
    int *prevData = nullptr;
    std::size_t prevCapacity = 0;
    std::set<int*> used;
    Debug capacities{testCaseInstanceId() ? nullptr : Debug::output()};
    CORRADE_BENCHMARK(1) {
        for(std::size_t i = 0; i != 1000000; ++i) {
            arrayAppend<int, Allocator<int>>(array, int(i));
            std::size_t capacity = arrayCapacity<int, Allocator<int>>(array);
            if(array.data() != prevData) {
                ++allocationCount;
                capacities << capacity;
                if(used.count(array.data())) {
                    ++allocationReuseCount;
                    capacities << Debug::nospace << "@";
                }
            } else if(capacity != prevCapacity) {
                ++reallocationCount;
                capacities << capacity << Debug::nospace << "!";
            }
            prevData = array.data();
            prevCapacity = capacity;
            used.insert(array.data());
        }
    }

    CORRADE_COMPARE(array.size(), 1000000);
}

}}}}

CORRADE_TEST_MAIN(Corrade::Containers::Test::GrowableArrayTest)

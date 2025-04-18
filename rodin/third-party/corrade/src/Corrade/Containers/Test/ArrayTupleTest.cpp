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

/* Included as first to check that we *really* don't need the
   [Strided]ArrayView headers for anything. We actually need, but the relevant
   functions are forward-declared so it should work. */
#include "Corrade/Containers/ArrayTuple.h"

#include <sstream>

#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/BitArrayView.h"
#include "Corrade/Containers/StridedArrayView.h"
#include "Corrade/Containers/StridedBitArrayView.h"
#include "Corrade/Containers/String.h"
#include "Corrade/TestSuite/Tester.h"
#include "Corrade/TestSuite/Compare/Container.h"
#include "Corrade/TestSuite/Compare/Numeric.h"
#include "Corrade/Utility/DebugStl.h"
#include "Corrade/Utility/Format.h"

namespace Corrade { namespace Containers { namespace Test { namespace {

struct ArrayTupleTest: TestSuite::Tester {
    explicit ArrayTupleTest();

    void constructEmpty();
    void constructEmptyArrays();
    void construct();
    void constructNoInit();

    void constructCustomAllocatorDefaultDeleter();
    void constructStatelessDeleter();
    void constructStatefulAlignedNonTriviallyDestructibleDeleter();

    void constructTriviallyDestructible();
    void constructTriviallyDestructibleCustomAllocatorDefaultDeleter();
    void constructTriviallyDestructibleStatelessDeleter();
    void constructTriviallyDestructibleStatefulAlignedNonTriviallyDestructibleDeleter();

    void constructTriviallyConstructibleNonTriviallyDestructible();

    void constructStringInvalidFlags();

    void constructCopy();
    void constructMove();

    void constructBig();

    void allocatorAlignmentEmpty();
    template<int a> void allocatorAlignmentFromItems();
    template<int a> void allocatorAlignmentFromDeleter();

    void convertArray();
    void convertArrayInvalid();

    void release();

    void emplaceConstructItemExplicitInCopyInitialization();
    void copyConstructPlainDeleterStruct();
};

ArrayTupleTest::ArrayTupleTest() {
    addTests({&ArrayTupleTest::constructEmpty,
              &ArrayTupleTest::constructEmptyArrays,
              &ArrayTupleTest::construct,
              &ArrayTupleTest::constructNoInit,

              &ArrayTupleTest::constructCustomAllocatorDefaultDeleter,
              &ArrayTupleTest::constructStatelessDeleter,
              &ArrayTupleTest::constructStatefulAlignedNonTriviallyDestructibleDeleter,

              &ArrayTupleTest::constructTriviallyDestructible,
              &ArrayTupleTest::constructTriviallyDestructibleCustomAllocatorDefaultDeleter,
              &ArrayTupleTest::constructTriviallyDestructibleStatelessDeleter,
              &ArrayTupleTest::constructTriviallyDestructibleStatefulAlignedNonTriviallyDestructibleDeleter,

              &ArrayTupleTest::constructTriviallyConstructibleNonTriviallyDestructible,

              &ArrayTupleTest::constructStringInvalidFlags,

              &ArrayTupleTest::constructCopy,
              &ArrayTupleTest::constructMove,

              &ArrayTupleTest::constructBig,

              &ArrayTupleTest::allocatorAlignmentEmpty,
              &ArrayTupleTest::allocatorAlignmentFromItems<1>,
              &ArrayTupleTest::allocatorAlignmentFromItems<16>,
              &ArrayTupleTest::allocatorAlignmentFromDeleter<1>,
              &ArrayTupleTest::allocatorAlignmentFromDeleter<16>,

              &ArrayTupleTest::convertArray,
              &ArrayTupleTest::convertArrayInvalid,

              &ArrayTupleTest::release,

              &ArrayTupleTest::emplaceConstructItemExplicitInCopyInitialization,
              &ArrayTupleTest::copyConstructPlainDeleterStruct});
}

void ArrayTupleTest::constructEmpty() {
    /* Const in order to verify const access */
    const ArrayTuple data;
    CORRADE_COMPARE(data.size(), 0);
    CORRADE_VERIFY(!data.data());
    CORRADE_VERIFY(!data.deleter());
}

struct NonCopyable {
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;
    NonCopyable() {
        ++constructed;
        thisPointerSuffix = reinterpret_cast<std::uintptr_t>(this) & 0xff;
    }

    ~NonCopyable() {
        /* Just a check that the deleter is really called on a correct address.
           If it's not, the memory will contain something different and this
           won't match. */
        if(thisPointerSuffix == (reinterpret_cast<std::uintptr_t>(this) & 0xff))
            ++destructed;
    }

    unsigned char thisPointerSuffix;

    static int constructed;
    static int destructed;
};

int NonCopyable::constructed = 0;
int NonCopyable::destructed = 0;

void ArrayTupleTest::constructEmptyArrays() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;

    {
        ArrayView<char> chars{reinterpret_cast<char*>(1337), 3};
        ArrayView<NonCopyable> noncopyable{reinterpret_cast<NonCopyable*>(1337), 3};
        ArrayView<int> ints{reinterpret_cast<int*>(1337), 3};
        StridedArrayView1D<double> strided{ArrayView<double>{reinterpret_cast<double*>(1337), 3}};
        StridedArrayView3D<float> strided3D{ArrayView<float>{reinterpret_cast<float*>(1337), 24}, {2, 3, 4}};
        StridedArrayView2D<char> stridedErased{ArrayView<char>{reinterpret_cast<char*>(1337), 3}, {1, 3}};
        MutableBitArrayView bits{reinterpret_cast<char*>(1337), 7, 53};
        MutableStridedBitArrayView1D bitsStrided{MutableBitArrayView{reinterpret_cast<char*>(1337), 7, 53}};
        MutableStridedBitArrayView3D bitsStrided3D{MutableBitArrayView{reinterpret_cast<char*>(1337), 7, 53}, {2, 7, 3}};
        MutableStringView string{reinterpret_cast<char*>(1337), 3, StringViewFlag::Global};
        ArrayTuple data{
            {0, chars},
            {0, noncopyable},
            {0, ints},
            {0, strided},
            {{0, 0, 0}, strided3D},
            {Corrade::NoInit, 0, 4, 2, stridedErased},
            {0, bits},
            {0, bitsStrided},
            {{0, 0, 0}, bitsStrided3D},
            {0, string},
        };

        CORRADE_COMPARE(data.size(), 0);
        CORRADE_VERIFY(!data.data());
        CORRADE_VERIFY(!data.deleter());

        CORRADE_COMPARE(NonCopyable::constructed, 0);
        CORRADE_COMPARE(NonCopyable::destructed, 0);

        /* Even though this is basically a no-op, all views should be reset to
           empty nullptr ones */
        CORRADE_COMPARE(chars.size(), 0);
        CORRADE_VERIFY(!chars.data());
        CORRADE_COMPARE(noncopyable.size(), 0);
        CORRADE_VERIFY(!noncopyable.data());
        CORRADE_COMPARE(ints.size(), 0);
        CORRADE_VERIFY(!ints.data());

        /* Last dimension stride is always set for strided views */
        CORRADE_COMPARE(strided.size(), 0);
        CORRADE_COMPARE(strided.stride(), 8);
        CORRADE_VERIFY(!strided.data());
        CORRADE_COMPARE(strided3D.size(), (Size3D{0, 0, 0}));
        CORRADE_COMPARE(strided3D.stride(),  (Stride3D{0, 0, 4}));
        CORRADE_VERIFY(!strided3D.data());

        /* Second dimension size and stride is always set for erased views */
        CORRADE_COMPARE(stridedErased.size(), (Size2D{0, 4}));
        CORRADE_COMPARE(stridedErased.stride(),  (Stride2D{4, 1}));
        CORRADE_VERIFY(!stridedErased.data());

        /* Bit array pointer and both size and offset should be reset */
        CORRADE_COMPARE(bits.offset(), 0);
        CORRADE_COMPARE(bits.size(), 0);
        CORRADE_VERIFY(!bits.data());

        /* Bit array pointer and both size and offset should be reset, last
           dimension stride is always set for strided bit views */
        CORRADE_COMPARE(bitsStrided.offset(), 0);
        CORRADE_COMPARE(bitsStrided.size(), 0);
        CORRADE_COMPARE(bitsStrided.stride(), 1);
        CORRADE_VERIFY(!bitsStrided.data());
        CORRADE_COMPARE(bitsStrided3D.offset(), 0);
        CORRADE_COMPARE(bitsStrided3D.size(), (Size3D{0, 0, 0}));
        CORRADE_COMPARE(bitsStrided3D.stride(),  (Stride3D{0, 0, 1}));
        CORRADE_VERIFY(!bitsStrided3D.data());

        /* String pointer, size and flags should be reset */
        CORRADE_COMPARE(string.size(), 0);
        CORRADE_COMPARE(string.flags(), StringViewFlag{});
        CORRADE_VERIFY(!string.data());
    }

    CORRADE_COMPARE(NonCopyable::constructed, 0);
    CORRADE_COMPARE(NonCopyable::destructed, 0);
}

template<int align> struct alignas(align) Aligned {
    Aligned() {
        ++constructed;
    }

    ~Aligned() {
        ++destructed;
    }

    static int constructed;
    static int destructed;
};

template<int align> int Aligned<align>::constructed = 0;
template<int align> int Aligned<align>::destructed = 0;

void ArrayTupleTest::construct() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;
    Aligned<16>::constructed = 0;
    Aligned<16>::destructed = 0;

    {
        ArrayView<char> chars;
        ArrayView<NonCopyable> noncopyable;
        ArrayView<int> ints;
        ArrayView<Aligned<16>> aligned;
        StridedArrayView1D<double> strided;
        StridedArrayView3D<float> strided3D;
        StridedArrayView2D<char> stridedErased;
        MutableBitArrayView bits;
        MutableStridedBitArrayView1D bitsStrided;
        MutableStridedBitArrayView3D bitsStrided3D;
        MutableStringView stringNullTerminated;
        MutableStringView string;
        ArrayTuple data{
            {17, chars},
            {4, noncopyable},
            {7, ints},
            {3, aligned},
            {5, strided},
            {{2, 1, 4}, strided3D},
            /* There's no ValueInit alternative for the type-erased variant */
            {Corrade::NoInit, 3, 4, 16, stridedErased},
            {27, bits},
            {4, bitsStrided},
            {{3, 4, 5}, bitsStrided3D},
            {12, stringNullTerminated, StringViewFlag::NullTerminated},
            {7, string},
        };

        /* Check base properties */
        CORRADE_COMPARE(data.size(),
            sizeof(void*) +         /* destructible item count */
            3*(4*sizeof(void*)) +   /* two destructible items + deleter */
            17 +                    /* chars, no padding */
            4 + 3 +                 /* noncopyable + padding to align ints */
            7*4 +                   /* ints + padding to align aligned */
                (sizeof(void*) == 4 ? 8 : 4) +
            3*16 +                  /* aligned */
            5*8 +                   /* strided, right after overaligned so no
                                       padding */
            8*4 +                   /* strided 3D, right after a field with
                                       higher padding */
                8 +                 /* padding to align the next to 16 again */
            3*4 +                   /* 12 bytes, but aligned to 16 */
            4 +                     /* 27 bits, padded to 32 so four bytes */
            1 +                     /* 4 bits, padded to 8 so one byte */
            8 +                     /* 3*4*5 = 60 bits, padded together (not
                                       individually) to 64 so 8 bytes */
            12 + 1 +                /* 12 chars + '\0' */
            7                       /* 7 chars */
        );
        CORRADE_VERIFY(data.data());
        /* Custom deleter to call the destructors */
        CORRADE_VERIFY(data.deleter());

        /* Check array sizes, strides, offsets ... */
        CORRADE_COMPARE(chars.size(), 17);
        CORRADE_COMPARE(noncopyable.size(), 4);
        CORRADE_COMPARE(ints.size(), 7);
        CORRADE_COMPARE(aligned.size(), 3);
        CORRADE_COMPARE(strided.size(), 5);
        CORRADE_COMPARE(strided.stride(), 8);
        CORRADE_COMPARE(strided3D.size(), (Size3D{2, 1, 4}));
        CORRADE_COMPARE(strided3D.stride(),  (Stride3D{16, 16, 4}));
        CORRADE_COMPARE(stridedErased.size(), (Size2D{3, 4}));
        CORRADE_COMPARE(stridedErased.stride(),  (Stride2D{4, 1}));
        CORRADE_COMPARE(bits.offset(), 0);
        CORRADE_COMPARE(bits.size(), 27);
        CORRADE_COMPARE(bitsStrided.offset(), 0);
        CORRADE_COMPARE(bitsStrided.size(), 4);
        CORRADE_COMPARE(bitsStrided.stride(), 1);
        CORRADE_COMPARE(bitsStrided3D.offset(), 0);
        CORRADE_COMPARE(bitsStrided3D.size(), (Size3D{3, 4, 5}));
        CORRADE_COMPARE(bitsStrided3D.stride(),  (Stride3D{20, 5, 1}));
        CORRADE_COMPARE(stringNullTerminated.size(), 12);
        CORRADE_COMPARE(stringNullTerminated.flags(), StringViewFlag::NullTerminated);
        CORRADE_COMPARE(stringNullTerminated[stringNullTerminated.size()], '\0');
        CORRADE_COMPARE(string.size(), 7);
        CORRADE_COMPARE(string.flags(), StringViewFlags{});

        /* ... and data pointers */
        CORRADE_COMPARE(static_cast<void*>(chars.data()), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)));
        CORRADE_COMPARE(static_cast<void*>(noncopyable.data()), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17);
        CORRADE_COMPARE(static_cast<void*>(ints.data()), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3);
        CORRADE_COMPARE(static_cast<void*>(aligned.data()), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4));
        CORRADE_COMPARE(strided.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16);
        CORRADE_COMPARE(strided3D.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8);
        CORRADE_COMPARE(stridedErased.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8);
        CORRADE_COMPARE(bits.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8 + 3*4);
        CORRADE_COMPARE(bitsStrided.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8 + 3*4 +
                    4);
        CORRADE_COMPARE(bitsStrided3D.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8 + 3*4 +
                    4 + 1);
        CORRADE_COMPARE(stringNullTerminated.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8 + 3*4 +
                    4 + 1 + 8);
        CORRADE_COMPARE(string.data(), data.data() +
            sizeof(void*) + 3*(4*sizeof(void*)) + 17 + 4 + 3 + 7*4 +
                (sizeof(void*) == 4 ? 8 : 4) + 3*16 + 5*8 + 8*4 + 8 + 3*4 +
                    4 + 1 + 8 + 12 + 1);

        /* Check that trivial types are zero-init'd and nontrivial had their
           constructor called */
        for(char i: chars) CORRADE_COMPARE(i, 0);
        CORRADE_COMPARE(NonCopyable::constructed, 4);
        CORRADE_COMPARE(NonCopyable::destructed, 0);
        for(int i: ints) CORRADE_COMPARE(i, 0);
        CORRADE_COMPARE(Aligned<16>::constructed, 3);
        CORRADE_COMPARE(Aligned<16>::destructed, 0);
        for(double i: strided) CORRADE_COMPARE(i, 0.0);
        /* MSVC 2015 needs the {}s, FFS */
        for(auto i: strided3D) {
            for(auto j: i) {
                for(float k: j) {
                    CORRADE_COMPARE(k, 0.0f);
                }
            }
        }
        for(std::size_t i = 0; i != bits.size(); ++i)
            CORRADE_VERIFY(!bits[i]);
        for(std::size_t i = 0; i != bitsStrided.size(); ++i)
            CORRADE_VERIFY(!bitsStrided[i]);
        for(std::size_t i = 0; i != bitsStrided3D.size()[0]; ++i)
            for(std::size_t j = 0; j != bitsStrided3D.size()[1]; ++j)
                for(std::size_t k = 0; k != bitsStrided3D.size()[2]; ++k)
                    CORRADE_VERIFY(!bitsStrided3D[i][j][k]);
        for(char i: stringNullTerminated) CORRADE_COMPARE(i, 0);
        for(char i: string) CORRADE_COMPARE(i, 0);
    }

    /* Check that non-trivial destructors were called */
    CORRADE_COMPARE(NonCopyable::constructed, 4);
    CORRADE_COMPARE(NonCopyable::destructed, 4);
    CORRADE_COMPARE(Aligned<16>::constructed, 3);
    CORRADE_COMPARE(Aligned<16>::destructed, 3);
}

void ArrayTupleTest::constructNoInit() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;

    char storage[357];
    for(char& i: storage) i = '\xce';

    CORRADE_VERIFY(true); /* to capture correct function name */

    {
        ArrayView<char> chars;
        ArrayView<char> initializedChars;
        ArrayView<NonCopyable> noncopyable;
        ArrayView<NonCopyable> initializedNoncopyable;
        StridedArrayView1D<double> strided;
        StridedArrayView1D<double> initializedStrided;
        StridedArrayView3D<float> strided3D;
        StridedArrayView2D<float> initializedStrided2D;
        StridedArrayView2D<char> stridedErased;
        MutableBitArrayView bits;
        MutableBitArrayView initializedBits;
        MutableStridedBitArrayView1D bitsStrided;
        MutableStridedBitArrayView1D initializedBitsStrided;
        MutableStridedBitArrayView3D bitsStrided3D;
        MutableStridedBitArrayView3D initializedBitsStrided3D;
        MutableStringView stringNullTerminated;
        MutableStringView initializedStringNullTerminated;
        MutableStringView string;
        MutableStringView initializedString;
        ArrayTuple data{
            {{Corrade::NoInit, 15, chars},
             {Corrade::ValueInit, 15, initializedChars},
             {Corrade::NoInit, 3, noncopyable},
             {Corrade::ValueInit, 2, initializedNoncopyable},
             {Corrade::NoInit, 5, strided},
             {Corrade::ValueInit, 4, initializedStrided},
             {Corrade::NoInit, {1, 2, 3}, strided3D},
             {Corrade::ValueInit, {3, 2}, initializedStrided2D},
             {Corrade::NoInit, 3, 4, 4, stridedErased},
             /* There's no ValueInit alternative for the type-erased variant */
             {Corrade::NoInit, 13, bits},
             {Corrade::ValueInit, 14, initializedBits},
             {Corrade::NoInit, 9, bitsStrided},
             {Corrade::ValueInit, 9, initializedBitsStrided},
             {Corrade::NoInit, {3, 5, 7}, bitsStrided3D},
             {Corrade::ValueInit, {3, 5, 7}, initializedBitsStrided3D},
             {Corrade::NoInit, 11, stringNullTerminated, StringViewFlag::NullTerminated},
             {Corrade::ValueInit, 9, initializedStringNullTerminated, StringViewFlag::NullTerminated},
             {Corrade::NoInit, 10, string},
             {Corrade::ValueInit, 13, initializedString}},
            [&](std::size_t size, std::size_t) -> Containers::Pair<char*, void(*)(char*, std::size_t)> {
                CORRADE_COMPARE_AS(size, arraySize(storage),
                    TestSuite::Compare::LessOrEqual);
                return {storage, [](char*, std::size_t) {}};
            }
        };

        /* Verify that NoInit stayed at 0xce, while the ValueInit are 0x0 and
           only the constructors for the ValueInit'd view were called */
        for(char i: chars) CORRADE_COMPARE(i, '\xce');
        for(char i: initializedChars) CORRADE_COMPARE(i, 0);
        for(auto i: arrayCast<2, char>(strided))
            CORRADE_COMPARE_AS(i, stridedArrayView({
                '\xce', '\xce', '\xce', '\xce', '\xce', '\xce', '\xce', '\xce'
            }), TestSuite::Compare::Container);
        for(double i: initializedStrided) CORRADE_COMPARE(i, 0.0);
        /* MSVC 2015 needs the {}s, FFS */
        for(auto i: arrayCast<4, char>(strided3D)) {
            for(auto j: i) {
                for(auto k: j) {
                    CORRADE_COMPARE_AS(k, stridedArrayView({
                        '\xce', '\xce', '\xce', '\xce'
                    }), TestSuite::Compare::Container);
                }
            }
        }
        /* MSVC 2015 needs the {}s, FFS */
        for(auto i: initializedStrided2D) {
            for(float j: i) {
                CORRADE_COMPARE(j, 0.0f);
            }
        }
        for(auto i: stridedErased)
            CORRADE_COMPARE_AS(i, stridedArrayView({
                '\xce', '\xce', '\xce', '\xce'
            }), TestSuite::Compare::Container);
        CORRADE_COMPARE(NonCopyable::constructed, 2);
        CORRADE_COMPARE(NonCopyable::destructed, 0);
        CORRADE_COMPARE((bits.size() + 7)/8, 2);
        CORRADE_COMPARE((initializedBits.size() + 7)/8, 2);
        for(std::size_t i = 0; i != 2; ++i) {
            CORRADE_ITERATION(i);
            CORRADE_COMPARE(static_cast<char*>(bits.data())[i], '\xce');
            CORRADE_COMPARE(static_cast<char*>(initializedBits.data())[i], 0);
        }
        CORRADE_COMPARE((bitsStrided.size() + 7)/8, 2);
        CORRADE_COMPARE((initializedBitsStrided.size() + 7)/8, 2);
        for(std::size_t i = 0; i != 2; ++i) {
            CORRADE_ITERATION(i);
            CORRADE_COMPARE(static_cast<char*>(bitsStrided.data())[i], '\xce');
            CORRADE_COMPARE(static_cast<char*>(initializedBitsStrided.data())[i], 0);
        }
        CORRADE_COMPARE((Implementation::sizeProduct(bitsStrided3D.size()) + 7)/8, 14);
        CORRADE_COMPARE((Implementation::sizeProduct(initializedBitsStrided3D.size()) + 7)/8, 14);
        for(std::size_t i = 0; i != 14; ++i) {
            CORRADE_ITERATION(i);
            CORRADE_COMPARE(static_cast<char*>(bitsStrided3D.data())[i], '\xce');
            CORRADE_COMPARE(static_cast<char*>(initializedBitsStrided3D.data())[i], 0);
        }
        for(char i: stringNullTerminated)
            CORRADE_COMPARE(i, '\xce');
        CORRADE_COMPARE(stringNullTerminated.flags(), StringViewFlag::NullTerminated);
        CORRADE_COMPARE(stringNullTerminated[stringNullTerminated.size()], 0);
        for(char i: initializedStringNullTerminated)
            CORRADE_COMPARE(i, 0);
        CORRADE_COMPARE(initializedStringNullTerminated.flags(), StringViewFlag::NullTerminated);
        CORRADE_COMPARE(initializedStringNullTerminated[initializedStringNullTerminated.size()], 0);
        for(char i: string) CORRADE_COMPARE(i, '\xce');
        CORRADE_COMPARE(string.flags(), StringViewFlag{});
        for(char i: initializedString) CORRADE_COMPARE(i, 0);
        CORRADE_COMPARE(initializedString.flags(), StringViewFlag{});

        /* Construct the remaining NonCopyables, so their destruction is
           correctly reported */
        for(NonCopyable& i: noncopyable)
            new(&i) NonCopyable{};
    }

    /* All destructors are called on destruction */
    CORRADE_COMPARE(NonCopyable::constructed, 5);
    CORRADE_COMPARE(NonCopyable::destructed, 5);
}

void ArrayTupleTest::constructCustomAllocatorDefaultDeleter() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;

    char* preallocated = new char[256];

    {
        ArrayView<char> chars;
        ArrayView<NonCopyable> noncopyable;
        ArrayTuple data{
            {{15, chars},
             {3, noncopyable}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, std::nullptr_t> {
                return {preallocated, nullptr};
            }
        };

        /* The preallocated memory should get used and later deleted using
           the default delete[] */
        CORRADE_COMPARE(data.data(), static_cast<void*>(preallocated));

        /* But the deleter needs to wrap the destructor calls, so it's not
           stored directly as nullptr */
        CORRADE_VERIFY(data.deleter());

        CORRADE_COMPARE(NonCopyable::constructed, 3);
        CORRADE_COMPARE(NonCopyable::destructed, 0);
    }

    CORRADE_COMPARE(NonCopyable::constructed, 3);
    CORRADE_COMPARE(NonCopyable::destructed, 3);
}

char* globalUsedDeleterPointer = nullptr;
std::size_t globalUsedDeleterSize = 0;

void ArrayTupleTest::constructStatelessDeleter() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;
    globalUsedDeleterPointer = nullptr;
    globalUsedDeleterSize = 0;

    char preallocated[256];
    void(*deleter)(char*, std::size_t) = [](char* data, std::size_t size) {
        globalUsedDeleterPointer = data;
        globalUsedDeleterSize = size;
    };

    const std::size_t expectedSize =
        sizeof(void*) +         /* destructible item count */
        2*(4*sizeof(void*)) +   /* one destructible item + deleter */
        15 + 3;                 /* chars and noncopyable data */

    {
        ArrayView<char> chars;
        ArrayView<NonCopyable> noncopyable;
        ArrayTuple data{
            {{15, chars},
             {3, noncopyable}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, void(*)(char*, std::size_t)> {
                return {preallocated, deleter};
            }
        };

        /* The preallocated memory should get used */
        CORRADE_COMPARE(data.data(), static_cast<void*>(preallocated));
        CORRADE_COMPARE(data.size(), expectedSize);

        /* But the deleter needs to wrap the destructor calls, so it's not
           stored directly */
        CORRADE_VERIFY(data.deleter() != deleter);

        CORRADE_COMPARE(NonCopyable::constructed, 3);
        CORRADE_COMPARE(NonCopyable::destructed, 0);
    }

    /* On deletion, correct parameters should get passed to the deleter */
    CORRADE_COMPARE(globalUsedDeleterPointer, static_cast<void*>(preallocated));
    CORRADE_COMPARE(globalUsedDeleterSize, expectedSize);

    CORRADE_COMPARE(NonCopyable::constructed, 3);
    CORRADE_COMPARE(NonCopyable::destructed, 3);
}

struct alignas(16) StatefulAlignedNonTriviallyDestructibleDeleter {
    explicit StatefulAlignedNonTriviallyDestructibleDeleter(void*& thisPointer, char*& usedDeleterPointer, std::size_t& usedDeleterSize, int& copyConstructorCallCount, int& destructorCallCount): _usedThisPointer{&thisPointer}, _usedDeleterPointer{&usedDeleterPointer}, _usedDeleterSize{&usedDeleterSize}, _copyConstructorCallCount{&copyConstructorCallCount}, _destructorCallCount{&destructorCallCount} {}

    void operator()(char* data, std::size_t size) {
        *_usedThisPointer = this;
        *_usedDeleterPointer = data;
        *_usedDeleterSize = size;
    }

    StatefulAlignedNonTriviallyDestructibleDeleter(const StatefulAlignedNonTriviallyDestructibleDeleter& other): _usedThisPointer{other._usedThisPointer}, _usedDeleterPointer{other._usedDeleterPointer}, _usedDeleterSize{other._usedDeleterSize}, _copyConstructorCallCount{other._copyConstructorCallCount}, _destructorCallCount{other._destructorCallCount} {
        ++*_copyConstructorCallCount;
    }

    ~StatefulAlignedNonTriviallyDestructibleDeleter() {
        /* I'm seriously tired of this crap warning already. I DO NOT see any
           way where this would be "used uninitialized". Fuck off, "helpful"
           GCC warnings. Happens since GCC 11 at least. */
        #if defined(CORRADE_TARGET_GCC) && __GNUC__ >= 11 && __OPTIMIZE__
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wuninitialized"
        #endif
        ++*_destructorCallCount;
        #if defined(CORRADE_TARGET_GCC) && __GNUC__ >= 11 && __OPTIMIZE__
        #pragma GCC diagnostic pop
        #endif
    }

    private:
        void** _usedThisPointer;
        char** _usedDeleterPointer;
        std::size_t* _usedDeleterSize;
        int* _copyConstructorCallCount;
        int* _destructorCallCount;
};

void ArrayTupleTest::constructStatefulAlignedNonTriviallyDestructibleDeleter() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;

    /* StatefulAlignedNonTriviallyDestructibleDeleter is 16-byte aligned and
       if this array is not aligned (like on Android x86), it may cause nasty
       crashes when calling the deleter */
    alignas(16) char preallocated[256];
    void* usedThisPointer;
    char* usedDeleterPointer;
    std::size_t usedDeleterSize;
    int copyConstructorCallCount{}, destructorCallCount{};

    const std::size_t expectedSize =
        sizeof(void*) +         /* destructible item count */
        2*(4*sizeof(void*)) +   /* one destructible item + deleter */
        15 + 3 +                /* chars and noncopyable data */
                                /* padding to align the deleter to 16 bytes */
        (sizeof(void*) == 4 ? 10 : 6) +
        sizeof(StatefulAlignedNonTriviallyDestructibleDeleter);
                                /* and the deleter state */

    {
        ArrayView<char> chars;
        ArrayView<NonCopyable> noncopyable;
        ArrayTuple data{
            {{15, chars},
             {3, noncopyable}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, StatefulAlignedNonTriviallyDestructibleDeleter> {
                return {preallocated, StatefulAlignedNonTriviallyDestructibleDeleter{usedThisPointer, usedDeleterPointer, usedDeleterSize, copyConstructorCallCount, destructorCallCount}};
            }
        };

        /* The preallocated memory should get used */
        CORRADE_COMPARE(data.data(), static_cast<void*>(preallocated));
        CORRADE_COMPARE(data.size(), expectedSize);

        /* The deleter is anything but the default one */
        CORRADE_VERIFY(data.deleter());

        CORRADE_COMPARE(NonCopyable::constructed, 3);
        CORRADE_COMPARE(NonCopyable::destructed, 0);
    }

    /* Correct pointer + size should be passed to the deleter */
    CORRADE_COMPARE(usedDeleterPointer, static_cast<void*>(preallocated));
    CORRADE_COMPARE(usedDeleterSize, expectedSize);

    /* The deleter should be copied out of the allocation to prevent it from
       unknowingly accessing gone memory after it frees it */
    CORRADE_VERIFY(usedThisPointer < preallocated || usedThisPointer > preallocated);

    /* Apart from all destructions coming from the copies, one extra destructor
       should be called at the end to match the initial construction */
    CORRADE_COMPARE(destructorCallCount, copyConstructorCallCount + 1);

    CORRADE_COMPARE(NonCopyable::constructed, 3);
    CORRADE_COMPARE(NonCopyable::destructed, 3);
}

void ArrayTupleTest::constructTriviallyDestructible() {
    ArrayView<int> ints;
    ArrayView<char> chars;
    ArrayView<double> doubles;
    ArrayTuple data{
        {3, ints},
        {13, chars},
        {2, doubles}
    };

    CORRADE_VERIFY(data.data());
    CORRADE_COMPARE(data.size(),
        3*4 +
        13 +
        /* 7 / 3 bytes padding after the chars to align doubles. Android x86 is
           the one with 4-byte-aligned doubles. Wonderful, eh? */
        (alignof(double) == 8 ? 7 : 3) +
        2*8
    );

    /* The default deleter is used, as there's nothing to non-trivially
       destruct */
    CORRADE_VERIFY(!data.deleter());

    /* Check array sizes and offsets. No metadata should be anywhere. */
    CORRADE_COMPARE(ints.size(), 3);
    CORRADE_COMPARE(chars.size(), 13);
    CORRADE_COMPARE(doubles.size(), 2);
    CORRADE_COMPARE(static_cast<void*>(ints.data()), data.data());
    CORRADE_COMPARE(static_cast<void*>(chars.data()), data.data() + 3*4);
    CORRADE_COMPARE(static_cast<void*>(doubles.data()), data.data() + 3*4 + 13 + (alignof(double) == 8 ? 7 : 3));
}

void ArrayTupleTest::constructTriviallyDestructibleCustomAllocatorDefaultDeleter() {
    char* preallocated = new char[256];

    ArrayView<int> ints;
    ArrayView<char> chars;
    ArrayView<double> doubles;
    ArrayTuple data{
        {{3, ints},
         {13, chars},
         {2, doubles}},
        [&](std::size_t, std::size_t) -> Containers::Pair<char*, std::nullptr_t> {
            return {preallocated, nullptr};
        }
    };

    CORRADE_VERIFY(data.data());
    CORRADE_COMPARE(data.size(),
        3*4 +
        13 +
        /* 7 / 3 bytes padding after the chars to align doubles. Android x86 is
           the one with 4-byte-aligned doubles. Wonderful, eh? */
        (alignof(double) == 8 ? 7 : 3) +
        2*8
    );

    /* The default deleter is used, as there's nothing to non-trivially
       destruct */
    CORRADE_VERIFY(!data.deleter());

    /* And no metadata at the front here either */
    CORRADE_COMPARE(static_cast<void*>(ints.data()), data.data());
}

void ArrayTupleTest::constructTriviallyDestructibleStatelessDeleter() {
    globalUsedDeleterPointer = nullptr;
    globalUsedDeleterSize = 0;

    char preallocated[256];
    void(*deleter)(char*, std::size_t) = [](char* data, std::size_t size) {
        globalUsedDeleterPointer = data;
        globalUsedDeleterSize = size;
    };

    const std::size_t expectedSize =
        3*4 +
        13 +
        /* 7 / 3 bytes padding after the chars to align doubles. Android x86 is
           the one with 4-byte-aligned doubles. Wonderful, eh? */
        (alignof(double) == 8 ? 7 : 3) +
        2*8;

    {
        ArrayView<int> ints;
        ArrayView<char> chars;
        ArrayView<double> doubles;
        ArrayTuple data{
            {{3, ints},
             {13, chars},
             {2, doubles}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, void(*)(char*, std::size_t)> {
                return {preallocated, deleter};
            }
        };

        CORRADE_VERIFY(data.data());
        CORRADE_COMPARE(data.size(), expectedSize);

        /* The stateless deleter is used directly, as there's nothing to
           non-trivially destruct */
        CORRADE_VERIFY(data.deleter() == deleter);

        /* And no metadata at the front here either */
        CORRADE_COMPARE(static_cast<void*>(ints.data()), data.data());
    }

    /* On deletion, correct parameters should get passed to the deleter */
    CORRADE_COMPARE(globalUsedDeleterPointer, static_cast<void*>(preallocated));
    CORRADE_COMPARE(globalUsedDeleterSize, expectedSize);
}

void ArrayTupleTest::constructTriviallyDestructibleStatefulAlignedNonTriviallyDestructibleDeleter() {
    /* StatefulAlignedNonTriviallyDestructibleDeleter is 16-byte aligned and
       if this array is not aligned (like on Android x86), it may cause nasty
       crashes when calling the deleter */
    alignas(16) char preallocated[256];
    void* usedThisPointer;
    char* usedDeleterPointer;
    std::size_t usedDeleterSize;
    int copyConstructorCallCount{}, destructorCallCount{};

    const std::size_t expectedSize =
        sizeof(void*) +     /* destructible item count */
        4*sizeof(void*) +   /* just one destructible item for the deleter */
        3*4 +
        13 +                /* padding after chars to align doubles */
            (sizeof(void*) == 4 ? 3 : 7) +
        2*8 +               /* padding to align the deleter to 16 bytes */
            (sizeof(void*) == 4 ? 0 : 8) +
        sizeof(StatefulAlignedNonTriviallyDestructibleDeleter);
                            /* and the deleter state */

    {
        ArrayView<int> ints;
        ArrayView<char> chars;
        ArrayView<double> doubles;
        ArrayTuple data{
            {{3, ints},
             {13, chars},
             {2, doubles}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, StatefulAlignedNonTriviallyDestructibleDeleter> {
                return {preallocated, StatefulAlignedNonTriviallyDestructibleDeleter{usedThisPointer, usedDeleterPointer, usedDeleterSize, copyConstructorCallCount, destructorCallCount}};
            }
        };

        /* The preallocated memory should get used */
        CORRADE_COMPARE(data.data(), static_cast<void*>(preallocated));
        CORRADE_COMPARE(data.size(), expectedSize);

        /* The deleter is anything but the default one */
        CORRADE_VERIFY(data.deleter());
    }

    /* Correct pointer + size should be passed to the deleter */
    CORRADE_COMPARE(usedDeleterPointer, static_cast<void*>(preallocated));
    CORRADE_COMPARE(usedDeleterSize, expectedSize);

    /* The deleter should be copied out of the allocation to prevent it from
       unknowingly accessing gone memory after it frees it */
    CORRADE_VERIFY(usedThisPointer < preallocated || usedThisPointer > preallocated);

    /* Apart from all destructions coming from the copies, one extra destructor
       should be called at the end to match the initial construction */
    CORRADE_COMPARE(destructorCallCount, copyConstructorCallCount + 1);
}

struct NonTriviallyDestructible {
    static int destructorCallCount;

    int a;

    ~NonTriviallyDestructible() {
        ++destructorCallCount;
    }
};

int NonTriviallyDestructible::destructorCallCount = 0;

void ArrayTupleTest::constructTriviallyConstructibleNonTriviallyDestructible() {
    /* If a type is trivially constructible but not trivially destructible, it
       *should not* go through the simplified constructor that just calls
       std::memcpy() on the whole data. This test verifies that.

       However, on many compilers, if a type isn't trivially destructible, it's
       also marked as not trivially constructible. Such case is with GCC and
       Clang. See the "Notes" here for more information:
       https://en.cppreference.com/w/cpp/types/is_constructible. */
    if(!
        #ifdef CORRADE_STD_IS_TRIVIALLY_TRAITS_SUPPORTED
        std::is_trivially_constructible<NonTriviallyDestructible>::value
        #else
        std::has_trivial_default_constructor<NonTriviallyDestructible>::value
        #endif
    )
        CORRADE_SKIP("Non-trivially-destructible types aren't marked as is_trivially_constructible on this compiler, can't test.");

    CORRADE_VERIFY(!std::is_trivially_destructible<NonTriviallyDestructible>::value);

    NonTriviallyDestructible::destructorCallCount = 0;
    {
        ArrayView<NonTriviallyDestructible> view;
        ArrayTuple data{
            {Corrade::ValueInit, 5, view}
        };
        CORRADE_COMPARE(NonTriviallyDestructible::destructorCallCount, 0);
    }
    CORRADE_COMPARE(NonTriviallyDestructible::destructorCallCount, 5);
}

void ArrayTupleTest::constructStringInvalidFlags() {
    CORRADE_SKIP_IF_NO_ASSERT();

    MutableStringView string;

    std::ostringstream out;
    Error redirectError{&out};
    ArrayTuple{
        {Corrade::ValueInit, 3, string, StringViewFlag::Global},
        {Corrade::NoInit, 3, string, StringViewFlag::NullTerminated|StringViewFlag::Global},
    };
    CORRADE_COMPARE(out.str(),
        "Containers::ArrayTuple: Containers::StringViewFlag::Global not allowed for a string view\n"
        "Containers::ArrayTuple: Containers::StringViewFlag::Global not allowed for a string view\n");
}

void ArrayTupleTest::constructCopy() {
    CORRADE_VERIFY(!std::is_copy_constructible<ArrayTuple>::value);
    CORRADE_VERIFY(!std::is_copy_constructible<ArrayTuple>::value);
}

void ArrayTupleTest::constructMove() {
    char preallocated[256];
    void(*deleter)(char*, std::size_t) = [](char*, std::size_t) {};

    ArrayView<int> ints;
    ArrayTuple a{
        {{5, ints}},
        [&](std::size_t, std::size_t) -> Containers::Pair<char*, void(*)(char*, std::size_t)> {
            return {preallocated, deleter};
        }
    };

    ArrayTuple b{Utility::move(a)};
    CORRADE_VERIFY(!a.data());
    CORRADE_VERIFY(!a.size());
    CORRADE_VERIFY(!a.deleter());
    CORRADE_COMPARE(b.data(), static_cast<void*>(preallocated));
    CORRADE_COMPARE(b.size(), 20);
    CORRADE_VERIFY(b.deleter() == deleter);

    ArrayTuple c;
    c = Utility::move(b);
    CORRADE_VERIFY(!b.data());
    CORRADE_VERIFY(!b.size());
    CORRADE_VERIFY(!b.deleter());
    CORRADE_COMPARE(c.data(), static_cast<void*>(preallocated));
    CORRADE_COMPARE(c.size(), 20);
    CORRADE_VERIFY(c.deleter() == deleter);

    CORRADE_VERIFY(std::is_nothrow_move_constructible<ArrayTuple>::value);
    CORRADE_VERIFY(std::is_nothrow_move_assignable<ArrayTuple>::value);
}

struct Big {
    Big(const Big&) = delete;
    Big(Big&&) = delete;
    Big& operator=(const Big&) = delete;
    Big& operator=(Big&&) = delete;
    Big() {
        ++constructed;
        thisPointer = this;
        thisPointer2 = this;
    }

    ~Big() {
        /* Just a check that the deleter is really called on a correct address.
           If it's not, the memory will contain something different and this
           won't match. */
        if(thisPointer == this && thisPointer2 == this)
            ++destructed;
    }

    Big* thisPointer;
    Big* thisPointer2;

    static int constructed;
    static int destructed;
};

int Big::constructed = 0;
int Big::destructed = 0;

void ArrayTupleTest::constructBig() {
    Big::constructed = 0;
    Big::destructed = 0;

    {
        ArrayView<char> chars;
        ArrayView<Big> bigs;
        ArrayTuple data{
            {17, chars},
            {7, bigs}
        };

        /* Check base properties */
        CORRADE_COMPARE(data.size(),
            sizeof(void*) +         /* destructible item count */
            2*(4*sizeof(void*)) +   /* one destructible item + deleter */
            17 +                    /* chars, padding */
                (sizeof(void*) == 4 ? 3 : 7) +
            7*sizeof(Big)           /* bigs */
        );
        CORRADE_VERIFY(data.data());
        /* Custom deleter to call the destructors */
        CORRADE_VERIFY(data.deleter());

        /* Check array sizes and offsets */
        CORRADE_COMPARE(chars.size(), 17);
        CORRADE_COMPARE(bigs.size(), 7);
        CORRADE_COMPARE(static_cast<void*>(chars.data()), data.data() +
            sizeof(void*) + 2*(4*sizeof(void*)));
        CORRADE_COMPARE(static_cast<void*>(bigs.data()), data.data() +
            sizeof(void*) + 2*(4*sizeof(void*)) + 17 + (sizeof(void*) == 4 ? 3 : 7));

        /* Check that trivial types are zero-init'd and nontrivial had their
           constructor called */
        for(char i: chars) CORRADE_COMPARE(i, 0);
        CORRADE_COMPARE(Big::constructed, 7);
        CORRADE_COMPARE(Big::destructed, 0);
    }

    /* Check that non-trivial destructors were called */
    CORRADE_COMPARE(Big::constructed, 7);
    CORRADE_COMPARE(Big::destructed, 7);
}

void ArrayTupleTest::allocatorAlignmentEmpty() {
    std::size_t alignmentRequirement = ~std::size_t{};

    CORRADE_VERIFY(true); /* Just to register correct function name */

    ArrayTuple data{
        {},
        [&](std::size_t size, std::size_t alignment) -> Containers::Pair<char*, std::nullptr_t> {
            CORRADE_COMPARE(size, 0);
            CORRADE_COMPARE(alignment, 1);
            alignmentRequirement = alignment;
            return {};
        }
    };

    /* Check again to verify the allocator actually got called */
    CORRADE_COMPARE(alignmentRequirement, 1);
}

template<int a> void ArrayTupleTest::allocatorAlignmentFromItems() {
    setTestCaseTemplateName(Utility::format("{}", a));

    CORRADE_VERIFY(true); /* Just to register correct function name */

    ArrayView<Aligned<a>> view;
    ArrayTuple data{
        {{3, view}},
        [&](std::size_t size, std::size_t alignment) -> Containers::Pair<char*, std::nullptr_t> {
            CORRADE_COMPARE(alignment, a);
            return {new char[size], nullptr};
        }
    };

    CORRADE_COMPARE(view.size(), 3);
}

template<int a> void ArrayTupleTest::allocatorAlignmentFromDeleter() {
    setTestCaseTemplateName(Utility::format("{}", a));

    CORRADE_VERIFY(true); /* Just to register correct function name */

    struct alignas(a) Deleter {
        #ifdef CORRADE_MSVC2015_COMPATIBILITY
        /* If this is not present on MSVC2015, the test segfaults. HEH */
        char aehhhhh;
        #endif

        void operator()(char* data, std::size_t) {
            delete[] data;
        }
    };

    ArrayView<char> view;
    ArrayTuple data{
        {{3, view}},
        [&](std::size_t size, std::size_t alignment) -> Containers::Pair<char*, Deleter> {
            CORRADE_COMPARE(alignment, a);
            return {new char[size], {}};
        }
    };

    CORRADE_COMPARE(view.size(), 3);
}

void ArrayTupleTest::convertArray() {
    char preallocated[256]{};
    void(*deleter)(char*, std::size_t) = [](char* data, std::size_t) {
        ++data[255];
    };

    {
        ArrayView<int> ints;
        ArrayView<char> chars;
        ArrayView<double> doubles;
        Array<char> data = ArrayTuple{
            {{3, ints},
            {13, chars},
            {2, doubles}},
            [&](std::size_t, std::size_t) -> Containers::Pair<char*, void(*)(char*, std::size_t)> {
                return {preallocated, deleter};
            }
        };

        CORRADE_VERIFY(data.data());
        CORRADE_COMPARE(data.size(),
            3*4 +
            13 +
            /* 7 / 3 bytes padding after the chars to align doubles. Android
               x86 is the one with 4-byte-aligned doubles. Wonderful, eh? */
            (alignof(double) == 8 ? 7 : 3) +
            2*8
        );

        /* The stateless deleter is used directly, as there's nothing to
        non-trivially destruct */
        CORRADE_VERIFY(data.deleter() == deleter);
    }

    /* Check the deleter was called just once */
    CORRADE_COMPARE(preallocated[255], 1);
}

void ArrayTupleTest::convertArrayInvalid() {
    CORRADE_SKIP_IF_NO_ASSERT();

    ArrayView<NonCopyable> noncopyable;
    ArrayTuple nonTrivialData{
        {5, noncopyable}
    };

    struct Deleter {
        int state;
        void operator()(char* data, std::size_t) {
            delete[] data;
        }
    };
    ArrayTuple nonTrivialDeleter{
        {},
        [&](std::size_t size, std::size_t) -> Containers::Pair<char*, Deleter> {
            return {new char[size], {}};
        }
    };

    std::ostringstream out;
    Error redirectError{&out};
    Array<char> a = Utility::move(nonTrivialData);
    Array<char> b = Utility::move(nonTrivialDeleter);
    CORRADE_COMPARE(out.str(),
        "Containers::ArrayTuple: conversion to Array allowed only with trivially destructible types and a stateless destructor\n"
        "Containers::ArrayTuple: conversion to Array allowed only with trivially destructible types and a stateless destructor\n");
}

void ArrayTupleTest::release() {
    NonCopyable::constructed = 0;
    NonCopyable::destructed = 0;

    ArrayView<NonCopyable> noncopyable;
    ArrayTuple data{
        {5, noncopyable}
    };

    std::size_t size = data.size();
    char* pointer = data.data();
    ArrayTuple::Deleter deleter = data.deleter();
    char* released = data.release();
    deleter(released, size);

    /* Not comparing pointers directly because then Clang Analyzer complains
       that printing the value of `released` is use-after-free. Um. */
    CORRADE_COMPARE(reinterpret_cast<std::intptr_t>(pointer), reinterpret_cast<std::intptr_t>(released));
    CORRADE_VERIFY(!data.data());
    CORRADE_COMPARE(data.size(), 0);
    CORRADE_VERIFY(!data.deleter());
}

void ArrayTupleTest::emplaceConstructItemExplicitInCopyInitialization() {
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
    ArrayView<ContainingExplicitDefaultWithImplicitConstructor> view;
    ArrayTuple data{
        {3, view}
    };
    CORRADE_COMPARE(data.size(), 3);
}

void ArrayTupleTest::copyConstructPlainDeleterStruct() {
    struct ExtremelyTrivialDeleter {
        int a;
        char b;

        void operator()(char*, std::size_t) {}
    };

    char storage[256];
    ArrayView<int> view;

    /* This needs special handling on GCC 4.8, where T{b} (copy-construction)
       attempts to convert ExtremelyTrivial to int to initialize the first
       argument and fails miserably. */
    ArrayTuple data{
        {{5, view}},
        [&](std::size_t, std::size_t) -> Containers::Pair<char*, ExtremelyTrivialDeleter> {
            return {storage, ExtremelyTrivialDeleter{}};
        }
    };

    CORRADE_COMPARE(view.size(), 5);
}

}}}}

CORRADE_TEST_MAIN(Corrade::Containers::Test::ArrayTupleTest)

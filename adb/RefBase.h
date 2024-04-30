#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <type_traits>

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>


#include "LightRefBase.h"

#include "StrongPointer.h"
#include "TypeHelpers.h"

// ---------------------------------------------------------------------------
namespace android {

// ---------------------------------------------------------------------------

#define COMPARE_WEAK(_op_)                                      \
template<typename U>                                            \
inline bool operator _op_ (const U* o) const {                  \
    return m_ptr _op_ o;                                        \
}                                                               \
/* Needed to handle type inference for nullptr: */              \
inline bool operator _op_ (const T* o) const {                  \
    return m_ptr _op_ o;                                        \
}

    template<template<typename C> class comparator, typename T, typename U>
    static inline bool _wp_compare_(T* a, U* b) {
        return comparator<typename std::common_type<T*, U*>::type>()(a, b);
    }

    // Use std::less and friends to avoid undefined behavior when ordering pointers
    // to different objects.
#define COMPARE_WEAK_FUNCTIONAL(_op_, _compare_)                 \
template<typename U>                                             \
inline bool operator _op_ (const U* o) const {                   \
    return _wp_compare_<_compare_>(m_ptr, o);                    \
}

// ---------------------------------------------------------------------------

// RefererenceRenamer is pure abstract, there is no virtual method
// implementation to put in a translation unit in order to silence the
// weak vtables warning.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

    class ReferenceRenamer {
    protected:
        // destructor is purposely not virtual so we avoid code overhead from
        // subclasses; we have to make it protected to guarantee that it
        // cannot be called from this base class (and to make strict compilers
        // happy).
        ~ReferenceRenamer() { }
    public:
        virtual void operator()(size_t i) const = 0;
    };

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

    // ---------------------------------------------------------------------------

    class RefBase
    {
    public:
        void            incStrong(const void* id) const;
        void            incStrongRequireStrong(const void* id) const;
        void            decStrong(const void* id) const;

        void            forceIncStrong(const void* id) const;

        //! DEBUGGING ONLY: Get current strong ref count.
        int32_t         getStrongCount() const;

        class weakref_type
        {
        public:
            RefBase* refBase() const;

            void                incWeak(const void* id);
            void                incWeakRequireWeak(const void* id);
            void                decWeak(const void* id);

            // acquires a strong reference if there is already one.
            bool                attemptIncStrong(const void* id);

            // acquires a weak reference if there is already one.
            // This is not always safe. see ProcessState.cpp and BpBinder.cpp
            // for proper use.
            bool                attemptIncWeak(const void* id);

            //! DEBUGGING ONLY: Get current weak ref count.
            int32_t             getWeakCount() const;

            //! DEBUGGING ONLY: Print references held on object.
            void                printRefs() const;

            //! DEBUGGING ONLY: Enable tracking for this object.
            // enable -- enable/disable tracking
            // retain -- when tracking is enable, if true, then we save a stack trace
            //           for each reference and dereference; when retain == false, we
            //           match up references and dereferences and keep only the
            //           outstanding ones.

            void                trackMe(bool enable, bool retain);
        };

        weakref_type* createWeak(const void* id) const;

        weakref_type* getWeakRefs() const;

        //! DEBUGGING ONLY: Print references held on object.
        inline  void            printRefs() const { getWeakRefs()->printRefs(); }

        //! DEBUGGING ONLY: Enable tracking of object.
        inline  void            trackMe(bool enable, bool retain)
        {
            getWeakRefs()->trackMe(enable, retain);
        }

    protected:
        // When constructing these objects, prefer using sp::make<>. Using a RefBase
        // object on the stack or with other refcount mechanisms (e.g.
        // std::shared_ptr) is inherently wrong. RefBase types have an implicit
        // ownership model and cannot be safely used with other ownership models.

        RefBase();
        virtual                 ~RefBase();

        //! Flags for extendObjectLifetime()
        enum {
            OBJECT_LIFETIME_STRONG = 0x0000,
            OBJECT_LIFETIME_WEAK = 0x0001,
            OBJECT_LIFETIME_MASK = 0x0001
        };

        void            extendObjectLifetime(int32_t mode);

        //! Flags for onIncStrongAttempted()
        enum {
            FIRST_INC_STRONG = 0x0001
        };

        // Invoked after creation of initial strong pointer/reference.
        virtual void            onFirstRef();
        // Invoked when either the last strong reference goes away, or we need to undo
        // the effect of an unnecessary onIncStrongAttempted.
        virtual void            onLastStrongRef(const void* id);
        // Only called in OBJECT_LIFETIME_WEAK case.  Returns true if OK to promote to
        // strong reference. May have side effects if it returns true.
        // The first flags argument is always FIRST_INC_STRONG.
        // TODO: Remove initial flag argument.
        virtual bool            onIncStrongAttempted(uint32_t flags, const void* id);
        // Invoked in the OBJECT_LIFETIME_WEAK case when the last reference of either
        // kind goes away.  Unused.
        // TODO: Remove.
        virtual void            onLastWeakRef(const void* id);

    private:
        friend class weakref_type;
        class weakref_impl;

        RefBase(const RefBase& o);
        RefBase& operator=(const RefBase& o);

    private:
        friend class ReferenceMover;

        static void renameRefs(size_t n, const ReferenceRenamer& renamer);

        static void renameRefId(weakref_type* ref,
            const void* old_id, const void* new_id);

        static void renameRefId(RefBase* ref,
            const void* old_id, const void* new_id);

        weakref_impl* const mRefs;
    };

    // ---------------------------------------------------------------------------

    template <typename T>
    class wp
    {
    public:
        typedef typename RefBase::weakref_type weakref_type;

        inline constexpr wp() : m_ptr(nullptr), m_refs(nullptr) { }

        // if nullptr, returns nullptr
        //
        // if a weak pointer is already available, this will retrieve it,
        // otherwise, this will abort
        static inline wp<T> fromExisting(T* other);

        // for more information about this flag, see above
#if defined(ANDROID_UTILS_REF_BASE_DISABLE_IMPLICIT_CONSTRUCTION)
        wp(std::nullptr_t) : wp() {}
#else
        wp(T* other);  // NOLINT(implicit)
        template <typename U>
        wp(U* other);  // NOLINT(implicit)
        wp& operator=(T* other);
        template <typename U>
        wp& operator=(U* other);
#endif

        wp(const wp<T>& other);
        explicit wp(const sp<T>& other);

        template<typename U> wp(const sp<U>& other);  // NOLINT(implicit)
        template<typename U> wp(const wp<U>& other);  // NOLINT(implicit)

        ~wp();

        // Assignment

        wp& operator = (const wp<T>& other);
        wp& operator = (const sp<T>& other);

        template<typename U> wp& operator = (const wp<U>& other);
        template<typename U> wp& operator = (const sp<U>& other);

        void set_object_and_refs(T* other, weakref_type* refs);

        // promotion to sp

        sp<T> promote() const;

        // Reset

        void clear();

        // Accessors

        inline  weakref_type* get_refs() const { return m_refs; }

        inline  T* unsafe_get() const { return m_ptr; }

        // Operators

        COMPARE_WEAK(== )
            COMPARE_WEAK(!= )
            COMPARE_WEAK_FUNCTIONAL(> , std::greater)
            COMPARE_WEAK_FUNCTIONAL(< , std::less)
            COMPARE_WEAK_FUNCTIONAL(<= , std::less_equal)
            COMPARE_WEAK_FUNCTIONAL(>= , std::greater_equal)

            template<typename U>
        inline bool operator == (const wp<U>& o) const {
            return m_refs == o.m_refs;  // Implies m_ptr == o.mptr; see invariants below.
        }

        template<typename U>
        inline bool operator == (const sp<U>& o) const {
            // Just comparing m_ptr fields is often dangerous, since wp<> may refer to an older
            // object at the same address.
            if (o == nullptr) {
                return m_ptr == nullptr;
            }
            else {
                return m_refs == o->getWeakRefs();  // Implies m_ptr == o.mptr.
            }
        }

        template<typename U>
        inline bool operator != (const sp<U>& o) const {
            return !(*this == o);
        }

        template<typename U>
        inline bool operator > (const wp<U>& o) const {
            if (m_ptr == o.m_ptr) {
                return _wp_compare_<std::greater>(m_refs, o.m_refs);
            }
            else {
                return _wp_compare_<std::greater>(m_ptr, o.m_ptr);
            }
        }

        template<typename U>
        inline bool operator < (const wp<U>& o) const {
            if (m_ptr == o.m_ptr) {
                return _wp_compare_<std::less>(m_refs, o.m_refs);
            }
            else {
                return _wp_compare_<std::less>(m_ptr, o.m_ptr);
            }
        }
        template<typename U> inline bool operator != (const wp<U>& o) const { return !operator == (o); }
        template<typename U> inline bool operator <= (const wp<U>& o) const { return !operator > (o); }
        template<typename U> inline bool operator >= (const wp<U>& o) const { return !operator < (o); }

    private:
        template<typename Y> friend class sp;
        template<typename Y> friend class wp;

        T* m_ptr;
        weakref_type* m_refs;
    };

#undef COMPARE_WEAK
#undef COMPARE_WEAK_FUNCTIONAL

    // ---------------------------------------------------------------------------
    // No user serviceable parts below here.

    // Implementation invariants:
    // Either
    // 1) m_ptr and m_refs are both null, or
    // 2) m_refs == m_ptr->mRefs, or
    // 3) *m_ptr is no longer live, and m_refs points to the weakref_type object that corresponded
    //    to m_ptr while it was live. *m_refs remains live while a wp<> refers to it.
    //
    // The m_refs field in a RefBase object is allocated on construction, unique to that RefBase
    // object, and never changes. Thus if two wp's have identical m_refs fields, they are either both
    // null or point to the same object. If two wp's have identical m_ptr fields, they either both
    // point to the same live object and thus have the same m_ref fields, or at least one of the
    // objects is no longer live.
    //
    // Note that the above comparison operations go out of their way to provide an ordering consistent
    // with ordinary pointer comparison; otherwise they could ignore m_ptr, and just compare m_refs.

    template <typename T>
    wp<T> wp<T>::fromExisting(T* other) {
        if (!other) return nullptr;

        auto refs = other->getWeakRefs();
        refs->incWeakRequireWeak(other);

        wp<T> ret;
        ret.m_ptr = other;
        ret.m_refs = refs;
        return ret;
    }

#if !defined(ANDROID_UTILS_REF_BASE_DISABLE_IMPLICIT_CONSTRUCTION)
    template<typename T>
    wp<T>::wp(T* other)
        : m_ptr(other)
    {
        m_refs = other ? m_refs = other->createWeak(this) : nullptr;
    }

    template <typename T>
    template <typename U>
    wp<T>::wp(U* other) : m_ptr(other) {
        m_refs = other ? other->createWeak(this) : nullptr;
    }

    template <typename T>
    wp<T>& wp<T>::operator=(T* other) {
        weakref_type* newRefs = other ? other->createWeak(this) : nullptr;
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = other;
        m_refs = newRefs;
        return *this;
    }

    template <typename T>
    template <typename U>
    wp<T>& wp<T>::operator=(U* other) {
        weakref_type* newRefs = other ? other->createWeak(this) : 0;
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = other;
        m_refs = newRefs;
        return *this;
    }
#endif

    template<typename T>
    wp<T>::wp(const wp<T>& other)
        : m_ptr(other.m_ptr), m_refs(other.m_refs)
    {
        if (m_ptr) m_refs->incWeak(this);
    }

    template<typename T>
    wp<T>::wp(const sp<T>& other)
        : m_ptr(other.m_ptr)
    {
        m_refs = m_ptr ? m_ptr->createWeak(this) : nullptr;
    }

    template<typename T> template<typename U>
    wp<T>::wp(const wp<U>& other)
        : m_ptr(other.m_ptr)
    {
        if (m_ptr) {
            m_refs = other.m_refs;
            m_refs->incWeak(this);
        }
        else {
            m_refs = nullptr;
        }
    }

    template<typename T> template<typename U>
    wp<T>::wp(const sp<U>& other)
        : m_ptr(other.m_ptr)
    {
        m_refs = m_ptr ? m_ptr->createWeak(this) : nullptr;
    }

    template<typename T>
    wp<T>::~wp()
    {
        if (m_ptr) m_refs->decWeak(this);
    }

    template<typename T>
    wp<T>& wp<T>::operator = (const wp<T>& other)
    {
        weakref_type* otherRefs(other.m_refs);
        T* otherPtr(other.m_ptr);
        if (otherPtr) otherRefs->incWeak(this);
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = otherPtr;
        m_refs = otherRefs;
        return *this;
    }

    template<typename T>
    wp<T>& wp<T>::operator = (const sp<T>& other)
    {
        weakref_type* newRefs =
            other != nullptr ? other->createWeak(this) : nullptr;
        T* otherPtr(other.m_ptr);
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = otherPtr;
        m_refs = newRefs;
        return *this;
    }

    template<typename T> template<typename U>
    wp<T>& wp<T>::operator = (const wp<U>& other)
    {
        weakref_type* otherRefs(other.m_refs);
        U* otherPtr(other.m_ptr);
        if (otherPtr) otherRefs->incWeak(this);
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = otherPtr;
        m_refs = otherRefs;
        return *this;
    }

    template<typename T> template<typename U>
    wp<T>& wp<T>::operator = (const sp<U>& other)
    {
        weakref_type* newRefs =
            other != nullptr ? other->createWeak(this) : 0;
        U* otherPtr(other.m_ptr);
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = otherPtr;
        m_refs = newRefs;
        return *this;
    }

    template<typename T>
    void wp<T>::set_object_and_refs(T* other, weakref_type* refs)
    {
        if (other) refs->incWeak(this);
        if (m_ptr) m_refs->decWeak(this);
        m_ptr = other;
        m_refs = refs;
    }

    template<typename T>
    sp<T> wp<T>::promote() const
    {
        sp<T> result;
        if (m_ptr && m_refs->attemptIncStrong(&result)) {
            result.set_pointer(m_ptr);
        }
        return result;
    }

    template<typename T>
    void wp<T>::clear()
    {
        if (m_ptr) {
            m_refs->decWeak(this);
            m_refs = 0;
            m_ptr = 0;
        }
    }

    // ---------------------------------------------------------------------------

    // this class just serves as a namespace so TYPE::moveReferences can stay
    // private.
    class ReferenceMover {
    public:
        // it would be nice if we could make sure no extra code is generated
        // for sp<TYPE> or wp<TYPE> when TYPE is a descendant of RefBase:
        // Using a sp<RefBase> override doesn't work; it's a bit like we wanted
        // a template<typename TYPE inherits RefBase> template...

        template<typename TYPE> static inline
            void move_references(sp<TYPE>* dest, sp<TYPE> const* src, size_t n) {

            class Renamer : public ReferenceRenamer {
                sp<TYPE>* d_;
                sp<TYPE> const* s_;
                virtual void operator()(size_t i) const {
                    // The id are known to be the sp<>'s this pointer
                    TYPE::renameRefId(d_[i].get(), &s_[i], &d_[i]);
                }
            public:
                Renamer(sp<TYPE>* d, sp<TYPE> const* s) : d_(d), s_(s) { }
                virtual ~Renamer() { }
            };

            memmove(dest, src, n * sizeof(sp<TYPE>));
            TYPE::renameRefs(n, Renamer(dest, src));
        }


        template<typename TYPE> static inline
            void move_references(wp<TYPE>* dest, wp<TYPE> const* src, size_t n) {

            class Renamer : public ReferenceRenamer {
                wp<TYPE>* d_;
                wp<TYPE> const* s_;
                virtual void operator()(size_t i) const {
                    // The id are known to be the wp<>'s this pointer
                    TYPE::renameRefId(d_[i].get_refs(), &s_[i], &d_[i]);
                }
            public:
                Renamer(wp<TYPE>* rd, wp<TYPE> const* rs) : d_(rd), s_(rs) { }
                virtual ~Renamer() { }
            };

            memmove(dest, src, n * sizeof(wp<TYPE>));
            TYPE::renameRefs(n, Renamer(dest, src));
        }
    };

    // specialization for moving sp<> and wp<> types.
    // these are used by the [Sorted|Keyed]Vector<> implementations
    // sp<> and wp<> need to be handled specially, because they do not
    // have trivial copy operation in the general case (see RefBase.cpp
    // when DEBUG ops are enabled), but can be implemented very
    // efficiently in most cases.

    template<typename TYPE> inline
        void move_forward_type(sp<TYPE>* d, sp<TYPE> const* s, size_t n) {
        ReferenceMover::move_references(d, s, n);
    }

    template<typename TYPE> inline
        void move_backward_type(sp<TYPE>* d, sp<TYPE> const* s, size_t n) {
        ReferenceMover::move_references(d, s, n);
    }

    template<typename TYPE> inline
        void move_forward_type(wp<TYPE>* d, wp<TYPE> const* s, size_t n) {
        ReferenceMover::move_references(d, s, n);
    }

    template<typename TYPE> inline
        void move_backward_type(wp<TYPE>* d, wp<TYPE> const* s, size_t n) {
        ReferenceMover::move_references(d, s, n);
    }

}  // namespace android

namespace libutilsinternal {
    template <typename T, typename = void>
    struct is_complete_type : std::false_type {};

    template <typename T>
    struct is_complete_type<T, decltype(void(sizeof(T)))> : std::true_type {};
}  // namespace libutilsinternal

namespace std {

    // Define `RefBase` specific versions of `std::make_shared` and
    // `std::make_unique` to block people from using them. Using them to allocate
    // `RefBase` objects results in double ownership. Use
    // `sp<T>::make(...)` instead.
    //
    // Note: We exclude incomplete types because `std::is_base_of` is undefined in
    // that case.

    template <typename T, typename... Args,
        typename std::enable_if<libutilsinternal::is_complete_type<T>::value, bool>::value = true,
        typename std::enable_if<std::is_base_of<android::RefBase, T>::value, bool>::value = true>
    std::shared_ptr<T> make_shared(Args...) {  // SEE COMMENT ABOVE.
        static_assert(!std::is_base_of<android::RefBase, T>::value, "Must use RefBase with sp<>");
    }

    template <typename T, typename... Args,
        typename std::enable_if<libutilsinternal::is_complete_type<T>::value, bool>::value = true,
        typename std::enable_if<std::is_base_of<android::RefBase, T>::value, bool>::value = true>
    std::unique_ptr<T> make_unique(Args...) {  // SEE COMMENT ABOVE.
        static_assert(!std::is_base_of<android::RefBase, T>::value, "Must use RefBase with sp<>");
    }

}  // namespace std


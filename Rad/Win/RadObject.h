#pragma once

namespace rad {
    template <class T>
    class Object
    {
    public:
        typedef T Handle;

        Object(Handle Object = NULL)
            : m_Object(Object)
        {
        }

        virtual ~Object()
        {
            Detach();
        }

        virtual void Detach()
        {
            Release();
        }

        void Attach(Handle Object)
        {
            Detach();
            m_Object = Object;
        }

        Handle Release()
        {
            return std::exchange(m_Object, Handle(NULL));
        }

        virtual void Delete() = 0;

        bool IsValid() const
        {
            return GetHandle() != NULL;
        }

        Handle GetHandle() const
        {
            return m_Object;
        }

        explicit operator bool() const _NOEXCEPT
        {	// test for non-null pointer
            return IsValid();
        }

    private:
        Handle m_Object;
    };

    template <class T>
    class Owner : public T
    {
    public:
        using T::T;

        Owner(typename T::Handle Object = NULL)
            : T(Object)
        {
        }

        Owner(const Owner&) = delete;

        Owner(Owner&& other)
            : T(other.Release())
        {
        }

        virtual void Detach()
        {
            Delete();
            //T::Detach();
        }
    };
}

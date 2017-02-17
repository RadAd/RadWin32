#ifndef WINHANDLE_H
#define WINHANDLE_H

#include <Rad/WinError.h>
#include <memory>

namespace rad
{
    inline void CheckCloseHandle(HANDLE Handle)
    {
        if (Handle != NULL && Handle != INVALID_HANDLE_VALUE)
        {
            if (!::CloseHandle(Handle))
                rad::ThrowWinError();
        }
    }

    template<class H, void(*F)(H)>
    class WinHandleDeleter
    {
    public:
        typedef H pointer;
        void operator()(H Handle)
        {
            F(Handle);
        }
    };

    class WinHandle : private std::unique_ptr<HANDLE, WinHandleDeleter<HANDLE, CheckCloseHandle> >
    {
    public:
        WinHandle(HANDLE Handle = INVALID_HANDLE_VALUE)
            : std::unique_ptr<HANDLE, WinHandleDeleter<HANDLE, CheckCloseHandle> >(Handle)
        {
        }

        void Close()
        {
            release();
        }

        HANDLE Get() const
        {
            return get();
        }

        void Attach(HANDLE Handle = INVALID_HANDLE_VALUE)
        {
            operator=(Handle);
        }

        HANDLE Release()
        {
            return release();
        }

        explicit operator bool() const _NOEXCEPT
        {   // test for non-null pointer
            return (get() != NULL && get() != INVALID_HANDLE_VALUE);
        }
    };
}

#endif

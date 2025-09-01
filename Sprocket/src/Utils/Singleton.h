#ifndef SINGLETON_H
#define SINGLETON_H

namespace Sprocket {

    template <typename T>
    class Singleton {
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

        static T* s_Instance;
    };

    template <class T>
    T* Singleton<T>::s_Instance = nullptr;

}

#endif
#pragma once

#include "Types/Containers/Vector.h"
#include "Types/Ref.h"
#include "o2/Utils/Debug/Assert.h"
#include "o2/Utils/Memory/MemoryManager.h"


namespace o2
{ 
    // ----------------------------------
    // Template class singleton interface
    // ----------------------------------
    template <typename _class_type> 
    class Singleton: public RefCounterable
    {
    public:
        // Default constructor
        Singleton(RefCounter* refCounter);

        // Destructor
        virtual ~Singleton();
                    
        // Return reference to instance
        static _class_type& Instance();
                                        
        // Returns pointer to instance
        static _class_type* InstancePtr();
                                        
        // Initializes singleton
        static void InitializeSingleton();

        // Deinitializes singleton
        static void DeinitializeSingleton();
                                
        // Returns true if singleton was initialized
        static bool IsSingletonInitialzed();

    public:
        static _class_type* mInstance;
    };

    // Returns list of all singletons
    Vector<Ref<RefCounterable>>& GetSingletonsList();

    // Declaring singleton macros
#define DECLARE_SINGLETON(CLASS) template<> CLASS* Singleton<CLASS>::mInstance = nullptr

    // Declaring and initializing singleton macros
#define CREATE_SINGLETON(CLASS)  template<> CLASS* Singleton<CLASS>::mInstance = nullptr; SingletonInitializer<CLASS> gSingleton##CLASS

    template <typename _class_type>
    struct SingletonInitializer
    {
        SingletonInitializer()
        {
            Singleton<_class_type>::InitializeSingleton();
        }
    };

    template <typename _class_type>
    Singleton<_class_type>::Singleton(RefCounter* refCounter):
        RefCounterable(refCounter)
    {
        mInstance = (_class_type*)this;
        GetSingletonsList().Add(DynamicCast<RefCounterable>(Ref(this)));
    }

    template <typename _class_type>
    Singleton<_class_type>::~Singleton()
    {}

    template <typename _class_type>
    _class_type& Singleton<_class_type>::Instance() 
    { 
        Assert(mInstance, "Singleton not initialized");
        return *mInstance; 
    }

    template <typename _class_type>
    _class_type* Singleton<_class_type>::InstancePtr()
    {
        return mInstance;
    }

    template <typename _class_type>
    void Singleton<_class_type>::InitializeSingleton()
    { 
        if (!mInstance) 
            mInstance = mmake<_class_type>().Get(); 
    }

    template <typename _class_type>
    void Singleton<_class_type>::DeinitializeSingleton() 
    { 
        delete mInstance; 
    }

    template <typename _class_type>
    bool Singleton<_class_type>::IsSingletonInitialzed()
    { 
        return mInstance != nullptr;
    }
}


/////////////////// Ordinary Lazy Implementation--Thread Insecurity//////////////////
#include <iostream> // std::cout
#include <mutex>    // std::mutex
#include <pthread.h> // pthread_create

class SingleInstance
{

public:
    // Getting singleton objects
    static SingleInstance *GetInstance();

    // Release the singleton and call it when the process exits
    static void deleteInstance();
    
    // Print singleton address
    void Print();

private:
    // Construct and analyze it as private, prohibit external structure and Deconstruction
    SingleInstance();
    ~SingleInstance();

    // Construct its copy construction and assignment into private functions, forbid external copy and assignment
    SingleInstance(const SingleInstance &signal);
    const SingleInstance &operator=(const SingleInstance &signal);

private:
    // Unique singleton object pointer
    static SingleInstance *m_SingleInstance;
};

//Initialize static member variables
SingleInstance *SingleInstance::m_SingleInstance = NULL;

SingleInstance* SingleInstance::GetInstance()
{

    if (m_SingleInstance == NULL)
    {
        m_SingleInstance = new (std::nothrow) SingleInstance;  // Threads are insecure without locking, and multiple instances are created when threads are concurrent
    }

    return m_SingleInstance;
}

void SingleInstance::deleteInstance()
{
    if (m_SingleInstance)
    {
        delete m_SingleInstance;
        m_SingleInstance = NULL;
    }
}

void SingleInstance::Print()
{
    std::cout << "My instance memory address is:" << this << std::endl;
}

SingleInstance::SingleInstance()
{
    std::cout << "Constructor" << std::endl;
}

SingleInstance::~SingleInstance()
{
    std::cout << "Destructive function" << std::endl;
}
/////////////////// Ordinary Lazy Implementation--Thread Insecurity//////////////////

// Thread function
void *PrintHello(void *threadid)
{
    // Main threads and sub-threads are separated, and they do not interfere with each other. At the same time, sub-threads end and the resources of sub-threads are automatically recovered.
    pthread_detach(pthread_self());

    // Mandatory type conversion of incoming parameters, from untyped pointer to integer pointer, and then read
    int tid = *((int *)threadid);

    std::cout << "Hi, I am a thread ID:[" << tid << "]" << std::endl;

    // Print instance address
    SingleInstance::GetInstance()->Print();

    pthread_exit(NULL);
}

#define NUM_THREADS 5// Number of Threads

int main(void)
{
    pthread_t threads[NUM_THREADS] = {0};
    int indexes[NUM_THREADS] = {0}; // Save the value of i with an array

    int ret = 0;
    int i = 0;

    std::cout << "main() : start ... " << std::endl;

    for (i = 0; i < NUM_THREADS; i++)
    {
        std::cout << "main() : Create threads:[" << i << "]" << std::endl;
        
        indexes[i] = i; //Save the value of i first
        
        // When passing in, you must cast to void* type, that is, untyped pointer
        ret = pthread_create(&threads[i], NULL, PrintHello, (void *)&(indexes[i]));
        if (ret)
        {
            std::cout << "Error:cannot create thread," << ret << std::endl;
            exit(-1);
        }
    }

    // Manual release of single instance resources
    SingleInstance::deleteInstance();
    std::cout << "main() : End! " << std::endl;
    
    return 0;
}













/////////////////// Lazy Man's Realization of Locking//////////////////
class SingleInstance
{

public:
    // Getting a single instance object
    static SingleInstance *&GetInstance();

    //Release single instance and call when process exits
    static void deleteInstance();
    
    // Print instance address
    void Print();

private:
    // Construct and analyze it as private, prohibit external structure and Deconstruction
    SingleInstance();
    ~SingleInstance();

    // Construct its copy construction and assignment into private functions, forbid external copy and assignment
    SingleInstance(const SingleInstance &signal);
    const SingleInstance &operator=(const SingleInstance &signal);

private:
    // Unique single instance object pointer
    static SingleInstance *m_SingleInstance;
    static std::mutex m_Mutex;
};

//Initialize static member variables
SingleInstance *SingleInstance::m_SingleInstance = NULL;
std::mutex SingleInstance::m_Mutex;

SingleInstance *&SingleInstance::GetInstance()
{

    //  The technique of using two if judgement statements here is called double check lock; the advantage is that only when the judgement pointer is empty can it be locked.
    //  Avoid locking every time the GetInstance method is called. After all, the cost of locking is a little high.
    if (m_SingleInstance == NULL) 
    {
        std::unique_lock<std::mutex> lock(m_Mutex); // Lock up
        if (m_SingleInstance == NULL)
        {
            m_SingleInstance = new (std::nothrow) SingleInstance;
        }
    }

    return m_SingleInstance;
}

void SingleInstance::deleteInstance()
{
    std::unique_lock<std::mutex> lock(m_Mutex); // Lock up
    if (m_SingleInstance)
    {
        delete m_SingleInstance;
        m_SingleInstance = NULL;
    }
}

void SingleInstance::Print()
{
    std::cout << "My instance memory address is:" << this << std::endl;
}

SingleInstance::SingleInstance()
{
    std::cout << "Constructor" << std::endl;
}

SingleInstance::~SingleInstance()
{
    std::cout << "Destructive function" << std::endl;
}
/////////////////// Lazy Man's Realization of Locking//////////////////

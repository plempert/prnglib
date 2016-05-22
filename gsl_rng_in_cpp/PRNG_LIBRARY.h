#ifndef PRNG_LIBRARY
#define PRNG_LIBRARY

#include <string>
using std::string;

// This abstract class contains function 
// prototypes for initialization and 
// generation of PRNs.
class PRNG{
protected:
    void* state;
public:
    virtual void set(int seed){}
    virtual void set(unsigned int * seed){}
    virtual void set(const char * seed, int size){}
    virtual void set_type(string){}
    virtual unsigned int get_int(){}
    virtual unsigned long get_long(){}
    virtual double get_double(){}
    
};

#endif


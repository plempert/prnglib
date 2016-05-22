#include "PRNGFactory.h"
#include <iostream>

#include <chrono>
using namespace std::chrono;

int main(){
    tinymt32_t* generator = new tinymt32_t();
    tinymt32_init(generator, 1234);

    PRNG* rng = PRNGFactory::generatePRNG("tinymt");
    rng->set(1234);

    for(int i=0; i<100000; i++){
        unsigned int original_generator = tinymt32_generate_uint32(generator);
        unsigned int PRNGFactory_generator = rng->get_int();
        std::cout << original_generator << " " << PRNGFactory_generator << "\n";
        if(original_generator != PRNGFactory_generator){
            std::cout << "The streams do not match\n";
            exit(0);
        }
    }

    for(int i=0; i<100000; i++){
        double original_generator = tinymt32_generate_float(generator);
        double PRNGFactory_generator = rng->get_double();
        std::cout << original_generator << " " << PRNGFactory_generator << "\n";
        if(original_generator != PRNGFactory_generator){
            std::cout << "The streams do not match\n";
            exit(0);
        }
    }


    high_resolution_clock::time_point t1, t2;
    
    // Measure the time it takes the original generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        unsigned int original_stream = tinymt32_generate_uint32(generator);
    }
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "Original tinymt generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    // Measure the time it takes the new generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        unsigned int PRNG_stream = rng->get_int();
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "PRNGFactory tinymt generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;


    return 0;
}
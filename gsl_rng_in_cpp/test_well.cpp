#include "well/WELL1024a.h"
#include "well/WELL1024a.c"
#include "PRNGFactory.h"
#include <iostream>
#include <time.h>

#include <chrono>
using namespace std::chrono;

#include <string>
using std::string;

int main(){
    unsigned int seed_value[32]; 
    for(int i=0; i<32; i++){
        seed_value[i] = i*2;    // Arbitrary seed value
    }

    InitWELLRNG1024a(seed_value);
    
    PRNG* rng = PRNGFactory::generatePRNG("well");
    rng->set(seed_value);
    
    for(int i=0; i<100000; i++){
        double original_generator = WELLRNG1024a();
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
        unsigned long original_stream = WELLRNG1024a();
    }
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "Original well generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    // Measure the time it takes the new generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        unsigned long PRNG_stream = rng->get_double();
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "PRNGFactory xorgens generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    return 0;
}
#include "xorgens/xorgens.h"
#include "xorgens/xorgens.c"
#include "PRNGFactory.h"
#include <iostream>
#include <time.h>
#include <chrono>
#include "util.h"
#include <string>

using namespace std::chrono;
using std::string;

int main(){
    int seed_value = 26341;  // An arbitrary seed value

    xor4096i(seed_value);   // Seed the original number generator
    
    PRNG* rng = PRNGFactory::generatePRNG("xorgens");   // Seed the PRNGFactory number generator
    rng->set_type("int");   // Determine whether to produce integers or doubles
    rng->set(seed_value);   // Set the seed value

    // Test that the generators produce the same values for 100 million iterations
    for(int i=0; i<10000; i++){
        unsigned long original_generator = xor4096i(0);
        unsigned long PRNGFactory_generator = rng->get_long();
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
        unsigned long original_stream = xor4096i(0);
    }
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "Original xorgens generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    // Measure the time it takes the original generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        unsigned long PRNG_stream = rng->get_long();
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "PRNGFactory xorgens generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;


    // print("Original xorgens generator:");
    // // Seed integer number generator
    // xor4096i(5);
    // // Generator random long
    // print(xor4096i(0));

    // // Seed real number generator
    // xor4096r(10);
    // // Generator random real number
    // print(xor4096r(0));

    // print("PRNGFactory version:");
    // // Allocate memory for a new xorgens RNG.
    // PRNG* rng = PRNGFactory::generatePRNG("xorgens");
    // // Set type to generate longs
    // rng->set_type("int");
    // rng->set(5);
    // print(rng->get_long());
    // print(rng->get_long());

    // PRNG* rng_i = PRNGFactory::generatePRNG("xorgens");
    // // Set type to generate longs
    // rng_i->set_type("int");
    // rng_i->set(5);

    // print(rng->get_long());
    // print(rng_i->get_long());

    // PRNG* rng_r = PRNGFactory::generatePRNG("xorgens");
    // // Set type to generate longs
    // rng_r->set_type("real");
    // rng_r->set(10);
    // print(rng_r->get_double());

    return 0;
}
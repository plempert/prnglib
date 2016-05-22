#include "dsfmt/dsfmt.h"
#include "dsfmt/dsfmt.c"
#include "PRNGFactory.h"
#include <ctime>
#include <iostream>

#include <chrono>
using namespace std::chrono;

int main(){
    init_gen_rand(12345);
    
    PRNG* rng = PRNGFactory::generatePRNG("dsfmt");
    rng->set(12345);

    for(int i=0; i<100000; i++){
        double original_generator = genrand_close_open();
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
        double original_stream = genrand_close_open();
    }
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "Original dsfmt generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    // Measure the time it takes the new generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        double PRNG_stream = rng->get_double();
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "PRNGFactory dsfmt generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;



    return 0;



}
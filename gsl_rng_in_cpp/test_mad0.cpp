#include "MaD0/mad0.h"
#include "PRNGFactory.h"
#include <iostream>

#include <chrono>
using namespace std::chrono;

int main(){

    mad0* gen = new mad0();
    const unsigned char * key = (const unsigned char *)"1234";
    //initialize the generator using the key "abcd"
    initGenFrKey(gen, 4, key);

    PRNG* rng = PRNGFactory::generatePRNG("mad0");
    rng->set("1234", 4);

    for(int i=0; i<100000; i++){
        unsigned int original_generator = generateUint32(gen);
        unsigned int PRNGFactory_generator = rng->get_int();
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
        unsigned int original_stream = generateUint32(gen);
    }
    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "Original mad0 generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

    // Measure the time it takes the new generator to produce 1 billion values
    t1 = high_resolution_clock::now();
    for(int i=0; i<1000000000; i++){
        unsigned int PRNG_stream = rng->get_int();
    }
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    std::cout << "PRNGFactory mad0 generator used "<< duration << " microseconds to generate 1 billion values" << std::endl;

}
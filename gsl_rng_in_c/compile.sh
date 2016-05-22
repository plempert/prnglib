gcc -c testPGL_RNG.c tinymt/pgl_rng_tinymt.c MaD0/pgl_rng_mad0.c pgl_rng.c
gcc testPGL_RNG.o pgl_rng_tinymt.o pgl_rng_mad0.o pgl_rng.o -o main
echo "run using ./main"
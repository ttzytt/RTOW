g++-12 main.cpp -std=c++20 -D AVX2 -o ./build/maindbg -g2 -march=native
g++-12 main.cpp -std=c++20 -D OUT -D AVX2 -o ./build/main -O3 -march=native
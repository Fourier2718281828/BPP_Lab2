#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <stdlib.h>
#include <chrono>

#ifndef NSIZE
#define NSIZE 10ull
#endif

constexpr static std::size_t n = NSIZE;
constexpr static std::size_t fixed_count = 3ull;

static_assert(fixed_count <= n, "Cannot fix more then n values.");

using value_type = double;
using fixed_values = std::array<std::pair<std::size_t, value_type>, fixed_count>;

#ifdef STD_ARRAY
using vector_type = std::array<value_type, n>;
#else
using vector_type = std::vector<value_type>;
#endif

void set_fixed(vector_type& vector, const fixed_values& fixed_values)
{
    for(const auto [index, value] : fixed_values)
    {
        vector[index] = value;
    }
}

vector_type smooth_vector(const vector_type& v, const fixed_values& fixed_values, const std::size_t m) 
{
#ifdef STD_ARRAY
    vector_type prev_iteration;
    std::copy(v.cbegin(), v.cend(), prev_iteration.begin());
#else
    vector_type prev_iteration { v.cbegin(), v.cend() };
#endif

    const std::size_t size = n;

    for (std::size_t iteration = 0; iteration < m; ++iteration) 
    {

#ifdef STD_ARRAY
        vector_type curr_iteration;
#else
        vector_type curr_iteration(size);
#endif 

        curr_iteration[0] = v[0];
        curr_iteration[size - 1] = v[size - 1];

        for (std::size_t i = 1ull; i < size - 1; ++i) 
        {
            curr_iteration[i] = (prev_iteration[i - 1] + prev_iteration[i + 1]) / 2;
        }

        set_fixed(curr_iteration, fixed_values);

#ifdef STD_ARRAY
        std::copy(curr_iteration.cbegin(), curr_iteration.cend(), prev_iteration.begin());
#else
        prev_iteration = std::move(curr_iteration);
#endif 

    }

    return prev_iteration;
}

int main(int argc, const char* argv[]) 
{
    std::size_t m = 1ull;
    std::size_t N = 1ull;

    std::cout << argc << '\n';

    if(argc <= 3)
        m = std::atoi(argv[1]);

    if(argc == 3)
        N = std::atoi(argv[2]);

    if(argc > 3)
        return EXIT_FAILURE;

#ifdef STD_ARRAY
        vector_type vector;
        vector.fill(0.0);
        std::cout << "Array based algorithm.\n";
#else
        vector_type vector(n, 0.0);
        std::cout << "Vector based algorithm.\n";
#endif 

        std::cout << "n = " << n << '\n';
        std::cout << "m = " << m << '\n';
        std::cout << "N = " << N << '\n';

#ifdef OUTPUT_RESULT
        std::cout << "Initial vector: ";
        for(const auto val : vector)
        {
            std::cout << val << ' ';
        }
        std::cout << '\n';
#endif

    fixed_values fixed_values {{ {0, 1.}, {n >> 1, 1.}, {n - 1, 1.} }};
    set_fixed(vector, fixed_values);

    std::cout << "Fixed elements (index, value): ";
    for(const auto [index, value] : fixed_values)
    {
        std::cout << '{' << index << ',' << ' ' << value << '}' << ' ';
    }
    std::cout << '\n';

std::cout << "before\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    vector_type result = smooth_vector(vector, fixed_values, m);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::cout << "after\n";
#ifdef OUTPUT_RESULT
    std::cout << "Result: ";
    for(const auto val : result)
    {
        std::cout << val << ' ';
    }
    std::cout << '\n';
#endif

    std::chrono::duration<double> elapsed_time = end_time - start_time;
    std::cout << "Time taken: " << elapsed_time.count() << "s." << std::endl;

    return EXIT_SUCCESS;
}
 
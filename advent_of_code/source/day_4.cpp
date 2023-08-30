#include <fmt/format.h>

#include <fstream>
#include <filesystem>
#include <sstream>

// DAY 4: Camp Cleanup

auto read_line(auto&& stream)
{
    std::string line {};
    std::getline(stream, line);

    return line;
}

auto count_overlapping_pairs(auto&& inputStream, auto&& fnPredicate)
{
    auto containedCount = 0;

    while (!inputStream.eof())
    {
        auto line = read_line(inputStream);
        
        auto [minimumA, maximumA] = std::pair { 0, 0 };
        auto [minimumB, maximumB] = std::pair { 0, 0 };
                                                                                                                
        std::stringstream lineStream { line };        
                                                                                                                
        auto ignore = '\0';
                                                                                                                
        lineStream >> minimumA >> ignore >> maximumA >> ignore;
        lineStream >> minimumB >> ignore >> maximumB;

        if (fnPredicate(std::pair { minimumA, maximumA }, std::pair { minimumB, maximumB }))
        {
            containedCount += 1;
        }
    }

    return containedCount;
}

void part_1(auto input)
{
    std::ifstream inputStream { input };

    auto fnCountingStrategy = [] (auto pairA, auto pairB)
    {
        auto [minimumA, maximumA] = pairA;
        auto [minimumB, maximumB] = pairB;

        return ((minimumB >= minimumA && maximumB <= maximumA) || (minimumA >= minimumB && maximumA <= maximumB));
    };
    
    fmt::println("part 1: {}", count_overlapping_pairs(inputStream, fnCountingStrategy));
}

auto part_2(auto input)
{
    std::ifstream inputStream { input };

    auto fnCountingStrategy = [] (auto pairA, auto pairB)
    {
        auto [minimumA, maximumA] = pairA;
        auto [minimumB, maximumB] = pairB;

        return maximumA >= minimumB && minimumA <= maximumB;
    };

    fmt::println("part 2: {}", count_overlapping_pairs(inputStream, fnCountingStrategy));
}

int main()
{
    std::filesystem::path input { "inputs/day_4.txt" };
     
    part_1(input);
    part_2(input);
}

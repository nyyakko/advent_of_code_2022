#include <fmt/format.h>
#include <fmt/std.h>

#include <algorithm>
#include <vector>
#include <ranges>
#include <filesystem>
#include <fstream>

// DAY 1: Calorie Counting

auto read_line(auto&& stream)
{
    std::string line {};
    std::getline(stream, line);

    return line;
}

auto calculate_calories(auto&& inputStream)
{
    std::vector<int> elvesSnacks {};

    while (!inputStream.eof())
    {
        auto calories = read_line(inputStream);
                                                     
        if (elvesSnacks.empty() || calories.empty())
        {
            elvesSnacks.push_back(0);
                                                     
            if (calories.empty())
            {
                continue;
            }
        }
        
        elvesSnacks.back() += std::stoi(calories);
    }

    std::ranges::sort(elvesSnacks, std::greater<int>{});
    
    return elvesSnacks;
}

void part_1(auto input)
{
    std::fstream inputStream { input }; 

    auto elvesSnacks = calculate_calories(inputStream);
    fmt::println("part 1: {}", elvesSnacks.front());
}

void part_2(auto input)
{
    std::fstream inputStream { input };

    auto elvesSnacks = calculate_calories(inputStream);
    auto caloriesTotal = 0;

    for (auto calories : elvesSnacks | std::views::take(3))
    {
        caloriesTotal += calories;
    }

    fmt::println("part 2: {}", caloriesTotal);
}

int main()
{ 
    std::filesystem::path input { "inputs/day_1.txt" };

    part_1(input);
    part_2(input);
}

#include <filesystem>
#include <fmt/format.h>

#include <fstream>
#include <ranges>
#include <vector>

// DAY 5: Supply Stacks

using namespace std::literals;

void process_input(auto input, auto fnMoveElement)
{
    std::fstream stream { input };
                                                                    
    while (!stream.eof())
    {
        auto command = ""s;
        auto count   = 0zu;
        auto from    = 0zu;
        auto to      = 0zu;
        
        stream >> command >> count >> command >> from >> command >> to;
                                                                        
        if (!(count && from && to))
        {
            continue;
        }
                                                                        
        fnMoveElement(count, from - 1, to - 1);
    }
}

auto get_crates_back(auto const& crates)
{
    std::string result {};
    
    for (auto& crate : crates)
    {
        result.push_back(crate.back());
    }

    return result;
}

void part_1(auto input, auto crates)
{
    auto fnMoveElement = [&crates] (auto count, auto from, auto to)
    {
        for ([[maybe_unused]]auto _ : std::views::iota(0zu, count))
        {
            crates.at(to).push_back(crates.at(from).back());
            crates.at(from).pop_back();
        }
    };

    process_input(input, fnMoveElement);

    fmt::println("part 1: {}", get_crates_back(crates));
}

void part_2(auto input, auto crates)
{
    auto fnMoveElement = [&crates] (auto count, auto from, auto to)
    {
        std::ranges::copy(crates.at(from) | std::views::drop(crates.at(from).size() - count), std::back_inserter(crates.at(to)));
                                                                                                                                  
        for ([[maybe_unused]]auto _ : std::views::iota(0zu, count))
        {
            crates.at(from).pop_back();
        }
    };
                                                                                                                                  
    process_input(input, fnMoveElement);

    fmt::println("part 2: {}", get_crates_back(crates));
}

int main()
{
    auto input = std::filesystem::path("inputs/day_5.txt");

    std::vector<std::vector<char>> crates
    {
        { 'Z', 'J', 'N', 'W', 'P', 'S' },
        { 'G', 'S', 'T', },
        { 'V', 'Q', 'R', 'L', 'H' },
        { 'V', 'S', 'T', 'D', },
        { 'Q', 'Z', 'T', 'D', 'B', 'M', 'J' },
        { 'M', 'W', 'T', 'J', 'D', 'C', 'Z', 'L' },
        { 'L', 'P', 'M', 'W', 'G', 'T', 'J' },
        { 'N', 'G', 'M', 'T', 'B', 'F', 'Q', 'H' },
        { 'R', 'D', 'G', 'C', 'P', 'B', 'Q', 'W' },
    };
 
    part_1(input, crates);
    part_2(input, crates);
}

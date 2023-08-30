#include <fmt/format.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <ranges>
#include <vector>

// DAY 3: Rucksack Reorganization

auto read_line(auto&& stream)
{
    std::string line {};
    std::getline(stream, line);

    return line;
}

auto get_badge_priority(auto badge)
{
    return badge >= 'a' && badge <= 'z' ? badge - 'a' + 1 : badge - 'A' + 27;
}

auto search_badge(auto groupItems)
{
    for (auto item : groupItems.front())
    {
        auto count = 0;

        for (auto items : groupItems | std::views::drop(1))
        {
            std::ranges::sort(items, std::less<char>{});
            count += std::ranges::binary_search(items, item);

            if (count == 2)
            {
                return item;
            }
        }
    }

    return std::numeric_limits<char>::max();
}

void part_1(auto input)
{
    std::fstream inputStream { input };

    auto prioritiesSum = 0;

    while (!inputStream.eof())
    {
        auto items = read_line(inputStream);
        
        std::span firstHalf { items.begin(), items.size() / 2 };
        std::span secondHalf { std::next(items.begin(), items.size() / 2), items.end() };

        std::ranges::sort(secondHalf, std::less<int>{});
        
        for (auto item : firstHalf)
        {
            auto result = std::ranges::find(secondHalf, item);

            if (result != std::end(secondHalf))
            {
                prioritiesSum += get_badge_priority(*result);
                break;
            }
        }
    }

    fmt::println("part 1: {}", prioritiesSum);
}

void part_2(auto input)
{
    auto fnReadChunk = [] (auto&& stream, auto chunkSize)
    {
        std::vector<std::string> chunks {};
                                                                              
        for ([[maybe_unused]]auto _ : std::views::iota(0zu, chunkSize))
        {
            chunks.push_back(read_line(stream));
        }
                                                                              
        return chunks;
    };

    std::fstream inputStream { input };

    auto prioritiesSum = 0;
    
    while (!inputStream.eof())
    {
        auto groupItems = fnReadChunk(inputStream, 3zu);
        std::ranges::sort(groupItems, std::ranges::less{});
        auto groupBadge = search_badge(groupItems);
                                                                              
        prioritiesSum += get_badge_priority(groupBadge);
    }

    fmt::println("part 2: {}", prioritiesSum);
}

auto main() -> int
{
    std::filesystem::path input { "inputs/day_3.txt" };

    part_1(input);
    part_2(input);
}

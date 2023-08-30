#include <filesystem>
#include <fmt/format.h>

#include <fstream>
#include <list>
#include <map>

// DAY 6: Tuning Trouble

auto has_duplicate(auto&& packet)
{
    std::map<char, std::size_t> bytesSeen {};
                                              
    for (auto byte : packet)
    {
        if ((bytesSeen[byte] += 1) >= 2)
            return true;
    }
                                              
    return false;
}

auto find_packet_marker(auto&& input, auto size)
{
    std::list<char> packet {};
                                                              
    for (auto index = 0zu; index != input.size(); index += 1)
    {
        auto isPacketFull = packet.size() == size;
        auto isPacketUnique = !has_duplicate(packet);
                                                              
        if (!(isPacketFull && isPacketUnique))
        {
            if (!isPacketUnique)
            {
                packet.pop_front();
            }
                                                              
            packet.push_back(input[index]);
            continue;
        }
                                                              
        return index;
    }

    return std::numeric_limits<std::size_t>::max();
}

void part_1(auto&& input)
{
    fmt::println("marker is at: {}", find_packet_marker(input, 4zu)); 
}

void part_2(auto&& input)
{
    fmt::println("marker is at: {}", find_packet_marker(input, 14zu));
}

int main()
{
    std::ifstream inputStream { std::filesystem::path("inputs/day_6.txt") };

    std::string input {};
    inputStream >> input;

    part_1(input);
    part_2(input);
}

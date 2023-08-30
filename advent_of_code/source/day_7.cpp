#include "advent_of_code/color.hpp"

#include <fmt/format.h>

#include <memory>
#include <numeric>
#include <algorithm>
#include <vector>
#include <cassert>
#include <fstream>

// DAY 7: No Space Left On Device

using namespace std::literals;

struct Node
{
    Node(auto&& name, std::weak_ptr<Node> parent):
        name { name }, parent { parent }
    {}

    virtual ~Node() = default;

    constexpr virtual std::string_view get_kind() = 0;

    std::string name;
    std::weak_ptr<Node> parent;
};

struct FileNode : Node
{
    FileNode(auto&& name, std::weak_ptr<Node> parent, std::size_t bytes):
        Node { name, parent }, bytes { bytes }
    {}

    constexpr virtual std::string_view get_kind() override
    {
        return "FILE_NODE";
    }

    std::size_t bytes;
};

struct FolderNode : Node
{
    FolderNode(auto&& name, std::weak_ptr<Node> parent = {}):
        Node { name, parent } {}

    constexpr virtual std::string_view get_kind() override
    {
        return "FOLDER_NODE";
    }

    std::size_t get_total_bytes() const
    {
        auto result = std::accumulate(this->children.begin(), this->children.end(), 0, [] (auto total, auto&& child)
        {
            assert(child && "CHILD WAS NULLPTR");
            
            if (child->get_kind() == "FOLDER_NODE")
            {
                total += std::static_pointer_cast<FolderNode>(child)->get_total_bytes();
            }

            if (child->get_kind() == "FILE_NODE")
            {
                total += std::static_pointer_cast<FileNode>(child)->bytes;
            }

            return total;
        });

        return std::size_t(result);
    }

    std::vector<std::shared_ptr<Node>> children;
};

void print_file_structure_tree(auto const node, std::size_t depth = 0)
{
    assert(node && "NODE WAS NULLPTR");
    assert(node->get_kind() == "FOLDER_NODE" && "NODE WASNT A FOLDER");

    fmt::println("{}{:>{}}{} ({}folder, size={}{})", color::RED, node->name, depth, color::RESET, color::GREEN, node->get_total_bytes(), color::RESET);

    for (auto child : node->children)
    {
        assert(child && "CHILD WAS NULLPTR");

        if (child->get_kind() == "FOLDER_NODE")
        {
            print_file_structure_tree(std::static_pointer_cast<FolderNode>(child), depth + 5);
        }

        if (child->get_kind() == "FILE_NODE")
        {
            auto file = std::static_pointer_cast<FileNode>(child);
            fmt::println("{}{:>{}}{} ({}file, size={}{})", color::YELLOW, file->name, depth + 5, color::RESET, color::GREEN, file->bytes, color::RESET);
        }
    }
}

template <class NodeKind>
auto search_node(auto const& workingFolder, auto nodeName)
{
    auto node = std::ranges::find_if(workingFolder->children, [nodeName] (auto&& children)
    {
        assert(children && "CHILD WAS NULLPTR");
        return children->name == nodeName;
    });

    return node != workingFolder->children.end() ? *node : nullptr;
}

void parse_list_command(auto&& inputStream, auto&& currentWorkingFolder)
{
    while (!inputStream.eof() && inputStream.ignore(1, ' ') && inputStream.peek() != '$')
    {
        auto lhs = ""s;
        auto rhs = ""s;
                                                                                                
        inputStream >> lhs >> rhs;
                                                                                                
        if (lhs == "dir" && !search_node<FolderNode>(currentWorkingFolder, rhs))
        {
            currentWorkingFolder->children.emplace_back(std::make_shared<FolderNode>(FolderNode
            {
                rhs, currentWorkingFolder
            }));
        }
                                                                                                
        if (lhs != "dir" && !search_node<FileNode>(currentWorkingFolder, rhs))
        {
            currentWorkingFolder->children.emplace_back(std::make_shared<FileNode>(FileNode
            {
                rhs, currentWorkingFolder, static_cast<std::size_t>(std::stoi(lhs))
            }));
        }
    }
}

void parse_change_directory_command(auto&& inputStream, auto&& currentWorkingFolder)
{
    auto argument = ""s;
    inputStream >> argument;
                                                                                                          
    if (argument == "..")
    {
        assert(currentWorkingFolder->parent.lock() && "CURRENT WORKING FOLDER'S PARENT WAS NULLPTR");
        currentWorkingFolder = std::static_pointer_cast<FolderNode>(currentWorkingFolder->parent.lock());
    }
                                                                                                          
    if (argument != "/" && argument != "..")
    {
        auto folder = search_node<FolderNode>(currentWorkingFolder, argument);
        assert(folder && "COULDNT FIND SUBFOLDER");
        currentWorkingFolder = std::static_pointer_cast<FolderNode>(folder);
    }
}

std::size_t calculate_file_structure_size(auto const node, std::size_t total = 0)
{
    assert(node && "NODE WAS NULLPTR");
    assert(node->get_kind() == "FOLDER_NODE" && "NODE WASNT A FOLDER");

    auto folderSize = std::static_pointer_cast<FolderNode>(node)->get_total_bytes();

    for (auto child : node->children)
    {
        assert(child && "CHILD WAS NULLPTR");

        if (child->get_kind() == "FOLDER_NODE")
        {
            total += calculate_file_structure_size(std::static_pointer_cast<FolderNode>(child));
        }
    }

    return (folderSize <= 100'000) ? total + folderSize : total;
}

std::size_t find_smallest_folder(auto const node, auto const requiredSize, std::size_t lastSeenSize = 0)
{
    assert(node && "NODE WAS NULLPTR");
    assert(node->get_kind() == "FOLDER_NODE" && "NODE WASNT A FOLDER");

    auto folderSize = std::static_pointer_cast<FolderNode>(node)->get_total_bytes();

    for (auto child : node->children)
    {
        assert(child && "CHILD WAS NULLPTR");

        if (child->get_kind() == "FOLDER_NODE")
        {
            lastSeenSize = find_smallest_folder(std::static_pointer_cast<FolderNode>(child), requiredSize, lastSeenSize);
        }
    }

    if (folderSize >= requiredSize && folderSize <= lastSeenSize)
    {
        return folderSize;
    }

    if (lastSeenSize && lastSeenSize >= requiredSize)
    {
        return lastSeenSize;
    }

    return folderSize;
}

void part_1(auto root)
{
    fmt::println("part 1: {}", calculate_file_structure_size(root));
}

void part_2(auto root)
{
    static constexpr auto AVAILABLE_DISK_TOTAL = 70'000'000;
    static constexpr auto REQUIRED_FREE_SPACE  = 30'000'000;

    auto requiredSize = REQUIRED_FREE_SPACE - (AVAILABLE_DISK_TOTAL - root->get_total_bytes());
    auto result = find_smallest_folder(root, requiredSize);

    fmt::println("part 2: {} ({} required)", result, requiredSize);
}

int main()
{
    auto root = std::make_shared<FolderNode>(FolderNode { "root" });
    std::shared_ptr<FolderNode> currentWorkingFolder { root };
    
    std::fstream inputStream { "inputs/day_7.txt" };

    while (!inputStream.eof())
    {
        auto prefix  = ""s;
        auto command = ""s;

        inputStream >> prefix >> command;

        if (command == "ls")
        {
            parse_list_command(inputStream, currentWorkingFolder);
        }

        if (command == "cd")
        {
            parse_change_directory_command(inputStream, currentWorkingFolder);
        }
    }

    print_file_structure_tree(root);

    part_1(root);
    part_2(root);
}

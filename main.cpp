#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>

using pairs = std::pair<std::wstring, size_t>;

std::wstring reading(const std::string &file_path) {
    std::wifstream file(file_path);
    std::wstring info;
    if (!file) {
        return {};
    }
    std::wstringstream buffer;
    buffer << file.rdbuf();
    info = buffer.str();
    return info;
}

void processing(std::wstring &text) {
    text.erase(std::remove_if(text.begin(), text.end(), [](wchar_t symbol) {
        return !(std::isalpha(symbol) || std::isdigit(symbol) || symbol == ' ');
    }), text.end());
    std::replace_if(text.begin(), text.end(), [](wchar_t symbol) {
        return symbol == L' ';
    }, L'_');
}


std::vector<pairs> sorting(const std::map<std::wstring, size_t> &treasure_map) {
    std::vector<pairs> trigrams(treasure_map.begin(), treasure_map.end());
    std::partial_sort(trigrams.begin(), trigrams.begin() + 10, trigrams.end(),
                      [](const pairs &f, const pairs &l) {
                          return (f.second != l.second) ? f.second > l.second : f.first < l.first;
                      });
    trigrams.resize(10);
    return trigrams;
}

int main(int argc, char **argv) {
    std::wstring data = reading(argv[argc - 1]);
    processing(data);
    std::map<std::wstring, size_t> trigrams_map;
    for (std::size_t i = 0; i < data.size() - 2; ++i) {
        std::wstring trigram = data.substr(i, 3);
        if (trigrams_map.count(trigram) == 0) {
            trigrams_map[trigram] = 1;
        } else {
            trigrams_map[trigram]++;
        }
    }
    std::vector<pairs> top = sorting(trigrams_map);
    int counter = 0;
    for (const auto& trig : top) {
        counter++;
        std::wcout << counter << L") " << trig.first << L" - " << trig.second << "\n";
    }

    return 0;
}
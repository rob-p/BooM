#include "BooMap.hpp"

#include <unordered_map>
#include <set>
#include <iostream>
#include <utility>
#include <random>
#include <vector>
#include <limits>
#include <chrono>

template <typename MapT, typename VerifyMapT, typename KeyT>
void testQuery(MapT& map, VerifyMapT& verifyMap, std::vector<KeyT>& absentKeys) {
    size_t targetSize = verifyMap.size();
std::cerr << "querying for existing keys: ";
    auto tStart = std::chrono::high_resolution_clock::now();
    for (auto kv : verifyMap) {
        auto it = map.find(kv.first);
        if (it->second != kv.second) {
            std::cerr << "True map [" << kv.first << "] = " << kv.second << ", but BooMap had " << it->second << '\n';
        }
    }
    auto tEnd= std::chrono::high_resolution_clock::now();
    std::cerr << "success!\n";
    auto ms = std::chrono::duration<double, std::nano>(tEnd - tStart).count();
    std::cerr << "average time is " << ms / static_cast<double>(targetSize) << " ns per item\n";

    std::cerr << "querying for existing keys (with operator []): ";
    tStart = std::chrono::high_resolution_clock::now();
    for (auto kv : verifyMap) {
        if (map[kv.first] != kv.second) {
            std::cerr << "True map [" << kv.first << "] = " << kv.second << ", but BooMap had " << map[kv.first] << '\n';
        }
    }
    tEnd= std::chrono::high_resolution_clock::now();
    std::cerr << "success!\n";
    ms = std::chrono::duration<double, std::nano>(tEnd - tStart).count();
    std::cerr << "average time is " << ms / static_cast<double>(targetSize) << " ns per item\n";



    std::cerr << "querying for absent keys: ";
    tStart = std::chrono::high_resolution_clock::now();
    for (auto k : absentKeys) {
        auto it = map.find(k);
        if (it != map.end()) {
            std::cerr << "Key " << k << "was absent from the true map but present in the BooMap\n";
        }
    }
    tEnd= std::chrono::high_resolution_clock::now();
    std::cerr << "success!\n";
    ms = std::chrono::duration<double, std::nano>(tEnd - tStart).count();
    std::cerr << "average time is " << ms / static_cast<double>(targetSize) << " ns per item\n";
}


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    BooMap<int64_t, int64_t> map;
    std::unordered_map<int64_t, int64_t> verifyMap;
    std::vector<size_t> presentKeys;
    size_t targetSize = 10000000;
    while (verifyMap.size() < targetSize) {
        auto k = dis(gen);
        if (verifyMap.find(k) == verifyMap.end()) {
            auto v = dis(gen);
            verifyMap[k] = v; 
            map.add(k, v);
            presentKeys.push_back(k);
        }
    }

    map.build();
    std::cerr << "\ndone\n";
    std::set<size_t> absentKeySet;
    size_t numReqAbsent = 10000000;
    while (absentKeySet.size() < numReqAbsent) {
        auto k = dis(gen);
        if (verifyMap.find(k) == verifyMap.end() and 
            absentKeySet.find(k) == absentKeySet.end()) {
            absentKeySet.insert(k);
        }
    }
    std::vector<size_t> absentKeys(absentKeySet.begin(), absentKeySet.end());
    testQuery(map, verifyMap, absentKeys);

    // Testing saving
    std::cerr << "testing save to disk ... ";
    map.save("boom_hash");
    std::cerr << "done\n";
    
    // Testing loading
    std::cerr << "testing loading from disk ... ";
    BooMap<uint64_t, uint64_t> lmap;
    lmap.load("boom_hash");
    std::cerr << "done\n";
    testQuery(lmap, verifyMap, absentKeys);
}

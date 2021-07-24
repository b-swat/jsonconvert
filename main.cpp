#include "json/json.h"
#include <fstream>
#include <iostream>
/** \brief Parse from stream, collect comments and capture error info.
 * Example Usage:
 * $g++ readFromStream.cpp -ljsoncpp -std=c++11 -o readFromStream
 * $./readFromStream
 * // comment head
 * {
 *    // comment before
 *    "key" : "value"
 * }
 * // comment after
 * // comment tail
 */

using  namespace std;

vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main(int argc, char* argv[]) {
    Json::Value root;
    Json::Value new_root;

    std::ifstream ifs;
    ifs.open(argv[1]);
    std::string out_file = argv[1];
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false  ;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &root, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }
    if (root["Items"].isArray()){
        auto ar = root["Items"];
        for(auto &it:ar){
            if (it.isMember("batch_ts")){
                auto b_ts = it["batch_ts"];
                if (b_ts.isMember("S")) {
                    auto str = b_ts["S"].asString();
                    auto vec = split(str, string("_"));
                    it.removeMember("batch_ts");
                    it["batch"]["N"] = (vec[0]);
                    it["ts"]["N"] = (vec[1]);
                }
            }
            new_root["Items"].append(it);

        }
    }
    std::cout << new_root << std::endl;
    return EXIT_SUCCESS;
}
//
// Created by bys on 7/9/20.
//
#ifndef GETOPT_GETOPT_H
#define GETOPT_GETOPT_H

#include <unordered_map>
#include <string_view>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <cstring>

namespace libgetopt{
    class getopt;
    // map ptr
    using args_ptr = std::unique_ptr<getopt>;

    static inline
    void splice(std::vector<std::string>& args, std::string_view arg, char opt){
        if (!args.empty()){
            args.clear();
        }
        if(auto equal_mark = arg.find_first_of(opt); equal_mark != std::string::npos){
            args.emplace_back(arg.substr(0, equal_mark));
            args.emplace_back(arg.substr(equal_mark, 1));
            args.emplace_back(arg.substr(equal_mark + 1, std::string::npos - equal_mark - 2));
        } else {
            args.emplace_back(arg);
        }
    }



    inline
    args_ptr create_commond_args_map() noexcept{
        std::unordered_map<std::string, std::string> map;
        std::vector<std::string> args;
        pid_t pid = getpid();
        {
            std::stringstream ss;
            ss << "/proc/" << pid << "/cmdline";
            if (std::ifstream ifs(ss.str()); ifs.good()) {
                std::stringstream file_stream;
                ifs >> file_stream.rdbuf();
                std::string arg(file_stream.str());

                for (auto beg = 0; beg < arg.length(); ++beg) {
                    auto end = beg;
                    while(arg[end] != '\0'){
                        end++;
                    }
                    args.emplace_back(arg.substr(beg, end - beg + 1));
                    beg = end;
                }
            }
        }
        // add to map
        std::for_each(std::cbegin(args), std::cend(args), [&map](std::string_view arg){
           std::vector<std::string> sub_args;
           splice(sub_args, arg, '=');
            switch (sub_args.size()) {
                case 1 :{
                    map[sub_args[0]] = "true";
                    break;
                }
                case 3 :{
                    map[sub_args[0]] = sub_args[2];
                    break;
                }
                default:
                    break;
            };
        });

        return std::make_unique<getopt>(std::move(map));
    }
    class getopt{
    public:
        getopt() = default;
        getopt(std::unordered_map<std::string, std::string>&& map) noexcept;

        getopt(const getopt&) = delete ;
        getopt& operator=(const getopt&) = delete ;

        bool has(std::string_view key);
        std::string operator[](std::string_view key);

        template<typename T>
        inline
        static T cast(const std::string& value);


    private:
        std::unordered_map<std::string, std::string> value_map;
    };

    getopt::getopt(std::unordered_map<std::string, std::string> &&map) noexcept
    : value_map(map)
    {}

    bool getopt::has(std::string_view key) {
        return value_map.find(std::string(key)) != std::end(value_map);
    }

    std::string getopt::operator[](std::string_view key) {
        if(value_map.count(std::string(key)) == 0){
            std::cerr << key << " error getopt operator[]\n";
            std::abort();
        }
        return value_map.at(std::string (key));
    }

    template<typename T>
    inline
    T getopt::cast(const std::string& value){
        T val;
        return (std::istringstream(value) >> val) ? val : static_cast<T>(-1);
    }
    template<>
    inline
    const char* getopt::cast(const std::string& value){
        return value.c_str();
    }
    template<>
    inline
    char getopt::cast( const std::string &value ) {
        return value.size() == 1 ? (char)(value[0]) : (char)(cast<int>(value));
    }
    template<>
    inline
    signed char getopt::cast( const std::string &value ) {
        return value.size() == 1 ? (signed char)(value[0]) : (signed char)(cast<int>(value));
    }
    template<>
    inline
    unsigned char getopt::cast( const std::string &value ) {
        return value.size() == 1 ? (unsigned char)(value[0]) : (unsigned char)(cast<int>(value));
    }
    template<>
    inline
    std::string getopt::cast(const std::string& value){
        return value;
    }
    template<>
    inline
    bool getopt::cast(const std::string& value){
        return true;
    }



// open interface
    template< typename T>
    inline
    T getarg( const T &defaults, const char *argv ) {
        auto map_ptr = create_commond_args_map();
        return map_ptr->has(argv) ? getopt::cast<T>(map_ptr->operator[](argv)) : defaults;
    }

    template< typename T, typename... Args >
    inline
    T getarg( const T &defaults, const char *arg0, Args... argv ) {
        T t = getarg<T>( defaults, arg0 );
        return t == defaults ? getarg<T>( defaults, argv... ) : t;
    }

    inline
    const char* getarg( const char *defaults, const char *argv ) {
        static auto map_ptr = create_commond_args_map();
        const char* result = defaults;
        if(map_ptr->has(argv)) {
            result = getopt::cast<const char*>(map_ptr->operator[](argv));
        }
        return result;
    }

    template< typename... Args >
    inline
    const char * getarg( const char *defaults, const char *arg0, Args... argv ) {
        const char *t = getarg( defaults, arg0 );
        return t == defaults ? getarg( defaults, argv... ) : t;
    }
};


#endif //GETOPT_GETOPT_H

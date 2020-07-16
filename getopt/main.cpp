#include <iostream>
#include "getopt.h"
#include <cstdio>
#include <cstring>

int main() {
    bool help = libgetopt::getarg( false, "-h", "--help", "-?" );
    int version = libgetopt::getarg( 20, "-v", "--version", "--show-version" );
    int depth = libgetopt::getarg( 1, "-d", "--depth", "--max-depth");
    std::string file = libgetopt::getarg( std::string ("qwe"), "-f", "--file" );
    const char* name = libgetopt::getarg("213", "-name");

    std::cout << help << ',' << file << ',' << version << std::endl;
    return 0;
}

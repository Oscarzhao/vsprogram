#ifndef CCRYPTCONFIG_20120509_H_
#define CCRYPTCONFIG_20120509_H_

#include <utils/Config.h>
#include <string>
using namespace std;

namespace utils
{
    class CCryptConfig
    {
    public:
        static bool load(utils::Configuration& config, string configFilePath);
        static bool save(utils::Configuration& config);

        static bool load(string configFilePath, string& content);
        static bool save(string configFilePath, string content);

        static void test();

    protected:
    private:
    };

    void sha1sum(unsigned char digest[20], const void *input, size_t length);

}

#endif
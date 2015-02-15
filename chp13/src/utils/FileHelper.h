#ifndef _FILE_HELPER_H_
#define _FILE_HELPER_H_
//
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
using namespace std;

namespace utils {

    class FileHelper
    {
    public:

        // used to open binary file
        static bool open(const string filename, string& content)
        {
            FILE *file = fopen(filename.c_str(), "rb");

            if (file == NULL)
                return false;

            fseek(file, 0, SEEK_END);
            int len = ftell(file);
            rewind(file);

            char *buffer = new char[len];
            fread(buffer, sizeof(char), len, file);
            content.assign(buffer, len);
            delete []buffer;

            fclose(file);
            return true;
        }

        // used to open text file
        static bool open(const string file_name, vector<string>& lines)
        {
            ifstream file(file_name.c_str(), ios::in);
            if (!file) 
            {
                return false;
            }

            lines.clear();
            char buffer[buffer_size];

            while (file.getline(buffer, buffer_size, '\n'))
            {
                lines.push_back(buffer);
            }

            return true;
        }
        
        static bool write(const std::string filename, std::string& content)
        {
            FILE* outf = fopen(filename.c_str(), "wb");
            if (outf == NULL)
            {
                return false;
            }

            size_t pos = 0;
            size_t bytesWritten = 0;
            while(pos < content.size())
            {
                bytesWritten = fwrite(&content[pos], 1, content.size() - pos, outf);
                pos += bytesWritten;

                if (bytesWritten <= 0)
                {
                    fclose(outf);
                    return false;
                }
            }
            fclose(outf);

            return true;
        }

        

    private:

        enum { buffer_size = 3000 };
    };

} // namespace MUtils

#endif // _FILE_HELPER_H_
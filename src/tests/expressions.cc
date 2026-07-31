#include <cstddef>
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <utility>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

namespace star
{
    class RegexProcessor
    {
    public:
        RegexProcessor(std::string_view pattern) : m_Pattern(pattern)
        {
            int errornumber;
            size_t erroroffset;

            // 1. Compile the regular expression
            m_RE = pcre2_compile(
                (PCRE2_SPTR)m_Pattern.c_str(),               /* the pattern */
                PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
                PCRE2_DOTALL,                     /* default options */
                &errornumber,          /* for error number */
                &erroroffset,          /* for error offset */
                NULL);                 /* use default compile context */

            if (m_RE == nullptr) {
                PCRE2_UCHAR buffer[256];
                pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
                printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
                return;
            }
        }

        ~RegexProcessor()
        {
            pcre2_code_free(m_RE);
        }

        std::vector<std::pair<size_t, size_t>> GetMatches(std::string_view text)
        {
            size_t *ovector;
            std::vector<std::pair<size_t, size_t>> matches;

            auto match_data = pcre2_match_data_create_from_pattern(m_RE, NULL);

            auto rc = pcre2_match(
                m_RE,                    /* the compiled pattern */
                (PCRE2_SPTR)text.data(),               /* the subject string */
                PCRE2_ZERO_TERMINATED, /* length of the subject string */
                0,                     /* start at offset 0 in the subject */
                0,                     /* default options */
                match_data,            /* block for storing the match result */
                NULL);                 /* use default match context */

            ovector = pcre2_get_ovector_pointer(match_data);
            for(int i = 0; i < rc; i++)
                matches.push_back({ovector[2*i],ovector[2*i+1]});

            pcre2_match_data_free(match_data);
            return matches;
        }

    private:
        std::string m_Pattern;
        pcre2_code *m_RE;

        void GetLastError(int code)
        {
            PCRE2_UCHAR buffer[256];
    
            // Convert numerical code to human-readable string
            int rc = pcre2_get_error_message(code, buffer, sizeof(buffer));
            
            if (rc >= 0) {
                printf("PCRE2 Error (%d): %s\n", code, buffer);
            } else {
                printf("Failed to look up error code %d (Error: %d)\n", code, rc);
            }
        }
    };

    TEST(expressions, identifier)
    {
        RegexProcessor regexp("\\/\\*.*?\\*\\/");
        std::string text = R"(/*This is a long comment
        with line break*/)";
        auto matches = regexp.GetMatches(text);
        ASSERT_EQ(matches.size(), 1);      
    }
}

/*
    * \\/\\*.*?\\*\\/ -> multiline comment
    * ^[a-zA-Z_][a-zA-Z0-9_]*$ identifier
    * \/\/[^\r\n]* -> single line comment
    * ([0-9](?:[0-9]|(?:'[0-9]{3}))*)([ui](?:8|16|32|64)) -> integer match
    * (0[xX][0-9a-fA-F]+)([ui](?:8|16|32|64))? -> hex match
    * ([0-9]*\.[0-9]+(?:[eE][+-]?[0-9]+)?|[0-9]+\.(?:[0-9]+)?(?:[eE][+-]?[0-9]+)?|[0-9]+(?:[eE][+-]?[0-9]+)?)([fF](?:16|32|64))? -> float match
    */

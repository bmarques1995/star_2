#include "RegexProcessor.hh"
#include <utility>

star::RegexProcessor::RegexProcessor(std::string_view pattern) : m_Pattern(pattern)
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

star::RegexProcessor::~RegexProcessor()
{
    pcre2_code_free(m_RE);
}

std::vector<std::pair<size_t, size_t>> star::RegexProcessor::GetMatches(std::string_view text)
{
    std::vector<std::pair<size_t, size_t>> matches;

    size_t *ovector;
    pcre2_match_data* match_data;

    auto matchesCount =AnalyzeExpression(text, &match_data, &ovector);

    for(int i = 0; i < matchesCount; i++)
        matches.push_back({ovector[2*i],ovector[2*i+1]});

    pcre2_match_data_free(match_data);
    return matches;
}

std::pair<size_t, size_t> star::RegexProcessor::GetFirstMatch(std::string_view text)
{
    std::pair<size_t, size_t> pos = {0, 0};
    size_t *ovector;
    pcre2_match_data* match_data;

    auto matchesCount =AnalyzeExpression(text, &match_data, &ovector);

    
    if(matchesCount > 0)
        pos = {ovector[0], ovector[1]};
#ifdef STAR_DEBUG
    else
        GetLastError(matchesCount);
#endif
    pcre2_match_data_free(match_data);
    return pos;
}

void star::RegexProcessor::GetLastError(int code)
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

int star::RegexProcessor::AnalyzeExpression(std::string_view text, pcre2_match_data** match_data, size_t** ovector)
{
    *match_data = pcre2_match_data_create_from_pattern(m_RE, NULL);

    auto rc = pcre2_match(
        m_RE,                    /* the compiled pattern */
        (PCRE2_SPTR)text.data(),               /* the subject string */
        text.size(), /* length of the subject string */
        0,                     /* start at offset 0 in the subject */
        0,                     /* default options */
        *match_data,            /* block for storing the match result */
        NULL);                 /* use default match context */

    *ovector = pcre2_get_ovector_pointer(*match_data);
    return rc;
}
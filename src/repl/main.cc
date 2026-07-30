#include <iostream>
#include <cstdlib>
#include "Star.hh"
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

void PCRE2Tests();

int main(int argc, char** argv)
{
    PCRE2Tests();
    if(argc > 2)
    {
        std::cerr << "Usage: " << argv[0] << " file.st" << std::endl;
        return EXIT_FAILURE;
    }

    if(argc == 2)
    {
        star::Star::RunFile(argv[1]);
    }
    else
    {
        star::Star::RunPrompt();
    }
    
    return EXIT_SUCCESS;
}

void PCRE2Tests()
{
    pcre2_code *re;
    pcre2_match_data *match_data;
    PCRE2_SPTR pattern = (PCRE2_SPTR)"\\d{4}-\\d{2}-\\d{2}"; // Matches YYYY-MM-DD
    PCRE2_SPTR subject = (PCRE2_SPTR)"Today's date is 2026-07-30.";
    
    int errornumber;
    size_t erroroffset;
    int rc;
    size_t *ovector;

    // 1. Compile the regular expression
    re = pcre2_compile(
        pattern,               /* the pattern */
        PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
        0,                     /* default options */
        &errornumber,          /* for error number */
        &erroroffset,          /* for error offset */
        NULL);                 /* use default compile context */

    if (re == NULL) {
        PCRE2_UCHAR buffer[256];
        pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
        printf("PCRE2 compilation failed at offset %d: %s\n", (int)erroroffset, buffer);
        return;
    }

    // 2. Create match data block based on the compiled pattern
    match_data = pcre2_match_data_create_from_pattern(re, NULL);

    // 3. Execute the match
    rc = pcre2_match(
        re,                    /* the compiled pattern */
        subject,               /* the subject string */
        PCRE2_ZERO_TERMINATED, /* length of the subject string */
        0,                     /* start at offset 0 in the subject */
        0,                     /* default options */
        match_data,            /* block for storing the match result */
        NULL);                 /* use default match context */

    // 4. Handle results
    if (rc < 0) {
        if (rc == PCRE2_ERROR_NOMATCH) {
            printf("No match found.\n");
        } else {
            printf("Matching error: %d\n", rc);
        }
        pcre2_match_data_free(match_data);
        pcre2_code_free(re);
        return;
    }

    // 5. Extract match locations
    ovector = pcre2_get_ovector_pointer(match_data);
    printf("Match succeeded at offset %d\n", (int)ovector[0]);

    // Loop through all captured substrings (rc is the count of matched groups)
    for (int i = 0; i < rc; i++) {
        PCRE2_SPTR substring_start = subject + ovector[2*i];
        size_t substring_length = ovector[2*i+1] - ovector[2*i];
        printf(" Group %d: %.*s\n", i, (int)substring_length, (char *)substring_start);
    }

    // 6. Clean up memory
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);
}
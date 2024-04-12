#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <main.h>
#include <string>
#include <vector>

typedef enum FileParserResultStatus_e {
    FILE_PARSER_RESULT_STATUS_OK,                  /* Returns when file parsing was Succesfull */
    FILE_PARSER_RESULT_STATUS_FAILED_TO_OPEN_FILE, /* Returns when failed to open the file */
    FILE_PARSER_RESULT_STATUS_BAD_FILE /* Returns when something wrong was encountered in a file */
} FileParserResultStatus;

class FileParser
{
public:
    FileParser();
    std::vector<LogEntry> getParsedLogEntries(void);
    FileParserResultStatus parseLogFile(const std::string &filename,
                                        std::vector<LogEntry> &parsedLogEntries);
};

#endif // FILEPARSER_H

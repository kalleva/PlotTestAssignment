#include "fileparser.h"
#include <fstream>
#include <string>

FileParser::FileParser() {}

/* parsedLogEntries contain valid data ontly if FILE_PARSER_RESULT_STATUS_OK */
FileParserResultStatus FileParser::parseLogFile(std::string filename,
                                                std::vector<LogEntry> &parsedLogEntries)

{
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        return FILE_PARSER_RESULT_STATUS_FAILED_TO_OPEN_FILE;
    }

    /* I've tried std::istringstream but it was 5+ times slower than sscanf */
    /* On largest file on my laptop sscanf parsing was ~1.5s.
     * Time can be improved with some parsing library */

    float timestamp, value;
    char line[256]; /* Seems enough for all files provided */

    while (infile.getline(line, sizeof(line))) {
        /* TODO: Ask if I shoud actually do something with this data */
        /* Skip lines starting with */
        if (line[0] == '#') {
            continue;
        }
        if (std::sscanf(line, "%f %f", &timestamp, &value) == 2) {
            LogEntry e;
            e.timestamp = timestamp;
            e.value = value;
            parsedLogEntries.push_back(e);
        } else {
            /* TODO: Some error correction */
            /* Skips over empty line */
            continue;
        }
    }

    return FILE_PARSER_RESULT_STATUS_OK;
}

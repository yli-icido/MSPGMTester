#ifndef PGM_COMMON_H_
#define PGM_COMMON_H_

const int PGM_FATAL = -1;           // error cause program to abort
const int PGM_SUCCESS = 0;
const int PGM_FAILURE = 1;          // failed
const int PGM_INVALID_PARAMS = 2;

// both
const int PGM_BUFFER_SIZE = 1024;
const std::string PGM_MULTICAST_ADDRESS = "234.5.6.7";
const int DEFAULT_DATA_DESTINATION_PORT = 5150;

#endif // PGM_COMMON_H_
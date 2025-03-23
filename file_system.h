/**
 * file_system.h
 * Ana başlık dosyası - Güvenli Dosya ve Dizin Yönetim Sistemi
 */

#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#define MAX_PATH_LENGTH 256
#define MAX_CONTENT_LENGTH 1024
#define LOG_FILE "log.txt"

/**
 * Hata kodları
 */
typedef enum {
    SUCCESS = 0,
    ERROR_FILE_EXISTS = -1,
    ERROR_FILE_NOT_FOUND = -2,
    ERROR_DIR_NOT_EMPTY = -3,
    ERROR_PERMISSION_DENIED = -4,
    ERROR_INVALID_ARGUMENT = -5,
    ERROR_UNKNOWN = -99
} ErrorCode;

/**
 * Komut türleri
 */
typedef enum {
    CMD_CREATE_DIR,
    CMD_CREATE_FILE,
    CMD_LIST_DIR,
    CMD_LIST_FILES_BY_EXTENSION,
    CMD_READ_FILE,
    CMD_APPEND_TO_FILE,
    CMD_DELETE_FILE,
    CMD_DELETE_DIR,
    CMD_SHOW_LOGS,
    CMD_HELP,
    CMD_UNKNOWN
} CommandType;

/**
 * Komut yapısı
 */
typedef struct {
    CommandType type;
    char arg1[MAX_PATH_LENGTH];
    char arg2[MAX_PATH_LENGTH];
} Command;

// Standart dosya tanımlayıcıları
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif
#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif
#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

#endif /* FILE_SYSTEM_H */ 
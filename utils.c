/**
 * utils.c
 * Yardımcı fonksiyonların implementasyonları
 */

#include "utils.h"
#include <stdarg.h>

/**
 * Komut satırı argümanlarını işler ve komut yapısını doldurur
 */
ErrorCode parseCommand(int argc, char* argv[], Command* cmd) {
    if (argc < 2 || cmd == NULL) {
        return ERROR_INVALID_ARGUMENT;
    }
    
    // Komut adını belirle
    if (strcmp(argv[1], "createDir") == 0) {
        cmd->type = CMD_CREATE_DIR;
        
        // createDir "folderName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "createFile") == 0) {
        cmd->type = CMD_CREATE_FILE;
        
        // createFile "fileName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "listDir") == 0) {
        cmd->type = CMD_LIST_DIR;
        
        // listDir "folderName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "listFilesByExtension") == 0) {
        cmd->type = CMD_LIST_FILES_BY_EXTENSION;
        
        // listFilesByExtension "folderName" ".txt"
        if (argc != 4) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
        
        strncpy(cmd->arg2, argv[3], MAX_PATH_LENGTH - 1);
        cmd->arg2[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "readFile") == 0) {
        cmd->type = CMD_READ_FILE;
        
        // readFile "fileName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "appendToFile") == 0) {
        cmd->type = CMD_APPEND_TO_FILE;
        
        // appendToFile "fileName" "new content"
        if (argc != 4) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
        
        strncpy(cmd->arg2, argv[3], MAX_PATH_LENGTH - 1);
        cmd->arg2[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "deleteFile") == 0) {
        cmd->type = CMD_DELETE_FILE;
        
        // deleteFile "fileName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "deleteDir") == 0) {
        cmd->type = CMD_DELETE_DIR;
        
        // deleteDir "folderName"
        if (argc != 3) {
            return ERROR_INVALID_ARGUMENT;
        }
        
        strncpy(cmd->arg1, argv[2], MAX_PATH_LENGTH - 1);
        cmd->arg1[MAX_PATH_LENGTH - 1] = '\0';
    } else if (strcmp(argv[1], "showLogs") == 0) {
        cmd->type = CMD_SHOW_LOGS;
        
        // showLogs
        if (argc != 2) {
            return ERROR_INVALID_ARGUMENT;
        }
    } else if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        cmd->type = CMD_HELP;
    } else {
        cmd->type = CMD_UNKNOWN;
        return ERROR_INVALID_ARGUMENT;
    }
    
    return SUCCESS;
}

/**
 * Hata koduna göre uygun hata mesajını döndürür
 */
const char* getErrorMessage(ErrorCode code) {
    switch (code) {
        case SUCCESS:
            return "İşlem başarılı";
        case ERROR_FILE_EXISTS:
            return "Dosya veya dizin zaten mevcut";
        case ERROR_FILE_NOT_FOUND:
            return "Dosya veya dizin bulunamadı";
        case ERROR_DIR_NOT_EMPTY:
            return "Dizin boş değil";
        case ERROR_PERMISSION_DENIED:
            return "İzin reddedildi";
        case ERROR_INVALID_ARGUMENT:
            return "Geçersiz argüman";
        case ERROR_UNKNOWN:
        default:
            return "Bilinmeyen hata";
    }
}

/**
 * Güvenli bir şekilde formatlanmış string oluşturur (snprintf yerine)
 */
int string_format(char* buffer, size_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    int result = 0;
    size_t remaining = size - 1; // null terminator için yer bırak
    char* current = buffer;
    
    // Format stringi işle
    for (const char* p = format; *p != '\0' && remaining > 0; p++) {
        if (*p != '%') {
            *current++ = *p;
            remaining--;
            result++;
            continue;
        }
        
        // % işareti geldi, sonraki karaktere bak
        p++;
        if (*p == '\0') break;
        
        switch (*p) {
            case 's': { // String
                const char* s = va_arg(args, const char*);
                if (s == NULL) s = "(null)";
                
                while (*s != '\0' && remaining > 0) {
                    *current++ = *s++;
                    remaining--;
                    result++;
                }
                break;
            }
            case 'd': { // Integer
                int num = va_arg(args, int);
                char num_str[20]; // int için yeterli
                int is_negative = 0;
                int idx = 0;
                
                // Sayı 0 ise doğrudan '0' yaz
                if (num == 0) {
                    if (remaining > 0) {
                        *current++ = '0';
                        remaining--;
                        result++;
                    }
                    break;
                }
                
                // Negatif sayıları işle
                if (num < 0) {
                    is_negative = 1;
                    num = -num;
                }
                
                // Sayıyı stringe dönüştür (tersten)
                while (num > 0) {
                    num_str[idx++] = '0' + (num % 10);
                    num /= 10;
                }
                
                // Önce - işaretini yaz
                if (is_negative && remaining > 0) {
                    *current++ = '-';
                    remaining--;
                    result++;
                }
                
                // Dönüştürülen stringi tersten yaz
                while (idx > 0 && remaining > 0) {
                    *current++ = num_str[--idx];
                    remaining--;
                    result++;
                }
                break;
            }
            case '%': // % karakterini yaz
                if (remaining > 0) {
                    *current++ = '%';
                    remaining--;
                    result++;
                }
                break;
            default: // Bilinmeyen format parametresi
                if (remaining > 0) {
                    *current++ = '%';
                    remaining--;
                    result++;
                }
                if (remaining > 0) {
                    *current++ = *p;
                    remaining--;
                    result++;
                }
                break;
        }
    }
    
    *current = '\0'; // null terminator
    
    va_end(args);
    return result;
}

/**
 * Yardım mesajını gösterir
 */
void showHelp() {
    const char help_msg[] = 
        "Güvenli Dosya ve Dizin Yönetim Sistemi\n"
        "=======================================\n"
        "Kullanım:\n"
        "  createDir \"folderName\"              - Dizin oluşturma\n"
        "  createFile \"fileName\"              - Dosya oluşturma\n"
        "  listDir \"folderName\"               - Dizin içeriği listeleme\n"
        "  listFilesByExtension \"folderName\" \".txt\" - Belirli uzantıya sahip dosyaları listeleme\n"
        "  readFile \"fileName\"                - Dosya içeriği okuma\n"
        "  appendToFile \"fileName\" \"new content\" - Dosyaya içerik ekleme\n"
        "  deleteFile \"fileName\"              - Dosya silme\n"
        "  deleteDir \"folderName\"             - Boş dizin silme\n"
        "  showLogs                           - Log kayıtlarını gösterme\n"
        "  help                               - Bu yardım mesajını gösterme\n\n";
    
    write(STDOUT_FILENO, help_msg, strlen(help_msg));
}

/**
 * Geçerli zaman damgasını formatlar
 */
void getCurrentTimeStamp(char* buffer, size_t size) {
    time_t rawtime;
    struct tm* timeinfo;
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

/**
 * Dosya veya dizin var mı kontrol eder
 */
int fileExists(const char* path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

/**
 * Yol bir dizin mi kontrol eder
 */
int isDirectory(const char* path) {
    struct stat st;
    
    if (stat(path, &st) != 0) {
        return 0;
    }
    
    return S_ISDIR(st.st_mode);
} 
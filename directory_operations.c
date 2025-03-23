/**
 * directory_operations.c
 * Dizin işlemleri için fonksiyon implementasyonları
 */

#include "directory_operations.h"
#include "logger.h"
#include "utils.h"

/**
 * Dizinin boş olup olmadığını kontrol eder
 */
int isDirEmpty(const char* dirName) {
    DIR* dir;
    struct dirent* entry;
    int count = 0;
                                            /*     struct dirent {
                                                ino_t          d_ino;        dosya düğüm numarası 
                                                off_t          d_off;        dosya offset değeri 
                                                unsigned short d_reclen;     bu kaydın uzunluğu 
                                                unsigned char  d_type;       dosya türü 
                                                char           d_name[256];  dosya adı 
                                            } */
   
    
    // Dizini aç
    dir = opendir(dirName);
    if (dir == NULL) {
        return -1;  // Hata
    }
    
    // Dizin içeriğini oku
    while ((entry = readdir(dir)) != NULL) {
        // "." ve ".." öğelerini atla
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            count++;
            break;
        }
    }
    
    closedir(dir);
    return (count == 0);  // Eğer count 0 ise dizin boştur
}

/**
 * Verilen isimde yeni bir dizin oluşturur
 */
ErrorCode createDir(const char* dirName) {
    char logMsg[MAX_PATH_LENGTH + 50];
    
    // Dizinin veya dosyanın zaten var olup olmadığını kontrol et
    if (fileExists(dirName)) {
        string_format(logMsg, sizeof(logMsg), "Dizin veya dosya zaten mevcut: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_EXISTS;
    }
    
    // Dizini oluştur
    if (mkdir(dirName, 0755) != 0) { // 0755 izni, sahibinin okuyabildiği, yazabildiği ve yürütebildiği, diğerlerinin ise sadece okuyabildiği ve yürütebildiği bir dizin oluşturur
        string_format(logMsg, sizeof(logMsg), "Dizin oluşturma hatası: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Log mesajı oluştur
    string_format(logMsg, sizeof(logMsg), "Dizin oluşturuldu: %s", dirName);
    logMessage(LOG_INFO, logMsg);
    
    return SUCCESS;
}

/**
 * Dizin içeriğini listeler
 */
ErrorCode listDir(const char* dirName) {
    pid_t pid;
    int status;
    char logMsg[MAX_PATH_LENGTH + 50];
    
    // Dizinin var olup olmadığını kontrol et
    if (!fileExists(dirName) || !isDirectory(dirName)) {
        string_format(logMsg, sizeof(logMsg), "Dizin bulunamadı: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Fork ile yeni bir işlem oluştur
    pid = fork();
    
    if (pid < 0) {
        // Fork hatası
        string_format(logMsg, sizeof(logMsg), "Fork hatası: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    } else if (pid == 0) {
        // Çocuk işlemi
        DIR* dir;
        struct dirent* entry;
        
        //printf("Dizin içeriği (%s):\n", dirName);
        //printf("---------------------------------------\n");
        const char* header = "Dizin içeriği (";
        write(STDOUT_FILENO, header, strlen(header));
        write(STDOUT_FILENO, dirName, strlen(dirName));
        const char* separator = "):\n---------------------------------------\n";
        write(STDOUT_FILENO, separator, strlen(separator));
        
        // Dizini aç
        dir = opendir(dirName);
        if (dir == NULL) {
            exit(EXIT_FAILURE);
        }
        
        // Dizin içeriğini oku ve yazdır
        while ((entry = readdir(dir)) != NULL) {
            // "." ve ".." öğelerini atla
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            
            char fullPath[MAX_PATH_LENGTH];
            string_format(fullPath, sizeof(fullPath), "%s/%s", dirName, entry->d_name);
            
            char line[MAX_PATH_LENGTH + 10];
            int len;
            
            if (isDirectory(fullPath)) {
                len = string_format(line, sizeof(line), "[DIR] %s\n", entry->d_name);
                write(STDOUT_FILENO, line, len);
            } else {
                len = string_format(line, sizeof(line), "[FILE] %s\n", entry->d_name);
                write(STDOUT_FILENO, line, len);
            }
        }
        
        const char* footer = "---------------------------------------\n";
        write(STDOUT_FILENO, footer, strlen(footer));
        
        closedir(dir);
        exit(EXIT_SUCCESS);
    } else {
        // Ebeveyn işlemi
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // Log mesajı oluştur
            string_format(logMsg, sizeof(logMsg), "Dizin listelendi: %s", dirName);
            logMessage(LOG_INFO, logMsg);
            return SUCCESS;
        } else {
            string_format(logMsg, sizeof(logMsg), "Dizin listeleme hatası: %s", dirName);
            logMessage(LOG_ERROR, logMsg);
            return ERROR_UNKNOWN;
        }
    }
}

/**
 * Belirtilen uzantıya sahip dosyaları listeler
 */
ErrorCode listFilesByExtension(const char* dirName, const char* extension) {
    pid_t pid;
    int status;
    char logMsg[MAX_PATH_LENGTH + 100];
    
    // Dizinin var olup olmadığını kontrol et
    if (!fileExists(dirName) || !isDirectory(dirName)) {
        string_format(logMsg, sizeof(logMsg), "Dizin bulunamadı: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Fork ile yeni bir işlem oluştur
    pid = fork();
    
    if (pid < 0) {
        // Fork hatası
        string_format(logMsg, sizeof(logMsg), "Fork hatası: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    } else if (pid == 0) {
        // Çocuk işlemi
        DIR* dir;
        struct dirent* entry;
        int found = 0;
        
        char header[MAX_PATH_LENGTH + 100];
        int len = string_format(header, sizeof(header), "'%s' uzantılı dosyalar (%s):\n---------------------------------------\n", extension, dirName);
        write(STDOUT_FILENO, header, len);
        
        // Dizini aç
        dir = opendir(dirName);
        if (dir == NULL) {
            exit(EXIT_FAILURE);
        }
        
        // Dizin içeriğini oku ve filtrele
        while ((entry = readdir(dir)) != NULL) {
            // Dosya adının uzunluğu
            size_t nameLen = strlen(entry->d_name);
            // Uzantı uzunluğu
            size_t extLen = strlen(extension);
            
            // Eğer dosya adı uzantıdan kısaysa, atla
            if (nameLen <= extLen) {
                continue;
            }
            
            // Uzantıyı kontrol et 
            //"belge.txt" + 9 - 4 = "belge.txt" dizisindeki 5. indeksten itibaren olan kısım = ".txt"
            //strcmp(".txt", ".txt") = 0
            if (strcmp(entry->d_name + nameLen - extLen, extension) == 0) {
                char fullPath[MAX_PATH_LENGTH];
                string_format(fullPath, sizeof(fullPath), "%s/%s", dirName, entry->d_name);
                
                // Dizin değilse ve uzantı eşleşiyorsa yazdır
                if (!isDirectory(fullPath)) {
                    char line[MAX_PATH_LENGTH + 2];
                    len = string_format(line, sizeof(line), "%s\n", entry->d_name);
                    write(STDOUT_FILENO, line, len);
                    found = 1;
                }
            }
        }
        
        if (!found) {
            const char* msg = "Bu uzantıya sahip dosya bulunamadı.\n";
            write(STDOUT_FILENO, msg, strlen(msg));
        }
        
        const char* footer = "---------------------------------------\n";
        write(STDOUT_FILENO, footer, strlen(footer));
        
        closedir(dir);
        exit(EXIT_SUCCESS);
    } else {
        // Ebeveyn işlemi
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // Log mesajı oluştur
            string_format(logMsg, sizeof(logMsg), "Uzantıya göre dosyalar listelendi: %s, uzantı: %s", dirName, extension);
            logMessage(LOG_INFO, logMsg);
            return SUCCESS;
        } else {
            string_format(logMsg, sizeof(logMsg), "Uzantıya göre listeleme hatası: %s", dirName);
            logMessage(LOG_ERROR, logMsg);
            return ERROR_UNKNOWN;
        }
    }
}

/**
 * Dizini siler (sadece boş dizinler silinebilir)
 */
ErrorCode deleteDir(const char* dirName) {
    pid_t pid;
    int status;
    char logMsg[MAX_PATH_LENGTH + 50];
    
    // Dizinin var olup olmadığını kontrol et
    if (!fileExists(dirName) || !isDirectory(dirName)) {
        string_format(logMsg, sizeof(logMsg), "Dizin bulunamadı: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Dizinin boş olup olmadığını kontrol et
    if (!isDirEmpty(dirName)) {
        string_format(logMsg, sizeof(logMsg), "Dizin boş değil: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_DIR_NOT_EMPTY;
    }
    
    // Fork ile yeni bir işlem oluştur
    pid = fork();
    
    if (pid < 0) {
        // Fork hatası
        string_format(logMsg, sizeof(logMsg), "Fork hatası: %s", dirName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    } else if (pid == 0) {
        // Çocuk işlemi
        if (rmdir(dirName) != 0) {
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    } else {
        // Ebeveyn işlemi
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // Log mesajı oluştur
            string_format(logMsg, sizeof(logMsg), "Dizin silindi: %s", dirName);
            logMessage(LOG_INFO, logMsg);
            return SUCCESS;
        } else {
            string_format(logMsg, sizeof(logMsg), "Dizin silme hatası: %s", dirName);
            logMessage(LOG_ERROR, logMsg);
            return ERROR_UNKNOWN;
        }
    }
} 
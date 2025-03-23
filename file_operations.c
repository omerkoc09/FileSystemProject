/**
 * file_operations.c
 * Dosya işlemleri için fonksiyon implementasyonları
 */

#include "file_operations.h"
#include "logger.h"
#include "utils.h"

/**
 * Dosyayı kilitleme işlemi
 */
ErrorCode lockFile(int fd) {
    struct flock fl;
    
    fl.l_type = F_WRLCK;  // Yazma kilidi
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;  // Tüm dosyayı kilitle
    fl.l_pid = getpid();
    
    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        return ERROR_PERMISSION_DENIED;
    }
    
    return SUCCESS;
}

/**
 * Dosya kilidini açma işlemi
 */
ErrorCode unlockFile(int fd) {
    struct flock fl;
    
    fl.l_type = F_UNLCK;  // Kilidi kaldır
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_pid = getpid();
    
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        return ERROR_PERMISSION_DENIED;
    }
    
    return SUCCESS;
}

/**
 * Verilen isimde yeni bir dosya oluşturur
 */
ErrorCode createFile(const char* fileName) {
    int fd;
    char logMsg[MAX_PATH_LENGTH + 50];
    char timeBuffer[50];
    ErrorCode result;
    
    // Dosyanın zaten var olup olmadığını kontrol et
    if (fileExists(fileName)) {
        string_format(logMsg, sizeof(logMsg), "Dosya zaten mevcut: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_EXISTS;
    }
    
    // Dosyayı oluştur
    fd = open(fileName, O_WRONLY | O_CREAT | O_EXCL, 0644); // 0644 dosya sahibinin okuyup yazabildiği, diğerlerinin ise sadece okuyabildiği bir izin seviyesi belirtir.
    if (fd == -1) {
        string_format(logMsg, sizeof(logMsg), "Dosya oluşturma hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Dosyayı kilitle
    result = lockFile(fd);
    if (result != SUCCESS) {
        close(fd);
        return result;
    }
    
    // Oluşturulma zamanını dosyaya yaz
    getCurrentTimeStamp(timeBuffer, sizeof(timeBuffer));
    char content[100];
    string_format(content, sizeof(content), "Dosya oluşturulma zamanı: %s\n", timeBuffer);
    
    if (write(fd, content, strlen(content)) == -1) {
        unlockFile(fd);
        close(fd);
        string_format(logMsg, sizeof(logMsg), "Dosyaya yazma hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Kilidi kaldır ve dosyayı kapat
    unlockFile(fd);
    close(fd);
    
    // Log mesajı oluştur
    string_format(logMsg, sizeof(logMsg), "Dosya oluşturuldu: %s", fileName);
    logMessage(LOG_INFO, logMsg);
    
    return SUCCESS;
}

/**
 * Dosya içeriğini okur
 */
ErrorCode readFile(const char* fileName) {
    int fd;
    char buffer[MAX_CONTENT_LENGTH];
    ssize_t bytesRead;
    char logMsg[MAX_PATH_LENGTH + 50];
    pid_t pid;
    
    // Dosyanın mevcut olup olmadığını kontrol et
    if (!fileExists(fileName)) {
        string_format(logMsg, sizeof(logMsg), "Dosya bulunamadı: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Dosyayı aç
    fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        string_format(logMsg, sizeof(logMsg), "Dosya açma hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Child process oluştur
    pid = fork();
    
    if (pid == -1) {
        close(fd);
        return ERROR_UNKNOWN;
    } else if (pid == 0) {
        // Child process - dosyayı oku
        const char* header = "\n--- Dosya İçeriği ---\n";
        write(STDOUT_FILENO, header, strlen(header));
        
        while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
            buffer[bytesRead] = '\0';
            write(STDOUT_FILENO, buffer, bytesRead);
        }
        
        const char* footer = "\n--------------------\n";
        write(STDOUT_FILENO, footer, strlen(footer));
        
        close(fd);
        exit(EXIT_SUCCESS);
    } else {
        // Ana process - child'ın tamamlanmasını bekle
        int status;
        waitpid(pid, &status, 0);
        close(fd);
    }
    
    // Log mesajı oluştur
    string_format(logMsg, sizeof(logMsg), "Dosya okundu: %s", fileName);
    logMessage(LOG_INFO, logMsg);
    
    return SUCCESS;
}

/**
 * Dosyaya içerik ekleme
 */
ErrorCode appendToFile(const char* fileName, const char* content) {
    int fd;
    char logMsg[MAX_PATH_LENGTH + 50];
    ErrorCode result;
    
    // Dosyanın mevcut olup olmadığını kontrol et
    if (!fileExists(fileName)) {
        string_format(logMsg, sizeof(logMsg), "Dosya bulunamadı: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Dosyayı aç
    fd = open(fileName, O_WRONLY | O_APPEND);
    if (fd == -1) {
        string_format(logMsg, sizeof(logMsg), "Dosya açma hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Dosyayı kilitle
    result = lockFile(fd);
    if (result != SUCCESS) {
        close(fd);
        return result;
    }
    
    // İçeriği dosyaya yaz
    if (write(fd, content, strlen(content)) == -1) {
        unlockFile(fd);
        close(fd);
        string_format(logMsg, sizeof(logMsg), "Dosyaya yazma hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    }
    
    // Yeni satır ekle
    const char newline = '\n';
    if (write(fd, &newline, 1) == -1) {
        unlockFile(fd);
        close(fd);
        return ERROR_UNKNOWN;
    }
    
    // Kilidi kaldır ve dosyayı kapat
    unlockFile(fd);
    close(fd);
    
    // Log mesajı oluştur
    string_format(logMsg, sizeof(logMsg), "Dosyaya içerik eklendi: %s", fileName);
    logMessage(LOG_INFO, logMsg);
    
    return SUCCESS;
}

/**
 * Dosya silme
 */
ErrorCode deleteFile(const char* fileName) {
    char logMsg[MAX_PATH_LENGTH + 50];
    pid_t pid;
    
    // Dosyanın mevcut olup olmadığını kontrol et
    if (!fileExists(fileName)) {
        string_format(logMsg, sizeof(logMsg), "Dosya bulunamadı: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Child process oluştur
    pid = fork();
    
    if (pid == -1) {
        string_format(logMsg, sizeof(logMsg), "Fork hatası: %s", fileName);
        logMessage(LOG_ERROR, logMsg);
        return ERROR_UNKNOWN;
    } else if (pid == 0) {
        // Child process - dosyayı sil
        if (unlink(fileName) == -1) {
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else {
        // Ana process - child'ın tamamlanmasını bekle
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
            // Log mesajı oluştur
            string_format(logMsg, sizeof(logMsg), "Dosya silindi: %s", fileName);
            logMessage(LOG_INFO, logMsg);
            return SUCCESS;
        } else {
            string_format(logMsg, sizeof(logMsg), "Dosya silme hatası: %s", fileName);
            logMessage(LOG_ERROR, logMsg);
            return ERROR_UNKNOWN;
        }
    }
} 
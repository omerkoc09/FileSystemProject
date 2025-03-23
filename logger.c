/**
 * logger.c
 * Loglama işlemleri için fonksiyon implementasyonları
 */

#include "logger.h"
#include "utils.h"

/**
 * Log dosyasının varlığını kontrol eder, yoksa oluşturur
 */
ErrorCode initializeLogFile() {
    int fd;
    
    // Log dosyasını aç veya oluştur
    fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) {
        return ERROR_UNKNOWN;
    }
    
    close(fd);
    return SUCCESS;
}

/**
 * Log mesajı ekler
 */
ErrorCode logMessage(LogType type, const char* message) {
    int fd;
    char logBuffer[MAX_CONTENT_LENGTH];
    char timeBuffer[50];
    const char* logTypeStr;
    
    // Log türünü belirle
    switch (type) {
        case LOG_INFO:
            logTypeStr = "INFO";
            break;
        case LOG_ERROR:
            logTypeStr = "ERROR";
            break;
        case LOG_WARNING:
            logTypeStr = "WARNING";
            break;
        default:
            logTypeStr = "UNKNOWN";
    }
    
    // Zaman damgasını al
    getCurrentTimeStamp(timeBuffer, sizeof(timeBuffer));
    
    // Log mesajını oluştur
    string_format(logBuffer, sizeof(logBuffer), "[%s] [%s] %s\n", timeBuffer, logTypeStr, message);
    
    // Log dosyasını aç
    fd = open(LOG_FILE, O_WRONLY | O_APPEND);
    if (fd == -1) {
        return ERROR_UNKNOWN;
    }
    
    // Dosyayı kilitle
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    
    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        close(fd);
        return ERROR_PERMISSION_DENIED;
    }
    
    // Log mesajını yaz
    if (write(fd, logBuffer, strlen(logBuffer)) == -1) {
        // Kilidi kaldır
        fl.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &fl);
        close(fd);
        return ERROR_UNKNOWN;
    }
    
    // Kilidi kaldır
    fl.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &fl);
    
    close(fd);
    return SUCCESS;
}

/**
 * Log dosyasındaki tüm kayıtları gösterir
 */
ErrorCode showLogs() {
    int fd;
    char buffer[MAX_CONTENT_LENGTH];
    ssize_t bytesRead;
    const char* header1 = "Log kayıtları:\n";
    const char* header2 = "=========================================\n";
    
    // Log dosyasının varlığını kontrol et
    if (!fileExists(LOG_FILE)) {
        const char* error_msg = "Log dosyası mevcut değil.\n";
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        return ERROR_FILE_NOT_FOUND;
    }
    
    // Log dosyasını aç
    fd = open(LOG_FILE, O_RDONLY);
    if (fd == -1) {
        const char* error_msg = "Log dosyası açılamadı.\n";
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        return ERROR_UNKNOWN;
    }
    
    // Log dosyasını oku ve yazdır
    write(STDOUT_FILENO, header1, strlen(header1));
    write(STDOUT_FILENO, header2, strlen(header2));
    
    while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytesRead] = '\0';
        write(STDOUT_FILENO, buffer, bytesRead);
    }
    
    write(STDOUT_FILENO, header2, strlen(header2));
    
    close(fd);
    
    // Başarı durumunu log dosyasına kaydet
    logMessage(LOG_INFO, "Log kayıtları görüntülendi");
    
    return SUCCESS;
} 
/**
 * logger.h
 * Loglama işlemleri için fonksiyon tanımlamaları
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "file_system.h"

/**
 * Log türleri
 */
typedef enum {
    LOG_INFO,
    LOG_ERROR,
    LOG_WARNING
} LogType;

/**
 * Log mesajı ekler
 * 
 * @param type Log türü
 * @param message Log mesajı
 * @return Başarı durumu
 */
ErrorCode logMessage(LogType type, const char* message);

/**
 * Log dosyasındaki tüm kayıtları gösterir
 * 
 * @return Başarı durumu
 */
ErrorCode showLogs();

/**
 * Log dosyasının varlığını kontrol eder, yoksa oluşturur
 * 
 * @return Başarı durumu
 */
ErrorCode initializeLogFile();

#endif /* LOGGER_H */ 
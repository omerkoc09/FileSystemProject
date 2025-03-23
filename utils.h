/**
 * utils.h
 * Yardımcı fonksiyonlar
 */

#ifndef UTILS_H
#define UTILS_H

#include "file_system.h"

/**
 * Güvenli bir şekilde formatlanmış string oluşturur (snprintf yerine)
 * 
 * @param buffer Hedef buffer
 * @param size Buffer boyutu
 * @param format Format string
 * @param ... Format argümanları
 * @return Oluşturulan string'in uzunluğu
 */
int string_format(char* buffer, size_t size, const char* format, ...);

/**
 * Komut satırı argümanlarını işler ve komut yapısını doldurur
 * 
 * @param argc Argüman sayısı
 * @param argv Argüman dizisi
 * @param cmd Doldurulacak komut yapısı
 * @return Başarı durumu
 */
ErrorCode parseCommand(int argc, char* argv[], Command* cmd);

/**
 * Hata koduna göre uygun hata mesajını döndürür
 * 
 * @param code Hata kodu
 * @return Hata mesajı
 */
const char* getErrorMessage(ErrorCode code);

/**
 * Yardım mesajını gösterir
 */
void showHelp();

/**
 * Geçerli zaman damgasını formatlar
 * 
 * @param buffer Formatlanmış zamanın yazılacağı buffer
 * @param size Buffer boyutu
 */
void getCurrentTimeStamp(char* buffer, size_t size);

/**
 * Dosya veya dizin var mı kontrol eder
 * 
 * @param path Kontrol edilecek yol
 * @return 1: Var, 0: Yok
 */
int fileExists(const char* path);

/**
 * Yol bir dizin mi kontrol eder
 * 
 * @param path Kontrol edilecek yol
 * @return 1: Dizin, 0: Değil
 */
int isDirectory(const char* path);

#endif /* UTILS_H */ 
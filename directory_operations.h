/**
 * directory_operations.h
 * Dizin işlemleri için fonksiyon tanımlamaları
 */

#ifndef DIRECTORY_OPERATIONS_H
#define DIRECTORY_OPERATIONS_H

#include "file_system.h"

/**
 * Verilen isimde yeni bir dizin oluşturur
 * 
 * @param dirName Oluşturulacak dizinin adı
 * @return Başarı durumu
 */
ErrorCode createDir(const char* dirName);

/**
 * Dizin içeriğini listeler
 * 
 * @param dirName Listelenecek dizinin adı
 * @return Başarı durumu
 */
ErrorCode listDir(const char* dirName);

/**
 * Belirtilen uzantıya sahip dosyaları listeler
 * 
 * @param dirName Dizin adı
 * @param extension Dosya uzantısı (örn: ".txt")
 * @return Başarı durumu
 */
ErrorCode listFilesByExtension(const char* dirName, const char* extension);

/**
 * Dizini siler (sadece boş dizinler silinebilir)
 * 
 * @param dirName Silinecek dizinin adı
 * @return Başarı durumu
 */
ErrorCode deleteDir(const char* dirName);

/**
 * Dizinin boş olup olmadığını kontrol eder
 * 
 * @param dirName Kontrol edilecek dizinin adı
 * @return 1: Boş, 0: Dolu, -1: Hata
 */
int isDirEmpty(const char* dirName);

#endif /* DIRECTORY_OPERATIONS_H */ 
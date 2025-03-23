/**
 * file_operations.h
 * Dosya işlemleri için fonksiyon tanımlamaları
 */

#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "file_system.h"

/**
 * Verilen isimde yeni bir dosya oluşturur
 * 
 * @param fileName Oluşturulacak dosyanın adı
 * @return Başarı durumu
 */
ErrorCode createFile(const char* fileName);

/**
 * Dosya içeriğini okur
 * 
 * @param fileName Okunacak dosyanın adı
 * @return Başarı durumu
 */
ErrorCode readFile(const char* fileName);

/**
 * Dosyaya yeni içerik ekler
 * 
 * @param fileName İçerik eklenecek dosyanın adı
 * @param content Eklenecek içerik
 * @return Başarı durumu
 */
ErrorCode appendToFile(const char* fileName, const char* content);

/**
 * Dosyayı siler
 * 
 * @param fileName Silinecek dosyanın adı
 * @return Başarı durumu
 */
ErrorCode deleteFile(const char* fileName);

/**
 * Dosyayı kilitleme işlemi
 * 
 * @param fd Dosya tanımlayıcısı
 * @return Başarı durumu
 */
ErrorCode lockFile(int fd);

/**
 * Dosya kilidini açma işlemi
 * 
 * @param fd Dosya tanımlayıcısı
 * @return Başarı durumu
 */
ErrorCode unlockFile(int fd);

#endif /* FILE_OPERATIONS_H */ 
# Güvenli Dosya ve Dizin Yönetim Sistemi

Bu proje, Linux ortamında C programlama dili kullanılarak geliştirilmiş güvenli bir dosya ve dizin yönetim sistemidir. Bu sistem, kullanıcıların dosya ve dizin oluşturma, listeleme, içerik okuma ve güncelleme, silme ve işlem kayıtlarını görüntüleme işlemlerini gerçekleştirmesine olanak tanır.

## Özellikler

- Dosya ve dizin oluşturma
- Dosya ve dizin listeleme (uzantıya göre filtreleme desteği ile)
- Dosya içeriği okuma ve yeni içerik ekleme
- Dosya ve dizin silme (boş dizinler için)
- İşlem kayıtları tutma ve görüntüleme
- Eşzamanlı yazma işlemlerine karşı dosya kilitleme
- Yardım ve kullanım rehberi

## Kurulum

Proje dosyalarını indirdikten sonra, aşağıdaki komutla derleyebilirsiniz:

```bash
make
```

Bu komut, gerekli tüm dosyaları derleyecek ve çalıştırılabilir bir dosya oluşturacaktır.

## Kullanım

Program aşağıdaki komutlarla kullanılabilir:

- Dizin oluşturma: `./file_system createDir "folderName"`
- Dosya oluşturma: `./file_system createFile "fileName"`
- Dizin içeriği listeleme: `./file_system listDir "folderName"`
- Belirli uzantıya sahip dosyaları listeleme: `./file_system listFilesByExtension "folderName" ".txt"`
- Dosya içeriği okuma: `./file_system readFile "fileName"`
- Dosyaya içerik ekleme: `./file_system appendToFile "fileName" "new content"`
- Dosya silme: `./file_system deleteFile "fileName"`
- Boş dizin silme: `./file_system deleteDir "folderName"`
- Log kayıtlarını gösterme: `./file_system showLogs`
- Yardım görüntüleme: `./file_system help`

## Sınırlamalar

- Boş olmayan dizinler silinemez
- Dosya ve dizinler benzersiz isimlere sahip olmalıdır
- Listeleme ve silme işlemleri ayrı işlemlerde (fork) gerçekleştirilir

## Örnek Test Senaryosu

```bash
# Test dizini oluştur
./file_system createDir "testDir"

# Örnek dosya oluştur
./file_system createFile "testDir/example.txt"

# Dosyaya içerik ekle
./file_system appendToFile "testDir/example.txt" "Bu bir örnek içeriktir."

# Dizin içeriğini listele
./file_system listDir "testDir"

# Dosya içeriğini oku
./file_system readFile "testDir/example.txt"

# İkinci bir örnek dosya oluştur
./file_system createFile "testDir/another.txt"

# Uzantıya göre dosyaları listele
./file_system listFilesByExtension "testDir" ".txt"

# Dosyayı sil
./file_system deleteFile "testDir/example.txt"

# Log kayıtlarını görüntüle
./file_system showLogs

# Dizini sil
./file_system deleteDir "testDir"
```

## Temizleme

Oluşturulan çalıştırılabilir dosyaları ve derlenmiş nesneleri kaldırmak için:

```bash
make clean
``` 
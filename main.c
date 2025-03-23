/**
 * main.c
 * Güvenli Dosya ve Dizin Yönetim Sistemi - Ana Program
 */

#include "file_system.h"
#include "file_operations.h"
#include "directory_operations.h"
#include "logger.h"
#include "utils.h"

/**
 * Ana program fonksiyonu
 */
int main(int argc, char* argv[]) {
    Command cmd;
    ErrorCode result;
    const char* unknownCmdMsg = "Bilinmeyen komut!\n";
    char errorMsg[MAX_CONTENT_LENGTH];

    // Log dosyasını başlat
    result = initializeLogFile();
    if (result != SUCCESS) {
        string_format(errorMsg, sizeof(errorMsg), "Log dosyası başlatılamadı: %s\n", getErrorMessage(result));
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        return EXIT_FAILURE;
    }

    // Eğer argüman yoksa yardım mesajını göster
    if (argc < 2) {
        showHelp();
        return EXIT_SUCCESS;
    }

    // Komut satırı argümanlarını işle
    result = parseCommand(argc, argv, &cmd);
    if (result != SUCCESS) {
        string_format(errorMsg, sizeof(errorMsg), "Komut işlenemedi: %s\n", getErrorMessage(result));
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        return EXIT_FAILURE;
    }

    // Komuta göre işlemleri gerçekleştir
    switch (cmd.type) {
        case CMD_CREATE_DIR:
            result = createDir(cmd.arg1);
            break;
        case CMD_CREATE_FILE:
            result = createFile(cmd.arg1);
            break;
        case CMD_LIST_DIR:
            result = listDir(cmd.arg1);
            break;
        case CMD_LIST_FILES_BY_EXTENSION:
            result = listFilesByExtension(cmd.arg1, cmd.arg2);
            break;
        case CMD_READ_FILE:
            result = readFile(cmd.arg1);
            break;
        case CMD_APPEND_TO_FILE:
            result = appendToFile(cmd.arg1, cmd.arg2);
            break;
        case CMD_DELETE_FILE:
            result = deleteFile(cmd.arg1);
            break;
        case CMD_DELETE_DIR:
            result = deleteDir(cmd.arg1);
            break;
        case CMD_SHOW_LOGS:
            result = showLogs();
            break;
        case CMD_HELP:
            showHelp();
            return EXIT_SUCCESS;
        case CMD_UNKNOWN:
            write(STDERR_FILENO, unknownCmdMsg, strlen(unknownCmdMsg));
            showHelp();
            return EXIT_FAILURE;
        default:
            write(STDERR_FILENO, unknownCmdMsg, strlen(unknownCmdMsg));
            showHelp();
            return EXIT_FAILURE;
    }

    // İşlem sonucunu kontrol et
    if (result != SUCCESS) {
        string_format(errorMsg, sizeof(errorMsg), "Hata: %s\n", getErrorMessage(result));
        write(STDERR_FILENO, errorMsg, strlen(errorMsg));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
} 
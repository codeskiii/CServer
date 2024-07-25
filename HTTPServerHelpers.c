/*
I just copierd all includes from HTTPServer.c
*/

#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ServerSettingsPath "HTTPServerRoutes.txt"

char *read_file(const char *path, long *out_size) {
    FILE *file = fopen(path, "rb");
    char *buffer = NULL;
    long file_size;

    if (file) {
        fseek(file, 0, SEEK_END);
        file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char*)malloc(file_size);
        if (buffer) {
            fread(buffer, 1, file_size, file);
        }
        fclose(file);
    }

    *out_size = file_size;
    return buffer;
}
/*
// Function to split the file content into a list of lines, and each line into words
char*** load_settings(const char *filePath, int *numLines) {
    long settingsSize;
    char *rawSettingsFile = read_file(filePath, &settingsSize);

    if (!rawSettingsFile) return NULL;

    // Allocate a maximum number of lines and words (for simplicity)
    int maxLines = 100000; // 100_000 lines
    int maxWordsPerLine = 2;

    char ***routingSettings = malloc(maxLines * sizeof(char**));
    if (!routingSettings) {
        perror("Failed to allocate memory");
        free(rawSettingsFile);
        return NULL;
    }

    int currentLine = 0;
    int currentWord = 0;

    char *lineStart = rawSettingsFile;
    char *lineEnd = NULL;

    while ((lineEnd = strchr(lineStart, '\n')) != NULL) {
        routingSettings[currentLine] = malloc(maxWordsPerLine * sizeof(char*));
        if (!routingSettings[currentLine]) {
            perror("Failed to allocate memory");
            free(rawSettingsFile);
            return NULL;
        }

        char *wordStart = lineStart;
        char *wordEnd = NULL;

        currentWord = 0;

        while ((wordEnd = strpbrk(wordStart, " \t\n")) != NULL && wordEnd < lineEnd) {
            if (wordStart != wordEnd) {
                routingSettings[currentLine][currentWord] = strndup(wordStart, wordEnd - wordStart);
                if (!routingSettings[currentLine][currentWord]) {
                    perror("Failed to allocate memory");
                    free(rawSettingsFile);
                    return NULL;
                }
                currentWord++;
            }
            wordStart = wordEnd + 1;
        }

        if (wordStart < lineEnd) {
            routingSettings[currentLine][currentWord] = strndup(wordStart, lineEnd - wordStart);
            if (!routingSettings[currentLine][currentWord]) {
                perror("Failed to allocate memory");
                free(rawSettingsFile);
                return NULL;
            }
            currentWord++;
        }

        routingSettings[currentLine][currentWord] = NULL; // End of words for this line
        currentLine++;

        lineStart = lineEnd + 1;
    }

    // Handle the last line (if no newline at the end of the file)
    if (*lineStart) {
        routingSettings[currentLine] = malloc(maxWordsPerLine * sizeof(char*));
        if (!routingSettings[currentLine]) {
            perror("Failed to allocate memory");
            free(rawSettingsFile);
            return NULL;
        }

        char *wordStart = lineStart;
        char *wordEnd = NULL;
        currentWord = 0;

        while ((wordEnd = strpbrk(wordStart, " \t\n")) != NULL) {
            if (wordStart != wordEnd) {
                routingSettings[currentLine][currentWord] = strndup(wordStart, wordEnd - wordStart);
                if (!routingSettings[currentLine][currentWord]) {
                    perror("Failed to allocate memory");
                    free(rawSettingsFile);
                    return NULL;
                }
                currentWord++;
            }
            wordStart = wordEnd + 1;
        }

        if (wordStart) {
            routingSettings[currentLine][currentWord] = strndup(wordStart, strlen(wordStart));
            if (!routingSettings[currentLine][currentWord]) {
                perror("Failed to allocate memory");
                free(rawSettingsFile);
                return NULL;
            }
            currentWord++;
        }

        routingSettings[currentLine][currentWord] = NULL; // End of words for this line
        currentLine++;
    }

    routingSettings[currentLine] = NULL; // End of lines
    *numLines = currentLine;

    free(rawSettingsFile);

    return routingSettings;
} 
*/ 
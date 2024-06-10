#include "okanoyo/okstring.h"

char* getline(char *line, char *buff) {
    s32 i = 0;

    if (*buff == '\0') {
        return (char *)-1;
    }
    while (1) {
        line[i] = buff[i];
        i++;
        if (buff[i - 1] == '\n' || buff[i - 1] == '\0') {
            break;
        }
    }
    line[i] = '\0';
    return buff + i;
}

char* findword(char *word, char *buff) {
    s32 i = 0;
    s32 j;
    s32 len = strlen(word);

    while (buff[i] != '\n' && buff[i] != '\0') {
        j = 0;
        while (buff[i] == word[j]) {
            i++;
            j++;
            if (j == len && (i == 0 || buff[i - j - 1] == '\0' || buff[i - j - 1] == ' ' || buff[i - j - 1] == '\t' || buff[i - j - 1] == '\t' || buff[i - j - 1] == '\n')) {
                if (buff[i] == '\0' || buff[i] == ' ' || buff[i] == '\t' || buff[i] == '\n') {
                    return buff + (i - len);
                }
            }
        }
        i++;
    }

    return (char *)-1;
}

void stripstr(char *obst, char *word) {
    s32 i;
    s32 j;
    s32 k;
    char tmp[256];
    s32 len = strlen(obst);

    k = 0;
    i = 0;

    while (word[i] != '\n' && word[i] != '\0') {
        j = 0;
        while (word[i] == obst[j] && j < len) {
            i++;
            j++;
        }

        if (j == 0) {
            tmp[k++] = word[i++];
        }
    }

    for (i = 0; i < k; i++) {
        word[i] = tmp[i];
    }
    word[i] = '\0';
}

void replacestr(char *obst, char *inst, char *word) {
    s32 i;
    s32 j;
    s32 k;
    s32 l;
    char tmp[256];
    s32 len = strlen(obst);

    k = 0;
    i = 0;

    while (word[i] != '\n' && word[i] != '\0') {
        j = 0;
        while (word[i] == obst[j] && j < len) {
            i++;
            j++;
        }

        l = 0;
        if (j != 0) {
            // why are you calling strlen on an immutable string in a while loop???
            while (strlen(inst) > l ) {
                tmp[k++] = inst[l++];
            }
        } else {
            tmp[k++] = word[i++];
        }
    }

    for (i = 0; i < k; i++) {
        word[i] = tmp[i];
    }
    word[i] = '\0';
}

char* getlineword(char *word, char *line) {
    s32 i = 0;
    s32 j = 0;

    while (line[i] == '\t' || line[i] == ' ') {
        i++;
    }

    if (line[i] == '\n' || line[i] == '\0') {
        return (char *)-1;
    }

    while (line[i] != '\n' && line[i] != '\0' && line[i] != '\t' && line[i] != ' ' && line[i] != '\r') {
        word[j++] = line[i++];
    }

    word[j] = '\0';
    return line + i;
}

char* getlineword2(char *word, char *line) {
    s32 i = 0;
    s32 j = 0;

    while (line[i] == '\t' || line[i] == ' ') {
        i++;
    }

    if (line[i] == '\n' || line[i] == '\0') {
        return (char *)-1;
    }

    while (line[i] != '\n' && line[i] != '\0' && line[i] != '\t' && line[i] != ' ' && line[i] != '\r') {
        word[j++] = line[i++];
    }
    word[j] = '\0';

    // why is line[i] != '\0' here twice
    while (line[i] != '\n' && line[i] != '\0' && line[i] != '\t' && line[i] != ' ' && line[i] != '\r' && line[i] != '\0') {
        i++;
    }

    return line + i;
}

s32 htoi(char *str) {
    s32 len = strlen(str);
    s32 ret = 0;
    s32 test = 0;
    s32 i;
    s32 val;

    if (len > 4) {
        test = *str;
        if (test >= 'A') {
            test -= 7;
        }
    }

    for (i = 0; i < len; i++) {
        val = str[i];
        if (val != '0') {
            if (val >= 'A') {
                val -= 7;
            }
            val = (val & 0xF) << (len - i - 1) * 4;
            ret |= val;
        }

    }

    if (test & 7) {
        for (i = 0; i < 8 - len; i++) {
            ret |= 0xF << (len - i - 1) * 4;
        }
    }

    return ret;
}

s32 stoi(char *str) {
    if (str[0] == '0' && str[1] == 'x') {
        return htoi(str + 2);
    } else {
        return atoi(str);
    }
}
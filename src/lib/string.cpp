unsigned long long strlen(const char* str)
{
    unsigned long long len = 0;
    for (len = 0; str[len] != 0; len++);
    return len;
}

bool strcmp(const char* str1, const char* str2)
{
    unsigned long long len = strlen(str1);

    if (strlen(str2) != len)
        return false;

    for (unsigned long long i = 0; i < len; i++)
        if (str1[i] != str2[i])
            return false;

    return true;
}

void strcpy(char* str1, char* str2)
{
    for(unsigned long long i = 0; i < strlen(str2); i++)
    {
        str1[i] = str2[i];
    }
    str1[strlen(str2)] = 0;
}

void strcat(char* str1, char* str2)
{
    unsigned long long len = strlen(str1);
    for(unsigned long long i = len; i < (len + strlen(str2)); i++)
    {
        str1[i] = str2[i-len];
    }
    str1[strlen(str2)+len] = 0;
}
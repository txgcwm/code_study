#include <string>

int DecryptBase64Aes(const char *key, const char *iv, const char *enc, char *value);

int EncypptBase64Aes(const char *key, const char *iv, const char *text, std::string &bval);
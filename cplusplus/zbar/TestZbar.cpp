#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string>

#include <zbar.h>
#include <zbar/Image.h>
#include <zbar/Scanner.h>



bool ScanQrcode(zbar::ImageScanner& scanner, int w, int h, char* y8data)
{
    zbar::Image image(w, h, "Y800", y8data, w * h);

    int retVal = scanner.scan(image);
    if (retVal > 0) {
        printf("qrcode  retVal:%d \n", retVal);
        for (zbar::Image::SymbolIterator symbol = image.symbol_begin();
             symbol != image.symbol_end(); ++symbol) {
            std::string const& qrcode_data = symbol->get_data();
		}
    } else if (retVal == 0) {
        printf("qrcode failed: No symbold found!\n");
    } else if (retVal < 0) {
        printf("qrcode failed: Zbar scan error!\n");
    }

    return false;
}

int ParseFile(char* filename)
{
    int width = 320;
    int height = 180;
    char buf[90000] = {0};
    zbar::ImageScanner zbarScanner;
    int fd = open(filename, O_RDONLY);

    if(fd == -1) {
        printf("error is %s\n", strerror(errno));
        return -1;
    }

    read(fd, buf, sizeof(buf) - 1);

    ScanQrcode(zbarScanner, width, height, buf);

    close(fd);

    return 0;
}

int main(int argc, char** argv)
{
    ParseFile(argv[1]);

    return 0;
}



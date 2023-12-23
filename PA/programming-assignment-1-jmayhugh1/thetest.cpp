#include <iostream>
#include <fstream>

int main() {
    std::ifstream sourceFile("./BIMDC/1.csv", std::ios::binary);
    if (!sourceFile) {
        std::cerr << "Failed to open source file." << std::endl;
        return 1;
    }

    // Other code for transferring the file.
    std::ofstream destinationFile("./BIMDC/testlocation.txt", std::ios::binary);
if (!destinationFile) {
    std::cerr << "Failed to open destination file." << std::endl;
    return 1;

}
const int bufferSize = 1024; // You can adjust the buffer size as needed.
char buffer[bufferSize];

while (!sourceFile.eof()) {
    sourceFile.read(buffer, bufferSize);
    destinationFile.write(buffer, sourceFile.gcount());
}


    sourceFile.close();
    destinationFile.close();
    return 0;
}

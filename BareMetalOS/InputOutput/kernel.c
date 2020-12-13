void printString(char *chars) {
 }

main(int argc, char* argv[])
{
putInMemory(0xB000, 0x8140, 'H');
putInMemory(0xB000, 0x8141, 0x7);
putInMemory(0xB000, 0x81E0, 'E');
putInMemory(0xB000, 0x81E1, 0x7);
while(1);
}
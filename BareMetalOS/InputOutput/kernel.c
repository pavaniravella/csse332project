void printString(char *chars) ;
void readString(char *line);
void putstringdirect(); 


main()
{
char line[80];
putstringdirect();
printString("Hello World\0");
printString("Enter a line: \0");
readString(line);
printString(line);
while(1);
return 0;
}
void printString(char *chars) {
    int i;
	char ah = 0xe;
    int length =0;
  
    for (i = 0; chars[i] != '\0'; i++) {
		length++;
	}

	for (i = 0; i <length;i++){
		char now = chars[i];
		interrupt(0x10,0xe*256+now,0,0,0);
	}
 }

 /*  Reads a line from the console using Interrupt 0x16. */
void readString(char *line)
{
  int i, lineLength, ax;
  char charRead, backSpace, enter;
  lineLength = 80;
  i = 0;
  ax = 0;
  backSpace = 0x8;
  enter = 0xd;
  charRead = interrupt(0x16, ax, 0, 0, 0);
  while (charRead != enter && i < lineLength-2) {
    if (charRead != backSpace) {
      interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
      line[i] = charRead;
      i++;
    } else {
      i--;
      if (i >= 0) {
	interrupt(0x10, 0xe*256+charRead, 0, 0, 0);
	interrupt(0x10, 0xe*256+'\0', 0, 0, 0);
	interrupt(0x10, 0xe*256+backSpace, 0, 0, 0);
      }
      else {
	i = 0;
      }
    }
    charRead = interrupt(0x16, ax, 0, 0, 0);  
  }
  line[i] = 0xa;
  line[i+1] = 0x0;
  
  /* correctly prints a newline */
  printString("\r\n");

  return;
}

void putstringdirect(){
    int i;
    int j;
    int ad ;
    char* hellow;
    j = 0; 
    ad = 0x8000; 
    hellow = "Hello World\n";
    for(i =0; i<11;i++){

    putInMemory(0xB000, ad+ j++, hellow[i]);
    putInMemory(0xB000, ad+ j++, 0x7);}
  

}

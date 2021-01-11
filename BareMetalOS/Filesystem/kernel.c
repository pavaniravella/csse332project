/*Made by Jacob Jagiella with Pavani Ravella and Liao Zeyu*/
void printString(char *chars) ;
void readString(char *line);
void putstringdirect(); 
int div(int a, int b);
int mod(int a, int b);
void readSector(char *buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void readFile(char *name, char* buff);
void writeSector(char *buffer, int sector);
void writeFile(char* name, char* file_buff, int sectors);
void filenamehelper();
main()
{
char buffer[13312];  /* this is the maximum size of a file */

makeInterrupt21();
interrupt(0x21, 3, "messag\0", buffer, 0);  /* read the file into buffer */
interrupt(0x21, 0, buffer, 0, 0);	    /* print out the file */

while(1);
return 0;
}
void readFile(char *name, char* buff){
    char file_buf[512];
    int ax;
    int bx;
    int i;
    int m;
    int match;
    readSector(file_buf,2);
    match =0;
    for(i =0; i<512;i+=32){
      int j=0;
      match =1;
      while (name[j] !='\0'&& j<6)
      {
        match &= (file_buf[i+j]==name[j]);
        j++;
      }
      j=0;
      if(match){
        printString("I am in if amtch\n"); 
        for(ax =6;ax<32;ax++){
          if(file_buf[i+ax] ==0){
            return;
          }
          readSector(buff,file_buf[i+ax]);
          buff+=512;
        }
        return;
      }
    }
    return;
}

void writeFile(char* name, char* file_buff, int sectors) {
  int j;
  int i;
  int m;
  int n;
  int count;
	char buff2[512];
  char buff3[512];
	char fb[512];
	count = 0;
	j = 0;
	/*fill in with \0*/
	for (i = 0; i < 512; i++) {
		buff3[i] = '\0';
		buff2[i] = '\0';
		fb[i] = '\0';
	}

	readSector(buff3, 1);
	readSector(buff2, 2);
	for(m =0; m< 512;m++){
		if (buff3[m] == 0) {
			count++;
		}
	}

	if (sectors> count){
     return;
      }
/*transfer the file from the buffer into the start (0000) */
	for (n = 0; n < 512; n+= 32) {
    j = 0;
		if (buff2[n] == 0x00) {
			for (m = 0; m < 32; m++) {
				buff2[n + m] = 0;
			}
			while (name[j] != '\0' && j < 6) {
				buff2[n + j] = name[j];
				j++;
			}

			break;
		}
	}
	/* same for buff3 */
	j = 0;
	for (i = 0; i < 512; i++) {
		if (sectors <= 0) {
			break;	
		}
		if (buff3[i] == 0) {
			buff3[i] = 0xFF;
			buff2[n + 6 + j++] = i;
			m = 0;
			while (m < 512) {
				fb[m] = file_buff[m];
				m++;
			}
      /*next ->*/
			file_buff += 512;
			writeSector(fb, i);
			sectors--;
		}
	}
	writeSector(buff3,1);
	writeSector(buff2,2);
}
void filenamehelper(){
  char abc[7];
  abc[0]= 's';
  abc[1]= 't';
  abc[2]= 'r';
  abc[3]= 'i';
  abc[4]= 'n';
  abc[5]= 'g';
  abc[6]= '\0';
  executeProgram(abc, 0x2000);
	while(1);
}

void executeProgram(char* name,int segment) {
	char buff[13312];
  int i;
	int addr = 0x0000;
	/* load into buffer */
	readFile(name, buff);
	for (i = 0; i < 13312; i++) {
		putInMemory(segment,addr + i, buff[i]);
	}
	launchProgram(segment);
}
void printString(char *chars) {
    int i;
	  char ah = 0xe;
    int length =0;
  
    for (i = 0; chars[i] != '\0'; i++) {
		length++;
	}

	for (i = 0; i <=length;i++){
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
    ad = 0x8140; 
    hellow = "Hello World\n";
    for(i =0; i<11;i++){
    putInMemory(0xB000, ad+ j++, hellow[i]);
    putInMemory(0xB000, ad+ j++, 0x7);}
}


int mod(int a, int b){
  int temp;
  temp = a;
  while (temp >= b) {
    temp = temp-b;
  }
  return temp;
}

int div(int a, int b)
{
  int quotient;
  quotient = 0;
  while ((quotient + 1) * b <= a) {
    quotient++;
  }
  
  return quotient;
}

void readSector(char *buffer, int sector){
  /*relative sector, head, track and call  */
  int dx, cx, ah,al,bx,ch,cl,dh,dl,ax;
  ah =2;
  al =1;
  cl = mod(sector,18) +1;
  dh = mod(div(sector,18),2);
  ch = div(sector,36);
  ax= ah*256 + al;
  cx = ch*256 + cl;
  dl = 0;
  dx = dh*256 + dl;
  interrupt(0x13,ax,buffer,cx,dx);
}

void writeSector(char *buffer, int sector){
  /*relative sector, head, track and call  */
  int dx, cx, ah,al,bx,ch,cl,dh,dl,ax;
  ah =3;
  al =1;
  cl = mod(sector,18) +1;
  dh = mod(div(sector,18),2);
  ch = div(sector,36);
  ax= ah*256 + al;
  cx = ch*256 + cl;
  dl = 0;
  dx = dh*256 + dl;
  interrupt(0x13,ax,buffer,cx,dx);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){
    if(ax == 0){
      /*print string printString("Hello World\0"); */
        printString(bx);
    }else if(ax == 1){
      /*read string*/
      readString(bx);
    }else if(ax ==2){
      /*read sector*/
      readSector(bx,cx);
    }else{
       printString("error ax >=3");
    }
} 
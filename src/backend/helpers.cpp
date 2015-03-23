char* currentTime() {
  time (&currentTimeObject);
  currentTimeInfo = localtime (&currentTimeObject);
  
  strftime (currentTimeBuffer, 80, "%Y-%m-%d %H:%M:%S", currentTimeInfo);

  return currentTimeBuffer;
}

char *strColor(int attr, int fg, int bg) {
  sprintf(colorCommand, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
  return colorCommand;
}

void txtColor(int attr, int fg, int bg) {
  strColor(attr, fg, bg);
  printf("%s", colorCommand);
}

void resetColor() {
  txtColor(RESET, WHITE, BLACK);
}



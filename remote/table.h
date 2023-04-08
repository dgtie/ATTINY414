#define FF (char)0xff

struct Device{
  char command[22];
  char null;		// specify null command
  char address;
  void (*func)(char*, char, char);
};

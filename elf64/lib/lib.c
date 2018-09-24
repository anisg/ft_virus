#ifndef NULL
 #define NULL 0x0
#endif

#ifndef TRUE
 #define TRUE 1
#endif

#ifndef FALSE
 #define FALSE 0
#endif

typedef long long unsigned int size_t;

typedef unsigned char u_char;
typedef unsigned short  u_short;
typedef unsigned int  u_int;
typedef unsigned long u_long;
typedef unsigned short  ushort;   /* Sys V compatibility */

typedef char *  caddr_t;    /* core address */
typedef long  daddr_t;    /* disk address */
typedef short dev_t;      /* device number */
typedef u_long  ino_t;      /* inode number */
typedef long  off_t;      /* file offset (should be a quad) */
typedef u_short nlink_t;    /* link count */
typedef long  swblk_t;    /* swap offset */
typedef long  segsz_t;    /* segment size */
typedef u_short uid_t;      /* user id */
typedef u_short gid_t;      /* group id */
typedef short pid_t;      /* process id */
typedef u_short mode_t;     /* permissions */
typedef u_long  fixpt_t;    /* fixed point number */

//====================================================

size_t call(size_t num, size_t a1, size_t a2, size_t a3, size_t a4, size_t a5, size_t a6){
  register size_t  x  asm("rax") = num;
  register size_t  x1 asm("rdi") = a1;
  register size_t  x2 asm("rsi") = a2;
  register size_t  x3  asm("rdx") = a3;
  register size_t  x4  asm("r10") = a4;
  register size_t  x5  asm("r8") = a5;
  register size_t  x6  asm("r9") = a6;
  asm("syscall");
  return x;
}

#define CALL(X) call(X,0,0,0,0,0,0)
#define CALL1(X,a1) call(X,a1,0,0,0,0,0)
#define CALL2(X,a1,a2) call(X,a1,a2,0,0,0,0)
#define CALL3(X,a1,a2,a3) call(X,a1,a2,a3,0,0,0)
#define CALL4(X,a1,a2,a3,a4) call(X,a1,a2,a3,a4,0,0)
#define CALL5(X,a1,a2,a3,a4,a5) call(X,a1,a2,a3,a4,a5,0)
#define CALL6(X,a1,a2,a3,a4,a5,a6) call(X,a1,a2,a3,a4,a5,a6)

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))

enum syscall{READ = 0, WRITE = 1, OPEN = 2, CLOSE = 3, LSEEK = 8,
    MMAP = 9, MUNMAP = 11, EXIT = 60};

//==================== LOW FUNCTIONS =====================

int exit(int n){
  CALL1(EXIT, n);
}

void *malloc(size_t size){
	size_t *p = (void*)CALL6(MMAP, NULL, sizeof(size_t)+size, 6,34,-1,0);
	p[0] = size; //storing the size
	return p+sizeof(size_t);
}

void free(void *p){
	CALL2(MUNMAP, ((size_t)p)-sizeof(size_t), ((size_t*)(p-1))[0]);
}

size_t write(int fd, void *s, size_t n){
  CALL3(WRITE, fd, (size_t)s, n);
}

int close(int fd){
  return CALL1(CLOSE, fd);
}

size_t open(char *filename, int flag, int mode){  
  return CALL3(OPEN, (size_t)filename, flag, mode);
}

size_t lseek(unsigned int fd, size_t offset, unsigned int whence){
  return CALL3(LSEEK, fd, offset, whence);
}

//==================== HIGHER FUNCTIONS =====================

size_t slen(char *s){
  int i;for(i=0;s[i];i++);return i;
}

void print(char *s){
  write(1, s, slen(s));
}

void println(char *s){
  print(s);
  print("\n");
}

int fget(char *filename, char **ptr, size_t *l){
  int       fd;

  if ((fd = open(filename, 0, 0755)) < 0)
    return FALSE;
  //get 
  (*l) = lseek(fd, 0, 2);
  if (((*ptr) = CALL6(MMAP, NULL, *l, 3, 2, fd, 0))
    == (void*)-1)
    return FALSE;
  close(fd);
  return TRUE;
}

int fput(char *filename, char *ptr, size_t l){
  int fd;

  if ((fd = open(filename, 65, 0755)) < 0)
    return FALSE;
  write(fd, ptr, l);
  close(fd);
  return TRUE;
}

int     str_equal(char *s1, char *s2)
{
        int i;

        i = 0;
        if (!s1 || !s2)
                return FALSE;
        while (s1[i] && s2[i]
                && (s1[i] == s2[i]))
                i++;
        return (s1[i] - s2[i]) == 0;
}

void  snbr(size_t nb, char *sn)
{
  int   i;
  int   j;
  char  tmp;

  i = 0;
  while (nb)
  {
    sn[i++] = (nb % 10) + '0';
    nb /= 10;
  }
  sn[i] = 0;
  i -= 1;
  j = 0;
  while (j <= i)
  {
    tmp = sn[j];
    sn[j] = sn[i];
    sn[i] = tmp;
    j += 1;
    i -= 1;
  }
}

void printnb(size_t nb){
  char s[64];
  snbr(nb, s);
  print(s);
}

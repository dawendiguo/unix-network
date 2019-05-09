/*网络编程用的头文件，来源《UNIX网络编程》
  
  #ifndef __unp_h
  #define __unp_h
  
  #include "../config.h"              //当前系统设置项
  
  // If anything changes in the following list of #include , must change acsite.m4 also, for configure's tests.
  
  #include <sys/types.h>    //basic system data types
  #include <sys/socket.h>   //basic socket deinitions
  #include <sys/time.h>     //timeval() for selec()
  #include <time.h>         //timespec() fro pselect()
  #include <netinet/in.h>   //sockadd_in() and other Internet defns
  #include <arpa/inet.h>    //inet(3) functions
  #include <errno.h>
  #include <fcntl.h>        //for noblocking
  #include <netdb.h>
  #include <signal.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/stat.h>     //for S_xxx file mode constants
  #include <sys/uio.h>      //for iovec() and readv/writev
  #include <unistd.h>       
  #include <sys/wait.h>     //wait() and waitfor()
  #include <sys/un.h>       //for Unix domain sockets
  
  #ifdef  HAVE_SYS_SELECT_H
  #include <sys/sysctl.h>
  #endif
  
  #ifdef  HAVE_POLL_H
  #include <poll.h>       //for convenience
  #endif
  
  #ifdef HAVE_SYS_EVENT_H
  #include <sys/event.h>
  #endif
  
  #ifdef HAVE_STRINGS_H
  #include <strings.h>
  #endif
  
  #ifdef HAVE_SYS_IOCTL_H
  #include <sys/ioctl.h>
  #endif
  
  #ifdef HAVE_SYS_IOCTL_H
  #include <sys/ioctl.h>
  #endif
  
  #ifdef HAVE_SYS_FILIO_H
  #include <sys/filio.h>
  #endif
  
  #ifdef  HAVE_SYS_SOCKIO_H
  #include <sys/sockio.h>
  #endif
  
  #ifdef  HAVE_PTHREAD_H
  #include <pthread.h>
  #endif
  
  #ifdef HAVE_NET_IF_DL_H
  #include <net/if_dl.h>
  #endif
  
  #ifdef HAVE_NETINET_SCTP_H
  #include <netinet/sctp.h>
  #endif
  
 // ISF/1 actully disables recv() and send() in <sys/socket.h>
 #ifdef __osf__
 #undef recv
 #undef send
 
 #define recv(a,b,c,d) recvfrom(a,b,c,d,0,0)
 #define send(a,b,c,d) sendto(a,b,c,d,0,0)
 
 #ifndef INADDR_NONE
 #define INADDR_NONE 0xffffffff         //should have been in <netinet/in.h>
 #endif
 
 #ifndef SHUT_RD                        //these three POSIX name are new
 #define SHUT_RD    0                   //shutdown for reading
 #define SHUT_WR    1                   //shutdown for writing
 #define SHUT_RDWR  2                   //shutdown for reading and writing
 #endif
 
 #ifndef INET_ADDRSTRLEN
 #define  INET_ADDRSTRLEN 16            //"ddd.ddd.ddd.ddd\0"1234567890123456
 #endif
 
 /* Define following even if IPV6 not supported , so we can always allocate an adequattely sized buffer without #ifdefs in the code.
 #ifndef INET6_ADDRSTRLEN
 #define INET6_ADDRSTRLEN 46            /*max size of IPv6 address string :
                                          "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx"or
                                          "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
                                          12345678901234567890123456*/
 #endif
 
 /* Define bzero() as a macro if it is not in standard C libray.
 #ifndef HAVE_BZERO
 #define bzer0(ptr,n) memset(ptr,0,n)
 #endif
 
 //Older resolvers do not have gethostbyname2()
 #ifndef HAVE_GETHOSTBYNAME2
 #define gethostbyname2(host,family)  gethostbyname((host))
 #endif
 
 // The structure returned by recvfrom_flags()
 struct unp_in_pktinfo{
  struct in_addr  ipi_addr;   //dst IPv4 address
  int ipi_ifindex;            //received interface index
  };
  
  /* We need the newer CMSG_LEN() and CMSG_SPACE() macros, but few implementations support them today, These two macros really need and 
  ALIGN() macro, but each implementation does this defferently.*/
  #ifndef CMSG_LEN
  #define CMSG_LEN(size) (sizeof(struct cmsghdr) + (size))
  #endif 
  
  #ifndef CMSG_SPACE
  #define CMSG_SPACE(size)    (sizeof(struct cmsghdr) + (size))
  #endif
  
  /* POSIX requires the SUN_LEN() marco,but not all implementations define it (yet). Note that this 4.4BSD macro works regardless whether there 
  is a length field or not.*/
  
  #ifndef  SUN_LEN
  #define  SUN_LEN(su)  (sizeof(*(su)) - sizeof((su)->sun_path) +strlen((su)->sun_path))
  #endif
  
  /* POSIX rename that an #include of <poll.h> define INFTLM, but many systems 
  still define it in <sys/stropts.h>. we don't want to include all the STREAMS stuff if it's 
  not needed , so we just define INFTIM here, Ths is the standard value, but there's no grarantee
  it is -l.*/
  #ifndef INFTIM
  #define INFTIM  (-1)
  #ifdef  HAVE_POLL_H
  #define INFTIM_UNPH
  #emdif
  #endif
  
  /* Following could be derived from SOMAXCONN in <sys/cocket.h>, but man
  kernels still #define it as 5, while actually supporting many more */
  #define LISTENQ 1024  //2nd argument to listen()
  
  //Miscellaneous constants
  #define MAXLINE 4096
  #define BUFFSIZE  8192
  
  // Define some port number that can be used for our examples
  #define SERV_PORT 9877    //TCP and UDP
  #define SERV_PORT_STR "9877"    //TCP and UDP
  #define UNIXSTR_PATH  "/home/html/unix-network/tmp/unix.str"    //UNIX domain stream
  #define UNIXDG_PATH "/home/html/unix-network/tmp/unix.dg"       //UNIX domain datagram
  
  // Following shortens all the typescasts of pointer arguments:
  #define SA  struct sockaddr
  
  #define HAVE_STRUCT_SOCKADDR_STORAGE  
  #ifndef HAVE_STRUCT_SOCKADDR_STORAGE
  //RFC 3493: protocol-independent placeholder for socket addresses
  
  #define __SS_MAXSIZE 128
  #define __SS_ALIGNSIZE (sizeof(int64_t))
  #ifdef HAVE_SOKADDR_SA_LEN
  #define __SS_PAD1SIZE (__SS_ALIGNSIZE - sizeof(u_char) - sizeof(sa_family_t))
  #else
  #define __SS_PAD1SIZE (__SS_ALIGNSIZE - sizeof(sa_family_t))
  #endif
  #define __SS_PAD2SIZE (__SS_MAXSIZE - 2*__SS_ALIGNSIZE)
  
  struct sockaddr_storage{
  #ifdef HAVE_SOCKADDR_SA_LEN
    u_hcar ss_len;
  #endif
    sa_family_t ss_family;
    char __ss_pad1[__SS_PAD1SIZE];
    int64_t  __ss_align;
    char __s_pad2[__SS_PAD2SIZE];
  };
  #endif
  
  #define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)   //default file access permissions for new files 
  #define DIR_MODE  (FILE_MODE|S_IXUSR|S_IXGRP|S_IXOTH)   //default permissions for new directories 
  
  typedef void Sigfunc(int);    // for signal handlers
  
  #define min(a,b)    ( (a) < (b) ? (a) : (b))
  #define max(a,b)    ( (a) > (b) ? (a) : (b))
  
  #ifndef HAVE_ADDRINFO_STRUCT
  #include "../lib/addrinfo.h"
  #endif
  
  #ifdef  HAVE_IF_NAMEINDEX_STRUCT
  struct if_nameindex{
    unsigned int if_index;
    char *if_name;
  };
  #endif
  
  #ifndef HAVE_TIMESPEC_STRUCT
  struct timespec{
    time_t  tv_sec;
    long tv_nsec;
  };
  #endif
  

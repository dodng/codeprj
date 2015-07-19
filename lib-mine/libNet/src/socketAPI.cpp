#include "socketAPI.h"
#include "socketdef.h"
#include "assert.h"
#include "strUtil.h"

#ifdef _LINUX64
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <sys/io.h>
extern int errno;
#endif

namespace tcpnet {

	char g_socketError[SOCKET_ERROR_SIZE];
	int  g_socketErrorCode = -1;

	namespace SocketAPI {

		SOCK socket_create(int domain, int type, int protocol)
		{
			__ENTER_FUNCTION

				SOCK s = ::socket(domain,type,protocol);
			if ( s == INVALID_SOCKET ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EACCES : 
					Strncpy( g_socketError, "EACCES", SOCKET_ERROR_SIZE ) ;
					break;
				case EAFNOSUPPORT :
					Strncpy( g_socketError, "EAFNOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break;
				case EINVAL :
					Strncpy( g_socketError, "EINVAL", SOCKET_ERROR_SIZE ) ;
					break;
				case EMFILE : 
					Strncpy( g_socketError, "EMFILE", SOCKET_ERROR_SIZE ) ;
					break;
				case ENFILE : 
					Strncpy( g_socketError, "ENFILE", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOBUFS : 
					Strncpy( g_socketError, "ENOBUFS", SOCKET_ERROR_SIZE ) ;
					break;
				case EPROTONOSUPPORT :
					Strncpy( g_socketError, "EPROTONOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break;
					}
				}//end of switch
	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEAFNOSUPPORT : 
					Strncpy( g_socketError, "WSAEAFNOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEMFILE : 
					Strncpy( g_socketError, "WSAEMFILE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOBUFS : 
					Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEPROTONOSUPPORT : 
					Strncpy( g_socketError, "WSAEPROTONOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEPROTOTYPE : 
					Strncpy( g_socketError, "WSAEPROTOTYPE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAESOCKTNOSUPPORT : 
					Strncpy( g_socketError, "WSAESOCKTNOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break ;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
			}

			return s;
			__LEAVE_FUNCTION
				return INVALID_SOCKET ;
		}

		bool socket_bind ( SOCK s , const struct sockaddr * addr , int addrlen ) 
		{
			__ENTER_FUNCTION

				if ( bind ( s , addr , addrlen ) == SOCKET_ERROR ) 
				{
	#ifdef _LINUX64
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EADDRINUSE :
						Strncpy( g_socketError, "EADDRINUSE", SOCKET_ERROR_SIZE ) ;
						break;
					case EINVAL : 
						Strncpy( g_socketError, "EINVAL", SOCKET_ERROR_SIZE ) ;
						break;
					case EACCES : 
						Strncpy( g_socketError, "EACCES", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOTSOCK : 
						Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break;
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break;
					case EROFS : 
						Strncpy( g_socketError, "EROFS", SOCKET_ERROR_SIZE ) ;
						break;
					case EFAULT : 
						Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
						break;
					case ENAMETOOLONG : 
						Strncpy( g_socketError, "ENAMETOOLONG", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOENT : 
						Strncpy( g_socketError, "ENOENT", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOMEM : 
						Strncpy( g_socketError, "ENOMEM", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOTDIR : 
						Strncpy( g_socketError, "ENOTDIR", SOCKET_ERROR_SIZE ) ;
						break;
					case ELOOP : 
						Strncpy( g_socketError, "ELOOP", SOCKET_ERROR_SIZE ) ;
						break;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break;
						}
					}//end of switch
	#elif _WIN32
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSAESOCKTNOSUPPORT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEADDRINUSE : 
						Strncpy( g_socketError, "WSAEADDRINUSE", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEADDRNOTAVAIL : 
						Strncpy( g_socketError, "WSAEADDRNOTAVAIL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT : 
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEAFNOSUPPORT : 
						Strncpy( g_socketError, "WSAEAFNOSUPPORT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL : 
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOBUFS : 
						Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
	#endif

					return false ;
				}

				return true ;

				__LEAVE_FUNCTION

					return false ;
		}

		//建立连接
		bool socket_connect(SOCK s , const struct sockaddr * addr , int addrlen)
		{
			__ENTER_FUNCTION

				if ( connect(s,addr,addrlen) == SOCKET_ERROR ) 
				{
	#ifdef _LINUX64
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EALREADY : 
						Strncpy( g_socketError, "EALREADY", SOCKET_ERROR_SIZE ) ;
						break;
					case EINPROGRESS : 
						Strncpy( g_socketError, "EINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break;
					case ECONNREFUSED :
						Strncpy( g_socketError, "ECONNREFUSED", SOCKET_ERROR_SIZE ) ;
						break;
					case EISCONN : 
						Strncpy( g_socketError, "EISCONN", SOCKET_ERROR_SIZE ) ;
						break;
					case ETIMEDOUT : 
						Strncpy( g_socketError, "ETIMEDOUT", SOCKET_ERROR_SIZE ) ;
						break;
					case ENETUNREACH : 
						Strncpy( g_socketError, "ENETUNREACH", SOCKET_ERROR_SIZE ) ;
						break;
					case EADDRINUSE : 
						Strncpy( g_socketError, "EADDRINUSE", SOCKET_ERROR_SIZE ) ;
						break;
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break;
					case EFAULT : 
						Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOTSOCK :
						Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break;
						}
					}//end of switch
	#elif _WIN32
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEADDRINUSE : 
						Strncpy( g_socketError, "WSAEADDRINUSE", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINTR : 
						Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEADDRNOTAVAIL : 
						Strncpy( g_socketError, "WSAEADDRNOTAVAIL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEAFNOSUPPORT : 
						Strncpy( g_socketError, "WSAEAFNOSUPPORT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAECONNREFUSED : 
						Strncpy( g_socketError, "WSAECONNREFUSED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEALREADY : 
						Strncpy( g_socketError, "WSAEALREADY", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT : 
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL : 
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEISCONN : 
						Strncpy( g_socketError, "WSAEISCONN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEMFILE : 
						Strncpy( g_socketError, "WSAEMFILE", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETUNREACH : 
						Strncpy( g_socketError, "WSAENETUNREACH", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOBUFS : 
						Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAETIMEDOUT : 
						Strncpy( g_socketError, "WSAETIMEDOUT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEWOULDBLOCK  : 
						Strncpy( g_socketError, "WSAEWOULDBLOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
	#endif
					return false ;
				}
				return true ;
				__LEAVE_FUNCTION
					return false ;
		}

		//监听
		bool socket_listen(SOCK s, int backlog)
		{
			__ENTER_FUNCTION

				if ( listen( s , backlog ) == SOCKET_ERROR ) 
				{
	#ifdef _LINUX64
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOTSOCK :
						Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break;
					case EOPNOTSUPP :
						Strncpy( g_socketError, "EOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
						break;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break;
						}
					}//end of switch
	#elif _WIN32
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEADDRINUSE : 
						Strncpy( g_socketError, "WSAEADDRINUSE", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL : 
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEISCONN : 
						Strncpy( g_socketError, "WSAEISCONN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEMFILE : 
						Strncpy( g_socketError, "WSAEMFILE", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOBUFS : 
						Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEOPNOTSUPP : 
						Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
	#endif
					return false ;
				}

				return true ;
				__LEAVE_FUNCTION
					return false ;
		}

		//接受连接
		SOCK socket_accept(SOCK s, struct sockaddr* addr, int* addrlen)
		{
			__ENTER_FUNCTION
	#ifdef _LINUX64
				SOCK client = accept( s , addr , (uint*)addrlen );
	#elif _WIN32
				SOCK client = accept( s , addr , addrlen );
	#endif
			if ( client == INVALID_SOCKET ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EWOULDBLOCK : 
					Strncpy( g_socketError, "EWOULDBLOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case ECONNRESET :
					Strncpy( g_socketError, "ECONNRESET", SOCKET_ERROR_SIZE ) ;
					break;
				case ECONNABORTED :
					Strncpy( g_socketError, "ECONNABORTED", SOCKET_ERROR_SIZE ) ;
					break;
				case EPROTO :
					Strncpy( g_socketError, "EPROTO", SOCKET_ERROR_SIZE ) ;
					break;
				case EINTR :
					Strncpy( g_socketError, "EINTR", SOCKET_ERROR_SIZE ) ;
					break;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTSOCK : 
					Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case EOPNOTSUPP : 
					Strncpy( g_socketError, "EOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break;
				case EFAULT : 
					Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
					break;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break;
					}
				}//end of switch
	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEFAULT : 
					Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINTR : 
					Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINVAL : 
					Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEMFILE : 
					Strncpy( g_socketError, "WSAEMFILE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOBUFS : 
					Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTSOCK : 
					Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEOPNOTSUPP : 
					Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEWOULDBLOCK : 
					Strncpy( g_socketError, "WSAEWOULDBLOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNRESET : 
					Strncpy( g_socketError, "WSAECONNRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
				return INVALID_SOCKET ;
			}
			return client;
			__LEAVE_FUNCTION
				return INVALID_SOCKET ;
		}

		//发送数据
		int socket_send(SOCK s, const void* buf, int length, int flags)
		{
			__ENTER_FUNCTION

			int nSent =send(s,(const char *)buf, length, flags);
			if ( nSent == SOCKET_ERROR ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK;
				case ECONNRESET :
					Strncpy( g_socketError, "ECONNRESET", SOCKET_ERROR_SIZE ) ;
					break;
				case EPIPE :
					Strncpy( g_socketError, "EPIPE", SOCKET_ERROR_SIZE ) ;
					break;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTSOCK : 
					Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case EFAULT : 
					Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
					break;
				case EMSGSIZE : 
					Strncpy( g_socketError, "EMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOBUFS : 
					Strncpy( g_socketError, "ENOBUFS", SOCKET_ERROR_SIZE ) ;
					break;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					}
				}//end of switch
	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEACCES : 
					Strncpy( g_socketError, "WSAEACCES", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINTR : 
					Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEFAULT : 
					Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETRESET : 
					Strncpy( g_socketError, "WSAENETRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOBUFS : 
					Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTCONN : 
					Strncpy( g_socketError, "WSAENOTCONN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTSOCK : 
					Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEOPNOTSUPP : 
					Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAESHUTDOWN : 
					Strncpy( g_socketError, "WSAESHUTDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK ;
					break ;
				case WSAEMSGSIZE : 
					Strncpy( g_socketError, "WSAEMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINVAL : 
					Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNABORTED : 
					Strncpy( g_socketError, "WSAECONNABORTED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNRESET : 
					Strncpy( g_socketError, "WSAECONNRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
			} 

			return nSent;
			__LEAVE_FUNCTION
				return 0 ;
		}

		int socket_sendto(SOCK s, const void* buf, int length, int flags, const struct sockaddr* to, int tolen)
		{
			__ENTER_FUNCTION

				int nSent = sendto(s,(const char *)buf,length,flags,to,tolen);
			if ( nSent == SOCKET_ERROR ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK;
				case ECONNRESET :
					Strncpy( g_socketError, "ECONNRESET", SOCKET_ERROR_SIZE ) ;
					break;
				case EPIPE :
					Strncpy( g_socketError, "EPIPE", SOCKET_ERROR_SIZE ) ;
					break;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTSOCK : 
					Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case EFAULT : 
					Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
					break;
				case EMSGSIZE : 
					Strncpy( g_socketError, "EMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOBUFS : 
					Strncpy( g_socketError, "ENOBUFS", SOCKET_ERROR_SIZE ) ;
					break;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					}
				}//end of switch
	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEACCES : 
					Strncpy( g_socketError, "WSAEACCES", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINTR : 
					Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEFAULT : 
					Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETRESET : 
					Strncpy( g_socketError, "WSAENETRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOBUFS : 
					Strncpy( g_socketError, "WSAENOBUFS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTCONN : 
					Strncpy( g_socketError, "WSAENOTCONN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTSOCK : 
					Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEOPNOTSUPP : 
					Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAESHUTDOWN : 
					Strncpy( g_socketError, "WSAESHUTDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK ;
					break ;
				case WSAEMSGSIZE : 
					Strncpy( g_socketError, "WSAEMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNABORTED : 
					Strncpy( g_socketError, "WSAECONNABORTED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNRESET : 
					Strncpy( g_socketError, "WSAECONNRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEADDRNOTAVAIL : 
					Strncpy( g_socketError, "WSAEADDRNOTAVAIL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEAFNOSUPPORT : 
					Strncpy( g_socketError, "WSAEAFNOSUPPORT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINVAL : 
					Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETUNREACH : 
					Strncpy( g_socketError, "WSAENETUNREACH", SOCKET_ERROR_SIZE ) ;
					break ;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
			}

			return nSent;
			__LEAVE_FUNCTION
				return 0 ;
		}

		//接收数据
		int socket_recv(SOCK s, void* buf, int length, int flags)
		{
			__ENTER_FUNCTION

				int nrecv = recv(s,(char*)buf, length, flags);
			if ( nrecv == SOCKET_ERROR ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK;
				case ECONNRESET :
					Strncpy( g_socketError, "ECONNRESET", SOCKET_ERROR_SIZE ) ;
					break;
				case EPIPE :
					Strncpy( g_socketError, "EPIPE", SOCKET_ERROR_SIZE ) ;
					break;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTCONN : 
					Strncpy( g_socketError, "ENOTCONN", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTSOCK : 
					Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case EINTR : 
					Strncpy( g_socketError, "EINTR", SOCKET_ERROR_SIZE ) ;
					break;
				case EFAULT : 
					Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
					break;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					}
				}//end of switch

	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTCONN : 
					Strncpy( g_socketError, "WSAENOTCONN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINTR : 
					Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTSOCK : 
					Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEOPNOTSUPP : 
					Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAESHUTDOWN : 
					Strncpy( g_socketError, "WSAESHUTDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK ;
					break ;
				case WSAEMSGSIZE : 
					Strncpy( g_socketError, "WSAEMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINVAL : 
					Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNABORTED : 
					Strncpy( g_socketError, "WSAECONNABORTED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNRESET : 
					Strncpy( g_socketError, "WSAECONNRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
			} 

			return nrecv;
			__LEAVE_FUNCTION
				return 0 ;
		}

		int socket_recvfrom(SOCK s, void* buf, int length, int flags, struct sockaddr* from, int* fromlen)
		{
			__ENTER_FUNCTION
	#ifdef _LINUX64
				int nReceived = recvfrom(s,(char*)buf,length,flags,from,(uint*)fromlen);
	#elif _WIN32
				int nReceived = recvfrom(s,(char*)buf,length,flags,from,fromlen);
	#endif
			if ( nReceived == SOCKET_ERROR ) 
			{
	#ifdef _LINUX64
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK;
				case ECONNRESET :
					Strncpy( g_socketError, "ECONNRESET", SOCKET_ERROR_SIZE ) ;
					break;
				case EPIPE :
					Strncpy( g_socketError, "EPIPE", SOCKET_ERROR_SIZE ) ;
					break;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTCONN : 
					Strncpy( g_socketError, "ENOTCONN", SOCKET_ERROR_SIZE ) ;
					break;
				case ENOTSOCK : 
					Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break;
				case EINTR : 
					Strncpy( g_socketError, "EINTR", SOCKET_ERROR_SIZE ) ;
					break;
				case EFAULT : 
					Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
					break;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					}
				}//end of switch

	#elif _WIN32
				int iErr = WSAGetLastError() ;
				g_socketErrorCode = iErr;
				switch ( iErr ) 
				{
				case WSANOTINITIALISED : 
					Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENETDOWN : 
					Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEFAULT : 
					Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINTR : 
					Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINPROGRESS : 
					Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEINVAL : 
					Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTCONN : 
					Strncpy( g_socketError, "WSAENOTCONN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAENOTSOCK : 
					Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEOPNOTSUPP : 
					Strncpy( g_socketError, "WSAEOPNOTSUPP", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAESHUTDOWN : 
					Strncpy( g_socketError, "WSAESHUTDOWN", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAEWOULDBLOCK : 
					return SOCKET_ERROR_WOULD_BLOCK ;
					break ;
				case WSAEMSGSIZE : 
					Strncpy( g_socketError, "WSAEMSGSIZE", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNABORTED : 
					Strncpy( g_socketError, "WSAECONNABORTED", SOCKET_ERROR_SIZE ) ;
					break ;
				case WSAECONNRESET : 
					Strncpy( g_socketError, "WSAECONNRESET", SOCKET_ERROR_SIZE ) ;
					break ;
				default :
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					};
				};//end of switch
	#endif
			}

			return nReceived;
			__LEAVE_FUNCTION
				return 0 ;
		}

		//关闭套接字
		bool socket_close(SOCK s)
		{
			__ENTER_FUNCTION

	#ifdef _LINUX64
				if ( close(s) < 0 ) 
				{
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break;
					default :
						{
							break;
						}
					}
					return false ;
				}
	#elif _WIN32
				if ( closesocket(s) == SOCKET_ERROR ) 
				{
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINTR : 
						Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEWOULDBLOCK : 
						Strncpy( g_socketError, "WSAEWOULDBLOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default : 
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
					return false ;
				}
	#endif

				return true ;
				__LEAVE_FUNCTION
					return false ;
		}

		//获得套接字信息
		bool socket_getsockopt(SOCK s, int level, int optname, void* optval, int* optlen)
		{
			__ENTER_FUNCTION
	#ifdef _LINUX64
				if ( getsockopt( s , level , optname , optval , (uint*)optlen ) == SOCKET_ERROR )
				{
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOTSOCK : 
						Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break;
					case ENOPROTOOPT : 
						Strncpy( g_socketError, "ENOPROTOOPT", SOCKET_ERROR_SIZE ) ;
						break;
					case EFAULT : 
						Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
						break;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						}
					}//end of switch

					return false ;
				}
	#elif _WIN32
				if ( getsockopt( s , level , optname , (char*)optval , optlen ) == SOCKET_ERROR ) 
				{
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT : 
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL : 
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOPROTOOPT : 
						Strncpy( g_socketError, "WSAENOPROTOOPT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default : 
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
					return false ;
				}
	#endif
				return true ;
				__LEAVE_FUNCTION
					return false;	
		}

		//设置套接字信息
		bool socket_setsockopt(SOCK s, int level, int optname, const void* optval, int optlen)
		{
			__ENTER_FUNCTION

	#ifdef _LINUX64
				if ( setsockopt( s , level , optname , optval , optlen ) == SOCKET_ERROR ) 
				{
					g_socketErrorCode = errno;
					switch ( errno ) 
					{
					case EBADF : 
						Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
						break ;
					case ENOTSOCK : 
						Strncpy( g_socketError, "ENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					case ENOPROTOOPT : 
						Strncpy( g_socketError, "ENOPROTOOPT", SOCKET_ERROR_SIZE ) ;
						break ;
					case EFAULT : 
						Strncpy( g_socketError, "EFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break;
						}
					}//end of switch
					return false ;
				}
	#elif _WIN32
				if ( setsockopt( s , level , optname , (char*)optval , optlen ) == SOCKET_ERROR ) 
				{
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT : 
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL : 
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETRESET : 
						Strncpy( g_socketError, "WSAENETRESET", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOPROTOOPT : 
						Strncpy( g_socketError, "WSAENOPROTOOPT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTCONN : 
						Strncpy( g_socketError, "WSAENOTCONN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};//end of switch
					return false ;
				}
	#endif

				return true ;
				__LEAVE_FUNCTION
					return false ;
		}

		//控制套接字选项
		bool socket_ioctlsocket(SOCK s, long cmd, unsigned long* argp)
		{
			__ENTER_FUNCTION

	#ifdef _LINUX64
	#elif _WIN32
				if ( ioctlsocket(s,cmd,argp) == SOCKET_ERROR ) 
				{
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN : 
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS : 
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK : 
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT : 
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};
					return false ;
				}
	#endif

				return true ;
				__LEAVE_FUNCTION
					return false ;
		}

		//设置阻塞及非阻塞模式
		bool setsocketnonblocking(SOCK s, bool on)
		{
			__ENTER_FUNCTION

	#ifdef _LINUX64

				int flags = fcntl( s , F_GETFL , 0 );
			if (flags < 0)
			{
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EINTR : 
					Strncpy( g_socketError, "EINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case EINVAL : 
					Strncpy( g_socketError, "EINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break ;
				case EACCES : 
					Strncpy( g_socketError, "EACCES", SOCKET_ERROR_SIZE ) ;
					break ;
				case EAGAIN : 
					Strncpy( g_socketError, "EAGAIN", SOCKET_ERROR_SIZE ) ;
					break ;
				case EDEADLK : 
					Strncpy( g_socketError, "EDEADLK", SOCKET_ERROR_SIZE ) ;
					break ;
				case EMFILE  : 
					Strncpy( g_socketError, "EMFILE", SOCKET_ERROR_SIZE ) ;
					break ;
				case ENOLCK : 
					Strncpy( g_socketError, "ENOLCK", SOCKET_ERROR_SIZE ) ;
					break ;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break;
					}
				}
				return false;
			}

			if ( on )
				flags |= O_NONBLOCK;
			else
				flags &= ~O_NONBLOCK;
			int result = fcntl ( s , F_SETFL , flags );
			if ( result < 0 ) 
			{
				g_socketErrorCode = errno;
				switch ( errno ) 
				{
				case EINTR : 
					Strncpy( g_socketError, "EINTR", SOCKET_ERROR_SIZE ) ;
					break ;
				case EINVAL : 
					Strncpy( g_socketError, "EINVAL", SOCKET_ERROR_SIZE ) ;
					break ;
				case EBADF : 
					Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
					break ;
				case EACCES : 
					Strncpy( g_socketError, "EACCES", SOCKET_ERROR_SIZE ) ;
					break ;
				case EAGAIN : 
					Strncpy( g_socketError, "EAGAIN", SOCKET_ERROR_SIZE ) ;
					break ;
				case EDEADLK : 
					Strncpy( g_socketError, "EDEADLK", SOCKET_ERROR_SIZE ) ;
					break ;
				case EMFILE  : 
					Strncpy( g_socketError, "EMFILE", SOCKET_ERROR_SIZE ) ;
					break ;
				case ENOLCK : 
					Strncpy( g_socketError, "ENOLCK", SOCKET_ERROR_SIZE ) ;
					break ;
				default : 
					{
						Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
						break;
					}
				}
				return false;
			}
			return true;

	#elif _WIN32

				unsigned long argp = ( on == true ) ? 1 : 0;
			return socket_ioctlsocket( s,FIONBIO,&argp);

	#endif

			__LEAVE_FUNCTION
				return false ;
		}


		uint socket_available( SOCK s )
		{
			__ENTER_FUNCTION

	#ifdef _LINUX64
				int arg = 0;
			if ( ioctl(s, (unsigned long int)FIONREAD, &arg) < 0 )
			{
				g_socketErrorCode = errno;
				switch ( errno ) {
			case EBADF : 
				Strncpy( g_socketError, "EBADF", SOCKET_ERROR_SIZE ) ;
				break;
			case ENOTTY :
				Strncpy( g_socketError, "ENOTTY", SOCKET_ERROR_SIZE ) ;
				break;
			case EINVAL : 
				Strncpy( g_socketError, "EINVAL", SOCKET_ERROR_SIZE ) ;
				break;
			default :
				{
					Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
					break;
				}
				}
				arg = 0;
			}
			return arg;
	#elif _WIN32
				unsigned long argp = 0;
			bool bRet = socket_ioctlsocket(s,FIONREAD,&argp);
			if (!bRet)
			{
				argp = 0;
			}
			return argp;
	#endif

			__LEAVE_FUNCTION
				return 0 ;
		}

		//查询状态
		int socket_select(int maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout)
		{
			__ENTER_FUNCTION

				int result;
			__MYTRY 
			{
				result = select( maxfdp1 , readset , writeset , exceptset , timeout );
				if( result == SOCKET_ERROR )
				{
	#ifdef _LINUX64

	#elif _WIN32
					int iErr = WSAGetLastError() ;
					g_socketErrorCode = iErr;
					switch ( iErr ) 
					{
					case WSANOTINITIALISED : 
						Strncpy( g_socketError, "WSANOTINITIALISED", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEFAULT:
						Strncpy( g_socketError, "WSAEFAULT", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENETDOWN:
						Strncpy( g_socketError, "WSAENETDOWN", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINVAL:
						Strncpy( g_socketError, "WSAEINVAL", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINTR:
						Strncpy( g_socketError, "WSAEINTR", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAEINPROGRESS:
						Strncpy( g_socketError, "WSAEINPROGRESS", SOCKET_ERROR_SIZE ) ;
						break ;
					case WSAENOTSOCK:
						Strncpy( g_socketError, "WSAENOTSOCK", SOCKET_ERROR_SIZE ) ;
						break ;
					default :
						{
							Strncpy( g_socketError, "UNKNOWN", SOCKET_ERROR_SIZE ) ;
							break ;
						};
					};
	#endif
				}//end if
			} 
			__MYCATCH
			{
			}
			return result;
			__LEAVE_FUNCTION
			return 0 ;
		}
	} //namespace SocketAPI

} //namespace tcpnet

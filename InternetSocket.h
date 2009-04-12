#ifndef NET_InternetSocket_h__
#define NET_InternetSocket_h__

#include "SimpleSocket.h"

namespace NET
{
	//! Internet Socket
	class InternetSocket : public SimpleSocket
	{
	public:
		/*!
			Establish a socket connection with the given foreign
			address and port

			If you are using a connection oriented socket, (like TCPSocket) you have to call connect() on
			that socket in order to send data.
			However, if you are using a non-connection oriented socket (like UDPSocket), calling connect
			on that socket will allow you to use the send() function like on a connection oriented socket.
			You don't have to use sendTo() in that case, but you are still allowed to.

			\param foreignAddress foreign address (IP address or name)
			\param foreignPort foreign port
			\exception SocketException thrown if unable to establish connection
		*/
		void connect( const std::string& foreignAddress, unsigned short foreignPort);

		/*!
			Unset foreign address and port
			\return true if disassociation is successful
			\exception SocketException thrown if unable to disconnect UDP socket
		*/
		void disconnect();

		//@{
		/*!
		 * \name
		 * Set the local port to the specified port and the local address
		 * to any interface.
		 *
		 * bind() attaches the local endpoint of the socket to the specified
		 * interface and port.
		 * Typically all servers use bind() to attach to the local port specified by the
		 * used protocol. You just need to call bind if you want to restrict the connection
		 * to a specific local interface, or if you want to listen on a specific port.
		 * If you are just a client using connect() to connect to a server, calling
		 * bind is most likely unnecessary.
		 *
		 * To bind to a port < 1023 you need eighter superuser access, or the SUID bit set on
		 * your program.
		 *
		 * \param localAddress local address
		 * \param localPort local port
		 * \exception SocketException thrown if setting local port fails
		 * \sa NET::resolveSerice
		*/
		void bind( unsigned short localPort = 0);
		void bind( const std::string& localAddress, unsigned short localPort = 0);
		//@}

		/*!
			Get the local address (after binding the socket)
			\return local address of socket
			\exception SocketException thrown if fetch fails
		*/
		std::string getLocalAddress();

		/*!
			Get the local port (after binding the socket)
			\return local port of socket
			\exception SocketException thrown if fetch fails
		*/
		unsigned short getLocalPort();

		/*!
			Get the foreign address.  Call connect() before using this function.
			\return foreign address
			\exception SocketException thrown if unable to fetch foreign address
		*/
		std::string getForeignAddress();

		/*!
			Get the foreign port.  Call connect() before using this function.
			\return foreign port
			\exception SocketException thrown if unable to fetch foreign port
		*/
		unsigned short getForeignPort();
	protected:
		//! create socket from a SocketHandle returned by an accept() call
		InternetSocket( int sockfd);

		InternetSocket( int type, int protocol);
	};

} // namespace NET

#endif // NET_InternetSocket_h__
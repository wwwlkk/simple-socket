#include "SimpleSocket.h"

#include <poll.h>
#include <cstring>
#include <cerrno>

using namespace NET;

SocketException::SocketException( const std::string& message, bool inclSysMsg /* = true */) throw()
: m_message(message)
, m_errorcode(0)
{
	if(inclSysMsg)
	{
		m_errorcode = errno;
		m_message += ": ";
		m_message += strerror(m_errorcode);
	}
}

SimpleSocket::SimpleSocket( int domain, int type, int protocol)
: m_peerDisconnected(false)
{
	if( (m_socket = socket( domain, type, protocol)) < 0)
		throw SocketException("Socket creation failed (socket)");
}

SimpleSocket::SimpleSocket( int sockfd)
: m_socket(sockfd)
, m_peerDisconnected(false)
{}

SimpleSocket::~SimpleSocket()
{
	::close(m_socket);
	// on Windows do cleanup here
}

int SimpleSocket::send( const void* buffer, size_t len)
{
	int sent = ::send( m_socket, (const raw_type*) buffer, len, 0);
	if( sent < 0)
	{
		switch(errno)
		{
		case ECONNRESET:
		case ECONNREFUSED:
			m_peerDisconnected = true;
			break;
		default:
			throw SocketException("Send failed (send)");
		}
	}
	return sent;
}

int SimpleSocket::receive( void* buffer, size_t len)
{
	int ret = ::recv( m_socket, (raw_type*) buffer, len, 0);
	if( ret < 0) throw SocketException("Received failed (receive)");
	return ret;
}

int SimpleSocket::timedReceive( void* buffer, size_t len, int timeout)
{
	struct pollfd poll;
	poll.fd = m_socket;
	poll.events = POLLIN | POLLPRI | POLLRDHUP;

	int ret = ::poll( &poll, 1, timeout);

	if( ret == 0) return 0;
	if( ret < 0)  throw SocketException("timedReceive failed (poll)");

	if( poll.revents & POLLRDHUP)
		m_peerDisconnected = true;

	if( poll.revents & POLLIN || poll.revents & POLLPRI)
		return receive( buffer, len);

	return 0;
}

void SimpleSocket::shutdown( ShutdownDirection type)
{
	if( ::shutdown( m_socket, type) < 0)
		throw SocketException("Shutdown failed (shutdown)");
}

bool SimpleSocket::peerDisconnected() const
{
	return m_peerDisconnected;
}

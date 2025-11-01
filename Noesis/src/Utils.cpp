#include "main.hpp"

class NetUtils {

	std::string ip;
	int port;
	SOCKET sock = INVALID_SOCKET;
	struct sockaddr_in sock_addr;
	WSADATA wsa;
	char recvBuffer[8000];
	char sendBuffer[8000];
	bool wsa_started = false;

public:
	NetUtils(std::string ipaddr, int _port) : ip(ipaddr),port(_port) {
		if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
		{
			wsa_started = true;
		}
		else
		{
			Logger::error("WsaStartup Failed {}", WSAGetLastError());
			
		}
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(static_cast<u_short>(port));
		if (InetPtonA(AF_INET, ip.c_str(), &sock_addr.sin_addr) != 1) {
			Logger::error("Invalid Ip addr {}", ip);
		}

	
	
	}
	~NetUtils() {
		Close();
		if (wsa_started) {
			WSACleanup();
			wsa_started = false;
		}
	}


	void Close() {
			if (sock != INVALID_SOCKET) {
				closesocket(sock);
				sock = INVALID_SOCKET;
			}
		}

	bool Connect(int timeout_ms = 5000)
	{
		if (!wsa_started) return false;
		if (sock != INVALID_SOCKET)
		{
			Logger::error("Socket Already Connected");
			closesocket(sock);
			sock = INVALID_SOCKET;
		}
		sock = WSASocketA(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
		if (sock == INVALID_SOCKET) {
		Logger::error("Failed To create Socket {}",WSAGetLastError());
		return false;
		}
		if (connect(sock, reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sock_addr)) == SOCKET_ERROR) {
			Logger::error("Failed To Connect {}", WSAGetLastError());
			closesocket(sock);
			sock = INVALID_SOCKET;
			return false;
		}
	
	}
	int SendPacket(const std::string& data) {
		if (sock == INVALID_SOCKET) {
			Logger::error("Socket Not Conncted");
			return -1;
		}

		const char* buf = data.data();
		int total = static_cast<int>(data.size());
		int sent = 0;

		while (sent < total) {
			int s = send(sock, buf + sent, total - sent, 0);
			if (s == SOCKET_ERROR) {
				int err = WSAGetLastError();
				Logger::error("Failed To Send {}", err);
				return -1;
			}
			sent += s;
		}
		return sent;
	}
	int Recv(char* outBuf, int bufSize, int flags = 0) {
		if (sock == INVALID_SOCKET) return -1;
		int r = recv(sock, outBuf, bufSize, flags);
		if (r == SOCKET_ERROR) {
			Logger::error("Failed To Recieve {}", WSAGetLastError());
			return -1;
		}
		return r;
	}

	bool IsConnected() const { return sock != INVALID_SOCKET; }
};

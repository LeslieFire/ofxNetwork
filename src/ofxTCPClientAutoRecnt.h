#pragma once

#include "ofxTcpClient.h"
#include "ofMain.h"


class ofxTCPClientAutoRecnt : public ofxTCPClient, public ofThread
{
public:
	ofxTCPClientAutoRecnt(void);
	~ofxTCPClientAutoRecnt(void);

	bool setup(string ip, int port, bool blocking = false);
	bool close();
	void AutoReconnect();

	void threadedFunction(){
		while (isThreadRunning() != 0) {
			ofSleepMillis(5*1000);
			if ( _bConnected){
				if (ofxTCPClient::send(_msgTx)){
					continue;
				}else if (!ofxTCPClient::isConnected()){
					if (lock()){
						_bConnected = false;
						unlock();
					}
				}
			}else{
				if (lock()){
					AutoReconnect();

					unlock();
				}
			}
		}
	}

private:

	int		_connectTime;
	int		_deltaTime;

	string  _msgTx;
	string	_ip;
	int		_port;
	bool	_blocking;

	bool	_bConnected;
};


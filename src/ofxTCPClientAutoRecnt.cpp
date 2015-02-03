#include "ofxTCPClientAutoRecnt.h"
#include "ofxNetworkUtils.h"


ofxTCPClientAutoRecnt::ofxTCPClientAutoRecnt(void)
{
	_connectTime = 0;
	_deltaTime = 0;
}


ofxTCPClientAutoRecnt::~ofxTCPClientAutoRecnt(void)
{
	close();
}



bool ofxTCPClientAutoRecnt::setup(string ip, int port, bool blocking)
{
	_ip = ip;
	_port = port;
	_blocking = blocking;

	ofxTCPClient::setMessageDelimiter("\r\n");

	startThread(true, false);   // blocking, verbose

	if (!ofxTCPClient::setup(ip, port, blocking)){
		ofLogError("ofxTCPClientAutoRecnt") << "setup failed.";
		return false;
	}

	

	return true;
}

bool ofxTCPClientAutoRecnt::close(){
	//ofxTCPClient::close();
	stopThread();

	return true;
}

void ofxTCPClientAutoRecnt::AutoReconnect()
{
	if ( !ofxTCPClient::isConnected()){
		_deltaTime = ofGetElapsedTimeMillis() - _connectTime;
		if (_deltaTime > 5000){

			_connectTime = ofGetElapsedTimeMillis();

			if (ofxTCPClient::setup(_ip, _port, _blocking)){
				ofLogNotice("ofxTCPClientAutoRecnt") << "Reconnect success." ;
			}else{
				ofLogNotice("ofxTCPClientAutoRecnt") << "Reconnect Failed." ;
			}
		}
	}
}
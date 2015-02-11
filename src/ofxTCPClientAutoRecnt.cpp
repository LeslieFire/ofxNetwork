#include "ofxTCPClientAutoRecnt.h"
#include "ofxNetworkUtils.h"


ofxTCPClientAutoRecnt::ofxTCPClientAutoRecnt(void)
{
	_connectTime = 0;
	_deltaTime = 0;
	_bConnected = false;

	_msgTx = "";
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

	_bConnected = ofxTCPClient::setup(ip, port, blocking);
	if (!_bConnected){
		ofLogError("ofxTCPClientAutoRecnt") << "setup failed.";
	}else{
		ofLogError("ofxTCPClientAutoRecnt") << "setup OK.";
	}
	
	startThread(true, false);   // blocking, verbose
	return _bConnected;
}

bool ofxTCPClientAutoRecnt::close(){
	//ofxTCPClient::close();
	stopThread();

	return true;
}

void ofxTCPClientAutoRecnt::AutoReconnect()
{
	_deltaTime = ofGetElapsedTimeMillis() - _connectTime;
	if (_deltaTime > 5000){

		_connectTime = ofGetElapsedTimeMillis();
		_bConnected = ofxTCPClient::setup(_ip, _port, _blocking);
		if (_bConnected){
			ofLogNotice("ofxTCPClientAutoRecnt") << "Reconnect success." ;
		}else{
			ofLogNotice("ofxTCPClientAutoRecnt") << "Reconnect Failed." ;
		}
	}
}
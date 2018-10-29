#pragma once

/*
* ÊÊÅäÆ÷ÀàµÄ×´Ì¬»ú
* @date : 2018/10/28
* @author : jihang
*/

class StateMachineCode {

private:
	static int CODE_UNSTART;
	static int CODE_JOINED;
	static int CODE_INITIALED;
	static int CODE_REGISTERED;

public:
	static int getUnstartCode() {
		return CODE_UNSTART;
	}
	static int getJoinedCode() {
		return CODE_JOINED;
	}
	static int getInitialedCode() {
		return CODE_INITIALED;
	}
	static int getRegisteredCode() {
		return CODE_REGISTERED;
	}
};
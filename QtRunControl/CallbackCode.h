#pragma once

/*
* 适配器给界面发回调的MsgEvent的Code
* @date : 2018/10/28
* @author : jihang
*/

class CallbackCode {

private:
	static int CODE_SyncSuccess;
	static int CODE_SyncFailure;
	static int CODE_AnnounceSync;
	static int CODE_SyncComplete;
	static int CODE_Discover;
	static int CODE_Remove;
	static int CODE_ProcessData;

public:
	static int getSyncSuccessCode() {
		return CODE_SyncSuccess;
	}
	static int getSyncFailureCode() {
		return CODE_SyncFailure;
	}
	static int getAnnounceSyncCode() {
		return CODE_AnnounceSync;
	}
	static int getSyncCompleteCode() {
		return CODE_SyncComplete;
	}
	static int getDiscoverCode() {
		return CODE_Discover;
	}
	static int getRemoveCode() {
		return CODE_Remove;
	}
	static int getProcessDataCode() {
		return CODE_ProcessData;
	}
};
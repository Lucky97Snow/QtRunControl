
/*
* RunControlAdapter��Դ�ļ���������¹��ܣ�
* 1. ��һ�����������������Ķ��󣬽����������Ҫ��������������eventʹ��
* 2. ���������Ĵ���������main�����������������
* 3. �������Լ��Ķ����Ǹ�������extern�õģ���֤��������ȫ��Ψһ
* 4. �������Ĺ��캯���ͻص�����
* 5. ��������Ϊ������ʵ������������������ʵ���������࣬��ȡ�������������������ʾ������
* 6. ����������������ͨ�ŷ�ʽΪpost event����Ϊ�źŲۻ���Ҫ����SIGNAL������ΪQObject
* @date : 2018/10/22
* @author : jihang
*/

#include <QtWidgets/QApplication>

#include "HLAPortAdapter.h"
#include "QtRunControl.h"

#include "MsgEvent.h"
#include "ControlCode.h"

#include "CallbackCode.h"

//���������
QtRunControl *qrc;
//����������
HLAPortAdapter *ha;

HLAPortAdapter::HLAPortAdapter() {
	//��ʼ��������
	qrc = new QtRunControl();
	//��ʼ��������
	ha = this;

	//��Ա����
	m_fedFile = "testfom.fed";
	m_federationName = "ExampleFederation";
	m_federateName = "runControl";
	isPause = false;
	isEnd = false;

	//�������
	objectPause.className = "ObjectRoot.PAUSE";
	objectPause.attributeList.push_back("pause");

	objectContinue.className = "ObjectRoot.CONTINUE";
	objectContinue.attributeList.push_back("continue");

	objectEnd.className = "ObjectRoot.End";
	objectEnd.attributeList.push_back("end");

	//����
	objectA.className = "ObjectRoot.A";
	objectA.attributeList.push_back("aa");
	objectA.attributeList.push_back("ab");
	objectA.attributeList.push_back("ac");

	objectB.className = "ObjectRoot.B";
	objectB.attributeList.push_back("ba");
	objectB.attributeList.push_back("bb");
	objectB.attributeList.push_back("bc");
}

//�߼�������
void HLAPortAdapter::initial() {
	
	MsgEvent *event0 = new MsgEvent(QEvent::Type(7777));
	event0->message = initOneHandle(objectPause)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event0);

	MsgEvent *event1 = new MsgEvent(QEvent::Type(7777));
	event1->message = initOneHandle(objectContinue)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event1);

	MsgEvent *event2 = new MsgEvent(QEvent::Type(7777));
	event2->message = initOneHandle(objectEnd)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event2);

	//����
	MsgEvent *event3 = new MsgEvent(QEvent::Type(7777));
	event3->message = initOneHandle(objectA)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event3);

	MsgEvent *event4 = new MsgEvent(QEvent::Type(7777));
	event4->message = initOneHandle(objectB)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event4);
}

void HLAPortAdapter::pubAndSub() {
	//�������
	publishOne(objectPause);
	publishOne(objectContinue);
	publishOne(objectEnd);
	//����
	subscribeOne(objectA);
	subscribeOne(objectB);
}

void HLAPortAdapter::rigister() {
	//ע�������
	handlePause = registerObject(objectPause.className)->getData();
	handleContinue = registerObject(objectContinue.className)->getData();
	handleEnd = registerObject(objectEnd.className)->getData();
}

void HLAPortAdapter::simulate() {
	while (true) {
		if (isEnd) {
			del();
			resignFederation();
			//�����й���������
			destroyFederation();
			break;
		}
		if (isPause) {
			while (isPause) {
				//��ͣ״̬
			}
		}
		advanceTime(1.0);
	}
}

void HLAPortAdapter::del() {
	deleteOne(handlePause);
	deleteOne(handleContinue);
	deleteOne(handleEnd);
}

void HLAPortAdapter::send(string type) {
	/*
	//���Ϳ�����Ϣ
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create(1);
	char controlInfo[16];
	if (type == ControlType::getPauseType()) {
		sprintf(controlInfo, "you should pause");
		attributes->add(objectPause.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handlePause, attributes);
		//���ܻ����¸������յ��������й����������ס��ʵ�����ղ�����
		//���Կ��ǵ��Ƕ����Լ��������Ļ��϶�ȫ���������¸������յ�
		isPause = true;
	}
	else if (type == ControlType::getContinueType()) {
		sprintf(controlInfo, "you should continue");
		attributes->add(objectContinue.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handleContinue, attributes);
		isPause = false;
	}
	else if (type == ControlType::getEndType()) {
		sprintf(controlInfo, "you should end");
		attributes->add(objectEnd.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handleEnd, attributes);
		isEnd = true;
	}
	*/
}
//����ص�
void HLAPortAdapter::processData(RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes, const char* theTag)
	throw(RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
		RTI::FederateInternalError) {

	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getProcessDataCode()));
	QString s1 = "data data.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);

	//Ԥ��ģ��յ����������Ϣ
	//���ɶ�������ȡ��������
	//int classHandle = getObjectClass(theObject);
	//���ݶ��������ж�
	//if (classHandle == objectHandlesPause->classHandle) {
	//	cout << "i should pause" << endl;
	//	isPause = true;
	//}
	//if (classHandle == objectHandlesContinue->classHandle) {
	//	cout << "i should continue" << endl;
	//	isPause = false;
	//}
	//if (classHandle == objectHandlesEnd->classHandle) {
	//	cout << "i should end" << endl;
	//	isEnd = true;
	//}
	//�Լ��ģ��û���ӣ��յ�������Ϣ
}

//����ص�����
void HLAPortAdapter::syncSuccess(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncSuccessCode()));
	QString s1 = "Successfully registered sync point : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::HLAPortAdapter::syncFailure(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncFailureCode()));
	QString s1 = "Failed to register sync point : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::announceSync(const char *label, const char *tag) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getAnnounceSyncCode()));
	QString s1 = "Synchronization point announced : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::syncComplete(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncCompleteCode()));
	QString s1 = "Federation Synchronized : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::discover(RTI::ObjectHandle theObject,
	RTI::ObjectClassHandle theObjectClass,
	const char * theObjectName) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getDiscoverCode()));
	QString s1 = "Discoverd Object.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::remove(RTI::ObjectHandle theObject, const char *theTag) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getRemoveCode()));
	QString s1 = "Object Removed.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);
}

//���������⣡����
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	HLAPortAdapter *ha = new HLAPortAdapter();
	qrc->show();
	return a.exec();
}
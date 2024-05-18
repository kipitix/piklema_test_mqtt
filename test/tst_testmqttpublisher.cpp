#include <QtTest>

// add necessary includes here

#include "../src/textfilemqttpublisher.h"

class TestMqttPublisher : public QObject
{
	Q_OBJECT

public:
	TestMqttPublisher();
	~TestMqttPublisher();

private slots:
	void test_publish_text_success();
	void test_publish_text_no_file();
	void test_publish_text_connection_error();
};

TestMqttPublisher::TestMqttPublisher()
{
	qRegisterMetaType<TextFileMQTTPublisher::State>();
}

TestMqttPublisher::~TestMqttPublisher()
{
}

void TestMqttPublisher::test_publish_text_success()
{
	TextFileMQTTPublisher mp {};

	QSignalSpy spy(&mp, SIGNAL(stateChanged(TextFileMQTTPublisher::State)));

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Idle);

	mp.setHost("test.mosquitto.org");
	mp.setPort(1884);
	mp.setUsername("rw");
	mp.setPassword("readwrite");
	mp.setTopic("piklema/test");
	mp.setFilepath(":/test.txt");
	mp.startPublishing();

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Busy);

	spy.wait(5000);

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Success);
}

void TestMqttPublisher::test_publish_text_no_file()
{
	TextFileMQTTPublisher mp {};

	QSignalSpy spy(&mp, SIGNAL(stateChanged(TextFileMQTTPublisher::State)));

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Idle);

	mp.setHost("test.mosquitto.org");
	mp.setPort(1884);
	mp.setUsername("rw");
	mp.setPassword("readwrite");
	mp.setTopic("piklema/test");
	mp.setFilepath("bad_file_name");
	mp.startPublishing();

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Busy);

	spy.wait(5000);

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Error);
}

void TestMqttPublisher::test_publish_text_connection_error()
{
	TextFileMQTTPublisher mp {};

	QSignalSpy spy(&mp, SIGNAL(stateChanged(TextFileMQTTPublisher::State)));

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Idle);

	mp.setHost("bad_broker_name");
	mp.setPort(1884);
	mp.setUsername("rw");
	mp.setPassword("readwrite");
	mp.setTopic("piklema/test");
	mp.setFilepath(":/test.txt");
	mp.startPublishing();

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Busy);

	spy.wait(5000);

	QCOMPARE(mp.state(), TextFileMQTTPublisher::State::Error);
}

QTEST_MAIN(TestMqttPublisher)

#include "tst_testmqttpublisher.moc"

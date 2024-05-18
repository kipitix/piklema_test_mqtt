#include "textfilemqttpublisher.h"

#include <QDebug>
#include <QFile>
#include <QSharedPointer>
#include <QtMqtt/QMqttClient>

TextFileMQTTPublisher::TextFileMQTTPublisher(QObject *parent) :
	QObject { parent },
	_mqttClient { new QMqttClient { this } }
{
	connect(_mqttClient, &QMqttClient::connected, this, &TextFileMQTTPublisher::onBrokerConnected);
	connect(_mqttClient, &QMqttClient::disconnected, this, &TextFileMQTTPublisher::onBrokerDisconnected);
	connect(_mqttClient, &QMqttClient::errorChanged, this, &TextFileMQTTPublisher::onBrokerError);

	setState(State::Idle);
}

QString TextFileMQTTPublisher::host() const
{
	return _host;
}

void TextFileMQTTPublisher::setHost(const QString &host)
{
	if (_host != host)
	{
		_host = host;
		emit hostChanged(_host);
	}
}

int TextFileMQTTPublisher::port() const
{
	return _port;
}

void TextFileMQTTPublisher::setPort(int port)
{
	if (_port != port)
	{
		_port = port;
		emit portChanged(_port);
	}
}

QString TextFileMQTTPublisher::username() const
{
	return _username;
}

void TextFileMQTTPublisher::setUsername(const QString &username)
{
	if (_username != username)
	{
		_username = username;
		emit usernameChanged(_username);
	}
}

QString TextFileMQTTPublisher::password() const
{
	return _password;
}

void TextFileMQTTPublisher::setPassword(const QString &password)
{
	if (_password != password)
	{
		_password = password;
		emit passwordChanged(_password);
	}
}

QString TextFileMQTTPublisher::topic() const
{
	return _topic;
}

void TextFileMQTTPublisher::setTopic(const QString &topic)
{
	if (_topic != topic)
	{
		_topic = topic;
		emit topicChanged(_topic);
	}
}

QString TextFileMQTTPublisher::filepath() const
{
	return _filepath;
}

void TextFileMQTTPublisher::setFilepath(const QString &filepath)
{
	if (_filepath != filepath)
	{
		_filepath = filepath;
		emit filepathChanged(_filepath);
	}
}

TextFileMQTTPublisher::State TextFileMQTTPublisher::state() const
{
	return _state;
}

int TextFileMQTTPublisher::sentMessageCount() const
{
	return _sentMessageCount;
}

void TextFileMQTTPublisher::setState(State state)
{
	if (_state != state)
	{
		_state = state;
		emit stateChanged(_state);
	}
}

void TextFileMQTTPublisher::setSentMessageCount(int sentMessageCount)
{
	if (_sentMessageCount != sentMessageCount)
	{
		_sentMessageCount = sentMessageCount;
		emit sentMessageCountChanged(_sentMessageCount);
	}
}

void TextFileMQTTPublisher::startPublishing()
{
	if (state() == State::Busy)
	{
		qCritical() << QString { "Publisher busy" };
	}

	setState(State::Busy);
	setSentMessageCount(0);

	qInfo() << QString { "Start publishing" };

	_mqttClient->setHostname(_host);
	_mqttClient->setPort(_port);
	_mqttClient->setUsername(_username);
	_mqttClient->setPassword(_password);
	_mqttClient->connectToHost();
}

void TextFileMQTTPublisher::onBrokerConnected()
{
	int msgCount = 0;
	QFile file(_filepath);

	// Action to exit score
	std::shared_ptr<void> _(nullptr, [&file, this, &msgCount](...)
		{
		_mqttClient->disconnectFromHost();
		setSentMessageCount(msgCount);
		file.close(); });

	qInfo() << QString { "Connected to broker: '%1:%2'" }.arg(_host).arg(_port);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qCritical() << QString { "Can`t oper file: '%1', reason: '%2'" }.arg(_filepath, file.errorString());

		setState(State::Error);
		return;
	}

	qInfo() << QString { "File '%1' opened" }.arg(_filepath);

	qDebug() << "File content:";

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();

		qDebug() << line;

		if (_mqttClient->publish(_topic, line.toUtf8(), 0, false) == -1)
		{
			qCritical() << QString { "Failed to publish message" };

			setState(State::Error);
			return;
		}

		msgCount++;
	}

	qInfo() << QString { "Publishing finished successfuly" };

	setState(State::Success);
}

void TextFileMQTTPublisher::onBrokerDisconnected()
{
	qInfo() << QString { "Disconnected from broker" };
}

void TextFileMQTTPublisher::onBrokerError(QMqttClient::ClientError error)
{
	if (error != QMqttClient::ClientError::NoError)
	{
		qCritical() << QString { "Broker error: '%1'" }.arg(error);
		_mqttClient->disconnectFromHost();
		setState(State::Error);
	}
}

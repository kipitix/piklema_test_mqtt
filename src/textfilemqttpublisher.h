#ifndef TEXTFILEMQTTPUBLISHER_H
#define TEXTFILEMQTTPUBLISHER_H

#include <QMqttClient>
#include <QObject>

class TextFileMQTTPublisher : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
	Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
	Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
	Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
	Q_PROPERTY(QString topic READ topic WRITE setTopic NOTIFY topicChanged)
	Q_PROPERTY(QString filepath READ filepath WRITE setFilepath NOTIFY filepathChanged)

	Q_PROPERTY(State state READ state NOTIFY stateChanged)

	Q_PROPERTY(int sentMessageCount READ sentMessageCount NOTIFY sentMessageCountChanged)

public:
	enum class State
	{
		Idle,
		Busy,
		Error,
		Success,
	};

	Q_ENUM(State)

	explicit TextFileMQTTPublisher(QObject *parent = nullptr);

	QString host() const;
	void setHost(const QString &host);

	int port() const;
	void setPort(int port);

	QString username() const;
	void setUsername(const QString &username);

	QString password() const;
	void setPassword(const QString &password);

	QString topic() const;
	void setTopic(const QString &topic);

	QString filepath() const;
	void setFilepath(const QString &filepath);

	State state() const;

	int sentMessageCount() const;

	Q_INVOKABLE void startPublishing();

signals:
	void hostChanged(QString host);
	void portChanged(int port);
	void usernameChanged(QString username);
	void passwordChanged(QString password);
	void topicChanged(QString topic);
	void filepathChanged(QString filapath);

	void stateChanged(TextFileMQTTPublisher::State state);

	void sentMessageCountChanged(int sentMessageCount);

private slots:
	void onBrokerConnected();
	void onBrokerDisconnected();
	void onBrokerError(QMqttClient::ClientError);

private:
	void setState(State state);
	void setSentMessageCount(int sentMessageCount);

	QMqttClient *_mqttClient { nullptr };
	QString _host;
	int _port { 0 };
	QString _username;
	QString _password;
	QString _filepath;
	QString _topic;

	State _state { State::Idle };

	int _sentMessageCount { 0 };
};

#endif // TEXTFILEMQTTPUBLISHER_H

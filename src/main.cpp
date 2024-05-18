#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QStringListModel>

#include "textfilemqttpublisher.h"

// Model to display logs to the screen
QStringListModel logsModel {};

// Function to handle qDebug, qInfo, qWarning, qCritical messages
void redirectLogs(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	static QTextStream stdOutStream { stdout };
	static QFile logFile { QString { "logs_%1.txt" }.arg(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz")) };
	static bool logFileIsOpen = logFile.open(QIODevice::Append | QIODevice::Text);
	static QTextStream fileOutStream { &logFile };

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	static const auto endLine = Qt::endl;
#else
	static const auto endLine = endl;
#endif

	// Compose message
	auto target = QString { "%1 %2" }.arg(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss.zzz"), msg);

	// Print to console
	stdOutStream << target << endLine;

	// Put to UI
	if (logsModel.insertRow(logsModel.rowCount()))
	{
		QModelIndex index = logsModel.index(logsModel.rowCount() - 1, 0);
		logsModel.setData(index, target);
	}

	// Put to file
	if (logFileIsOpen)
	{
		fileOutStream << target << endLine;
	}
}

int main(int argc, char *argv[])
{
	qInstallMessageHandler(redirectLogs);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
	QGuiApplication app(argc, argv);

	QQuickStyle::setStyle("Material");

	qmlRegisterType<TextFileMQTTPublisher>("TextFileMQTTPublisher", 1, 0, "TextFileMQTTPublisher");

	QQmlApplicationEngine engine;
	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(
		&engine, &QQmlApplicationEngine::objectCreated,
		&app, [url](QObject *obj, const QUrl &objUrl)
		{
		if (!obj && url == objUrl)
			QCoreApplication::exit(-1); },
		Qt::QueuedConnection);

	engine.rootContext()->setContextProperty("logsModel", &logsModel);

	engine.load(url);


	return app.exec();
}

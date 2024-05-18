import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Window 2.15
import TextFileMQTTPublisher 1.0

ApplicationWindow {
	id: root

	visible: true

	Material.theme: themeCombo.theme
	Material.accent: accentCombo.accent

	width: 1024
	height: 768

	title: qsTr("MQTT text file publisher")

	QtObject {
		id: priv

		function stateName(state) {
			switch (state) {
			case TextFileMQTTPublisher.Idle:
				return "Idle";
			case TextFileMQTTPublisher.Busy:
				return "Busy";
			case TextFileMQTTPublisher.Error:
				return "Error";
			case TextFileMQTTPublisher.Success:
				return "Success";
			}

			return "Undefined";
		}
	}

	TextFileMQTTPublisher {
		id: textFileMQTTPublisher
		host: hostValue.text
		port: portValue.value
		username: usernameValue.text
		password: passwordValue.text
		topic: topicValue.text
		filepath: filepathValue.text

		onStateChanged: {
			status.text = priv.stateName(state);

			if (state !== TextFileMQTTPublisher.Busy) {
				resultDialog.visible = true;
			}
		}
	}

	Dialog {
		id: resultDialog

		x: (root.width - width) / 2
		y: (root.height - height) / 2
		parent: ApplicationWindow.overlay
		width: 300
		height: 200

		title: "Publishing finished"
		modal: true
		contentItem: Column {
			Label {
				text: "Sending result: " + priv.stateName(textFileMQTTPublisher.state)
			}
			Label {
				text: "Sent message count: " + textFileMQTTPublisher.sentMessageCount
			}
		}

		standardButtons: Dialog.Ok

		onAccepted: {
			resultDialog.visible = false;
		}
	}

	GroupBox {
		id: parametersGroup
		title: qsTr("Parameters")

		anchors {
			top: parent.top
			topMargin: 10

			left: parent.left
			leftMargin: 10
		}

		Grid {
			columns: 2
			rows: 6

			anchors.fill: parent

			horizontalItemAlignment: Text.AlignHCenter
			verticalItemAlignment: Text.AlignVCenter
			spacing: 10

			Label {
				text: qsTr("host")
			}
			TextField {
				id: hostValue
				text: "test.mosquitto.org"
				width: 300
			}
			Label {
				text: qsTr("port")
			}
			SpinBox {
				id: portValue
				value: 1884
				from: 1000
				to: 65535
				width: 300
			}
			Label {
				text: qsTr("username")
			}
			TextField {
				id: usernameValue
				text: "rw"
				width: 300
			}
			Label {
				text: qsTr("password")
			}
			TextField {
				id: passwordValue
				text: "readwrite"
				width: 300
			}
			Label {
				text: qsTr("topic")
			}
			TextField {
				id: topicValue
				text: "piklema/test"
				width: 300
			}
			Label {
				text: qsTr("filepath")
			}
			TextField {
				id: filepathValue
				text: ""
				width: 300
			}
		}
	}

	GroupBox {
		id: executionGroup
		title: qsTr("Execution")

		anchors {
			top: parametersGroup.bottom
			topMargin: 10

			left: parent.left
			leftMargin: 10
		}

		Grid {

			columns: 3
			rows: 1

			horizontalItemAlignment: Text.AlignHCenter
			verticalItemAlignment: Text.AlignVCenter
			spacing: 10

			Button {
				id: runButton
				text: "RUN"

				onPressed: {
					textFileMQTTPublisher.startPublishing();
				}
			}

			Label {
				text: qsTr("Last status: ")
			}

			Label {
				id: status
				text: "Idle"
			}
		}
	}

	GroupBox {
		id: settingsGroup
		title: qsTr("Theme settings")

		anchors {
			top: executionGroup.bottom
			topMargin: 10

			left: parent.left
			leftMargin: 10
		}

		Row {

			ComboBox {
				id: themeCombo

				property int theme: Material.System

				textRole: "text"
				valueRole: "value"

				onActivated: theme = currentValue
				Component.onCompleted: currentIndex = indexOfValue(theme)

				model: [
					{ value: Material.System, text: qsTr("System") },
					{ value: Material.Light, text: qsTr("Light") },
					{ value: Material.Dark, text: qsTr("Dark") }
				]
			}

			ComboBox {
				id: accentCombo

				property int accent: Material.Purple

				textRole: "text"
				valueRole: "value"

				onActivated: accent = currentValue
				Component.onCompleted: currentIndex = indexOfValue(accent)

				model: [
					{ value: Material.Red, text: qsTr("Red") },
					{ value: Material.Green, text: qsTr("Green") },
					{ value: Material.Blue, text: qsTr("Blue") },
					{ value: Material.Purple, text: qsTr("Purple") }
				]
			}
		}
	}

	GroupBox {
		id: logsGroup

		title: qsTr("Logs")

		anchors {
			top: parent.top
			topMargin: 10

			left: parametersGroup.right
			leftMargin: 10

			right: parent.right
			rightMargin: 10

			bottom: parent.bottom
			bottomMargin: 10
		}

		ListView {
			anchors.fill: parent

			model: logsModel
			delegate: Label {
				text: display
			}

			ScrollBar.vertical: ScrollBar {
				active: true
			}
		}
	}
}

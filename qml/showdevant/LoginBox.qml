import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.0
import "commands.js" as Commands

ModalBox {
	signal cancel()
	signal login(string user, string password, bool rememberMe)

	Keys.onPressed: {
		if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
			__login();
		else if (event.key === Qt.Key_Escape)
			cancel();
	}

	function initFocus() {
		loginTextField.forceActiveFocus();
	}

	function __login() {
		var user = loginTextField.text;
		var password = passwordTextField.text;
		var rememberMe = rememberMeCheckbox.checked;

		if (!user || !password)
			return;

		password = Qt.md5(password);

		login(user, password, rememberMe);

//		Commands.membersAuth(user, password, function(error, root) {
//			if (root.code === 0) {
//				// something goes wrong
//				// TODO warn the user with a sign or something

//				return;
//			}

//			// logged!
//			if (rememberMe) {
//				// record it in the settings
//				settings.setValue("account/user", user);
//				settings.setValue("account/password", password);
//				settings.setValue("account/autologin", rememberMe);
//			} else {
//				settings.remove("account/user");
//				settings.remove("account/password");
//				settings.remove("account/autologin");
//			}

//			login(user, root.member.token);
//		});
	}

	ShadowRectangle {
		id: loginRectangle
		color: "#FFEEEE"
		anchors.centerIn: parent
		width: 340
		height: 210

		Column {
			spacing: 4
			anchors {
				fill: parent
				leftMargin: 8
				rightMargin: 8
				topMargin: 8
			}
			Label {
				text: 'Login'
				width: parent.width
				font.pointSize: 14
			}
			Label {
				text: 'Please enter your login/password from the <a href="http://http://www.betaseries.com/">betaseries.com</a> website.'
				wrapMode: "WordWrap"
				width: parent.width
				height: 50
			}

			RowLayout {
				anchors {
					left: parent.left
					right: parent.right
					leftMargin: 10
					rightMargin: 10
				}
				spacing: 8
				Label {
					id: loginLabel
					Layout.minimumWidth: Math.max(loginLabel.contentWidth, loginPassword.contentWidth)
					text: "User:"
					horizontalAlignment: Text.AlignRight
				}
				TextField {
					id: loginTextField
					Layout.fillWidth: true
				}
			}
			RowLayout {
				anchors {
					left: parent.left
					right: parent.right
					leftMargin: 10
					rightMargin: 10
				}
				spacing: 8
				Label {
					id: loginPassword
					Layout.minimumWidth: Math.max(loginLabel.contentWidth, loginPassword.contentWidth)
					text: "Password:"
					horizontalAlignment: Text.AlignRight
				}
				TextField {
					id: passwordTextField
					Layout.fillWidth: true
					echoMode: TextInput.Password
				}
			}
			RowLayout {
				anchors {
					left: parent.left
					right: parent.right
					leftMargin: 10
					rightMargin: 10
				}
				spacing: 8
				Label {
					Layout.minimumWidth: Math.max(loginLabel.contentWidth, loginPassword.contentWidth)
					text: ""
					horizontalAlignment: Text.AlignRight
				}
				CheckBox {
					id: rememberMeCheckbox
					Layout.fillWidth: true
					text: "Remember me on this computer"

				}
			}
		}
		Row {
			anchors {
				bottom: parent.bottom
				right: parent.right
				rightMargin: 18
				bottomMargin: 8
			}
			Button {
				text: '<b>Login</b>'
				onClicked: {
					__login();
				}
			}
			Button {
				text: 'Cancel'
				onClicked: {
					cancel();
				}
			}
		}
	}
}

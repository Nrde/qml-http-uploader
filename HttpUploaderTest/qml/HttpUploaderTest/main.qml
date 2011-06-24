import QtQuick 1.0
import HttpUp 1.0

Rectangle {
    width: 240
    height: 320

    HttpUploader {
        id: theUploader

        onStateChanged: {
            if( state == HttpUploader.Done ) {
                console.log("Upload done with status " + status);
            }
        }

        onProgressChanged: {
            console.log("Upload progress = " + progress)
        }

        Component.onCompleted: {
            theUploader.open("http://test_url");
            theUploader.addField("name", "Dooom !!!!");
            theUploader.addFile("filetoUpload", "testfile.txt");
            theUploader.send()
        }
    }
}

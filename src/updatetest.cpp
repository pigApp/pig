#include <QCryptographicHash>

#include "updatetest.h"

/*
TODO: Tiene que haber dos listas en heroku. Una para linux y otra para windows.
*/

UpdateTest::UpdateTest(QObject *parent) : QObject(parent)
{
    #ifdef _WIN32
        url = url_lista_heroku_windows
    #else
        url = url_lista_heroku_linux
    #endif
}

void UpdateTest::doCheck(QObject *mRoot)
{
    _mRoot = mRoot;
    _mRoot->setProperty("status", "CHECK UPDATE");
    _mRoot->setProperty("showSpinner", true);

    if (db.open()) {
        QSqlQuery qry;
        qry.prepare("SELECT DbVersion, BinVersion FROM PigData");
        if (!qry.exec()) {
            db.close();
            emit updateErrorDb();
        } else {
            qry.next();
            currentDBVersion = qry.value(0).toInt();
            currentBinaryVersion = qry.value(1).toInt();
            db.close();
            getLastVesion();
        }
    } else {
      emit updateErrorDb();
    }
}

void UpdateTest::getLastVersion()
{
    // TODO: Tiene que haber 2 host, uno de backup.
    // TODO: Chekear si host 1 o 2 estan up. Entonces QString host = x.
    mSocket.host = host;
    mSocket.url = url;
    mSocket.fileName = '';
    mSocket.order = "getVersion";
    mSocket.doConnect();

    connect(&mSocket, SIGNAL(versionReady(QString)), this, SLOT(evaluate(QString)));
}

void UpdateTest::evaluate(QString lastVersion)
{
    QStringList last = lastVersion.split(","); // TODO: Sumar el valor de release, tambien pedirlo a la db.

    if (last[0].toInt() > currentDBVersion) {
        dbMD5 = last[2];
        newDBAvailable = true;
    }
    if (last[6].toInt() > currentBinaryVersion) {
        binaryMD5 = last[7];
        newBinaryAvailable = true;
    }

    if (newDBAvailable || newBinaryAvailable) {
        getUpdate(last[1], last[7]); //getUpdate(last[x], last[1], last[7]); // TODO: last[x] deberia ser el host.
    } else {                                                                 // Cambiar la lista en heroku. Agregando release y url(solo el path /...).
        _mRoot->setProperty("showSpinner", false);
        emit updateCallFinder();
    }
}

void UpdateTest::getUpdate(QString host, QString dbUrl, QString binaryUrl)
{
    if (newDBAvailable) {
        mSocket.host = "gamenetworkmanager.herokuapp.com"; //host // TODO: Falta el host verdadero.
        mSocket.url = dbUrl;
        mSocket.fileName = "db.sqlite";
        mSocket.order = "getUpdate";
        mSocket.doConnect();
    } else if (newBinaryAvailable) {
        mSocket.host = "gamenetworkmanager.herokuapp.com"; //host // TODO: Falta el host verdadero.
        mSocket.url = binaryUrl;
        #ifdef _WIN32
            mSocket.fileName = "pig.exe";
        #else
            mSocket.fileName = "pig";
        #endif
        mSocket.order = "getUpdate";
        mSocket.doConnect();
    }

    connect(&mSocket, SIGNAL(fileReady(QString, QString)), this, SLOT(integrityFile(QString, QString)));
}

void UpdateTest::integrityFile(QString path, QString fileName)
{
    QFile file(path+fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray fileMD5 = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);

    if (fileName == "db.sqlite") {
        if (fileMD5 == dbMD5)
            remplaceFile(path, fileName);
        else
            emit updateFail();
    } else {
        if (fileMD5 == binaryMD5)
            replaceFile(path, fileName);
        else
            emit updateFail();
    }
}

void UpdateTest::replaceFile(QString path, QString fileName)
{
    #ifdef _WIN32
        QString target = "C:/PIG/.pig/db.sqlite";
    #else
        QString target = QDir::homePath()+"/.pig/db.sqlite";
    #endif

    if (fileName == "db.sqlite") {
        newDBAvailable = false;
        QFile::copy(path+fileName, target);
        if (newBinaryAvailable)
            getUpdate(); // TODO: Pasarle los paramentros.
    } else {
        newBinaryAvailable = false;
        //mover fila
    }
}



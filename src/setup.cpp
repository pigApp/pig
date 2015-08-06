#include "setup.h"
#include "authorization.h"

Setup::Setup(const QString *PIG_PATH, QSqlDatabase *db, QWidget *parent) :
    QWidget(parent),
    _PIG_PATH(PIG_PATH),
    _db(db),
    ui(new Ui::Setup)
{
    QStringList data;

    if (_db->open()) {
        QSqlQuery query;
        query.prepare("SELECT binary, release, database, hostSite, urlSite FROM data");

        if (!query.exec()) {
            _db->close();
            //QTimer::singleShot(100, this, SLOT(error()));
        } else {
            query.next();

            data.append(query.value(0).toString());
            data.append(query.value(1).toString());
            data.append(query.value(2).toString());
            data.append(query.value(3).toString());
            data.append(query.value(5).toString());

            _db->close();
        }
    } else {
        //QTimer::singleShot(100, this, SLOT(error()));
    }

    Authorization *authorization = new Authorization(PIG_PATH, true, this);

    //QObject::connect (authorization-, &Authorization::destroyed, [&] { init_update(); });

    ui->setupUi(&data, authorization, this);
}

Setup::~Setup()
{
    delete ui;
}

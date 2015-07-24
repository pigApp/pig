#ifndef UI_H
#define UI_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>//
#include <QScrollBar>//
#include <QScrollArea>//

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QVBoxLayout *mainLayout;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(24); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        mainLayout = new QVBoxLayout(PIG);

        PIG->setFont(f);
        PIG->setPalette(p);
        PIG->setLayout(mainLayout);
    }
};

class Ui_Auth
{
public:
    QLineEdit *input;
    QPushButton *btnReset;
    QHBoxLayout *layout;

    void setupUi(QWidget *Auth)
    {
        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        input = new QLineEdit(Auth);

        btnReset = new QPushButton("RESET", Auth);
        btnReset->setFlat(true);

        layout = new QHBoxLayout(Auth);
        layout->addWidget(input);
        layout->addWidget(btnReset);

        Auth->setPalette(p);
        Auth->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QLabel *label;
    QPushButton *btnAccept;
    QPushButton *btnCancel;
    QHBoxLayout *layout;

    void setupUi(QWidget *Update)
    {
        label = new QLabel(Update);
        label->setText("UPDATE AVAILABLE");

        btnAccept = new QPushButton("ACCEPT", Update);
        btnAccept->setFlat(true);

        btnCancel = new QPushButton("CANCEL", Update);
        btnCancel->setFlat(true);

        layout = new QHBoxLayout(Update);
        layout->addWidget(label);
        layout->addWidget(btnAccept);
        layout->addWidget(btnCancel);

        Update->setLayout(layout);
    }
};

class Ui_Topbar
{
public:
    QPushButton *btnSetup;
    QGridLayout *layout;

    void setupUi(QWidget *Topbar)
    {
        QFont f(":/font-global");
        f.setPointSize(24); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        btnSetup = new QPushButton("≡", Topbar);
        btnSetup->setFont(f);
        btnSetup->setPalette(p);
        btnSetup->setFlat(true);

        layout = new QGridLayout(Topbar);
        layout->setAlignment(Qt::AlignTop);

        Topbar->setPalette(p);
        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:
    QGridLayout *__layoutTopbar;

    QPushButton *btnFilters;
    QRadioButton *btnAllp;
    QRadioButton *btn720p;
    QRadioButton *btn1080p;
    QRadioButton *btnFullMovie;
    QLineEdit *input;
    QGroupBox *groupRadio;
    QHBoxLayout *layoutRadio;
    QHBoxLayout *layoutFilters;
    QGridLayout *layout;

    QVector<QPushButton*> btn_categories_vector;

    void setupUi(QGridLayout *_layoutTopbar, QWidget *Finder)
    {
        __layoutTopbar = _layoutTopbar;

        QFont f(":/font-global");
        f.setPointSize(24); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f1(":/font-global");//
        f1.setPointSize(12); // TODO: PORCENTAJE
        f1.setCapitalization(QFont::AllUppercase);///
        f1.setBold(true);//

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(110, 10, 10, 255));
        QBrush b2(QColor(255, 255, 255, 255));
        QBrush b3(QColor(15, 15, 15, 255));//

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b1);
        p.setBrush(QPalette::Active, QPalette::Window, b1);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b1);
        p.setBrush(QPalette::Disabled, QPalette::Window, b1);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        QPalette p1;
        p1.setBrush(QPalette::Active, QPalette::Button, b);
        p1.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p1.setBrush(QPalette::Active, QPalette::Text, b2);
        p1.setBrush(QPalette::Active, QPalette::Base, b3);
        p1.setBrush(QPalette::Active, QPalette::Window, b3);
        p1.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p1.setBrush(QPalette::Active, QPalette::Highlight, b);
        p1.setBrush(QPalette::Disabled, QPalette::Button, b);
        p1.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p1.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p1.setBrush(QPalette::Disabled, QPalette::Base, b3);
        p1.setBrush(QPalette::Disabled, QPalette::Window, b3);
        p1.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p1.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        input = new QLineEdit(Finder);
        input->setFont(f);
        input->setPalette(p);
        input->setFocus();
        input->setAlignment(Qt::AlignCenter);

        btnFilters = new QPushButton("≡", Finder);
        btnFilters->setFont(f);
        btnFilters->setPalette(p1);
        btnFilters->setFlat(true);
        btnFilters->setParent(input);

        layout = new QGridLayout;
        layout->setContentsMargins(75, 0, 0, 0);// TODO: PORCENTAJE
        layout->setSpacing(0); // TODO: PORCENTAJE
        layout->addWidget(input, 0, 0);
        layout->addWidget(btnFilters, 0, 0, 0, 0, Qt::AlignLeft);

        __layoutTopbar->addLayout(layout, 0, 0);
    }

    void setupFilterUi(QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(24); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f1(":/font-global");//
        f1.setPointSize(12); // TODO: PORCENTAJE
        f1.setCapitalization(QFont::AllUppercase);//
        f1.setBold(true);//

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(110, 10, 10, 255));
        QBrush b2(QColor(255, 255, 255, 255));
        QBrush b3(QColor(15, 15, 15, 255));//

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Active, QPalette::Text, b2);
        p.setBrush(QPalette::Active, QPalette::Base, b1);
        p.setBrush(QPalette::Active, QPalette::Window, b1);
        p.setBrush(QPalette::Active, QPalette::WindowText, b2);
        p.setBrush(QPalette::Active, QPalette::Highlight, b3);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Base, b1);
        p.setBrush(QPalette::Disabled, QPalette::Window, b1);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        QPalette p1;
        p1.setBrush(QPalette::Active, QPalette::Button, b);
        p1.setBrush(QPalette::Active, QPalette::ButtonText, b2);
        p1.setBrush(QPalette::Active, QPalette::Text, b2);
        p1.setBrush(QPalette::Active, QPalette::Base, b);
        p1.setBrush(QPalette::Active, QPalette::Window, b);
        p1.setBrush(QPalette::Active, QPalette::WindowText, b3);
        p1.setBrush(QPalette::Active, QPalette::Highlight, b);
        p1.setBrush(QPalette::Disabled, QPalette::Button, b);
        p1.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);
        p1.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p1.setBrush(QPalette::Disabled, QPalette::Base, b);
        p1.setBrush(QPalette::Disabled, QPalette::Window, b);
        p1.setBrush(QPalette::Disabled, QPalette::WindowText, b3);
        p1.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        groupRadio = new QGroupBox(Finder);
        groupRadio->setStyleSheet("QGroupBox { background-color: rgba(10, 10, 10, 255);"
                                    "border: 0; margin: 0; }"
                                    "QGroupBox::title { border: 0;margin: 0; }");
        groupRadio->setFlat(true);

        btnAllp = new QRadioButton("ALL", groupRadio);
        btnAllp->setFont(f1);
        btnAllp->setPalette(p);
        btnAllp->setChecked(true);

        btn720p = new QRadioButton("720p", groupRadio);
        btn720p->setFont(f1);
        btn720p->setPalette(p);

        btn1080p = new QRadioButton("1080p", groupRadio);
        btn1080p->setFont(f1);
        btn1080p->setPalette(p);

        btnFullMovie = new QRadioButton("FULLMOVIE", groupRadio);
        btnFullMovie->setFont(f1);
        btnFullMovie->setPalette(p);

        layoutRadio = new QHBoxLayout;;
        layoutRadio->addWidget(btnAllp);
        layoutRadio->addWidget(btn720p);
        layoutRadio->addWidget(btn1080p);
        layoutRadio->addSpacing(25); // TODO: PORCENTAJE
        layoutRadio->addWidget(btnFullMovie);

        groupRadio->setLayout(layoutRadio);

        layoutFilters = new QHBoxLayout;
        layoutFilters->setContentsMargins(76, 0, 0, 0); // TODO: PORCENTAJE
        layoutFilters->addWidget(groupRadio);

        __layoutTopbar->addLayout(layoutFilters, 1, 0);
    }
};

class Ui_View
{
public:
    QLabel *label_meta[5];
    QPushButton *btnClear;
    QPushButton *btnHideInfo;
    QWidget *player;//
    QScrollArea *scrollAreaCovers;
    QGroupBox *groupCovers;
    QGroupBox *groupInfo;
    QGroupBox *groupMeta;
    QGroupBox *groupMulti;
    QGridLayout *layoutCovers;
    QVBoxLayout *layoutInfo;
    QVBoxLayout *layoutMeta;
    QHBoxLayout *layoutMulti;
    QVBoxLayout *layout;

    QVector<QPushButton*> btn_covers_vector;

    void setupUi(QWidget *View)
    {
        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        scrollAreaCovers = new QScrollArea(View);
        scrollAreaCovers->setFrameStyle(QFrame::NoFrame);
        scrollAreaCovers->setWidgetResizable(true);
        scrollAreaCovers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollAreaCovers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        groupCovers = new QGroupBox(scrollAreaCovers);
        groupCovers->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                   "border: 0; margin: 0; }"
                                   "QGroupBox::title { border: 0;margin: 0; }");
        groupCovers->setFlat(true);

        layoutCovers = new QGridLayout(groupCovers);
        layoutCovers->setHorizontalSpacing(0);
        layoutCovers->setVerticalSpacing(5);
        layoutCovers->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

        groupCovers->setLayout(layoutCovers);

        scrollAreaCovers->setWidget(groupCovers);

        layout = new QVBoxLayout(View);
        layout->addWidget(scrollAreaCovers);

        View->setLayout(layout);

        //        btnClear = new QPushButton("x", groupCovers);
        //        btnClear->setFont(f);
        //        btnClear->setPalette(p);
        //        btnClear->setFlat(true);
        //layoutCovers->addWidget(btnClear);
    }

    void setupInfoUi(const int &index,  const QStringList **data, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Text, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);

        groupInfo = new QGroupBox(View);
        groupInfo->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                 "border: 0; margin: 0; }"
                                 "QGroupBox::title { border: 0;margin: 0; }");
        groupInfo->setFlat(true);

        groupMeta = new QGroupBox(View);
        groupMeta->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                 "border: 0; margin: 0; }"
                                 "QGroupBox::title { border: 0;margin: 0; }");
        groupMeta->setFlat(true);

        groupMulti = new QGroupBox(View);
        groupMulti->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                  "border: 0; margin: 0; }"
                                  "QGroupBox::title { border: 0;margin: 0; }");
        groupMulti->setFlat(true);

        layoutMeta = new QVBoxLayout(groupMeta);
        layoutMeta->setSpacing(20); // TODO: PORCENTAJE
        layoutMeta->setSizeConstraint(QLayout::SetMaximumSize);

        QStringList labels;
        labels << "TITLE " << "CASTING " << "CATEGORY " << "QUALITY " << "TIME ";
        for (int i = 0; i < 5; i++) {
            label_meta[i] = new QLabel(groupMeta);
            label_meta[i]->setStyleSheet("QLabel{ background-color: rgba(110, 10, 10, 255);"
                                         "border: 0; margin: 0; }");
            label_meta[i]->setFont(f);
            label_meta[i]->setPalette(p);
            label_meta[i]->setText("<b>"+labels[i]+"</b>"+(**_data)[((index * 15) + (i + 1))]);
            label_meta[i]->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
            layoutMeta->addWidget(label_meta[i]);
        }

        groupMeta->setLayout(layoutMeta);

        player = new QWidget(groupMulti);
        player->setStyleSheet("QWidget { background-color: rgba(15, 15, 15, 255); }");
        player->setPalette(p);
        player->setFixedSize(QSize(640, 480)); // TODO: PORCENTAJE

        layoutMulti = new QHBoxLayout(groupMulti);
        layoutMulti->setSpacing(0);
        layoutMulti->setAlignment(Qt::AlignLeft);
        layoutMulti->addWidget(btn_covers_vector.at(index));
        layoutMulti->addWidget(btn_covers_vector.at(index+1));
        layoutMulti->insertSpacing(2, 20); // TODO: PORCENTAJE
        layoutMulti->addWidget(player);

        groupMulti->setLayout(layoutMulti);

        btnHideInfo = new QPushButton("←", groupInfo);
        btnHideInfo->setFont(f);
        btnHideInfo->setPalette(p);
        btnHideInfo->setFlat(true);

        layoutInfo = new QVBoxLayout(groupInfo);
        layoutInfo->setSpacing(20); // TODO: PORCENTAJE
        layoutInfo->setAlignment(Qt::AlignVCenter);
        layoutInfo->addWidget(btnHideInfo);
        layoutInfo->addWidget(groupMeta);
        layoutInfo->addWidget(groupMulti);

        groupInfo->setLayout(layoutInfo);

        scrollAreaCovers->setDisabled(true);
        scrollAreaCovers->hide();

        layout->addWidget(groupInfo);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Auth: public Ui_Auth {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class View: public Ui_View {};
}

QT_END_NAMESPACE

#endif

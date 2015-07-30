#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QDesktopWidget>//
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QVBoxLayout *layout_main;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(24); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));
        QBrush b2(QColor(0, 0, 0, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b2);
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);

        layout_main = new QVBoxLayout(PIG);

        PIG->setFont(f);
        PIG->setPalette(p);
        PIG->setLayout(layout_main);
    }
};

class Ui_Auth
{
public:
    QLineEdit *input;
    QPushButton *btn_reset;
    QHBoxLayout *layout;

    void setupUi(QWidget *Auth)
    {
        input = new QLineEdit(Auth);

        btn_reset = new QPushButton("RESET", Auth);
        btn_reset->setFlat(true);

        layout = new QHBoxLayout(Auth);
        layout->addWidget(input);
        layout->addWidget(btn_reset);

        Auth->setLayout(layout);
    }
};

class Ui_Update
{
public:
    QLabel *label;
    QPushButton *btn_accept;
    QPushButton *btn_cancel;
    QHBoxLayout *layout;

    void setupUi(QWidget *Update)
    {
        label = new QLabel(Update);
        label->setText("UPDATE AVAILABLE");

        btn_accept = new QPushButton("ACCEPT", Update);
        btn_accept->setFlat(true);

        btn_cancel = new QPushButton("CANCEL", Update);
        btn_cancel->setFlat(true);

        layout = new QHBoxLayout(Update);
        layout->addWidget(label);
        layout->addWidget(btn_accept);
        layout->addWidget(btn_cancel);

        Update->setLayout(layout);
    }
};

class Ui_Topbar
{
public:
    QPushButton *btn_setup;
    QGridLayout *layout;

    void setupUi(QWidget *Topbar)
    {
        btn_setup = new QPushButton("≡", Topbar);
        btn_setup->setFlat(true);

        layout = new QGridLayout(Topbar);
        layout->setMargin(0);
        layout->setAlignment(Qt::AlignTop);
        layout->addWidget(btn_setup, 0, 1);

        Topbar->setLayout(layout);
    }
};

class Ui_Finder
{
public:
    QGridLayout *__layout_topbar;

    QPalette p;
    QLineEdit *input;
    QPushButton *btn_filters;
    QRadioButton *radio_allp;
    QRadioButton *radio_720p;
    QRadioButton *radio_1080p;
    QRadioButton *radio_fullMovie;
    QHBoxLayout *layout_radios;
    QHBoxLayout *layout_filters;
    QGridLayout *layout;

    QVector<QPushButton*> btn_categories_vector;

    void setupUi(QGridLayout *_layout_topbar, QWidget *Finder)
    {
        __layout_topbar = _layout_topbar;

        QBrush b(QColor(15, 15, 15, 255));
        QBrush b1(QColor(255, 255, 255, 255));
        QBrush b2(QColor(0, 0, 0, 255));

        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Highlight, b2);
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);
        p.setBrush(QPalette::Disabled, QPalette::Base, b);
        p.setBrush(QPalette::Disabled, QPalette::Window, b);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, b2);
        p.setBrush(QPalette::Disabled, QPalette::Text, b2);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, b);
        p.setBrush(QPalette::Disabled, QPalette::Button, b);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, b2);

        input = new QLineEdit(Finder);
        input->setPalette(p);
        input->setAlignment(Qt::AlignCenter);

        btn_filters = new QPushButton("≡", Finder);
        btn_filters->setPalette(p);
        btn_filters->setFlat(true);
        btn_filters->setParent(input);

        layout = new QGridLayout;
        layout->setContentsMargins(82, 0, 2, 0);// TODO: PORCENTAJE
        layout->setSpacing(0); // TODO: PORCENTAJE
        layout->addWidget(input, 0, 0);
        layout->addWidget(btn_filters, 0, 0, 0, 0, Qt::AlignLeft);

        __layout_topbar->addLayout(layout, 0, 0);

        input->setFocus();
    }

    void setupFilterUi(QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(12); // TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush b(QColor(50, 50, 50, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);

        radio_allp = new QRadioButton("ALL", Finder);
        radio_allp->setFont(f);
        radio_allp->setPalette(p);
        radio_allp->setChecked(true);

        radio_720p = new QRadioButton("720p", Finder);
        radio_720p->setFont(f);
        radio_720p->setPalette(p);

        radio_1080p = new QRadioButton("1080p", Finder);
        radio_1080p->setFont(f);
        radio_1080p->setPalette(p);

        radio_fullMovie = new QRadioButton("FULLMOVIE", Finder);
        radio_fullMovie->setFont(f);
        radio_fullMovie->setPalette(p);
        radio_fullMovie->setAutoExclusive(false);

        layout_radios = new QHBoxLayout;;
        layout_radios->addWidget(radio_allp);
        layout_radios->addWidget(radio_720p);
        layout_radios->addWidget(radio_1080p);
        layout_radios->addSpacing(25); // TODO: PORCENTAJE
        layout_radios->addWidget(radio_fullMovie);

        layout_filters = new QHBoxLayout;
        layout_filters->setContentsMargins(115, 0, 10, 0); // TODO: PORCENTAJE
        layout_filters->addLayout(layout_radios);

        __layout_topbar->addLayout(layout_filters, 1, 0);
    }
};

class Ui_Setup
{
public:
    QLabel *label;
    QHBoxLayout *layout;

    void setupUi(QWidget *Setup)
    {
        label = new QLabel(Setup);
        label->setText("TEST");
        label->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));

        layout = new QHBoxLayout(Setup);
        layout->addWidget(label);

        Setup->setLayout(layout);
    }
};

class Ui_View
{
public:
    QPalette p;
    QLabel *label_meta[5];
    QLabel *cover;
    QLabel *backCover;
    QPushButton *btn_clear;
    QPushButton *btn_hideInfo;
    QWidget *player;//
    QScrollArea *scrollArea_covers;
    QGroupBox *group_covers;
    QGroupBox *group_info;
    QGridLayout *layout_covers;
    QVBoxLayout *layout_meta;
    QHBoxLayout *layout_multi;
    QVBoxLayout *layout_info;
    QVBoxLayout *layout;

    QVector<QPushButton*> btn_vector_covers;

    void setupUi(QWidget *View)
    {
        QBrush b(QColor(10, 10, 10, 255));
        QBrush b1(QColor(255, 255, 255, 255));

        p.setBrush(QPalette::Active, QPalette::Base, b);
        p.setBrush(QPalette::Active, QPalette::Window, b);
        p.setBrush(QPalette::Active, QPalette::WindowText, b1);
        p.setBrush(QPalette::Active, QPalette::Text, b1);
        p.setBrush(QPalette::Active, QPalette::Button, b);
        p.setBrush(QPalette::Active, QPalette::ButtonText, b1);

        scrollArea_covers = new QScrollArea(View);
        scrollArea_covers->setFrameStyle(QFrame::NoFrame);
        scrollArea_covers->setWidgetResizable(true);
        scrollArea_covers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_covers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        group_covers = new QGroupBox(scrollArea_covers);
        group_covers->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                    "border: 0; margin: 0; }"
                                    "QGroupBox::title { border: 0;margin: 0; }");
        group_covers->setFlat(true);

        layout_covers = new QGridLayout(group_covers);
        layout_covers->setContentsMargins(70, 0, 0, 20);
        layout_covers->setHorizontalSpacing(0);
        layout_covers->setVerticalSpacing(5);// TODO: PORCENTAJE
        layout_covers->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        group_covers->setLayout(layout_covers);

        scrollArea_covers->setWidget(group_covers);

        layout = new QVBoxLayout(View);
        layout->addWidget(scrollArea_covers);

        View->setLayout(layout);

        //        btn_clear = new QPushButton("x", group_covers);
        //        btn_clear->setFont(f);
        //        btn_clear->setPalette(p);
        //        btn_clear->setFlat(true);
        //layout_covers->addWidget(btn_clear);
    }

    void setupInfoUi(const int &index, const QString &path, const QStringList **data, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(24); //TODO: PORCENTAJE
        f.setCapitalization(QFont::AllUppercase);

        group_info = new QGroupBox(View);
        group_info->setStyleSheet("QGroupBox{ background-color: rgba(10, 10, 10, 255);"
                                  "border: 0; margin: 0; }"
                                  "QGroupBox::title { border: 0;margin: 0; }");
        group_info->setFlat(true);

        layout_meta = new QVBoxLayout;
        layout_meta->setSpacing(10); // TODO: PORCENTAJE
        layout_meta->setSizeConstraint(QLayout::SetMaximumSize);

        for (int i = 0; i < 5; i++) {
            label_meta[i] = new QLabel(group_info);
            label_meta[i]->setStyleSheet("QLabel{ background-color: rgba(10, 10, 10, 255);"
                                         "border: 0; margin: 0; }");
            label_meta[i]->setFont(f);
            label_meta[i]->setPalette(p);
            label_meta[i]->setText(" "+(**_data)[((index * 17) + i)]);
            layout_meta->addWidget(label_meta[i]);
        }

        QPixmap pixmap_cover(path);
        cover = new QLabel(group_info);
        cover->setPixmap(pixmap_cover.scaled(335, 480, Qt::KeepAspectRatio));//

        QPixmap pixmap_backCover(":/img-cover-back");
        backCover = new QLabel(group_info);
        backCover->setPixmap(pixmap_backCover);//

        player = new QWidget(group_info);
        player->setStyleSheet("QWidget { background-color: rgba(15, 15, 15, 255); }");
        player->setFixedSize(QSize(640, 480)); // TODO: PORCENTAJE

        layout_multi = new QHBoxLayout;
        layout_multi->setSpacing(0);
        layout_multi->setAlignment(Qt::AlignLeft);
        layout_multi->addWidget(cover);
        layout_multi->insertSpacing(1, 20); // TODO: PORCENTAJE
        layout_multi->addWidget(backCover);
        layout_multi->insertSpacing(3, 20); // TODO: PORCENTAJE
        layout_multi->addWidget(player);

        btn_hideInfo = new QPushButton("", group_info);
        btn_hideInfo->setFont(f);
        btn_hideInfo->setPalette(p);
        btn_hideInfo->setFlat(true);

        layout_info = new QVBoxLayout(group_info);
        layout_info->setSpacing(110); // TODO: PORCENTAJE
        layout_info->setAlignment(Qt::AlignVCenter);
        layout_info->addWidget(btn_hideInfo);
        layout_info->addLayout(layout_meta);
        layout_info->addLayout(layout_multi);

        group_info->setLayout(layout_info);

        scrollArea_covers->setDisabled(true);
        scrollArea_covers->hide();

        layout->addWidget(group_info);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Auth: public Ui_Auth {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class Setup: public Ui_Setup {};
    class View: public Ui_View {};
}

QT_END_NAMESPACE

#endif

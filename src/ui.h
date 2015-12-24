#ifndef UI_H
#define UI_H

#include "player.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QPixmap>
#include <QScrollArea>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>//
#include <QDesktopServices>//
#include <QAbstractItemView>
#include <QCompleter>
#include <QClipboard>
#include <QToolTip>

#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_PIG
{
public:
    QPushButton *b_back;
    QPushButton *b_minimize;
    QPushButton *b_quit;
    QVBoxLayout *main_layout;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(16);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f_toolTip(":/font-global");
        f_toolTip.setPointSize(12);//TODO: PORCENTAJE.
        f_toolTip.setCapitalization(QFont::AllUppercase);
        f_toolTip.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_toolTip = QToolTip::palette();
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipBase, br_black);
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipText, br_white);

        QToolTip::setFont(f_toolTip);
        QToolTip::setPalette(p_toolTip);

        b_back = new QPushButton(QIcon(":/icon-back"), NULL, PIG);
        //b_back->setFixedSize(QSize(16, 16));
        b_back->setGeometry(1863, 7, 10, 14);//TODO: PORCENTAJE.
        b_back->setToolTip("BACK");
        b_back->setMouseTracking(true);
        b_back->setFlat(true);
        b_back->setHidden(true);
        b_back->setParent(PIG);

        b_minimize = new QPushButton(QIcon(":/icon-minimize"), NULL, PIG);
        //b_minimize->setFixedSize(QSize(16, 16));//TODO: PORCENTAJE.
        b_minimize->setGeometry(1880, 5, 16, 16);//TODO: PORCENTAJE.
        b_minimize->setMouseTracking(true);
        b_minimize->setFlat(true);
        b_minimize->setParent(PIG);

        b_quit = new QPushButton(QIcon(":/icon-quit"), NULL, PIG);
        //b_quit->setFixedSize(QSize(32, 32));
        b_quit->setGeometry(1900, 5, 16, 16);//TODO: PORCENTAJE.
        b_quit->setMouseTracking(true);
        b_quit->setFlat(true);
        b_quit->setParent(PIG);

        main_layout = new QVBoxLayout(PIG);

        PIG->setFont(f);
        PIG->setPalette(p);
        PIG->setLayout(main_layout);
    }
};

class Ui_Authotization
{
public:
    QPalette p;
    QPalette p_error;
    QLabel *lb;
    QLineEdit *input;
    QPushButton *b_reset;
    QHBoxLayout *l_input;
    QVBoxLayout *l;

    void setupUi(bool set, bool hasSet, QWidget *Authorization)
    {
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_gray(QColor(122, 122, 122, 255));
        QBrush br_red(QColor(115, 10, 10, 255));

        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_gray);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_gray);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_gray);

        p_error.setBrush(QPalette::Active, QPalette::Base, br_red);
        p_error.setBrush(QPalette::Active, QPalette::Window, br_red);
        p_error.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_error.setBrush(QPalette::Active, QPalette::Highlight, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Base, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Window, br_red);
        p_error.setBrush(QPalette::Disabled, QPalette::Text, br_white);
        p_error.setBrush(QPalette::Disabled, QPalette::Highlight, br_red);

        QString str;
        if (set)
            str = "PASSWORD";
        else
            str = "ENTER PASSWORD";

        lb = new QLabel(str, Authorization);
        if (!set)
            lb->setAlignment(Qt::AlignHCenter);

        input = new QLineEdit(Authorization);
        input->setPalette(p);
        input->setEchoMode(QLineEdit::Password);
        if (!set)
            input->setFocus();
        input->setDisabled(set && hasSet);
        input->setAlignment(Qt::AlignCenter);

        b_reset = new QPushButton(Authorization);
        if  (hasSet)
            b_reset->setIcon(QIcon(":/icon-reset"));
        else
            b_reset->setIcon(QIcon(":/icon-reset-dark"));
        b_reset->setToolTip("RESET PASSWORD");
        b_reset->setFlat(true);
        b_reset->setDisabled(!hasSet);
        b_reset->setHidden(!set);

        l_input = new QHBoxLayout;
        l_input->setSpacing(10);//TODO: PORCENTAJE.
        if (set)
            l_input->addWidget(lb);
        l_input->addWidget(input);
        l_input->addWidget(b_reset);

        l = new QVBoxLayout(Authorization);
        l->setAlignment(Qt::AlignCenter);
        if (!set)
            l->addWidget(lb);
        l->addLayout(l_input);

        Authorization->setLayout(l);
    }
};

class Ui_Update
{
public:
    QLabel *lb;
    QPushButton *b_1;
    QPushButton *b_2;
    QHBoxLayout *l;

    void setupUi(QWidget *Update)
    {
        lb = new QLabel("UPDATE AVAILABLE", Update);

        b_1 = new QPushButton(QIcon(":/icon-install"), NULL, Update);
        b_1->setToolTip("ACCEPT UPDATE");
        b_1->setFlat(true);

        b_2 = new QPushButton(QIcon(":/icon-cancel"), NULL, Update);
        b_2->setToolTip("CANCEL UPDATE");
        b_2->setFlat(true);

        l = new QHBoxLayout(Update);
        l->setAlignment(Qt::AlignHCenter);
        l->addWidget(lb);
        l->addSpacing(10);//TODO: PORCENTAJE
        l->addWidget(b_1);
        l->addWidget(b_2);

        Update->setLayout(l);
    }
};

class Ui_Topbar
{
public:
    QPushButton *b_setup;
    QGridLayout *l;

    void setupUi(QWidget *Topbar)
    {
        b_setup = new QPushButton(QIcon(":/icon-setup"), NULL, Topbar);
        b_setup->setToolTip("SETUP");
        b_setup->setFlat(true);

        l = new QGridLayout(Topbar);
        l->setSpacing(0);
        l->setContentsMargins(0, 10, 46, 0);//TODO: PORCENTAJE.
        l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        l->addWidget(b_setup, 0, 1);

        Topbar->setLayout(l);
    }
};

class Ui_Finder
{
public:
    QGridLayout *_l_topbar;

    QPalette p_fullMovie;
    QPalette p_fullMovie_active;
    QLineEdit *input;
    QPushButton *b_filters;
    QComboBox *cb_categories;
    QComboBox *cb_pornstars;
    QComboBox *cb_quality;
    QPushButton *b_fullMovie;
    QRadioButton *r_apply_on_results;
    QHBoxLayout *l_filters;
    QGridLayout *l;

    QString cb_style;
    QString cb_style_active;

    void setupUi(QStringList *movies, QGridLayout *l_topbar, QWidget *Finder)
    {
        _l_topbar = l_topbar;
        b_filters = NULL;
        cb_categories = NULL;

        QFont f(":/font-global");
        f.setPointSize(12);//TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_gray(QColor(122, 122, 122, 255));
        QBrush br_red(QColor(115, 10, 10, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_red);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_completer;
        p_completer.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p_completer.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p_completer.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_completer.setBrush(QPalette::Active, QPalette::Highlight, br_gray);
        p_completer.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p_completer.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p_completer.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_completer.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);

        p_fullMovie.setBrush(QPalette::Active, QPalette::Button, br_alternate_base);
        p_fullMovie.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_fullMovie.setBrush(QPalette::Disabled, QPalette::Button, br_alternate_base);
        p_fullMovie.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        p_fullMovie_active.setBrush(QPalette::Active, QPalette::Button, br_gray);
        p_fullMovie_active.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_fullMovie_active.setBrush(QPalette::Disabled, QPalette::Button, br_gray);
        p_fullMovie_active.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QCompleter *completer = new QCompleter(*movies, Finder);
        completer->setMaxVisibleItems(10);
        completer->setFilterMode(Qt::MatchContains);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);
        completer->popup()->setFont(f);
        completer->popup()->setPalette(p_completer);

        input = new QLineEdit(Finder);
        input->setPalette(p);
        input->setMaxLength(65);
        input->setMinimumHeight(45);//TODO: PORCENTAJE.
        input->setFrame(QFrame::NoFrame);
        input->setCompleter(completer);
        input->setAlignment(Qt::AlignCenter);

        b_filters = new QPushButton(QIcon(":/icon-filters"), NULL, Finder);
        b_filters->setGeometry(14, 11, 24, 24);//TODO: PORCENTAJE.
        b_filters->setCursor(Qt::ArrowCursor);
        b_filters->setFlat(true);
        b_filters->setParent(input);

        l = new QGridLayout;
        l->setSpacing(0);
        l->setContentsMargins(83, 0, 12, 0);//TODO: PORCENTAJE.
        l->addWidget(input, 0, 0);

        _l_topbar->addLayout(l, 0, 0);
    }

    void setupFilterUi(const QStringList *categories, const QStringList *pornstars, QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(12);//TODO: PORCENTAJE.
        f.setBold(true);

        cb_style = "QComboBox { color: rgb(255, 255, 255); \
                    background-color: rgb(15, 15, 15); \
                    selection-background-color: rgb(122, 122, 122); } \
                    QComboBox QAbstractItemView { color: rgb(255, 255, 255); \
                    background-color: rgb(15, 15, 15); \
                    border-radius: 0px; }";

        cb_style_active = "QComboBox { color: rgb(255, 255, 255); \
                           background-color: rgb(122, 122, 122); \
                           selection-background-color: rgb(122, 122, 122); } \
                           QComboBox QAbstractItemView { color: rgb(255, 255, 255); \
                           background-color: rgb(15, 15, 15); \
                           border-radius: 0px; }";

        cb_categories = new QComboBox(Finder); //TODO: ESTETICA.
        cb_categories->setFont(f);
        cb_categories->setStyleSheet(cb_style);
        cb_categories->addItem("                  CATEGORY");
        for (int i = 0; i < (*categories).size(); i++)
            cb_categories->addItem((*categories)[i]);

        cb_pornstars = new QComboBox(Finder);
        cb_pornstars->setFont(f);
        cb_pornstars->setStyleSheet(cb_style);
        cb_pornstars->addItem("                  PORNSTAR");
        for (int i = 0; i < (*pornstars).size(); i++)
            cb_pornstars->addItem((*pornstars)[i]);

        cb_quality = new QComboBox(Finder);
        cb_quality->setFont(f);
        cb_quality->setStyleSheet(cb_style);
        cb_quality->addItem("                  QUALITY");
        cb_quality->addItem("720p");
        cb_quality->addItem("1080p");

        b_fullMovie = new QPushButton("FULLMOVIE", Finder);
        b_fullMovie->setFont(f);
        b_fullMovie->setPalette(p_fullMovie);
        b_fullMovie->setCheckable(true);
        b_fullMovie->setChecked(false);
        b_fullMovie->setAutoFillBackground(true);

        r_apply_on_results = new QRadioButton("APPLY ON RESULTS", Finder);
        r_apply_on_results->setFont(f);
        r_apply_on_results->setCheckable(true);
        r_apply_on_results->setChecked(true);

        l_filters = new QHBoxLayout;
        l_filters->setSpacing(10);//TODO: PORCENTAJE.
        l_filters->setContentsMargins(84, 10, 13, 0);//TODO: PORCENTAJE.
        l_filters->addWidget(cb_categories);
        l_filters->addWidget(cb_pornstars);
        l_filters->addWidget(cb_quality);
        l_filters->addWidget(b_fullMovie);
        l_filters->addWidget(r_apply_on_results, 0, Qt::AlignHCenter);

        _l_topbar->addLayout(l_filters, 1, 0);
    }
};

class Ui_Setup
{
public:
    QPalette p_torrent;
    QPalette p_torrent_edit;
    QPalette p_wallet;
    QPalette p_wallet_copied;
    QLabel *lb_data_binary;
    QLabel *lb_data_database;
    QLabel *lb_data_license;
    QLabel *lb_torrent_ports;
    QLabel *lb_contribute_torrent;
    QLabel *lb_contribute_code;
    QLabel *lb_contribute_bug;
    QLabel *lb_contribute_support;
    QLabel *lb_contribute_wallet;
    QLineEdit *input_torrent_port_1;
    QLineEdit *input_torrent_port_2;
    QRadioButton *r_folder_covers;
    QRadioButton *r_folder_torrents;
    QRadioButton *r_folder_movies;
    QPushButton *b_folder_covers_reset;
    QPushButton *b_folder_torrents_reset;
    QPushButton *b_folder_movies_reset;
    QPushButton *b_torrent_ports_reset;
    QPushButton *b_contribute_torrent;
    QPushButton *b_contribute_code;
    QPushButton *b_contribute_bug;
    QPushButton *b_contribute_support;
    QPushButton *b_contribute_copy_wallet;
    QWidget *w_data;
    QWidget *w_authorization;
    QWidget *w_folder;
    QWidget *w_torrent;
    QWidget *w_contribute;
    QHBoxLayout *l_data;
    QHBoxLayout *l_authorization;
    QHBoxLayout *l_folder_covers;
    QHBoxLayout *l_folder_torrents;
    QHBoxLayout *l_folder_movies;
    QVBoxLayout *l_folder;
    QHBoxLayout *l_torrent;
    QHBoxLayout *l_contribute_torrent;
    QHBoxLayout *l_contribute_code;
    QHBoxLayout *l_contribute_bug;
    QHBoxLayout *l_contribute_support;
    QVBoxLayout *l_contribute;
    QVBoxLayout *l;

    QClipboard *clipboard;

    void setupUi(QStringList *data, bool **keep_covers, bool **keep_torrents,
                 bool **keep_movies, int **torrent_port_1, int **torrent_port_2,
                 QWidget *authorization, QWidget *Setup)
    {
        QFont f_wallet(":/font-global");
        f_wallet.setPointSize(16);//TODO: PORCENTAJE.
        f_wallet.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_gray(QColor(122, 122, 122, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);

        p_torrent.setBrush(QPalette::Active, QPalette::Base, br_gray);
        p_torrent.setBrush(QPalette::Active, QPalette::Window, br_gray);
        p_torrent.setBrush(QPalette::Active, QPalette::Text, br_base);
        p_torrent.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Base, br_gray);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Window, br_gray);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_torrent.setBrush(QPalette::Disabled, QPalette::Highlight, br_gray);

        p_torrent_edit.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p_torrent_edit.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p_torrent_edit.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_torrent_edit.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_torrent_edit.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p_torrent_edit.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p_torrent_edit.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_torrent_edit.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);

        p_wallet.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p_wallet.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p_wallet.setBrush(QPalette::Active, QPalette::WindowText, br_gray);
        p_wallet.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p_wallet.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p_wallet.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);

        p_wallet_copied.setBrush(QPalette::Active, QPalette::Base, br_dark);
        p_wallet_copied.setBrush(QPalette::Active, QPalette::Window, br_dark);
        p_wallet_copied.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::Base, br_dark);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::Window, br_dark);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);

        w_data = new QWidget(Setup);
        w_data->setPalette(p);
        w_data->setAutoFillBackground(true);

        lb_data_binary = new QLabel("PIG "+(*data)[0]+"."+(*data)[1], w_data);
        lb_data_database = new QLabel("DATABASE "+(*data)[2], w_data);
        lb_data_license = new QLabel("GPL 2.0", w_data);

        l_data = new QHBoxLayout(w_data);
        l_data->setSpacing(20);//TODO: PORCENTAJE.
        l_data->addWidget(lb_data_binary);
        l_data->addWidget(lb_data_database);
        l_data->addWidget(lb_data_license);
        l_data->addStretch();

        w_data->setLayout(l_data);

        w_authorization = new QWidget(Setup);
        w_authorization->setPalette(p);
        w_authorization->setAutoFillBackground(true);

        l_authorization = new QHBoxLayout(w_authorization);
        l_authorization->setMargin(0);
        l_authorization->addWidget(authorization);

        w_authorization->setLayout(l_authorization);

        w_folder = new QWidget(Setup);
        w_folder->setPalette(p);
        w_folder->setAutoFillBackground(true);

        r_folder_covers = new QRadioButton("KEEP COVERS", w_folder);
        r_folder_covers->setChecked(**keep_covers);
        r_folder_covers->setAutoExclusive(false);

        b_folder_covers_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_covers_reset->setToolTip("CLEAN FOLDER");
        b_folder_covers_reset->setFlat(true);

        r_folder_torrents = new QRadioButton("KEEP TORRENTS", w_folder);
        r_folder_torrents->setChecked(**keep_torrents);
        r_folder_torrents->setAutoExclusive(false);

        b_folder_torrents_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_torrents_reset->setToolTip("CLEAN FOLDER");
        b_folder_torrents_reset->setFlat(true);

        r_folder_movies = new QRadioButton("KEEP MOVIES", w_folder);
        r_folder_movies->setChecked(**keep_movies);
        r_folder_movies->setAutoExclusive(false);

        b_folder_movies_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_movies_reset->setToolTip("CLEAN FOLDER");
        b_folder_movies_reset->setFlat(true);

        l_folder_covers = new QHBoxLayout;
        l_folder_covers->addWidget(r_folder_covers);
        l_folder_covers->insertStretch(1);
        l_folder_covers->addWidget(b_folder_covers_reset);

        l_folder_torrents = new QHBoxLayout;
        l_folder_torrents->addWidget(r_folder_torrents);
        l_folder_torrents->insertStretch(1);
        l_folder_torrents->addWidget(b_folder_torrents_reset);

        l_folder_movies = new QHBoxLayout;
        l_folder_movies->addWidget(r_folder_movies);
        l_folder_movies->insertStretch(1);
        l_folder_movies->addWidget(b_folder_movies_reset);

        l_folder = new QVBoxLayout(w_folder);
        l_folder->addLayout(l_folder_covers);
        l_folder->addLayout(l_folder_torrents);
        l_folder->addLayout(l_folder_movies);

        w_folder->setLayout(l_folder);

        w_torrent = new QWidget(Setup);
        w_torrent->setPalette(p);
        w_torrent->setAutoFillBackground(true);

        lb_torrent_ports = new QLabel("PORTS", w_torrent);

        QValidator *validPort = new QIntValidator(0, 65000, Setup);

        input_torrent_port_1 = new QLineEdit(QString::number(**torrent_port_1), w_torrent);
        input_torrent_port_1->setPalette(p_torrent);
        input_torrent_port_1->setValidator(validPort);
        input_torrent_port_1->setAlignment(Qt::AlignCenter);

        input_torrent_port_2 = new QLineEdit(QString::number(**torrent_port_2), w_torrent);
        input_torrent_port_2->setPalette(p_torrent);
        input_torrent_port_2->setValidator(validPort);
        input_torrent_port_2->setAlignment(Qt::AlignCenter);

        b_torrent_ports_reset = new QPushButton(w_torrent);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset-dark"));
        else
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset"));
        b_torrent_ports_reset->setToolTip("RESET PORTS");
        b_torrent_ports_reset->setFlat(true);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setDisabled(true);

        l_torrent = new QHBoxLayout(w_torrent);
        l_torrent->setSpacing(10);//TODO: PORCENTAJE.
        l_torrent->addWidget(lb_torrent_ports);
        l_torrent->addWidget(input_torrent_port_1);
        l_torrent->addWidget(input_torrent_port_2);
        l_torrent->addWidget(b_torrent_ports_reset);

        w_torrent->setLayout(l_torrent);

        w_contribute = new QWidget(Setup);
        w_contribute->setPalette(p);
        w_contribute->setAutoFillBackground(true);

        lb_contribute_torrent = new QLabel("TORRENT", w_contribute);

        b_contribute_torrent = new QPushButton(QIcon(":/icon-browser"), NULL, w_contribute);
        b_contribute_torrent->setToolTip("CONTRIBUTE TORRENT");
        b_contribute_torrent->setFlat(true);

        l_contribute_torrent = new QHBoxLayout;
        l_contribute_torrent->addWidget(lb_contribute_torrent);
        l_contribute_torrent->insertStretch(1);
        l_contribute_torrent->addWidget(b_contribute_torrent);

        lb_contribute_code = new QLabel("CODE", w_contribute);

        b_contribute_code = new QPushButton(QIcon(":/icon-browser"), NULL, w_contribute);
        b_contribute_code->setToolTip("CONTRIBUTE CODE");
        b_contribute_code->setFlat(true);

        l_contribute_code = new QHBoxLayout;
        l_contribute_code->addWidget(lb_contribute_code);
        l_contribute_code->insertStretch(1);
        l_contribute_code->addWidget(b_contribute_code);

        lb_contribute_bug = new QLabel("BUG", w_contribute);

        b_contribute_bug = new QPushButton(QIcon(":/icon-browser"), NULL, w_contribute);
        b_contribute_bug->setToolTip("REPORT BUG");
        b_contribute_bug->setFlat(true);;

        l_contribute_bug = new QHBoxLayout;
        l_contribute_bug->addWidget(lb_contribute_bug);
        l_contribute_bug->insertStretch(1);
        l_contribute_bug->addWidget(b_contribute_bug);

        lb_contribute_support = new QLabel("SUPPORT", w_contribute);

        lb_contribute_wallet = new QLabel("aCef523jkD8239OEwMfn7692BH23489BsJk", w_contribute);
        lb_contribute_wallet->setFont(f_wallet);
        lb_contribute_wallet->setPalette(p_wallet);
        lb_contribute_wallet->setTextInteractionFlags(Qt::TextSelectableByMouse);
        lb_contribute_wallet->setAutoFillBackground(true);
        lb_contribute_wallet->setHidden(true);

        b_contribute_support = new QPushButton(QIcon(":/icon-more"), NULL, w_contribute);
        b_contribute_support->setToolTip("BITCOIN");
        b_contribute_support->setFlat(true);

        b_contribute_copy_wallet= new QPushButton(QIcon(":/icon-copy"), NULL, w_contribute);
        b_contribute_copy_wallet->setToolTip("COPY WALLET");
        b_contribute_copy_wallet->setFlat(true);
        b_contribute_copy_wallet->setHidden(true);

        l_contribute_support = new QHBoxLayout;
        l_contribute_support->addWidget(lb_contribute_support);
        l_contribute_support->addSpacing(5);//TODO: PORCENTAJE.
        l_contribute_support->addWidget(lb_contribute_wallet);
        l_contribute_support->addWidget(b_contribute_copy_wallet);
        l_contribute_support->insertStretch(5);//TODO: PORCENTAJE.
        l_contribute_support->addWidget(b_contribute_support);

        l_contribute = new QVBoxLayout(w_contribute);
        l_contribute->addLayout(l_contribute_torrent);
        l_contribute->addLayout(l_contribute_code);
        l_contribute->addLayout(l_contribute_bug);
        l_contribute->addLayout(l_contribute_support);

        w_contribute->setLayout(l_contribute);

        l = new QVBoxLayout(Setup);
        l->setSpacing(10);//TODO: PORCENTAJE.
        l->setContentsMargins(80, 13, 80, 10);//TODO: PORCENTAJE.
        l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        l->addWidget(w_data);
        l->addWidget(w_authorization);
        l->addWidget(w_folder);
        l->addWidget(w_torrent);
        l->addWidget(w_contribute);
        l->addStretch();

        Setup->setLayout(l);
    }
};

class Ui_View
{
public:
    QScrollArea *sa_covers;
    QLabel *lb_download;
    QLabel *lb_info_metadata[5];
    QLabel *lb_info_cover;
    QLabel *lb_info_backCover;
    //QLabel *lb_info_player;//
    QComboBox *cb_info_scenes;
    //Player *player_info;
    QWidget *w_covers;
    QWidget *w_info;
    QGridLayout *l_covers;
    QVBoxLayout *l_info_metadata;
    QVBoxLayout *l_info_scenes;//
    QHBoxLayout *l_info_multimedia;
    QVBoxLayout *l_info;
    QVBoxLayout *l;

    QVector<QPushButton*> v_b_covers;
    QVector<QPushButton*> v_b_info_scenes;//

    void setupUi(QWidget *View)
    {
        sa_covers = new QScrollArea(View);
        sa_covers->setWidgetResizable(true);
        sa_covers->setFrameStyle(QFrame::NoFrame);
        sa_covers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sa_covers->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        w_covers = new QWidget(sa_covers);

        QPixmap px_download(":/icon-download");
        lb_download = new QLabel(View);
        lb_download->setPixmap(px_download);
        lb_download->setHidden(true);
        lb_download->setParent(View);

        l_covers = new QGridLayout(w_covers);
        l_covers->setHorizontalSpacing(0);
        l_covers->setVerticalSpacing(5);//TODO: PORCENTAJE.
        l_covers->setMargin(0);//TODO: PORCENTAJE.
        l_covers->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        w_covers->setLayout(l_covers);

        sa_covers->setWidget(w_covers);

        l = new QVBoxLayout(View);
        l->setContentsMargins(80, 10, 80, 11);//TODO: PORCENTAJE.
        l->addWidget(sa_covers);

        View->setLayout(l);
    }

    void setupInfoUi(const int &ID, const QString &path, const QStringList **data,
                     const int &sizeData, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(12);//TODO: PORCENTAJE.
        f.setBold(true);

        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_alternate_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_alternate_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        w_info = new QWidget(View);

        l_info_metadata = new QVBoxLayout;
        l_info_metadata->setSpacing(10);//TODO: PORCENTAJE.

        QStringList metadata_titles;
        metadata_titles << "NAME " << "DATE " << "CASTING " << "QUALITY " << "TIME " << "FULL "  << "CATEGORIES ";
        for (int i = 0; i < metadata_titles.size(); i++) {
            lb_info_metadata[i] = new QLabel(w_info);
            lb_info_metadata[i]->setPalette(p);
            lb_info_metadata[i]->setText("<font color = '#8a8a8a'>"+metadata_titles[i]+"</font> "+(**_data)[((ID * sizeData) + i)]);
            lb_info_metadata[i]->setAutoFillBackground(true);
            lb_info_metadata[i]->setContentsMargins(10, 5, 10, 5);
            l_info_metadata->addWidget(lb_info_metadata[i]);
        }
        l_info_metadata->addStretch();

        QPixmap px_cover(path);
        lb_info_cover = new QLabel(w_info);
        lb_info_cover->setPixmap(px_cover.scaled(335, 480, Qt::KeepAspectRatio));//TODO: PORCENTAJE.

        QPixmap px_backCover(":/img-back_cover");
        lb_info_backCover = new QLabel(w_info);
        lb_info_backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio));//TODO: PORCENTAJE.

        cb_info_scenes = new QComboBox(w_info);
        cb_info_scenes->setFont(f);
        cb_info_scenes->setStyleSheet("QComboBox { color: rgb(255, 255, 255); \
                                       background-color: rgb(15, 15, 15); \
                                       selection-background-color: rgb(122, 122, 122); }"
                                      "QComboBox QAbstractItemView { color: rgb(255, 255, 255); \
                                       background-color: rgb(15, 15, 15); \
                                       border-radius: 0px; }");
        cb_info_scenes->addItem("                    WATCH");

        QString scenes = (**_data)[((ID * sizeData) + 19)];
        for (int i = 0; i < scenes.toInt(); i++) 
            cb_info_scenes->addItem(QIcon(":/icon-watch"), " SCENE "+QString::number(i + 1));

        l_info_scenes = new QVBoxLayout;
        l_info_scenes->addWidget(cb_info_scenes);
        l_info_scenes->addStretch();

        //QPixmap px_player(":/img-back_cover");
        //lb_info_player = new QLabel(w_info);
        //lb_info_player->setPixmap(px_player.scaled(700, 480));

        //QString preview = "http://" + ((**_data)[((ID * sizeData) + 13)]) + ((**_data)[((ID * sizeData) + 14)]);
        //player_info = new Player(&preview, w_info);
        //player_info->setFixedSize(QSize(700, 480));//TODO: PORCENTAJE.

        l_info_multimedia = new QHBoxLayout;
        l_info_multimedia->setSpacing(10);//TODO: PORCENTAJE.
        l_info_multimedia->addWidget(lb_info_cover);
        l_info_multimedia->addWidget(lb_info_backCover);
        l_info_multimedia->addLayout(l_info_scenes);
        //l_info_multimedia->addWidget(player_info);
        //l_info_multimedia->addWidget(lb_info_player);//

        l_info = new QVBoxLayout(w_info);
        l_info->setContentsMargins(0, 3, 0, 0);//TODO: PORCENTAJE.
        l_info->addLayout(l_info_metadata, 1);
        l_info->addLayout(l_info_multimedia);

        w_info->setLayout(l_info);

        sa_covers->setDisabled(true);
        sa_covers->hide();

        l->addWidget(w_info);
    }
};

class Ui_Player
{
public:
    QPalette p_player;
    QLabel *lb_peers;
    QLabel *lb_bitrate;
    QVBoxLayout *l;

    void setupUi(QWidget *Player)
    {
        QFont f(":/font-global");
        f.setPointSize(16);//TODO: PORCENTAJE.
        f.setBold(true);

        QBrush br_alternate_base(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));

        p_player.setBrush(QPalette::Active, QPalette::Base, br_alternate_base);
        p_player.setBrush(QPalette::Active, QPalette::Window, br_alternate_base);
        p_player.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p_player.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_player.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_player.setBrush(QPalette::Active, QPalette::Button, br_alternate_base);
        p_player.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_player.setBrush(QPalette::Disabled, QPalette::Base, br_alternate_base);
        p_player.setBrush(QPalette::Disabled, QPalette::Window, br_alternate_base);
        p_player.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p_player.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_player.setBrush(QPalette::Disabled, QPalette::Highlight, br_alternate_base);
        p_player.setBrush(QPalette::Disabled, QPalette::Button, br_alternate_base);
        p_player.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        lb_bitrate = new QLabel(Player);
        lb_bitrate->setFont(f);
        lb_bitrate->setPalette(p_player);
        lb_bitrate->setAutoFillBackground(true);
        
        lb_peers = new QLabel(Player);
        lb_peers->setFont(f);
        lb_peers->setPalette(p_player);
        lb_peers->setAutoFillBackground(true);
        
        l = new QVBoxLayout(Player);
        l->addWidget(lb_bitrate);
        l->addWidget(lb_peers);

        Player->setLayout(l);
    }
};

class Ui_Error
{
public:
    QLabel *lb;
    QPushButton *b;
    QHBoxLayout *l;

    void setupUi(const QString **errorMsg, QWidget *Error)
    {
        lb = new QLabel(**errorMsg, Error);

        b = new QPushButton(QIcon(":/icon-cancel"), NULL, Error);
        b->setToolTip("EXIT");
        b->setFlat(true);

        l = new QHBoxLayout(Error);
        l->setAlignment(Qt::AlignHCenter);
        l->addWidget(lb);
        l->addSpacing(10);//TODO: PORCENTAJE
        l->addWidget(b);

        Error->setLayout(l);
    }
};

namespace Ui {
    class PIG: public Ui_PIG {};
    class Authorization: public Ui_Authotization {};
    class Update: public Ui_Update {};
    class TopBar: public Ui_Topbar {};
    class Finder: public Ui_Finder {};
    class Setup: public Ui_Setup {};
    class View: public Ui_View {};
    class Player: public Ui_Player {};
    class Error: public Ui_Error {};
}

QT_END_NAMESPACE

#endif

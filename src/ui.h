#ifndef UI_H
#define UI_H

#include "movie.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QComboBox>
#include <QPixmap>
#include <QScrollArea>
#include <QScrollBar>
#include <QProgressBar>
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
    QVBoxLayout *main_layout;

    void setupUi(QWidget *PIG)
    {
        QFont f(":/font-global");
        f.setPointSize(16); //TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QFont f_toolTip(":/font-global");
        f_toolTip.setPointSize(12); //TODO: PORCENTAJE.
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
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipBase, br_base);
        p_toolTip.setBrush(QPalette::All, QPalette::ToolTipText, br_white);

        QToolTip::setFont(f_toolTip);
        QToolTip::setPalette(p_toolTip);

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
        QBrush br_base_light(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_gray(QColor(122, 122, 122, 255));
        QBrush br_red(QColor(115, 10, 10, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        p.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_dark);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_dark);
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
        b_reset->setToolTip("RESET");
        b_reset->setFlat(true);
        b_reset->setDisabled(!hasSet);
        b_reset->setHidden(!set);

        l_input = new QHBoxLayout;
        l_input->setSpacing(10); //TODO: PORCENTAJE.
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

        b_2 = new QPushButton(QIcon(":/icon-error"), NULL, Update);
        b_2->setToolTip("CANCEL UPDATE");
        b_2->setFlat(true);

        l = new QHBoxLayout(Update);
        l->setAlignment(Qt::AlignHCenter);
        l->addWidget(lb);
        l->addSpacing(10); //TODO: PORCENTAJE.
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
        l->setContentsMargins(0, 10, 46, 0); //TODO: PORCENTAJE.
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
    QPushButton *b_apply_on_results;
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
        f.setPointSize(12); //TODO: PORCENTAJE.
        f.setCapitalization(QFont::AllUppercase);
        f.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_base_light(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_green(QColor(0, 112, 0, 255));
        QBrush br_red(QColor(115, 10, 10, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_red);
        p.setBrush(QPalette::Active, QPalette::Button, br_base);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        QPalette p_completer;
        p_completer.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p_completer.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p_completer.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_completer.setBrush(QPalette::Active, QPalette::Highlight, br_green);
        p_completer.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p_completer.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p_completer.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_completer.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);

        p_fullMovie.setBrush(QPalette::Active, QPalette::Button, br_base_light);
        p_fullMovie.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_fullMovie.setBrush(QPalette::Disabled, QPalette::Button, br_base_light);
        p_fullMovie.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        p_fullMovie_active.setBrush(QPalette::Active, QPalette::Button, br_green);
        p_fullMovie_active.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_fullMovie_active.setBrush(QPalette::Disabled, QPalette::Button, br_base_light);
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
        input->setMinimumHeight(45); //TODO: PORCENTAJE.
        input->setFrame(QFrame::NoFrame);
        input->setCompleter(completer);
        input->setAlignment(Qt::AlignCenter);

        b_filters = new QPushButton(QIcon(":/icon-filters"), NULL, Finder);
        b_filters->setGeometry(14, 11, 24, 24); //TODO: PORCENTAJE.
        b_filters->setCursor(Qt::ArrowCursor);
        b_filters->setFlat(true);
        b_filters->setParent(input);

        l = new QGridLayout;
        l->setSpacing(0);
        l->setContentsMargins(83, 0, 12, 0); //TODO: PORCENTAJE.
        l->addWidget(input, 0, 0);

        _l_topbar->addLayout(l, 0, 0);
    }

    void setupFilterUi(const QStringList *categories, const QStringList *pornstars, QWidget *Finder)
    {
        QFont f(":/font-global");
        f.setPointSize(12); //TODO: PORCENTAJE.
        f.setBold(true);

        cb_style = "QComboBox { color: rgb(255, 255, 255); \
                    background-color: rgb(15, 15, 15); \
                    selection-background-color: rgb(0, 112, 0); } \
                    QComboBox QAbstractItemView { color: rgb(255, 255, 255); \
                    background-color: rgb(15, 15, 15); \
                    border-radius: 0px; }";

        cb_style_active = "QComboBox { color: rgb(255, 255, 255); \
                           background-color: rgb(0, 112, 0); \
                           selection-background-color: rgb(0, 112, 0); } \
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

        b_apply_on_results = new QPushButton(QIcon(":/icon-ok"), " APPLY ON RESULTS", Finder);
        b_apply_on_results->setFont(f);
        b_apply_on_results->setFlat(true);

        l_filters = new QHBoxLayout;
        l_filters->setSpacing(10); //TODO: PORCENTAJE.
        l_filters->setContentsMargins(84, 10, 13, 0); //TODO: PORCENTAJE.
        l_filters->addWidget(cb_categories);
        l_filters->addWidget(cb_pornstars);
        l_filters->addWidget(cb_quality);
        l_filters->addWidget(b_fullMovie);
        l_filters->addWidget(b_apply_on_results, 0, Qt::AlignHCenter);

        _l_topbar->addLayout(l_filters, 1, 0);
    }
};

class Ui_Setup
{
public:
    QPalette p_torrent_ports;
    QPalette p_torrent_ports_edit;
    QPalette p_wallet;
    QPalette p_wallet_copied;
    QLabel *lb_data_binary;
    QLabel *lb_data_database;
    QLabel *lb_data_license;
    QLabel *lb_torrent_ports;
    QLabel *lb_contribute_torrents;
    QLabel *lb_contribute_code;
    QLabel *lb_contribute_bugs;
    QLabel *lb_contribute_support;
    QLabel *lb_contribute_wallet;
    QLineEdit *input_torrent_port_1;
    QLineEdit *input_torrent_port_2;
    QPushButton *b_folder_covers;
    QPushButton *b_folder_torrents;
    QPushButton *b_folder_movies;
    QPushButton *b_folder_covers_reset;
    QPushButton *b_folder_torrents_reset;
    QPushButton *b_folder_movies_reset;
    QPushButton *b_torrent_ports_reset;
    QPushButton *b_contribute_torrents;
    QPushButton *b_contribute_code;
    QPushButton *b_contribute_bugs;
    QPushButton *b_contribute_support;
    QPushButton *b_contribute_wallet_copy;
    QWidget *w_data;
    QWidget *w_authorization;
    QWidget *w_folder;
    QWidget *w_torrent_ports;
    QWidget *w_contribute;
    QHBoxLayout *l_data;
    QHBoxLayout *l_authorization;
    QHBoxLayout *l_folder_covers;
    QHBoxLayout *l_folder_torrents;
    QHBoxLayout *l_folder_movies;
    QVBoxLayout *l_folder;
    QHBoxLayout *l_torrent_ports;
    QHBoxLayout *l_contribute_torrents;
    QHBoxLayout *l_contribute_code;
    QHBoxLayout *l_contribute_bugs;
    QHBoxLayout *l_contribute_support;
    QVBoxLayout *l_contribute;
    QVBoxLayout *l;

    QClipboard *clipboard;

    void setupUi(QStringList *data, bool **keep_covers, bool **keep_torrents,
                 bool **keep_movies, int **torrent_port_1, int **torrent_port_2,
                 QWidget *authorization, QWidget *Setup)
    {
        QFont f_wallet(":/font-global");
        f_wallet.setPointSize(16); //TODO: PORCENTAJE.
        f_wallet.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_base_light(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_gray(QColor(122, 122, 122, 255));
        QBrush br_green(QColor(0, 112, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);

        QPalette p_data;
        p_data.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p_data.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p_data.setBrush(QPalette::Active, QPalette::WindowText, br_gray);
        p_data.setBrush(QPalette::Active, QPalette::Text, br_gray);
        p_data.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_data.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p_data.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p_data.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p_data.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_data.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);

        p_torrent_ports.setBrush(QPalette::Active, QPalette::Base, br_dark);
        p_torrent_ports.setBrush(QPalette::Active, QPalette::Window, br_dark);
        p_torrent_ports.setBrush(QPalette::Active, QPalette::Text, br_base);
        p_torrent_ports.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_torrent_ports.setBrush(QPalette::Disabled, QPalette::Base, br_gray);
        p_torrent_ports.setBrush(QPalette::Disabled, QPalette::Window, br_gray);
        p_torrent_ports.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_torrent_ports.setBrush(QPalette::Disabled, QPalette::Highlight, br_gray);

        p_torrent_ports_edit.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p_torrent_ports_edit.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p_torrent_ports_edit.setBrush(QPalette::Active, QPalette::Text, br_white);
        p_torrent_ports_edit.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_torrent_ports_edit.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p_torrent_ports_edit.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p_torrent_ports_edit.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_torrent_ports_edit.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);

        p_wallet.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p_wallet.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p_wallet.setBrush(QPalette::Active, QPalette::WindowText, br_gray);
        p_wallet.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p_wallet.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p_wallet.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);

        p_wallet_copied.setBrush(QPalette::Active, QPalette::Base, br_green);
        p_wallet_copied.setBrush(QPalette::Active, QPalette::Window, br_green);
        p_wallet_copied.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::Base, br_dark);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::Window, br_dark);
        p_wallet_copied.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);

        w_data = new QWidget(Setup);
        w_data->setPalette(p_data);
        w_data->setAutoFillBackground(true);

        lb_data_binary = new QLabel("PIG "+(*data)[0]+"."+(*data)[1], w_data);
        lb_data_database = new QLabel("DATABASE "+(*data)[2], w_data);
        lb_data_license = new QLabel("GPL 2.0", w_data);

        l_data = new QHBoxLayout(w_data);
        l_data->setSpacing(20); //TODO: PORCENTAJE.
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

        b_folder_covers = new QPushButton(" KEEP COVERS", w_folder);
        if (**keep_covers)
            b_folder_covers->setIcon(QIcon(":/icon-ok"));
        else
            b_folder_covers->setIcon(QIcon(":/icon-ok-dark"));
        b_folder_covers->setFlat(true);

        b_folder_torrents = new QPushButton(" KEEP TORRENTS", w_folder);
        if (**keep_torrents)
            b_folder_torrents->setIcon(QIcon(":/icon-ok"));
        else
            b_folder_torrents->setIcon(QIcon(":/icon-ok-dark"));
        b_folder_torrents->setFlat(true);

        b_folder_movies = new QPushButton(" KEEP MOVIES", w_folder);
        if (**keep_movies)
            b_folder_movies->setIcon(QIcon(":/icon-ok"));
        else
            b_folder_movies->setIcon(QIcon(":/icon-ok-dark"));
        b_folder_movies->setFlat(true);

        b_folder_covers_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_covers_reset->setToolTip("CLEAN");
        b_folder_covers_reset->setFlat(true);

        b_folder_torrents_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_torrents_reset->setToolTip("CLEAN");
        b_folder_torrents_reset->setFlat(true);

        b_folder_movies_reset = new QPushButton(QIcon(":/icon-reset"), NULL, w_folder);
        b_folder_movies_reset->setToolTip("CLEAN");
        b_folder_movies_reset->setFlat(true);

        l_folder_covers = new QHBoxLayout;
        l_folder_covers->addWidget(b_folder_covers);
        l_folder_covers->insertStretch(1);
        l_folder_covers->addWidget(b_folder_covers_reset);

        l_folder_torrents = new QHBoxLayout;
        l_folder_torrents->addWidget(b_folder_torrents);
        l_folder_torrents->insertStretch(1);
        l_folder_torrents->addWidget(b_folder_torrents_reset);

        l_folder_movies = new QHBoxLayout;
        l_folder_movies->addWidget(b_folder_movies);
        l_folder_movies->insertStretch(1);
        l_folder_movies->addWidget(b_folder_movies_reset);

        l_folder = new QVBoxLayout(w_folder);
        l_folder->addLayout(l_folder_covers);
        l_folder->addLayout(l_folder_torrents);
        l_folder->addLayout(l_folder_movies);

        w_folder->setLayout(l_folder);

        w_torrent_ports = new QWidget(Setup);
        w_torrent_ports->setPalette(p);
        w_torrent_ports->setAutoFillBackground(true);

        lb_torrent_ports = new QLabel("PORTS", w_torrent_ports);

        QValidator *validPort = new QIntValidator(0, 65000, Setup);

        input_torrent_port_1 = new QLineEdit(QString::number(**torrent_port_1), w_torrent_ports);
        input_torrent_port_1->setPalette(p_torrent_ports);
        input_torrent_port_1->setValidator(validPort);
        input_torrent_port_1->setAlignment(Qt::AlignCenter);

        input_torrent_port_2 = new QLineEdit(QString::number(**torrent_port_2), w_torrent_ports);
        input_torrent_port_2->setPalette(p_torrent_ports);
        input_torrent_port_2->setValidator(validPort);
        input_torrent_port_2->setAlignment(Qt::AlignCenter);

        b_torrent_ports_reset = new QPushButton(w_torrent_ports);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset-dark"));
        else
            b_torrent_ports_reset->setIcon(QIcon(":/icon-reset"));
        b_torrent_ports_reset->setToolTip("RESET");
        b_torrent_ports_reset->setFlat(true);
        if ((**torrent_port_1 == 6900) && (**torrent_port_2 == 6999))
            b_torrent_ports_reset->setDisabled(true);

        l_torrent_ports = new QHBoxLayout(w_torrent_ports);
        l_torrent_ports->setSpacing(10); //TODO: PORCENTAJE.
        l_torrent_ports->addWidget(lb_torrent_ports);
        l_torrent_ports->addWidget(input_torrent_port_1);
        l_torrent_ports->addWidget(input_torrent_port_2);
        l_torrent_ports->addWidget(b_torrent_ports_reset);

        w_torrent_ports->setLayout(l_torrent_ports);

        w_contribute = new QWidget(Setup);
        w_contribute->setPalette(p);
        w_contribute->setAutoFillBackground(true);

        lb_contribute_torrents = new QLabel("TORRENTS", w_contribute);

        b_contribute_torrents = new QPushButton(QIcon(":/icon-upload"), NULL, w_contribute);
        b_contribute_torrents->setToolTip("UPLOAD");
        b_contribute_torrents->setFlat(true);

        l_contribute_torrents = new QHBoxLayout;
        l_contribute_torrents->addWidget(lb_contribute_torrents);
        l_contribute_torrents->insertStretch(1);
        l_contribute_torrents->addWidget(b_contribute_torrents);

        lb_contribute_code = new QLabel("CODE", w_contribute);

        b_contribute_code = new QPushButton(QIcon(":/icon-upload"), NULL, w_contribute);
        b_contribute_code->setToolTip("UPLOAD");
        b_contribute_code->setFlat(true);

        l_contribute_code = new QHBoxLayout;
        l_contribute_code->addWidget(lb_contribute_code);
        l_contribute_code->insertStretch(1);
        l_contribute_code->addWidget(b_contribute_code);

        lb_contribute_bugs = new QLabel("BUGS", w_contribute);

        b_contribute_bugs = new QPushButton(QIcon(":/icon-upload"), NULL, w_contribute);
        b_contribute_bugs->setToolTip("REPORT");
        b_contribute_bugs->setFlat(true);;

        l_contribute_bugs = new QHBoxLayout;
        l_contribute_bugs->addWidget(lb_contribute_bugs);
        l_contribute_bugs->insertStretch(1);
        l_contribute_bugs->addWidget(b_contribute_bugs);

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

        b_contribute_wallet_copy= new QPushButton(QIcon(":/icon-copy"), NULL, w_contribute);
        b_contribute_wallet_copy->setToolTip("COPY");
        b_contribute_wallet_copy->setFlat(true);
        b_contribute_wallet_copy->setHidden(true);

        l_contribute_support = new QHBoxLayout;
        l_contribute_support->addWidget(lb_contribute_support);
        l_contribute_support->addSpacing(5); //TODO: PORCENTAJE.
        l_contribute_support->addWidget(lb_contribute_wallet);
        l_contribute_support->addWidget(b_contribute_wallet_copy);
        l_contribute_support->insertStretch(5); //TODO: PORCENTAJE.
        l_contribute_support->addWidget(b_contribute_support);

        l_contribute = new QVBoxLayout(w_contribute);
        l_contribute->addLayout(l_contribute_torrents);
        l_contribute->addLayout(l_contribute_code);
        l_contribute->addLayout(l_contribute_bugs);
        l_contribute->addLayout(l_contribute_support);

        w_contribute->setLayout(l_contribute);

        l = new QVBoxLayout(Setup);
        l->setSpacing(10); //TODO: PORCENTAJE.
        l->setContentsMargins(80, 13, 80, 10); //TODO: PORCENTAJE.
        l->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        l->addWidget(w_data);
        l->addWidget(w_authorization);
        l->addWidget(w_folder);
        l->addWidget(w_torrent_ports);
        l->addWidget(w_contribute);
        l->addStretch();

        Setup->setLayout(l);
    }
};

class Ui_View
{
public:
    QScrollArea *sa_covers;
    QLabel *lb_px_download_status;
    QLabel *lb_info_metadata[5];
    QLabel *lb_info_px_cover;
    QLabel *lb_info_px_backCover;
    QLabel *lb_info_player;//
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

        QPixmap px_download_status(":/icon-download_status");
        lb_px_download_status = new QLabel(View);
        lb_px_download_status->setPixmap(px_download_status);
        lb_px_download_status->setHidden(true);
        lb_px_download_status->setParent(View);

        l_covers = new QGridLayout(w_covers);
        l_covers->setHorizontalSpacing(0);
        l_covers->setVerticalSpacing(5); //TODO: PORCENTAJE.
        l_covers->setMargin(0); //TODO: PORCENTAJE.
        l_covers->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        w_covers->setLayout(l_covers);

        sa_covers->setWidget(w_covers);

        l = new QVBoxLayout(View);
        l->setContentsMargins(80, 10, 80, 11); //TODO: PORCENTAJE.
        l->addWidget(sa_covers);

        View->setLayout(l);
    }

    void setupInfoUi(const int &ID, const QString &path, const QStringList **data,
                     const int &sizeData, QWidget *View)
    {
        const QStringList **_data = data;

        QFont f(":/font-global");
        f.setPointSize(12); //TODO: PORCENTAJE.
        f.setBold(true);

        QBrush br_base_light(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Active, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base_light);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        w_info = new QWidget(View);

        l_info_metadata = new QVBoxLayout;
        l_info_metadata->setSpacing(10); //TODO: PORCENTAJE.

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
        lb_info_px_cover = new QLabel(w_info);
        lb_info_px_cover->setPixmap(px_cover.scaled(335, 480, Qt::KeepAspectRatio)); //TODO: PORCENTAJE.

        QPixmap px_backCover(":/img-back_cover");
        lb_info_px_backCover = new QLabel(w_info);
        lb_info_px_backCover->setPixmap(px_backCover.scaled(335, 480, Qt::KeepAspectRatio)); //TODO: PORCENTAJE.

        cb_info_scenes = new QComboBox(w_info);
        cb_info_scenes->setFont(f);
        cb_info_scenes->setStyleSheet("QComboBox { color: rgb(255, 255, 255); \
                                       background-color: rgb(15, 15, 15); \
                                       selection-background-color: rgb(0, 112, 0); }"
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

        QPixmap px_player(":/img-back_cover");
        lb_info_player = new QLabel(w_info);
        lb_info_player->setPixmap(px_player.scaled(700, 480));

        //QString preview = "http://" + ((**_data)[((ID * sizeData) + 13)]) + ((**_data)[((ID * sizeData) + 14)]);
        //player_info = new Player(&preview, w_info);
        //player_info->setFixedSize(QSize(700, 480)); //TODO: PORCENTAJE.

        l_info_multimedia = new QHBoxLayout;
        l_info_multimedia->setSpacing(10); //TODO: PORCENTAJE.
        l_info_multimedia->addWidget(lb_info_px_cover);
        l_info_multimedia->addWidget(lb_info_px_backCover);
        l_info_multimedia->addLayout(l_info_scenes);
        //l_info_multimedia->addWidget(player_info);
        l_info_multimedia->addWidget(lb_info_player);//

        l_info = new QVBoxLayout(w_info);
        l_info->setContentsMargins(0, 3, 0, 0); //TODO: PORCENTAJE.
        l_info->addLayout(l_info_metadata, 1);
        l_info->addLayout(l_info_multimedia);

        w_info->setLayout(l_info);

        sa_covers->setDisabled(true);
        sa_covers->hide();

        l->addWidget(w_info);
    }
};

class Ui_Movie
{
public:
    QPalette p_stats_controls;
    QLabel *lb_bitrate;
    QLabel *lb_peers;
    QProgressBar *pb_minimum_download;
    QPushButton *b_play;
    QPushButton *b_mute;
    QSlider *sl_volume;
    QSlider *sl_position;
    QLabel *lb_current_time;
    QLabel *lb_total_time;
    QPushButton *b_more;
    QWidget *w_player;
    QWidget *w_controls;
    QHBoxLayout *l_horizontal_stats;
    QVBoxLayout *l_vertical_stats;
    QHBoxLayout *l_controls;
    QVBoxLayout *l;

    void setupUi(QWidget *Movie)
    {
        QFont f(":/font-global");
        f.setPointSize(16); //TODO: PORCENTAJE.
        f.setCapitalization(QFont::MixedCase);
        f.setBold(true);

        QBrush br_base(QColor(10, 10, 10, 255));
        QBrush br_base_light(QColor(15, 15, 15, 255));
        QBrush br_white(QColor(255, 255, 255, 255));
        QBrush br_black(QColor(0, 0, 0, 255));
        QBrush br_dark(QColor(40, 40, 40, 255));

        QPalette p;
        p.setBrush(QPalette::Active, QPalette::Base, br_base);
        p.setBrush(QPalette::Active, QPalette::Window, br_base);
        p.setBrush(QPalette::Active, QPalette::WindowText, br_white);
        p.setBrush(QPalette::Active, QPalette::Text, br_white);
        p.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p.setBrush(QPalette::Active, QPalette::Button, br_base_light);
        p.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::Button, br_base_light);
        p.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        p_stats_controls.setBrush(QPalette::Active, QPalette::Base, br_base);
        p_stats_controls.setBrush(QPalette::Active, QPalette::Window, br_base);
        p_stats_controls.setBrush(QPalette::Active, QPalette::WindowText, br_dark);
        p_stats_controls.setBrush(QPalette::Active, QPalette::Text, br_dark);
        p_stats_controls.setBrush(QPalette::Active, QPalette::Highlight, br_black);
        p_stats_controls.setBrush(QPalette::Active, QPalette::Button, br_base_light);
        p_stats_controls.setBrush(QPalette::Active, QPalette::ButtonText, br_white);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::Base, br_base_light);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::Window, br_base_light);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::WindowText, br_black);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::Text, br_black);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::Highlight, br_base_light);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::Button, br_base_light);
        p_stats_controls.setBrush(QPalette::Disabled, QPalette::ButtonText, br_black);

        lb_bitrate = new QLabel(Movie);
        lb_bitrate->setFont(f);
        lb_bitrate->setPalette(p);
        lb_bitrate->setText("0 KBPS");
        lb_bitrate->setAutoFillBackground(true);

        lb_peers = new QLabel(Movie);
        lb_peers->setFont(f);
        lb_peers->setPalette(p);
        lb_peers->setText("0 PEERS");
        lb_peers->setAutoFillBackground(true);

        l_horizontal_stats = new QHBoxLayout;
        l_horizontal_stats->setSpacing(15); //TODO: PORCENTAJE.
        l_horizontal_stats->setAlignment(Qt::AlignCenter);
        l_horizontal_stats->addWidget(lb_bitrate);
        l_horizontal_stats->addWidget(lb_peers);

        pb_minimum_download = new QProgressBar();
        pb_minimum_download->setFixedHeight(3); //TODO: PORCENTAJE.
        pb_minimum_download->setFixedWidth(500); //TODO: PORCENTAJE.
        pb_minimum_download->setStyleSheet ("QProgressBar { background: rgb(15, 15, 15); \
                                             border: none; }"
                                            "QProgressBar::chunk { background: rgb(255, 255, 255); }");
        pb_minimum_download->setTextVisible(false);
        pb_minimum_download->setMinimum(0);

        l_vertical_stats = new QVBoxLayout;
        l_vertical_stats->setSpacing(15); //TODO: PORCENTAJE.
        l_vertical_stats->setAlignment(Qt::AlignCenter);
        l_vertical_stats->addLayout(l_horizontal_stats);
        l_vertical_stats->addWidget(pb_minimum_download);
        
        w_player = new QWidget;
        w_player->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        w_player->setMouseTracking(true);
        w_player->hide();

        w_controls = new QWidget;
        w_controls->hide();

        b_play = new QPushButton(QIcon(":/icon-pause"), NULL, Movie);
        b_play->setIconSize(QSize(24, 24));
        b_play->setFlat(true);

        b_mute = new QPushButton(QIcon(":/icon-mute"), NULL, Movie);
        b_mute->setIconSize(QSize(24, 24));
        b_mute->setFlat(true);

        sl_volume = new QSlider(Qt::Horizontal);
        sl_volume->setMaximumWidth(100); //TODO: PORCENTAJE.
        sl_volume->setStyleSheet ("QSlider::groove:horizontal { background: rgb(15, 15, 15); \
                                   height: 3px; \
                                   border: none; }"
                                  "QSlider::handle:horizontal { background: white; \
                                   width: 3px; \
                                   border: 5px solid white; }"
                                  "QSlider::sub-page:horizontal { background: white; \
                                   height: 3px; \
                                   border: none; }");
        sl_volume->setValue(80);

        sl_position = new QSlider(Qt::Horizontal);
        sl_position->setStyleSheet ("QSlider::groove:horizontal { background: rgb(15, 15, 15); \
                                     height: 3px; \
                                     border: none; }"
                                    "QSlider::handle:horizontal { background: white; \
                                     width: 3px; \
                                     border: 5px solid white; }"
                                    "QSlider::sub-page:horizontal { background: white; \
                                     height: 3px; \
                                     border: none; }");
        sl_position->setMaximum(1000);//
        
        lb_current_time = new QLabel(Movie);
        lb_current_time->setFont(f);
        lb_current_time->setPalette(p);
        lb_current_time->setText("00:00:00");
        lb_current_time->setAutoFillBackground(true);
        
        lb_total_time = new QLabel(Movie);
        lb_total_time->setFont(f);
        lb_total_time->setPalette(p);
        lb_total_time->setText("00:00:00");
        lb_total_time->setAutoFillBackground(true);

        b_more = new QPushButton(QIcon(":/icon-more"), NULL, Movie);
        b_more->setFlat(true);

        l_controls = new QHBoxLayout;
        l_controls->setSpacing(15); //TODO: PORCENTAJE.
        l_controls->setAlignment(Qt::AlignCenter);
        l_controls->addWidget(b_play);
        l_controls->addWidget(b_mute);
        l_controls->addWidget(sl_volume);
        l_controls->addWidget(sl_position);
        l_controls->addWidget(lb_current_time);
        l_controls->addWidget(lb_total_time);
        l_controls->addWidget(b_more);
        
        w_controls->setLayout(l_controls);

        l = new QVBoxLayout(Movie);
        l->setMargin(0);
        l->setContentsMargins(0, 0, 0, 0);
        l->setSpacing(0);
        l->addLayout(l_vertical_stats);

        Movie->setLayout(l);
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

        b = new QPushButton(QIcon(":/icon-error"), NULL, Error);
        b->setToolTip("EXIT");
        b->setFlat(true);

        l = new QHBoxLayout(Error);
        l->setAlignment(Qt::AlignHCenter);
        l->addWidget(lb);
        l->addSpacing(10); //TODO: PORCENTAJE.
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
    class Movie: public Ui_Movie {};
    class Error: public Ui_Error {};
}

QT_END_NAMESPACE

#endif
